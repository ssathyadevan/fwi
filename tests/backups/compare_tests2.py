#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, filecmp

if (len(sys.argv) != 3):
    print " please supply two arguments after python compare_tests.py\n \
the first argument the benchmark, the second argument the new test"
    sys.exit() # Back to the terminal, we are done
else:
    print " You supplied two arguments, well done"
    print " Your benchmark: " + sys.argv[1]
    print " Your new test:  " + sys.argv[2]
    benchin = sys.argv[1]+".in"
    newin   = sys.argv[2]+".in"
shouldexist1 = os.path.isfile(benchin)#sys.argv[1] + ".in")
shouldexist2 = os.path.isfile(newin)#sys.argv[2] + ".in")
if shouldexist1:
    print " Your benchmark file exists"
else:
    print " Your benchmark does not exist"
    sys.exit()
if shouldexist2:
    print " Your new test also exists"
else:
    print " You lack a new test to compare"

if (filecmp.cmp(benchin,newin)):
    print " Your benchmark and test input are identical"
    inputfilediff = False
else:
    print " There is a difference between how you generated the input"
    inputfilediff = True


#print filecmp.cmp("hi.in","hi2.in")
