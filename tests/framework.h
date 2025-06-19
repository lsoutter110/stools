#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <stdio.h>

#define TEST_SUCCESS 0
#define TEST_FAIL 1
#define tassertf(expr, fstr, ...) do{if(!(expr)){printf("\r\n%s:%d: " fstr, __FILE__, __LINE__ __VA_OPT__(,) __VA_ARGS__);return TEST_FAIL;}}while(0)

void test_register(int (*test)());
void test_run_all();

#endif