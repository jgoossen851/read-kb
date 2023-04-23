/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

// #include <cstdio>
// #include <fstream>
// #include <string>
// #include <map>
// #include <poll.h>
// #include <fcntl.h>
// #include <sys/types.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <cstring>

// use `tail -f "/tmp/read-kb-debug-log.txt"` from a terminal to see debug messages
#define DEBUG_LIB_READ_KB 0

#define errorExit(msg)  do { perror(msg); exit(EXIT_FAILURE); \
                           } while (0)

#if DEBUG_LIB_READ_KB == 0
#   define printlog(...)   do {} while (0)
#else
#   define printlog(...)   do { fprintf(pDebugLogFile, __VA_ARGS__); \
                                fseek(pDebugLogFile, 0, SEEK_END); \
                              } while (0)
#endif

struct pollfd* setup_readkb(char* pipe);
std::string getChar_readkb(struct pollfd* pfds);
