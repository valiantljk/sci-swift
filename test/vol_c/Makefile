#all: 
#	h5cc -o sciswift vol_test.c swift_vol.c
#clean :
#	rm sciswift swift_vol.o vol_test.o *.h5
dir=../../src/vol_c/
export PYTHONHOME=/root/anaconda
CC = gcc
CFLAGS = -g -c -fno-strict-aliasing -Wmissing-include-dirs -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -I/root/anaconda/include/python2.7
LDFLAGS = -L/root/anaconda/lib -lpython2.7 -lpthread -ldl -lutil -lm -Xlinker -export-dynamic

ALL: swift_list
swift_list: swift_list.o test_swift_list.o
	$(CC) swift_list.o test_swift_list.o  $(LDFLAGS) -o swift_list
swift_list.o: ${dir}/swift_list.c
	$(CC) $(CFLAGS) ${dir}/swift_list.c -o swift_list.o
test_swift_list.o: test_swift_list.c
	$(CC) $(CFLAGS) test_swift_list.c -o test_swift_list.o
clean:
	rm *.o swift_list 
