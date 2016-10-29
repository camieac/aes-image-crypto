
#include "crypto.h"
#include "logging.h"

#include <sys/socket.h>
#include <linux/if_alg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char * alg_to_string(enum cipher_alg alg){
  switch(alg){
    case AES_CBC:
      return "AES (CBC)";
    case AES_ECB:
      return "AES (ECB)";
    default:
    return "unknown!";
  }
}

int cipher(struct cipher_data *cd){

	struct sockaddr_alg sa = {
		.salg_family = AF_ALG,
		.salg_type = "skcipher",
		.salg_name = "cbc(aes)"
	};

  int sockfd;
	if((sockfd = socket(AF_ALG, SOCK_SEQPACKET, 0)) == -1){
    ERR();
  }

	if(bind(sockfd, (struct sockaddr*)&sa, sizeof(sa)) == -1 ){
    ERR();
  }
  LOG_DEBUG("key len: %d\n" ,cd->type.cbc.key.key_length);
  unsigned char *key;
  unsigned key_length;
  if(cd->alg == AES_CBC){
    key = cd->type.cbc.key.key;
    key_length = cd->type.cbc.key.key_length;
  } else if (cd->alg == AES_ECB){
    key = cd->type.ecb.key.key;
    key_length = cd->type.ecb.key.key_length;
  }
	if (setsockopt(sockfd, SOL_ALG, ALG_SET_KEY, key, key_length) == -1){
    ERR();
  }

  int connfd;
  connfd = accept(sockfd, NULL, 0);
  if (connfd == -1) {
    ERR();
  }
  unsigned cbuf_length;
  if(cd->alg == AES_CBC){
    cbuf_length = CMSG_SPACE(4) + CMSG_SPACE(cd->type.cbc.iv.iv_length + 4);
  }else if(cd->alg == AES_ECB){
    cbuf_length = CMSG_SPACE(4);
  }
	unsigned char cbuf[cbuf_length];
  memset(cbuf, 0, sizeof(cbuf));
	struct msghdr msg;
	struct cmsghdr *cmsg;
	struct iovec iov;
	struct af_alg_iv *ivm;
	__u32 t = ALG_OP_ENCRYPT;
	int ret;

	memset(&msg, 0, sizeof(msg));

	msg.msg_control = cbuf;
	msg.msg_controllen = sizeof(cbuf);

	cmsg = CMSG_FIRSTHDR(&msg);
	if (cmsg == NULL)
	  ERR();

	cmsg->cmsg_level = SOL_ALG;
	cmsg->cmsg_type = ALG_SET_OP;
	cmsg->cmsg_len = CMSG_LEN(4);
	memcpy(CMSG_DATA(cmsg), &t, 4);

	/* NOTE that in glibc this might be broken.
	 * http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=514917
	 */
  if(cd->alg == AES_CBC){
    cmsg = CMSG_NXTHDR(&msg, cmsg);
  	if (cmsg == NULL)
  	  ERR();

    cmsg->cmsg_level = SOL_ALG;
  	cmsg->cmsg_type = ALG_SET_IV;
  	cmsg->cmsg_len = CMSG_LEN(cd->type.cbc.iv.iv_length + 4);

  	t = cd->type.cbc.iv.iv_length;
  	memcpy(CMSG_DATA(cmsg), &t, 4);
  	memcpy(CMSG_DATA(cmsg)+4, cd->type.cbc.iv.iv, cd->type.cbc.iv.iv_length);
  }


	iov.iov_base = (void*)cd->plaintext;

	iov.iov_len = cd->text_length;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;


	ret = sendmsg(connfd, &msg, 0);

	if (ret != cd->text_length) {
	  if (ret == -1) perror("sendmsg");
	  fprintf(stderr, "%d: written %d, expected %d\n", __LINE__, ret, cd->text_length);
	  return CRYPTO_ERROR;
        }

	ret = read(connfd, cd->ciphertext, cd->text_length);
	if (ret != cd->text_length) {
	  if (ret == -1) perror("read");
	  fprintf(stderr, "%d: read %d, expected %d\n", __LINE__, ret, cd->text_length);
	  return CRYPTO_ERROR;
        }

	if( sockfd != -1 ){
    	close(sockfd);
  }

	if( connfd != -1 ){
    close(connfd);
  }


	return 1;
}

int cbc_encrypt(struct cipher_data *cd){
  return cipher(cd);
}
