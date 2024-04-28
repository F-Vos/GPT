#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_TEXT_LENGTH 10000
#define MAX_VOCAB 400
#define TOTAL_VOCAB_SIZE 100240

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


void train(FILE *input_file, const char *vocab_file_name) {
    int id[MAX_TEXT_LENGTH];
    int length = 0;
    int num_merges = 0;
    int new_token = 0;
    Token most_common_token;

    // Read input from file
    int c;
    while ((c = fgetc(input_file)) != EOF && length < MAX_TEXT_LENGTH) {
        if (c >= 0 && c <= 127) {
            id[length++] = c;
        }
    }
    fclose(input_file);

    int vocab[TOTAL_VOCAB_SIZE][4];
	int vocab_size = 0;

    FILE *vocab_file = fopen(vocab_file_name, "r");
    if (vocab_file == NULL) {
        printf("Failed to open vocabulary file for reading.\n");
        return;
    }

	// Read existing vocabulary from vocab_file
    rewind(vocab_file); // Reset file pointer to the beginning
    while (fscanf(vocab_file, "%d %d %d %d", &vocab[vocab_size][0], &vocab[vocab_size][1], &vocab[vocab_size][2], &vocab[vocab_size][3]) == 4 && vocab_size < TOTAL_VOCAB_SIZE) {
        vocab_size++;
    }
	if (ferror(vocab_file)) {
		 perror("Error reading vocabulary file");
		 fclose(vocab_file);
		 return;
	}

	fclose(vocab_file);

	printf("Initial vocabulary:\n");
    for (int i = 0; i < vocab_size; i++) {
        printf("%d %d %d %d\n", vocab[i][0], vocab[i][1], vocab[i][2], vocab[i][3]);
    }
    printf("\n");
	
	// Open vocab_file in write mode to write updated vocabulary
    FILE *vocab_file_w = fopen(vocab_file_name, "w");
    if (vocab_file_w == NULL) {
		printf("Failed to open vocabulary file for writing.\n");
		return;
    }
    
	while (num_merges < MAX_VOCAB) {
        find_common_tokens(id, length, &most_common_token);
        int first_int = most_common_token.first_int;
        int second_int = most_common_token.second_int;
        int merge_count = 0;
        for (int i = 0; i < length - 1; i++) {
            if (id[i] == first_int && id[i + 1] == second_int) {
                new_token = num_merges + 128;
                

                // Perform lookup and update
                int found = 0;
                for (int k = 0; k < vocab_size; k++) {
                    if (vocab[k][2] == first_int && vocab[k][3] == second_int) {
                        new_token = vocab[k][1];
                        vocab[k][0]++; // Increment merge_count
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    // Insert new entry into vocabulary
                    vocab[vocab_size][0] = 1;
					if(vocab[vocab_size -1][1] < 128){
						vocab[vocab_size][1] = 128;
					} else {
						vocab[vocab_size][1] = (vocab[vocab_size - 1][1]) + 1; 
					}
                    vocab[vocab_size][2] = first_int;
                    vocab[vocab_size][3] = second_int;
					printf("Not found! adding %d\n", (vocab[vocab_size - 1][1]) + 1);
                    vocab_size++;
                }
                
				id[i] = new_token;
                merge_count++;
                
				// Shift subsequent tokens to the left to remove the second token
				for (int j = i + 2; j < length; j++) {
                    id[j - 1] = id[j];
                }
                length--; // Decrease the length of the text
            }
        }
		printf("Merging (%d, %d) -> %d\n", first_int, second_int, new_token);
        num_merges++;
    }
    // Write to vocab_file
    for (int i = 0; i < vocab_size; i++) {
		fprintf(vocab_file_w, "%d %d %d %d\n", vocab[i][0], vocab[i][1], vocab[i][2], vocab[i][3]);
    }
    fclose(vocab_file_w);
    printf("Model trained and saved.\n");
}

void rerank_vocab(const char *vocab_file_name){
	int vocab[TOTAL_VOCAB_SIZE][4];
	int vocab_size = 0;
	FILE *vocab_file = fopen(vocab_file_name, "r");
    if (vocab_file == NULL) {
        printf("Failed to open vocabulary file for reading.\n");
        return;
    }

	// Read existing vocabulary from vocab_file
    rewind(vocab_file); // Reset file pointer to the beginning
    while (fscanf(vocab_file, "%d %d %d %d", &vocab[vocab_size][0], &vocab[vocab_size][1], &vocab[vocab_size][2], &vocab[vocab_size][3]) == 4 && vocab_size < TOTAL_VOCAB_SIZE) {
        vocab_size++;
	}
	if (ferror(vocab_file)) {
		 perror("Error reading vocabulary file");
		 fclose(vocab_file);
		 return;
	}
    fclose(vocab_file);

	for (int i = 0; i < vocab_size - 1; i++) {
        for (int j = 0; j < vocab_size - i - 1; j++) {
            if (vocab[j][0] < vocab[j + 1][0]) {
                // Swap rows
                for (int k = 0; k < 4; k++) {
                    int temp = vocab[j][k];
                    vocab[j][k] = vocab[j + 1][k];
                    vocab[j + 1][k] = temp;
                }
            }
        }
    }
	int new_value = 128;
	for (int i = 0; i < vocab_size; i++) {
        vocab[i][1] = new_value++;
    }	
	
	// Open vocab_file in write mode to write updated vocabulary
	FILE *vocab_file_w = fopen(vocab_file_name, "w");
    if (vocab_file_w == NULL) {
        printf("Failed to open vocabulary file for writing.\n");
        return;
    }
    // Write to vocab_file
    for (int i = 0; i < vocab_size; i++) {
        fprintf(vocab_file_w, "%d %d %d %d\n", vocab[i][0], vocab[i][1], vocab[i][2], vocab[i][3]);
    }
    fclose(vocab_file_w);
}

char *encode(const char *input_string, const char *vocab_file_name);
char *decode(const char *encoded_string, const char *vocab_file_name);
void demo(const char *input_string, const char *vocab_file_name);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <option> <input_string> <vocab_file>\n", argv[0]);
        printf("Options:\n");
        printf("  train   Train the model\n");
        printf("  encode  Encode the input string\n");
        printf("  decode  Decode the input string\n");
        printf("  demo    Encode and decode the input string\n");
        return 1;
    }

    char *option = argv[1];
    char *input_string = argv[2];
    char *vocab_file_name = argv[3];

    if (strcmp(option, "train") == 0) {
        FILE *input_file = fopen(input_string, "r");
        if (input_file == NULL) {
            printf("Failed to open input file.\n");
            return 1;
        }
        train(input_file, vocab_file_name);
    } else if (strcmp(option, "encode") == 0) {
		return 1;
    } else if (strcmp(option, "decode") == 0) {
		return 1;
    } else if (strcmp(option, "demo") == 0) {
		return 1;
    } else {
        printf("Invalid option.\n");
        return 1;
    }

    return 0;
}
