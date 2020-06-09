#!/bin/sh

gcc \
  -pedantic \
  -Wall -Werror \
  -s -Os \
  -nostdlib \
  -ffreestanding \
  -fno-stack-protector \
  -fdata-sections \
  -ffunction-sections \
  -fno-unwind-tables \
  -fno-asynchronous-unwind-tables \
  -Wl,-n \
  -Wl,--gc-sections \
  -Wl,--build-id=none \
   main.c start.s \
  -o robert &&
strip -R .comment robert
