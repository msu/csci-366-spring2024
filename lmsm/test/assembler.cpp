#include "gtest/gtest.h"
extern "C" {
#include "assembler.h"
}

//==========================================================================
// Instruction constructor tests
//==========================================================================

TEST(instruction_construction, make_instruction_basic_instruction_fields_are_set) {
    asm_instruction *instruction = asm_make_instruction("Foo", "Bar", "Doh", 1, NULL);
    ASSERT_EQ(instruction->instruction, "Foo");
    ASSERT_EQ(instruction->label, "Bar");
    ASSERT_EQ(instruction->label_reference, "Doh");
    ASSERT_EQ(instruction->value, 1);
    ASSERT_EQ(instruction->slots, 1);
    ASSERT_EQ(instruction->offset, 0);
    asm_delete_instruction(instruction);
}

TEST(instruction_construction, make_instruction_spushi_takes_two_slots) {
    asm_instruction *instruction = asm_make_instruction("SPUSHI", NULL, NULL, 0, NULL);
    ASSERT_EQ(instruction->instruction, "SPUSHI");
    ASSERT_EQ(instruction->slots, 2);
    asm_delete_instruction(instruction);
}

TEST(instruction_construction, make_instruction_call_takes_three_slots) {
    asm_instruction *instruction = asm_make_instruction("CALL", NULL, NULL, 0, NULL);
    ASSERT_EQ(instruction->instruction, "CALL");
    ASSERT_EQ(instruction->slots, 3);
    asm_delete_instruction(instruction);
}

TEST(instruction_construction, make_instruction_properly_offsets_based_on_preceding_instruction) {
    asm_instruction *next_instruction;
    asm_instruction *instruction;

    instruction = asm_make_instruction("Foo", NULL, NULL, 1, NULL);
    instruction->slots = 3;  // start this asm_instruction at slot three
    instruction->offset = 3; // this asm_instruction takes up three slots

    next_instruction = asm_make_instruction("Foo", NULL, NULL, 0, instruction);

    ASSERT_EQ(next_instruction->offset, 6);

    asm_delete_instruction(instruction);
}

//==========================================================================
// Parsing Tests
//==========================================================================

TEST(parsing_tests, simple_instruction_parsing_works) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_parse_src(result, "OUT");
    ASSERT_EQ(result->error, nullptr);
    ASSERT_FALSE(result->root == nullptr);
    ASSERT_STREQ(result->root->instruction, "OUT");
    ASSERT_EQ(result->root->next, nullptr);
    asm_delete_compilation_result(result);
}

TEST(parsing_tests, two_instruction_parsing_works_w_space) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_parse_src(result, "INP OUT");
    ASSERT_EQ(result->error, nullptr);
    ASSERT_FALSE(result->root == nullptr);
    ASSERT_STREQ(result->root->instruction, "INP");
    ASSERT_STREQ(result->root->next->instruction, "OUT");
    asm_delete_compilation_result(result);
}

TEST(parsing_tests, two_instruction_parsing_works_w_newline) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_parse_src(result, "INP\nOUT");
    ASSERT_EQ(result->error, nullptr);
    ASSERT_FALSE(result->root == nullptr);
    ASSERT_STREQ(result->root->instruction, "INP");
    ASSERT_STREQ(result->root->next->instruction, "OUT");
    asm_delete_compilation_result(result);
}

TEST(parsing_tests, label_is_parsed_correctly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_parse_src(result, "FOO OUT");
    ASSERT_EQ(result->error, nullptr);
    ASSERT_FALSE(result->root == nullptr);
    ASSERT_STREQ(result->root->label, "FOO");
    ASSERT_STREQ(result->root->instruction, "OUT");
    ASSERT_EQ(result->root->next, nullptr);
    asm_delete_compilation_result(result);
}

TEST(parsing_tests, numeric_reference_works_for_branch) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_parse_src(result, "BRA 22");
    ASSERT_EQ(result->error, nullptr);
    ASSERT_FALSE(result->root == nullptr);
    ASSERT_STREQ(result->root->instruction, "BRA");
    ASSERT_EQ(result->root->value, 22);
    ASSERT_EQ(result->root->next, nullptr);
    asm_delete_compilation_result(result);
}

TEST(parsing_tests, label_reference_works_for_branch) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_parse_src(result, "BRA FOO");
    ASSERT_EQ(result->error, nullptr);
    ASSERT_FALSE(result->root == nullptr);
    ASSERT_STREQ(result->root->instruction, "BRA");
    ASSERT_STREQ(result->root->label_reference, "FOO");
    ASSERT_EQ(result->root->next, nullptr);
    asm_delete_compilation_result(result);
}

TEST(parsing_tests, unknown_instruction_is_handled) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_parse_src(result, "FOO FOO");
    ASSERT_EQ(result->error, ASM_ERROR_UNKNOWN_INSTRUCTION);
    asm_delete_compilation_result(result);
}

TEST(parsing_tests, no_argument_for_instruction_causes_error) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_parse_src(result, "BRA");
    ASSERT_EQ(result->error, ASM_ERROR_ARG_REQUIRED);
    asm_delete_compilation_result(result);
}

TEST(parsing_tests, numbers_out_of_machine_range_cause_error) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_parse_src(result, "DAT 1000");
    ASSERT_EQ(result->error, ASM_ERROR_OUT_OF_RANGE);
    asm_delete_compilation_result(result);
}

TEST(parsing_tests, negative_numbers_out_of_machine_range_are_capped_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_parse_src(result, "DAT -1000");
    ASSERT_EQ(result->error, ASM_ERROR_OUT_OF_RANGE);
    asm_delete_compilation_result(result);
}

//==========================================================================
// Code generation tests
//==========================================================================

TEST(code_generation, add_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("ADD", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 101);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, sub_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SUB", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 201);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, store_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("STA", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 301);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, load_immediate_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("LDI", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 401);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, load_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("LDA", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 501);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, branch_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("BRA", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 601);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, branch_if_zero_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("BRZ", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 701);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, branch_if_positive_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("BRP", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 801);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, input_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("INP", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 901);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, out_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("OUT", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 902);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, halt_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("HLT", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 000);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, cob_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("COB", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 000);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, dat_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("DAT", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 001);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, spush_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SPUSH", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 920);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, spop_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SPOP", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 921);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, sdup_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SDUP", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 922);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, sdrop_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SDROP", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 923);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, sswap_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SSWAP", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 924);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, sadd_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SADD", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 930);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, ssub_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SSUB", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 931);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, smax_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SMAX", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 934);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, smin_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SMIN", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 935);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, smul_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SMUL", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 932);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, sdiv_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SDIV", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 933);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, spushi_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("SPUSHI", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    /// SPUSHI is a synthetic asm_instruction that compiles down to a SPUSH and an LDI
    ASSERT_EQ(result->code[0], 401);
    ASSERT_EQ(result->code[1], 920);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, call_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("CALL", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    /// CALL is a synthetic asm_instruction that compiles down to a SPUSH and an LDI of the target address to jump to
    // and finally a CALL machine code (911)
    ASSERT_EQ(result->code[0], 401);
    ASSERT_EQ(result->code[1], 920);
    ASSERT_EQ(result->code[2], 910);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}

TEST(code_generation, ret_instruction_generates_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction = asm_make_instruction("RET", NULL, NULL, 1, NULL);

    asm_gen_code_for_instruction(result, instruction);

    ASSERT_EQ(result->code[0], 911);

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction);
}


TEST(code_generation, instructions_next_to_one_another_generate_in_order_properly) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction1 = asm_make_instruction("SPUSHI", NULL, NULL, 1, NULL);
    asm_instruction * instruction2 = asm_make_instruction("OUT", NULL, NULL, 1, instruction1);
    asm_instruction * instruction3 = asm_make_instruction("HLD", NULL, NULL, 1, instruction2);

    asm_gen_code_for_instruction(result, instruction1);
    asm_gen_code_for_instruction(result, instruction2);
    asm_gen_code_for_instruction(result, instruction3);

    ASSERT_EQ(result->code[0], 401); // SPUSHI
    ASSERT_EQ(result->code[1], 920);
    ASSERT_EQ(result->code[2], 902); // OUT
    ASSERT_EQ(result->code[3], 000); // HLT

    asm_delete_compilation_result(result);
    asm_delete_instruction(instruction1);
}

TEST(code_generation, labels_resolve_at_code_generation_time) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction1 = asm_make_instruction("LDA", NULL, "FOO", 0, NULL);
    asm_instruction * instruction2 = asm_make_instruction("DAT", "FOO", NULL, 1, instruction1);
    result->root = instruction1; // allow for search of instructions for "FOO" label

    asm_gen_code_for_instruction(result, instruction1);
    asm_gen_code_for_instruction(result, instruction2);

    ASSERT_EQ(result->code[0], 501); // LDA should load data at address 01 (The DAT w/ the lable "FOO")
    ASSERT_EQ(result->code[1], 001); // DAT

    asm_delete_compilation_result(result);
}

TEST(code_generation, bad_label_causes_error) {
    asm_compilation_result *result = asm_make_compilation_result();
    asm_instruction * instruction1 = asm_make_instruction("LDA", NULL, "BAR", 0, NULL);
    asm_instruction * instruction2 = asm_make_instruction("DAT", "FOO", NULL, 1, instruction1);
    result->root = instruction1; // allow for search of instructions for "BAR" label

    asm_gen_code_for_instruction(result, instruction1);
    asm_gen_code_for_instruction(result, instruction2);

    ASSERT_EQ(result->error, ASM_ERROR_BAD_LABEL);

    asm_delete_compilation_result(result);
}

//==========================================================================
// Complete assembly tests
//==========================================================================

TEST(end_to_end, bootstrap){
    /* get the first value */
    asm_compilation_result *result = asm_assemble("INP OUT");
    EXPECT_EQ( result->code[0], 901);
    EXPECT_EQ(result->code[1], 902);
    EXPECT_EQ(result->code[2], 0);
    asm_delete_compilation_result(result);
}