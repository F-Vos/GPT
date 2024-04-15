#include <stdio.h>


// Encoder
int char_to_ascii(char c) {
    return (unsigned int)c;
}

// Decoder
char ascii_to_char(int ascii_value) {
    return (char)ascii_value;
}

int main(){
    const char *test_text = "Привет, мир! 안녕하세요 こんにちは مرحبا To be, or not to be, that is the question:\n"
                             "Whether 'tis nobler in the mind to suffer\n"
                             "The slings and arrows of outrageous fortune,\n"
                             "Or to take arms against a sea of troubles\n"
                             "And by opposing end them.";

    for (int i = 0; test_text[i] != '\0'; i++) {
        int ascii_value = char_to_ascii(test_text[i]);
        printf("%d, ", ascii_value); 
    }

	int ascii_values[] = {-48, -97, -47, -128, -48, -72, -48, -78, -48, -75, -47, -126, 44, 32, -48, -68, -48, -72, -47, -128, 33, 32, -20, -107, -120, -21, -123, -107, -19, -107, -104, -20,
-124, -72, -20, -102, -108, 32, -29, -127, -109, -29, -126, -109, -29, -127, -85, -29, -127, -95, -29, -127, -81, 32, -39, -123, -40, -79, -40, -83, -40, -88, -40, -89, 32, 84, 111, 32, 98, 101, 44, 32, 111, 114, 32, 110, 111, 116, 32, 116, 111, 32, 98, 101, 44, 32, 116, 104, 97, 116, 32, 105, 115, 32, 116, 104, 101, 32, 113, 117, 101, 115, 116, 105, 111, 110, 58, 10, 87, 104, 101, 116, 104, 101, 114, 32, 39, 116, 105, 115, 32, 110, 111, 98, 108, 101, 114, 32, 105, 110, 32, 116, 104, 101, 32, 109, 105, 110, 100, 32, 116, 111, 32, 115, 117, 102, 102, 101, 114, 10, 84, 104, 101, 32, 115, 108, 105, 110, 103, 115, 32, 97, 110, 100, 32, 97, 114, 114, 111, 119, 115, 32, 111, 102, 32, 111, 117, 116, 114, 97, 103, 101, 111, 117, 115, 32, 102, 111, 114, 116, 117, 110, 101, 44, 10, 79, 114, 32, 116, 111, 32, 116, 97, 107, 101, 32, 97, 114, 109, 115, 32, 97, 103, 97, 105, 110, 115, 116, 32, 97, 32, 115, 101, 97, 32, 111, 102, 32, 116, 114, 111, 117, 98, 108, 101, 115, 10, 65, 110, 100,
32, 98, 121, 32, 111, 112, 112, 111, 115, 105, 110, 103, 32, 101, 110, 100, 32, 116, 104, 101, 109, 46}; 

	int length = sizeof(ascii_values) / sizeof(ascii_values[0]);

    for (int i = 0; i < length; i++) {
        printf("%c", ascii_to_char(ascii_values[i])); // Print character corresponding to each ASCII value
	}
	printf("\n");
	return 0;
}
