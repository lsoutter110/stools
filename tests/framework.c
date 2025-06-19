#include "framework.h"

#include <stdlib.h>
#include <stdio.h>

struct {
    int (**ptrs)();
    size_t len;
    size_t alloc;
} tests;

void test_register(int (*test)()) {
    if(tests.len == tests.alloc) {
        if(tests.alloc > 0) {
            tests.alloc *= 2;
        } else {
            tests.alloc = 8;
        }
        tests.ptrs = realloc(tests.ptrs, sizeof(*tests.ptrs)*tests.alloc);
    }
    tests.ptrs[tests.len] = test;
    tests.len++;
}

void test_run_all() {
    for(size_t t=0; t<tests.len; t++) {
        printf("Running test %d", t);
        int failed = tests.ptrs[t]();
        if(failed) {
            printf("\r\n");
        } else {
            printf(" - Success\r\n");
        }
    }
}