#!/bin/bash

# This script is run by example.sh and is not intended to be run independetly

INPUT_PIPE="${1}"

# Read from input pipe
while read KEY_NAME; do

  case "${KEY_NAME}" in
    # Define an exit command to receive through the pipe
    'SIGINT')   exit ;;
    # Process output of read-kb program
    'h')        echo "Help" ;;
    'Up')       echo -en "\e[A" ;;
    'Down')     echo -en "\e[B" ;;
    'Right')    echo -en "\e[C" ;;
    'Left')     echo -en "\e[D" ;;
    *)          echo -n "<${KEY_NAME}>";;
  esac

done < "${INPUT_PIPE}"
