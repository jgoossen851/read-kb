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

  std::fstream fs;
  std::string input;
  fs.open(argv[1], std::fstream::in);
  while(std::getline(fs, input)) { // quit the program with Ctrl+D (EOF) or Ctrl+C (SIGINT)

    switch (dictionary[input]) {
    case EXIT_CODE : // Exit Condition
        fs.close();
        return 0;
        break;
    case HELP : // Print Usage
        std::cout << "Help" << std::endl;
        break;
    case UP :
        std::cout << "^" << std::endl;
        break;
    case DOWN :
        std::cout << "v" << std::endl;
        break;
    default :
        std::cout << "(" << input << ")" << std::endl; 
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

  return dictionary;
}
