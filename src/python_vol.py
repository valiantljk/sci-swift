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
    dt_types={ 0:"int16", 1:"int32",2:"float32",3:"float64"}    
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
	    self.obj_curid=1 #reset
            self.obj_list={} #reset
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
    def H5VL_python_file_open(self, name, flags, fapl_id, dxpl_id, req, ipvol):
        print ("------- PYTHON H5Fopen:%s"%name)
        print ("------- PYTHON Vol: %d"%ipvol)
        if(ipvol==0):
          try:
            import h5py
	    print ('flags:%d'%flags)
	    self.obj_curid=1 #reset
	    self.obj_list={} #reset
            f = h5py.File(name,'r')
            self.obj_list[self.obj_curid]=f
            curid=self.obj_curid
            self.obj_curid=curid+1
            return curid
          except Exception as e:
            print ('file create failed in python with error: ',e)
            return -1
        else:
          print ("%d py vol is not implemented"%ipvol)

    def H5VL_python_dataset_open(self, obj_id, loc_params, name, dapl_id, dxpl_id, req):#iself, name, flags, fapl_id, dxpl_id, req, ipvol):
        print ("------- PYTHON H5Dopen:%s"%name)
        try:
           dst_parent_obj=self.obj_list[obj_id]
           try:
                dst_obj=dst_parent_obj[name]#,dims,dtype=self.dt_types[pytype])
                curid = self.obj_curid
                self.obj_list[curid] = dst_obj # insert new object 
                self.obj_curid = curid+1       # update current index
                print ("------- PYTHON H5Dopen OK")
                print ('dataset id is %d'%curid)
                return curid
           except Exception as e:
                print ('dataset open in python failed with error: ',e)
        except Exception as e:
           print ('retrieve obj failed in python dataset open:',e)
           return -1



    def H5VL_python_file_close(self, file_id, dxpl_id, req):
	try:
	   if(len(self.obj_list)==0):
	      print ("------- PYTHON H5Fclose OK") 
	      return 1
	   file_obj = self.obj_list[file_id] # retrive the file handle/obj based on id 'file_id'
	   if file_obj == None:
	     print ("Python File Obj is none")
	     return 1
           else:
   	     file_obj.close()
	     self.obj_list={} # empty all ojects 
	     self.obj_curid=1 # reset index to 0, (1 for now, due to bug of PyLong_AsVoidPtr can not take 0)
	     print ("------- PYTHON H5Fclose OK")
	     return 1
        except Exception as e:
           print ('file close failed in python with error:',e)
	   return -1
    def H5VL_python_dataset_close(self, dset_id, dxpl_id, req):                                        
        try:
           if (len(self.obj_list)==0): 
	      print ("------- PYTHON H5Dclose OK")
	      return 1 
           dset_obj = self.obj_list[dset_id] # retrive the file handle/obj based on id 'dataset_id'    
           if dset_obj == None:                                                                     
             print ("Python Dataset Obj is none")
	     return 1
           else:                                                                                    
             #dset_obj.close()
	     del(self.obj_list[dset_id])                                                                       
	     print ("------- PYTHON H5Dclose OK")
	     return 1     
        except Exception as e:                                                                      
           print ('dataset close failed in python with error:',e)
           return -1 

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
	      print ("------- PYTHON H5Gcreate OK")
              return curid
           except Exception as e:
              print ('group create in python failed with error:',e)
        except Exception as e:
           print ('retrieve obj failed in python group create')
        return -1 
    def H5VL_python_dataset_create(self, obj_id, loc_params, name, dcpl_id, dapl_id, dxpl_id, req, ndims,pytype,dims,maxdims):    
	print ("------- PYTHON H5Dcreate:%s"%name)
	try:
	   dst_parent_obj=self.obj_list[obj_id] 
	   try:
		dst_obj=dst_parent_obj.create_dataset(name,dims,dtype=self.dt_types[pytype])  
		curid = self.obj_curid
		self.obj_list[curid] = dst_obj # insert new object 
		self.obj_curid = curid+1       # update current index
		print ("------- PYTHON H5Dcreate OK")
		print ('dataset id is %d'%curid)
		return curid
	   except Exception as e:
		print ('dataset create in python failed with error: ',e)
	except Exception as e:
	   print ('retrieve obj failed in python dataset create:',e)
	   return -1   
    def H5VL_python_dt_info(self, obj_id):
        try:
         dims=self.obj_list[obj_id].shape
         ndims=len(dims)
         for k,v in self.dt_types.items():
            if v ==self.obj_list[obj_id].dtype:
	       dt=k
         axx=list()
         axx.append(ndims)
         axx.append(dt)
         axx=axx+list(dims)
         import numpy as np
         axx=np.asarray(axx,dtype='int64')
         return axx
        except Exception as e:
         print (e)
         pass
    def H5VL_python_dataset_write(self, obj_id, mem_type_id, mem_space_id, file_space_id, plist_id, buf, req):
	try:
           print ('in python dataset write, obj is ',obj_id)
           dst_parent_obj=self.obj_list[obj_id]
           try:
	        dst_parent_obj[:] = buf
                curid = self.obj_curid
		print ("------- PYTHON H5Dwrite OK")
                return curid
           except Exception as e:
                print ('dataset write in python failed with error: ',e)
        except Exception as e:
           print ('retrieve obj failed in python dataset write')
           return -1
    def H5VL_python_dataset_read(self, obj_id, mem_type_id, mem_space_id, file_space_id, plist_id, buf, req):
        try:
           print ('in python dataset read, obj is ',obj_id)
           dst_parent_obj=self.obj_list[obj_id]
           try:
                #buf[:] = dst_parent_obj[:] # TODO: make sure memcopy free
	        print ("passed in buffer has shape:,",buf.shape)
	        print ("data to be returned has shape:,",dst_parent_obj)
		buf[:] = dst_parent_obj[:]
	        print (buf)
		print ("------- PYTHON H5Dread OK")
                return 1
           except Exception as e:
                print ('dataset write in python failed with error: ',e)
        except Exception as e:
           print ('retrieve obj failed in python dataset write')
           return -1
    def H5VL_python_group_close(self, grp_id, dxpl_id, req):
	try:
           if (len(self.obj_list)==0):
              print ("------- PYTHON H5Gclose OK")
              return 1
           grp_obj = self.obj_list[grp_id] # retrive the group handle/obj based on id 'grp_id'    
           if grp_obj == None:
             print ("Python Group Obj is none")
	     return 1
           else:
             del(self.obj_list[grp_id])                                                                      
             print ("------- PYTHON H5Gclose OK")
             return 1
        except Exception as e:
           print ('dataset close failed in python with error:',e)
           return -1
     
