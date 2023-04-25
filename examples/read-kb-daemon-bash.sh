#!/bin/bash

# Read keynames on standard input and process events

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

done
