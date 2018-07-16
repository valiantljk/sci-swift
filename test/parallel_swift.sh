#!/bin/bash
LD_LIBRARY_PATH_BEFORE=$LD_LIBRARY_PATH
echo 'source final config to add python path into lib'
source final.config.nersc

srun -n 2 ./HDF5_file_create_parallel some.h5 1
#srun -n 1 ./HDF5_file_create_parallel some2.h5 10

LD_LIBRARY_PATH=$LD_LIBRARY_PATH_BEFORE
