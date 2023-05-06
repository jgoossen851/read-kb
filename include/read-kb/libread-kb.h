/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

#ifndef LIBREAD_KB_H
#define LIBREAD_KB_H

#include <poll.h>

#include <cstdio>
#include <ostream>
#include <string>

// use `tail -f "/tmp/read-kb-debug-log.txt"` from a terminal to see debug messages
#define DEBUG_LIB_READ_KB 0

enum ReadKbMode {
  KB_CHAR,
  KB_LINE,
  KB_FILE
};


class ReadKB {
  public:
    enum class Key {
      DoubleQuote,
      LeftAngle,
      Underscore,
      RightAngle,
      Question,
      RightParen,
      Exclamation,
      At,
      Hash,
      Dollar,
      Percent,
      Circumflex,
      Ampersand,
      Asterisk,
      LeftParen,
      Colon,
      Plus,
      Space,
      Quote,
      Comma,
      Dash,
      Period,
      Slash,
      Semicolon,
      Equal,
      Tilde,
      LeftBrace,
      Pipe,
      RightBrace,
      Grave,
      LeftBracket,
      Backslash,
      RightBracket,
      Backspace,
      Insert,
      Delete,
      PageUp,
      PageDown,
      F1,
      F2,
      F3,
      F4,
      F5,
      F6,
      F7,
      F8,
      F9,
      F10,
      F11,
      F12,
      Up,
      Down,
      Right,
      Left,
      Center,
      End,
      Home,
      Tab,
      Enter,
      Esc,
      UNDEFINED_CSI,
      UNDEFINED_SS3,
      UNDEFINED_ESCAPE,
      UNDEFINED,
      ERROR
    };

    ReadKB();
    Key read_key() const;
    std::string read_line() const { return "Not yet implemented"; };
    std::string read_file() const { return "Not yet implemented"; };

  private:
    ReadKbMode mode_ = KB_CHAR;
    struct pollfd *pfds;
    #if DEBUG_LIB_READ_KB == 1
      FILE* g_pDebugLogFile;
    #endif

    Key categorizeBuffer(const u_char *buf, const ssize_t len) const;
};

std::ostream& operator<<(std::ostream& os, const ReadKB::Key kb);

#endif // LIBREAD_KB_H
