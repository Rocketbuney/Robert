SYNTAX 	= 0
CC			= arm-none-eabi-gcc

ifeq ($(SYNTAX), 1)
	CFLAGS  = -fsyntax-only
else
	CFLAGS  = -pedantic \
  -Wall -Werror \
  -s -Os \
  -ffunction-sections \
  -fdata-sections \
  -nostdlib -nostartfiles \
  -Wl,--gc-sections
endif

default: robert

upload: clean
		sshpass -p "robert" scp -r [!.]* pi@robert.local:/home/pi/Robert
		sshpass -p "robert" ssh pi@robert.local "cd Robert && ls && make CC=gcc"

robert:  main.o start.o
ifeq ($(SYNTAX), 0)
		$(CC) $(CFLAGS) -o robert main.o start.o
		strip --strip-all robert
endif

main.o: main.c sys.h
	$(CC) $(CFLAGS) -c main.c

start.o: start.s
	$(CC) $(CFLAGS) -c start.s

clean:
	$(RM) count *.o *~
