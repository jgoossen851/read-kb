#!/bin/bash

# Read keynames on standard input and process events

while true; do
  # Read from keyboard
  KEY_NAME="$(read-kb)"
  case "${KEY_NAME}" in
    # Process output of read-kb program
    'Esc')      exit ;;
    'x' | 'X')  exit ;;
    'h')        echo "Help" ;;
    'Up')       echo -en "\e[A" ;;
    'Down')     echo -en "\e[B" ;;
    'Right')    echo -en "\e[C" ;;
    'Left')     echo -en "\e[D" ;;
    *)          echo -n "<${KEY_NAME}>";;
  esac

done
