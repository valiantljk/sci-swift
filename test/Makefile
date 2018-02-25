dir=../src/
CC = gcc 
CFLAGS = -c -fno-strict-aliasing -Wmissing-include-dirs -O2 -DNDEBUG -fwrapv -O3 -Wall -Wstrict-prototypes -I/root/anaconda/include/python2.7 -isystem/root/anaconda/lib/python2.7/site-packages/numpy/core/include  -Wno-unused-variable  -Wno-strict-prototypes -Wno-cpp
LDFLAGS = -L/root/anaconda/lib -lpython2.7 -lpthread -ldl -lutil -lm -Xlinker -export-dynamic #-Wl,--verbose

ALL: swift python_vol_file_create python_vol_file_open test_pylong

swift: swift.o test_swift.o
	$(CC) swift.o test_swift.o  $(LDFLAGS) -o swift
swift.o: ${dir}/swift.c
	$(CC) $(CFLAGS) ${dir}/swift.c -o swift.o
test_swift.o: test_swift.c
	$(CC) $(CFLAGS) test_swift.c -o test_swift.o

python_vol_file_open: python_vol.o test_python_vol_file_open.o
	h5cc -g test_python_vol_file_open.o python_vol.o $(LDFLAGS) -o python_vol_file_open

test_python_vol_file_open.o: test_python_vol_file_open.c
	h5cc -g $(CFLAGS) test_python_vol_file_open.c -o test_python_vol_file_open.o

python_vol_file_create: python_vol.o test_python_vol_file_create.o
	h5cc -g test_python_vol_file_create.o python_vol.o $(LDFLAGS) -o python_vol_file_create

test_python_vol_file_create.o: test_python_vol_file_create.c
	h5cc -g $(CFLAGS) test_python_vol_file_create.c -o test_python_vol_file_create.o

python_vol.o: ${dir}/python_vol.c
	h5cc -g $(CFLAGS) ${dir}python_vol.c -o python_vol.o
test_pylong: test_pylong.o
	$(CC) test_pylong.o  $(LDFLAGS) -o test_pylong
test_pylong.o: test_pylong.c
	$(CC) $(CFLAGS) test_pylong.c -o test_pylong.o        
clean:
	rm *.o ../src/*.pyc *.pyc swift python_vol test_pylong *.h5 python_vol_file_open python_vol_file_create 2>> /dev/null 
