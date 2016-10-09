#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsmn.h"
#include "strmap.h"
#include "logging.h"

#include "i18n.h"

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int i18n_init(struct i18n_h *i18n, char *language, char *filename, char * lang_pack){
	int i;
	int r;

	i18n->language = language;
	i18n->filename = filename;

	i18n->str_map = sm_new(1000);//TODO:size?


	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	r = jsmn_parse(&p, lang_pack, strlen(lang_pack), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		//Ensure language is what is expected
		if (jsoneq(lang_pack, &t[i], "language") == 0) {
			/* We may use strndup() to fetch string value */
			if (strncmp(i18n->language, lang_pack + t[i+1].start, t[i+1].end-t[i+1].start) == 0){
				LOG_INFO("Parsed language matches set language\n");
			}else{
				LOG_WARNING("Unexpected language has been parsed\n");
			}
			i++;
		} else if (jsoneq(lang_pack, &t[i], "admin") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- Admin: %.*s\n", t[i+1].end-t[i+1].start,
					lang_pack + t[i+1].start);
			i++;
		} else if (jsoneq(lang_pack, &t[i], "uid") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
					lang_pack + t[i+1].start);
			i++;
		} else if (jsoneq(lang_pack, &t[i], "groups") == 0) {
			int j;
			printf("- Groups:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, lang_pack + g->start);
			}
			i += t[i+1].size + 1;
		} else {
			printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					lang_pack + t[i].start);
		}
	}

	sm_put(i18n->str_map, "application name", "Test Application");

	sm_delete(i18n->str_map);

}

int i18n_close(struct i18n_h *i18n){
	return 0;
}


char * i18n_get_string(){
	return "no";
}
