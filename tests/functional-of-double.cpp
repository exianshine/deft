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

#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include "Functionals.h"
#include "OptimizedFunctionals.h"
#include "ContactDensity.h"

void took(const char *action) {
  static clock_t start = 0;
  clock_t end = clock();
  printf("    %s took %g seconds.\n", action, (end - double(start))/CLOCKS_PER_SEC);
  start = end;
}

int test_functional(const char *name, Functional f, double n, double fraccuracy=1e-14) {
  const double kT = 1e-3;

  printf("\n**************************");
  for (unsigned i=0;i<strlen(name);i++) printf("*");
  printf("\n* Testing %s of %10g *\n", name, n);
  for (unsigned i=0;i<strlen(name);i++) printf("*");
  printf("**************************\n\n");

  // Here we set up the lattice.
  Lattice lat(Cartesian(4.2,0,-0.1), Cartesian(0.1,5.2,0), Cartesian(0,0,5));
  double resolution = 0.2;
  GridDescription gd(lat, resolution);
  Grid nr(gd, n*VectorXd::Ones(gd.NxNyNz));

  const double Edouble = f(kT, n);
  took("Evaluating functional of a double");
  printf("Edouble = %g\n", Edouble);
  const double Egrid = f.integral(kT, nr)/gd.Lat.volume();
  took("Evaluating functional of a 20x20x20 grid");

  int retval = 0;
  printf("Edouble = %g\n", Edouble);
  printf("Egrid   = %g\n", Egrid);

  const double deriv_double = f.derive(kT, n);
  took("Evaluating derivative of a double");
  Grid grad(nr);
  grad.setZero();
  f.integralgrad(kT, gd, nr, &grad);
  took("Evaluating gradient of a 20x20x20 grid");
  const double deriv_grid = grad[0]/gd.dvolume;
  printf("deriv double = %g\n", deriv_double);
  printf("deriv grid   = %g\n", deriv_grid);
  if (Edouble == 0) {
    // If the true answer is zero, the Edouble should say so!
    printf("absolute error = %g\n", Edouble - Egrid);
    if (fabs(Edouble - Egrid) > fraccuracy) {
      printf("FAIL: Error in the energy is too big!\n");
      retval++;
    }
  } else {
    printf("fractional error in energy = %g\n", (Edouble - Egrid)/fabs(Edouble));
    if (!(fabs((Edouble - Egrid)/Edouble) < fraccuracy)) {
      printf("FAIL: Error in the energy is too big!\n");
      retval++;
    }
  }

  if (deriv_double == 0) {
    printf("absolute error = %g\n", deriv_double - deriv_grid);
    if (fabs(deriv_double - deriv_grid) > fraccuracy) {
      printf("FAIL: Error in the gradient is too big!\n");
      retval++;
    }
  } else {
    printf("fractional error in gradient = %g\n", (deriv_double - deriv_grid)/fabs(deriv_double));
    if (!(fabs((deriv_double - deriv_grid)/deriv_double) < fraccuracy)) {
      printf("FAIL: Error in the gradient is just too darn big!\n");
      retval++;
    }
  }

  if (fabs(deriv_double) > 1e-30) {
    retval += f.run_finite_difference_test(name, kT, nr);
    took("Finite difference test");
  }

  return retval;
}

jmp_buf sig_int_response;

void dieplease(int) {
  longjmp(sig_int_response, 1);
}

int main(int, char *argv[]) {
  if (setjmp(sig_int_response)) {
    printf("I was interrupted by control-C...\n");
    printf("I will exit now!\n");
    exit(1);
  }
  signal(SIGINT, dieplease);
  int retval = 0;
  const Functional n = EffectivePotentialToDensity();
  const double kT = 1e-3;

  {
    double Veff = -1e-3*log(1e-5);
    Functional x = Identity();

    retval += test_functional("sqr(yzShellConvolve(1)(x)))", sqr(yzShellConvolve(1)(x)), 1, 1e-13);
    retval += test_functional("sqr(xyShellConvolve(1)(x)))", sqr(xyShellConvolve(1)(x)), 1, 1e-13);
    retval += test_functional("zxShellConvolve(1)(x))", zxShellConvolve(1)(x), 1, 1e-13);
    retval += test_functional("sqr(zzShellConvolve(1)(x)))", sqr(zzShellConvolve(1)(x)), 1, 1e-13);
    retval += test_functional("1-yyShellConvolve(1)(x))", 1-yyShellConvolve(1)(x), 1, 1e-13);
    retval += test_functional("sqr(xxShellConvolve(1)(x)))", sqr(xxShellConvolve(1)(x)), 1, 1e-13);
    retval += test_functional("sqr(xShellConvolve(1)(x)))", sqr(xShellConvolve(1)(x)), 1, 1e-13);
    retval += test_functional("sqr(yShellConvolve(1)(x)))", sqr(yShellConvolve(1)(x)), 1, 1e-13);
    retval += test_functional("sqr(zShellConvolve(1)(x)))", sqr(zShellConvolve(1)(x)), 1, 1e-13);
    retval += test_functional("StepConvolve(1)(x)", StepConvolve(1)(x), 1e-5, 1e-13);
    retval += test_functional("ShellConvolve(1)(x))", ShellConvolve(1)(x), 1e-5, 2e-13);

    Functional stepped = StepConvolve(1);
    retval += test_functional("StepConvolve(1)(sqr(StepConvolve(x)))", ShellConvolve(1)(sqr(stepped)), 1e-5, 2e-13);

    retval += test_functional("OfEffectivePotential(sqr(StepConvolve(1)))",
                              OfEffectivePotential(sqr(StepConvolve(1))), -0.01, 2e-13);
    retval += test_functional("OfEffectivePotential(sqr(StepConvolve(1)))",
                              OfEffectivePotential(sqr(StepConvolve(1))), 0.2, 2e-13);
    retval += test_functional("OfEffectivePotential(sqr(StepConvolve(1)))",
                              OfEffectivePotential(sqr(StepConvolve(1))), 0, 3e-14);

    retval += test_functional("HardSpheres(2,1e-3)", HardSpheres(2), 1e-5, 1e-13);
    //retval += test_functional("HardSpheresWBnotensor(...)",
    //                          HardSpheresWBnotensor(2)(n), Veff, 1e-13);
    retval += test_functional("HardSpheresNoTensor2(...)",
                              HardSpheresNoTensor2(2)(n), Veff, 2e-13);
    retval += test_functional("IdealGasOfVeff", IdealGasOfVeff(), Veff, 2e-13);
    retval += test_functional("SaftFluid2(...)",
                              SaftFluid2(2,1e-2,0.02, 1e-4, 1.8, 0.7,0), 1e-4, 4e-13);
    //retval += test_functional("SaftFluidSlow(...)",
    //                          SaftFluidSlow(2,1e-2,0.02, 1e-4, 1.8, 0.7,0), 1e-4, 2e-13);
    //retval += test_functional("OfEffectivePotential(SaftFluidSlow(...))",
    //                          OfEffectivePotential(SaftFluidSlow(2,1e-2,0.02, 1e-4, 1.8, 0.7,0)), Veff, 2e-13);
    retval += test_functional("OfEffectivePotential(SaftFluid2(...))",
                              OfEffectivePotential(SaftFluid2(2,1e-2,0.02, 1e-4, 1.8, 0.7,0)), Veff, 4e-13);

    retval += test_functional("x*x)", x*x, 0.1, 1e-13);
    retval += test_functional("3*x*x)", 3*x*x, 0.1, 1e-13); 
    retval += test_functional("3*x*x)", 3*x*x, 0.1, 1e-13);
    retval += test_functional("3*sqr(4*x))", 3*sqr(4*x), 0.1, 1e-13);
    retval += test_functional("Gaussian(2)(-3*sqr(4*x)))", Gaussian(2)(-3*sqr(4*x)), 0.1, 1e-13);
    retval += test_functional("Pow(4)(x))", Pow(4)(x), 0.1, 1e-13);

    retval += test_functional("log(x)", log(x), 0.1, 1e-13);
    retval += test_functional("exp(x)", exp(x), 0.1, 1e-13);
  }

  {
    Functional f = ChemicalPotential(0.1);
    retval += test_functional("chemical potential", f, 1e-9, 1e-12);
    retval += test_functional("chemical potential", f, 1e9, 1e-14);
    retval += test_functional("chemical potential", f, 1e-2, 1e-12);
    retval += test_functional("chemical potential of V", f(n), -kT*log(1e-9), 1e-12);
    retval += test_functional("chemical potential of V", f(n), -kT*log(1e-3), 1e-12);
  }

  if (retval == 0) {
    printf("\n%s passes!\n", argv[0]);
  } else {
    printf("\n%s fails %d tests!\n", argv[0], retval);
    return retval;
  }
}
