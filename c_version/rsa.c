#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define PRIME_LIST "output.txt"
#define SMALL_PRIMES "small_primes.txt"

int get_random_bytes(void *buffer, size_t length){
   /* Uses the /dev/urandom file provided in mac/linux to get random bytes*/
   FILE *fptr = fopen("/dev/urandom", "rb");
   size_t read = fread(buffer, 1, length, fptr);
   fclose(fptr);
   return (read == length) ? 0 : -1;
}

uint64_t get_entry(FILE *fptr, unsigned int line_num){
   /* Gets the entry at spefified line number*/
   char result[32]; // maximum string buffer size for a uint32_t
   for(int i = 0; i < line_num; i++){
      fgets(result, 32, fptr);
   }
   return (uint64_t)atoi(result);
}

uint64_t get_small_prime(FILE *fptr, int totient){
   /* Gets a small prime number coprime to the totient*/
   char small_prime_chars[32];
   int small_prime;
   for(int i = 0; i < 9800; i++){
      fgets(small_prime_chars, 32, fptr);
      small_prime = atoi(small_prime_chars);
      if(small_prime % totient != 0){
         return (uint64_t)small_prime;
      }
   }
   return (uint64_t)small_prime;
}

uint64_t multiplicative_modular_inv(uint64_t e, uint64_t totient){
    int64_t old_r = e;
    int64_t r = totient;
    int64_t old_s = 1;
    int64_t s = 0;

    while(r != 0){
        int64_t quotient = old_r / r;

        int64_t temp_r = r;
        r = old_r - quotient * r;
        old_r = temp_r;

        int64_t temp_s = s;
        s = old_s - quotient * s;
        old_s = temp_s;
    }

    // Adjust to positive
    if (old_s < 0){
        old_s += totient;
    }

    // Verify and return
    if (((int64_t)(old_s * e)) % (int64_t)totient == 1){
        return (uint64_t)old_s;
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

    FILE *fptr = fopen(PRIME_LIST, "r");
    uint64_t p = get_entry(fptr, random_num_1);  // fix: uint64_t
    uint64_t q = get_entry(fptr, random_num_2);  // fix: uint64_t
    fclose(fptr);

    // now multiplication happens in 64-bit
    uint64_t n = p * q;
    uint64_t totient = (p - 1) * (q - 1);

    FILE *fptr_small_primes = fopen(SMALL_PRIMES, "r");
    uint64_t e = get_small_prime(fptr_small_primes, totient);  // fix: uint64_t
    fclose(fptr_small_primes);

    uint64_t d = multiplicative_modular_inv(e, totient);
    if (d == 0){
        printf("Failed to find modular inverse\n");
        return 1;
    }

    pub_key->n = n;
    pub_key->value = e;

    priv_key->n = n;
    priv_key->value = d;

    return 0;
}

uint64_t rl_binary_modexp(uint64_t base_num, uint64_t exp, uint64_t modulus){
    if (modulus == 0) return 0;
    if (exp == 0) return 1 % modulus;

    uint64_t result = 1;
    uint64_t base = base_num % modulus;
    uint64_t exponent = exp;

    while (exponent > 0){
        if (exponent % 2 == 1){
            result = ((__uint128_t)result * base) % modulus;
        }
        exponent = exponent >> 1;
        base = ((__uint128_t)base * base) % modulus;
    }

    return result;
}

uint64_t encrypt(uint64_t m, key pub_key){
  return rl_binary_modexp(m, pub_key.value, pub_key.n);
}

uint64_t decrypt(uint64_t c, key priv_key){
   return rl_binary_modexp(c, priv_key.value, priv_key.n);
}

int main(int argc, char** argv){
   key pub_key;
   key priv_key;
   gen_key_pair(&pub_key, &priv_key);
   printf("pub_key n: %llu, pub_key e: %llu\n", pub_key.n, pub_key.value);
   printf("priv_key n: %llu, priv d: %llu\n", priv_key.n, priv_key.value);

   uint64_t m = atoi(argv[1]);
   uint64_t encrypted = encrypt(m, pub_key);
   uint64_t decrypted = decrypt(encrypted, priv_key);
   printf("message: %llu, cyphertext: %llu, decrypted: %llu\n", m, encrypted, decrypted);
   return 0;
}