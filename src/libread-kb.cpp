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


std::ostream& operator<<(std::ostream& os, ReadKB::Key kb) {
  if (kb >= ReadKB::Key::SPACE && kb < ReadKB::Key::DEL) {
    // Printable
    os << static_cast<char>(kb);
  } else {
    // Unprintable
    switch (kb) {
      case ReadKB::Key::NUL : os << "NUL";        break;
      case ReadKB::Key::SOH : os << "SOH";        break;
      case ReadKB::Key::STX : os << "STX";        break;
      case ReadKB::Key::ETX : os << "ETX";        break;
      case ReadKB::Key::EOT : os << "EOT";        break;
      case ReadKB::Key::ENQ : os << "ENQ";        break;
      case ReadKB::Key::ACK : os << "ACK";        break;
      case ReadKB::Key::BEL : os << "BEL";        break;
      case ReadKB::Key::BS  : os << "Ctrl-Bksp";  break;
      case ReadKB::Key::HT  : os << "Tab";        break;
      case ReadKB::Key::LF  : os << "Enter";      break;
      case ReadKB::Key::VT  : os << "VT";         break;
      case ReadKB::Key::FF  : os << "FF";         break;
      case ReadKB::Key::CR  : os << "CR";         break;
      case ReadKB::Key::SO  : os << "SO";         break;
      case ReadKB::Key::SI  : os << "SI";         break;
      case ReadKB::Key::DLE : os << "DLE";        break;
      case ReadKB::Key::DC1 : os << "DC1";        break;
      case ReadKB::Key::DC2 : os << "DC2";        break;
      case ReadKB::Key::DC3 : os << "DC3";        break;
      case ReadKB::Key::DC4 : os << "DC4";        break;
      case ReadKB::Key::NAK : os << "NAK";        break;
      case ReadKB::Key::SYN : os << "SYN";        break;
      case ReadKB::Key::ETB : os << "ETB";        break;
      case ReadKB::Key::CAN : os << "CAN";        break;
      case ReadKB::Key::EM  : os << "EM";         break;
      case ReadKB::Key::SUB : os << "SUB";        break;
      case ReadKB::Key::ESC : os << "Esc";        break;
      case ReadKB::Key::FS  : os << "FS";         break;
      case ReadKB::Key::GS  : os << "GS";         break;
      case ReadKB::Key::RS  : os << "RS";         break;
      case ReadKB::Key::US  : os << "US";         break;
      case ReadKB::Key::DEL : os << "Bksp";       break;
      // Beyond ASCII
      case ReadKB::Key::SPACE :
      case ReadKB::Key::UNDEFINED : os << "Undefined: " << static_cast<char>(kb); break;
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
    char buf[8]; // Unicode less than 5 bytes and longest ANSI sequence (that I know of) is e[nn;n~

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
        printlog("    read %zd bytes: \033[1m%.*s\033[0m\n", s, (int) s, buf);

        key_pressed = static_cast<Key>(buf[0]);

      } else {
        // Process other signals (POLLERR | POLLHUP | POLLNVAL)
        key_pressed = Key::UNDEFINED;
      }
    }
  }
  return key_pressed;
}
