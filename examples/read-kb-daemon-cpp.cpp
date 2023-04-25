/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

/* This program reads keynames on standard input and processes events
 * based on the name received.
 */

#include <string>
#include <map>
#include <iostream>
#include <poll.h>

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

  // Initialize the command map
  std::map<std::string, COMMANDS> dictionary = InitializeMap();

  // Initialize the keyboard input
  struct pollfd *pfds = setup_readkb();

  // Keep calling poll() as long as at least one file descriptor is open.
  std::string key_pressed;
  bool keep_reading = true;
  while(keep_reading) {

    key_pressed = getChar_readkb(pfds);

    switch (dictionary[key_pressed]) {
    case EXIT_CODE : // Exit Condition
        close_readkb(pfds);
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
