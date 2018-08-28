import time
import numpy
def h5py():
	try:
		from _h5py import H5PVol
		return H5PVol() #return an instantiated object
	except Exception as e:
		print (time.strftime("%H:%M:%S"))
		print ("Python HDF5 Vol is not implemented")
		pass

def swift():
	try:
		from _swift import H5PVol
		return H5PVol() #return an instantiated object
	except Exception as e:
		print (">>")
		print (time.strftime("%H:%M:%S"))
		print ("Python Swift Vol is not implemented")
		print (e)
		print ("<<")
		pass
