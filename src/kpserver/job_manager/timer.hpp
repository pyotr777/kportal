/**
  @file timer.h
  @author 
  @copyright 2015 XXX
  */

#ifndef TIMER_H_
#define TIMER_H_

class Timer {
 public:
  Timer();

  ~Timer();

  void Reset();

  double Seconds();

  void Start();

  void Stop();

 private:
  double seconds_;
  double ref_;
};

#endif  // TIMER_H_
