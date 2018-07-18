dir=../src/
CC = gcc 
CFLAGS = -c -fno-strict-aliasing -Wmissing-include-dirs -O2 -DNDEBUG -fwrapv -O3 -Wall -Wstrict-prototypes -I/usr/common/software/python/2.7-anaconda-4.4/include/python2.7 -isystem/usr/common/software/python/2.7-anaconda-4.4/lib/python2.7/site-packages/numpy/core/include  -Wno-unused-variable  -Wno-strict-prototypes -I/usr/common/software/hdf5-parallel/hdf5-1.9.234/src
LDFLAGS = -L/usr/common/software/python/2.7-anaconda-4.4/lib -lpython2.7 -lpthread -ldl -lutil -lm -Xlinker -export-dynamic #-Wl,--verbose

#ALL: swift test_pylong 
ALL: HDF5_file_create HDF5_file_open HDF5_objects_create HDF5_objects_open HDF5_dataset_read HDF5_dataset_write HDF5_file_create_parallel HDF5_objects_create_parallel HDF5_dataset_write_parallel	
#VPIC_IO

swift: swift.o test_swift.o
	$(CC) swift.o test_swift.o  $(LDFLAGS) -o swift
swift.o: ${dir}/swift.c
	$(CC) $(CFLAGS) ${dir}/swift.c -o swift.o
test_swift.o: test_swift.c
	$(CC) $(CFLAGS) test_swift.c -o test_swift.o

#VPIC_IO: python_vol.o vpicio_uni_h5.o
#	h5pcc -g vpicio_uni_h5.o python_vol.o $(LDFLAGS) -o VPIC_IO
HDF5_objects_create: python_vol.o HDF5_objects_create.o
	h5pcc -g HDF5_objects_create.o python_vol.o $(LDFLAGS) -o HDF5_objects_create

HDF5_objects_open: python_vol.o HDF5_objects_open.o
	h5pcc -g HDF5_objects_open.o python_vol.o $(LDFLAGS) -o HDF5_objects_open

HDF5_file_create: python_vol.o HDF5_file_create.o
	h5pcc -g HDF5_file_create.o python_vol.o $(LDFLAGS) -o HDF5_file_create

HDF5_file_create_parallel: python_vol.o HDF5_file_create_parallel.o
	h5pcc -g HDF5_file_create_parallel.o python_vol.o $(LDFLAGS) -o HDF5_file_create_parallel

HDF5_objects_create_parallel: python_vol.o HDF5_objects_create_parallel.o
	h5pcc -g HDF5_objects_create_parallel.o python_vol.o $(LDFLAGS) -o HDF5_objects_create_parallel

HDF5_dataset_write_parallel: python_vol.o HDF5_dataset_write_parallel.o
	h5pcc -g HDF5_dataset_write_parallel.o python_vol.o $(LDFLAGS) -o HDF5_dataset_write_parallel

HDF5_file_open: python_vol.o HDF5_file_open.o
	h5pcc -g HDF5_file_open.o python_vol.o $(LDFLAGS) -o HDF5_file_open

#vpicio_uni_h5.o: vpicio_uni_h5.c
#	h5pcc -g $(CFLAGS) vpicio_uni_h5.c -o vpicio_uni_h5.o

HDF5_file_create.o: HDF5_file_create.c 
	h5pcc -g $(CFLAGS) HDF5_file_create.c -o HDF5_file_create.o

HDF5_file_open.o: HDF5_file_open.c
	h5pcc -g $(CFLAGS) HDF5_file_open.c -o HDF5_file_open.o

HDF5_objects_create.o: HDF5_objects_create.c
	h5pcc -g $(CFLAGS) HDF5_objects_create.c -o HDF5_objects_create.o

HDF5_objects_open.o: HDF5_objects_open.c
	h5pcc -g $(CFLAGS) HDF5_objects_open.c -o HDF5_objects_open.o

HDF5_file_create_parallel.o: HDF5_file_create_parallel.c
	h5pcc -g $(CFLAGS) HDF5_file_create_parallel.c -o HDF5_file_create_parallel.o

HDF5_objects_create_parallel.o: HDF5_objects_create_parallel.c
	h5pcc -g $(CFLAGS) HDF5_objects_create_parallel.c -o HDF5_objects_create_parallel.o

HDF5_dataset_write_parallel.o: HDF5_dataset_write_parallel.c
	h5pcc -g $(CFLAGS) HDF5_dataset_write_parallel.c -o HDF5_dataset_write_parallel.o

HDF5_dataset_read: python_vol.o HDF5_dataset_read.o
	h5pcc -g HDF5_dataset_read.o python_vol.o $(LDFLAGS) -o HDF5_dataset_read

HDF5_dataset_read.o: HDF5_dataset_read.c
	h5pcc -g $(CFLAGS) HDF5_dataset_read.c -o HDF5_dataset_read.o

HDF5_dataset_write: python_vol.o HDF5_dataset_write.o
	h5pcc -g HDF5_dataset_write.o python_vol.o $(LDFLAGS) -o HDF5_dataset_write

HDF5_dataset_write.o: HDF5_dataset_write.c
	h5pcc -g $(CFLAGS) HDF5_dataset_write.c -o HDF5_dataset_write.o

python_vol.o: ${dir}/python_vol.c
	h5pcc -g $(CFLAGS) ${dir}python_vol.c -o python_vol.o
test_pylong: test_pylong.o
	$(CC) test_pylong.o  $(LDFLAGS) -o test_pylong
test_pylong.o: test_pylong.c
	$(CC) $(CFLAGS) test_pylong.c -o test_pylong.o        
clean:
	rm HDF5_file_create HDF5_file_open HDF5_objects_create HDF5_objects_open HDF5_dataset_read HDF5_dataset_write HDF5_file_create_parallel HDF5_objects_create_parallel HDF5_dataset_write_parallel *.o 

#	rm *.o HDF5_* ../src/*.pyc *.pyc HDF5_file_create HDF5_file_open HDF5_objects_create HDF5_objects_open HDF5_dataset_read HDF5_dataset_write
