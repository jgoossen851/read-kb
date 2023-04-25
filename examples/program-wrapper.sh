#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
TEMP_DIR="/tmp/read-kb"
BASH_DAEMON="read-kb-daemon-bash.sh"
CPP_DAEMON="../build/examples/read-kb-daemon-cpp"
DAEMON="${CPP_DAEMON}"


cleanup() {
  # Send command through pipe to shutdown
  echo 'SIGINT' > "${TEMP_DIR}/ipipe"

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
  KEY="$(read-kb 2>/dev/null)"
  echo "${KEY}" > "${TEMP_DIR}/ipipe"
done
