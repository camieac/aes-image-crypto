/**
* @file i18n.c
* @author Cameron A. Craig
* @date 23 Oct 2016
* @copyright 2016 Cameron Craig
* @brief Handles the importing of a JSON language pack.
*        A dictionary is constucted containing key:value pairs,
*        allowing applications to support multiple languagues.
*
* @details Parses a JSON string using JSMN. The following API is used:
*             - i18n_init(): Takes a JSON string and parses into a string map.
*             - i18n_get(): Get the word for the given key.
*             - i18n_put(): Put a word into the string map that was not in
*                           the original JSON language pack.
*             - i18n_close(): Clean up all resources used by i18n.
*          See doxygen comments for further detail.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsmn.h"
#include "strmap.h"
#include "logging.h"
#include "common.h"

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

	i18n->buffer = malloc(1000);
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

	//Insert each key value language dictionary value into the string map
	for(i = 0 ; i < r; i++){
		//jsmntype_t type = t[i].type;
		if(jsoneq(lang_pack, &t[i], language) == 0){
			sm_put(i18n->str_map, "language", language);
			int j;
			//Loop through the remaining tokens for dictionary items
			for(j = i ; j < r; j++){
				//The occurence of a JSMN_OBJECT guarantees two following tokens containing a key:value pair
				if(t[j].type == JSMN_OBJECT){
					int keylen = t[j+1].end - t[j+1].start;
					int vallen = t[j+2].end - t[j+2].start;
					char key[keylen + 1];// extra byte for NULL termination
					char value[vallen + 1];// extra byte for NULL termination

					//Copy the key and value to a temporary eritable bit of memory,
					// to add a null termination before entering into the string map.
					strlcpy(key, lang_pack + t[j+1].start, keylen + 1);
					strlcpy(value, lang_pack + t[j+2].start, vallen + 1);

					//Enter the key:value pair into the string map
					sm_put(i18n->str_map, key, value);

					//Skip the next 2 tokens, as these contain the key and value, already taken above.
					i += 2;
				}
			}
			break;
		}
	}
	return I18N_SUCCESS;
}

int i18n_close(struct i18n_h *i18n){
	//sm_delete(i18n->str_map);
	if(i18n->buffer != NULL){
		free(i18n->buffer);
		i18n->buffer = NULL;
	}
	return I18N_SUCCESS;
}

int i18n_get(struct i18n_h *i18n, char * key, char *result, unsigned int result_len){
	return sm_get(i18n->str_map, key, result, result_len);
}

int i18n_get_size(struct i18n_h *i18n, char * key){
	return sm_get(i18n->str_map, key, NULL, 0);
}
