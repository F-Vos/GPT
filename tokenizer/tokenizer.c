#include <stdio.h>
#include <stdlib.h>


void create_pair_list(int *id, int length, int (*count)[3]){
	for(int i=0;i<length-1;i++){
		count[i][0] = id[i];
		count[i][1] = id[i+1];
		count[i][2] = 0;
	}
}

void find_common_pairs(int length, int (*count)[3]){
	for(int i=0;i<length-1;i++) {
        if(count[i][2] == -1) {
            continue; 
        }

        for(int j=i+1;j<length-1;j++) {
            if(count[j][0] == count[i][0] && count[j][1] == count[i][1]) {
				++count[i][2];
				count[j][2] = -1; // Mark as counted
            }
        }
    }

}

void merge_pairs(int length, int (*count)[3]){
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
	int count[length-1][3];
    
	create_pair_list(id, length, count);
    find_common_pairs(length, count);
   
	printf("Pairs and their counts:\n");
    for (int i = 0; i < length - 1; i++) {
        printf("(%d, %d): %d\n", count[i][0], count[i][1], count[i][2]);
    }
	return 0;
}
