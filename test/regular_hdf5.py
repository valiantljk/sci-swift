import h5py, numpy
a=numpy.arange(100*20*30)
f=h5py.File('regular.h5','a')
d=f.create_dataset('spacex/falcon_int32', data=a,dtype='int32',shape=(100,20,30))
f.close()
