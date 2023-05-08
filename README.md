# read-kb

Single-key keyboard reader for interactive program input

## Installation

```bash
cmake -S . -B build/                   # Generate build directory and configure project
cmake --build build/ --target install  # Install program
```

Making the `install` target installs the following:

* Program `read-kb` to `/usr/local/bin/`

## Usage

First make the `install` target.

### C++

Values are returned as a `ReadKB::Key` class. Key values are provided as an unscoped enum within the class.
Objects of the `ReadKB::Key` class can be promoted to an integral type and can therefore be used in a `switch` statement.
The output stream operator `<<` is also defined for the class to return the key's display name.
A key in the `ReadKB::Key` class may be modified by modifier keys in the `ReadKB::Mod` class using the bitwise "and" operator (`&`).
The following key names are enumerated, listed with their associated display value:

| Enum Name    | Display  |
| ------------ | -------- |
| DoubleQuote  | `"`      |
| LeftAngle    | `<`      |
| Underscore   | `_`      |
| RightAngle   | `>`      |
| Question     | `?`      |
| RightParen   | `)`      |
| Exclamation  | `!`      |
| At           | `@`      |
| Hash         | `#`      |
| Dollar       | `$`      |
| Percent      | `%`      |
| Circumflex   | `^`      |
| Ampersand    | `&`      |
| Asterisk     | `*`      |
| LeftParen    | `)`      |
| Colon        | `:`      |
| Plus         | `+`      |
| Space        | ` `      |
| Quote        | `'`      |
| Comma        | `,`      |
| Dash         | `-`      |
| Period       | `.`      |
| Slash        | `/`      |
| Semicolon    | `;`      |
| Equal        | `=`      |
| Tilde        | `~`      |
| LeftBrace    | `{`      |
| Pipe         | `\|`     |
| RightBrace   | `}`      |
| Grave        | `` ` ``  |
| LeftBracket  | `[`      |
| Backslash    | `\`      |
| RightBracket | `]`      |
| Backspace    | `Bksp`   |
| Insert       | `Ins`    |
| Delete       | `Del`    |
| PageUp       | `PgUp`   |
| PageDown     | `PgDn`   |
| F1           | `F1`     |
| F2           | `F2`     |
| F3           | `F3`     |
| F4           | `F4`     |
| F5           | `F5`     |
| F6           | `F6`     |
| F7           | `F7`     |
| F8           | `F8`     |
| F9           | `F9`     |
| F10          | `F10`    |
| F11          | `F11`    |
| F12          | `F12`    |
| Up           | `Up`     |
| Down         | `Down`   |
| Right        | `Right`  |
| Left         | `Left`   |
| Center       | `Center` |
| End          | `End`    |
| Home         | `Home`   |
| Tab          | `Tab`    |
| Enter        | `Enter`  |
| Esc          | `Esc`    |

Unnamed enums may be constructed from any 7-bit ASCII character, such as `ReadKB::Key('a')` or `ReadKB::(0)` for unprintable control codes.

### Bash

```bash
read-kb
# Press any key to print it's name - either a letter or a special key like "Shft-Ctrl-Alt-F8"
```
