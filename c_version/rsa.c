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

uint64_t multiplicative_modular_inv(uint64_t e, uint64_t totient){
   uint64_t old_r = e;
   uint64_t r = totient;
   uint64_t old_s = 1;
   uint64_t s = 0;
   uint64_t old_t = 0;
   uint64_t t = 1;

   while(r != 0){
      uint64_t quotient = old_r / r;

      uint64_t temp_r = r;
      r = old_r - quotient*r;
      old_r = temp_r;

      uint64_t temp_s = s;
      s = old_s - quotient*s;
      old_s = temp_s;

      uint64_t temp_t = t;
      t = old_t - quotient*t;
      old_t = temp_t;
   }
   if ((old_s*e)%totient == 1){
      return old_s;
   }
   else{
      return 0;
   }
}

typedef struct{
   uint64_t n;
   uint64_t value;
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
      8: modifies the public and private keys that were handed to the function
      9: return 0 if all executed correctly
      */

   uint64_t d = 0;
   uint64_t *d_ptr = &d;
   
   while(d == 0){
   uint64_t random_num_1;
   uint64_t random_num_2;

   // generates the random bytes for the random indexes
   if (get_random_bytes(&random_num_1, sizeof(random_num_1)) != 0){
      printf("Failed to read from /dev/urandom");
      return 1;
   }
   if (get_random_bytes(&random_num_2, sizeof(random_num_2)) != 0){
      printf("Failed to read from /dev/urandom");
      return 1;
   }

   // converts the bytes to indexes of the prime list
   random_num_1 = random_num_1 % 4459;
   random_num_2 = random_num_2 % 4459;

   // opens the list of prime numbers and assigns the random numbers p and q
   FILE *fptr = fopen(PRIME_LIST, "r");
   int p = get_entry(fptr, random_num_1);
   int q = get_entry(fptr, random_num_2);
   fclose(fptr);


   // calculates n and the totient
   uint64_t n = p*q;
   uint64_t totient = (p-1)*(q-1);

   // opens the list of small primes to select the remainders
   FILE *fptr_small_primes = fopen(SMALL_PRIMES, "r");
   int e = get_small_prime(fptr_small_primes, totient);
   fclose(fptr_small_primes);


   // calculates the private exponent
   *d_ptr = multiplicative_modular_inv(e, totient);

   // sets the values of the public and private keys
   pub_key->n = n;
   pub_key->value = e;

   priv_key->n = n;
   priv_key->value = d;
}
   
   // returns 0 if all happened correctly
   return 0;
}

int encrypt(int m, key pub_key){
   int c = pow(m, pub_key.value);
   return c % pub_key.n;
}

int decrypt(int c, key priv_key){
   int m = pow(c, priv_key.value);
   return m;
}

int main(){
   key pub_key;
   key priv_key;
   gen_key_pair(&pub_key, &priv_key);
   printf("pub_key n: %llu, pub_key e: %llu\n", pub_key.n, pub_key.value);
   printf("priv_key n: %llu, priv d: %llu\n", priv_key.n, priv_key.value);
   return 0;
}