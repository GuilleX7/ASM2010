/** @file trace_log.c */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include "trace_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool trace_log_init(trace_log *log, size_t trace_max_length, size_t log_max_length) {
    if (!log) {
        return false;
    }
    log->trace = malloc(trace_max_length + 1);
    if (!log->trace) {
        return false;
    }
    log->log = malloc(log_max_length + 1);
    if (!log->log) {
        free(log->trace);
        return false;
    }
    log->max_trace_space = trace_max_length;
    log->max_log_space   = log_max_length;
    trace_log_clear(log);
    return true;
}

void trace_log_free(trace_log *log) {
    if (!log) {
        return;
    }
    if (log->log) {
        free(log->log);
    }
    if (log->trace) {
        free(log->trace);
    }
}

void trace_log_clear(trace_log *log) {
    if (!log || !log->trace || !log->log) {
        return;
    }
    log->trace[0]            = '\0';
    log->log[0]              = '\0';
    log->available_log_space = log->max_log_space;
}

bool trace_log_printf(trace_log *log, char const *const format, ...) {
    va_list va;
    size_t  trace_length;
    if (!log || !log->trace || !log->log) {
        return false;
    }
    va_start(va, format);
    if (vsnprintf(log->trace, log->max_trace_space, format, va) < 0) {
        va_end(va);
        return false;
    }
    trace_length = strlen(log->trace);
    if (trace_length <= log->available_log_space) {
        strncat(log->log, log->trace, log->available_log_space);
        log->available_log_space -= trace_length;
        va_end(va);
        return true;
    }

    va_end(va);
    return false;
}

bool trace_log_is_empty(trace_log *log) {
    return !log || !log->log || !*log->log;
}

char const *trace_log_get(trace_log *log) {
    if (!log) {
        return 0;
    }
    return log->log;
}
