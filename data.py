#!/usr/bin/env python
import random

f = open('input', 'w')
for i in range(0, 10):
	j = random.randint(1, 21)
	s = str(j) + ' '
	for k in range(0, j):
		s += str(random.randint(0, 100)) + ' '
	f.write(s + '\n')
