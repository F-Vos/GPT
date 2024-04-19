#include <stdio.h>
#include <stdlib.h>


void create_pair_list(int *id, int length, int *count){
	
	for(int i=0;i<length-1;i++){
		for(int j=0;j<length-1;j++){
			if(id[i] == id[j]){
				printf("%d\n", id[i]);
			}
		}
	}
}



// Trainer


// Encoder
int char_to_ascii(char c) {
    return (unsigned int)c;
}

// Decoder
char ascii_to_char(int ascii_value) {
    return (char)ascii_value;
}


int main(){
	int id[] = {1, 2, 3, 1, 2, 3, 3, 1023, 1202, 12, 1201, 555, 555};
    int length = sizeof(id) / sizeof(id[0]);
  
	int count[length * length];

    create_pair_list(id, length, count);
    
    return 0;
}
