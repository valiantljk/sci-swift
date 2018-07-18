#!/bin/bash
echo 'start'
#salloc -N 2 -q interactive -C haswell -t 5
LD_LIBRARY_PATH_BEFORE=$LD_LIBRARY_PATH
echo 'source final config to add python path into lib'
source final.config.nersc

srun -n 2 ./HDF5_file_create_parallel some.h5 2 # run as ./HDF5_file_create_parallel filename number_of_files
srun -n 2 ./HDF5_objects_create_parallel some.h5 mygroup mydset 3 10 20 30 # run as ./HDF5_objects_create_parallel filename groupname datasetname ndims dims0 dims1

LD_LIBRARY_PATH=$LD_LIBRARY_PATH_BEFORE
echo 'done'
#exit
