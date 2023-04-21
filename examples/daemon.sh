#!/bin/bash

# This script is run by example.sh and is not intended to be run independetly

INPUT_PIPE="${1}"

# Read from input pipe
tail -f "${INPUT_PIPE}" | while read KEY_NAME; do

  case "${KEY_NAME}" in
    # Define an exit command to receive through the pipe
    'SIGINT')   exit ;;
    # Process output of read-kb program
    'Down')     echo D ;;
    'Up')       echo U ;;
    'Left')     echo L ;;
    'Right')    echo R ;;
    *)          echo -n "<${KEY_NAME}>";;
  esac

done
