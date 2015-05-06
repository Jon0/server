#!/usr/bin/python3

import os, subprocess
# save current dir
start_path = subprocess.check_output(['pwd']).decode('ascii').split('\n')[0]

# navigate to the src directory
f = open('src_path', 'r')
src_path = f.read().split('\n')[0]
os.chdir(src_path)

# use git to pull updates
str = subprocess.call(['git', 'pull'])

# return to build directory and make
os.chdir(start_path)
str = subprocess.call(['make'])
