#HDF5 VOL Dispatch Layer in Python
#Nov 28 2017
#Jialin Liu
#LBNL/NERSC
#class H5VL_class_g:
    #call python vol 
     #import h5py 
     #f= h5py.File('test.h5','a') 
    #TODO:  
    # Inatialize an python generic object, 
    # Select the swift vol, return that swift file object 
    # and cast to a generic python object
    #print ("creating hdf5 file with h5py\n")    
 
#H5VL_python_file_create
#import gc
#gc.disable()
#print ('gc is enabled? ',gc.isenabled())

#class PyVol
class H5PVol:
    file_NO=(None,None) # (file Name, file Object)
    obj_list={} # dictionary for various h5py objects, e.g., groups, datasets, links, etc. 
    def H5VL_python_file_create(name, flags, fcpl_id, fapl_id, dxpl_id, req, ipvol):
    	print ("------- PYTHON H5Fcreate:%s"%name)
    	print ("------- PYTHON Vol: %d"%ipvol)
	if(ipvol==0):
     	  import h5py
     	  f = h5py.File(name,'a')
          self.file_NO=(name,f)
          # return a hash value of name, TODO: potential conflict with HDF5 objects' name
          return name.__hash__()
        else:
          print ("%d py vol is not implemented"%ipvol)
    def H5VL_python_file_close(file, dxpl_id, req):
    	print ("------- PYTHON H5Fclose OK")
	# check if file obj is available in file_NO
        try:
	   obj = exist_obj(self.)
   	self.h5file.close()
        return 1
    def H5VL_python_group_create(obj, loc_params, name, gcpl_id, gapl_id, dxpl_id, req):
    	print ("------- PYTHON H5Gcreate:%s"%name)
	try:
	   print ('in python group create, obj is ',obj)
           #fx=objects_by_id(obj)
           grp=fx.create_group(name)
     return id(grp)
    except Exception as e:
     print ('group create in python failed')
     exit (1)
     return 0 # TODO figure out failure return code

def objects_by_id(id_):
    print ('need to find obj id:',id_)
    print ("Total number of objs:%d"%(len(gc.get_objects())))
    for obj in gc.get_objects():
        if id(obj) == id_:
	    print ("found id:%d"%(id_))
            return obj
        else:
           difid=id(obj)-id_
           #print (id(obj) )
           if abs(difid)<10:
            print ("diff:%d"%difid)
    raise Exception("No found")

def H5VL_python_file_create(name, flags, fcpl_id, fapl_id, dxpl_id, req, ipvol):
    print ("------- PYTHON H5Fcreate:%s"%name)
    print ("------- PYTHON Vol: %d"%ipvol)
    #get h5py vol based on ipvol
    import gc
    #disable auto garbage collector
    gc.disable()
    print ("gc is enabled?",gc.isenabled())
#    global f
    if(ipvol==0):
     import h5py 
     f = h5py.File(name,'a')
     print ("id of file object is:",id(f)," and hex:",hex(id(f)))
     #f1=objects_by_id(id(f))

     return id(f)
     #return f
    else:
     print ("%d py vol is not implemented"%ipvol)

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
    import gc
    gc.disable()
    fx=objects_by_id(file)
    #print ("file close fx:",fx)
    fx.close()
    return 1

#static void *
#H5VL_python_group_create(void *obj, H5VL_loc_params_t loc_params, const char *name,
#                      hid_t gcpl_id, hid_t gapl_id, hid_t dxpl_id, void **req)
def H5VL_python_group_create(obj, loc_params, name, gcpl_id, gapl_id, dxpl_id, req):
    print ("------- PYTHON H5Gcreate:%s"%name)
    #recast python obj from void pointer, i.e., id in python layer
    #TODO: recast into a generic python object as QK suggests, but now we cast into a h5py File obj, Dec. 17, 5:59pm
    import gc
    gc.disable()
    try:
     print ('in python group create, obj is ',obj)
     fx=objects_by_id(obj)
     grp=fx.create_group(name)
     return id(grp)
    except Exception as e:
     print ('group create in python failed')
     exit (1)
     return 0 # TODO figure out failure return code

#static herr_t
#H5VL_python_group_close(void *grp, hid_t dxpl_id, void **req)
def H5VL_python_group_close(grp, dxpl_id, req):
    print ("------- PYTHON H5Gclose OK")
    import gc
    gc.disable()
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
