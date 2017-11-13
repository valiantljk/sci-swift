export PYTHONHOME=/root/anaconda
CC = gcc
CFLAGS = -c -fno-strict-aliasing -Wmissing-include-dirs -O2 -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -I/root/anaconda/include/python2.7  
LDFLAGS = -L/root/anaconda/lib -lpython2.7 -lpthread -ldl -lutil -lm -Xlinker -export-dynamic 
#LDFLAGS = /root/anaconda/lib/python2.7/config/libpython2.7.a # -lpthread -ldl -lutil -lm -Xlinker -export-dynamic
ALL: call
call: multi.o
	$(CC) multi.o  $(LDFLAGS) -o call
multi.o: multi.c 
	$(CC) $(CFLAGS) multi.c -o multi.o
clean:
	rm *.o call
