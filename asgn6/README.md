This program is a compression and decompression tool that includes two programs: encode and decode. This program is built using the C language and implements LZ78 Compression/Decompression.

To use this program, ensure the following files are correctly downloaded and in the same directory:
* encode.c : contains the main() function for the encode program.
* decode.c : contains the main() function for the decode program.
* trie.c: the source file for the Trie ADT.
* trie.h: the header file for the Trie ADT. 
* word.c: the source file for the Word ADT.
* word.h: the header file for the Word ADT. 
* io.c: the source file for the I/O module.
* io.h: the header file for the I/O module. 
* endian.h: the header file for the endianness module. 
* code.h: the header file containing macros for reserved codes. 
* Makefile

The following files contain more information about the programs:
* README.md
* DESIGN.pdf
* WRITEUP.pdf

To use the file, open terminal and navigate to the appropriate directory

Compile all the files (make clean, make all)

Type "chmod +x {file}" into the terminal if you want to edit the file

Once compiled without errors, the following options are avaliable:

encode:
* -v : Print compression statistics to stderr.
* i <input> : Specify input to compress (stdin by default)
* -o <output> : Specify output of compressed input (stdout by default)

decode:
* -v : Print decompression statistics to stderr.
* -i <input> : Specify input to decompress (stdin by default)
* -o <output> : Specify output of decompressed input (stdout by default)

Example: 
*./encode -v -i input.txt -o output.txt* would compress the contents of the input.txt file and print said compressed contents to output.txt. The verbose option was also selected so the compressed data size, uncompressed data size, and compression ratio would be displayed.
