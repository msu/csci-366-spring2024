#include "gtest/gtest.h"
extern "C" {
#include "lmsm.h"
}

TEST(lmsm_machine_suite,test_add_instruction_works){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 0;
    the_machine->memory[0] = 10;
    lmsm_exec_instruction(the_machine, 100); // ADD 00
    ASSERT_EQ(the_machine->accumulator, 10);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_add_instruction_properly_caps_values){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 995;
    the_machine->memory[0] = 10;
    lmsm_exec_instruction(the_machine, 100); // ADD 00
    ASSERT_EQ(the_machine->accumulator, 999);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sub_instruction_works){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 20;
    the_machine->memory[0] = 10;
    lmsm_exec_instruction(the_machine, 200); // SUB 00
    ASSERT_EQ(the_machine->accumulator, 10);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sub_instruction_properly_caps_values){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = -995;
    the_machine->memory[0] = 10;
    lmsm_exec_instruction(the_machine, 200); // SUB 00
    ASSERT_EQ(the_machine->accumulator, -999);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_store_instruction_works){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 300); // STA 00
    ASSERT_EQ(the_machine->memory[0], 10);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_load_instruction_works){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 0;
    the_machine->memory[20] = 10;
    lmsm_exec_instruction(the_machine, 520); // LDA 20
    ASSERT_EQ(the_machine->accumulator, 10);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_branch_instruction_works){
    lmsm *the_machine = lmsm_create();
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_exec_instruction(the_machine, 620); // BRA 20
    ASSERT_EQ(the_machine->program_counter, 20);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_branch_if_zero_instruction_works_when_val_zero){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 0;
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_exec_instruction(the_machine, 720); // BRZ 20
    ASSERT_EQ(the_machine->program_counter, 20);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_branch_if_zero_instruction_works_when_val_not_zero){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_exec_instruction(the_machine, 720); // BRZ 20
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_branch_if_positive_instruction_works_when_val_zero){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 0;
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_exec_instruction(the_machine, 820); // BRZ 20
    ASSERT_EQ(the_machine->program_counter, 20);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_branch_if_positive_instruction_works_when_val_positive){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_exec_instruction(the_machine, 820); // BRZ 20
    ASSERT_EQ(the_machine->program_counter, 20);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_branch_if_positive_instruction_works_when_val_not_positive){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = -10;
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_exec_instruction(the_machine, 820); // BRZ 20
    ASSERT_EQ(the_machine->program_counter, 0);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_output_instruction){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 902); // OUT 20
    lmsm_exec_instruction(the_machine, 902); // OUT 20
    ASSERT_STREQ(the_machine->output_buffer, "10 10 ");
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_halt_instruction){
    lmsm *the_machine = lmsm_create();
    ASSERT_EQ(the_machine->status, machine_status::STATUS_READY);
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 000); // HLT
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_load_immediate_instruction){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 0;
    lmsm_exec_instruction(the_machine, 499); // LDI 99
    ASSERT_EQ(the_machine->accumulator, 99);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_push_instruction){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    ASSERT_EQ(the_machine->accumulator, 10);
    ASSERT_EQ(the_machine->memory[199], 10);
    ASSERT_EQ(the_machine->stack_pointer, 199);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_pop_instruction_removes_top_of_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    ASSERT_EQ(the_machine->accumulator, 10);
    ASSERT_EQ(the_machine->memory[199], 10);
    ASSERT_EQ(the_machine->stack_pointer, 199);

    the_machine->accumulator = 0;

    lmsm_exec_instruction(the_machine, 921); // SPOP
    ASSERT_EQ(the_machine->accumulator, 10);
    ASSERT_EQ(the_machine->stack_pointer, 200);

    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_pop_instruction_enters_error_state_if_nothing_to_pop){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 921); // SPOP
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_BAD_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_dup_instruction_duplicates_top_of_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    lmsm_exec_instruction(the_machine, 922); // SDUP
    ASSERT_EQ(the_machine->memory[199], 10);
    ASSERT_EQ(the_machine->memory[198], 10);
    ASSERT_EQ(the_machine->stack_pointer, 198);
    ASSERT_EQ(the_machine->accumulator, 10);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_drop_instruction_removes_top_of_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    ASSERT_EQ(the_machine->accumulator, 10);
    ASSERT_EQ(the_machine->memory[199], 10);
    ASSERT_EQ(the_machine->stack_pointer, 199);

    the_machine->accumulator = 0;

    lmsm_exec_instruction(the_machine, 923); // SDROP
    ASSERT_EQ(the_machine->accumulator, 0);
    ASSERT_EQ(the_machine->stack_pointer, 200);

    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_drop_instruction_enters_error_state_if_nothing_to_pop){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 923); // SDROP
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_BAD_STACK);
    lmsm_delete(the_machine);
}


TEST(lmsm_machine_suite,test_swap_instruction_swaps_top_of_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    the_machine->accumulator = 20;
    lmsm_exec_instruction(the_machine, 920); // SPUSH

    ASSERT_EQ(the_machine->accumulator, 20);
    ASSERT_EQ(the_machine->memory[199], 10);
    ASSERT_EQ(the_machine->memory[198], 20);
    ASSERT_EQ(the_machine->stack_pointer, 198);

    lmsm_exec_instruction(the_machine, 924); // SSWAP

    ASSERT_EQ(the_machine->accumulator, 20);
    ASSERT_EQ(the_machine->memory[199], 20);
    ASSERT_EQ(the_machine->memory[198], 10);
    ASSERT_EQ(the_machine->stack_pointer, 198);

    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_swap_instruction_enters_error_state_if_nothing_to_swap){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 924); // SSWAP
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_BAD_STACK);
    lmsm_delete(the_machine);
}


TEST(lmsm_machine_suite,test_sadd_instruction_adds_the_values_on_the_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    lmsm_exec_instruction(the_machine, 922); // SDUP
    lmsm_exec_instruction(the_machine, 930); // SADD
    ASSERT_EQ(the_machine->accumulator, 10);
    ASSERT_EQ(the_machine->memory[199], 20);
    ASSERT_EQ(the_machine->stack_pointer, 199);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sadd_instruction_properly_caps_values){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 500;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    lmsm_exec_instruction(the_machine, 922); // SDUP
    lmsm_exec_instruction(the_machine, 930); // SADD
    ASSERT_EQ(the_machine->accumulator, 500);
    ASSERT_EQ(the_machine->memory[199], 999);
    ASSERT_EQ(the_machine->stack_pointer, 199);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sadd_instruction_enters_error_state_if_not_enough_elts){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 923); // SADD
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_BAD_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_ssub_instruction_subtracts_the_values_on_the_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    lmsm_exec_instruction(the_machine, 922); // SDUP
    lmsm_exec_instruction(the_machine, 931); // SSUB
    ASSERT_EQ(the_machine->accumulator, 10);
    ASSERT_EQ(the_machine->memory[199], 0);
    ASSERT_EQ(the_machine->stack_pointer, 199);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_ssub_instruction_properly_caps_values){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = -500;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    the_machine->accumulator = 500;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    lmsm_exec_instruction(the_machine, 931); // SSUB
    ASSERT_EQ(the_machine->accumulator, 500);
    ASSERT_EQ(the_machine->memory[199], -999);
    ASSERT_EQ(the_machine->stack_pointer, 199);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_ssub_instruction_subtracts_the_values_on_the_stack_in_the_right_order){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    the_machine->accumulator = 20;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    lmsm_exec_instruction(the_machine, 931); // SSUB
    ASSERT_EQ(the_machine->accumulator, 20);
    ASSERT_EQ(the_machine->memory[199], -10);
    ASSERT_EQ(the_machine->stack_pointer, 199);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_ssub_instruction_enters_error_state_if_not_enough_elts){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 924); // SSUB
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_BAD_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smax_instruction_sets_max_val_on_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    ASSERT_EQ(the_machine->memory[199], 10);

    the_machine->accumulator = 20;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    lmsm_exec_instruction(the_machine, 934); // SMAX
    ASSERT_EQ(the_machine->accumulator, 20);
    ASSERT_EQ(the_machine->memory[199], 20); // top stack value should now be 20 rather than 10
    ASSERT_EQ(the_machine->stack_pointer, 199);

    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smax_instruction_enters_error_state_if_not_enough_elts){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 934); // SMAX
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_BAD_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smin_instruction_sets_min_val_on_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 20;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    ASSERT_EQ(the_machine->memory[199], 20);

    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    lmsm_exec_instruction(the_machine, 935); // SMIN
    ASSERT_EQ(the_machine->accumulator, 10);
    ASSERT_EQ(the_machine->memory[199], 10); // top stack value should now be 10 rather than 20
    ASSERT_EQ(the_machine->stack_pointer, 199);

    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smin_instruction_enters_error_state_if_not_enough_elts){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 935); // SMIN
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_BAD_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smul_instruction_multiplies_the_values_on_the_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    lmsm_exec_instruction(the_machine, 922); // SDUP
    lmsm_exec_instruction(the_machine, 932); // SMUL
    ASSERT_EQ(the_machine->accumulator, 10);
    ASSERT_EQ(the_machine->memory[199], 100);
    ASSERT_EQ(the_machine->stack_pointer, 199);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smul_instruction_properly_caps_values){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 50;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    lmsm_exec_instruction(the_machine, 922); // SDUP
    lmsm_exec_instruction(the_machine, 932); // SMUL
    ASSERT_EQ(the_machine->accumulator, 50);
    ASSERT_EQ(the_machine->memory[199], 999);
    ASSERT_EQ(the_machine->stack_pointer, 199);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_smul_instruction_enters_error_state_if_not_enough_elts){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 932); // SMUL
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_BAD_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sdiv_instruction_divides_the_values_on_the_stack){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 20;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH
    lmsm_exec_instruction(the_machine, 933); // SDIV
    ASSERT_EQ(the_machine->accumulator, 10);
    ASSERT_EQ(the_machine->memory[199], 2);
    ASSERT_EQ(the_machine->stack_pointer, 199);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,test_sdiv_instruction_enters_error_state_if_not_enough_elts){
    lmsm *the_machine = lmsm_create();
    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 933); // SDIV
    ASSERT_EQ(the_machine->status, machine_status::STATUS_HALTED);
    ASSERT_EQ(the_machine->error_code, error_code::ERROR_BAD_STACK);
    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite, test_call_instruction_jumps_to_correct_location_and_consumes_top_of_stack){
    lmsm *the_machine = lmsm_create();

    the_machine->accumulator = 10;
    lmsm_exec_instruction(the_machine, 920); // SPUSH

    the_machine->program_counter = 5;                                 // the CALL is in position 5

    ASSERT_EQ(the_machine->stack_pointer, 199);                        // 10 should now be on the top of the stack
    ASSERT_EQ(the_machine->memory[199], 10);

    lmsm_exec_instruction(the_machine, 910); // JAL

    ASSERT_EQ(the_machine->program_counter, 10);                       // program counter should be updated to the function

    ASSERT_EQ(the_machine->return_address_pointer, 100);               // return_address_pointer should be bumped
    ASSERT_EQ(the_machine->memory[100], 5);                            // the value in the memory slot the return_address_pointer points to the current program counter
                                                                       //  (at the time of execution, the PC has already been bumped to the next instruction)

    ASSERT_EQ(the_machine->stack_pointer, 200);                        // JAL should have consumed the address off the top of the stack

    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite, return_set_program_counter_to_return_address_and_restores_return_address){
    lmsm *the_machine = lmsm_create();

    the_machine->program_counter = 50; // RET instruction is located at position 50 in memory
    the_machine->stack_pointer = 199;  // return value of function is on the top of the stack
    the_machine->memory[199] = 5;      //

    the_machine->memory[100] = 2;     // return address pushed onto the return_address stack by JAL
    the_machine->return_address_pointer = 100;  // points to next available stack slot

    lmsm_exec_instruction(the_machine, 911); // RET

    ASSERT_EQ(the_machine->program_counter, 2);                       // program counter should be updated to the return_address value on the stack

    ASSERT_EQ(the_machine->stack_pointer, 199);                        // stack should still have the return value on it
    ASSERT_EQ(the_machine->memory[199], 5);

    ASSERT_EQ(the_machine->return_address_pointer, 99);                // return_address_pointer should be decremented

    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,step_steps_to_the_next_instruction_and_executes_current){

    lmsm *the_machine = lmsm_create();

    the_machine->accumulator = 10;
    the_machine->program_counter = 0;
    the_machine->memory[0] = 902; // OUT
    the_machine->memory[1] = 000; // HLT

    lmsm_step(the_machine); // should execute the 902 asm_instruction and move to the next position

    ASSERT_EQ(the_machine->program_counter, 1); // should have bumped the pc
    ASSERT_EQ(the_machine->current_instruction, 902); // should have loaded the asm_instruction from mem[0]
    ASSERT_STREQ(the_machine->output_buffer, "10 "); // should have executed the asm_instruction (OUT)

    lmsm_delete(the_machine);
}

TEST(lmsm_machine_suite,step_does_not_step_to_the_next_instruction_and_execute_if_machine_is_halted){

    lmsm *the_machine = lmsm_create();

    the_machine->accumulator = 10;
    the_machine->program_counter = 0;
    the_machine->memory[0] = 902; // OUT
    the_machine->memory[1] = 000; // HLT

    the_machine->status = STATUS_HALTED; // halt the machine
    lmsm_step(the_machine); // should execute the 902 asm_instruction and move to the next position

    ASSERT_EQ(the_machine->program_counter, 0); // should not have bumped the pc since halted
    ASSERT_EQ(the_machine->current_instruction, 0); // should not have loaded the asm_instruction from mem[0]
    ASSERT_STREQ(the_machine->output_buffer, ""); // should not have executed the asm_instruction (OUT)

    lmsm_delete(the_machine);
}

