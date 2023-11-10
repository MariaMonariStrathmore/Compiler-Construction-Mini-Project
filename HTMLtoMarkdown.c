#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define token types
typedef enum {
    TAG_START,
    TAG_END,
    ATTRIBUTE_NAME,
    ATTRIBUTE_VALUE,
    TEXT_CONTENT,
    ERROR,
    END_OF_FILE
} TokenType;

// Define token structure
typedef struct {
    TokenType type;
    char *value;
} Token;

// Function to initialize a token
Token *createToken(TokenType type, const char *value) {
    Token *token = (Token *)malloc(sizeof(Token));
    if (token == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    token->type = type;
    token->value = strdup(value);

    return token;
}

// Function to free a token
void freeToken(Token *token) {
    free(token->value);
    free(token);
}

// Function to tokenize HTML content
Token *getNextToken(const char *input, int *index) {
    char buffer[1024];
    int bufferIndex = 0;

    // Skip whitespaces
    while (isspace(input[*index])) {
        (*index)++;
    }

    // Check for end of file
    if (input[*index] == '\0') {
        return createToken(END_OF_FILE, "");
    }

    // Check for tags
    if (input[*index] == '<') {
        (*index)++;
        return createToken(TAG_START, "<");
    } else if (input[*index] == '>') {
        (*index)++;
        return createToken(TAG_END, ">");
    }

    // Check for attributes
    while (isalnum(input[*index]) || input[*index] == '-') {
        buffer[bufferIndex++] = input[(*index)++];
    }
    buffer[bufferIndex] = '\0';

    if (bufferIndex > 0) {
        return createToken(ATTRIBUTE_NAME, buffer);
    }

    // Check for attribute values
    if (input[*index] == '=') {
        (*index)++;
        bufferIndex = 0;

        while (input[*index] != '\0' && !isspace(input[*index]) && input[*index] != '>') {
            buffer[bufferIndex++] = input[(*index)++];
        }
        buffer[bufferIndex] = '\0';

        return createToken(ATTRIBUTE_VALUE, buffer);
    }

    // Default to text content
    bufferIndex = 0;

    while (input[*index] != '\0' && input[*index] != '<') {
        buffer[bufferIndex++] = input[(*index)++];
    }
    buffer[bufferIndex] = '\0';

    return createToken(TEXT_CONTENT, buffer);
}

int main() {
    const char *htmlContent = "<html lang=\"en\"><head><title>Sample HTML</title></head><body><p>Hello, World!</p></body></html>";
    int index = 0;

    Token *token;
    do {
        token = getNextToken(htmlContent, &index);

        // Print token information
        printf("Type: %d, Value: %s\n", token->type, token->value);

        freeToken(token);
    } while (token->type != END_OF_FILE);

    return 0;
}