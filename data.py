#!/usr/bin/env python
import random

f = open('input', 'w')
n = 10000;
f.write(str(n) + '\n')
for i in range(0, n):
	j = random.randint(3, 15)
	s = str(j) + ' '
	for k in range(0, j):
		s += str(random.randint(0, 20)) + ' '
	f.write(s + '\n')
