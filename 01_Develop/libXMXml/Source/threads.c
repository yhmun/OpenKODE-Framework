/**
 * threads.c: set of generic threading related routines
 *
 * See Copyright for the status of this software.
 *
 * Gary Pennington <Gary.Pennington@uk.sun.com>
 * daniel@veillard.com
 */

#define IN_LIBXML
#include "libxml.h"

#include <XMXML/threads.h>
#include <XMXML/globals.h>

/*
 * xmlMutex are a simple mutual exception locks
 */
struct _xmlMutex 
{
	KDThreadMutex*   lock;
};

/*
 * xmlRMutex are reentrant mutual exception locks
 */
struct _xmlRMutex 
{
    KDThreadMutex*   lock;
    KDuint           held;
    KDuint           waiters;
    KDThread*        tid;
    KDThreadCond*    cv;
};

/*
 * This module still has some internal static data.
 *   - xmlLibraryLock a global lock
 *   - globalkey used for per-thread data
 */

static KDThread*           mainthread;
static KDThreadOnce        once_control = KD_THREAD_ONCE_INIT;
static KDThreadMutex*      global_init_lock;
#ifdef HAVE_PTHREAD_H
static pthread_key_t globalkey;
#endif

static xmlRMutexPtr xmlLibraryLock = KD_NULL;

#ifdef LIBXML_THREAD_ENABLED
static void xmlOnceInit(void);
#endif

/**
 * xmlNewMutex:
 *
 * xmlNewMutex() is used to allocate a libxml2 token struct for use in
 * synchronizing access to data.
 *
 * Returns a new simple mutex pointer or KD_NULL in case of error
 */
xmlMutexPtr
xmlNewMutex(void)
{
    xmlMutexPtr tok;

    if ( ( tok = (xmlMutexPtr) kdMalloc ( sizeof ( xmlMutex ) ) ) == KD_NULL )
	{
        return ( KD_NULL );
	}

    tok->lock = kdThreadMutexCreate ( KD_NULL );
	
    return ( tok );
}

/**
 * xmlFreeMutex:
 * @tok:  the simple mutex
 *
 * xmlFreeMutex() is used to reclaim resources associated with a libxml2 token
 * struct.
 */
void
xmlFreeMutex(xmlMutexPtr tok)
{
    if ( tok == KD_NULL )
	{
        return;
	}

    kdThreadMutexFree  ( tok->lock );
	
    kdFree ( tok );
}

/**
 * xmlMutexLock:
 * @tok:  the simple mutex
 *
 * xmlMutexLock() is used to lock a libxml2 token.
 */
void
xmlMutexLock(xmlMutexPtr tok)
{
    if ( tok == KD_NULL )
	{
        return;
	}

    kdThreadMutexLock ( tok->lock );	
}

/**
 * xmlMutexUnlock:
 * @tok:  the simple mutex
 *
 * xmlMutexUnlock() is used to unlock a libxml2 token.
 */
void
xmlMutexUnlock(xmlMutexPtr tok)
{
    if ( tok == KD_NULL )
	{
        return;
	}
	
	kdThreadMutexUnlock ( tok->lock );	
}

/**
 * xmlNewRMutex:
 *
 * xmlRNewMutex() is used to allocate a reentrant mutex for use in
 * synchronizing access to data. token_r is a re-entrant lock and thus useful
 * for synchronizing access to data structures that may be manipulated in a
 * recursive fashion.
 *
 * Returns the new reentrant mutex pointer or KD_NULL in case of error
 */
xmlRMutexPtr
xmlNewRMutex(void)
{
    xmlRMutexPtr tok;

    if ( ( tok = (xmlRMutexPtr) kdMalloc ( sizeof ( xmlRMutex ) ) ) == KD_NULL )
	{
        return (KD_NULL);
	}

	tok->lock    = kdThreadMutexCreate ( KD_NULL );
    tok->held    = 0;
    tok->waiters = 0;
    tok->cv      = kdThreadCondCreate ( KD_NULL );

    return (tok);
}

/**
 * xmlFreeRMutex:
 * @tok:  the reentrant mutex
 *
 * xmlRFreeMutex() is used to reclaim resources associated with a
 * reentrant mutex.
 */
void
xmlFreeRMutex(xmlRMutexPtr tok ATTRIBUTE_UNUSED)
{
    if ( tok == KD_NULL )
	{
        return;
	}

    kdThreadMutexFree ( tok->lock );
    kdThreadCondFree ( tok->cv );

    kdFree ( tok );
}

/**
 * xmlRMutexLock:
 * @tok:  the reentrant mutex
 *
 * xmlRMutexLock() is used to lock a libxml2 token_r.
 */
void
xmlRMutexLock(xmlRMutexPtr tok)
{
    if ( tok == KD_NULL )
	{
        return;
	}

    kdThreadMutexLock ( tok->lock );

    if ( tok->held )
	{
        if ( tok->tid == kdThreadSelf ( ) ) 
		{
            tok->held++;
            kdThreadMutexUnlock ( tok->lock );
            return;
        } 
		else 
		{
            tok->waiters++;
            while ( tok->held )
			{
                kdThreadCondWait ( tok->cv, tok->lock );
			}
            tok->waiters--;
        }
    }
    tok->tid = kdThreadSelf ( );
    tok->held = 1;
    kdThreadMutexUnlock ( tok->lock );
}

/**
 * xmlRMutexUnlock:
 * @tok:  the reentrant mutex
 *
 * xmlRMutexUnlock() is used to unlock a libxml2 token_r.
 */
void
xmlRMutexUnlock(xmlRMutexPtr tok ATTRIBUTE_UNUSED)
{
    if ( tok == KD_NULL )
	{
        return;
	}

    kdThreadMutexLock ( tok->lock );

    tok->held--;
    if ( tok->held == 0 ) 
	{
        if ( tok->waiters )
		{
            kdThreadCondSignal ( tok->cv );
		}

		tok->tid = 0;
    }

    kdThreadMutexUnlock ( tok->lock );
}

/**
 * xmlGlobalInitMutexLock
 *
 * Makes sure that the global initialization mutex is initialized and
 * locks it.
 */
void
__xmlGlobalInitMutexLock(void)
{
    /* Make sure the global init lock is initialized and then lock it. */
#ifdef HAVE_PTHREAD_H
    /* The mutex is statically initialized, so we just lock it. */
    if (pthread_mutex_lock)
        pthread_mutex_lock(&global_init_lock);
#elif defined HAVE_WIN32_THREADS
    LPCRITICAL_SECTION cs;

    /* Create a new critical section */
    if (global_init_lock == KD_NULL) {
        cs = malloc(sizeof(CRITICAL_SECTION));
        if (cs == KD_NULL) {
            xmlGenericError(xmlGenericErrorContext,
                            "xmlGlobalInitMutexLock: out of memory\n");
            return;
        }
        InitializeCriticalSection(cs);

        /* Swap it into the global_init_lock */
#ifdef InterlockedCompareExchangePointer
        InterlockedCompareExchangePointer(&global_init_lock, cs, KD_NULL);
#else /* Use older void* version */
        InterlockedCompareExchange((void **) &global_init_lock,
                                   (void *) cs, KD_NULL);
#endif /* InterlockedCompareExchangePointer */

        /* If another thread successfully recorded its critical
         * section in the global_init_lock then discard the one
         * allocated by this thread. */
        if (global_init_lock != cs) {
            DeleteCriticalSection(cs);
            free(cs);
        }
    }

    /* Lock the chosen critical section */
    EnterCriticalSection(global_init_lock);
#elif defined HAVE_BEOS_THREADS
    int32 sem;

    /* Allocate a new semaphore */
    sem = create_sem(1, "xmlGlobalinitMutex");

    while (global_init_lock == -1) {
        if (atomic_add(&global_init_count, 1) == 0) {
            global_init_lock = sem;
        } else {
            snooze(1);
            atomic_add(&global_init_count, -1);
        }
    }

    /* If another thread successfully recorded its critical
     * section in the global_init_lock then discard the one
     * allocated by this thread. */
    if (global_init_lock != sem)
        delete_sem(sem);

    /* Acquire the chosen semaphore */
    if (acquire_sem(global_init_lock) != B_NO_ERROR) {
#ifdef DEBUG_THREADS
        xmlGenericError(xmlGenericErrorContext,
                        "xmlGlobalInitMutexLock():BeOS:Couldn't acquire semaphore\n");
        exit();
#endif
    }
#endif
}

void
__xmlGlobalInitMutexUnlock(void)
{
#ifdef HAVE_PTHREAD_H
    if (pthread_mutex_unlock)
        pthread_mutex_unlock(&global_init_lock);
#elif defined HAVE_WIN32_THREADS
    if (global_init_lock != KD_NULL) {
	LeaveCriticalSection(global_init_lock);
    }
#elif defined HAVE_BEOS_THREADS
    release_sem(global_init_lock);
#endif
}

/**
 * xmlGlobalInitMutexDestroy
 *
 * Makes sure that the global initialization mutex is destroyed before
 * application termination.
 */
void
__xmlGlobalInitMutexDestroy(void)
{
#ifdef HAVE_PTHREAD_H
#elif defined HAVE_WIN32_THREADS
    if (global_init_lock != KD_NULL) {
        DeleteCriticalSection(global_init_lock);
        free(global_init_lock);
        global_init_lock = KD_NULL;
    }
#endif
}

/************************************************************************
 *									*
 *			Per thread global state handling		*
 *									*
 ************************************************************************/

#ifdef LIBXML_THREAD_ENABLED
#ifdef xmlLastError
#undef xmlLastError
#endif

/**
 * xmlFreeGlobalState:
 * @state:  a thread global state
 *
 * xmlFreeGlobalState() is called when a thread terminates with a non-KD_NULL
 * global state. It is is used here to reclaim memory resources.
 */
static void
xmlFreeGlobalState(void *state)
{
    xmlGlobalState *gs = (xmlGlobalState *) state;

    /* free any memory allocated in the thread's xmlLastError */
    xmlResetError(&(gs->xmlLastError));
    free(state);
}

/**
 * xmlNewGlobalState:
 *
 * xmlNewGlobalState() allocates a global state. This structure is used to
 * hold all data for use by a thread when supporting backwards compatibility
 * of libxml2 to pre-thread-safe behaviour.
 *
 * Returns the newly allocated xmlGlobalStatePtr or KD_NULL in case of error
 */
static xmlGlobalStatePtr
xmlNewGlobalState(void)
{
    xmlGlobalState *gs;

    gs = malloc(sizeof(xmlGlobalState));
    if (gs == KD_NULL) {
	xmlGenericError(xmlGenericErrorContext,
			"xmlGetGlobalState: out of memory\n");
        return (KD_NULL);
    }

    memset(gs, 0, sizeof(xmlGlobalState));
    xmlInitializeGlobalState(gs);
    return (gs);
}
#endif /* LIBXML_THREAD_ENABLED */

#ifdef HAVE_PTHREAD_H
#elif defined HAVE_WIN32_THREADS
#if !defined(HAVE_COMPILER_TLS)
#if defined(LIBXML_STATIC) && !defined(LIBXML_STATIC_FOR_DLL)
typedef struct _xmlGlobalStateCleanupHelperParams {
    HANDLE thread;
    void *memory;
} xmlGlobalStateCleanupHelperParams;

static void XMLCDECL
xmlGlobalStateCleanupHelper(void *p)
{
    xmlGlobalStateCleanupHelperParams *params =
        (xmlGlobalStateCleanupHelperParams *) p;
    WaitForSingleObject(params->thread, INFINITE);
    CloseHandle(params->thread);
    xmlFreeGlobalState(params->memory);
    free(params);
    _endthread();
}
#else /* LIBXML_STATIC && !LIBXML_STATIC_FOR_DLL */

typedef struct _xmlGlobalStateCleanupHelperParams {
    void *memory;
    struct _xmlGlobalStateCleanupHelperParams *prev;
    struct _xmlGlobalStateCleanupHelperParams *next;
} xmlGlobalStateCleanupHelperParams;

static xmlGlobalStateCleanupHelperParams *cleanup_helpers_head = KD_NULL;
static CRITICAL_SECTION cleanup_helpers_cs;

#endif /* LIBXMLSTATIC && !LIBXML_STATIC_FOR_DLL */
#endif /* HAVE_COMPILER_TLS */
#endif /* HAVE_WIN32_THREADS */

#if defined HAVE_BEOS_THREADS

/**
 * xmlGlobalStateCleanup:
 * @data: unused parameter
 *
 * Used for Beos only
 */
void
xmlGlobalStateCleanup(void *data)
{
    void *globalval = tls_get(globalkey);

    if (globalval != KD_NULL)
        xmlFreeGlobalState(globalval);
}
#endif

/**
 * xmlGetGlobalState:
 *
 * xmlGetGlobalState() is called to retrieve the global state for a thread.
 *
 * Returns the thread global state or KD_NULL in case of error
 */
xmlGlobalStatePtr
xmlGetGlobalState(void)
{
#ifdef HAVE_PTHREAD_H
    xmlGlobalState *globalval;

    if (libxml_is_threaded == 0)
        return (KD_NULL);

    pthread_once(&once_control, xmlOnceInit);

    if ((globalval = (xmlGlobalState *)
         pthread_getspecific(globalkey)) == KD_NULL) {
        xmlGlobalState *tsd = xmlNewGlobalState();
	if (tsd == KD_NULL)
	    return(KD_NULL);

        pthread_setspecific(globalkey, tsd);
        return (tsd);
    }
    return (globalval);
#elif defined HAVE_WIN32_THREADS
#if defined(HAVE_COMPILER_TLS)
    if (!tlstate_inited) {
        tlstate_inited = 1;
        xmlInitializeGlobalState(&tlstate);
    }
    return &tlstate;
#else /* HAVE_COMPILER_TLS */
    xmlGlobalState *globalval;
    xmlGlobalStateCleanupHelperParams *p;

    xmlOnceInit();
#if defined(LIBXML_STATIC) && !defined(LIBXML_STATIC_FOR_DLL)
    globalval = (xmlGlobalState *) TlsGetValue(globalkey);
#else
    p = (xmlGlobalStateCleanupHelperParams *) TlsGetValue(globalkey);
    globalval = (xmlGlobalState *) (p ? p->memory : KD_NULL);
#endif
    if (globalval == KD_NULL) {
        xmlGlobalState *tsd = xmlNewGlobalState();

        if (tsd == KD_NULL)
	    return(KD_NULL);
        p = (xmlGlobalStateCleanupHelperParams *)
            malloc(sizeof(xmlGlobalStateCleanupHelperParams));
	if (p == KD_NULL) {
            xmlGenericError(xmlGenericErrorContext,
                            "xmlGetGlobalState: out of memory\n");
            xmlFreeGlobalState(tsd);
	    return(KD_NULL);
	}
        p->memory = tsd;
#if defined(LIBXML_STATIC) && !defined(LIBXML_STATIC_FOR_DLL)
        DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
                        GetCurrentProcess(), &p->thread, 0, TRUE,
                        DUPLICATE_SAME_ACCESS);
        TlsSetValue(globalkey, tsd);
        _beginthread(xmlGlobalStateCleanupHelper, 0, p);
#else
        EnterCriticalSection(&cleanup_helpers_cs);
        if (cleanup_helpers_head != KD_NULL) {
            cleanup_helpers_head->prev = p;
        }
        p->next = cleanup_helpers_head;
        p->prev = KD_NULL;
        cleanup_helpers_head = p;
        TlsSetValue(globalkey, p);
        LeaveCriticalSection(&cleanup_helpers_cs);
#endif

        return (tsd);
    }
    return (globalval);
#endif /* HAVE_COMPILER_TLS */
#elif defined HAVE_BEOS_THREADS
    xmlGlobalState *globalval;

    xmlOnceInit();

    if ((globalval = (xmlGlobalState *) tls_get(globalkey)) == KD_NULL) {
        xmlGlobalState *tsd = xmlNewGlobalState();
	if (tsd == KD_NULL)
	    return (KD_NULL);

        tls_set(globalkey, tsd);
        on_exit_thread(xmlGlobalStateCleanup, KD_NULL);
        return (tsd);
    }
    return (globalval);
#else
    return (KD_NULL);
#endif
}

/************************************************************************
 *									*
 *			Library wide thread interfaces			*
 *									*
 ************************************************************************/

/**
 * xmlGetThreadId:
 *
 * xmlGetThreadId() find the current thread ID number
 * Note that this is likely to be broken on some platforms using pthreads
 * as the specification doesn't mandate pthread_t to be an integer type
 *
 * Returns the current thread ID number
 */
int
xmlGetThreadId(void)
{
#ifdef HAVE_PTHREAD_H
    pthread_t id;
    int ret;

    if (libxml_is_threaded == 0)
        return (0);
    id = pthread_self();
    /* horrible but preserves compat, see warning above */
    memcpy(&ret, &id, sizeof(ret));
    return (ret);
#elif defined HAVE_WIN32_THREADS
    return GetCurrentThreadId();
#elif defined HAVE_BEOS_THREADS
    return find_thread(KD_NULL);
#else
    return ((int) 0);
#endif
}

/**
 * xmlIsMainThread:
 *
 * xmlIsMainThread() check whether the current thread is the main thread.
 *
 * Returns 1 if the current thread is the main thread, 0 otherwise
 */
int
xmlIsMainThread(void)
{
#ifdef HAVE_PTHREAD_H
    if (libxml_is_threaded == -1)
        xmlInitThreads();
    if (libxml_is_threaded == 0)
        return (1);
    pthread_once(&once_control, xmlOnceInit);
#elif defined HAVE_WIN32_THREADS
    xmlOnceInit();
#elif defined HAVE_BEOS_THREADS
    xmlOnceInit();
#endif

#ifdef DEBUG_THREADS
    xmlGenericError(xmlGenericErrorContext, "xmlIsMainThread()\n");
#endif
#ifdef HAVE_PTHREAD_H
    return (pthread_equal(mainthread,pthread_self()));
#elif defined HAVE_WIN32_THREADS
    return (mainthread == GetCurrentThreadId());
#elif defined HAVE_BEOS_THREADS
    return (mainthread == find_thread(KD_NULL));
#else
    return (1);
#endif
}

/**
 * xmlLockLibrary:
 *
 * xmlLockLibrary() is used to take out a re-entrant lock on the libxml2
 * library.
 */
void
xmlLockLibrary(void)
{
#ifdef DEBUG_THREADS
    xmlGenericError(xmlGenericErrorContext, "xmlLockLibrary()\n");
#endif
    xmlRMutexLock(xmlLibraryLock);
}

/**
 * xmlUnlockLibrary:
 *
 * xmlUnlockLibrary() is used to release a re-entrant lock on the libxml2
 * library.
 */
void
xmlUnlockLibrary(void)
{
#ifdef DEBUG_THREADS
    xmlGenericError(xmlGenericErrorContext, "xmlUnlockLibrary()\n");
#endif
    xmlRMutexUnlock(xmlLibraryLock);
}

/**
 * xmlInitThreads:
 *
 * xmlInitThreads() is used to to initialize all the thread related
 * data of the libxml2 library.
 */
void
xmlInitThreads(void)
{
#ifdef HAVE_PTHREAD_H
    if (libxml_is_threaded == -1) {
        if ((pthread_once != KD_NULL) &&
            (pthread_getspecific != KD_NULL) &&
            (pthread_setspecific != KD_NULL) &&
            (pthread_key_create != KD_NULL) &&
            (pthread_key_delete != KD_NULL) &&
            (pthread_mutex_init != KD_NULL) &&
            (pthread_mutex_destroy != KD_NULL) &&
            (pthread_mutex_lock != KD_NULL) &&
            (pthread_mutex_unlock != KD_NULL) &&
            (pthread_cond_init != KD_NULL) &&
            (pthread_cond_destroy != KD_NULL) &&
            (pthread_cond_wait != KD_NULL) &&
            (pthread_equal != KD_NULL) &&
            (pthread_self != KD_NULL) &&
            (pthread_cond_signal != KD_NULL)) {
            libxml_is_threaded = 1;

/* fprintf(stderr, "Running multithreaded\n"); */
        } else {

/* fprintf(stderr, "Running without multithread\n"); */
            libxml_is_threaded = 0;
        }
    }
#elif defined(HAVE_WIN32_THREADS) && !defined(HAVE_COMPILER_TLS) && (!defined(LIBXML_STATIC) || defined(LIBXML_STATIC_FOR_DLL))
    InitializeCriticalSection(&cleanup_helpers_cs);
#endif
}

/**
 * xmlCleanupThreads:
 *
 * xmlCleanupThreads() is used to to cleanup all the thread related
 * data of the libxml2 library once processing has ended.
 *
 * WARNING: if your application is multithreaded or has plugin support
 *          calling this may crash the application if another thread or
 *          a plugin is still using libxml2. It's sometimes very hard to
 *          guess if libxml2 is in use in the application, some libraries
 *          or plugins may use it without notice. In case of doubt abstain
 *          from calling this function or do it just before calling exit()
 *          to avoid leak reports from valgrind !
 */
void
xmlCleanupThreads(void)
{
#ifdef DEBUG_THREADS
    xmlGenericError(xmlGenericErrorContext, "xmlCleanupThreads()\n");
#endif
#ifdef HAVE_PTHREAD_H
    if ((libxml_is_threaded)  && (pthread_key_delete != KD_NULL))
        pthread_key_delete(globalkey);
#elif defined(HAVE_WIN32_THREADS) && !defined(HAVE_COMPILER_TLS) && (!defined(LIBXML_STATIC) || defined(LIBXML_STATIC_FOR_DLL))
    if (globalkey != TLS_OUT_OF_INDEXES) {
        xmlGlobalStateCleanupHelperParams *p;

        EnterCriticalSection(&cleanup_helpers_cs);
        p = cleanup_helpers_head;
        while (p != KD_NULL) {
            xmlGlobalStateCleanupHelperParams *temp = p;

            p = p->next;
            xmlFreeGlobalState(temp->memory);
            free(temp);
        }
        cleanup_helpers_head = 0;
        LeaveCriticalSection(&cleanup_helpers_cs);
        TlsFree(globalkey);
        globalkey = TLS_OUT_OF_INDEXES;
    }
    DeleteCriticalSection(&cleanup_helpers_cs);
#endif
}

#ifdef LIBXML_THREAD_ENABLED

/**
 * xmlOnceInit
 *
 * xmlOnceInit() is used to initialize the value of mainthread for use
 * in other routines. This function should only be called using
 * pthread_once() in association with the once_control variable to ensure
 * that the function is only called once. See man pthread_once for more
 * details.
 */
static void
xmlOnceInit(void)
{
#ifdef HAVE_PTHREAD_H
    (void) pthread_key_create(&globalkey, xmlFreeGlobalState);
    mainthread = pthread_self();
#elif defined(HAVE_WIN32_THREADS)
    if (!run_once.done) {
        if (InterlockedIncrement(&run_once.control) == 1) {
#if !defined(HAVE_COMPILER_TLS)
            globalkey = TlsAlloc();
#endif
            mainthread = GetCurrentThreadId();
            run_once.done = 1;
        } else {
            /* Another thread is working; give up our slice and
             * wait until they're done. */
            while (!run_once.done)
                Sleep(0);
        }
    }
#elif defined HAVE_BEOS_THREADS
    if (atomic_add(&run_once_init, 1) == 0) {
        globalkey = tls_allocate();
        tls_set(globalkey, KD_NULL);
        mainthread = find_thread(KD_NULL);
    } else
        atomic_add(&run_once_init, -1);
#endif
}
#endif

/**
 * DllMain:
 * @hinstDLL: handle to DLL instance
 * @fdwReason: Reason code for entry
 * @lpvReserved: generic pointer (depends upon reason code)
 *
 * Entry point for Windows library. It is being used to free thread-specific
 * storage.
 *
 * Returns TRUE always
 */
#ifdef HAVE_PTHREAD_H
#elif defined(HAVE_WIN32_THREADS) && !defined(HAVE_COMPILER_TLS) && (!defined(LIBXML_STATIC) || defined(LIBXML_STATIC_FOR_DLL))
#if defined(LIBXML_STATIC_FOR_DLL)
BOOL XMLCALL
xmlDllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
#else
BOOL WINAPI
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
#endif
{
    switch (fdwReason) {
        case DLL_THREAD_DETACH:
            if (globalkey != TLS_OUT_OF_INDEXES) {
                xmlGlobalState *globalval = KD_NULL;
                xmlGlobalStateCleanupHelperParams *p =
                    (xmlGlobalStateCleanupHelperParams *)
                    TlsGetValue(globalkey);
                globalval = (xmlGlobalState *) (p ? p->memory : KD_NULL);
                if (globalval) {
                    xmlFreeGlobalState(globalval);
                    TlsSetValue(globalkey, KD_NULL);
                }
                if (p) {
                    EnterCriticalSection(&cleanup_helpers_cs);
                    if (p == cleanup_helpers_head)
                        cleanup_helpers_head = p->next;
                    else
                        p->prev->next = p->next;
                    if (p->next != KD_NULL)
                        p->next->prev = p->prev;
                    LeaveCriticalSection(&cleanup_helpers_cs);
                    free(p);
                }
            }
            break;
    }
    return TRUE;
}
#endif
#define bottom_threads
#include "elfgcchack.h"
