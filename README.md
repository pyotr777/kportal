[![Build Status](https://travis-ci.org/pyotr777/kportal.svg?branch=master)](https://travis-ci.org/pyotr777/kportal)

# K-portal installation tools.

version 1.01


Note: In the case, a kportal server (installed kp_server) have a difference kernel version comparse with k-computer.
  You need to build a kdeskdeamon yourself:
  + copy src/kdeskdaemon/ to k-computer
  + build & get output kdeskdeamon binary
  
  + manual copy to src/release before run make install.
    If you want to modify installed:
    + replace the kdeskdaemon file at /etc/kportal/kdeskdeamon/kdeskdaemon.
