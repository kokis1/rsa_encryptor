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

int get_entry(FILE *fptr, unsigned int line_num){
   /* Gets the entry at spefified line number*/
   char result[32];
   for(int i = 0; i < line_num; i++){
      fgets(result, 32, fptr);
   }
   return atoi(result);
}

typedef struct{
   int n;
   int value;
} key;

int gen_priv_key(key *key){
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
   random_num_1 = random_num_1 % 4459;
   random_num_2 = random_num_2 % 4459;

   // opens the list of prime numbers
   FILE *fptr = fopen(PRIME_LIST, "r");

   int p = get_entry(fptr, random_num_1);
   int q = get_entry(fptr, random_num_2);

   //debugging code
   printf("num1: %llu\n", random_num_1);
   printf("p: %d\n", p);
   printf("num2: %llu\n", random_num_2);
   printf("q: %d\n", q);

   fclose(fptr);
   return 0;
}


int main(){
   key new_key;
   gen_priv_key(&new_key);
   return 0;
}