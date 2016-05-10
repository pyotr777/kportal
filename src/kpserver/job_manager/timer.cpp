/**
  @file timer.cc
  @author 
  @copyright 2015 XXX
  */

#include "timer.hpp"

#include <omp.h>

Timer::Timer() {
  seconds_ = 0.0;
  ref_ = 0.0;  // OpenMP wall time.
}

Timer::~Timer() {
}

void Timer::Reset() {
  seconds_ = 0.0;
  ref_ = 0.0;
}

double Timer::Seconds() {
  return seconds_;
}

void Timer::Start() {
  ref_ = omp_get_wtime();
}

void Timer::Stop() {
  seconds_ -= ref_;
  ref_ = omp_get_wtime();
  seconds_ += ref_;
}
