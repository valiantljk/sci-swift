dir=../src/
export PYTHONHOME=/root/anaconda
CC = gcc 
CFLAGS = -g -c -fno-strict-aliasing -Wmissing-include-dirs -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -I/root/anaconda/include/python2.7 -Wno-unused-variable -Wno-strict-prototypes
LDFLAGS = -L/root/anaconda/lib -lpython2.7 -lpthread -ldl -lutil -lm -Xlinker -export-dynamic #-Wl,--verbose

ALL: swift swift_vol
swift: swift.o test_swift.o
	$(CC) swift.o test_swift.o  $(LDFLAGS) -o swift
swift.o: ${dir}/swift.c
	$(CC) $(CFLAGS) ${dir}/swift.c -o swift.o
test_swift.o: test_swift.c
	$(CC) $(CFLAGS) test_swift.c -o test_swift.o
swift_vol:
	h5cc -o swift_vol test_swift_vol.c ${dir}swift_vol.c
clean:
	rm *.o swift swift_vol *.h5 *.pyc
