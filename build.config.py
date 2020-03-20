import os

# use these flags for very strict C
cflags = [ "-Wall", "-Wextra" ]
lflags = [ "-lm" ]
include = [ "lib/dyad/src" ]
source = [ "lib/dyad/src" ]

if "client" in opt:
    include += [ "lib/microui/src" ]
    source += [ "src/Control-Panel", "lib/microui/src" ]
    output = "robert-control-panel"
    pass
if "robot" in opt:
    lflags += [ "-lwiringPi", "-lncurses" ]
    source += [ "src/Robot" ]
    output = "robert"
    pass

if "sanitize" in opt:
    log("address sanitizer enabled")
    cflags += [ "-fsanitize=address" ]
    lflags += [ "-fsanitize=address" ]
