# HDF5 Swift VOL in Python
# Mar 18 2018
# Jialin Liu
# LBNL/NERSC

import numpy
import sys
from __swift_file import swift_container_create
from __swift_file import swift_container_open
from __swift_dataset import swift_object_create
from __swift_dataset import swift_object_open
from __swift_dataset import swift_metadata_create
from __swift_dataset import swift_metadata_get
from __swift_dataset import swift_object_download

print ('in python_vol:_swift.py, numpy version is:%s'%(numpy.version.version))

def Takesecond(elem):
	return elem[1]

def Takefirst(elem):
	return elem[0]


class H5PVol:
	dt_types = {0: "int16", 1: "int32", 2: "float32", 3: "float64"}
	obj_curid = 1  # PyLong_AsVoidPtr can not convert 0 correctly
	obj_list = {}
	'''
	Dictionary for various h5py objects, e.g., groups, datasets, links, etc.
	starting obj is reserved for HDF5 file handle, with index 0 as its key
	{0:hdf5_handle, ...}
	'''

	def H5VL_python_file_create(self, name, flags, fcpl_id, fapl_id, dxpl_id, req, ipvol):
		if (ipvol == 0):
			try:
				self.obj_curid = 1
				self.obj_list = {}  # reset
				# create a container for this file, container name = file name
				swift_container_create(name)
				# save container_name in the runtime dictionary, at user level, code can get back container by the returned id,
				self.obj_list[self.obj_curid] = name
				curid = self.obj_curid
				self.obj_curid = curid + 1
				return curid
			except Exception as e:
				print('file create failed in python with error: ', e)
				return -1
		else:
			print("%d py vol is not implemented" % ipvol)
			return -1

	def H5VL_python_file_open(self, name, flags, fapl_id, dxpl_id, req, ipvol):
		if (ipvol == 0):
				try:
					self.obj_curid = 1  # reset
					self.obj_list = {}  # reset
					# Probably just check if (metadata, container validation, file)
					# save file name--> container name in the runtime dictionary.
					# at user level, dataset api, etc, can get back container name for read/write (get/put)
					container_name = name
					container_id = swift_container_open(container_name)
					if container_id == 1:  # file exists
						self.obj_list[self.obj_curid] = container_name
						curid = self.obj_curid
						self.obj_curid = curid + 1
						return curid
					else:
						print("returned -1 in python file open")
						return -1
				except Exception as e:
					print('file create failed in python with error: ', e)
					return -1
		else:
			print("%d py vol is not implemented" % ipvol)
			return -1

	def H5VL_python_dataset_open(self, obj_id, loc_params, name, dapl_id, dxpl_id, req):
		try:
			container_name = self.obj_list[obj_id]
			full_path = container_name + "/" + name
			z = full_path.replace("/", "\\")
			container_name = z[:z.find(z.split('\\')[-1]) - 1]
			obj_name = z.split('\\')[-1]
			metadata = swift_metadata_get(
				container=container_name, sciobj_name=obj_name)
			curid = self.obj_curid
			self.obj_list[curid] = full_path
			self.obj_curid = curid + 1
			return curid
		except Exception as e:
			print('retrieve obj failed in python dataset open:', e)
			return -1

	def H5VL_python_file_close(self, file_id, dxpl_id, req):
		try:
			if (len(self.obj_list) == 0):
				return 1
			file_obj = self.obj_list[file_id]
			if file_obj == None:
				return 1
			else:
				self.obj_list = {}  # empty all ojects
				self.obj_curid = 1
				return 1
		except Exception as e:
			print('file close failed in python with error:', e)
			return -1

	def H5VL_python_dataset_close(self, dset_id, dxpl_id, req):
		try:
			if (len(self.obj_list) == 0):
				return 1
			dset_obj = self.obj_list[dset_id]
			if dset_obj == None:
				return 1
			else:
				del (self.obj_list[dset_id])
				return 1
		except Exception as e:
			print('dataset close failed in python with error:', e)
			return -1

	def H5VL_python_group_create(self, obj_id, loc_params, name, gcpl_id, gapl_id, dxpl_id, req):
		try:
			# print ('in python group create, obj is ',obj_id)
			# now the grp_parent_obj is the parent container's name
			grp_parent_obj = self.obj_list[obj_id]
			try:
				# grp_obj=grp_parent_obj.create_group(name)
				new_container_name = grp_parent_obj + '\\' + name
				# print ("new container name:%s"%new_container_name)
				swift_container_create(new_container_name)
				curid = self.obj_curid
				self.obj_list[curid] = new_container_name  # insert new object
				empty_sciobj_name = new_container_name
				# put this object into parent container for tracking purpose.
				swift_object_create(container=grp_parent_obj, sciobj_name=empty_sciobj_name)
				self.obj_list[curid + 1] = grp_parent_obj + '\\' + empty_sciobj_name
				self.obj_curid = curid + 2  # update current head, points to future object
				# #print ("------- PYTHON H5Gcreate OK")
				return curid  # return new obj's id
			except Exception as e:
				print('group create in python failed with error:', e)
				return -1
		except Exception as e:
			print('retrieve obj failed in python group create')
			return -1

	def H5VL_python_dataset_create(self, obj_id, loc_params, name, dcpl_id, dapl_id, dxpl_id, req, ndims, pytype, dims,
				maxdims):
		try:
			dst_parent_obj = self.obj_list[obj_id]
			name = name.replace("/", "\\")
			z = dst_parent_obj + '\\' + name
			dst_container_name = z[:z.find(z.split('\\')[-1]) - 1]
			dst_obj_name = z.split('\\')[-1]
			try:
				sci_obj_source = numpy.empty(
				dims, dtype=self.dt_types[pytype], order='C')
				swift_object_create(container=dst_container_name, sciobj_name=dst_obj_name)
				sci_obj_meta = {}
				sci_obj_meta['type'] = str(self.dt_types[pytype])
				sci_obj_meta['dims'] = numpy.array_str(dims)
				sci_obj_meta['ndim'] = str(ndims)
				r1 = swift_metadata_create(container=dst_container_name, sciobj_name=dst_obj_name, sciobj_metadata=sci_obj_meta)
				# TODO: append shape, type info into object's metadata
				curid = self.obj_curid
				# insert new object#TODO: need full name or not? April Fool Day Puzzle
				self.obj_list[curid] = z
				self.obj_curid = curid + 1  # update current index
				return curid
			except Exception as e:
				print('dataset create in python failed with error: ', e)
				return -1
		except Exception as e:
			print('retrieve obj failed in python dataset create:', e)
			return -1
	def H5VL_python_dt_info(self, obj_id):
		try:
			z = self.obj_list[obj_id]
			z = z.replace("/", "\\")
			obj_name = z.split("\\")[-1]
			container_name = z[:z.find(z.split('\\')[-1]) - 1]
			metadata = swift_metadata_get(container=container_name, sciobj_name=obj_name)
			m = metadata['dims']
			m = m[1:len(m) - 1]
			dims = numpy.fromstring(m, dtype=int, sep=' ')
			ndims = int(metadata['ndim'])
			for k, v in self.dt_types.items():
				if v == metadata['type']:
					dt = k
					axx = list()
					axx.append(ndims)
					axx.append(dt)
					axx = axx + list(dims)
					axx = numpy.asarray(axx, dtype='int64')
					return axx
		except Exception as e:
			return None
			pass

	def H5VL_python_dataset_write(self, obj_id, mem_type_id, mem_space_id, file_space_id, plist_id, buf, req):
	# print ("-----------------ENTER Dataset Write-----------------")
		try:
			dst_parent_obj = self.obj_list[obj_id]
			z = dst_parent_obj
			dst_container_name = z[:z.find(z.split('\\')[-1]) - 1]
			dst_object_name = z.split('\\')[-1]
			try:
				metadata = swift_metadata_get(container=dst_container_name, sciobj_name=dst_object_name)
				if (req >= 0):
					dst_object_name = dst_object_name + '_' + str(req)  # data of this hyperslab block, ended with start offset
				if (req == -2):
					dst_object_name = dst_object_name + '_' + 'meta'  # meta of this hyperslab block
				if (req == -3):
					dst_object_name = dst_object_name + '_' + 'simple'  # no hyperslab enabled
				swift_object_create(container=dst_container_name, sciobj_name=dst_object_name, sciobj_source=buf)
				sci_obj_meta = {}
				if (sys.getsizeof(self.obj_list) % 52428800 == 0):
					print("objects in memory is %.2f MB now" %
						(5 * sys.getsizeof(self.obj_list) / 5242880))
				curid = self.obj_curid
				return curid
			except Exception as e:
				print('dataset write in python failed with error: ', e)
				return -1
		except Exception as e:
			print('retrieve obj failed in python dataset write: ', e)
			return -1

	def H5VL_python_dataset_read(self, obj_id, mem_type_id, mem_space_id, file_space_id, plist_id, buf, req):
		'''
		Python wrapper for H5VL_dataset_read, return numpy array, interperated as object in C
		Input:
	    	same with H5VL_python_dataset_read at C layer
		Output:
	    	numpy array
		'''
		try:
			dst_parent_obj = self.obj_list[obj_id]
			z = dst_parent_obj.replace("/", "\\")
			dst_container_name = z[:z.find(z.split('\\')[-1]) - 1]
			dst_object_name = z.split('\\')[-1]
			try:
				metadata = swift_metadata_get(container=dst_container_name, sciobj_name=dst_object_name)
				curtype = str(metadata['type'])
				x = swift_object_download(container=dst_container_name, sciobj_name=dst_object_name,
					dtype=curtype).reshape(buf.shape)
				return x
			except Exception as e:
				print('dataset read in python failed with error: ', e)
		except Exception as e:
			print('retrieve obj failed in python dataset read')
			return -1

	def H5VL_python_group_close(self, grp_id, dxpl_id, req):
		try:
			if (len(self.obj_list) == 0):
				return 1
			grp_obj = self.obj_list[grp_id]
			if grp_obj == None:
				print("Python Group Obj is none")
				return 1
			else:
				del (self.obj_list[grp_id])
				return 1
		except Exception as e:
			print('dataset close failed in python with error:', e)
			return -1

	def H5VL_python_dstobj_scan(self, obj_id, global_meta, meta_offlen, req):
		try:
			dst_parent_obj = self.obj_list[obj_id]
			z = dst_parent_obj.replace("/", "\\")
			dst_container_name = z[:z.find(z.split('\\')[-1]) - 1]
			dst_object_name = z.split('\\')[-1]
			object_mappings, meta_offlen_list = self.Meta_to_Object_Mappings(
			global_meta, meta_offlen)
			object_selected = self.Object_Binary_Search(
			object_mappings, meta_offlen_list)
			con_data = self.dst_oneshot_io(object_selected, obj_id, dst_object_name)
			return con_data
		except Exception as e:
			pass

	def Dataset_object_internal_read(self, obj_id, dstobj_name):
		'''
		Python wrapper for H5VL_dataset_read, return numpy array, interperated as object in C
		Input:
	    		same with H5VL_python_dataset_read at C layer
		Output:
	    		numpy array
		'''
		try:
			dst_parent_obj = self.obj_list[obj_id]
			z = dst_parent_obj.replace("/", "\\")
			dst_container_name = z[:z.find(z.split('\\')[-1]) - 1]
			dsobj_name = z.split('\\')[-1]
			try:
				metadata = swift_metadata_get(
				container=dst_container_name, sciobj_name=dstobj_name)
				curtype = str(metadata['type'])
				x = swift_object_download(
				container=dst_container_name, sciobj_name=dstobj_name, dtype=curtype)
				return x
			except Exception as e:
				print('dataset read in python failed with error: ', e)
		except Exception as e:
			print('retrieve obj failed in internal read:', e)
			return -1

	def dst_oneshot_io(self, objsel, obj_id, dstparent):
		obj_data = {}  # record the data that has completed I/O, so no need to read again
		all_objs = {v[0] for k in objsel.keys() for v in objsel[k]}
		all_objs = list(set(all_objs))
		all_objs.sort()
		for iobj in all_objs:
			dstname = dstparent + '_' + str(iobj)
			if dstname not in obj_data:
				ddt = self.Dataset_object_internal_read(obj_id, dstname)
				obj_data[dstname] = ddt
		# Construct a contiguous array
		data = numpy.empty(shape=(0))
		for ol in objsel.keys():
			meta_list = objsel[ol]
			for k in range(len(meta_list)):
				cur_meta = meta_list[k]
				# get object data by objectid i.e., offset
				cur_obj = obj_data[dstparent + '_' + str(cur_meta[0])]
				data_start = cur_meta[3]  # -cur_meta[1]
				data_length = cur_meta[5] - cur_meta[4] + 1
				data_cur = cur_obj[data_start:(data_start + data_length)]
				if data.size == 0:
					data = data_cur
				else:
					data = numpy.append(data, data_cur)
		return data

	def Meta_to_Object_Mappings(self, global_meta, meta_offlen):
	#:param global_meta:
	#:return: object_mapping dictionary, [objid, offset, length, off_in_obj]
	# obj_name=dset_object_name+'_'+ str(ol[0]) # dset object name with offset as uinque tag
	# Skip first three elements, which is total length of this array, min offset, max offset.
		i = 3
		object_mappings = list()
		number_obj = 0
		while i < global_meta[0]:
			cur_seq_len = global_meta[i]
			j = i + 3  # points j to the first (offset, length) pair
			cur_seq_len -= 3  # removed min and max offset
			off_in_obj = 0
			while (cur_seq_len > 0):  # append other offset lengths
				ol = [global_meta[i + 1], global_meta[j],
				global_meta[j + 1], off_in_obj]
				# calculate the offset of next byte sequence within the object
				off_in_obj += global_meta[j + 1]
				cur_seq_len -= 2
				j += 2
				object_mappings.append(ol)
			i = i + global_meta[i]  # jump to next object
			number_obj += 1
		object_mappings.sort(key=Takesecond)  # sort list by offset
		# print ('sorted global off/len pair:',object_mappings) # added for test
		len_mt = meta_offlen[0]
		meta_offlens = list()
		i = 1
		while i < len_mt:
			ol = [meta_offlen[i], meta_offlen[i + 1]]
			i += 2
			meta_offlens.append(ol)
		meta_offlens.sort(key=Takefirst)
		return object_mappings, meta_offlens

	def Object_Binary_Search(self, object_mappings, meta_offlen):
		object_selected = {}
		l = 0
		r = len(object_mappings) - 1
		for imeta in meta_offlen:
			object_selected[imeta[0]] = self.obj_binary_search(object_mappings, l, r, imeta)
		return object_selected

	def obj_binary_search(self, objm, l, r, imeta):
		if (r >= l):
			mid = int(l + (r - l) / 2)
			olap = self.obj_overlap(objm[mid], imeta)
			if (olap == 0):
				# print('overlaping detected for %s'%imeta)
				return self.obj_following(objm, mid, imeta)
			elif (olap == -1):
				# print('looking left for %s'%imeta)
				return self.obj_binary_search(objm, l, mid - 1, imeta)
			elif (olap == 1):
				# print('looking right for %s'%imeta)
				return self.obj_binary_search(objm, mid + 1, r, imeta)

	def obj_overlap(self, objm, imeta):
		objm_l = objm[1]
		objm_r = objm[2] + objm_l - 1
		imeta_l = imeta[0]
		imeta_r = imeta_l + imeta[1] - 1
		# case 1: -----
		#        --------
		if (objm_l >= imeta_l and objm_r <= imeta_r):
			# print ('case 1')
			return 0
		# case 2: ------
		#           ----
		elif (objm_l <= imeta_l and objm_r >= imeta_l):
			# print ('case 2')
			return 0
		# case 3:   ------
		#        ------
		elif (objm_l >= imeta_l and objm_l <= imeta_r):
			# print('case 3')
			return 0
		elif (objm_r < imeta_l):
			# print('on the left')
			return 1
		elif (objm_l > imeta_r):
			# print('on the right')
			return -1
		else:
			print('not sure')

	def obj_following(self, objm, found_index, imeta):
		result = list()
		imeta_offset_min = imeta[0]
		imeta_offset_max = imeta[1] + imeta_offset_min - 1
		objm_rich = numpy.append(objm[found_index], max(imeta_offset_min, objm[found_index][1]))
		objm_rich = numpy.append(objm_rich, min(imeta_offset_max, objm[found_index][1] + objm[found_index][2] - 1))
		result.append(objm_rich)
		# search on left
		left_start = found_index - 1
		right_start = found_index + 1
		while (left_start >= 0):
			if (self.obj_overlap(objm[left_start], imeta) == 0):
				objm_rich = numpy.append(objm[left_start], min(imeta_offset_min, objm[left_start][1]))
				objm_rich = numpy.append(objm_rich,min(imeta_offset_max, objm[left_start][1] + objm[left_start][2] - 1))
				result.insert(0, objm_rich)
				left_start -= 1
			else:
				break
		# search on the right
		while (right_start < len(objm)):
			if (self.obj_overlap(objm[right_start], imeta) == 0):
				objm_rich = numpy.append(objm[right_start], min(imeta_offset_min, objm[right_start][1]))
				objm_rich = numpy.append(objm_rich,min(imeta_offset_max, objm[right_start][1] + objm[right_start][2] - 1))
				result.append(objm_rich)
				right_start += 1
			else:
				break
		return result
