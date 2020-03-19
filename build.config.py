import os

# use these flags for very strict C
cflags = [ "-Wall", "-Wextra" ]
lflags = [ "-lwiringPi", "-lncurses", "-lm" ]
include = [ "src", "include", "lib/dyad/src" ]
source = [ "src", "lib/dyad/src" ]
output = "robert"


if "sanitize" in opt:
    log("address sanitizer enabled")
    cflags += [ "-fsanitize=address" ]
    lflags += [ "-fsanitize=address" ]
