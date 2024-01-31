#include <stdio.h>

int main(){
    unsigned short value = 0xD085;
    unsigned char high_byte = (value >> 8);
    unsigned char low_byte = value & 0xFF;

    printf("High byte: 0x%02X\n", high_byte);
    printf("Low byte: 0x%02X\n", low_byte);
}