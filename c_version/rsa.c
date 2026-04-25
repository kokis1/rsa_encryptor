#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define PRIME_LIST "output.txt"

int get_random_bytes(void *buffer, size_t length){
   FILE *fptr = fopen("/dev/urandom", "rb");
   size_t read = fread(buffer, 1, length, fptr);
   fclose(fptr);
   return (read == length) ? 0 : -1;
}

typedef struct{
   int n;
   int value;
} key;

key *gen_priv_key(){
   uint64_t random_num_1;
   uint64_t random_num_2;

   if (get_random_bytes(&random_num_1, sizeof(random_num_1)) != 0){
      printf("Failed to read from /dev/urandom");
      return 1;
   }
   if (get_random_bytes(&random_num_2, sizeof(random_num_2)) != 0){
      printf("Failed to read from /dev/urandom");
      return 1;
   }
}