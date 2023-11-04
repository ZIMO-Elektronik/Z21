#pragma once

#include <assert.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)

#define PREFIX_LOG(STD, ...)                                                   \
  do {                                                                         \
    assert(pthread_mutex_lock(&log_mutex) == 0);                               \
    fprintf(STD, __FILE__ ":" STRINGIZE(__LINE__) " ");                        \
    fprintf(STD, __VA_ARGS__);                                                 \
    fprintf(STD, "\n");                                                        \
    assert(pthread_mutex_unlock(&log_mutex) == 0);                             \
  } while (0)

#define LOGI(...) PREFIX_LOG(stdout, __VA_ARGS__)
#define LOGD(...) PREFIX_LOG(stdout, __VA_ARGS__)
#define LOGE(...) PREFIX_LOG(stderr, __VA_ARGS__)

extern pthread_mutex_t log_mutex;