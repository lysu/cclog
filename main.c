#include <unistd.h>
#include "stdio.h"
#include "cclog.h"
#include "cclogfile.h"

int main(void) {

    cc_logfile_t *lf = cc_logfile_create("/code/c/cclog/build/ha", 10000, 1000);
    char *content = "aaaaaaaaaaaaaaaaaaaacccc";
    cc_logfile_append(lf, content, strlen(content));
    cc_logfile_flush(lf);

    printf("put log done...\n");

    cclogger_t *logger = logger_create("test", 2, 10000);
    printf("create...\n");
    logger_start(logger);
    printf("starting....\n");
    char *x = "1211111111111111111111111111111111111111111";
    logger_write(logger, x, strlen(x));
    printf("writed\n");
    sleep(5);
    printf("all donn\n");
}