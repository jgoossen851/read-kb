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

The values of the enum are given below:

| Value   | Base   | Shift | Ctrl  | Alt   |
| ------- | ------ | ----- | ----- | ----- |
| Offset: | 0x00   | -0x20 | 0x100 | 0x200 |
| 0x20    | Space  |       |       |       |
| 0x27    | '      | "     |       |       |
| 0x2C    | ,      | <     |       |       |
| 0x2D    | -      | _     |       |       |
| 0x2E    | .      | >     |       |       |
| 0x2F    | /      | ?     |       |       |
| 0x20    | 0      | )     |       |       |
| 0x21    | 1      | !     |       |       |
| 0x22    | 2      | @     |       |       |
| 0x23    | 3      | #     |       |       |
| 0x24    | 4      | $     |       |       |
| 0x25    | 5      | %     |       |       |
| 0x26    | 6      | ^     |       |       |
| 0x27    | 7      | &     |       |       |
| 0x28    | 8      | *     |       |       |
| 0x29    | 9      | (     |       |       |
| 0x2B    | ;      | :     |       |       |
| 0x2D    | =      | +     |       |       |
| 0x60    | `      | ~     |       |       |
| 0x61    | a      | A     |       |       |
| 0x62    | b      | B     |       |       |
| 0x63    | c      | C     |       |       |
| 0x64    | d      | D     |       |       |
| 0x65    | e      | E     |       |       |
| 0x66    | f      | F     |       |       |
| 0x67    | g      | G     |       |       |
| 0x68    | h      | H     |       |       |
| 0x69    | i      | I     |       |       |
| 0x6A    | j      | J     |       |       |
| 0x6B    | k      | K     |       |       |
| 0x6C    | l      | L     |       |       |
| 0x6D    | m      | M     |       |       |
| 0x6E    | n      | N     |       |       |
| 0x6F    | o      | O     |       |       |
| 0x60    | p      | P     |       |       |
| 0x61    | q      | Q     |       |       |
| 0x62    | r      | R     |       |       |
| 0x63    | s      | S     |       |       |
| 0x64    | t      | T     |       |       |
| 0x65    | u      | U     |       |       |
| 0x66    | v      | V     |       |       |
| 0x67    | w      | W     |       |       |
| 0x68    | x      | X     |       |       |
| 0x69    | y      | Y     |       |       |
| 0x6A    | z      | Z     |       |       |
| 0x6B    | [      | {     |       |       |
| 0x6C    | \\     | \|    |       |       |
| 0x6D    | ]      | }     |       |       |
| 0x6F    | Bksp   |       |       |       |
| 0xA2    | Ins    |       |       |       |
| 0xA3    | Del    |       |       |       |
| 0xA5    | PgUp   |       |       |       |
| 0xA6    | PgDn   |       |       |       |
| 0xAB    | F1     |       |       |       |
| 0xAC    | F2     |       |       |       |
| 0xAD    | F3     |       |       |       |
| 0xAE    | F4     |       |       |       |
| 0xAF    | F5     |       |       |       |
| 0xA1    | F6     |       |       |       |
| 0xA2    | F7     |       |       |       |
| 0xA3    | F8     |       |       |       |
| 0xA4    | F9     |       |       |       |
| 0xA5    | F10    |       |       |       |
| 0xA6    | F11    |       |       |       |
| 0xA7    | F12    |       |       |       |
| 0xE1    | Up     |       |       |       |
| 0xE2    | Down   |       |       |       |
| 0xE3    | Right  |       |       |       |
| 0xE4    | Left   |       |       |       |
| 0xE5    | Center |       |       |       |
| 0xE6    | End    |       |       |       |
| 0xE8    | Home   |       |       |       |
| 0xE9    | Tab    |       |       |       |
| 0xEA    | Enter  |       |       |       |
| 0xEB    | Esc    |       |       |       |

### Bash

```bash
read-kb
# Press any key to print it's name - either a letter or a special key like "Shft-Ctrl-Alt-F8"
```
