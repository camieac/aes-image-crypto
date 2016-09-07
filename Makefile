VERSION = 1.0
APPNAME = image_crypto
AUTHOR = Cameron A. Craig

CC = gcc
CFLAGS = -Wall

SRCS = main.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(APPNAME)

bmp_image:
	$(CC) $(CFLAGS) bmp_image.c -o bmp_image

clean:
	rm -f $(APPNAME)
	rm -f bmp_image
