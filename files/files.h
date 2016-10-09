#ifndef IC_FILES_H
#define IC_FILES_H

struct files_h {
	FILE *f;
	char *buffer; //Stores contents of file.
	long bytes; //Number of bytes in file.
};

#endif
