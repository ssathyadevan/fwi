#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys, os, filecmp

if (len(sys.argv) != 3):
    print " please supply two arguments after python compare_tests.py\n \
the first argument the benchmark, the second argument the new test"
    sys.exit() # Back to the terminal, we are done
else:
    print " You supplied two arguments, well done"
    print ""
    print " Your benchmark: " + sys.argv[1]
    print " Your new test:  " + sys.argv[2]
    bench   = sys.argv[1]
    new     = sys.argv[2]
    benchin = sys.argv[1]+".in"
    newin   = sys.argv[2]+".in"
shouldexist1 = os.path.isfile(benchin)#sys.argv[1] + ".in")
shouldexist2 = os.path.isfile(newin)#sys.argv[2] + ".in")
print ""

if shouldexist1:
    print " Your benchmark file exists"
else:
    print " Your benchmark does not exist"
    sys.exit()
print ""

if shouldexist2:
    print " Your new test also exists"
else:
    print " You lack a new test to compare"
    sys.exit()
print ""

if (filecmp.cmp(benchin,newin)):
    print " Your benchmark and test parametrization are identical, but your reservoirs could be different"
    inputfilediff = False
else:
    print " There is a difference between how you parametrized the input, your reservoirs could be identical"
    inputfilediff = True
print ""

chi_ref_bench_txt = "chi_ref_" + bench + ".txt"
chi_ref_new_txt   = "chi_ref_" + new   + ".txt"
if (filecmp.cmp(chi_ref_bench_txt, chi_ref_new_txt)):
    print " You used the same reservoir in both instances !"
    reservoirdiff = False
else:
    print " You used different reservoirs"
    reservoirdiff = True
print ""
chi_est_bench_txt = "chi_est_" + bench + ".txt"
chi_est_new_txt =   "chi_est_" + new   + ".txt"
if (filecmp.cmp(chi_est_bench_txt, chi_est_new_txt)):
    print " It appears that you have the same output in terms of chi, so I exit, \n \
although this part of the code is extended later "
    sys.exit()
else:
    print "You have different output in terms of chi, we will study them in more detail"

#print filecmp.cmp("hi.in","hi2.in")





