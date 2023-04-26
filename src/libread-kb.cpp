/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

#include <string>
#include <cstdio>
#include <poll.h>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>

#include "libread-kb.h"

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

  // Get file descriptors for the poll command
  pfds = static_cast<pollfd*>(calloc(1, sizeof(struct pollfd)));
  errorIf(pfds == NULL, "malloc");

  // Open each file, and add it to 'pfds' array
  pfds[0].fd = STDIN_FD;
  errorIf(pfds[0].fd == -1, "open");
  printlog("Opened \"%s\" on fd %d\n", "stdin", pfds[0].fd);

  // Request poll() to scan file descriptor for data available to read (POLLIN signal)
  pfds[0].events = POLLIN;
}

std::string ReadKB::getKey() {
  int num_ready;
  std::string key_pressed;

  printlog("Polling pipe for signal or data... ");
  num_ready = poll(pfds, 1, -1);
  errorIf(num_ready == -1, "poll");
  printlog("Pipes ready: %d\n", num_ready);

  // Deal with array returned by poll()
  {
    // Read from pipe one buffer-length at a time
    char buf[27]; // Longest name is "Shft-Ctrl-Alt-NumpadCenter"

    if (pfds[0].revents != 0) {

      // Log signals (events) found by poll
      printlog("  fd=%d; events: %s%s%s%s\n", pfds[0].fd,
          (pfds[0].revents & POLLIN)   ? "\e[32mPOLLIN\e[0m "   : "",
          (pfds[0].revents & POLLHUP)  ? "\e[33mPOLLHUP\e[0m "  : "",
          (pfds[0].revents & POLLERR)  ? "\e[31mPOLLERR\e[0m "  : "",
          (pfds[0].revents & POLLNVAL) ? "\e[31mPOLLNVAL\e[0m " : "");

      if (pfds[0].revents & POLLIN) {
        // Read from the pipe if there is data available (POLLIN)

        ssize_t s = read(pfds[0].fd, buf, sizeof(buf));
        errorIf(s == -1, "read");
        printlog("    read %zd bytes: \e[1m%.*s\e[0m\n", s, (int) s, buf);

        key_pressed.assign(buf, s-1);

      } else {
        // Process other signals (POLLERR | POLLHUP | POLLNVAL)
        key_pressed.assign("SIGINT");
      }
    }
  }
  return key_pressed;
}
