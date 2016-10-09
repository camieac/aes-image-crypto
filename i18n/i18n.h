#ifndef IC_I18N_H
#define IC_I18N_H

#include "jsmn.h"
#include "strmap.h"

struct i18n_h {
	char *language;
	char *filename;

	StrMap *str_map;
};

#endif
