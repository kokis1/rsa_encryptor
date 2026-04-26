#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define PRIME_LIST "output.txt"
#define SMALL_PRIMES "small_primes.txt"

int get_random_bytes(void *buffer, size_t length){
   FILE *fptr = fopen("/dev/urandom", "rb");
   size_t read = fread(buffer, 1, length, fptr);
   fclose(fptr);
   return (read == length) ? 0 : -1;
}

int get_entry(FILE *fptr, unsigned int line_num){
   /* Gets the entry at spefified line number*/
   char result[32]; // maximum string buffer size for a uint32_t
   for(int i = 0; i < line_num; i++){
      fgets(result, 32, fptr);
   }
   return atoi(result);
}

int get_small_prime(FILE *fptr, int totient){
   /* Gets a small prime number to be coprime to the totient*/
   char small_prime_chars[32];
   int small_prime;
   for(int i = 0; i < 9800; i++){
      fgets(small_prime_chars, 32, fptr);
      small_prime = atoi(small_prime_chars);
      if(small_prime % totient != 0){
         return small_prime;
      }
   }
   return small_prime;
}

uint64_t multiplicative_modulat_inv(uint64_t e, uint64_t totient){

   /* #!: Improve this!!!*/
   for(uint64_t i = 2; i < UINT64_MAX; i++){
      if((i * e) % totient == 1){
         return i;
      }
   }
   printf("INVERSE NOT FOUND");
   return -1;
}

typedef struct{
   int n;
   int value;
} key;

int gen_key_pair(key *pub_key, key *priv_key){

   /* Generates the private key:
      1: creates two random large integers, using urandom file in OS
      2: turns them in to line numbers by taking the modulus w.r.t the number of primes
      3: fetches the primes stored at that location
      4: calculates n
      5: calculates the totient -> (p - 1)x(q-1)
      6: chooses a value for e such that e is coprime to the totient
      7: calculates d
      */
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
   fclose(fptr);

   uint64_t n = p*q;
   uint64_t totient = (p-1)*(q-1);

   FILE *fptr_small_primes = fopen(SMALL_PRIMES, "r");
   int e = get_small_prime(fptr_small_primes, totient);
   fclose(fptr_small_primes);

   uint64_t d = multiplicative_modulat_inv(e, totient);

   pub_key->n = n;
   pub_key->value = e;

   priv_key->n = n;
   priv_key->value = d;

   debug:
      //debugging code
      printf("num1: %llu\n", random_num_1);
      printf("p: %d\n", p);
      printf("num2: %llu\n", random_num_2);
      printf("q: %d\n", q);
      printf("n: %llu\n", n);
      printf("totient: %llu\n", totient);
      printf("e: %d\n", e);
      printf("d: %llu\n", d);
      return 0;
   return 0;
}


int main(){
   key pub_key;
   key priv_key;
   gen_key_pair(&pub_key, &priv_key);
   printf("pub_key n: %d, pub_key e: %d\n", pub_key.n, pub_key.value);
   printf("priv_key n: %d, priv d: %d\n", priv_key.n, priv_key.value);
   return 0;
}