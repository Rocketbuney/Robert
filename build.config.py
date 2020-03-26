import os

# use these flags for very strict C
cflags = [ "-Wall", "-Wextra", "-pedantic", "-Wno-deprecated", "-std=c99" ]
lflags = [ "-lm" ]
include = [ "lib/dyad/src" ]
source = [ "lib/dyad/src" ]

if "client" in opt:
    cflags += [ "-O3", "-g" ]
    lflags += [ "-F/Library/Frameworks -framework SDL2", "-framework OpenGL" ]
    include += [ "lib/CSFML/include", "lib/microui/src" ]
    source += [ "src/Control-Panel", "lib/microui/src" ]
    output = "robert-control-panel"

if "robot" in opt:
    lflags += [ "-lwiringPi", "-lncurses" ]
    source += [ "src/Robot" ]
    output = "robert"
