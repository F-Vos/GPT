#include <stdio.h>


// Encoder
int char_to_ascii(char c) {
    return (int)c;
}

// Decoder
char ascii_to_char(int ascii_value) {
    return (char)ascii_value;
}

int main(){
    const char *test_text = "To be, or not to be, that is the question:\n"
                             "Whether 'tis nobler in the mind to suffer\n"
                             "The slings and arrows of outrageous fortune,\n"
                             "Or to take arms against a sea of troubles\n"
                             "And by opposing end them.";

    for (int i = 0; test_text[i] != '\0'; i++) {
        int ascii_value = char_to_ascii(test_text[i]);
        printf("%d ", ascii_value); 
    }

	return 0;
}
