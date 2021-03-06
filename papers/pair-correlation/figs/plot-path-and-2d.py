#!/usr/bin/python

from __future__ import division
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import pylab, numpy, sys, scipy.ndimage
import os.path
import math

from matplotlib.colors import NoNorm

# these are the things to set
colors = ['k', 'b', 'g', 'r']
plots = ['mc', 'this-work', 'fischer'] # , 'gloor'
titles = ['Monte Carlo', 'this work', 'Fischer et al'] # , 'gloor'
dx = 0.1
############################

able_to_read_file = True
z0 = 0.05

#Careful there may be a difference between mc and other with these
zmax = 20
rmax = 10


if len(sys.argv) != 2:
    print("Usage:  " + sys.argv[0] + " ff")
    exit(1)
ff = float(sys.argv[1])

def read_walls_path(ff,z0,fun):
  if fun == 'mc':
    filename = "figs/mc/wallsMC-pair-%1.1f-path.dat" % ff
  else:
    filename = "figs/walls/wallsWB-path-%s-pair-%1.2f-%1.2f.dat" %(fun, ff, z0)
  if (os.path.isfile(filename) == False):
    print "File does not exist:", filename
    return pylab.zeros((10,10)) # just return zeros for unknown data
  data=numpy.loadtxt(filename)
  if fun == 'mc':
    data[:,0]-=4.995
  return data[:,0:2]

def read_walls(ff, z0, fun):
  if fun == 'mc':
    filename = "figs/mc/wallsMC-pair-%1.1f-%1.2f.dat" % (ff, z0)
  else:
    filename = "figs/walls/wallsWB-%s-pair-%1.2f-%1.2f.dat" %(fun, ff, z0)
  #print 'Using', filename
  if (os.path.isfile(filename) == False):
    print "File does not exist:", filename
    x = pylab.arange(0,10,1)
    X,Y = pylab.meshgrid(x,x)
    return X # return bogus values for unknown data
  data = numpy.loadtxt(filename)
  return data

# First plot

pylab.figure(figsize=(10,5))

pylab.subplot(1,2,2)
for i in range(len(plots)):
    g2_path = read_walls_path(ff, z0, plots[i])
    if able_to_read_file == False:
        matplotlib.pyplot.plot(numpy.arange(0,10,1), [0]*10, 'k')
        matplotlib.pyplot.suptitle('!!!!WARNING!!!!! There is data missing from this plot!', fontsize=25)
        savedfilename = "figs/pair-correlation-path-" + str(int(ff*10)) + ".pdf"
        pylab.savefig(savedfilename)
        exit(0)
    pylab.plot(g2_path[:,0],g2_path[:,1], label=titles[i], color=colors[i])

g2nice = read_walls_path(ff, z0, 'this-work')
def g2pathfunction(x):
    return numpy.interp(x, g2nice[:,0], g2nice[:,1])

rA = 3.9
rE = 4.0
rpath = 2.005
xBoff = 10 - rpath
xDoff = 10 - rpath + numpy.pi*rpath/2
xAoff = xBoff - rA + rpath
xEoff = xDoff + rE - rpath
pylab.axvline(xBoff, color='k')
pylab.axvline(xDoff, color='k')

hw = 4 # headwidth of arrows

pylab.annotate('$A$', xy=(xAoff, g2pathfunction(xAoff)), xytext=(xAoff-1,1.3),
               arrowprops=dict(shrink=0.01, width=1, headwidth=hw))
pylab.annotate('$B$', xy=(xBoff,g2pathfunction(xBoff)), xytext=(xBoff-1,g2pathfunction(xBoff)-0.2),
               arrowprops=dict(shrink=0.01, width=1, headwidth=hw))
pylab.annotate('$C$', xy=(0.5*(xBoff+xDoff),g2pathfunction(0.5*(xBoff+xDoff))),
               xytext=(0.5*(xBoff+xDoff)+0.2,g2pathfunction(0.5*(xBoff+xDoff))-0.5),
               arrowprops=dict(shrink=0.01, width=1, headwidth=hw))
pylab.annotate('$D$', xy=(xDoff,g2pathfunction(xDoff)), xytext=(xDoff+1,g2pathfunction(xDoff)-0.2),
               arrowprops=dict(shrink=0.01, width=1, headwidth=hw))
pylab.annotate('$E$', xy=(xEoff,g2pathfunction(xEoff)), xytext=(xEoff+0.7,1.3),
               arrowprops=dict(shrink=0.01, width=1, headwidth=hw))


pylab.ylim(0)
pylab.xlim(xAoff - rpath,xEoff + rpath)
pylab.xlabel('path distance')
pylab.ylabel(r'$g^{(2)}(\left< 0,0,0\right>,\mathbf{r}_2)$')
pylab.legend(loc='best')

pylab.subplot(1,2,1).set_aspect('equal')
g2mc = read_walls(ff, z0, 'mc')
gmax = g2mc.max()
dx = 0.1
rmax = len(g2mc[:,0])*dx
zmax = len(g2mc[0,:])*dx
r = numpy.arange(0, rmax, dx)
z = numpy.arange(0, zmax, dx)
Z, R = numpy.meshgrid(z, r)

levels = numpy.linspace(0, gmax, gmax*100)
xlo = 0.85/gmax
xhi = 1.15/gmax
xhier = (1 + xhi)/2.0
ymax = 4

cdict = {'red':   [(0.0,  0.0, 0.0),
                   (xlo,  1.0, 1.0),
                   (1.0/gmax,  1.0, 1.0),
                   (xhi,  0.0, 0.0),
                   (xhier,0.0, 0.0),
                   (1.0,  1.0, 1.0)],

         'green': [(0.0, 0.0, 0.0),
                   (xlo,  0.1, 0.1),
                   (1.0/gmax, 1.0, 1.0),
                   (xhi, 0.0, 0.0),
                   (xhier,1.0, 1.0),
                   (1.0, 1.0, 1.0)],

         'blue':  [(0.0,  0.0, 0.0),
                   (xlo,  0.1, 0.1),
                   (1.0/gmax,  1.0, 1.0),
                   (xhi,  1.0, 1.0),
                   (xhier,0.0, 0.0),
                   (1.0,  0.0, 0.0)]}
cmap = matplotlib.colors.LinearSegmentedColormap('mine', cdict)

# pylab.contourf(Z-1, R, 0*g2mc, levels, cmap=cmap, extend='both')
# pylab.contourf(Z-1, 1-2*R, 0*g2mc, levels, cmap=cmap, extend='both')
# CS = pylab.contourf(Z, R, g2mc, levels, cmap=cmap, extend='both')
# pylab.contourf(Z, -R, g2mc, levels, cmap=cmap, extend='both')


pylab.pcolor(Z-1, R, 0*g2mc, vmin=0, vmax=gmax, cmap=cmap)
pylab.pcolor(Z-1, 1-2*R, 0*g2mc, vmin=0, vmax=gmax, cmap=cmap)
CS = pylab.pcolor(Z, R, g2mc, vmax=gmax, vmin=0, cmap=cmap)
pylab.pcolor(Z, -R, g2mc, vmax=gmax, vmin=0, cmap=cmap)

myticks = numpy.arange(0, numpy.floor(2.0*gmax)/2 + 0.1, 0.5)
pylab.colorbar(CS, extend='neither', ticks=myticks)
pylab.ylabel('$x_2$');
pylab.xlabel('$z_2$');

xs = [0, 0]
ys = [ymax, rpath]
dtheta = pylab.pi/80
for theta in pylab.arange(0, pylab.pi/2 + dtheta/2, dtheta):
    xs.append(rpath*pylab.sin(theta))
    ys.append(rpath*pylab.cos(theta))
xs.append(2*ymax)
ys.append(0)
pylab.plot(xs, ys, 'w-', linewidth=2)
pylab.plot(xs, ys, 'k--', linewidth=2)

pylab.annotate('$A$', xy=(0,rA), xytext=(1,3), arrowprops=dict(shrink=0.01, width=1, headwidth=hw))
pylab.annotate('$B$', xy=(0,rpath), xytext=(1,2.5), arrowprops=dict(shrink=0.01, width=1, headwidth=hw))
pylab.annotate('$C$', xy=(rpath/pylab.sqrt(2.0),rpath/pylab.sqrt(2.0)), xytext=(2.3,2.0), arrowprops=dict(shrink=0.01, width=1, headwidth=hw))
pylab.annotate('$D$', xy=(rpath,0), xytext=(3,1), arrowprops=dict(shrink=0.01, width=1, headwidth=hw))
pylab.annotate('$E$', xy=(rE,0), xytext=(5,1), arrowprops=dict(shrink=0.01, width=1, headwidth=hw))

pylab.xlim(-0.5, 1.5*ymax)
pylab.ylim(-ymax, ymax)

savedfilename = "figs/pair-correlation-pretty-" + str(int(ff*10)) + ".pdf"
pylab.title(r'$g^{(2)}(\left< 0,0,0\right>, \left<x_2, 0, z_2\right>)$ at $\eta = %g$' % ff)
pylab.savefig(savedfilename)
pylab.show()

