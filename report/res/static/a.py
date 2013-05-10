#!/usr/bin/env python2
# File: a.py
# Date: Fri Aug 31 01:07:12 2012 +0800
# Author: Yuxin Wu <ppwwyyxxc@gmail.com>
#data = dict(dict(dict()))
for i in [ 24, 48, 60, 72 ]:
    inf = open("raw_proc_{0}".format(i), 'r')
    lines = inf.readlines()
    for line in lines:
        units = line.split(" ");
        if units[2].strip() != "30000":
            continue
        print units[0], units[2],units[4]
        out = open("iter_{0}".format(units[4]), 'a')
        out.write("{0}  {1}\n".format(i, units[0]))
        out.close()

