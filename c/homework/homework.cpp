#include "gtest/gtest.h"

//==========================================================================================
// PROBLEM 1: printing a number in binary form
//==========================================================================================

/* Implement a print_binary_representation function that takes an
 * unsigned integer and created as string representation of the binary values
 * of that number.
 *
 * The test below shows what the expected values are for given inputs, the string will start with "0b" folowed
 * by the string "1" when the bit for a given position is 1 and "0" when it is 0.  You will want to use
 * shifting and bit-masking to test for the value of a given bit.
 */

char * print_binary_representation(unsigned int i, char *buffer){
    buffer[0] = '0';
    buffer[1] = 'b';
    // fill out remaining 32 bits, 1 or 0 depending on the value in the number i
    return buffer;
}

TEST(print_binary_representation, works) {
    // row 1
    char buffer[50] = {0}; // init to 0
    EXPECT_STREQ("0b00000000000000000000000000000000", print_binary_representation(0, buffer));
    EXPECT_STREQ("0b00000000000000000000000000000001", print_binary_representation(1, buffer));
    EXPECT_STREQ("0b00000000000000000000000000000010", print_binary_representation(2, buffer));
    EXPECT_STREQ("0b00000000000000000000000000000011", print_binary_representation(3, buffer));
    EXPECT_STREQ("0b00000000000000000000000000000100", print_binary_representation(4, buffer));
    EXPECT_STREQ("0b00000001110111111001101001000010", print_binary_representation(31431234, buffer));
    EXPECT_STREQ("0b00011011111000100100001101011101", print_binary_representation(467813213, buffer));
    EXPECT_STREQ("0b11111111111111111111111111111111", print_binary_representation(UINT32_MAX, buffer));
}

//==========================================================================================
// PROBLEM 2: Pass By Reference vs. Pass By Value
//==========================================================================================

/* The test below fails.  Change the signature of set_my_age and the
 * call of the function in get_my_age so that the expected value is returned.
 *
 * HINT: C is pass by value
 */
typedef struct person {
    char * name;
    int age;
} person;

void set_my_age(person p) {
    p.age = 44;
}

int get_my_age() {
    person me;          // Question to ask yourself: where is this memory allocated?  The stack?  The heap?
    me.name = "Joe";    // what does this statement do?  Where is the string "Joe" located?
    set_my_age(me);
    return me.age;
}

TEST(set_my_age, works) {
    EXPECT_EQ(44, get_my_age());
}

//==========================================================================================
// PROBLEM 3: Using A Linked List
//==========================================================================================

/* It is very common in C programming to use linked lists (we will use them a lot in our
 * project) because of the poverty of the standard library.  This means you have to do a
 * lot of stuff by hand that would be better done in Java with a Map or List object.  Oh well.
 *
 * Below write a function that, given a persons name and a linked list of people, returns the
 * age for the person with that name.  If they are not in the list, return -1
 */

typedef struct person_list {
    person * person;
    person_list * next;
} person_list;

// Question: why do we have to declare these here?
person_list * make_person_list();
person_list * make_list_node(char *string, int age);

int find_persons_age(char *name, person_list *list){
    return 0;
}

TEST(find_persons_age, works) {
    person_list * list = make_person_list();
    EXPECT_EQ(44, find_persons_age("Joe", list));
    EXPECT_EQ(23, find_persons_age("Jim", list));
    EXPECT_EQ(35, find_persons_age("Jessica", list));
    EXPECT_EQ(57, find_persons_age("Jack", list));
    EXPECT_EQ(-1, find_persons_age("James", list));

    // Question: why don't I bother to de-allocate the list?
}

// ignore this C++ garbage, have to do it this way to make the compiler happy
person_list * make_person_list() {
    person_list *first = make_list_node("Joe", 44);
    person_list *second = make_list_node("Jim", 23);
    first->next = second;
    person_list *third = make_list_node("Jessica", 35);
    second->next = third;
    person_list *fourth = make_list_node("Jack", 57);
    third->next = fourth;
    fourth->next = NULL; // null terminate
    return first;
}

person_list *make_list_node(char *string, int age) {
    person_list *root = new person_list;
    root->person = new person;
    root->person->age = age;
    root->person->name = string;
    return root;
}
