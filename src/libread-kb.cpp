/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

#include "libread-kb.h"

#include <termios.h>

#include <cstdio>
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

}

std::string ReadKB::read_key() {
  char ch;
  std::cin >> ch;
  return std::to_string((int)ch);
}
