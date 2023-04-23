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
  DOWN,
  FORWARD,
  BACK
};

struct pollfd* setup_readkb(char* pipe) {
  // Initialize debugging log file
  #if DEBUG == 1
    FILE* pDebugLogFile;
    pDebugLogFile = fopen ("/tmp/read-kb-debug-log.txt", "w");
  #endif

  // Get file descriptors for the poll command
  struct pollfd *pfds;
  pfds = static_cast<pollfd*>(calloc(1, sizeof(struct pollfd)));
  if (pfds == NULL) {
    errorExit("malloc");
  }

  // Open each file, and add it to 'pfds' array
  pfds[0].fd = open(pipe, O_RDONLY);
  if (pfds[0].fd == -1) {
      errorExit("open");
  }
  printlog("Opened \"%s\" on fd %d\n", argv[1], pfds[0].fd);

  // Request poll() to scan file descriptor for data available to read (POLLIN signal)
  pfds[0].events = POLLIN;

  return pfds;
}

std::string getChar_readkb(struct pollfd* pfds) {
  int num_ready;
  std::string key_pressed;

  printlog("Polling pipe for signal or data... ");
  num_ready = poll(pfds, 1, -1);
  if (num_ready == -1) {
    errorExit("poll");
  }
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
        if (s == -1) {
          errorExit("read");
        }
        printlog("    read %zd bytes: \e[1m%.*s\e[0m\n", s, (int) s, buf);

        key_pressed.assign(buf, s-1);
      //   std::cout << "(" << key_pressed << ")" << std::flush;

      } else {
        // Process other signals (POLLERR | POLLHUP | POLLNVAL)

        printlog("    closing fd %d\n", pfds[0].fd);
        if (close(pfds[0].fd) == -1) {
          errorExit("close");
        }
        key_pressed.assign("SIGINT");
      }
    }
  }
  return key_pressed;
}

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

  // Initialize the keyboard input
  struct pollfd *pfds = setup_readkb(argv[1]);
 
  // Keep calling poll() as long as at least one file descriptor is open.
  std::string key_pressed;
  bool keep_reading = true;
  while(keep_reading) {

    key_pressed = getChar_readkb(pfds);

    switch (dictionary[key_pressed]) {
    case EXIT_CODE : // Exit Condition
        printlog("    processing exit condition and closing fd %d\n", pfds[0].fd);
        if (close(pfds[0].fd) == -1) {
          errorExit("close");
        }
        keep_reading = false;
        break;
    case HELP : // Print Usage
        std::cout << "Help" << std::endl;
        break;
    case UP :
        std::cout << "\e[A" << std::flush;
        break;
    case DOWN :
        std::cout << "\e[B" << std::flush;
        break;
    case FORWARD :
        std::cout << "\e[C" << std::flush;
        break;
    case BACK :
        std::cout << "\e[D" << std::flush;
        break;
    default :
        std::cout << "(" << key_pressed << ")" << std::flush;
    }

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
  dictionary["Right"]   = FORWARD;
  dictionary["Left"]    = BACK;

  return dictionary;
}
