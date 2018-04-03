# https://docs.openstack.org/python-swiftclient/latest/service-api.html
# Modified by Jialin Liu
# Date Mar 1 2018
# This script can test the write performance on lustre and openstack-swift
# It first generates a HDF5 file in memory with a 2D float dataset, 
# then writes that directly to targeted storage
try:
	from __swift_connect import swift_connect
	import numpy
	from StringIO import StringIO
	from swiftclient.service import SwiftError, SwiftService, SwiftUploadObject
except Exception as e:
	print ("connect error:",e)
#from __swift_core import *
'''
An HDF5 Dataset is mapped one or many swift object(s)
An HDF5 Group is mapped to one swift object
An HDF5 Attribute is associated to an Swift object as metadata, paired into (key,value), extendable
'''
def swift_object_create(container, sciobj_name, sciobj_source=None, options=None):
	try:	
		with SwiftService(options = options) as swift:
			try:
				objs = []
				if sciobj_source is None: 
					#obj_name is container's name, or a tracking group's name,: file.h5/grp1
					#the following step is to create a empty object in parent container when a sub-group is created, 
					#so the sub-group is trackable in the parent group as a virtual object
					#print ("empty object")
					try:	
						objs = [SwiftUploadObject(
							None, sciobj_name
							)]
					except Exception as e:
						print ('construct object error: ',e)
				else:
					objs = sci_swift_object(sciobj_name, sciobj_source)
				#print ('in swift.upload:')
				try:
					r=swift.upload(container, objs)
				except Exception as e:
					print ("swift.upload: ",e)
				#print ('finish swift.upload r:',r)
				try:
					for ri in r:
						if not ri['success']:
							print ('object upload error')
							print ('error:%s',r['error'])
						#else:
						#	print ('object upload ok')
				except Exception as e:
					print ('ri[''] failed, ',e)
			except Exception as e:
				print ("object create error:",e)
	except Exception as e:
		print ("swift object create initialize failed:",e)

def sci_swift_object(sciobj_name, sciobj_source):
	#import numpy
	#from StringIO import StringIO
	sci_stream = StringIO()
	numpy.save(sci_stream,sciobj_source) # need to check if sciobj_source is a numpy array
	objs = [SwiftUploadObject(
		sci_stream, sciobj_name
		)]
	return objs
def swift_object_open(container, sciobj_name, options=None):
	if 1==1: #TODO: query swift store to see if object exists
		#TODO: retrieve object shape and type info via swift metadata query
		return 1 
	else:
		return -1
