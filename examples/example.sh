#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
TEMP_DIR="/tmp/read-kb"
DAEMON_SCRIPT="daemon.sh"

cleanup() {
  # Send command through pipe to shutdown
  echo 'SIGINT' > "${TEMP_DIR}/ipipe"

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
"${SCRIPT_DIR}/${DAEMON_SCRIPT}" "${TEMP_DIR}/ipipe" &

while true; do
  # Send keyboard events to the pipe
  echo "$(read-kb 2>/dev/null)" > "${TEMP_DIR}/ipipe"
done
