#include <sys/time.h>
#include <time.h>
#include <memory.h>
#include "cclogfile.h"

#define PERIOD_SIZE (60*60*24)

char *cc_get_log_file_name(char *basename, time_t *now) {
    char *filename = malloc(strlen(basename) + 65);
    memset(filename, 0, strlen(basename) + 65);

    char *curr = filename;
    memcpy(curr, basename, strlen(basename));
    curr = curr + strlen(basename);

    char timebuf[32];
    struct tm tm;
    *now = time(NULL);
    gmtime_r(now, &tm);
    strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M.", &tm);
    memcpy(curr, timebuf, strlen(timebuf));
    curr = curr + strlen(timebuf);

    char *hostname =  "12121";
    memcpy(curr, hostname, strlen(hostname));
    curr = curr + strlen(hostname);

    char *pid = "2333";
    char pidbuf[32];
    snprintf(pidbuf, sizeof pidbuf, ".%s", pid);
    memcpy(curr, pidbuf, strlen(pidbuf));
    curr = curr + strlen(pidbuf);

    char *suffix = ".log";
    memcpy(curr, suffix, strlen(suffix));

    return filename;
}

cc_logfile_t *cc_logfile_create(char *basename, size_t roll_size, int flush_interval, int check_every_n) {
    cc_logfile_t *l = calloc(1, sizeof(cc_logfile_t));
    l->basename = basename;
    l->flush_interval = flush_interval;
    l->roll_size = roll_size;
    l->count = 0;
    l->start_of_period = 0;
    l->last_roll = 0;
    l->last_flush = 0;
    l->check_every_n = check_every_n;
    l->appender = calloc(1, sizeof(cc_file_appender_t));
    cc_logfile_roll_file(l);
    return l;
}

void cc_logfile_append(cc_logfile_t *logfile, char *logline, size_t len) {
    cc_file_append(logfile->appender, logline, len);
    if (cc_file_appender_written_bytes(logfile->appender) > logfile->roll_size) {
        cc_logfile_roll_file(logfile);
    } else {
        logfile->count++;
        if (logfile->count > logfile->check_every_n) {
            logfile->count = 0;
            time_t now = time(NULL);
            time_t this_period = now / PERIOD_SIZE * PERIOD_SIZE;
            if (this_period != logfile->start_of_period) {
                cc_logfile_roll_file(logfile);
            } else if (now - logfile->last_flush) {
                logfile->last_flush = now;
                cc_file_appender_flush(logfile->appender);
            }
        }
    }
}

void cc_logfile_flush(cc_logfile_t *logfile) {
    cc_file_appender_flush(logfile->appender);
}

int cc_logfile_roll_file(cc_logfile_t *logfile) {
    time_t now = 0;
    char *filename = cc_get_log_file_name(logfile->basename, &now);
    long start = now / PERIOD_SIZE * PERIOD_SIZE;

    if (now > logfile->last_roll)
    {
        logfile->last_roll = now;
        logfile->last_flush = now;
        logfile->start_of_period = start;
        cc_file_appender_t *o_appender = logfile->appender;
        logfile->appender = cc_file_create(filename);
        if (o_appender != NULL) {
            if (o_appender->fp != NULL) {
                fclose(o_appender->fp);
            }
            free(o_appender);
        }
        return 1;
    }
    return 0;
}
