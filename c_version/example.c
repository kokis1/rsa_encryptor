#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "rsa.h"

int main(int argc, char** argv){
   key* pub_key = make_key();
   key* priv_key = make_key();

   if(gen_key_pair(pub_key, priv_key) == 1){
      return 1;
   }

   uint64_t m = atoi(argv[1]);
   uint64_t encrypted = encrypt(m, pub_key);
   uint64_t decrypted = decrypt(encrypted, priv_key);
   printf("message: %llu, cyphertext: %llu, decrypted: %llu\n", m, encrypted, decrypted);

   kill_key(pub_key);
   kill_key(priv_key);
   return 0;
}