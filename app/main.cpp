/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

#include "read-kb.h"

#include <iostream>

int main() {
  ReadKB kb;
  std::cout << kb.read_key() << std::endl;
  return EXIT_SUCCESS;
}
