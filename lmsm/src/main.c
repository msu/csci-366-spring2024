#include "assembler.h"
#include "lmsm.h"
#include "repl.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("Little Man Stack Machine...\n\n");

    lmsm * our_little_machine = lmsm_create();
    if (argc == 2) {
        int result = repl_load_file(our_little_machine, argv[1]);
        if (result) {
            lmsm_run(our_little_machine);
        }
    } else {
        repl_start(our_little_machine);
    }
}