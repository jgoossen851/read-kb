#!/bin/bash

# This script is run by example.sh and is not intended to be run independetly

INPUT_PIPE="${1}"

# Read from input pipe
tail -f "${INPUT_PIPE}" | while read KEY_NAME; do

  case "${KEY_NAME}" in
    'Down')     echo D ;;
    'Up')       echo U ;;
    'Left')     echo L ;;
    'Right')    echo R ;;
    'EOT')      echo End ;;
    *)          echo -n "<${KEY_NAME}>";;
  esac

done
