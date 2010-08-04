// -*- mode: C++; -*-

#pragma once

#include "Functional.h"

class Minimizer {
protected:
  Functional f;
  VectorXd *x; // Note that we don't own this data!
  int iter;
public:
  Minimizer(Functional myf, VectorXd *data)
    : f(myf), x(data), last_grad(0) {
    iter = 0;
    energy_valid = false;
  }

  // improve_energy returns false if the energy is fully converged
  // (i.e. it didn't improve), and there is no reason to call this
  // minimizer any more.  Thus improve_energy can be naturally used as
  // the conditional in a while or for loop.
  virtual bool improve_energy(bool verbose = false) = 0;

  // The print_info function should be called at each iteration,
  // unless verbose is set to false in improve_energy.  But you can
  // also call it manually.
  virtual void print_info(int iter) const;

  // energy returns the current energy.
  double energy() const {
    if (!energy_valid) {
      last_energy = f(*x);
      energy_valid = true;
    }
    return last_energy;
  }
  const VectorXd &grad() const {
    if (!last_grad) {
      last_grad = new VectorXd(*x); // hokey
      last_grad->setZero(); // Have to remember to zero it out first!
      f.grad(*x, last_grad);
    }
    return *last_grad;
  }

  // Note that we're changing the position x.
  void invalidate_cache() {
    energy_valid = false;
    delete last_grad;
    last_grad = 0;
  }
private:
  mutable double last_energy;
  mutable VectorXd *last_grad;
  mutable bool energy_valid;
};