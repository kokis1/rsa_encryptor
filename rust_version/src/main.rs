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
    pub fn new() -> Encryptor{
        /*Encryptor { 
            modulus: Self::generateModulus(),
            totient: Self::generateTotient(),
            public_key: Self::generatePubKey(),
            private_key: Self::generatePrivKey() }
            */
        Self::generateModulusTotient()
    }
    fn generateModulus(){
        // opens the prime number file

        // generates two random numbers

        // reads the line associated with the numbers

        // sets self's modulus
        // sets self's totient
    }
    fn generateTotient() -> u32{
        // generates the totient
        return 3
    }
    fn generatePubKey() -> Key{
        // generates the public key
        return Key{modulus: 3, exponent: 3}
    }
    fn generatePrivKey() -> Key{
        // generates the private key
        return Key{modulus: 3, exponent: 3}
    }
    pub fn encode(message: u32, pub_key: Key) -> u32{
        return message.pow(pub_key.exponent) % pub_key.modulus
    }
    pub fn decode(self, message: u32) -> u32{
        return message.pow(self.private_key.exponent) % self.private_key.modulus
    }
}

fn main(){
    println!("hello world");
}
