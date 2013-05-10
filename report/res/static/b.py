#!/usr/bin/env python2
# File: b.py
# Date: Fri Aug 31 00:14:58 2012 +0800
# Author: Yuxin Wu <ppwwyyxxc@gmail.com>
for i in [ 24, 48, 60, 72 ]:
    inf = open("raw_proc_{0}".format(i), 'r')
    lines = inf.readlines()
    for line in lines:
        units = line.split(" ");
        if units[4].strip() != "15":
            continue
        print units[0], units[2],units[4]
        out = open("proc{0}".format(i), 'a')
        out.write("{0}  {1}\n".format(units[2], units[0]))
        out.close()

