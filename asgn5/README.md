This program is a secure encryption tool that includes three programs: encrypt, decrypt, and keygen. The program is built using the C language and implements public-private cryptography using the SS library and number theory functions.

To use this program, ensure the following files are correctly downloaded and in the same directory:
* decrypt.c - Contains the implementation and main() function for the decrypt program.
* encrypt.c - Contains the implementation and main() function for the encrypt program.
* keygen.c - Contains the implementation and main() function for the keygen program.
* numtheory.c - Contains the implementations of the number theory functions.
* numtheory.h - Specifies the interface for the number theory functions.
* randstate.c - Contains the implementation of the random state interface for the SS library and number theory functions.
* randstate.h - Specifies the interface for initializing and clearing the random state.
* ss.c - Contains the implementation of the SS library.
* ss.h - Specifies the interface for the SS library.
* Makefile

The following files contain more information about the programs:
* README.md
* DESIGN.pdf
* WRITEUP.pdf

To use the file, open terminal and navigate to the appropriate directory

Compile all the files (make clean, make all)

Type "chmod +x {file}" into the terminal if you want to edit the file

Once compiled without errors, the following options are avaliable:

keygen:
* -b : specifies the minimum bits needed for the public modulus n.
* -i : specifies the number of Miller-Rabin iterations for testing primes (default: 50).
* -n pbfile : specifies the public key file (default: ss.pub).
* -d pvfile : specifies the private key file (default: ss.priv).
* -s : specifies the random seed for the random state initialization (default: the seconds since the UNIX epoch, given by time(NULL)).
* -v : enables verbose output.
* -h : displays program synopsis and usage.

encrypt:
* -i : specifies the input file to encrypt (default: stdin).
* -i : specifies the input file to encrypt (default: stdin).
* -n : specifies the file containing the public key (default: ss.pub).
* -v : enables verbose output.
* -h : displays program synopsis and usage.

decrypt:
* -i : specifies the input file to decrypt (default: stdin). 
* -o : specifies the output file to decrypt (default: stdout).
* -n : specifies the file containing the private key (default: ss.priv).
* -v : enables verbose output.
* -h : displays program synopsis and usage.

Example: 
1. *./keygen* would generate the public and private keys that encrypt and decrypt use.
2. *./encrypt -v* would encrypt the input and also print the verbose.
3. *./decrypt* will decrypt the input from encrypt.
