#ifndef RSA_H
#define RSA_H

#include <stdint.h>

typedef struct k key;

int gen_key_pair(key* pub_key, key* priv_key);

uint64_t encrypt(uint64_t m, key* pub_key);
uint64_t decrypt(uint64_t c, key* priv_key);


#endif