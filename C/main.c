#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct node {
    // an operator or a number
    char* value;
    struct node* right;
    struct node* left;
} Node;

// parse line into array of strings 
char** parse_line(char* line) {
    // ToDo-4: the current code only works when the input 
    // is separated with spaces. Modify parse_line so it 
    // also supports inputs that have no spaces.  
    // for instance, parse_line should return {"2", "*", "3", "/", "5", NULL}
    // for both 2*3/5 and 2 * 3 / 5 inputs 
    int i = 0;
    char** tokenized = (char**) malloc(256*sizeof(char*));
    char* chunk = strtok(line, " ");
    while (chunk != NULL) {
        tokenized[i++] = chunk;
        chunk = strtok(NULL, " ");
    }
    tokenized[i] = NULL;
    return tokenized;
}

double operator(double a, double b, char* op) {
    // ToDo-1: complete this section for - / * 
    if (strcmp(op, "+") == 0) return a+b;
    
    return 0;
}

double calculate(Node* root) {
    if (root == NULL) { return 0; }
    if (root->left == NULL && root->right == NULL) {
        char* ptr;
        return strtod(root->value, &ptr);
    }
    double a = calculate(root->left);
    double b = calculate(root->right);
    double result = operator(a, b, root->value);
    return result;
}

int find(char** input, char* target) {
    int i = 0;
    while (input[i] != NULL){
        if (strcmp(input[i], target) == 0)
            return i;
        i++;
    }
    return -1;
}

int len(char** parsed) {
    int i = 0;
    while (parsed[i] != NULL) { i++; }
    return i;
}

Node* build_tree(char** parsed) {
    int index;
    int leaf = 1;
    Node* root = (Node*) malloc(sizeof(Node));
    while ((index = find(parsed, "+")) != -1 || (index = find(parsed, "-")) != -1){
        leaf = 0;
        root->value = parsed[index];
        parsed[index] = NULL;
        root->left = build_tree(parsed);
        root->right = build_tree(&parsed[index + 1]);
    }
    for (index = len(parsed) - 1; index > 0 && parsed[index] != NULL; index--){
        if (strcmp(parsed[index], "/") == 0 || strcmp(parsed[index], "*") == 0){
            leaf = 0;
            root->value = parsed[index];
            parsed[index] = NULL;
            root->left = build_tree(parsed);
            root->right = build_tree(&parsed[index + 1]);
        }
    }
    if (leaf == 1) {
        root->value = parsed[0];
        root->left = NULL;
        root->right = NULL;
    }
    return root;
}

void destroy_tree(Node* root) {
    if (root == NULL) { return; }
    destroy_tree(root->left);
    destroy_tree(root->right);
    free(root);
}

int main(int argc, char* argv[]) {
    char* buff = 0; 
    size_t size = 256; 
    if (argc == 1) {
        printf("Usage: ./cal [filename]\n");
        return 1;
    }
    char* filename = argv[1];
    // ToDo-2: create a file if it doesn't exist and set its permissions to 
    // allow everybody to read and execute, but only the owner can write to it (i.e., rwxr-xr-x) 
    // int fd = open(filename, , );

    // ToDo-2: uncomment me for error handling
    // if (fd == -1) {
    // 	perror("File error");
    // 	exit(0);
    // }
    printf("$ ");
    while (getline(&buff, &size, stdin) > 0) {
        char output[256] = {0};
        char** parsed = parse_line(buff);
        Node* root = build_tree(parsed);
        double answer = calculate(root);
        printf("%f\n", answer);
        // ToDo-3: create a string from the answer (note that answer is not a string) and write it into the file 
        destroy_tree(root);
        free(parsed);
        printf("$ ");
    }
    // ToDo-2: uncomment me (don't forget to close the file)
    // close(fd);
    return 0;
}





