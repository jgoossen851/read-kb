#!/bin/bash

# Halts and waits for a keypress
# Echos the key name to stdout

function nameChar {
  local char=${1:-""}
  case ${char} in
    $'\x01' ) echo "SOH"        ;; # SOH
    $'\x02' ) echo "STX"        ;; # STX
    $'\x03' ) echo "ETX"        ;; # ETX
    $'\x04' ) echo "EOT"        ;; # EOT
    $'\x05' ) echo "ENQ"        ;; # ENQ
    $'\x06' ) echo "ACK"        ;; # ACK
    $'\a'   ) echo "BEL"        ;; # BEL
    $'\b'   ) echo "BS"         ;; # BS
    $'\t'   ) echo "HT"         ;; # HT
    ''      ) echo "LF"         ;; # LF
    $'\v'   ) echo "VT"         ;; # VT
    $'\f'   ) echo "FF"         ;; # FF
    $'\r'   ) echo "CR"         ;; # CR
    $'\x0e' ) echo "SO"         ;; # SO
    $'\x0f' ) echo "SI"         ;; # SI
    $'\x10' ) echo "DLE"        ;; # DLE
    $'\x11' ) echo "DC1"        ;; # DC1
    $'\x12' ) echo "DC2"        ;; # DC2
    $'\x13' ) echo "DC3"        ;; # DC3
    $'\x14' ) echo "DC4"        ;; # DC4
    $'\x15' ) echo "NAK"        ;; # NAK
    $'\x16' ) echo "SYN"        ;; # SYN
    $'\x17' ) echo "ETB"        ;; # ETB
    $'\x18' ) echo "CAN"        ;; # CAN
    $'\x19' ) echo "EM"         ;; # EM
    $'\x1a' ) echo "SUB"        ;; # SUB
    $'\x1b' ) nameEscSeq        ;; # ESC
    $'\x1c' ) echo "FS"         ;; # FS
    $'\x1d' ) echo "GS"         ;; # GS
    $'\x1e' ) echo "RS"         ;; # RS
    $'\x1f' ) echo "US"         ;; # US
    $'\x7f' ) echo "DEL"        ;; # DEL
    *       ) echo "${char}"    ;; # Printable
  esac
}

function nameEscSeq {
  local MODE
  read -rsn1 -t 0.01 MODE # Get 1 more character, if available
  case ${MODE} in
    '')   echo "ESC" ;;
    '[')                # SGR Sequences
          read -rst 0.01 MODE # Get remaining characters, if available
          case ${MODE} in
            '')       echo "Alt-[" ;; # Sequence Introducer
            'A')      echo "Up" ;;
            'B')      echo "Down" ;;
            'C')      echo "Right" ;;
            'D')      echo "Left" ;;
            'E')      echo "NumpadCenter" ;;
            'F')      echo "End" ;;
            'H')      echo "Home" ;;
            'Z')      echo "Shft-Tab" ;;
            '1;2A')   echo "Shft-Up" ;;
            '1;2B')   echo "Shft-Down" ;;
            '1;2C')   echo "Shft-Right" ;;
            '1;2D')   echo "Shft-Left" ;;
            '1;2E')   echo "Shft-NumpadCenter" ;;
            '1;2P')   echo "Shft-F1" ;;
            '1;2Q')   echo "Shft-F2" ;;
            '1;2R')   echo "Shft-F3" ;;
            '1;2S')   echo "Shft-F4" ;;
            '1;3A')   echo "Alt-Up" ;;
            '1;3B')   echo "Alt-Down" ;;
            '1;3C')   echo "Alt-Right" ;;
            '1;3D')   echo "Alt-Left" ;;
            '1;3E')   echo "Alt-NumpadCenter" ;;
            '1;3F')   echo "Alt-End" ;;
            '1;3H')   echo "Alt-Home" ;;
            '1;4A')   echo "Shft-Alt-Up" ;;
            '1;4B')   echo "Shft-Alt-Down" ;;
            '1;4C')   echo "Shft-Alt-Right" ;;
            '1;4D')   echo "Shft-Alt-Left" ;;
            '1;4E')   echo "Shft-Alt-NumpadCenter" ;;
            '1;4P')   echo "Shft-Alt-F1" ;;
            '1;4Q')   echo "Shft-Alt-F2" ;;
            '1;4R')   echo "Shft-Alt-F3" ;;
            '1;4S')   echo "Shft-Alt-F4" ;;
            '1;5A')   echo "Ctrl-Up" ;;
            '1;5B')   echo "Ctrl-Down" ;;
            '1;5C')   echo "Ctrl-Right" ;;
            '1;5D')   echo "Ctrl-Left" ;;
            '1;5E')   echo "Ctrl-NumpadCenter" ;;
            '1;5F')   echo "Ctrl-End" ;;
            '1;5H')   echo "Ctrl-Home" ;;
            '1;6C')   echo "Shft-Ctrl-Right" ;;
            '1;6D')   echo "Shft-Ctrl-Left" ;;
            '1;6E')   echo "Shft-Ctrl-NumpadCenter" ;;
            '1;6P')   echo "Shft-Ctrl-F1";;
            '1;6Q')   echo "Shft-Ctrl-F2" ;;
            '1;6R')   echo "Shft-Ctrl-F3" ;;
            '1;6S')   echo "Shft-Ctrl-F4" ;;
            '1;8C')   echo "Shft-Ctrl-Alt-Right" ;;
            '1;8D')   echo "Shft-Ctrl-Alt-Left" ;;
            '1;8E')   echo "Shft-Ctrl-Alt-NumpadCenter" ;;
            '1;8P')   echo "Shft-Ctrl-Alt-F1" ;;
            '1;8Q')   echo "Shft-Ctrl-Alt-F2" ;;
            '1;8R')   echo "Shft-Ctrl-Alt-F3" ;;
            '1;8S')   echo "Shft-Ctrl-Alt-F4" ;;
            '2~')     echo "Insert" ;;
            '3~')     echo "Delete" ;;
            '3;2~')   echo "Shft-Delete" ;;
            '3;4~')   echo "Shft-Alt-Delete";;
            '3;5~')   echo "Ctrl-Delete" ;;
            '3;6~')   echo "Shft-Ctrl-Delete" ;;
            '3;8~')   echo "Shft-Ctrl-Alt-Delete" ;;
            '5~')     echo "PgUp" ;;
            '5;3~')   echo "Alt-PgUp" ;;
            '5;5~')   echo "Ctrl-PgUp" ;;
            '5;7~')   echo "Ctrl-Alt-PgUp" ;;
            '6~')     echo "PgDown" ;;
            '6;3~')   echo "Alt-PgDown" ;;
            '6;5~')   echo "Ctrl-PgDown" ;;
            '6;7~')   echo "Ctrl-Alt-PgDown" ;;
            '15~')    echo "F5" ;;
            '15;2~')  echo "Shft-F5" ;;
            '15;3~')  echo "Alt-F5" ;;
            '15;4~')  echo "Shft-Alt-F5" ;;
            '15;6~')  echo "Shft-Ctrl-F5" ;;
            '15;8~')  echo "Shft-Ctrl-Alt-F5" ;;
            '17~')    echo "F6" ;;
            '17;2~')  echo "Shft-F6" ;;
            '17;4~')  echo "Shft-Alt-F6" ;;
            '17;6~')  echo "Shft-Ctrl-F6" ;;
            '17;8~')  echo "Shft-Ctrl-Alt-F6" ;;
            '18~')    echo "F7" ;;
            '18;2~')  echo "Shft-F7" ;;
            '18;4~')  echo "Shft-Alt-F7" ;;
            '18;6~')  echo "Shft-Ctrl-F7" ;;
            '18;8~')  echo "Shft-Ctrl-Alt-F7" ;;
            '19~')    echo "F8" ;;
            '19;2~')  echo "Shft-F8" ;;
            '19;4~')  echo "Shft-Alt-F8" ;;
            '19;6~')  echo "Shft-Ctrl-F8" ;;
            '19;8~')  echo "Shft-Ctrl-Alt-F8" ;;
            '20~')    echo "F9" ;;
            '20;2~')  echo "Shft-F9" ;;
            '20;4~')  echo "Shft-Alt-F9" ;;
            '20;6~')  echo "Shft-Ctrl-F9" ;;
            '20;8~')  echo "Shft-Ctrl-Alt-F9" ;;
            '23;2~')  echo "Shft-F11" ;;
            '23;4~')  echo "Shft-Alt-F11" ;;
            '23;6~')  echo "Shft-Ctrl-F11" ;;
            '23;8~')  echo "Shft-Ctrl-Alt-F11" ;;
            '24~')    echo "F12" ;;
            '24;2~')  echo "Shft-F12" ;;
            '24;4~')  echo "Shft-Alt-F12" ;;
            '24;6~')  echo "Shft-Ctrl-F12" ;;
            '24;8~')  echo "Shft-Ctrl-Alt-F12" ;;
            *)        MODE="\e[${MODE}" ;;
          esac
          ;;
    'O')                # Other sequences
          read -rs -t 0.01 MODE # Get remaining characters, if available
          case ${MODE} in
            '')       echo "Alt-O" ;; # Sequence Introducer
            'Q')      echo "F2" ;;
            'R')      echo "F3" ;;
            'S')      echo "F4" ;;
            *)        MODE="\eO${MODE}" ;;
          esac
          ;;
    '{')                # Sequences By Name
          read -rs -t 0.01 MODE # Get remaining characters, if available
          if [[ "${MODE}" = "" ]]; then
            echo "Alt-{"
          else
            echo "${MODE}"
          fi
          ;;     
    *)                  # Alt Keys
          echo "Alt-$(nameChar ${MODE})"
          ;;
  esac
}

# change IFS for distinguishing " ", "\t" from "\n"
IFS= read -rsn1 MODE # Get 1 character
nameChar "${MODE}"
