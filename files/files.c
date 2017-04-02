#include <stdio.h>
#include <stdlib.h>

#include "files.h"
#include "../image-crypto.h"

int file_open(struct files_h *f, char *filename, int flags){
	f->buffer = NULL;
	f->bytes = 0;

	char * open_mode;
	if((flags & FILE_READ) && (flags & FILE_WRITE)){
		open_mode = "r+"; //Read and write
	}else if (flags & FILE_READ){
		open_mode = "r"; //Read only
	}else{
		open_mode = "r"; //Default to read only
	}

	if((f->f = fopen (filename, open_mode)) == NULL){
		return IC_FILE_NOT_FOUND;
	};

	return FILES_SUCCESS;
}

int file_read(struct files_h *f){
	fseek (f->f, 0, SEEK_END);
	f->bytes = ftell (f->f);
	fseek (f->f, 0, SEEK_SET);

	f->buffer = malloc (f->bytes);
	if (f->buffer == NULL)
	{
		return IC_NULL_POINTER;
	}

	if(fread(f->buffer, 1, f->bytes, f->f) != f->bytes){
		return FILES_ERROR;
	}

	rewind(f->f);
	return FILES_SUCCESS;
}

int file_close(struct files_h *f){
	fclose(f->f);
	if(f->buffer){
		free(f->buffer);
	}
	return FILES_SUCCESS;
}
