import os
import sys

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

if "upload" in opt:
    os.system("scp -r `pwd` pi@10.0.0.147:/home/pi/")
    sys.exit()
