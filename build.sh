#!/bin/sh

gcc \
  -pedantic \
  -Wall -Werror \
  -s -Os \
  -ffunction-sections \
  -fdata-sections \
  -nostdlib -nostartfiles \
  -Wl,--gc-sections \
   main.c start.s \
  -o robert &&
strip --strip-all robert
