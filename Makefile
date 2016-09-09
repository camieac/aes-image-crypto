VERSION = 1.0
APPNAME = image_crypto
AUTHOR = Cameron A. Craig

CC = gcc
CFLAGS = -Wall

SRCS = main.c

all: clean
	$(CC) $(CFLAGS) $(SRCS) -o $(APPNAME)

bmp_image: clean
	$(CC) $(CFLAGS) bmp_image.c image.c -o bmp_image

clean:
	rm -f $(APPNAME)
	rm -f bmp_image
