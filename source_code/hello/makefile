CC=arm-buildroot-linux-gnueabihf-gcc
CFLAGS=-I.
OBJ = hello.o 

%.o: %.c $(DEPS)
    $(CC) -c -o $@ $< $(CFLAGS)

all: hello

hello: $(OBJ)
    $(CC) -o $@ $^ $(CFLAGS)

upload: hello
    scp hello root@192.168.31.230:
        
run: upload
    ssh root@192.168.31.230 './hello'


clean:
    rm -f $(OBJ) hello

