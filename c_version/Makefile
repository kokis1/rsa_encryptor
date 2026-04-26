rsa: rsa.c output.txt
	clang rsa.c -o rsa

output.txt: generate_primes
	./generate_primes output.txt 50000 100000

generate_primes: generate_primes.c
	clang generate_primes.c -o generate_primes