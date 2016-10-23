VERSION = 1.0
APPNAME = image-crypto
AUTHOR = Cameron A. Craig

CC = gcc
CFLAGS = -Wall
GTKFLAGS=-export-dynamic `pkg-config --cflags --libs gtk+-2.0`
DEBUGFLAGS = -g

SRCS = image-crypto.c files/files.c vendor/jsmn/jsmn.c i18n/i18n.c vendor/settings/settings.c vendor/strmap/strmap.c crypto/crypto.c gui/gui.c lib/common.c
INCS = -Ifiles/ -Ivendor/jsmn/ -Ii18n/ -Ivendor/settings/ -Ivendor/strmap/ -Ilib/ -Iimage/ -Icrypto/ -Igui/

all: clean
	$(CC) $(INCS) $(CFLAGS) $(SRCS) $(GTKFLAGS) $(DEBUGFLAGS) -o $(APPNAME)


bmp_image_write: clean
	$(CC) $(CFLAGS) bmp_image.c image.c -o bmp_image

bmp_image_read: clean
	$(CC) $(CFLAGS) bmp_image_read.c image.c -o bmp_image_read

args: clean
	$(CC) $(CFLAGS) args.c -o args
clean:
	rm -f $(APPNAME)
	rm -f bmp_image
	rm -f args
