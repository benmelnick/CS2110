#!/bin/bash
e=0; [[ $(sha512sum list.h | cut -d " " -f 1) == 7a07f7a8c46b5b1d238b24836d954f52ffaa5612f1d2226e8ae71bfe51942f897076e7c2479be41601f5d796516a5db07b278a783875d25f05bc095f9c8e5c04 ]] || { printf 'error: list.h was modified! re-download it and try again\n'>&2; e=1; }; exit $e
