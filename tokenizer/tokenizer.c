#include <stdio.h>
#include <stdlib.h>

#define MAX_TEXT_LENGTH 10000
#define MAX_VOCAB 150



typedef struct {
    int merged_token;
    int original_first_token;
    int original_second_token;
} MergeInfo;

void encode_text(const char *filename, int *id, int *length) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open file.\n");
        exit(1);
    }

    int j = 0;
    int input_char;
    while ((input_char = fgetc(fp)) != EOF && j < MAX_TEXT_LENGTH - 1) {
        if (input_char >= 0 && input_char <= 127) {
            id[j] = (int)input_char;
            j++;
        }
    }
    fclose(fp);

    *length = j;
}


void create_token_list(int *id, int length, int (*count)[3]){
	for(int i=0;i<length-1;i++){
		count[i][0] = id[i];
		count[i][1] = id[i+1];
		count[i][2] = 0;
	}
}

void find_common_tokens(int length, int (*count)[3]){
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

void merge_tokens(int *id, int length, int (*count)[3], MergeInfo *merge_info, int *merge_count){
	int num_merges = MAX_VOCAB - 128;
	for (int i = 0; i < length - 1 && num_merges < MAX_VOCAB; i++) {
		int first_token = count[i][0];
		int second_token = count[i][1];
		int new_token = i+128;
		num_merges++;
		printf("%d from %d\n", num_merges, MAX_VOCAB);
		for (int j = 0; j < length; j++) {
                if (id[j] == first_token && id[j + 1] == second_token) {
					merge_info[*merge_count].merged_token = new_token;
                    merge_info[*merge_count].original_first_token = first_token;
                    merge_info[*merge_count].original_second_token = second_token;
                    (*merge_count)++;
                    id[j] = new_token;
                    id[j+1] = -1; // Mark the second token for removal
					j++;
                }
        }
	}

	// Remove marked tokens
    int j = 0;
    for (int i = 0; i < length; i++) {
        if (id[i] != -1) {
            id[j++] = id[i];
        }
    }
	printf("After removal of marked tokens:\n");
    for (int i = 0; i < j; i++) {
        printf("%d ", id[i]);
    }
    printf("\n");
}

void bubble_sort(int (*arr)[3], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j][2] < arr[j + 1][2]) {
                int temp0 = arr[j][0];
                int temp1 = arr[j][1];
                int temp2 = arr[j][2];
                arr[j][0] = arr[j + 1][0];
                arr[j][1] = arr[j + 1][1];
                arr[j][2] = arr[j + 1][2];
                arr[j + 1][0] = temp0;
                arr[j + 1][1] = temp1;
                arr[j + 1][2] = temp2;
            }
        }
    }
}

int main() {
	int length;
    int id[MAX_TEXT_LENGTH];
    int count[MAX_TEXT_LENGTH][3];
    int input_char;
	MergeInfo merge_info[10000];
    // Encode the text
    encode_text("taylor.txt", id, &length);

    // Create token list
    create_token_list(id, length, count);

    // Find common tokens
    find_common_tokens(length, count);

    // Sort the pairs based on counts
    bubble_sort(count, length - 1);
	int merge_count = 0;
	merge_tokens(id, length, count, merge_info, &merge_count);


	
	printf("Merged tokens:\n");
    for (int i = 0; i < merge_count; i++) {
        printf("Merged token: %d, %d -> %d\n", 
               merge_info[i].original_first_token,
               merge_info[i].original_second_token, 
               merge_info[i].merged_token);
    } 

    return 0;
}
