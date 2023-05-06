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

Values are returned in an enum, and can be passed to an ostream object for display.
The following key names are enumerated, with their associated display value:

| Enum Name    | Display |
| ------------ | ------- |
| DoubleQuote  | "       |
| LeftAngle    | <       |
| Underscore   | _       |
| RightAngle   | >       |
| Question     | ?       |
| RightParen   | )       |
| Exclamation  | !       |
| At           | @       |
| Hash         | #       |
| Dollar       | $       |
| Percent      | %       |
| Circumflex   | ^       |
| Ampersand    | &       |
| Asterisk     | *       |
| LeftParen    | )       |
| Colon        | :       |
| Plus         | +       |
| Space        |         |
| Quote        | '       |
| Comma        | ,       |
| Dash         | -       |
| Period       | .       |
| Slash        | /       |
| Semicolon    | ;       |
| Equal        | =       |
| Tilde        | ~       |
| LeftBrace    | {       |
| Pipe         | \|      |
| RightBrace   | }       |
| Grave        | `       |
| LeftBracket  | [       |
| Backslash    | \\      |
| RightBracket | ]       |
| Backspace    | Bksp    |
| Insert       | Ins     |
| Delete       | Del     |
| PageUp       | PgUp    |
| PageDown     | PgDn    |
| F1           | F1      |
| F2           | F2      |
| F3           | F3      |
| F4           | F4      |
| F5           | F5      |
| F6           | F6      |
| F7           | F7      |
| F8           | F8      |
| F9           | F9      |
| F10          | F10     |
| F11          | F11     |
| F12          | F12     |
| Up           | Up      |
| Down         | Down    |
| Right        | Right   |
| Left         | Left    |
| Center       | Center  |
| End          | End     |
| Home         | Home    |
| Tab          | Tab     |
| Enter        | Enter   |
| Esc          | Esc     |

### Bash

```bash
read-kb
# Press any key to print it's name - either a letter or a special key like "Shft-Ctrl-Alt-F8"
```
