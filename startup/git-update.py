#!/usr/bin/python3

import sys, os, subprocess
# save current dir
start_path = ""
if len(sys.argv) > 1:
	start_path = sys.argv[1]
else:
	start_path = subprocess.check_output(['pwd']).decode('ascii').split('\n')[0]

# find the src path from a file called 'src_path'
os.chdir(start_path)
f = open('src_path', 'r')
src_path = f.read().split('\n')[0]

# navigate to the src directory
os.chdir(src_path)

# use git to pull updates
str = subprocess.call(['git', 'pull'])

# return to build directory and make
os.chdir(start_path)
str = subprocess.call(['make'])
