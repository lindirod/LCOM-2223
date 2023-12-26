#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define BIT(n) (1<<(n))

char *byte2bin(uint8_t n, char *binstr) 
{
    // one element per bit (0/1)
    uint8_t binary[8];

    int i = 0;
    for(i = 0; i < 8; i++) {
        binary[i] = n % 2;
        n = n / 2;
    }

    // printing binary array in reverse order
    for (int j = 7, k= 0; j >= 0; j--, k++)
        sprintf(binstr + k, "%d", binary[j]);
        
	return binstr;
}

void print_usage(char *name) {
	printf("Usage: %s <action> <byte> <bit no>\n", name);
	printf("\tWhere: <action> one of 'h'|'l'|'r'|'s'|'t'\n"
		"\t\t <byte> 8-bit integer in hexadecimal\n"
		"\t\t <bit no> of bit to operate on\n");
}

int main(int argc, char *argv[])
{
	char a;	// action: 'h', 'l', 'r', 's', 't'
	unsigned long n; // value to convert must be smaller than 256
	int bit; // bit to operate on: must be between 0 an 7
    char binary[9]; // array for binary representation of n,
					//  remember that in C strings are terminated with a 0
 
	// Validate command line arguments
    if(argc != 4) {
        printf("Please insert the correct quantity of arguments (three)");
    }

    if(argv[1] != 'h' || argv[1] != 'l' || argv[1] != 's' || argv[1] != 'r' || argv[1] != 't') {
        printf("Please input a correct option: \n");
        printf("h - test for bit 1 (high)\n");
        printf("l - test for bit 0 (low)");
        printf("s - set bit to 1");
        printf("r - reset bit to 0");
        printf("t - toggle bit value");
        return -1;
    }
	
	// Print to stdout the binary representation of n
	
	// Do what the user asked and print the result
	
    return 0;
}

int main(int argc, char **argv) {
    
    
    unsigned char value;

    if(argv[2] > 0xFF) {
        printf("Please input a value smaller than 256");
        return -1;
    }

    if(!argv[3] == 0 && !argv[3] == 7) {
        printf("Please input 0 for LSB");
        printf("Please input 7 for MSB");
        return -1;
    }

    printf("%c %s %d", argv[1], byte2bin(argv[2]), argv[3]);
    
}