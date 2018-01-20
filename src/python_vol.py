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
    obj_curid = 1  # PyLong_AsVoidPtr can not convert 0 correctly
    obj_list  = {} # dictionary for various h5py objects, e.g., groups, datasets, links, etc. 
	           # starting obj is reserved for HDF5 file handle, with index 0 as its key
                   # {0:hdf5_handle, ...}
    def H5VL_python_file_create(self, name, flags, fcpl_id, fapl_id, dxpl_id, req, ipvol):
    	print ("------- PYTHON H5Fcreate:%s"%name)
    	print ("------- PYTHON Vol: %d"%ipvol)
	if(ipvol==0):
          try:
     	    import h5py
     	    f = h5py.File(name,'a')
            self.obj_list[self.obj_curid]=f
            curid=self.obj_curid
	    self.obj_curid=curid+1
            return curid
          except Exception as e:
            print ('file create failed in python with error: ',e)
            return -1
        else:
          print ("%d py vol is not implemented"%ipvol)
    def H5VL_python_file_close(self, file_id, dxpl_id, req):
    	print ("------- PYTHON H5Fclose OK")
	# check if file obj is available in file_NO
        try:
	   file_obj = self.obj_list[file_id] # retrive the file handle/obj based on id 'file_id'
	   if file_obj == None:
	     print ("Python File Obj is none")
           else:
   	     file_obj.close()
	     self.obj_list={} # empty all ojects 
	     self.obj_curid=1 # reset index to 0, (1 for now, due to bug of PyLong_AsVoidPtr can not take 0)
        except Exception as e:
           print ('file close failed in python with error:',e)
	   return -1
        return 1
    def H5VL_python_dataset_close(self, dset_id, dxpl_id, req):                                        
        print ("------- PYTHON H5Dclose OK")
        try:
           if (len(self.obj_list)==0):
	    return 1 
           dset_obj = self.obj_list[dset_id] # retrive the file handle/obj based on id 'dataset_id'    
           if dset_obj == None:                                                                     
             print ("Python Dataset Obj is none")
           else:                                                                                    
             #dset_obj.close()                                                                       
             self.obj_list={} # empty all ojects 
             self.obj_curid=1 # reset index to 0, (1 for now, due to bug of PyLong_AsVoidPtr can not take 0)      
        except Exception as e:                                                                      
           print ('dataset close failed in python with error:',e)
           return -1 
        return 1

    def H5VL_python_group_create(self, obj_id, loc_params, name, gcpl_id, gapl_id, dxpl_id, req):
    	print ("------- PYTHON H5Gcreate:%s"%name)
	try:
	   print ('in python group create, obj is ',obj_id)
	   grp_parent_obj=self.obj_list[obj_id]
	   try:
              grp_obj=grp_parent_obj.create_group(name)
	      curid = self.obj_curid
	      self.obj_list[curid] = grp_obj # insert new object
	      self.obj_curid = curid+1       # update current index
              return curid
           except Exception as e:
              print ('group create in python failed with error:',e)
        except Exception as e:
           print ('retrieve obj failed in python group create')
        return -1 
    def H5VL_python_dataset_create(self, obj_id, loc_params, name, dcpl_id, dapl_id, dxpl_id, req):    
	print ("------- PYTHON H5Dcreate:%s"%name) 
	try:
	   print ('in python dataset create, obj is ',obj_id)  
	   dst_parent_obj=self.obj_list[obj_id] 
	   try:
		#TODO: figure out shape,dtype from loc_params
		dst_obj=dst_parent_obj.create_dataset(name,(10,10),dtype='f8')  
		curid = self.obj_curid
		self.obj_list[curid] = dst_obj # insert new object 
		self.obj_curid = curid+1       # update current index
		print ('dataset id is %d'%curid)
		return curid
	   except Exception as e:
		print ('dataset create in python failed with error: ',e)
	except Exception as e:
	   print ('retrieve obj failed in python dataset create')
	   return -1   
    def H5VL_python_dataset_write(dset, mem_type_id, mem_space_id, file_space_id, plist_id, buf, req):
	print ("------- PYTHON H5Dwrite") 
	
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
