#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
TEMP_DIR="/tmp/read-kb"
BASH_DAEMON="daemon.sh"
CPP_DAEMON="../build/examples/read-kb-example-daemon"


cleanup() {
  # Send command through pipe to shutdown
  echo 'SIGINT' > "${TEMP_DIR}/ipipe_bash"
  # echo 'SIGINT' > "${TEMP_DIR}/ipipe_cpp"

  # Delete all temp files
  rm -rf "${TEMP_DIR}"

  exit
}
# Run cleanup on any exit
trap 'cleanup' EXIT

echo "Press 'Ctrl-C' to quit."


# Create a pipe for input to the daemon
mkdir -p "${TEMP_DIR}"
if [ ! -e "${TEMP_DIR}/ipipe_bash" ]; then
  mkfifo "${TEMP_DIR}/ipipe_bash"
fi
if [ ! -e "${TEMP_DIR}/ipipe_cpp" ]; then
  mkfifo "${TEMP_DIR}/ipipe_cpp"
fi


# Start the daemon in the background with input pipe to read
if [ ! -f "${SCRIPT_DIR}/${BASH_DAEMON}" ]; then
  echo "${SCRIPT_DIR}/${BASH_DAEMON}" could not be found
  exit
fi
"${SCRIPT_DIR}/${BASH_DAEMON}" "${TEMP_DIR}/ipipe_bash" &

if [ ! -f "${SCRIPT_DIR}/${CPP_DAEMON}" ]; then
  echo "${SCRIPT_DIR}/${CPP_DAEMON}" could not be found
  exit
fi
# "${SCRIPT_DIR}/${CPP_DAEMON}"  "${TEMP_DIR}/ipipe_cpp" &
"${SCRIPT_DIR}/${CPP_DAEMON}"  "text.txt" &


while true; do
  # Send keyboard events to the pipe
  KEY="$(read-kb 2>/dev/null)"
  echo "${KEY}" > "${TEMP_DIR}/ipipe_bash"
  # echo "${KEY}" > "${TEMP_DIR}/ipipe_cpp"
done
