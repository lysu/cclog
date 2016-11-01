#include <unistd.h>
#include "cclog.h"
#include "stdio.h"

int main(void) {
    cclogger_t *logger = logger_create("/code/c/cclog/build/abc", 5, 1);
    printf("create...\n");
    logger_start(logger);
    printf("starting....\n");
    for (int i = 0; i < 1000000; i++) {
        char *x =
            "121212122222222222222222222222222222222231231231241232131312312312"
            "3123124121221312312313123123123";
        sleep(3);
        logger_write(logger, x, strlen(x));
    }
    printf("writed\n");
    sleep(5);
    printf("all donn\n");
}
