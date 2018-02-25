def h5py():
    try:
       from _h5py import H5PVol
       return H5PVol() #return an instantiated object
    except Exception as e:
       print ("Python HDF5 Vol is not implemented")
       pass

def swift():
    try:
       from _swift import H5PVol
       return H5PVol() #return an instantiated object
    except Exception as e:
       print ("Python Swift Vol is not implemented")
       pass
