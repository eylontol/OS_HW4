
#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/*
 arg[1] = filepath
 arg[2] = 0 or 1 (expected)
 arg[3] = # test
 */

#define EXPGOOD     "1"
#define EXPBAD      "0"

#define STR_EQUAL(s1, s2)   (!strcmp((s1),(s2)))

int main(int argc, char *argv[]) {
    
    FILE *fout = fopen("out.txt", "a");
    if (!fout) return 0;
    
    int res = unlink(argv[1]);
    
    int testRes = 0;
    if ((STR_EQUAL(argv[2],EXPGOOD)) && (res == 0)) testRes = 1;
    else if ((STR_EQUAL(argv[2], EXPBAD)) && (res != 0) && (errno == 13)) testRes = 1;
    
    fprintf(fout, "<TEST %s> %s\n", argv[3], testRes ? "SUCCESS" : "FAILED");
    return 0;
}
