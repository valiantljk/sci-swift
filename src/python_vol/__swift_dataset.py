# https://docs.openstack.org/python-swiftclient/latest/service-api.html
# Modified by Jialin Liu
# Date Mar 1 2018
# This script can test the write performance on lustre and openstack-swift
# It first generates a HDF5 file in memory with a 2D float dataset, 
# then writes that directly to targeted storage
try:
	from __swift_connect import swift_connect
	import numpy
        import sys
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
	"""
	`Object create' is called by different HDF5 object create, e.g., group create, dataset create, 
	and dataset write (since there is no update in swift)

	Input: 
		Container name
		Object name
		Object source
	Output:
		None
	"""
	try:	
		with SwiftService(options = options) as swift:
			try:
				objs = []
				post_options={}
				try:
					meta_old=swift_metadata_get(container=container,sciobj_name=sciobj_name)
			        	sci_obj_meta={}
					sci_obj_meta['type'] = str(meta_old['type'])
                                	sci_obj_meta['dims'] = str(meta_old['dims'])
                                	sci_obj_meta['ndim'] = str(meta_old['ndim'])
					post_options={"meta": sci_obj_meta, "long": "True"}
				except Exception as e:
					pass
				if sciobj_source is None:
					try:	
						objs = [SwiftUploadObject(
							None, sciobj_name
							)]
					except Exception as e:
						print ('construct object error: ',e)
				else:
					objs = sci_swift_object(sciobj_name, sciobj_source)
				try:
					r=swift.upload(container=container,objects=objs)
				except Exception as e:
					print ("swift.upload: ",e)
				try:
					for ri in r:
						if not ri['success']:
							print ('object upload error')
							print ('error:%s',r['error'])
				except Exception as e:
					print ('ri[''] failed, ',e)
			except Exception as e:
				print ("object create error:",e)
	except Exception as e:
		print ("swift object create initialize failed:",e)
def swift_object_download(container, sciobj_name, sciobj_dst=None, dtype=None, options=None):
	conn= swift_connect()
	container.replace('/','\\')
	res, obj=conn.get_object(container,sciobj_name)
	if dtype==None:
		dtype='int32'
	sciobj_dst=numpy.frombuffer(obj,dtype=dtype)
	return sciobj_dst
def swift_object_get(container, sciobj_name,options=None):
	try:
		with  SwiftService(options = options) as swift:
			print ("container:%s, obj:%s"%(container,sciobj_name))
			for r in swift.download(container=container,objects=[sciobj_name]):
				if r['success']:
					print ('Downloaded object:%s'%(sciobj_name))
					print r
				else:
					print ('for r error:',r)
	except Exception as e:
		print ('in swift object get:error:',e)

def sci_swift_object(sciobj_name, sciobj_source,options=None):
	"""
	Construct swift object

	Input:
		Object name
		Object source
		Options
	Output:
		Swift Object
	"""
	from io import BytesIO
	try:
		objs = [SwiftUploadObject(
			BytesIO(sciobj_source), sciobj_name
			,options=options)]
		return objs
	except Exception as e:
		print ('swift object construct failed:',e)


def swift_object_open(container, sciobj_name, options=None):
	if 1==1: 
		return 1 
	else:
		return -1

def swift_metadata_create(container, sciobj_name, sciobj_metadata,options=None):
	"""
	Post metadata to existing object, by first GET existing metadata then concatenating two meta

	Input: 
		Container name
		Object name
		Metadata object
	Output:
		None
	TODO: GET existing metadata
	"""
	#first retrieve existing metadata, otherwise, meta post will destroy existing one
	with SwiftService(options = options) as swift:
		try:
			post_options = {"meta": sciobj_metadata}
			r=swift.post(container=container, objects=[sciobj_name],options=post_options)
			try:
				for ri in r:
					if not ri['success']:
						print ('object upload error')
						print ('error:%s',r['error'])
			except Exception as e:
				print ('ri[''] failed, ',e)
		except Exception as e: 
			print ('stat error: ',e)

def swift_metadata_get(container,sciobj_name, options=None):
	"""
	Use swift.stat to query the metadata of the object 
	Input: 
		Container name
		Object name
		options
	Output:
		Dictionary of metadata, e.g., dims, type, ndim. 
	"""
	with SwiftService(options = options) as swift:
		try:
			meta_data={}
			r=swift.stat(container=container, objects=[sciobj_name])
			for item in r:
				if item['success']:
					meta_data['dims']=item['headers']['x-object-meta-dims']
					meta_data['type']=item['headers']['x-object-meta-type']
					meta_data['ndim']=item['headers']['x-object-meta-ndim']
			return meta_data
		except Exception as e:
			#print ('stat error:',e)
			pass
