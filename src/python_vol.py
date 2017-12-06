#HDF5 VOL Dispatch Layer in Python
#Nov 28 2017
#Jialin Liu
#LBNL/NERSC
 
#H5VL_python_file_create
def H5VL_python_file_create(name, flags, fcpl_id, fapl_id, dxpl_id, req):
    print ("------- PYTHON H5Fcreate:%s"%name)
    #call python vol 
    #import h5py 
    #f= h5py.File('test.h5','a')    
    return 12
#H5VL_python_file_open


#H5VL_python_file_close

def H5VL_python_group_create(obj, loc_params, name, gcpl_id, gapl_id, dxpl_id, req):
    print ("------- PYTHON H5Gcreate:%s"%name)

    return 12
