#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
TEMP_DIR="/tmp/read-kb"
DAEMON_SCRIPT="daemon.sh"

echo "Press 'Ctrl-C' to quit."
# Define cleanup routine
cleanup() {
  echo "SIGINT caught"

  echo -n "Removing named pipe ..."
  rm -rf "${TEMP_DIR}"
  echo "Done."

  echo "Killing background daemon processes ..."
  for ind in $(pgrep "${DAEMON_SCRIPT}"); do
    ps $ind
    sudo kill -9 $ind
  done
  echo "Done."

  exit
}
trap 'cleanup' SIGINT

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
