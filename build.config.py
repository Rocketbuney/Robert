import os
import sys

cflags = [ "-Wall", "-Wextra", "-Wno-deprecated" ]
lflags = [ "-lm", "-lpthread" ]
output = "robert_robot"
source = [ "src/" ]

def post():
	if "upload" in opt:
		print('Uploading....')
		os.system('sudo sshpass -p "robert" scp -r * pi@robert.local:/home/pi/')

