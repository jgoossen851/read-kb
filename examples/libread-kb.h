/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

#ifndef LIBREAD_KB_H
#define LIBREAD_KB_H

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

struct pollfd*  setup_readkb(char* pipe);
std::string     getChar_readkb(struct pollfd* pfds);
void            close_readkb(struct pollfd* pfds);

#endif // LIBREAD_KB_H
