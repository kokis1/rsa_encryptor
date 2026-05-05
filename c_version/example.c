#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "rsa.h"

int main(int argc, char** argv){

   key* alice_priv = make_key();
   key* alice_pub = make_key();

   key* bob_priv = make_key();
   key* bob_pub = make_key();


   // ensuring the keys are created properly
   if(gen_key_pair(alice_pub, alice_priv) != 0){
      return 1;
   }
   if(gen_key_pair(bob_pub, bob_priv) != 0){
      return 1;
   }

   // Bob wants to send a message to Alice
   uint64_t bob_message = 1234;

   // First, Bob encrypts the message using Alice's public key
   uint64_t bob_cyphertext = encrypt(bob_message, alice_pub);

   // Then Alice decrypts the message using her private key
   uint64_t alice_decrypted = decrypt(bob_cyphertext, alice_priv);

   // Then Alice sends the message back to Bob to confirm she recieved the message
   uint64_t re_encrypted = encrypt(alice_decrypted, bob_pub);

   // Bob then checks that his message matches the one recieved by Alice
   uint64_t re_decrypted = decrypt(re_encrypted, bob_priv);

   printf("Original Message: %llu\n", bob_message);
   printf("Encrypted using Alice's public key: %llu\n", bob_cyphertext);
   printf("Alice recieved the message and decrypted it using her private key: %llu\n", alice_decrypted);
   printf("To confirm she recieved the message she first re-encrypts the message with Bob's public key: %llu\n", re_encrypted);
   printf("Bob then decrypts his message from Alice to confirm the channel is secure: %llu\n", re_decrypted);


   if(re_decrypted == bob_message){
      printf("Woohoo!! secure channel achieved!\n");
   }
   else{
      printf("Bad channel :(\n");
   }





   // freeing all the keys
   kill_key(alice_priv);
   kill_key(alice_pub);
   kill_key(bob_priv);
   kill_key(bob_pub);
   return 0;
}