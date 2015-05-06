#!/usr/bin/python3

import time, subprocess

#
time.sleep(30)

# update and run
subprocess.call(['git-update'])
subprocess.call(['ServerMain', '80'])
