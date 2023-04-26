/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

#ifndef LIBREAD_KB_H
#define LIBREAD_KB_H

#include <string>
#include <cstdio>
#include <poll.h>

// use `tail -f "/tmp/read-kb-debug-log.txt"` from a terminal to see debug messages
#define DEBUG_LIB_READ_KB 0

class ReadKB {
  public:
    ReadKB();
    std::string getKey(void);

  private:
    struct pollfd * pfds;
    #if DEBUG_LIB_READ_KB == 1
      FILE* g_pDebugLogFile;
    #endif
};

#endif // LIBREAD_KB_H
