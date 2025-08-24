import csv
from math import sqrt

file_name = "prime_list.csv"

def is_prime(n) -> bool:
   '''returns true if n is prime'''
   for i in range(2, int(sqrt(n)) + 1):
      if n % i == 0:
         return False
   return True

def generate_primes() -> None:
   """generates a list of prime numbers between 2000 and 4000 and writes them to a csv"""
   with open(file_name, "w", newline="") as file:
      writer = csv.writer(file)
      for i in range(5000, 10000):
         if is_prime(i):
            row = (i, )
            writer.writerow(row)



def testing():
   assert is_prime(2), "2 is prime"
   assert is_prime(2707), "2707 is prime"
   assert not(is_prime(100)), "100 is not prime"
   assert not(is_prime(2500)), "2500 is not prime"

testing()
generate_primes()