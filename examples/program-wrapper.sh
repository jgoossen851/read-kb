#!/bin/bash

# This script gives an example of the designed use of the read-kb suite.
# As keyboard input is Operating System dependent, C++ only reads lines
# of input by default. The `read-kb` program is implemented as a shell
# script that reads one charater of input at a time using the system's
# `read` function and assigns a name to the key that was pressed. All
# key names are sequences of printable ASCII characters, even for modified
# keys or unprintable control keys (e.g., Shft-Ctrl-F2 or EOT)
#
# To run a program that can respond to individual keypresses like arrow
# keys or other unprintable characters, the program should be run as a
# daemon process managed by a shell wrapper funtion like this one.
# To run the program, run the wrapper script, similar to this one, which
# will:
#   - Make a named FIFO pipe for sending data to the daemon program
#   - Start the daemon program in the background with the above pipe
#     redirected to the daemon's standard input
#   - Open the pipe for writing and keep the pipe open until the wrapper
#     terminates
#   - Translate keyboard key-presses into printable names which are sent
#     to the daemon program over the pipe.
#
# This wrapper program works for both Bash and C++ daemon programs.
# The library may have limited (non-existent?) portability to non-Posix
# systems.

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
TEMP_DIR="/tmp/read-kb"
BASH_DAEMON="read-kb-daemon-bash.sh"
CPP_DAEMON="../build/examples/read-kb-daemon-cpp"
DAEMON="${CPP_DAEMON}"


cleanup() {
  # Send command through pipe to shutdown
  echo 'SIGTERM' > "${TEMP_DIR}/ipipe"

  # Close pipe (stop redirecting fd 3 to named fifo pipe)
  exec 3>&-

  # Delete all temp files
  rm -rf "${TEMP_DIR}"

  exit
}
# Run cleanup on any exit
trap 'cleanup' EXIT

echo "Press 'Ctrl-C' to quit."


# Create a pipe for input to the daemon
mkdir -p "${TEMP_DIR}"
if [ ! -e "${TEMP_DIR}/ipipe" ]; then
  mkfifo "${TEMP_DIR}/ipipe"
fi


# Start the daemon in the background with input pipe to read
if [ ! -f "${SCRIPT_DIR}/${DAEMON}" ]; then
  echo "${SCRIPT_DIR}/${DAEMON}" could not be found
  exit
fi
# Daemon will halt when trying to read from pipe until pipe is also opened for writing
"${SCRIPT_DIR}/${DAEMON}" < "${TEMP_DIR}/ipipe" &


# Open pipe for writing (redirect fd 3 to named fifo pipe)
echo -n "Opening pipe for writing. Waiting for daemon to attach... "
# This program will halt until the daemon also opens pipe for reading
exec 3> "${TEMP_DIR}/ipipe"
echo "Done."


while true; do
  # Send keyboard events to the pipe
  read-kb 2> /dev/null 1> "${TEMP_DIR}/ipipe"
done
