
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "files.h"
#include "i18n.h"
#include "logging.h"
#include "image.h"
#include "crypto.h"
#include "i18n.h"
#include "gui.h"

#include "settings.h"

#include "image-crypto.h"

void usage(char *name){
  fprintf(stderr, "Usage: %s -a (ecb|cbc)\n", name);
}

static const char* const ic_error_strings[] = {
	"Error.",
	"Success.",
	0
}; // all const

char * ic_err_to_string(int err){
	if(err > MAX_IC_ERROR_CODE){
		return "Unknown";
	}
	if(err < 0){
		return "Unknown";
	}
	return ic_error_strings[err];

}

int main(int argc, char *argv[]){

	int err;

	LOG_INFO("Image Crypto Tool v0.1\n");

	//Load settings
	struct files_h settings_file;
	if((err = file_open(&settings_file, "settings/default.conf")) != IC_SUCCESS){
		fprintf(stderr, "error: %s", ic_err_to_string(err));
	}
	LOG_INFO("settings contents: %s, length: %d\n", settings_file.buffer, settings_file.bytes);

	Settings *settings;
	settings = settings_open(settings_file.f);
  if(settings ==NULL){
    LOG_ERROR("Failed to load settings.\n");
  }
	char language[10]; //TODO:Size?
	int result;
	result = settings_get(settings, "user", "language", language, sizeof(language));
  if(result == 0){
    LOG_ERROR("Failed to get language from settings file. Using en-gb.\n");
    strncpy(language, "en-gb", 6);
  }

	LOG_INFO("Using %s language pack\n", language);
	//Done with settings file, so close
	file_close(&settings_file);


	//Load language pack
	struct files_h lang_pack_file;
	char lang_filename[30];
	snprintf(lang_filename,30,"i18n/lang/%s.json",language);
	LOG_INFO("Loading language pack from %s\n", lang_filename);

	if((err = file_open(&lang_pack_file, lang_filename)) != IC_SUCCESS){
		fprintf(stderr, "error: %s", ic_err_to_string(err));
	}
	LOG_INFO("Opened %s\n", lang_filename);
	LOG_INFO("lang pack contents: %s, length: %d\n", lang_pack_file.buffer, lang_pack_file.bytes);

	struct i18n_h i18n;
	i18n_init(&i18n, language, lang_filename, lang_pack_file.buffer);
  file_close(&lang_pack_file);
	//char * title = i18n_get_string(&i18n, "title");

  char *title = malloc(100);
  if(i18n_get(&i18n, "title", title, 100) == 0){
    LOG_ERROR("Title get failed\n");
  }
  printf("Title: %s\n", title);
  free(title);





	//Read command arguments
  enum cipher_alg alg;
  char *filename = NULL;
  int opt = 0;
  while ((opt = getopt(argc, argv,"a:f:")) != -1) {
    switch (opt) {
      case 'a':
        if (strncmp("cbc", optarg, 3) == 0){
          alg = AES_CBC;
        } else if (strncmp("ecb", optarg, 3) == 0){
          alg = AES_ECB;
        }else{
          usage(argv[0]);
          exit(EXIT_FAILURE);
        }
        break;
      case 'f':
        filename = optarg;
        break;
      default:
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  LOG_INFO("Encrypting %s with %s\n", filename, alg_to_string(alg));
  //Get image data
  unsigned char *image_data;
  unsigned char *enc_image_buffer;
  unsigned image_data_length;

  //Set key & IV for CBC mode encryption
  LOG_INFO("Using default keys\n");
  unsigned char key[16] = "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b";
  unsigned char iv[16] = "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b";
  LOG_DEBUG("j\n");
  union cipher_alg_inputs alg_inputs;

  if (alg == AES_CBC){
    alg_inputs.cbc.key.key = key;
    alg_inputs.cbc.key.key_length = sizeof(key);
    LOG_DEBUG("sizeof(key): %d\n", sizeof(key));

    alg_inputs.cbc.iv.iv = iv;
    alg_inputs.cbc.iv.iv_length = sizeof(iv);

  } else if (alg == AES_ECB){
    alg_inputs.ecb.key.key = key;
    alg_inputs.ecb.key.key_length = sizeof(key);
  }

  struct cipher_data cd = {
    .plaintext = image_data,
    .ciphertext = enc_image_buffer,
    .text_length = image_data_length,

    .op = ENCRYPT,
    .alg = alg,

    .type = alg_inputs,

  };

  cbc_encrypt(&cd);


  //Launch GUI
  LOG_INFO("Launching GUI\n");
  //Start the GUI, passing in the selected language pack.
  gui_start(&i18n);

	i18n_close(&i18n);
  settings_delete(settings);
  LOG_INFO("Complete\n");
	return EXIT_SUCCESS;


}
