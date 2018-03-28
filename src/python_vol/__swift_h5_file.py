# https://docs.openstack.org/python-swiftclient/latest/service-api.html
# Modified by Jialin Liu
# Date Oct 5 2017
# This script can test the write performance on lustre and openstack-swift
# It first generates a HDF5 file in memory with a 2D float dataset,
# then writes that directly to targeted storage

from __swift_core import *

'''
A HDF5 file is a container in the Swift world # we think this design is conceptually genius.
Authentication in swift is corresponding to the posix file's unix permission.

'''
def swift_file_commit(_opts, container, obj_name, obj_source=None):
	with SwiftService(options=_opts) as swift, OutputManager() as out_manager:
		try:
			if obj_source is None:
				obj_source = obj_name  # file path
				# check if file exists
				if (not os.path.exists(obj_source)):
					print('File %s does not exist' % obj_source)
					exit()
			objs = [SwiftUploadObject(obj_source, obj_name)]  # (source, object name) --> (value,key)
			for r in swift.upload(container, objs):
				if r['success']:
					if 'object' in r:
						print(r['object'])
					elif 'for_object' in r:
						print(
							'%s segment %s' % (r['for_object'],
											   r['segment_index'])
						)
				else:
					error = r['error']
					if r['action'] == "create_container":
						logger.warning(
							'Warning: failed to create container '
							"'%s'%s", container, error
						)
					elif r['action'] == "upload_object":
						logger.error(
							"Failed to upload object %s to container %s: %s" %
							(container, r['object'], error)
						)
					else:
						logger.error("%s" % error)

		except SwiftError as e:
			logger.error(e.value)
