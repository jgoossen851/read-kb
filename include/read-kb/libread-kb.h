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
 private:
  enum class BitmaskSet : uint { /// @todo Convert class to enum of {5, 6, 7, 8, 9, 16}
    // When mask is applied with &, the corresponding bits will be set
    Lowercase = 1<<5,
    Alpha     = 1<<6,
    Function  = 1<<7,
    Control   = 1<<8,
    Alternate = 1<<9,
    ERROR     = 1<<16,
    UNDEFINED
  };
  
  enum class BitmaskClear : uint {
    // When mask is applied with &, the corresponding bits will be cleared
    Shift   = static_cast<uint>(BitmaskSet::Lowercase),
    Number  = static_cast<uint>(BitmaskSet::Alpha),
    ERROR   = 1<<16,
    UNDEFINED
  };

 public:
  struct Mod {
    static const BitmaskClear Shft = BitmaskClear::Shift;
    static const BitmaskSet   Ctrl = BitmaskSet::Control;
    static const BitmaskSet   Alt  = BitmaskSet::Alternate;
  };

  /// A combination of a key with potential modifier keys (Shift, Ctrl, Alt)
  class Key {
   public:
    // Unscoped enum to enable implicit conversion to uint
    enum KeyValue: uint {
      DoubleQuote = 7,
      LeftAngle   = 12,  Underscore, RightAngle, Question,
                         RightParen, Exclamation, At, Hash, Dollar,
                         Percent, Circumflex, Ampersand, Asterisk, LeftParen,
      Colon       = 27,
      Plus        = 29,
      Space       = 32,
      Quote       = 39,
      Comma       = 44,  Dash, Period, Slash,
      Semicolon   = 59,
      Equal       = 61,
      Tilde       = 64,
      LeftBrace   = 91,  Pipe, RightBrace,
      Grave       = 96,
      LeftBracket = 123, Backslash, RightBracket,
      Backspace   = 127,
      Insert      = 162, Delete,
      PageUp      = 165, PageDown,
      F1          = 171, F2, F3, F4, F5,
      F6          = 177, F7, F8, F9, F10, F11, F12,
      Up          = 225, Down, Right, Left, Center, End,
      Home        = 232, Tab, Enter,
      Esc         = 251,
      ERROR       = 1<<16,
      UNDEFINED_CSI,
      UNDEFINED_SS3,
      UNDEFINED_ESCAPE,
      UNDEFINED
    };

    constexpr Key()
      : mkey(KeyValue::ERROR) {};
    constexpr Key(const uint key)
      : mkey(static_cast<KeyValue>(key)) {};

    // Promoter to integral type for use in switch
    constexpr operator uint() const {return mkey;}

    // Bitwise operators
    friend constexpr ReadKB::Key operator&(const ReadKB::Key& key, const ReadKB::BitmaskSet& mSet) {
      return ReadKB::Key(key.mkey | static_cast<uint>(mSet));
    }
    friend constexpr ReadKB::Key operator&(const ReadKB::Key& key, const ReadKB::BitmaskClear& mClr) {
      return ReadKB::Key(key.mkey & ~static_cast<uint>(mClr));
    }

    friend std::ostream& operator<<(std::ostream& os, const ReadKB::Key& kb);

   private:
    KeyValue mkey;
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

#endif // LIBREAD_KB_H
