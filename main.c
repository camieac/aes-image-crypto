#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "main.h"
#include "crypto.h"
#include "image.h"
#include "logging.h"

void usage(char *name){
  fprintf(stderr, "Usage: %s -a (ecb|cbc)\n", name);
}

int main(int argc, char *argv[]){
  LOG_INFO("Image Crypto Tool v0.1\n");
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

  LOG_INFO("Complete\n");
  return EXIT_SUCCESS;
}
