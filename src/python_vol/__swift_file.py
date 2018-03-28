from __swift_connect import swift_connect 
'''
A HDF5 file is a container in the Swift world # we think this design is conceptually genius.
Authentication in swift is corresponding to the posix file's unix permission.

'''
def swift_file_create(file_name):
	try: 
		conn= swift_connect()
		container=file_name
		conn.put_container(container)
	except Exception as e:
		print ("file create failed in swift")
		print (e)
def swift_file_open(file_name):
	try: 
		conn= swift_connect()
		container_name=file_name
		resp_headers, containers = conn.get_account()
		#print("Response headers: %s" % resp_headers)
		#print (containers)
		for icontainer in containers:
			if (icontainer['name']==container_name):
				return 1 # not found
		return -1
	except Exception as e:
		print ("file open failed in swift")
		print (e)
