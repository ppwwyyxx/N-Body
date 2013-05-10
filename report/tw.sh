#!/bin/bash
# File: tw.sh
# Date: Wed Aug 15 22:48:08 2012 +0800
# Author: Yuxin Wu <ppwwyyxxc@gmail.com>
file=`ls *.tex`
for i in $file; do
	echo $i
	opencc -i $i -o ../report_tw/$i -c zhs2zhtw_vp.ini
done
