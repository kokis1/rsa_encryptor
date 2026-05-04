rsa: rsa.c output.txt small_primes.txt
	clang rsa.c -o rsa

program: rsa.c example.c
	clanf rsa.c example.c -o example

output.txt: generate_primes
	./generate_primes output.txt 50000 100000

small_primes.txt: generate_primes
	./generate_primes small_primes.txt 200 10000

generate_primes: generate_primes.c
	clang generate_primes.c -o generate_primes