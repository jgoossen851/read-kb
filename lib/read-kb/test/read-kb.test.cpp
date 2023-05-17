/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2023, Jeremy Goossen jeremyg995@gmail.com
 */

#include "read-kb.h"

#include <fcntl.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <utility>
#include <vector>

#define ANSI_RED "\033[31m"
#define ANSI_GRN "\033[32m"
#define ANSI_RST "\033[0m"

#define U_LA "\u27e8" //!< Left Angle Bracket
#define U_RA "\u27e9" //!< Right Angle Bracket

// Define error-handling function
#define errorIf(cond, msg) do { if( cond ) { \
                                  perror(msg); exit(EXIT_FAILURE); \
                              }} while (0)

// Check that global namespace is not polluted
enum TestEnum {
  Enter,
  UNDEFINED
};

int failTest( std::string ansString,
              std::string resultString,
              std::string description ) {
  std::cout << ANSI_RED << "Test Failed: " << ANSI_RST;
  std::cout << "Should be " U_LA << ansString << U_RA " but was " U_LA << resultString
            << U_RA " : " << description << std::endl;
  return EXIT_FAILURE;
}

int testEq( const std::string &result,
            const std::string &answer,
            const std::string &description = "No description") {
  if( answer != result) {
    return failTest(answer, result, description);
  }
  return EXIT_SUCCESS;
}

int main() {

  // Initialize exit status
  int st = EXIT_SUCCESS;

  // Define desired display names for unprintable ASCII control codes
  std::map<char, std::string> asciiUnprintableDisplayNames;
  asciiUnprintableDisplayNames[0]  = "Ctrl-@";
  asciiUnprintableDisplayNames[1]  = "Ctrl-a";
  asciiUnprintableDisplayNames[2]  = "Ctrl-b";
  asciiUnprintableDisplayNames[3]  = "Ctrl-c";
  asciiUnprintableDisplayNames[4]  = "Ctrl-d";
  asciiUnprintableDisplayNames[5]  = "Ctrl-e";
  asciiUnprintableDisplayNames[6]  = "Ctrl-f";
  asciiUnprintableDisplayNames[7]  = "Ctrl-g";
  asciiUnprintableDisplayNames[8]  = "Ctrl-Bksp"; // BS
  asciiUnprintableDisplayNames[9]  = "Tab";       // HT
  asciiUnprintableDisplayNames[10] = "Enter";     // LF
  asciiUnprintableDisplayNames[11] = "Ctrl-k";
  asciiUnprintableDisplayNames[12] = "Ctrl-l";
  asciiUnprintableDisplayNames[13] = "Ctrl-m";
  asciiUnprintableDisplayNames[14] = "Ctrl-n";
  asciiUnprintableDisplayNames[15] = "Ctrl-o";
  asciiUnprintableDisplayNames[16] = "Ctrl-p";
  asciiUnprintableDisplayNames[17] = "Ctrl-q";
  asciiUnprintableDisplayNames[18] = "Ctrl-r";
  asciiUnprintableDisplayNames[19] = "Ctrl-s";
  asciiUnprintableDisplayNames[20] = "Ctrl-t";
  asciiUnprintableDisplayNames[21] = "Ctrl-u";
  asciiUnprintableDisplayNames[22] = "Ctrl-v";
  asciiUnprintableDisplayNames[23] = "Ctrl-w";
  asciiUnprintableDisplayNames[24] = "Ctrl-x";
  asciiUnprintableDisplayNames[25] = "Ctrl-y";
  asciiUnprintableDisplayNames[26] = "Ctrl-z";
  asciiUnprintableDisplayNames[27] = "Esc";       // ESC
  asciiUnprintableDisplayNames[28] = "Ctrl-\\";
  asciiUnprintableDisplayNames[29] = "Ctrl-]";
  asciiUnprintableDisplayNames[30] = "Ctrl-^";
  asciiUnprintableDisplayNames[31] = "Ctrl-_";

  // Printable ASCII chars are displayed as such after construction
  for (char cc = 0; cc >= 0; cc++) { // Loop until char overflows
    std::ostringstream os;
    os << ReadKB::Key(cc);
    std::string ans;
    if (cc < 32)        { ans = asciiUnprintableDisplayNames[cc]; } // C0 Control Codes
    else if (cc < 127)  { ans = std::string(1, cc); } // Printable ASCII
    else                { ans = "Bksp"; } // ASCII(127)=DEL
    st |= testEq(os.str(), ans, "Char Constructor");
  }

  // Test Display of Keys with Modifiers
  for (char cc = ' '; cc <= '~'; cc++) {
    std::ostringstream osCtrl;
    osCtrl << (ReadKB::Key(cc) & ReadKB::Mod::Ctrl);
    st |= testEq(osCtrl.str(), "Ctrl-" + std::string(1, cc), "Display with Ctrl Mod");

    std::ostringstream osAlt;
    osAlt << (ReadKB::Key(cc) & ReadKB::Mod::Alt);
    st |= testEq(osAlt.str(), "Alt-" + std::string(1, cc), "Display with Alt Mod");

    std::ostringstream osCtrlAlt;
    osCtrlAlt << (ReadKB::Key(cc) & ReadKB::Mod::Ctrl & ReadKB::Mod::Alt);
    st |= testEq(osCtrlAlt.str(), "Ctrl-Alt-" + std::string(1, cc), "Display with Ctrl-Alt Mod");
  }

  // Test that Modifiers in a switch compile
  ReadKB::Key key = ReadKB::Key::UNDEFINED;
  switch(key) {
    case ReadKB::Key::Comma: break;
    case ReadKB::Key::Period: break;
    case ReadKB::Key::Exclamation & ReadKB::Mod::Shft: break;
    case ReadKB::Mod::Shft & ReadKB::Key::Equal: break; /// Modifier on left
    case (ReadKB::Mod::Alt & ReadKB::Mod::Ctrl) & ReadKB::Key::Up: break; /// Combine modifiers
    case ReadKB::Key::Question & ReadKB::Mod::Ctrl & ReadKB::Mod::Alt : break;
    default: break;
  }


  // Load test data set
  std::ifstream datafile("input.txt");
  std::vector<std::pair<std::string, std::string>> data;
  errorIf(!datafile.is_open(), "open data");
  std::string line;
  while ( getline(datafile, line) ) {
    if ( line.length() == 0 || line.at(0) == ' ' ) { continue; } // Comment character
    std::pair<std::string, std::string> keyValue;
    keyValue.first  = line.substr(0, line.find(" "));   // Key
    keyValue.second = line.substr(line.rfind(" ") + 1); // Value
    data.push_back(keyValue);
  }
  datafile.close();

  // Test reading input from a file (will read to end of file, so temp file used and filled incrementally)
  char path[] = "/tmp/read-kb";
  int fd = open(path, O_RDWR | O_TMPFILE, S_IRUSR | S_IWUSR );
  errorIf(fd == -1, "open temp");

  ReadKB kb;

  // Check that input source can be switched to file and back again
  kb.setInput(fd, ReadKB::InputMode::Char);
  kb.setInput(0, ReadKB::InputMode::Char);

  // Read input from file
  kb.setInput(fd, ReadKB::InputMode::Char);
  std::ostringstream os;
  ssize_t s;
  for (auto it = data.begin(); it != data.end(); it++) {
    const char *buf = static_cast<const char*>(it->second.c_str());
    int len = it->second.length();
    errorIf((s = write(fd, buf, len)) == -1, "write");
    errorIf(s != len && (errno = EIO), "write all data");
    errorIf(lseek(fd, -s, SEEK_CUR) == -1, "lseek");
    os.str("");
    os << kb.read_key();
    st |= testEq(os.str(), it->first, "Read input from file");
  }

  // Display Test Statuses
  std::cout << (st ? ANSI_RED : ANSI_GRN)
            << std::string(15, '#')
            << " Tests " << (st ? "Failed!" : "Passed!") << " "
            << std::string(15, '#')
            << ANSI_RST << std::endl;

  return st;
}
