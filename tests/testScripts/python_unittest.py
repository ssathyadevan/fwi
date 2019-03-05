#!/usr/bin/python
# -*- coding: utf-8 -*-

import unittest

f=open("RegressionTest_Tolerance.txt", "r")
tolerance1 = f.read()
b=float(tolerance1)

g=open("RegressionTest_Passed.txt", "r")
passed = g.read()
print(passed)
if passed == "True":
	a=1
else:
	a=0
 

class TestRegression(unittest.TestCase):
	"""
	Test whether the Regression Tests work properly. Two test cases have been defined.
	"""
	def test_regression_preprocessing(self):
		self.assertTrue(b <= 10e-5 and b>=-10e-5)
	def test_regression_processing(self):
		self.assertEqual(1, a)

if __name__ == '__main__':
    unittest.main()
