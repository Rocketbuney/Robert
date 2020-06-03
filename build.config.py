import os
import sys

cflags = [ "-Wall", "-Wextra", "-Wno-deprecated", "-std=c99" ]
lflags = [ "-lm", "-lpthread" ]
output = "robert_robot"
source = [ "Robert/src/" ]
