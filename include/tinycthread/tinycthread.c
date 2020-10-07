/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; -*-
Copyright (c) 2012 Marcus Geelnard
Copyright (c) 2013-2016 Evan Nemerson

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#include "tinycthread.h"
#include <stdlib.h>

/* Platform specific includes */
#if defined(_TTHREAD_POSIX_)
  #include <signal.h>
  #include <sched.h>
  #include <unistd.h>
  #include <sys/time.h>
  #include <errno.h>
#elif defined(_TTHREAD_WIN32_)
  #include <process.h>
  #include <sys/timeb.h>
#endif

/* Standard, good-to-have defines */
#ifndef NULL
  #define NULL (void*)0
#endif
#ifndef TRUE
  #define TRUE 1
#endif
#ifndef FALSE
  #define FALSE 0
#endif

#ifdef __cplusplus
extern "C" {
#endif


int mtx_init(mtx_t *mtx, int type)
{
#if defined(_TTHREAD_WIN32_)
  mtx->mAlreadyLocked = FALSE;
  mtx->mRecursive = type & mtx_recursive;
  mtx->mTimed = type & mtx_timed;
  if (!mtx->mTimed)
  {
    InitializeCriticalSection(&(mtx->mHandle.cs));
  }
  else
  {
    mtx->mHandle.mut = CreateMutex(0, FALSE, 0);
    if (mtx->mHandle.mut == 0)
    {
      return thrd_error;
    }
  }
  return thrd_success;
#else
  int ret;
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  if (type & mtx_recursive)
  {
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  }
  ret = pthread_mutex_init(mtx, &attr);
  pthread_mutexattr_destroy(&attr);
  return ret == 0 ? thrd_success : thrd_error;
#endif
}

void mtx_destroy(mtx_t *mtx)
{
#if defined(_TTHREAD_WIN32_)
  if (!mtx->mTimed)
  {
    DeleteCriticalSection(&(mtx->mHandle.cs));
  }
  else
  {
    CloseHandle(mtx->mHandle.mut);
  }
#else
  pthread_mutex_destroy(mtx);
#endif
}

int mtx_lock(mtx_t *mtx)
{
#if defined(_TTHREAD_WIN32_)
  if (!mtx->mTimed)
  {
    EnterCriticalSection(&(mtx->mHandle.cs));
  }
  else
  {
    switch (WaitForSingleObject(mtx->mHandle.mut, INFINITE))
    {
      case WAIT_OBJECT_0:
        break;
      case WAIT_ABANDONED:
      default:
        return thrd_error;
    }
  }

  if (!mtx->mRecursive)
  {
    while(mtx->mAlreadyLocked) Sleep(1); /* Simulate deadlock... */
    mtx->mAlreadyLocked = TRUE;
  }
  return thrd_success;
#else
  return pthread_mutex_lock(mtx) == 0 ? thrd_success : thrd_error;
#endif
}

int mtx_timedlock(mtx_t *mtx, const struct timespec *ts)
{
  /* No need for timedlocks :) */
  return mtx_lock(mtx);
}

int mtx_trylock(mtx_t *mtx)
{
#if defined(_TTHREAD_WIN32_)
  int ret;

  if (!mtx->mTimed)
  {
    ret = TryEnterCriticalSection(&(mtx->mHandle.cs)) ? thrd_success : thrd_busy;
  }
  else
  {
    ret = (WaitForSingleObject(mtx->mHandle.mut, 0) == WAIT_OBJECT_0) ? thrd_success : thrd_busy;
  }

  if ((!mtx->mRecursive) && (ret == thrd_success))
  {
    if (mtx->mAlreadyLocked)
    {
      if (!mtx->mTimed)
      {
        LeaveCriticalSection(&(mtx->mHandle.cs));
      }
      ret = thrd_busy;
    }
    else
    {
      mtx->mAlreadyLocked = TRUE;
    }
  }
  return ret;
#else
  return (pthread_mutex_trylock(mtx) == 0) ? thrd_success : thrd_busy;
#endif
}

int mtx_unlock(mtx_t *mtx)
{
#if defined(_TTHREAD_WIN32_)
  mtx->mAlreadyLocked = FALSE;
  if (!mtx->mTimed)
  {
    LeaveCriticalSection(&(mtx->mHandle.cs));
  }
  else
  {
    if (!ReleaseMutex(mtx->mHandle.mut))
    {
      return thrd_error;
    }
  }
  return thrd_success;
#else
  return pthread_mutex_unlock(mtx) == 0 ? thrd_success : thrd_error;;
#endif
}

#if defined(_TTHREAD_WIN32_)
#define _CONDITION_EVENT_ONE 0
#define _CONDITION_EVENT_ALL 1
#endif

int cnd_init(cnd_t *cond)
{
#if defined(_TTHREAD_WIN32_)
  cond->mWaitersCount = 0;

  /* Init critical section */
  InitializeCriticalSection(&cond->mWaitersCountLock);

  /* Init events */
  cond->mEvents[_CONDITION_EVENT_ONE] = CreateEvent(0, FALSE, FALSE, 0);
  if (cond->mEvents[_CONDITION_EVENT_ONE] == 0)
  {
    cond->mEvents[_CONDITION_EVENT_ALL] = 0;
    return thrd_error;
  }
  cond->mEvents[_CONDITION_EVENT_ALL] = CreateEvent(0, TRUE, FALSE, 0);
  if (cond->mEvents[_CONDITION_EVENT_ALL] == 0)
  {
    CloseHandle(cond->mEvents[_CONDITION_EVENT_ONE]);
    cond->mEvents[_CONDITION_EVENT_ONE] = 0;
    return thrd_error;
  }

  return thrd_success;
#else
  return pthread_cond_init(cond, 0) == 0 ? thrd_success : thrd_error;
#endif
}

void cnd_destroy(cnd_t *cond)
{
#if defined(_TTHREAD_WIN32_)
  if (cond->mEvents[_CONDITION_EVENT_ONE] != 0)
  {
    CloseHandle(cond->mEvents[_CONDITION_EVENT_ONE]);
  }
  if (cond->mEvents[_CONDITION_EVENT_ALL] != 0)
  {
    CloseHandle(cond->mEvents[_CONDITION_EVENT_ALL]);
  }
  DeleteCriticalSection(&cond->mWaitersCountLock);
#else
  pthread_cond_destroy(cond);
#endif
}

int cnd_signal(cnd_t *cond)
{
#if defined(_TTHREAD_WIN32_)
  int haveWaiters;

  /* Are there any waiters? */
  EnterCriticalSection(&cond->mWaitersCountLock);
  haveWaiters = (cond->mWaitersCount > 0);
  LeaveCriticalSection(&cond->mWaitersCountLock);

  /* If we have any waiting threads, send them a signal */
  if(haveWaiters)
  {
    if (SetEvent(cond->mEvents[_CONDITION_EVENT_ONE]) == 0)
    {
      return thrd_error;
    }
  }

  return thrd_success;
#else
  return pthread_cond_signal(cond) == 0 ? thrd_success : thrd_error;
#endif
}

int cnd_broadcast(cnd_t *cond)
{
#if defined(_TTHREAD_WIN32_)
  int haveWaiters;

  /* Are there any waiters? */
  EnterCriticalSection(&cond->mWaitersCountLock);
  haveWaiters = (cond->mWaitersCount > 0);
  LeaveCriticalSection(&cond->mWaitersCountLock);

  /* If we have any waiting threads, send them a signal */
  if(haveWaiters)
  {
    if (SetEvent(cond->mEvents[_CONDITION_EVENT_ALL]) == 0)
    {
      return thrd_error;
    }
  }

  return thrd_success;
#else
  return pthread_cond_broadcast(cond) == 0 ? thrd_success : thrd_error;
#endif
}


#if defined(_TTHREAD_WIN32_)
static int _cnd_timedwait_win32(cnd_t *cond, mtx_t *mtx, DWORD timeout) {
    DWORD result;
    int lastWaiter;

    /* Increment number of waiters */
    EnterCriticalSection(&cond->mWaitersCountLock);
    ++cond->mWaitersCount;
    LeaveCriticalSection(&cond->mWaitersCountLock);

    /* Release the mutex while waiting for the condition (will decrease
       the number of waiters when done)... */
    mtx_unlock(mtx);

    /* Wait for either event to become signaled due to cnd_signal() or
       cnd_broadcast() being called */
    result = WaitForMultipleObjects(2, cond->mEvents, FALSE, timeout);
    if (result == WAIT_TIMEOUT) {
        /* The mutex is locked again before the function returns, even if an error occurred */
        mtx_lock(mtx);
        return thrd_timedout;
    } else if (result == WAIT_FAILED) {
        /* The mutex is locked again before the function returns, even if an error occurred */
        mtx_lock(mtx);
        return thrd_error;
    }

    /* Check if we are the last waiter */
    EnterCriticalSection(&cond->mWaitersCountLock);
    --cond->mWaitersCount;
    lastWaiter = (result == (WAIT_OBJECT_0 + _CONDITION_EVENT_ALL)) &&
        (cond->mWaitersCount == 0);
    LeaveCriticalSection(&cond->mWaitersCountLock);

    /* If we are the last waiter to be notified to stop waiting, reset the event */
    if (lastWaiter) {
        if (ResetEvent(cond->mEvents[_CONDITION_EVENT_ALL]) == 0) {
            /* The mutex is locked again before the function returns, even if an error occurred */
            mtx_lock(mtx);
            return thrd_error;
        }
    }

    /* Re-acquire the mutex */
    mtx_lock(mtx);

    return thrd_success;
}
#endif

int cnd_wait(cnd_t *cond, mtx_t *mtx)
{
#if defined(_TTHREAD_WIN32_)
  return _cnd_timedwait_win32(cond, mtx, INFINITE);
#else
  return pthread_cond_wait(cond, mtx) == 0 ? thrd_success : thrd_error;
#endif
}

int cnd_timedwait(cnd_t *cond, mtx_t *mtx, const struct timespec *ts)
{
  /* No need for timedwaits :) */
    return cnd_wait(cond, mtx);
}

#if defined(_TTHREAD_WIN32_)
struct TinyCThreadTSSData {
  void* value;
  tss_t key;
  struct TinyCThreadTSSData* next;
};

static tss_dtor_t _tinycthread_tss_dtors[1088] = { 0, };

static _Thread_local struct TinyCThreadTSSData* _tinycthread_tss_head = 0;
static _Thread_local struct TinyCThreadTSSData* _tinycthread_tss_tail = 0;

static void _tinycthread_tss_cleanup (void);

static void _tinycthread_tss_cleanup (void) {
  struct TinyCThreadTSSData* data;
  int iteration;
  unsigned int again = 1;
  void* value;

  for (iteration = 0 ; iteration < TSS_DTOR_ITERATIONS && again > 0 ; iteration++)
  {
    again = 0;
    for (data = _tinycthread_tss_head ; data != 0 ; data = data->next)
    {
      if (data->value != 0)
      {
        value = data->value;
        data->value = 0;

        if (_tinycthread_tss_dtors[data->key] != 0)
        {
          again = 1;
          _tinycthread_tss_dtors[data->key](value);
        }
      }
    }
  }

  while (_tinycthread_tss_head != 0) {
    data = _tinycthread_tss_head->next;
    free (_tinycthread_tss_head);
    _tinycthread_tss_head = data;
  }
  _tinycthread_tss_head = 0;
  _tinycthread_tss_tail = 0;
}

static void NTAPI _tinycthread_tss_callback(PVOID h, DWORD dwReason, PVOID pv)
{
  (void)h;
  (void)pv;

  if (_tinycthread_tss_head != 0 && (dwReason == DLL_THREAD_DETACH || dwReason == DLL_PROCESS_DETACH))
  {
    _tinycthread_tss_cleanup();
  }
}

#if defined(_MSC_VER)
  #ifdef _M_X64
    #pragma const_seg(".CRT$XLB")
  #else
    #pragma data_seg(".CRT$XLB")
  #endif
  PIMAGE_TLS_CALLBACK p_thread_callback = _tinycthread_tss_callback;
  #ifdef _M_X64
    #pragma data_seg()
  #else
    #pragma const_seg()
  #endif
#else
  PIMAGE_TLS_CALLBACK p_thread_callback __attribute__((section(".CRT$XLB"))) = _tinycthread_tss_callback;
#endif

#endif /* defined(_TTHREAD_WIN32_) */

/** Information to pass to the new thread (what to run). */
typedef struct {
  thrd_start_t mFunction; /**< Pointer to the function to be executed. */
  void * mArg;            /**< Function argument for the thread function. */
} _thread_start_info;

/* Thread wrapper function. */
#if defined(_TTHREAD_WIN32_)
static DWORD WINAPI _thrd_wrapper_function(LPVOID aArg)
#elif defined(_TTHREAD_POSIX_)
static void * _thrd_wrapper_function(void * aArg)
#endif
{
  thrd_start_t fun;
  void *arg;
  int  res;

  /* Get thread startup information */
  _thread_start_info *ti = (_thread_start_info *) aArg;
  fun = ti->mFunction;
  arg = ti->mArg;

  /* The thread is responsible for freeing the startup information */
  free((void *)ti);

  /* Call the actual client thread function */
  res = fun(arg);

#if defined(_TTHREAD_WIN32_)
  if (_tinycthread_tss_head != 0)
  {
    _tinycthread_tss_cleanup();
  }

  return (DWORD)res;
#else
  return (void*)(intptr_t)res;
#endif
}

int thrd_create(thrd_t *thr, thrd_start_t func, void *arg)
{
  /* Fill out the thread startup information (passed to the thread wrapper,
     which will eventually free it) */
  _thread_start_info* ti = (_thread_start_info*)malloc(sizeof(_thread_start_info));
  if (ti == 0)
  {
    return thrd_nomem;
  }
  ti->mFunction = func;
  ti->mArg = arg;

  /* Create the thread */
#if defined(_TTHREAD_WIN32_)
  *thr = CreateThread(0, 0, _thrd_wrapper_function, (LPVOID) ti, 0, 0);
#elif defined(_TTHREAD_POSIX_)
  if(pthread_create(thr, 0, _thrd_wrapper_function, (void *)ti) != 0)
  {
    *thr = 0;
  }
#endif

  /* Did we fail to create the thread? */
  if(!*thr)
  {
    free(ti);
    return thrd_error;
  }

  return thrd_success;
}

thrd_t thrd_current(void)
{
#if defined(_TTHREAD_WIN32_)
  return GetCurrentThread();
#else
  return pthread_self();
#endif
}

int thrd_detach(thrd_t thr)
{
#if defined(_TTHREAD_WIN32_)
  /* https://stackoverflow.com/questions/12744324/how-to-detach-a-thread-on-windows-c#answer-12746081 */
  return CloseHandle(thr) != 0 ? thrd_success : thrd_error;
#else
  return pthread_detach(thr) == 0 ? thrd_success : thrd_error;
#endif
}

int thrd_equal(thrd_t thr0, thrd_t thr1)
{
  /* No thrd equal :) */
  return 1;
}

void thrd_exit(int res)
{
#if defined(_TTHREAD_WIN32_)
  if (_tinycthread_tss_head != 0)
  {
    _tinycthread_tss_cleanup();
  }

  ExitThread((DWORD)res);
#else
  pthread_exit((void*)(intptr_t)res);
#endif
}

int thrd_join(thrd_t thr, int *res)
{
#if defined(_TTHREAD_WIN32_)
  DWORD dwRes;

  if (WaitForSingleObject(thr, INFINITE) == WAIT_FAILED)
  {
    return thrd_error;
  }
  if (res != 0)
  {
    if (GetExitCodeThread(thr, &dwRes) != 0)
    {
      *res = (int) dwRes;
    }
    else
    {
      return thrd_error;
    }
  }
  CloseHandle(thr);
#elif defined(_TTHREAD_POSIX_)
  void *pres;
  if (pthread_join(thr, &pres) != 0)
  {
    return thrd_error;
  }
  if (res != 0)
  {
    *res = (int)(intptr_t)pres;
  }
#endif
  return thrd_success;
}

int thrd_sleep(const struct timespec *duration, struct timespec *remaining)
{
  /* No thread sleep :) */
  return 0;
}

void thrd_yield(void)
{
#if defined(_TTHREAD_WIN32_)
  Sleep(0);
#else
  sched_yield();
#endif
}

int tss_create(tss_t *key, tss_dtor_t dtor)
{
#if defined(_TTHREAD_WIN32_)
  *key = TlsAlloc();
  if (*key == TLS_OUT_OF_INDEXES)
  {
    return thrd_error;
  }
  _tinycthread_tss_dtors[*key] = dtor;
#else
  if (pthread_key_create(key, dtor) != 0)
  {
    return thrd_error;
  }
#endif
  return thrd_success;
}

void tss_delete(tss_t key)
{
#if defined(_TTHREAD_WIN32_)
  struct TinyCThreadTSSData* data = (struct TinyCThreadTSSData*) TlsGetValue (key);
  struct TinyCThreadTSSData* prev = 0;
  if (data != 0)
  {
    if (data == _tinycthread_tss_head)
    {
      _tinycthread_tss_head = data->next;
    }
    else
    {
      prev = _tinycthread_tss_head;
      if (prev != 0)
      {
        while (prev->next != data)
        {
          prev = prev->next;
        }
      }
    }

    if (data == _tinycthread_tss_tail)
    {
      _tinycthread_tss_tail = prev;
    }

    free (data);
  }
  _tinycthread_tss_dtors[key] = 0;
  TlsFree(key);
#else
  pthread_key_delete(key);
#endif
}

void *tss_get(tss_t key)
{
#if defined(_TTHREAD_WIN32_)
  struct TinyCThreadTSSData* data = (struct TinyCThreadTSSData*)TlsGetValue(key);
  if (data == 0)
  {
    return 0;
  }
  return data->value;
#else
  return pthread_getspecific(key);
#endif
}

int tss_set(tss_t key, void *val)
{
#if defined(_TTHREAD_WIN32_)
  struct TinyCThreadTSSData* data = (struct TinyCThreadTSSData*)TlsGetValue(key);
  if (data == 0)
  {
    data = (struct TinyCThreadTSSData*)malloc(sizeof(struct TinyCThreadTSSData));
    if (data == 0)
    {
      return thrd_error;
	}

    data->value = 0;
    data->key = key;
    data->next = 0;

    if (_tinycthread_tss_tail != 0)
    {
      _tinycthread_tss_tail->next = data;
    }
    else
    {
      _tinycthread_tss_tail = data;
    }

    if (_tinycthread_tss_head == 0)
    {
      _tinycthread_tss_head = data;
    }

    if (!TlsSetValue(key, data))
    {
      free (data);
	  return thrd_error;
    }
  }
  data->value = val;
#else
  if (pthread_setspecific(key, val) != 0)
  {
    return thrd_error;
  }
#endif
  return thrd_success;
}

#if defined(_TTHREAD_WIN32_)
void call_once(once_flag *flag, void (*func)(void))
{
  /* The idea here is that we use a spin lock (via the
     InterlockedCompareExchange function) to restrict access to the
     critical section until we have initialized it, then we use the
     critical section to block until the callback has completed
     execution. */
  while (flag->status < 3)
  {
    switch (flag->status)
    {
      case 0:
        if (InterlockedCompareExchange (&(flag->status), 1, 0) == 0) {
          InitializeCriticalSection(&(flag->lock));
          EnterCriticalSection(&(flag->lock));
          flag->status = 2;
          func();
          flag->status = 3;
          LeaveCriticalSection(&(flag->lock));
          return;
        }
        break;
      case 1:
        break;
      case 2:
        EnterCriticalSection(&(flag->lock));
        LeaveCriticalSection(&(flag->lock));
        break;
    }
  }
}
#endif /* defined(_TTHREAD_WIN32_) */

#ifdef __cplusplus
}
#endif
