# https://docs.openstack.org/python-swiftclient/latest/service-api.html
# Modified by Jialin Liu
# Date Mar 1 2018
# This script can test the write performance on lustre and openstack-swift
# It first generates a HDF5 file in memory with a 2D float dataset, 
# then writes that directly to targeted storage

from __swift_core import *
'''
An HDF5 Dataset is mapped one or many swift object(s)
An HDF5 Group is mapped to one swift object
An HDF5 Attribute is associated to an Swift object as metadata, paired into (key,value), extendable
'''

def swift_dataset_write(_opts, path, fname, dname, dimx, dimy):
	try:
		fx = tables.open_file(h5fname, 'a', driver='H5FD_CORE', driver_core_backing_store=0)
	except Exception as e:
		print('creating file in test swift fails')
		exit()
		'''
		Do an implicit swift.upload for this empty file
		swift_file_commit(container,h5fname)
		Fill in the file with real data
		[future] do an implicit swift.download
		for now, just read it locally
	'''
	try:
		arr = np.ndarray(shape=(dimx, dimy), dtype=float)
		arr.fill(3.1415926)
	# write will implicitly happens by default, we want to bypass this, and directly talk with swift
	except Exception as e:
		print('creating dataset in swift test fails')
		exit()

	start = time.time()
	# get image of hdf5 file
	try:
		dset = fx.create_array(fx.root, dname, arr)
		image = fx.get_file_image()
	except Exception as e:
		print('geting h5 file image fails')
		exit()
	# convert image to file-like object
	try:
		fobj = io.BytesIO(image)
	except Exception as e:
		print('geting file-like object fails')
		exit()
	start2 = time.time()
	swift_file_commit(_opts, container, h5fname, fobj)
	end = time.time()

	print('[Swift] Writing cost %.2f second' % (end - start))
	print('[Swift] 1. File image/array creating cost %.2f second' % (start2 - start))
	print('[Swift] 2. Uploading cost %.2f second' % (end - start2))
	try:
		fx.close()
	except Exception as e:
		print('file closing fails in swift test')
		exit()


def lustre_dataset_write(path, fname, dname, dimx, dimy):
	if path == 'scratch':
		fdir = os.environ['SCRATCH']
		ftestdir = fdir + '/swift-lustre-test/'
		try:
			os.mkdirs(ftestdir)
		except:
			pass
		fname = ftestdir + fname
	if os.path.exists(fname):
		print("%s exists in lustre write test\n" % fname)
		exit()
	arr = np.ndarray(shape=(dimx, dimy), dtype=float)
	arr.fill(3.1415926)
	start = time.time()
	try:
		f = h5py.File(fname, 'a', driver='core')
		dset = f.create_dataset(dname, (dimx, dimy), data=arr)  # no write with core driver
		f.close()  # writes happens, but probably in page buffer
	except Exception as e:
		print('lustre write fails in file write')
		print(e)
		exit()
	end = time.time()
	print('[Lustre] Writing cost %.2f second' % (end - start))

'''
if __name__ == '__main__':
	_opts = {'object_uu_threads': 10, 'segment_size': 1048576}
	if (len(argv) < 6):
		print("Parameters: container/path name, filename, dname, dimx, dimy")
		exit()
	h5fname = argv[2]
	container = argv[1]
	dimx = int(argv[4])
	dimy = int(argv[5])
	dname = argv[3]

	swift_dataset_write(_opts, container, h5fname, dname, dimx, dimy)
	lustre_dataset_write(container, h5fname, dname, dimx, dimy)
'''