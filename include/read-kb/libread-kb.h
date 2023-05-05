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
      NUL = 0,  SOH,  STX,  ETX,
      EOT,      ENQ,  ACK,  BEL,
      BS,       HT,   LF,   VT,
      FF,       CR,   SO,   SI,
      DLE,      DC1,  DC2,  DC3,
      DC4,      NAK,  SYN,  ETB,
      CAN,      EM,   SUB,  ESC,
      FS,       GS,   RS,   US,
      SPACE = 32, // First printable character
      DEL = 127,
      // Aliases
      CTRL_BKSP = BS,
      TAB = HT,
      ENTER = LF,
      XON = DC1,
      XOFF = DC3,
      BACKSPACE = DEL,
      // Beyond ASCII
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
