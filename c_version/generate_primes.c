#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int check_inputs(int argc){
   if (argc != 3){
      return 0;
   }
   return 1;
}

int is_prime(unsigned int n){
   int max_check = sqrt(n);
   for(int i = 2; i <= max_check; i++){
      if(n % i == 0)
            return 0;
   }
   return 1;
}

void print_primes(int max, char *output){
   FILE *fptr = fopen(output, "w");
   for(int i = 2; i <= max; i++){
      if(is_prime(i) == 1){
         fprintf(fptr, "%d\n", i);
      }
   }
   fclose(fptr);
}

int main(int argc, char **argv){
   if (check_inputs(argc) == 0){
      printf("File Descriptor Missing\n");
   }

   char *output = argv[1];
   int max_primes = atoi(argv[2]);


   print_primes(max_primes, output);

   return 0;
}