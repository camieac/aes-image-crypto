#ifndef CCRYPTO_H
#define CCRYPTO_H

#include <linux/if_alg.h>

#define AES_BLOCK_SIZE 16

#ifndef SOL_ALG
#define SOL_ALG 279
#endif

#define CRYPTO_ERROR 0

struct key {
  unsigned char *key;
  unsigned key_length;
};

struct iv {
  unsigned char *iv;
  unsigned iv_length;
};

struct cbc {
  struct key key;
  struct iv iv;
};

struct ecb {
  struct key key;
};

enum cipher_op {
  ENCRYPT = ALG_OP_ENCRYPT,
  DECRYPT = ALG_OP_DECRYPT
};

enum cipher_alg {
  AES_CBC,
  AES_ECB
};

union cipher_alg_inputs {
  struct cbc cbc;
  struct ecb ecb;
};

struct cipher_data {
  unsigned char *plaintext;
  unsigned char *ciphertext;
  unsigned text_length;

  enum cipher_op op;
  enum cipher_alg alg;
  union cipher_alg_inputs type;

};

char * alg_to_string(enum cipher_alg alg);
int cbc_encrypt(struct cipher_data *cd);

#endif //ifdef CCRYPTO_H
