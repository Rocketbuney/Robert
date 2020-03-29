import os

# use these flags for very strict C
cflags = [ "-Wall", "-Wextra", "-Wno-deprecated", "-std=c99" ]
lflags = [ "-lm", "-pthread" ]
include = [ "lib/dyad/src" ]
source = [ "lib/dyad/src", "src" ]

if "client" in opt:
    cflags += [ "-DCONTROL" ]
    lflags += [ "-F/Library/Frameworks -framework SDL2", "-framework OpenGL" ]
    include += [ "lib/microui/src" ]
    source += ["lib/microui/src" ]
    output = "robert-control-panel"

if "robot" in opt:
    cflags += [ "-DROBOT" ]
    lflags += [ "-lwiringPi", "-lncurses" ]
    source += [ "src/Robot" ]
    output = "robert"
