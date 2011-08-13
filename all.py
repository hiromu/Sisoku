#!/usr/bin/env python
import os

for i in range(1, 10):
	for j in range(1, 10):
		for k in range(1, 10):
			for l in range(1, 10):
				m = os.system("echo '4 %d %d %d %d' | ./main.out" % (i, j, k, l));
