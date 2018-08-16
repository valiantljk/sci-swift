#!/bin/bash -l
#SBATCH -N 1
#SBATCH -q regular
#SBATCH -C haswell
#SBATCH -t 1:50:00
#SBATCH -L SCRATCH
#SBATCH --mail-user=jalnliu@lbl.gov
#SBATCH --mail-type=END
#Rados Usage
#Usage: srun -n n_proc ./h5boss_write_dummy_data /path/to/dset_list.txt output_file n_dsets 

export NIMPW=eiYvDiWLHmEzZMrsEJ#oYu3r
source readme_cori
logfile=h5boss_swift_finally_1.1.log
rm $logfile
export LD_LIBRARY_PATH_BEFORE=$LD_LIBRARY_PATH
source final.config.nersc
for k in 128 1024 10240 #100000 #1000000 # number of datasets
do
# for i in 1 2 3 # number of repeat tests
# do
  for j in 1 2 4 8 16 32 # number of processes
  do 
   srun -n $j ./SWIFT_H5BOSS dset_stats.csv boss_${k}_${j}.h5 $k >> $logfile
  done
# done
done

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH_BEFORE
