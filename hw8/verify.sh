#!/bin/bash
e=0; [[ $(sha512sum errcodes.h | cut -d " " -f 1) == "11c19de9575cfccd0dc5aea9a04f872970ca93a320a4214922c44ac7cfea28bfe5c114e4645d4a8ad799681fed95d6e3397f208292634778a6776d0f1f44d686" ]] || { printf '*****ERROR: errcodes.h was modified! Undo your changes or re-download the homework!*****\n'>&2; e=1; }; exit $e
