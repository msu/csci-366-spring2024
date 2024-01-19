#ifndef LMSM_FIRTH_H
#define LMSM_FIRTH_H

typedef struct firth_token {
    char *value;
    struct firth_token *next;
} firth_token;

typedef struct firth_tokens {
    struct firth_token *start;
    struct firth_token *current;
    char * original_src;
} firth_tokens;

typedef enum firth_parse_element_type {
    SYMBOL,
    OP,
    NUMBER,
    ZERO_TEST,
    DEF,
    CALL,
    ERROR
} firth_parse_element_type;

typedef struct firth_parse_element {
    firth_token *token;  // start token
    firth_token *name;   // name, if any
    firth_parse_element_type type;  // type

    struct firth_parse_element *next_sibling; // next element in the Firth program

    struct firth_parse_elements *left_children;  // left children, if any
    struct firth_parse_elements *right_children; // right children, if any
} firth_parse_element;

typedef struct firth_parse_elements {
    struct firth_parse_element *first;
    struct firth_parse_element *last;
} firth_parse_elements;

typedef struct firth_compilation_result {
    firth_tokens * tokens;
    firth_parse_elements * root_elements;
    char lmsm_assembly[4000];   // the assembly for this program
    char * error;         // any error that occurred (e.g. a missing label)
    int label_num;
} firth_compilation_result;

// compiles a Firth program to LMSM assembly
firth_compilation_result * firth_compile(char *firth_src);

void firth_delete_compilation_result(firth_compilation_result * result);

#endif // LMSM_FIRTH_H