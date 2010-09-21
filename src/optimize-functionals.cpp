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

#include "Functionals.h"
#include "utilities.h"

const double kT = water_prop.kT; // room temperature in Hartree
const double R = 2.7;

int main(int, char **) {
  IdealGas(kT).create_header("src/IdealGasFast.h", "IdealGasFast", "kT");
  HardSpheres(kT, R).create_header("src/HardSpheresFast.h", "HardSpheresFast", "kT", "R");
  HardSpheresRF(kT, R).create_header("src/HardSpheresRFFast.h", "HardSpheresRFFast", "kT", "R");
}