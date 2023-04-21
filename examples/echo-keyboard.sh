#!/bin/bash

echo "Press 'Ctrl-C' to quit."

while true; do
  echo "$(read-kb 2>/dev/null)"
done
