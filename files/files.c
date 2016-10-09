#include <stdio.h>
#include <stdlib.h>

#include "files.h"
#include "../image-crypto.h"

int file_open(struct files_h *f, char *filename){
	if((f->f = fopen (filename, "rb")) == NULL){
		return IC_FILE_NOT_FOUND;
	};

  fseek (f->f, 0, SEEK_END);
  f->bytes = ftell (f->f);
  fseek (f->f, 0, SEEK_SET);

  f->buffer = malloc (f->bytes);
  if (f->buffer == NULL)
  {
    return IC_NULL_POINTER;
  }

	if(fread(f->buffer, 1, f->bytes, f->f) != f->bytes){
		return IC_ERROR;
	}

	return IC_SUCCESS;
}

int file_close(struct files_h *f){
	fclose(f->f);
	free(f->buffer);
}
