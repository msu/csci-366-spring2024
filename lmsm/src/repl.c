//
// Created by carson on 11/18/21.
//

#include "repl.h"
#include "stdio.h"
#include "string.h"
#include "assembler.h"
#include "firth.h"
#include "lmsm.h"
#include <stdlib.h>

char * repl_read_file(char * filename){
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Unknown file: '%s'\n\n", filename);
        return "";
    } else {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);
        char *contents = malloc(file_size + 1);
        fread(contents, 1, file_size, file);
        fclose(file);
        contents[file_size] = 0;
        return contents;
    }
}

int repl_load_file(lmsm *our_little_machine, char *filename) {
    char *contents = repl_read_file(filename);
    printf("Loading:\n%s\n\n", contents);
    asm_compilation_result *result = asm_assemble(contents);
    if (result->error) {
        printf("Assembly Error:\n%s\n\n", result->error);
        return 0;
    } else {
        lmsm_reset(our_little_machine);
        lmsm_load(our_little_machine, result->code, 100);
        return 1;
    }
}

int repl_comp_firth(lmsm *our_little_machine, char *filename) {
    char *contents = repl_read_file(filename);
    printf("Compiling:\n%s\n\n", contents);
    firth_compilation_result *compilation_result = firth_compile(contents);
    if (compilation_result->error) {
        printf("Compilation Error:\n%s\n\n", compilation_result->error);
        return 0;
    }
    printf("Assembly:\n%s\n\n", compilation_result->lmsm_assembly);
    asm_compilation_result *result = asm_assemble(compilation_result->lmsm_assembly);
    if (result->error) {
        printf("Assembly Error:\n%s\n\n", result->error);
        return 0;
    } else {
        lmsm_reset(our_little_machine);
        lmsm_load(our_little_machine, result->code, 100);
        return 1;
    }
}

int repl_load_firth(lmsm *our_little_machine, char *src) {
    printf("Compiling:\n%s\n\n", src);
    firth_compilation_result *compilation_result = firth_compile(src);
    if (compilation_result->error) {
        printf("Compilation Error:\n%s\n\n", compilation_result->error);
        return 0;
    }
    printf("Assembly:\n%s\n\n", compilation_result->lmsm_assembly);
    asm_compilation_result *result = asm_assemble(compilation_result->lmsm_assembly);
    if (result->error) {
        printf("Assembly Error:\n%s\n\n", result->error);
        return 0;
    } else {
        lmsm_reset(our_little_machine);
        lmsm_load(our_little_machine, result->code, 100);
        return 1;
    }
}


void repl_print_to_buffer(lmsm *our_little_machine, char* output) {
    int offset =       sprintf(output, "=========================== LMSM State ===========================\n");
    offset += sprintf(output + offset, "Program Counter:  %02d                      Current Instruction: %03d\n", our_little_machine->program_counter, our_little_machine->current_instruction);

    offset += sprintf(output + offset, "Accumulator:     %03d                      Status: %d\n", our_little_machine->accumulator, our_little_machine->status);

    offset += sprintf(output + offset, "Stack Pointer: ");
    offset += sprintf(output + offset, "%03d\n", our_little_machine->stack_pointer);

    offset += sprintf(output + offset, "  Value Stack: [");
    for (int i = TOP_OF_MEMORY; i >= our_little_machine->stack_pointer; --i) {
        if (i != TOP_OF_MEMORY) {
            offset += sprintf(output + offset, ", ", our_little_machine->memory[i]);
        }
        offset += sprintf(output + offset, "%03d", our_little_machine->memory[i]);
    }
    offset += sprintf(output + offset, "]\n");

    offset += sprintf(output + offset, "Return Address Pointer:");
    offset += sprintf(output + offset, " %03d\n", our_little_machine->return_address_pointer);
    offset += sprintf(output + offset, "  Return Address Stack: [");
    for (int i = 100; i <= our_little_machine->return_address_pointer; ++i) {
        if (i != 100) {
            offset += sprintf(output + offset, ", ", our_little_machine->memory[i]);
        }
        offset += sprintf(output + offset, "%03d", our_little_machine->memory[i]);
    }
    offset += sprintf(output + offset, "]\n\n");

    offset += sprintf(output + offset, "========================== Lower Memory ==========================\n");
    for (int i = 0; i < 100; ++i) {
        if (i % 10 == 0) {
            offset += sprintf(output + offset, "  %03d:  ", i);
        }
        int currentValue = our_little_machine->memory[i];
        if (i == our_little_machine->program_counter) {
            offset += sprintf(output + offset, "[%03d] ", currentValue);
        } else {
            offset += sprintf(output + offset, " %03d  ", currentValue);
        }
        if (i % 10 == 9) {
            offset += sprintf(output + offset, "\n");
        }
    }
    offset += sprintf(output + offset, "========================== Upper Memory ==========================\n");
    for (int i = 100; i <= TOP_OF_MEMORY; ++i) {
        if (i % 10 == 0) {
            offset += sprintf(output + offset, "  %03d:  ", i);
        }
        int currentValue = our_little_machine->memory[i];
        if (i == our_little_machine->return_address_pointer) {
            offset += sprintf(output + offset, "{%03d} ", currentValue);
        } else if (i == our_little_machine->stack_pointer) {
            offset += sprintf(output + offset, "[%03d] ", currentValue);
        } else {
            offset += sprintf(output + offset, " %03d  ", currentValue);
        }
        if (i % 10 == 9) {
            offset += sprintf(output + offset, "\n");
        }
    }
    offset += sprintf(output + offset, "==================================================================\n");
    sprintf(output + offset, "Output: %s\n", our_little_machine->output_buffer);
}

void repl_process_command(lmsm *our_little_machine, char *line) {
    line[strlen(line) - 1] = '\0'; // nuke newline char
    if (strcmp("x", line) == 0 || strcmp(line, "exit") == 0) {
        printf("goodbye");
        exit(EXIT_SUCCESS);  // We received an EOF
    } else if (strcmp("?", line) == 0 || strcmp(line, "help") == 0) {
        printf("LMSM Commands:\n");
        printf("  e[x]it - exits the emulator\n");
        printf("  help or ? - prints this message\n");
        printf("  [l]oad <file_name> - loads a new program into the LMSM from a file\n");
        printf("  [c]omp <file_name> - compiles a Firth file into LMSM assembly, then loads it into memory\n");
        printf("  [s]tep - executes one step in the LMSM\n");
        printf("  [r]un  - runs the current program\n");
        printf("  rese[t]  - resets the LMSM\n");
        printf("  [p]rint  - prints the state of the LMSM\n");
        printf("  [w]rite <num> <slot>  - saves the number in the given slot\n");
        printf("  [e]xec <num> - executes the raw asm_instruction\n");
        printf("  <any LMSM asm_instruction>  - executes a single asm_instruction (no label support)\n\n");
        printf("  f: <firth commands> - executes firth commands\n");
    } else if (strncmp("load ", line, strlen("load ")) == 0) {
        char fileName[100] = {0};
        strncat(fileName, line + 5, 100);
        repl_load_file(our_little_machine, fileName);
    } else if (strncmp("l ", line, strlen("l ")) == 0) {
        char fileName[100] = {0};
        strncat(fileName, line + 2, 100);
        repl_load_file(our_little_machine, fileName);
    } else if (strncmp("comp ", line, strlen("fout ")) == 0) {
        char fileName[100] = {0};
        strncat(fileName, line + 5, 100);
        repl_comp_firth(our_little_machine, fileName);
    } else if (strncmp("c ", line, strlen("l ")) == 0) {
        char fileName[100] = {0};
        strncat(fileName, line + 2, 100);
        repl_comp_firth(our_little_machine, fileName);
    } else if (strncmp("write ", line, strlen("write ")) == 0) {
        char *command = strtok(line, " ");
        char *num = strtok(NULL, " ");
        char *slot = strtok(NULL, " ");
        our_little_machine->memory[atoi(slot)] = atoi(num);
    } else if (strncmp("w ", line, strlen("w ")) == 0) {
        char *command = strtok(line, " ");
        char *num = strtok(NULL, " ");
        char *slot = strtok(NULL, " ");
        our_little_machine->memory[atoi(slot)] = atoi(num);
    } else if (strncmp("exec ", line, strlen("exec ")) == 0) {
        char *command = strtok(line, " ");
        char *raw = strtok(NULL, " ");
        lmsm_exec_instruction(our_little_machine, atoi(raw));
    } else if (strncmp("e ", line, strlen("e ")) == 0) {
        char *command = strtok(line, " ");
        char *raw = strtok(NULL, " ");
        lmsm_exec_instruction(our_little_machine, atoi(raw));
    } else if (strcmp("p", line) == 0 || strcmp("print", line) == 0) {
        char output[5000] = {0};
        repl_print_to_buffer(our_little_machine, output);
        printf("%s", output);
    } else if (strcmp("s", line) == 0 || strcmp("step", line) == 0) {
        lmsm_step(our_little_machine);
        char output[5000] = {0};
        repl_print_to_buffer(our_little_machine, output);
        printf("%s", output);
    } else if (strcmp("t", line) == 0 || strcmp("reset", line) == 0) {
        lmsm_reset(our_little_machine);
        char output[5000] = {0};
        repl_print_to_buffer(our_little_machine, output);
        printf("%s", output);
    } else if (strcmp("r", line) == 0 || strcmp("run", line) == 0) {
        printf("Running...\n\n");
        lmsm_run(our_little_machine);
    } else if (strncmp("f:", line, strlen("f:")) == 0) {
        printf("Loading Firth...\n\n");
        repl_load_firth(our_little_machine, line + 2);
    } else if (strcmp("\n", line) == 0) {
        printf("\n");
    } else if (strcmp("", line) == 0) {
        // ignore
    } else {
        char start[100] = {0};
        strncpy(start, line, 100);
        char *firstWord = strtok(start, " ");
        if (asm_is_instruction(firstWord)) {
            asm_compilation_result *result = asm_assemble(line);
            if (result->error) {
                printf("ERROR: %s\n", result->error);
            } else if(result->root->next != NULL) {
                printf("Only one asm_instruction can be executed at a time");
            } else {
                lmsm_exec_instruction(our_little_machine, result->code[0]);
                if (result->code[1]) {
                    lmsm_exec_instruction(our_little_machine, result->code[1]); // support 2-asm_instruction pseudo-instructions
                }
            }
            asm_delete_compilation_result(result);
        } else {
            printf("unknown command: %s\n", line);
        }
    }
}

void repl_start(lmsm *our_little_machine) {
    while (1) {
        size_t buffer_size = 2000;
        char *line = calloc(sizeof(char), buffer_size);
        printf("lmsm > ");
        if(getline(&line, &buffer_size, stdin) == -1){
            printf("goodbye");
            exit(EXIT_SUCCESS);  // We received an EOF
        }
        repl_process_command(our_little_machine, line);
        free(line);
    }
}

