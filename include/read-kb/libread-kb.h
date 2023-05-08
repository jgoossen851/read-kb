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
#include <utility>

// use `tail -f "/tmp/read-kb-debug-log.txt"` from a terminal to see debug messages
#define DEBUG_LIB_READ_KB 0

class ReadKB {
 public:
  enum class InputMode {
    Char,
    Line,
    File
  };

  ReadKB();
  ~ReadKB();

  /// A standalone key or combination of a key with potential modifier keys (Shift, Ctrl, Alt)
  class Key;
  /// Modifier keys that can be combined via & operator with a ReadKB::Key
  struct Mod;

  Key read_key() const;
  std::string read_line() const { return "Not yet implemented"; };
  std::string read_file() const { return "Not yet implemented"; };

  void setInput(const int &fd, const InputMode &mode);

 private:
  enum class BitmaskSet : uint { /// @todo Convert class to enum of {5, 6, 7, 8, 9, 16}
    // When mask is applied with &, the corresponding bits will be set
    Lowercase = 1<<5,
    Alpha     = 1<<6,
    Event     = 1<<7,
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

  typedef std::pair<BitmaskSet, BitmaskClear> Modifier;

  friend constexpr Modifier& operator&=(Modifier& m1, const Modifier& m2) {
    m1.first  = static_cast<BitmaskSet>  (static_cast<uint>(m1.first)   | static_cast<uint>(m2.first));
    m1.second = static_cast<BitmaskClear>(static_cast<uint>(m1.second)  | static_cast<uint>(m2.second));
    return m1;
  }
  friend constexpr Modifier operator&(const Modifier& m1, const Modifier& m2) {
    Modifier mod(m1);
    return mod  &= m2;
  }

  InputMode mode_ = InputMode::Char;
  struct pollfd *pfds;
  #if DEBUG_LIB_READ_KB == 1
    FILE* g_pDebugLogFile;
  #endif

  void      resetTerminal(const int fd);
  Key       categorizeBuffer(const u_char *buf, const ssize_t len) const;
  Modifier  categorizeMod(const u_char c) const;
};


struct ReadKB::Mod {
  static constexpr Modifier Shft  {static_cast<BitmaskSet>(0),  BitmaskClear::Shift};
  static constexpr Modifier Ctrl  {BitmaskSet::Control,         static_cast<BitmaskClear>(0)};
  static constexpr Modifier Alt   {BitmaskSet::Alternate,       static_cast<BitmaskClear>(0)};
  static constexpr Modifier Event {BitmaskSet::Event,           static_cast<BitmaskClear>(0)};
};

class ReadKB::Key {
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

 private:
  constexpr KeyValue ascii2key(const char& ascii) {
    // Cast directly from underlying ASCII value (Most Alphanumerics)
    if ((ascii == ' ') ||
        (ascii == '\'') ||
        (ascii >= ',' && ascii <= '9') ||
        (ascii == ';') ||
        (ascii == '=') ||
        (ascii >= 'A' && ascii <= 'Z') ||
        (ascii >= '`' && ascii <= 'z') ||
        (ascii == 127) // DEL
       ) {
      return static_cast<KeyValue>(ascii);

    // Add offset of 0x160 (Most Control Codes)
    } else if ((ascii >= 1  && ascii <= 7)  || // SOH - BEL
               (ascii >= 11 && ascii <= 26) || // VT - SUB
               (ascii >= 28 && ascii <= 29)) { // FS - GS
      return static_cast<KeyValue>(ascii + (1<<8) + (6<<4));

    // Add offset of -0x10 (Few shifted symbols)
    } else if ((ascii == '!') ||
               (ascii >= '#' && ascii <= '%')) {
      return static_cast<KeyValue>(ascii - (1<<4));

    // Add offset of 0x20 (Base brackets)
    } else if (ascii >= '[' && ascii <= ']') {
      return static_cast<KeyValue>(ascii + (1<<5));

    // Add offset of -0x20 (Shifted brackets)
    } else if (ascii >= '{' && ascii <= '}') {
      return static_cast<KeyValue>(ascii - (1<<5));

    // Add offset of 0xE0 (Tab, Enter, Esc)
    } else if ((ascii >= 9 && ascii <= 10) || // HT - LF
               (ascii == 27)) {               // ESC
      return static_cast<KeyValue>(ascii + (14<<4));

    // Individually Set
    } else {
      switch (ascii) {
        case 0  : return (KeyValue::At & Mod::Ctrl).mkey; break;
        case 8  : return (KeyValue::Backspace & Mod::Ctrl).mkey; break;
        case 30 : return (KeyValue::Circumflex & Mod::Ctrl).mkey; break;
        case 31 : return (KeyValue::Underscore & Mod::Ctrl).mkey; break;
        case '"': return KeyValue::DoubleQuote; break;
        case '&': return KeyValue::Ampersand; break;
        case '(': return KeyValue::LeftParen; break;
        case ')': return KeyValue::RightParen; break;
        case '*': return KeyValue::Asterisk; break;
        case '+': return KeyValue::Plus; break;
        case ':': return KeyValue::Colon; break;
        case '<': return KeyValue::LeftAngle; break;
        case '>': return KeyValue::RightAngle; break;
        case '?': return KeyValue::Question; break;
        case '@': return KeyValue::At; break;
        case '^': return KeyValue::Circumflex; break;
        case '_': return KeyValue::Underscore; break;
        case '~': return KeyValue::Tilde; break;
        default : return KeyValue::UNDEFINED;
      }
    }
  }

 public:
  /// Constructor Functions
  constexpr Key()
    : mkey(KeyValue::ERROR)             {};
  constexpr Key(const uint &key)
    : mkey(static_cast<KeyValue>(key))  {};
  constexpr Key(const char &key)
    : mkey(ascii2key(key))              {};

  // Promoter to integral type for use in switch
  constexpr operator uint() const {return static_cast<uint>(mkey);}

  // Bitwise operators
  friend constexpr Key operator&(const Modifier& mod, const Key& key) {
    return Key((key | static_cast<uint>(mod.first)) & ~static_cast<uint>(mod.second));
  }
  friend constexpr Key operator&(const Key& key, const Modifier& mod) {
    return mod & key;
  }

  /// Stream insertion operator
  friend std::ostream& operator<<(std::ostream& os, const Key& kb);

 private:
  KeyValue mkey;

  constexpr Key& unsetMask(const Modifier mod) {
    mkey = static_cast<KeyValue>((mkey & ~static_cast<uint>(mod.first)) | static_cast<uint>(mod.second));
    return *this;
  }
};


#endif // LIBREAD_KB_H
