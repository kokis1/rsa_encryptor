import numpy as np

class key:
   def __init__(self, modulus, exponent):
      self.modulus = modulus
      self.exponent = exponent

class encryptor:
   def __init__(self):
      self.n, self.totient = self.generate_modulus_and_totient()
      self.public_key = self.generate_public_key()
      self.private_key = self.generate_private_key()

   def generate_modulus_and_totient(self) -> tuple:
      """chooses 2 random numbers from a list of prime numbers and returns their product and totient"""
      
      filename = "../prime_list.csv"
      with open(filename, "r", newline="") as file:
         lines = file.readlines()
         prime_1 = int(lines[np.random.randint(0, len(lines))])
         prime_2 = int(lines[np.random.randint(0, len(lines))])
      return (prime_1 * prime_2, (prime_1-1)*(prime_2-1))

   def calculate_e(self) -> int:
      """calculates e"""
      return 2477
   
   def generate_public_key(self) -> key:
      return key(self.n, self.calculate_e())
   
   def calculate_d(self) -> int:
      d = self.public_key.exponent
      while d * self.public_key.exponent % self.totient != 1:
         d -= 1
      return d
   
   def generate_private_key(self) -> key:
      return key(self.n, self.calculate_d())
   
   def encode(self, message: int, public_key: key) -> int:
      '''encrypts the message'''
      return pow(message, public_key.exponent, public_key.modulus)
   
   def decrypt(self, cyphertext: int) -> int:
      '''decrypts the message'''
      return pow(cyphertext, self.private_key.exponent, mod=self.private_key.modulus)

def write_message(message: int, filename: str) -> None:
   with open(filename, "w") as file:
      file.write(str(message))

def read_message(filename: str) -> int:
   with open(filename, "r") as file:
      line = file.readline()
      return int(line)

def main():
   bob = encryptor()
   alice = encryptor()
   message = 1234
   filename = "./encrypted.txt"

   # alice sends a message to bob using his public key
   write_message(alice.encode(message, bob.public_key), filename)
   
   # bob bob reads the message and decodes it using his private key
   decrypted_from_bob = bob.decrypt(read_message(filename))
   print("Message:", message, "what Bob reckoned:", decrypted_from_bob)

if __name__ == "__main__":
   main()