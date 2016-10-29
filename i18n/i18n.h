#ifndef IC_I18N_H
#define IC_I18N_H

#include "jsmn.h"
#include "strmap.h"

#define I18N_ERROR 0
#define I18N_SUCCESS 1

struct i18n_h {
	char *language;
	char *filename;
	char *buffer;//TODO: Do this better? Keep all dictionary key:value pairs
	StrMap *str_map;
};

//Function Definitions
int i18n_init(struct i18n_h *i18n, char *language, char *filename, char * lang_pack);
int i18n_get(struct i18n_h *i18n, char * key, char *result, unsigned int result_len);
int i18n_get_size(struct i18n_h *i18n, char * key);
int i18n_close(struct i18n_h *i18n);

#endif
