use std::fs::File;
use std::io::{self, BufRead};
use rand;
use rand::seq::IteratorRandom;
struct Key{
    modulus: i64,
    exponent: i64,
}

struct Encryptor{
    modulus: i64,
    totient: i64,
    public_key: Key,
    private_key: Key,
}

impl Encryptor{
    pub fn new() -> Self{
        
        // creates an initial mutable state
        let mut encrypt = Self{ modulus: 0,
            totient: 0, 
            public_key: Key { modulus: 0, exponent: 0 }, 
            private_key: Key { modulus: 0, exponent: 0 }};
        
        // calls the actual constructor methods
        encrypt.generate_modulus_totient();
        encrypt.generate_pub_key();
        encrypt.generate_priv_key();
        
        // returns the fully constructed encryptor
        encrypt
         
    }
    pub fn generate_modulus_totient(&mut self){
        
        // opens the prime number file
        let path = "../prime_list.csv";
        

        // opens the file
        let file  = File::open(path).expect("unable to open file");
        
        // creates an iterator over the lines of the file
        let reader = io::BufReader::new(file);
        let lines: Vec<String> = reader.lines().map(|x| x.unwrap()).collect();
        
        // chooses two lines from the file randomly

        let mut rng = rand::rng();
        let numbers = lines.iter().choose_multiple(&mut rng, 2);
        
        // changes the primes into numbers to be manipulated
        let primes: Vec<i64> = numbers.iter().filter_map(|x| x.parse::<i64>().ok()).collect();

        // sets the totient and modulus attribute
        self.modulus = primes[0] * primes[1];
        self.totient = (primes[0] - 1) * (primes[1] - 1);

    }
    fn generate_pub_key(&mut self){
        // generates the public key
         self.public_key = Key{modulus: self.modulus, exponent: 2477};
    }

    fn safe_modular(&mut self, argument: i64, base: i64) -> Option<i64>{
        let mut remainder = argument;
        while remainder > base {
            remainder -= base;
        }
        Some(remainder)
    }

    fn extended_euclidean(&mut self, exponent: i64, base: i64) -> Option<i64>{
        let mut t: i64 = 0;
        let mut r: i64 = base as i64;
        let mut newt: i64 = 1;
        let mut newr: i64 = exponent as i64;

        while newr != 0 {
            let quotient = r / newr;
            
            // updates the value of t and newt
            let mut dummy_1 = t;
            let mut dummy_new = newt;
            t = dummy_new;
            newt = dummy_1 - quotient * dummy_new;

            dummy_1 = r;
            dummy_new = newr;
            r = dummy_new;
            newr = dummy_1 - quotient * dummy_new;

        }

        if r > 1 {
            return Option::None
        }
        if t < 0{
            t += base as i64;
        }
        return Some(t as i64)
    }

    fn generate_priv_key(&mut self){
        // generates the private key

        let d = self.extended_euclidean(self.public_key.exponent, self.modulus).unwrap();

        self.private_key = Key{modulus: self.modulus, exponent: d};
    }

    fn fast_modulus(self, number: i64, base: i64) -> i64{
        let mut remainder = number;
        while remainder > 0{
            remainder -= base;
        }
        return remainder as i64
    }

    pub fn exponentiate(message: i64, exponent: i64) -> i64{
        let mut counter = exponent;
        let mut answer = message;
        while counter > 0 {
            answer *= answer;
            counter -= 1;
        }
        return answer
    }

    pub fn encode(self, message: i64, pub_key: &Key) -> i64{
        let number = Encryptor::exponentiate(message, pub_key.exponent);
        return self.fast_modulus(number, pub_key.modulus)
    }
    pub fn decode(self, message: i64) -> i64{
        let number = Encryptor::exponentiate(message, self.private_key.exponent);

        // copied to prevent ownership error
        let modulus = self.private_key.modulus;
        return self.fast_modulus(number, modulus)
    }
}


fn main(){
    let alice = Encryptor::new();
    let bob = Encryptor::new();
    
    let message = 123;
    let encrypted_message = alice.encode(message, &bob.public_key);
    let decrypted_message = bob.decode(encrypted_message);
    println!("message: {}, encrypted message: {}, decrypted message: {}", message, encrypted_message, decrypted_message);
}