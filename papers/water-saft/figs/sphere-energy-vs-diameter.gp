#!/usr/bin/gnuplot
#
# guide for line and point styles:
#
#  0  ..............  .                    broken line
#  1  --------------  +                    red
#  2  -- -- -- -- --  x                    green
#  3  -  -  -  -  -   *                    blue
#  4  ..............  empty square         magenta
#  5  __.__.__.__.__  full  square         cyan
#  6  _ . _ . _ . _   empty circle         yellow
#  7  - -  - -  - -   full  circle         black
#  8  - - -  - - -    empty up triangle    brown
#  9  - - - -  - - -  full  up triangle    grey
# 10 (1)              empty down triangle
# 11 (2)              full  down triangle
# 12 (3)              empty diamond
# 13 (4)              full  diamond
# 14 (5)              empty pentagon
# 15 (6)              full  pentagon
# 16-31               watches

set terminal postscript eps enhanced color solid "Helvetica" 20
set output 'figs/sphere-energy-vs-diameter.eps'

#set key noauto outside top
set key bottom right

# set multiplot

set size 1,1          # The first plot (host plot)
set origin 0,0
set xlabel 'Radius (nm)'
set ylabel '{/Symbol m}_{ex}/Area (mN/m)'

set style line 1 lt 3 lw 3 pt 7 ps 2
set style line 2 lt 7 pt 6 lw 3 ps 2

mNpermeter = 6.4230498e-07 # in atomic units
nm = 18.8972613 # in atomic units

plot [0:1.05] [0:] \
'figs/sphere.dat' u ($1/2):($2/(pi*($1+0.00001)*($1+0.00001)*nm*nm)/mNpermeter) title 'CDFT' with lp ls 1, \
'figs/sphere-energy-vs-diameter-spce.dat' u ($1/10):2 title 'SPC/E molecular dynamics' with p ls 2, \
'figs/sphere-energy-vs-diameter-spce.dat' u ($1/9):(71.73) notitle with l ls 1, \
'figs/sphere-energy-vs-diameter-spce.dat' u ($1/9):(63.4) notitle with l ls 2
