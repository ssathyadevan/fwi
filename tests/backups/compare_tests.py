#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

if (len(sys.argv) != 3):
    print " please supply two arguments after python compare_tests.py\n \
the first argument the benchmark, the second argument the new test"
    sys.exit() # Back to the terminal, we are done
else:
    print "You supplied two arguments, well done"
    sys.exit()
