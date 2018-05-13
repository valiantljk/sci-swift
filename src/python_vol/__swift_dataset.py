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
					#print ("getting metadata of obj:%s failed"%sciobj_name)
					pass
				#print ('in object create:post_options:',post_options)
				if sciobj_source is None:
					#print ("object source is None") 
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
					#print ("object source is not None")
					#objs = sci_swift_object(sciobj_name, sciobj_source,options=post_options)
					objs = sci_swift_object(sciobj_name, sciobj_source)
					#print('in swift.upload, obj name is %s:',sciobj_name)
					#print ('in swift.upload, swift objs:',objs)
				#print ('in swift.upload:')
				try:
					#print('geting old meta from container:%s, with obj:%s'%(container,sciobj_name))
					#meta_old=swift_metadata_get(container=container,sciobj_name=sciobj_name)
					#print ("=========objects:")
					#print (objs)
					#print ("=========objects end")
					#print ("**************swift.upload start")
					r=swift.upload(container=container,objects=objs)
					#print('updating with old meta')
					#print ("**************swift.upload end")
					#print(meta_old)
					#post_options={"meta": meta_old}
					#r1=swift_metadata_create(container, sciobj_name, sciobj_metadata=sci_obj_meta)
					#print ("r1:\n",r1)
					#print ("^^^^^^^^^^^^^^Checking new metadata 1")
					#swift.post(container=container, objects=[sciobj_name],options=post_options)
					#meta_new=swift_metadata_get(container=container,sciobj_name=sciobj_name)
					#print("^^^^^^^^^^^^^^^Checking new metadata end 1")
					#print ('meta new 1:,',meta_new)
					#print ('swift.upload, r:',r)
				except Exception as e:
					print ("swift.upload: ",e)
				#print ('finish swift.upload r:',r)
                                #print ("^^^^^^^^^^^^^^Checking new metadata 1.5")
                                #swift.post(container=container, objects=[sciobj_name],options=post_options)
                                #meta_new=swift_metadata_get(container=container,sciobj_name=sciobj_name)
                                #print("^^^^^^^^^^^^^^^Checking new metadata end 1.5")
                                #print ('meta new 1.5:,',meta_new)
				#print("++++++++++++++Try print r:")
				try:
					for ri in r:
						if not ri['success']:
							print ('object upload error')
							print ('error:%s',r['error'])
						#else:
						#	print ('HAHAHAHAH object upload ok')
						#	print ('in swift.upload, ri:',ri)
				except Exception as e:
					print ('ri[''] failed, ',e)
				#print('++++++++++++++End print r')
				#print ("^^^^^^^^^^^^^^Checking new metadata 2")
                                #swift.post(container=container, objects=[sciobj_name],options=post_options)
                                #meta_new=swift_metadata_get(container=container,sciobj_name=sciobj_name)
                                #print("^^^^^^^^^^^^^^^Checking new metadata end 2")
                                #print ('meta new 2:,',meta_new)
			except Exception as e:
				print ("object create error:",e)
	except Exception as e:
		print ("swift object create initialize failed:",e)
def swift_object_download(container, sciobj_name, sciobj_dst=None, dtype=None, options=None):
        #print ('-----------------------Testing swift object download')
	#first try download the binary blob from swift store
	conn= swift_connect()
	container.replace('/','\\')
	#print("container:(%s),obj:(%s)"%(container,sciobj_name))
	res, obj=conn.get_object(container,sciobj_name)
	if dtype==None:
		dtype='int32'
	sciobj_dst=numpy.frombuffer(obj,dtype=dtype)
	#numpy.copyto(sciobj_dst,obj,dtype=dtype)
	#print sciobj_dst		
	#print('-----------------------Testing swift object download END')
	#swift_object_get(container=container,sciobj_name=sciobj_name,options=post_options)
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
	if 1==1: #TODO: query swift store to see if object exists
		#TODO: retrieve object shape and type info via swift metadata query
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
			#print("in metadata create: container=%s,object=%s"%(container,sciobj_name))
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
			#print ("in swift.post, r=",r)
			try:
				for ri in r:
					if not ri['success']:
						print ('object upload error')
						print ('error:%s',r['error'])
					#else:
					#	print ('object upload ok')
					#	print ('ri:',ri)
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
			#print('stat:container:%s,obj:%s'%(container,sciobj_name))
			r=swift.stat(container=container, objects=[sciobj_name])
			for item in r:
				if item['success']:
					#print ("in metadata_get, success, item:")
					#print(item)
					meta_data['dims']=item['headers']['x-object-meta-dims']
					meta_data['type']=item['headers']['x-object-meta-type']
					meta_data['ndim']=item['headers']['x-object-meta-ndim']
					#print('got meta:',meta_data)
					#header_data[item['object']]=item['headers']['meta']
			#print ('got meta2:',meta_data)
				#else:
				#	print ('failed,item:',item)
			return meta_data
		except Exception as e:
			print ('stat error:',e)
			#print ('r:',r)

