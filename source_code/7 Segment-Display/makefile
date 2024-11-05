##The name of the main executable

TARGET=display_test

# The source-code that make up the main executables

SOURCE=test.c display.c gpio.c

# IP
IP= "192.168.2.169"

# Flags 

OPTIMZE=-O3

CFLAGS=$(OPTIMIZE) -Wall

LDFLAGS= -lm

CC=arm-buildroot-linux-gnueabihf-gcc

# The first target (all) is allways the default target

all: build upload

# Our buld target depends on the real target

.PHONY: build

build: $(TARGET) display.h gpio.h

# Our target is built up from the objects

$(TARGET): $(SOURCE)

	$(CC) -Wall -o $(TARGET) $(SOURCE)  

# Upload to raspberry
.PHONY: upload

upload:

	scp $(TARGET) root@192.168.2.169:~/


# Cleanup all generated files

clean:

	rm -Rf  $(TARGET) 
