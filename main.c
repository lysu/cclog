#include <unistd.h>
#include "stdio.h"
#include "cclog.h"

int main(void) {
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