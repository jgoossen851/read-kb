/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

#include "libread-kb.h"

#include <iostream>
#include <map>
#include <string>

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
static std::map<ReadKB::Key, COMMANDS> InitializeMap();

int main() {

  // Initialize the command map and keyboard input
  auto dictionary = InitializeMap();
  ReadKB kb;

  bool keep_reading = true;
  while(keep_reading) {
    ReadKB::Key key_pressed = kb.read_key();
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

std::map<ReadKB::Key, COMMANDS> InitializeMap(){
  
  std::map< ReadKB::Key, COMMANDS > dictionary;

  // Define user commands
  dictionary[ReadKB::Key::Esc]              = EXIT_CODE;
  dictionary[static_cast<ReadKB::Key>('x')] = EXIT_CODE;
  dictionary[static_cast<ReadKB::Key>('X')] = EXIT_CODE;
  dictionary[static_cast<ReadKB::Key>('h')] = HELP;
  dictionary[ReadKB::Key::Up]               = UP;
  dictionary[ReadKB::Key::Down]             = DOWN;
  dictionary[ReadKB::Key::Right]            = FORWARD;
  dictionary[ReadKB::Key::Left]             = BACK;

  return dictionary;
}
