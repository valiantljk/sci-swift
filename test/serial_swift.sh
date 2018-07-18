#!/bin/bash
LD_LIBRARY_PATH_BEFORE=$LD_LIBRARY_PATH
echo 'source final config to add python path into lib'
source final.config.nersc

./HDF5_file_create some.h5 10

LD_LIBRARY_PATH=$LD_LIBRARY_PATH_BEFORE
