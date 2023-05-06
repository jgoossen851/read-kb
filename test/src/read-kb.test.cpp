/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

#include "libread-kb.h"

#include <iostream>
#include <string>

#define ANSI_RED "\033[31m"
#define ANSI_GRN "\033[32m"
#define ANSI_RST "\033[0m"



int main() {

  // Initialize exit status
  int st = EXIT_SUCCESS;

  // Test that Modifiers in a switch compile
  ReadKB::Combo key = ReadKB::Key::UNDEFINED;
  switch(key) {
    case static_cast<uint>(ReadKB::Key::Comma): break;
    // case ReadKB::Key::Period: break; /// @todo Remove static_cast from end usage
    case ReadKB::Key::Exclamation & ReadKB::Mod::Shft: break;
    // case ReadKB::Mod::Shft & ReadKB::Key::Equal: break; /// @todo Allow key to be placed last
    case ReadKB::Key::Question & ReadKB::Mod::Ctrl & ReadKB::Mod::Alt : break;
    default: break;
  }

  // Display Test Status
  std::cout << (st ? ANSI_RED : ANSI_GRN)
            << std::string(15, '#')
            << " Tests " << (st ? "Failed!" : "Passed!") << " "
            << std::string(15, '#')
            << ANSI_RST << std::endl;

  return st;
}
