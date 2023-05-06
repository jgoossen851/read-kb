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

#include <cassert>
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


std::ostream& operator<<(std::ostream& os, const ReadKB::Combo& kb) {
  if ((kb.mkey >= static_cast<uint>('a') && kb.mkey <= static_cast<uint>('z')) ||
      (kb.mkey >= static_cast<uint>('A') && kb.mkey <= static_cast<uint>('Z')) ||
      (kb.mkey >= static_cast<uint>('0') && kb.mkey <= static_cast<uint>('9'))) {
    // Alphanumeric
    os << static_cast<char>(kb.mkey);
  } else {
    switch (kb.mkey) {
      case static_cast<uint>(ReadKB::Key::DoubleQuote)  : os << "\"";     break;
      case static_cast<uint>(ReadKB::Key::LeftAngle)    : os << "<";      break;
      case static_cast<uint>(ReadKB::Key::Underscore)   : os << "_";      break;
      case static_cast<uint>(ReadKB::Key::RightAngle)   : os << ">";      break;
      case static_cast<uint>(ReadKB::Key::Question)     : os << "?";      break;
      case static_cast<uint>(ReadKB::Key::RightParen)   : os << ")";      break;
      case static_cast<uint>(ReadKB::Key::Exclamation)  : os << "!";      break;
      case static_cast<uint>(ReadKB::Key::At)           : os << "@";      break;
      case static_cast<uint>(ReadKB::Key::Hash)         : os << "#";      break;
      case static_cast<uint>(ReadKB::Key::Dollar)       : os << "$";      break;
      case static_cast<uint>(ReadKB::Key::Percent)      : os << "%";      break;
      case static_cast<uint>(ReadKB::Key::Circumflex)   : os << "^";      break;
      case static_cast<uint>(ReadKB::Key::Ampersand)    : os << "&";      break;
      case static_cast<uint>(ReadKB::Key::Asterisk)     : os << "*";      break;
      case static_cast<uint>(ReadKB::Key::LeftParen)    : os << ")";      break;
      case static_cast<uint>(ReadKB::Key::Colon)        : os << ":";      break;
      case static_cast<uint>(ReadKB::Key::Plus)         : os << "+";      break;
      case static_cast<uint>(ReadKB::Key::Space)        : os << "";       break;
      case static_cast<uint>(ReadKB::Key::Quote)        : os << "'";      break;
      case static_cast<uint>(ReadKB::Key::Comma)        : os << ",";      break;
      case static_cast<uint>(ReadKB::Key::Dash)         : os << "-";      break;
      case static_cast<uint>(ReadKB::Key::Period)       : os << ".";      break;
      case static_cast<uint>(ReadKB::Key::Slash)        : os << "/";      break;
      case static_cast<uint>(ReadKB::Key::Semicolon)    : os << ";";      break;
      case static_cast<uint>(ReadKB::Key::Equal)        : os << "=";      break;
      case static_cast<uint>(ReadKB::Key::Tilde)        : os << "~";      break;
      case static_cast<uint>(ReadKB::Key::LeftBrace)    : os << "{";      break;
      case static_cast<uint>(ReadKB::Key::Pipe)         : os << "|";      break;
      case static_cast<uint>(ReadKB::Key::RightBrace)   : os << "}";      break;
      case static_cast<uint>(ReadKB::Key::Grave)        : os << "`";      break;
      case static_cast<uint>(ReadKB::Key::LeftBracket)  : os << "[";      break;
      case static_cast<uint>(ReadKB::Key::Backslash)    : os << "\\";     break;
      case static_cast<uint>(ReadKB::Key::RightBracket) : os << "]";      break;
      case static_cast<uint>(ReadKB::Key::Backspace)    : os << "Bksp";   break;
      case static_cast<uint>(ReadKB::Key::Insert)       : os << "Ins";    break;
      case static_cast<uint>(ReadKB::Key::Delete)       : os << "Del";    break;
      case static_cast<uint>(ReadKB::Key::PageUp)       : os << "PgUp";   break;
      case static_cast<uint>(ReadKB::Key::PageDown)     : os << "PgDn";   break;
      case static_cast<uint>(ReadKB::Key::F1)           : os << "F1";     break;
      case static_cast<uint>(ReadKB::Key::F2)           : os << "F2";     break;
      case static_cast<uint>(ReadKB::Key::F3)           : os << "F3";     break;
      case static_cast<uint>(ReadKB::Key::F4)           : os << "F4";     break;
      case static_cast<uint>(ReadKB::Key::F5)           : os << "F5";     break;
      case static_cast<uint>(ReadKB::Key::F6)           : os << "F6";     break;
      case static_cast<uint>(ReadKB::Key::F7)           : os << "F7";     break;
      case static_cast<uint>(ReadKB::Key::F8)           : os << "F8";     break;
      case static_cast<uint>(ReadKB::Key::F9)           : os << "F9";     break;
      case static_cast<uint>(ReadKB::Key::F10)          : os << "F10";    break;
      case static_cast<uint>(ReadKB::Key::F11)          : os << "F11";    break;
      case static_cast<uint>(ReadKB::Key::F12)          : os << "F12";    break;
      case static_cast<uint>(ReadKB::Key::Up)           : os << "Up";     break;
      case static_cast<uint>(ReadKB::Key::Down)         : os << "Down";   break;
      case static_cast<uint>(ReadKB::Key::Right)        : os << "Right";  break;
      case static_cast<uint>(ReadKB::Key::Left)         : os << "Left";   break;
      case static_cast<uint>(ReadKB::Key::Center)       : os << "Center"; break;
      case static_cast<uint>(ReadKB::Key::End)          : os << "End";    break;
      case static_cast<uint>(ReadKB::Key::Home)         : os << "Home";   break;
      case static_cast<uint>(ReadKB::Key::Tab)          : os << "Tab";    break;
      case static_cast<uint>(ReadKB::Key::Enter)        : os << "Enter";  break;
      case static_cast<uint>(ReadKB::Key::Esc)          : os << "Esc";    break;
      // Error Codes ASCII
      case static_cast<uint>(ReadKB::Key::UNDEFINED_CSI) : os << "Undef CSI"; break;
      case static_cast<uint>(ReadKB::Key::UNDEFINED_SS3) : os << "Undef SS3"; break;
      case static_cast<uint>(ReadKB::Key::UNDEFINED_ESCAPE) : os << "Undef Esc"; break;
      case static_cast<uint>(ReadKB::Key::UNDEFINED) : os << "Undefined"; break;
      case static_cast<uint>(ReadKB::Key::ERROR) : os << "Error"; break;
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

ReadKB::Combo ReadKB::read_key() const {
  int num_ready;
  ReadKB::Combo key_pressed;

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

        key_pressed = categorizeBuffer(buf, s);

      } else {
        // Process other signals (POLLERR | POLLHUP | POLLNVAL)
        key_pressed = Key::ERROR;
      }
    }
  }
  return key_pressed;
}


ReadKB::Combo ReadKB::categorizeBuffer(const u_char *buf, const ssize_t len) const {
  assert(len > 0 && "Nothing in buffer to process");
  ReadKB::Combo key_pressed;
  if (len == 1 && buf[0] <= 127) {
    // ASCII
    key_pressed = static_cast<ReadKB::Combo>(buf[0]);
  } else {
    switch (buf[0]) {
      case '\033' : // Esc
        assert(len > 1 && "No more chars in buffer to read");
        switch (buf[1]) {
          case '[' : // Control Sequence Introducer
            key_pressed = Key::UNDEFINED_CSI;
            break;
          case 'O' : // Single Shift Three
            key_pressed = Key::UNDEFINED_SS3;
            break;
          // Alt-key
          default : key_pressed = Key::UNDEFINED_ESCAPE;
        }
        break;
      default : key_pressed = Key::UNDEFINED;
    }
  }

  return key_pressed;
};
