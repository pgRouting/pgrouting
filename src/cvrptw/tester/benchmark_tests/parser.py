#!/usr/bin/python
# -*- coding: utf-8 -*-

import math


fp=open('input.txt','r')
rt=open('output.txt','w')

text=fp.readlines()



for i in range(1,len(text)):
	for j in range(1,len(text)):
		if(i==j):
			continue
		else:
		 	str1=text[i].split()
		 	str2=text[j].split()
		 	x1=float(str1[1])
		 	x2=float(str2[1])
		 	y1=float(str1[2])
		 	y2=float(str2[2])
		 	cost=math.sqrt((x2-x1)**(2) +(y2-y1)**2)
		 	temp=str(i)+" "+str(j)+" "+str(cost)+"\n"
		 	rt.write(temp)


