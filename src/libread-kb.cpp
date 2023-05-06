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

std::ostream& operator<<(std::ostream& os, const ReadKB::Key& kbMods) {
  // Remove modifiers and add to output stream
  ReadKB::Key kb = kbMods;
  if(kb != kb.unsetMask(ReadKB::Mod::Ctrl)) { os << "Ctrl-"; }
  if(kb != kb.unsetMask(ReadKB::Mod::Alt))  { os << "Alt-"; }
  if(kb == (kb & ReadKB::Mod::Event) &&
     kb != kb.unsetMask(ReadKB::Mod::Shft)) { os << "Shft-"; }

  // Decode base key
  if ((kb >= static_cast<uint>('a') && kb <= static_cast<uint>('z')) ||
      (kb >= static_cast<uint>('A') && kb <= static_cast<uint>('Z')) ||
      (kb >= static_cast<uint>('0') && kb <= static_cast<uint>('9'))) {
    // Alphanumeric
    os << static_cast<char>(kb.mkey);
  } else {
    switch (kb) {
      case ReadKB::Key::DoubleQuote  : os << "\"";     break;
      case ReadKB::Key::LeftAngle    : os << "<";      break;
      case ReadKB::Key::Underscore   : os << "_";      break;
      case ReadKB::Key::RightAngle   : os << ">";      break;
      case ReadKB::Key::Question     : os << "?";      break;
      case ReadKB::Key::RightParen   : os << ")";      break;
      case ReadKB::Key::Exclamation  : os << "!";      break;
      case ReadKB::Key::At           : os << "@";      break;
      case ReadKB::Key::Hash         : os << "#";      break;
      case ReadKB::Key::Dollar       : os << "$";      break;
      case ReadKB::Key::Percent      : os << "%";      break;
      case ReadKB::Key::Circumflex   : os << "^";      break;
      case ReadKB::Key::Ampersand    : os << "&";      break;
      case ReadKB::Key::Asterisk     : os << "*";      break;
      case ReadKB::Key::LeftParen    : os << "(";      break;
      case ReadKB::Key::Colon        : os << ":";      break;
      case ReadKB::Key::Plus         : os << "+";      break;
      case ReadKB::Key::Space        : os << " ";      break;
      case ReadKB::Key::Quote        : os << "'";      break;
      case ReadKB::Key::Comma        : os << ",";      break;
      case ReadKB::Key::Dash         : os << "-";      break;
      case ReadKB::Key::Period       : os << ".";      break;
      case ReadKB::Key::Slash        : os << "/";      break;
      case ReadKB::Key::Semicolon    : os << ";";      break;
      case ReadKB::Key::Equal        : os << "=";      break;
      case ReadKB::Key::Tilde        : os << "~";      break;
      case ReadKB::Key::LeftBrace    : os << "{";      break;
      case ReadKB::Key::Pipe         : os << "|";      break;
      case ReadKB::Key::RightBrace   : os << "}";      break;
      case ReadKB::Key::Grave        : os << "`";      break;
      case ReadKB::Key::LeftBracket  : os << "[";      break;
      case ReadKB::Key::Backslash    : os << "\\";     break;
      case ReadKB::Key::RightBracket : os << "]";      break;
      case ReadKB::Key::Backspace    : os << "Bksp";   break;
      case ReadKB::Key::Insert       : os << "Ins";    break;
      case ReadKB::Key::Delete       : os << "Del";    break;
      case ReadKB::Key::PageUp       : os << "PgUp";   break;
      case ReadKB::Key::PageDown     : os << "PgDn";   break;
      case ReadKB::Key::F1           : os << "F1";     break;
      case ReadKB::Key::F2           : os << "F2";     break;
      case ReadKB::Key::F3           : os << "F3";     break;
      case ReadKB::Key::F4           : os << "F4";     break;
      case ReadKB::Key::F5           : os << "F5";     break;
      case ReadKB::Key::F6           : os << "F6";     break;
      case ReadKB::Key::F7           : os << "F7";     break;
      case ReadKB::Key::F8           : os << "F8";     break;
      case ReadKB::Key::F9           : os << "F9";     break;
      case ReadKB::Key::F10          : os << "F10";    break;
      case ReadKB::Key::F11          : os << "F11";    break;
      case ReadKB::Key::F12          : os << "F12";    break;
      case ReadKB::Key::Up           : os << "Up";     break;
      case ReadKB::Key::Down         : os << "Down";   break;
      case ReadKB::Key::Right        : os << "Right";  break;
      case ReadKB::Key::Left         : os << "Left";   break;
      case ReadKB::Key::Center       : os << "Center"; break;
      case ReadKB::Key::End          : os << "End";    break;
      case ReadKB::Key::Home         : os << "Home";   break;
      case ReadKB::Key::Tab          : os << "Tab";    break;
      case ReadKB::Key::Enter        : os << "Enter";  break;
      case ReadKB::Key::Esc          : os << "Esc";    break;
      // Error Codes
      case ReadKB::Key::UNDEFINED_CSI    : os << "Undef CSI"; break;
      case ReadKB::Key::UNDEFINED_SS3    : os << "Undef SS3"; break;
      case ReadKB::Key::UNDEFINED_ESCAPE : os << "Undef Esc"; break;
      case ReadKB::Key::UNDEFINED        : os << "Undefined"; break;
      case ReadKB::Key::ERROR            : os << "Error";     break;
      default : os << "Error"; break;
    }
  }
  return os;
}

ReadKB::ReadKB() {
  // Initialize debugging log file
  #if DEBUG_LIB_READ_KB == 1
    g_pDebugLogFile = fopen ("/tmp/read-kb-debug-log.txt", "w");
  #endif

  // Set input mode to get single characters
  setInputMode(STDIN_FD, InputMode::Char);

  // Allocate memory for file descriptor for the poll command
  pfds = static_cast<pollfd*>(calloc(1, sizeof(struct pollfd)));
  errorIf(pfds == NULL, "malloc");

  // Assign file descriptor to poll structure
  setInputFile(STDIN_FD);

  // Request poll() to scan file descriptor for data available to read (POLLIN signal)
  pfds->events = POLLIN;
}

ReadKB::~ReadKB() {
  resetTerminal(STDIN_FD);
}

ReadKB::Key ReadKB::read_key() const {
  int num_ready;
  ReadKB::Key key_pressed;

  printlog("Polling pipe for signal or data... ");
  num_ready = poll(pfds, 1, -1);
  errorIf(num_ready == -1, "poll");
  printlog("Pipes ready: %d\n", num_ready);

  // Deal with array returned by poll()
  {
    // Read from pipe one buffer-length at a time
    u_char buf[BUFF_SIZE_CHARS];

    if (pfds->revents != 0) {

      // Log signals (events) found by poll
      printlog("  fd=%d; events: %s%s%s%s\n", pfds->fd,
          (pfds->revents & POLLIN)   ? "\033[32mPOLLIN\033[0m "   : "",
          (pfds->revents & POLLHUP)  ? "\033[33mPOLLHUP\033[0m "  : "",
          (pfds->revents & POLLERR)  ? "\033[31mPOLLERR\033[0m "  : "",
          (pfds->revents & POLLNVAL) ? "\033[31mPOLLNVAL\033[0m " : "");

      if (pfds->revents & POLLIN) {
        // Read from the pipe if there is data available (POLLIN)

        ssize_t s = read(pfds->fd, buf, sizeof(buf));
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

void ReadKB::resetTerminal(const int fd) {
  struct termios term;
  tcgetattr(fd, &term);
  term.c_lflag |= ICANON; // Canonical mode
  term.c_lflag |= ECHO;   // Print input
  errorIf(tcsetattr(fd, TCSANOW, &term) == -1, "termios reset");
}

void ReadKB::setInputMode(const int &fd, const InputMode &mode) {
  switch (mode) {
    case InputMode::Char :
      // Turn off OS buffering on standard input (non-canonical mode)
      struct termios term;
      tcgetattr(fd, &term);
      term.c_lflag &= ~ICANON; // Non-canonical mode
      term.c_lflag &= ~ECHO;   // Do not print input
      errorIf(tcsetattr(fd, TCSANOW, &term) == -1, "termios");
      break;
    case InputMode::Line :
      resetTerminal(fd);
      break;
    case InputMode::File :
      resetTerminal(fd);
      break;
  }
  mode_ = mode;
}

void ReadKB::setInputFile(const int &fd) {
  // Assign file descriptor to poll structure
  pfds->fd = fd;
  errorIf(pfds->fd == -1, "open");
  printlog("Reading input from fd %d\n", pfds->fd);
}


ReadKB::Key ReadKB::categorizeBuffer(const u_char *buf, const ssize_t len) const {
  assert(len > 0 && "Nothing in buffer to process");
  ReadKB::Key key_pressed;
  if (len == 1 && buf[0] <= 127) {
    // ASCII
    key_pressed = static_cast<ReadKB::Key>(char(buf[0]));
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
