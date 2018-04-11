dir=../src/
CC = gcc 
CFLAGS = -c -fno-strict-aliasing -Wmissing-include-dirs -O2 -DNDEBUG -fwrapv -O3 -Wall -Wstrict-prototypes -I/usr/common/software/python/2.7-anaconda-4.4/include/python2.7 -isystem/usr/common/software/python/2.7-anaconda-4.4/lib/python2.7/site-packages/numpy/core/include  -Wno-unused-variable  -Wno-strict-prototypes -Wno-cpp -I//usr/common/software/hdf5-parallel/hdf5-vol/docker/hdf5/src
LDFLAGS = -L/usr/common/software/python/2.7-anaconda-4.4/lib -lpython2.7 -lpthread -ldl -lutil -lm -Xlinker -export-dynamic #-Wl,--verbose

#ALL: swift test_pylong 
ALL: HDF5_File_create HDF5_File_open HDF5_Objects_create HDF5_Objects_open HDF5_Dataset_read HDF5_Dataset_write

swift: swift.o test_swift.o
	$(CC) swift.o test_swift.o  $(LDFLAGS) -o swift
swift.o: ${dir}/swift.c
	$(CC) $(CFLAGS) ${dir}/swift.c -o swift.o
test_swift.o: test_swift.c
	$(CC) $(CFLAGS) test_swift.c -o test_swift.o

HDF5_Objects_create: python_vol.o test_python_vol_HDF5_Objects_create.o
	h5pcc -g test_python_vol_HDF5_Objects_create.o python_vol.o $(LDFLAGS) -o HDF5_Objects_create

HDF5_Objects_open: python_vol.o test_python_vol_HDF5_Objects_open.o
	h5pcc -g test_python_vol_HDF5_Objects_open.o python_vol.o $(LDFLAGS) -o HDF5_Objects_open

HDF5_File_create: python_vol.o test_python_vol_HDF5_File_create.o
	h5pcc -g test_python_vol_HDF5_File_create.o python_vol.o $(LDFLAGS) -o HDF5_File_create

HDF5_File_open: python_vol.o test_python_vol_HDF5_File_open.o
	h5pcc -g test_python_vol_HDF5_File_open.o python_vol.o $(LDFLAGS) -o HDF5_File_open

test_python_vol_HDF5_File_create.o: test_python_vol_HDF5_File_create.c 
	h5pcc -g $(CFLAGS) test_python_vol_HDF5_File_create.c -o test_python_vol_HDF5_File_create.o

test_python_vol_HDF5_File_open.o: test_python_vol_HDF5_File_open.c
	h5pcc -g $(CFLAGS) test_python_vol_HDF5_File_open.c -o test_python_vol_HDF5_File_open.o

test_python_vol_HDF5_Objects_create.o: test_python_vol_HDF5_Objects_create.c
	h5pcc -g $(CFLAGS) test_python_vol_HDF5_Objects_create.c -o test_python_vol_HDF5_Objects_create.o

test_python_vol_HDF5_Objects_open.o: test_python_vol_HDF5_Objects_open.c
	h5pcc -g $(CFLAGS) test_python_vol_HDF5_Objects_open.c -o test_python_vol_HDF5_Objects_open.o


HDF5_Dataset_read: python_vol.o test_python_vol_HDF5_Dataset_read.o
	h5pcc -g test_python_vol_HDF5_Dataset_read.o python_vol.o $(LDFLAGS) -o HDF5_Dataset_read

test_python_vol_HDF5_Dataset_read.o: test_python_vol_HDF5_Dataset_read.c
	h5pcc -g $(CFLAGS) test_python_vol_HDF5_Dataset_read.c -o test_python_vol_HDF5_Dataset_read.o

HDF5_Dataset_write: python_vol.o test_python_vol_HDF5_Dataset_write.o
	h5pcc -g test_python_vol_HDF5_Dataset_write.o python_vol.o $(LDFLAGS) -o HDF5_Dataset_write

test_python_vol_HDF5_Dataset_write.o: test_python_vol_HDF5_Dataset_write.c
	h5pcc -g $(CFLAGS) test_python_vol_HDF5_Dataset_write.c -o test_python_vol_HDF5_Dataset_write.o

python_vol.o: ${dir}/python_vol.c
	h5pcc -g $(CFLAGS) ${dir}python_vol.c -o python_vol.o
test_pylong: test_pylong.o
	$(CC) test_pylong.o  $(LDFLAGS) -o test_pylong
test_pylong.o: test_pylong.c
	$(CC) $(CFLAGS) test_pylong.c -o test_pylong.o        
clean:
	rm HDF5_File_create HDF5_File_open HDF5_Objects_create HDF5_Objects_open HDF5_Dataset_read HDF5_Dataset_write 

#	rm *.o HDF5_* ../src/*.pyc *.pyc HDF5_File_create HDF5_File_open HDF5_Objects_create HDF5_Objects_open HDF5_Dataset_read HDF5_Dataset_write
