#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_TEXT_LENGTH 10000
#define MAX_VOCAB 600

typedef struct {
    int first_int;
    int second_int;
} Token;

void find_common_tokens(int *id, int length, Token *most_common_token) {
    int max_count = 0;
    int max_index = -1;

    for (int i = 0; i < length - 1; i++) {
        int first_int = id[i];
        int second_int = id[i + 1];
        int count = 1;

        for (int j = i + 1; j < length - 1; j++) {
            if (id[j] == first_int && id[j + 1] == second_int) {
                count++;
            }
        }

        if (count > max_count) {
            max_count = count;
            max_index = i;
        }
    }

    most_common_token->first_int = id[max_index];
    most_common_token->second_int = id[max_index + 1];
}



void train(FILE *input_file, FILE *vocab_file) {
    int id[MAX_TEXT_LENGTH];
    int length = 0;
    int num_merges = 0;
	int new_token = 0;
    Token most_common_token;
    
	// Read input from file
    int c;
    while ((c = fgetc(input_file)) != EOF && length < MAX_TEXT_LENGTH) {
		if(c>=0&&c<=127){
			id[length++] = c;
		}
    }
	fclose(input_file);


	while(num_merges < MAX_VOCAB) {
        find_common_tokens(id, length, &most_common_token);
		int first_int = most_common_token.first_int;
        int second_int = most_common_token.second_int;
		int merge_count = 0;
		for (int i = 0; i < length - 1; i++) {
            if (id[i] == first_int && id[i + 1] == second_int) {
				new_token = num_merges + 128;
                id[i] = new_token;
				merge_count++;
                // Shift subsequent tokens to the left to remove the second token
                for (int j = i + 2; j < length; j++) {
                    id[j - 1] = id[j];
                }

                length--; // Decrease the length of the text
            }
        }
		fprintf(vocab_file, "%d %d %d\n", merge_count, first_int, second_int);
		printf("Merging (%d, %d) -> %d\n", first_int, second_int, new_token);
		num_merges++;
    }
}



int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s train <input_file> <vocab file>\n", argv[0]);
        return 1;
    }

    char *input_file_name = argv[2];
    char *vocab_file_name = argv[3];

    FILE *input_file = fopen(input_file_name, "r");
    if (input_file == NULL) {
        printf("Failed to open input file.\n");
        return 1;
    }

    FILE *vocab_file = fopen(vocab_file_name, "r");
    if (vocab_file == NULL) {
        printf("Vocabulary file does not exist or cannot be opened. Creating a new one...\n");
		sleep(2);
        vocab_file = fopen(vocab_file_name, "w");
        if (vocab_file == NULL) {
            printf("Failed to create vocabulary file.\n");
            return 1;
        }
    }
	train(input_file, vocab_file);
	fclose(vocab_file);
	printf("Model trained and saved.\n");
    return 0;
}
