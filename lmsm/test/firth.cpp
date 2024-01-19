#include "gtest/gtest.h"

extern "C" {
#include "lmsm.h"
#include "assembler.h"
#include "firth.h"
}

//==========================================================================
// Instruction constructor tests
//==========================================================================

TEST(instruction_construction, multiplication_works_in_firth) {
    lmsm *the_machine = lmsm_create();
    firth_compilation_result *firth_result = firth_compile("2 3 * pop");
    asm_compilation_result *asm_result = asm_assemble(firth_result->lmsm_assembly);
    lmsm_load(the_machine, asm_result->code, 100);
    lmsm_run(the_machine);
    ASSERT_EQ(the_machine->accumulator, 6);
    lmsm_delete(the_machine);
    asm_delete_compilation_result(asm_result);
    firth_delete_compilation_result(firth_result);
}

TEST(instruction_construction, max_works_in_firth) {
    lmsm *the_machine = lmsm_create();
    firth_compilation_result *firth_result = firth_compile("3 2 max pop");
    asm_compilation_result *asm_result = asm_assemble(firth_result->lmsm_assembly);
    lmsm_load(the_machine, asm_result->code, 100);
    lmsm_run(the_machine);
    ASSERT_EQ(the_machine->accumulator, 3);
    lmsm_delete(the_machine);
    asm_delete_compilation_result(asm_result);
    firth_delete_compilation_result(firth_result);
}
