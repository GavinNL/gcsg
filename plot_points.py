#!/bin/env python3
import numpy as np
import pylab as pl
import sys
from matplotlib import collections  as mc


array = []
lines = []
pointsx = []
pointsy = []

with open(sys.argv[1]) as f:

    for line in f: # read rest of lines
       
        array = [float(x) for x in line.split()]
        lines.append( [ (array[0], array[1]), (array[2], array[3])   ])
        pointsx.append( array[0] )
        pointsx.append( array[2] )
        pointsy.append( array[1] )
        pointsy.append( array[3] )

lc = mc.LineCollection(lines, linewidths=2)
fig, ax = pl.subplots()
ax.add_collection(lc)
ax.autoscale()
ax.margins(0.1)
pl.xlabel('x')
pl.ylabel('y')

pl.plot(pointsx, pointsy, 'o')
pl.show()

