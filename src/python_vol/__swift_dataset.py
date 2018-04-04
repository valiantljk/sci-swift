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
					#print ("empty object:%s"%sciobj_name)
					try:	
						objs = [SwiftUploadObject(
							None, sciobj_name
							)]
					except Exception as e:
						print ('construct object error: ',e)
				else:
					objs = sci_swift_object(sciobj_name, sciobj_source)
					#print('in swift.upload, obj name is %s:',sciobj_name)
					#print ('in swift.upload, swift objs:',objs)
				#print ('in swift.upload:')
				try:
					r=swift.upload(container=container,objects=objs)
				except Exception as e:
					print ("swift.upload: ",e)
				#print ('finish swift.upload r:',r)
				try:
					for ri in r:
						if not ri['success']:
							print ('object upload error')
							print ('error:%s',r['error'])
						else:
							print ('object upload ok')
							print ('ri:',ri)
				except Exception as e:
					print ('ri[''] failed, ',e)
			except Exception as e:
				print ("object create error:",e)
	except Exception as e:
		print ("swift object create initialize failed:",e)

def sci_swift_object(sciobj_name, sciobj_source):
	#import numpy
	#from StringIO import StringIO
	#sci_stream = StringIO()
	#numpy.save(sci_stream,sciobj_source) # need to check if sciobj_source is a numpy array
	#print ('sci_stream:',sci_stream)
	#print ('sci_source:',sciobj_source)
	from io import BytesIO
	
	objs = [SwiftUploadObject(
		BytesIO(sciobj_source), sciobj_name
		)]
	return objs
def swift_object_open(container, sciobj_name, options=None):
	if 1==1: #TODO: query swift store to see if object exists
		#TODO: retrieve object shape and type info via swift metadata query
		return 1 
	else:
		return -1
def swift_metadata_create(container, sciobj_name, sciobj_metadata,options=None):
	#print ("hello")
	#header_data={}
	#first retrieve existing metadata, otherwise, meta post will destroy existing one
	with SwiftService(options = options) as swift:
		try:
			#print("start stat:container=%s,object=%s"%(container,sciobj_name))
			#r=swift.stat(container=container, objects=[sciobj_name])
			#print ("end stat")
		 	#header_data={}
			#for item in r:
				#print ("item is:",item)
			#	if item['success']:
			#		header_data[item['object']]=item['headers']
				#else:
				#	print ('Failed to retrieve meta')	
			#print (header_data)
			#test_medata={"dtype":"float","ndims":"3"}
			post_options = {"meta": sciobj_metadata}
			#post_options =  {"meta": test_medata}
			#print('going to post the metadata:post_options:',post_options)
			r=swift.post(container=container, objects=[sciobj_name],options=post_options)
		except Exception as e: 
			print ('stat error: ',e)

def swift_metadata_get(container,sciobj_name, options=None):
	with SwiftService(options = options) as swift:
		try:
			meta_data={}
			r=swift.stat(container=container, objects=[sciobj_name])
			for item in r:
				if item['success']:
					#print ("success")
					meta_data['dims']=item['headers']['x-object-meta-dims']
					meta_data['type']=item['headers']['x-object-meta-type']
					meta_data['ndim']=item['headers']['x-object-meta-ndim']
					#print('got meta:',meta_data)
					#header_data[item['object']]=item['headers']['meta']
			#print ('got meta2:',meta_data)
			return meta_data
		except Exception as e:
			print ('stat error:',e)
