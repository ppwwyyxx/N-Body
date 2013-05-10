#!/bin/bash
# File: bat.sh
# Date: Fri Aug 31 00:14:43 2012 +0800
# Author: Yuxin Wu <ppwwyyxxc@gmail.com>

procs=(24 48 60 72)
for i in ${procs[*]}; do
	cat o_$i* |grep 'total' -C 1 | sed 's/^-.*//g' | sed 's/second.*//g' | sed 's/.*-b//g' | sed 's/-t//g' | grep -v '^$' |awk '{printf $0 ; getline; print $0 " "}' > raw_proc_$i
done
