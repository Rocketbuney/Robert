SYNTAX 	= 0
DEBUG 	= 0
CC			= arm-none-eabi-gcc
CFLAGS  = -pedantic \
-Wall -Werror \
-s -Os \
-ffunction-sections \
-fdata-sections \
-nostdlib -nostartfiles \
-Wl,--gc-sections

ifeq ($(SYNTAX), 1)
	CFLAGS  = -fsyntax-only -pedantic
else ifeq ($(DEBUG), 1)
	CFLAGS += -ggdb
endif

default: robert

upload: clean
		sshpass -p "robert" scp -r [!.]* pi@robert.local:/home/pi/Robert
ifeq ($(DEBUG), 1)
		sshpass -p "robert" ssh pi@robert.local "cd Robert && ls && make CC=gcc"
else
		sshpass -p "robert" ssh pi@robert.local "cd Robert && ls && make CC=gcc DEBUG=1"
endif

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
