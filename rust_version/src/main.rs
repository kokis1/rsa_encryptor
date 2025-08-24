use std::fs::File;
use std::io::{self, BufRead};
use rand;
use rand::seq::IteratorRandom;
struct Key{
    modulus: u32,
    exponent: u32,
}

struct Encryptor{
    modulus: u32,
    totient: u32,
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
        let primes: Vec<u32> = numbers.iter().filter_map(|x| x.parse::<u32>().ok()).collect();

        self.modulus = primes[0] * primes[1];
        self.totient = (primes[0] - 1) * (primes[1] - 1);

    }
    fn generate_pub_key(&mut self){
        // generates the public key
         self.public_key = Key{modulus: self.modulus, exponent: 2477};
    }
    fn generate_priv_key(&mut self){
        // generates the private key

        let mut d = self.public_key.exponent;
        while (d * self.public_key.exponent) % self.totient != 1 {
            d -= 1;
        }

        self.private_key = Key{modulus: self.modulus, exponent: d};
    }
    pub fn encode(message: u32, pub_key: Key) -> u32{
        return message.pow(pub_key.exponent) % pub_key.modulus
    }
    pub fn decode(self, message: u32) -> u32{
        return message.pow(self.private_key.exponent) % self.private_key.modulus
    }
}

fn main(){
    println!("hello!");
    let encr = Encryptor::new();
    println!("modulus: {:?}, totient: {:?}", encr.modulus, encr.totient);
}
