#!/bin/bash
export logfile=h5boss_swift_1.1.log
rm $logfile
echo $logfile
export LD_LIBRARY_PATH_BEFORE=$LD_LIBRARY_PATH
source final.config.nersc
for k in 128 1024 10240 #100000 #1000000 # number of datasets
do
# for i in 1 2 3 # number of repeat tests
# do
  for j in 1 2 4 8 16 32 # number of processes
  do 
   srun -n $j ./SWIFT_H5BOSS dset_stats.csv boss_${k}_${j}.h5 $k >> $logfile
   wait
  done
# done
done

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH_BEFORE
