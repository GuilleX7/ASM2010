/** @file trace_log.h */

#ifndef TRACE_LOG_H
#define TRACE_LOG_H

#include <stdarg.h>
#include <stddef.h>

#include "utils.h"

struct trace_log {
    /** @brief Trace buffer */
    char *trace;
    /** @brief Log */
    char *log;
    /** @brief Maximum trace space */
    size_t max_trace_space;
    /** @brief Remaining log available space */
    size_t available_log_space;
    /** @brief Maximum log space */
    size_t max_log_space;
};
typedef struct trace_log trace_log;

/**
 * @brief Initiates a new trace log
 * @param log Pointer to the trace log to be initiated
 * @param trace_max_length Maximum length of a single trace
 * @param log_max_length Maximum length of the log
 * @return true if success, false otherwise
 */
bool trace_log_init(trace_log *log, size_t trace_max_length, size_t log_max_length);

/**
 * @brief Frees the memory associated with a trace log
 * @param log Pointer to the trace log to be freed
 */
void trace_log_free(trace_log *log);

/**
 * @brief Clears the content of a trace log
 * @param log Pointer to the trace log to be cleared
 */
void trace_log_clear(trace_log *log);

/**
 * @brief Prints the given formatted string to the log
 * @param log Pointer to the trace log
 * @param format Format of the string
 * @param ... Parameters to be passed to printf function
 * @return true if there's space enough for the trace,
 *			false otherwise
 */
bool trace_log_printf(trace_log *log, char const *format, ...);

/**
 * @brief Checks whether the log is empty or not
 * @param log Pointer to the trace log
 * @return true if log is empty, false otherwise
 */
bool trace_log_is_empty(trace_log *log);

/**
 * @brief Gets the current log
 * @param log Pointer to the trace log
 * @return Pointer to the log if success, null pointer otherwise
 */
char const *trace_log_get(trace_log *log);

#endif /* TRACE_LOG_H */
