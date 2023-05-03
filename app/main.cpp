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

int main() {

  // Initialize the command map and keyboard input
  auto dictionary = InitializeMap();
  ReadKB kb;

  // Keep calling poll() as long as at least one file descriptor is open.
  bool keep_reading = true;
  while(keep_reading) {

    std::string key_pressed = kb.getKey();
    switch (dictionary[key_pressed]) {
      case EXIT_CODE : // Exit Condition
        keep_reading = false;
        break;
      case HELP : // Print Usage
        std::cout << "Help" << std::endl;
        break;
      case UP :       std::cout << "\033[A" << std::flush;  break;
      case DOWN :     std::cout << "\033[B" << std::flush;  break;
      case FORWARD :  std::cout << "\033[C" << std::flush;  break;
      case BACK :     std::cout << "\033[D" << std::flush;  break;
      default :
        std::cout << "(" << key_pressed << ")" << std::flush;
    }
  }

  return 0;
}

std::map<std::string, COMMANDS> InitializeMap(){
  
  std::map< std::string, COMMANDS > dictionary;

  // Define user commands
  dictionary["SIGTERM"] = EXIT_CODE;
  dictionary["x"]       = EXIT_CODE;
  dictionary["X"]       = EXIT_CODE;
  dictionary["h"]       = HELP;
  dictionary["Up"]      = UP;
  dictionary["Down"]    = DOWN;
  dictionary["Right"]   = FORWARD;
  dictionary["Left"]    = BACK;

  return dictionary;
}
