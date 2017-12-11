#HDF5 VOL Dispatch Layer in Python
#Nov 28 2017
#Jialin Liu
#LBNL/NERSC

#class H5VL_class_g:
    
 
#H5VL_python_file_create
def H5VL_python_file_create(name, flags, fcpl_id, fapl_id, dxpl_id, req):
    print ("------- PYTHON H5Fcreate:%s"%name)

    #call python vol 
     #import h5py 
     #f= h5py.File('test.h5','a') 
    #TODO:  
    # Inatialize an python generic object, 
    # Select the swift vol, return that swift file object 
    # and cast to a generic python object
    #print ("creating hdf5 file with h5py\n")
    try: 
     print ("importing start")
     import h5py #numpy3#, h5py 
     print ("importing end")
     print (numpy.__file__) 
    except Exception as e:
     print (e)
     pass
    #import h5py 
#    try: 
#     import h5py
     #print (h5py.__file__)
     #f = h5py.File('test1.h5','a')
#    except Exception as e:
#     print (e)
    #    print (e)
    #    pass
    #return id(f)
    # return id(generic_py_object)   
    return 12
#H5VL_python_file_open
# const char *name, unsigned flags, hid_t fapl_id, hid_t dxpl_id, void **req
def H5VL_python_file_open(name, flags, fapl_id, dxpl_id, req):
    print ("------- PYTHON H5Fopen:%s"%name)
    #call python vol 
    #import h5py 
    #f= h5py.File('test.h5','a')    
    return 12
#static herr_t
#H5VL_python_file_close(void *file, hid_t dxpl_id, void **req)
def H5VL_python_file_close(file, dxpl_id, req):
    print ("------- PYTHON H5Fclose OK")
    #call python vol 
    #import h5py 
    #f= h5py.File('test.h5','a')    
    return 12

#static void *
#H5VL_python_group_create(void *obj, H5VL_loc_params_t loc_params, const char *name,
#                      hid_t gcpl_id, hid_t gapl_id, hid_t dxpl_id, void **req)
def H5VL_python_group_create(obj, loc_params, name, gcpl_id, gapl_id, dxpl_id, req):
    print ("------- PYTHON H5Gcreate:%s"%name)

    return 12


#static herr_t
#H5VL_python_group_close(void *grp, hid_t dxpl_id, void **req)
def H5VL_python_group_close(grp, dxpl_id, req):
    print ("------- PYTHON H5Gclose OK")

    return 12

#static void *
#H5VL_python_dataset_create(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t dcpl_id, hid_t dapl_id, hid_t dxpl_id, void **req)

def H5VL_python_dataset_create(obj, loc_params, name, dcpl_id, dapl_id, dxpl_id, req):
    print ("------- PYTHON H5Dcreate:%s"%name)

    return 12

#static void *
#H5VL_python_dataset_open(void *obj, H5VL_loc_params_t loc_params, const char *name, hid_t dapl_id, hid_t dxpl_id, void **req)
def H5VL_python_dataset_open(obj, loc_params, name, dapl_id, dxpl_id, req):
    print ("------- PYTHON H5Dopen:%s"%name)

    return 12

#static herr_t
#H5VL_python_dataset_read(void *dset, hid_t mem_type_id, hid_t mem_space_id,
#                      hid_t file_space_id, hid_t plist_id, void *buf, void **req)

def H5VL_python_dataset_read(dset, mem_type_id, mem_space_id, file_space_id, plist_id, buf, req):
    print ("------- PYTHON H5Dread OK")

    return 12


#static herr_t
#H5VL_python_dataset_write(void *dset, hid_t mem_type_id, hid_t mem_space_id,
#                       hid_t file_space_id, hid_t plist_id, const void *buf, void **req)

def H5VL_python_dataset_write(dset, mem_type_id, mem_space_id, file_space_id, plist_id, buf, req):
    print ("------- PYTHON H5Dwrite OK")

    return 12

#static herr_t
#H5VL_python_dataset_close(void *dset, hid_t dxpl_id, void **req)
def H5VL_python_dataset_close(dset, dxpl_id, req):
    print ("------- PYTHON H5Dclose OK")

    return 12
