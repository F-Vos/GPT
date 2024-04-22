#include <stdio.h>
#include <stdlib.h>

#define MAX_TEXT_LENGTH 10000
#define MAX_VOCAB 2000

typedef struct {
    int first_token;
    int second_token;
} Token;

typedef struct {
    int original_first_token;
    int original_second_token;
    int merged_token;
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

    for (int i = 0; i < *length - 1 && num_merges < MAX_VOCAB && *merge_count < MAX_TEXT_LENGTH - 1; i++) {
		Token most_common_token;
        find_common_tokens(id, *length, &most_common_token);
		int first_token = most_common_token.first_token;
        int second_token = most_common_token.second_token;
        int new_token = i + 128; // Assign a new token for the merged pair
        num_merges++;

        for (int j = 0; j < *length - 1; j++) {
            if (id[j] == first_token && id[j + 1] == second_token) {
                // Store merge info
                merge_info[*merge_count].merged_token = new_token;
                merge_info[*merge_count].original_first_token = first_token;
                merge_info[*merge_count].original_second_token = second_token;
                (*merge_count)++;

				printf("Merging (%c, %c) -> %d\n", first_token, second_token, new_token);
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

void unmerge_tokens(int *id, int *length, MergeInfo *merge_info, int merge_count) {
    // Flag to keep track of whether any tokens were unmerged in this iteration
    int unmerged = 0;

    // Iterate through the merged tokens and replace them with the original tokens
    do {
        unmerged = 0; // Reset the unmerged flag at the beginning of each iteration

        for (int i = 0; i < merge_count; i++) {
            // Find the merged token in the text and replace it with the original tokens
            for (int j = 0; j < *length; j++) {
                if (id[j] == merge_info[i].merged_token) {
                    // Replace the merged token with the original first token
                    id[j] = merge_info[i].original_first_token;

                    // Shift subsequent tokens to the right to make space for the second token
                    for (int k = *length; k > j + 1; k--) {
                        id[k] = id[k - 1];
                    }

                    // Insert the original second token after the first token
                    id[j + 1] = merge_info[i].original_second_token;

                    // Increment the length of the text
                    (*length)++;

                    // Set the unmerged flag to indicate that a token was unmerged
                    unmerged = 1;
                }
            }
        }
    } while (unmerged); // Continue looping until no more tokens are unmerged
}

int main() {
	int length = 0;	
    int id[MAX_TEXT_LENGTH];
	int input_char;
	MergeInfo merge_info[MAX_TEXT_LENGTH - 1];	
    // Encode the text
    encode_text("taylor.txt", id, &length);


    // Sort the pairs based on counts
	int merge_count = 0;
	merge_tokens(id, &length, merge_info, &merge_count);
	for (int i = 0; i < length; i++) {
        printf("%c", id[i]);
		
    }
	unmerge_tokens(id, &length, merge_info, merge_count);
	printf("\n\n\n");
	for (int i = 0; i < length; i++) {
        printf("%c", id[i]);
	}
	printf("\n");

	// Calculate compression ratio
    float original_length = (float)length;
    float compressed_length = length - (float)merge_count;
    float compression_ratio = original_length / compressed_length;

	printf("Original Length: %.0f\n", original_length);
    printf("Compressed Length: %.0f\n", compressed_length);
    printf("Compression Ratio: %.2f\n", compression_ratio);
    return 0;
}
