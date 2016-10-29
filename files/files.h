#ifndef IC_FILES_H
#define IC_FILES_H

#define FILE_READ		(1 << 0)
#define FILE_WRITE	(1 << 1)

#define FILES_ERROR 0
#define FILES_SUCCESS 1

struct files_h {
	FILE *f;
	char *buffer; //Stores contents of file.
	long bytes; //Number of bytes in file.
};

//Function Definitions
int file_open(struct files_h *f, char *filename, int flags);
int file_close(struct files_h *f);
#endif
