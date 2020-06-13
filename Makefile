SYNTAX 	= 0
DEBUG 	= 0
CC			= arm-none-eabi-gcc
CFLAGS  = -s -Os \
-fno-builtin \
-ffreestanding \
-ffunction-sections \
-fdata-sections \
-nostdlib -nostartfiles \
-Wl,--gc-sections
TARGET = robert

ifeq ($(SYNTAX), 1)
	CFLAGS  = -fno-builtin -ffreestanding -fsyntax-only -nostdlib -nostartfiles
else ifeq ($(DEBUG), 1)
	CFLAGS = -ggdb -fno-builtin -ffreestanding -nostdlib -nostartfiles
	TARGET = robert-d
endif

default: $(TARGET)

upload: clean
		sshpass -p "robert" scp -r [!.]* pi@robert.local:/home/pi/Robert
ifeq ($(DEBUG), 1)
		sshpass -p "robert" ssh pi@robert.local "cd Robert && ls && make CC=gcc DEBUG=1"
else
		sshpass -p "robert" ssh pi@robert.local "cd Robert && ls && make CC=gcc"
endif

$(TARGET):  main.o start.o sys.o
ifeq ($(SYNTAX), 0)
		$(CC) $(CFLAGS) -o $(TARGET) main.o start.o sys.o
endif
ifeq ($(DEBUG), 0)
		strip --strip-all robert
endif

main.o: main.c Modules/sys.h
	$(CC) $(CFLAGS) -c main.c

start.o: start.s
	$(CC) $(CFLAGS) -c start.s

sys.o: Modules/sys.c Modules/sys.h
		$(CC) $(CFLAGS) -c Modules/sys.c

clean:
	$(RM) count *.o *~
