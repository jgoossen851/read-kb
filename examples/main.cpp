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

#include "libread-kb.h"


enum COMMANDS {
  NOT_DEFINED,
  EXIT_CODE,
  HELP,
  UP,
  DOWN,
  FORWARD,
  BACK
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
