
#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

int main() {
    unlink("Test");
    printf("%s\n", errno == 13 ? "Good :)" : "Bad :(");
    return 0;
}
