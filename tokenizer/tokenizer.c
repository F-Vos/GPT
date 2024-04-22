#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#define MAX_TEXT_LENGTH 12000
#define MAX_VOCAB 600

typedef struct {
    int first_token;
    int second_token;
} Token;

typedef struct {
    int original_first_token;
    int original_second_token;
    int merged_token;
} MergeInfo;

void find_common_tokens(int *id, int length, Token *most_common_token) {
    int max_count = 0;
    int max_index = -1;
 
    for (int i = 0; i < length - 1; i++) {
        int first_token = id[i];
        int second_token = id[i + 1];
        int count = 1;

        for (int j = i + 1; j < length - 1; j++) {
            if (id[j] == first_token && id[j + 1] == second_token) {
                count++;
            }
        }

        if (count > max_count) {
            max_count = count;
            max_index = i;
        }
    }

    most_common_token->first_token = id[max_index];
    most_common_token->second_token = id[max_index + 1];
}

void merge_tokens(int *id, int *length, MergeInfo *merge_info, int *merge_count) {
    int num_merges = 0;

    while(num_merges < MAX_VOCAB) {
		Token most_common_token;
        find_common_tokens(id, *length, &most_common_token);
		int first_token = most_common_token.first_token;
        int second_token = most_common_token.second_token;
        int new_token = num_merges + 128; // Assign a new token for the merged pair
        num_merges++;

        for (int j = 0; j < *length - 1; j++) {
            if (id[j] == first_token && id[j + 1] == second_token) {
                // Store merge info
                merge_info[*merge_count].merged_token = new_token;
                merge_info[*merge_count].original_first_token = first_token;
                merge_info[*merge_count].original_second_token = second_token;
                (*merge_count)++;
				//printf("Merging (%c, %c) -> %d\n", first_token, second_token, new_token);
                // Replace the pair with the new token
                id[j] = new_token;

                // Shift subsequent tokens to the left to remove the second token
                for (int k = j + 2; k < *length; k++) {
                    id[k - 1] = id[k];
                }
                (*length)--; // Decrease the length of the text
            }
        }
    }
}

void encode_text(const char *filename, int *id, int *length, MergeInfo *merge_info, int *merge_count) {
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
	merge_tokens(id, length, merge_info, merge_count);
}


void unmerge_tokens(int *id, int *length, MergeInfo *merge_info, int merge_count) {
    int unmerged = 0;

    do {
        unmerged = 0;

        for (int i = 0; i < merge_count; i++) {
            for (int j = 0; j < *length; j++) {
                if (id[j] == merge_info[i].merged_token) {
                    id[j] = merge_info[i].original_first_token;

                    for (int k = *length; k > j + 1; k--) {
                        id[k] = id[k - 1];
                    }
                    id[j + 1] = merge_info[i].original_second_token;
                    (*length)++;
                    unmerged = 1;
                }
            }
        }
    } while (unmerged);
}

int main() {
	int length = 0;	
    int id[MAX_TEXT_LENGTH];
	MergeInfo merge_info[MAX_TEXT_LENGTH - 1];	

	int merge_count = 0;
	encode_text("taylor.txt", id, &length, merge_info, &merge_count);
	for (int i = 0; i < length; i++) {
        printf("%c", id[i]);
		
    }
	unmerge_tokens(id, &length, merge_info, merge_count);
	for (int i = 0; i < length; i++) {
        printf("%c", id[i]);
	}

    float original_length = (float)length;
    float compressed_length = length - (float)merge_count;
    float compression_ratio = original_length / compressed_length;
	printf("\n");
	printf("Original Length: %.0f\n", original_length);
    printf("Compressed Length: %.0f\n", compressed_length);
    printf("Compression Ratio: %.2f\n", compression_ratio);
    return 0;
}
