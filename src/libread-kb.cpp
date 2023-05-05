/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

#include "libread-kb.h"

#include <termios.h>
#include <poll.h>
#include <unistd.h>

#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#if DEBUG_LIB_READ_KB == 0
#   define printlog(...)   do {} while (0)
#else
#   define printlog(...)   do { fprintf(g_pDebugLogFile, __VA_ARGS__); \
                                fseek(g_pDebugLogFile, 0, SEEK_END); \
                              } while (0)
#endif

// Define error-handling function
#define errorIf(cond, msg) do { if( cond ) { \
                                  perror(msg); exit(EXIT_FAILURE); \
                              }} while (0)

#define STDIN_FD 0 // Standard input file descriptor
#define BUFF_SIZE_CHARS 8 // Unicode less than 5 bytes and longest ANSI sequence (that I know of) is of the form e[nn;n~


std::ostream& operator<<(std::ostream& os, ReadKB::Key kb) {
  if (kb >= ReadKB::Key::SPACE && kb < ReadKB::Key::DEL) {
    // Printable
    os << static_cast<char>(kb);
  } else {
    // Unprintable
    switch (kb) {
      case ReadKB::Key::NUL : os << "NUL";        break;
      case ReadKB::Key::SOH : os << "Ctrl-A";     break;
      case ReadKB::Key::STX : os << "Ctrl-B";     break;
      case ReadKB::Key::ETX : os << "ETX";        break;
      case ReadKB::Key::EOT : os << "Ctrl-D";     break;
      case ReadKB::Key::ENQ : os << "Ctrl-E";     break;
      case ReadKB::Key::ACK : os << "Ctrl-F";     break;
      case ReadKB::Key::BEL : os << "Ctrl-G";     break;
      case ReadKB::Key::BS  : os << "Ctrl-Bksp";  break;
      case ReadKB::Key::HT  : os << "Tab";        break;
      case ReadKB::Key::LF  : os << "Enter";      break;
      case ReadKB::Key::VT  : os << "Ctrl-K";     break;
      case ReadKB::Key::FF  : os << "Ctrl-L";     break;
      case ReadKB::Key::CR  : os << "CR";         break;
      case ReadKB::Key::SO  : os << "Ctrl-N";     break;
      case ReadKB::Key::SI  : os << "Ctrl-O";     break;
      case ReadKB::Key::DLE : os << "Ctrl-P";     break;
      case ReadKB::Key::DC1 : os << "DC1";        break;
      case ReadKB::Key::DC2 : os << "Ctrl-R";     break;
      case ReadKB::Key::DC3 : os << "DC3";        break;
      case ReadKB::Key::DC4 : os << "Ctrl-T";     break;
      case ReadKB::Key::NAK : os << "Ctrl-U";     break;
      case ReadKB::Key::SYN : os << "Ctrl-V";     break;
      case ReadKB::Key::ETB : os << "Ctrl-W";     break;
      case ReadKB::Key::CAN : os << "Ctrl-X";     break;
      case ReadKB::Key::EM  : os << "Ctrl-Y";     break;
      case ReadKB::Key::SUB : os << "SUB";        break;
      case ReadKB::Key::ESC : os << "Esc";        break;
      case ReadKB::Key::FS  : os << "FS";         break;
      case ReadKB::Key::GS  : os << "GS";         break;
      case ReadKB::Key::RS  : os << "RS";         break;
      case ReadKB::Key::US  : os << "US";         break;
      case ReadKB::Key::DEL : os << "Bksp";       break;
      // Beyond ASCII
      case ReadKB::Key::SPACE :
      case ReadKB::Key::UNDEFINED : os << "Undefined"; break;
    }
  }
  return os;
}

ReadKB::ReadKB() {
  // Initialize debugging log file
  #if DEBUG_LIB_READ_KB == 1
    g_pDebugLogFile = fopen ("/tmp/read-kb-debug-log.txt", "w");
  #endif

  // Turn off OS buffering on standard input (non-canonical mode)
  struct termios term;
  tcgetattr(STDIN_FD, &term);
  term.c_lflag &= ~ICANON; // Non-canonical mode
  term.c_lflag &= ~ECHO; // Do not print input
  tcsetattr(STDIN_FD, TCSANOW, &term);

  // Get file descriptors for the poll command
  pfds = static_cast<pollfd*>(calloc(1, sizeof(struct pollfd)));
  errorIf(pfds == NULL, "malloc");

  // Open each file, and add it to 'pfds' array
  pfds[0].fd = STDIN_FD;
  errorIf(pfds[0].fd == -1, "open");
  printlog("Opened \"%s\" on fd %d\n", "stdin", pfds[0].fd);

  // Request poll() to scan file descriptor for data available to read (POLLIN signal)
  pfds[0].events = POLLIN;
}

ReadKB::Key ReadKB::read_key() {
  int num_ready;
  Key key_pressed;

  printlog("Polling pipe for signal or data... ");
  num_ready = poll(pfds, 1, -1);
  errorIf(num_ready == -1, "poll");
  printlog("Pipes ready: %d\n", num_ready);

  // Deal with array returned by poll()
  {
    // Read from pipe one buffer-length at a time
    u_char buf[BUFF_SIZE_CHARS];

    if (pfds[0].revents != 0) {

      // Log signals (events) found by poll
      printlog("  fd=%d; events: %s%s%s%s\n", pfds[0].fd,
          (pfds[0].revents & POLLIN)   ? "\033[32mPOLLIN\033[0m "   : "",
          (pfds[0].revents & POLLHUP)  ? "\033[33mPOLLHUP\033[0m "  : "",
          (pfds[0].revents & POLLERR)  ? "\033[31mPOLLERR\033[0m "  : "",
          (pfds[0].revents & POLLNVAL) ? "\033[31mPOLLNVAL\033[0m " : "");

      if (pfds[0].revents & POLLIN) {
        // Read from the pipe if there is data available (POLLIN)

        ssize_t s = read(pfds[0].fd, buf, sizeof(buf));
        errorIf(s == -1, "read");
        errorIf(s + 1 > BUFF_SIZE_CHARS, "read buffer overflow");
        printlog("    read %zd bytes: \033[1m", s);
        for (int ii = 0; ii < s; ii++) {
          printlog("%d  ", (uint)buf[ii]);
        }
        printlog("\033[0m\n");

        if (s == 1 && buf[0] <= 127) {
          key_pressed = static_cast<Key>(buf[0]);
        } else {
          key_pressed = Key::UNDEFINED;
        }

      } else {
        // Process other signals (POLLERR | POLLHUP | POLLNVAL)
        key_pressed = Key::UNDEFINED;
      }
    }
  }
  return key_pressed;
}
