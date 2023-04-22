/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <map>
#include <poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

// use `tail -f "/tmp/read-kb-debug-log.txt"` from a terminal to see debug messages
#define DEBUG 0

#define errorExit(msg)  do { perror(msg); exit(EXIT_FAILURE); \
                           } while (0)

#if DEBUG == 0
#   define printlog(...)   do {} while (0)
#else
#   define printlog(...)   do { fprintf(pDebugLogFile, __VA_ARGS__); \
                                fseek(pDebugLogFile, 0, SEEK_END); \
                              } while (0)
#endif

enum COMMANDS {
  NOT_DEFINED,
  EXIT_CODE,
  HELP,
  UP,
  DOWN
};

// Initialization function for command map
static std::map<std::string, COMMANDS> InitializeMap();

int main(int argc, char* argv[]) {

  #if DEBUG == 1
    FILE* pDebugLogFile;
    pDebugLogFile = fopen ("/tmp/read-kb-debug-log.txt", "w");
  #endif

  // Check that a single argument was given
  if (argc != 2) {
    std::cerr << argv[0] << " requires a single argument containing the input pipe." << std::endl;
    return 1;
  }

  // Initialize the command map
  std::map<std::string, COMMANDS> dictionary = InitializeMap();

  // Get file descriptors for the poll command
  struct pollfd *pfds;
  pfds = static_cast<pollfd*>(calloc(1, sizeof(struct pollfd)));
  if (pfds == NULL) {
    errorExit("malloc");
  }

  // Open each file, and add it to 'pfds' array
  pfds[0].fd = open(argv[1], O_RDONLY);
  if (pfds[0].fd == -1) {
      errorExit("open");
  }

  printlog("Opened \"%s\" on fd %d\n", argv[1], pfds[0].fd);

  // Request poll() to scan file descriptor for data available to read (POLLIN signal)
  pfds[0].events = POLLIN;
  



//   std::fstream fs;
//   std::string input;
//   fs.open(argv[1], std::fstream::in);
// //   while(std::getline(fs, input)) { // quit the program with Ctrl+D (EOF) or Ctrl+C (SIGINT)
 
 
 
  // Keep calling poll() as long as at least one file descriptor is open.
  while(true) {
    int num_ready;

    // printf("About to poll()\n");
    num_ready = poll(pfds, 1, -1);
    if (num_ready == -1) {
      errorExit("poll");
    }

    // printf("Ready: %d\n", num_ready);

    // Deal with array returned by poll()

    {
      char buf[10];

      if (pfds[0].revents != 0) {
        // printf("  fd=%d; events: %s%s%s\n", pfds[0].fd,
        //     (pfds[0].revents & POLLIN)  ? "POLLIN "  : "",
        //     (pfds[0].revents & POLLHUP) ? "POLLHUP " : "",
        //     (pfds[0].revents & POLLERR) ? "POLLERR " : "");

        if (pfds[0].revents & POLLIN) {
          ssize_t s = read(pfds[0].fd, buf, sizeof(buf));
          if (s == -1) {
            errorExit("read");
          }
          if (s != 0) {
            printlog("    read %zd bytes: %.*s\n",
                   s, (int) s, buf);
          }
        } else {                /* POLLERR | POLLHUP */
          printlog("    closing fd %d\n", pfds[0].fd);
          if (close(pfds[0].fd) == -1) {
            errorExit("close");
          }
          break;
        }
      }
    }



    // switch (dictionary[input]) {
    // case EXIT_CODE : // Exit Condition
    //     fs.close();
    //     return 0;
    //     break;
    // case HELP : // Print Usage
    //     std::cout << "Help" << std::endl;
    //     break;
    // case UP :
    //     std::cout << "^" << std::endl;
    //     break;
    // case DOWN :
    //     std::cout << "v" << std::endl;
    //     break;
    // default :
    //     std::cout << "(" << input << ")" << std::endl; 
    // }

  }

  printlog("All file descriptors closed; bye\n");

  return 0;
}

std::map<std::string, COMMANDS> InitializeMap(){
  
  std::map< std::string, COMMANDS > dictionary;

  // Define user commands
  dictionary["SIGINT"]  = EXIT_CODE;
  dictionary["h"]       = HELP;
  dictionary["Up"]      = UP;
  dictionary["Down"]    = DOWN;

  return dictionary;
}
