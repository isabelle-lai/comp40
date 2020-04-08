#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <bitpack.h>
#include <assert.h>

void test_fitsu();
void test_fitss();
void test_getu();
void test_gets();
void test_newu();
void test_news();


int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv; 

    // test_fitsu();
    test_fitss();
    // test_getu();
    // test_gets();
    // test_newu();
    // test_news();
    
    return 0;
}

void test_fitsu()
{
    unsigned width = 63;
    uint64_t num = (((uint64_t)1 << width) - 1);
    bool result = Bitpack_fitsu(num, width);
    if (result){
        fprintf(stderr, "Bitpack_fitsu: (%ld, %u) is true\n", num, width);
    } else {
        fprintf(stderr, "Bitpack_fitsu: (%ld, %u) is false\n", num, width);
    }

    // uint64_t num = 16;
    // unsigned width = 1;
    // bool result;
    // for (int i = 0; i < 65; i++){
    //     width = i;
    //     for (uint64_t j = 0; j < (((uint64_t)1 << i) - 1); j++) {
    //         num = j;
    //         result = Bitpack_fitsu(num, width);
    //         if (result){
    //             fprintf(stderr, "Bitpack_fitsu: (%ld, %u) is true\n", num, width);
    //         } else {
    //             fprintf(stderr, "Bitpack_fitsu: (%ld, %u) is false\n", num, width);
    //         }
    //     }
    // }
    
}

void test_fitss()
{
    // int width = 63;
    // int64_t num = (-1 * ((int64_t)1 << width) / 2);
    int width = 3;
    int64_t num = 4;
    bool result = Bitpack_fitss(num, width);
    if (result){
        fprintf(stderr, "Bitpack_fitss: (%ld, %u) is true\n", num, width);
    } else {
        fprintf(stderr, "Bitpack_fitss: (%ld, %u) is false\n", num, width);
    }

    // int64_t num = 16;
    // unsigned width = 1;
    // bool result;
    // for (unsigned i = 0; i < 10; i++){
    //     width = i;
    //     for (int64_t j = (-1 * ((int64_t)1 << i) / 2); j <= (((int64_t)1 << i) / 2) - 1; j++) {
    //         num = j;
    //         result = Bitpack_fitss(num, width);
    //         if (result){
    //             fprintf(stderr, "Bitpack_fitss: (%ld, %u) is true\n", num, width);
    //         } else {
    //             fprintf(stderr, "Bitpack_fitss: (%ld, %u) is false\n", num, width);
    //         }
    //     }
    // }
}

void test_getu()
{
    uint64_t word = 0x3f4;
    unsigned width = 6;
    unsigned lsb = 2;

    uint64_t result = Bitpack_getu(word, width, lsb);
    fprintf(stderr, "Bitpack_getu: (%ld, %u, %u) result: %ld\n", word, width, lsb, result);
}

void test_gets()
{
    uint64_t word = 0x3f4;
    unsigned width = 6;
    unsigned lsb = 2;

    int64_t result = Bitpack_gets(word, width, lsb);
    fprintf(stderr, "Bitpack_gets: (%ld, %u, %u) result: %ld\n", word, width, lsb, result);
}

void test_newu()
{
    uint64_t word = ~0;
    unsigned width = 63;
    unsigned lsb = 0;
    uint64_t val = 0;

    uint64_t result = Bitpack_newu(word, width, lsb, val);
    fprintf(stderr, "Bitpack_newu: (%lu, %u, %u, %lu) result: %lu\n", word, width, lsb, val, result);
    if(Bitpack_getu(result, width, lsb) == val) {
        fprintf(stderr, "Successfully updated the value!\n");
    }
    else {
        fprintf(stderr, "Failed to update the value :(\n");
    }
    
}

void test_news()
{
    uint64_t word = ~0;
    unsigned width = 63;
    unsigned lsb = 1;
    int64_t val = 0;

    uint64_t result = Bitpack_news(word, width, lsb, val);
    fprintf(stderr, "Bitpack_news: (%ld, %u, %u, %ld) result: %ld\n", word, width, lsb, val, result);
    if(Bitpack_gets(result, width, lsb) == val) {
        fprintf(stderr, "Successfully updated the value!\n");
    }
    else {
        fprintf(stderr, "Failed to update the value :(\n");
    }
}

