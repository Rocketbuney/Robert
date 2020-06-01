import os
import sys

cflags = [ "-Wall", "-Wextra", "-Wno-deprecated", "-std=c++17" ]
lflags = [ "-lm", "-lstdc++", "-lwiringPi" ]
output = "robert_robot"
source = [ "Robert/" ]
