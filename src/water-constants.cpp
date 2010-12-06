// Deft is a density functional package developed by the research
// group of Professor David Roundy
//
// Copyright 2010 The Deft Authors
//
// Deft is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// You should have received a copy of the GNU General Public License
// along with deft; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// Please see the file AUTHORS for a list of authors.

#include "equation-of-state.h"

static const double kB = 3.16681539628059e-6; // This is Boltzmann's constant in Hartree/Kelvin
static const double angstrom = 1.8897261; // An angstrom in atomic units

LiquidProperties water_prop = {
  //2.93, // hard sphere radius of water in bohr
  3.03420*angstrom/2, // hard sphere radius of water in bohr
  4.9388942e-3, // density of liquid water
  1.14e-7, // vapor density of water
  298.15*kB, // room temperature in Hartree
  250.0*kB, // epsilon_dispersion
  1.78890, // lambda_dispersion
  1400.00*kB, // epsilonAB
  1.06673*angstrom*angstrom*angstrom, // kappaAB from Clark et al
};