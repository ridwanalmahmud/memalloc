#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "bump.h"

void setup(void) {
    head = NULL;
    tail = NULL;
}

void teardown(void) {

}

Test(allocator, align_function) {
    cr_assert_eq(align(1), sizeof(word_t));
    cr_assert_eq(align(sizeof(word_t)), sizeof(word_t));
    cr_assert_eq(align(sizeof(word_t)+ 1), sizeof(word_t));
    cr_assert_eq(align(0), 0);
}
