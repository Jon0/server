#!/usr/bin/python3

import subprocess

# update and run
subprocess.call(['./git-update'])
subprocess.call(['./ServerMain 80'])
