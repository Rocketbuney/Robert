SYNTAX 	= 0
DEBUG 	= 0
CC			= arm-none-eabi-gcc
CFLAGS  = -pedantic \
-Wall  \
-s -Os \
-ffunction-sections \
-fdata-sections \
-nostdlib -nostartfiles \
-Wl,--gc-sections

ifeq ($(SYNTAX), 1)
	CFLAGS  = -fsyntax-only -pedantic -Wall -nostdlib -nostartfiles
else ifeq ($(DEBUG), 1)
	CFLAGS = -ggdb -nostdlib -nostartfiles
endif

default: robert

upload: clean
		sshpass -p "robert" scp -r [!.]* pi@robert.local:/home/pi/Robert
ifeq ($(DEBUG), 1)
		sshpass -p "robert" ssh pi@robert.local "cd Robert && ls && make CC=gcc DEBUG=1"
else
		sshpass -p "robert" ssh pi@robert.local "cd Robert && ls && make CC=gcc"
endif

robert:  main.o start.o sys.o
ifeq ($(DEBUG), 0)
		strip --strip-all robert
endif
ifeq ($(SYNTAX), 0)
		$(CC) $(CFLAGS) -o robert main.o start.o sys.o
endif

main.o: main.c Modules/sys.h
	$(CC) $(CFLAGS) -c main.c

start.o: start.s
	$(CC) $(CFLAGS) -c start.s

sys.o: Modules/sys.c Modules/sys.h
		$(CC) $(CFLAGS) -c Modules/sys.c

clean:
	$(RM) count *.o *~
