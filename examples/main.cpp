/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

#define errorExit(msg)  do { perror(msg); exit(EXIT_FAILURE); \
                           } while (0)

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

  // Check that a single argument was given
  if (argc != 2) {
    std::cerr << argv[0] << " requires a single argument containing the input pipe." << std::endl;
    return 1;
  }

  // Initialize the command map
  std::map<std::string, COMMANDS> dictionary = InitializeMap();

  // Get file descriptors for the poll command
  int nfds, num_open_fds;
  struct pollfd *pfds;
  num_open_fds = nfds = argc - 1; // Number of file descriptors
  pfds = static_cast<pollfd*>(calloc(nfds, sizeof(struct pollfd)));
  if (pfds == NULL) {
    errorExit("malloc");
  }

  // Open each file, and add it to 'pfds' array
  for (int jj = 0; jj < nfds; jj++) {
    pfds[jj].fd = open(argv[jj + 1], O_RDONLY);
    if (pfds[jj].fd == -1) {
      errorExit("open");
    }

    printf("Opened \"%s\" on fd %d\n", argv[jj + 1], pfds[jj].fd);

    pfds[jj].events = POLLIN;
  }



//   std::fstream fs;
//   std::string input;
//   fs.open(argv[1], std::fstream::in);
// //   while(std::getline(fs, input)) { // quit the program with Ctrl+D (EOF) or Ctrl+C (SIGINT)
 
 
 
  // Keep calling poll() as long as at least one file descriptor is open.
  while(num_open_fds > 0) {
    int ready;

    // printf("About to poll()\n");
    ready = poll(pfds, nfds, -1);
    if (ready == -1) {
      errorExit("poll");
    }

    // printf("Ready: %d\n", ready);

    // Deal with array returned by poll()

    for (int j = 0; j < nfds; j++) {
      char buf[10];

      if (pfds[j].revents != 0) {
        // printf("  fd=%d; events: %s%s%s\n", pfds[j].fd,
        //     (pfds[j].revents & POLLIN)  ? "POLLIN "  : "",
        //     (pfds[j].revents & POLLHUP) ? "POLLHUP " : "",
        //     (pfds[j].revents & POLLERR) ? "POLLERR " : "");

        if (pfds[j].revents & POLLIN) {
          ssize_t s = read(pfds[j].fd, buf, sizeof(buf));
          if (s == -1) {
            errorExit("read");
          }
          if (s != 0) {
            printf("    read %zd bytes: %.*s\n",
                   s, (int) s, buf);
          }
        } else {                /* POLLERR | POLLHUP */
        //   printf("    closing fd %d\n", pfds[j].fd);
        //   if (close(pfds[j].fd) == -1) {
        //     errorExit("close");
        //   }
        //   num_open_fds--;
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

  printf("All file descriptors closed; bye\n");

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
