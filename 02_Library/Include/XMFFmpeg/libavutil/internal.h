/*
 * copyright (c) 2006 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * common internal API header
 */

#ifndef AVUTIL_INTERNAL_H
#define AVUTIL_INTERNAL_H

#if !defined(DEBUG) && !defined(NDEBUG)
#    define NDEBUG
#endif

#include "../type.h"
#include "../config.h"
#include "attributes.h"
#include "timer.h"
#include "cpu.h"
#include "dict.h"

struct AVDictionary {
    int count;
    AVDictionaryEntry *elems;
};

#ifndef attribute_align_arg
#if ARCH_X86_32 && AV_GCC_VERSION_AT_LEAST(4,2)
#    define attribute_align_arg __attribute__((force_align_arg_pointer))
#else
#    define attribute_align_arg
#endif
#endif

#ifndef INT8_MIN
#define INT8_MIN        ( -0x7f - 1 )
#endif

#ifndef INT8_MAX
#define INT8_MAX        0x7f
#endif

#ifndef UINT8_MAX
#define UINT8_MAX       0xffU
#endif

#ifndef INT16_MIN
#define INT16_MIN       (-0x7fff - 1)
#endif

#ifndef INT16_MAX
#define INT16_MAX       0x7fff
#endif

#ifndef UINT16_MAX
#define UINT16_MAX		0xffffU
#endif

#ifndef INT32_MIN
#define INT32_MIN       (-0x7fffffff - 1)
#endif

#ifndef INT32_MAX
#define INT32_MAX       0x7fffffff
#endif

#ifndef UINT32_MAX
#define UINT32_MAX      0xffffffffU
#endif

#ifndef INT64_MIN
#define INT64_MIN       (-0x7fffffffffffffffLL - 1)
#endif

#ifndef INT64_MAX
#define INT64_MAX		INT64_C(9223372036854775807)
#endif

#ifndef UINT64_MAX
#define UINT64_MAX		UINT64_C(0xFFFFFFFFFFFFFFFF)
#endif

#if !defined (ANDROID)
#ifndef CHAR_MIN
#define CHAR_MIN        INT8_MIN
#endif

#ifndef CHAR_MAX
#define CHAR_MAX        INT8_MAX
#endif

#ifndef INT_MIN
#define INT_MIN         INT32_MIN
#endif

#ifndef INT_MAX
#define INT_MAX         INT32_MAX
#endif

#ifndef UINT_MAX
#define UINT_MAX        UINT32_MAX
#endif
#endif // ANDROID

#ifndef SIZE_MAX
#define SIZE_MAX        0x7fffffff
#endif

#ifndef CHAR_BIT
#define CHAR_BIT        8
#endif

#ifndef INT_BIT
#    define INT_BIT (CHAR_BIT * sizeof(int))
#endif

#ifndef offsetof
#define offsetof(s,m)   (size_t)&reinterpret_cast<const volatile char&>((((s *)0)->m))
//#    define offsetof(T, F) ((unsigned int)((char *)&((T *)0)->F))
#endif

/* Use to export labels from asm. */
#define LABEL_MANGLE(a) EXTERN_PREFIX #a

// Use rip-relative addressing if compiling PIC code on x86-64.
#if ARCH_X86_64 && defined(PIC)
#    define LOCAL_MANGLE(a) #a "(%%rip)"
#else
#    define LOCAL_MANGLE(a) #a
#endif

#define MANGLE(a) EXTERN_PREFIX LOCAL_MANGLE(a)

/* debug stuff */
#define av_abort()      do { av_log(NULL, AV_LOG_ERROR, "Abort at %s:%d\n", __FILE__, __LINE__); kdExit(0); } while (0)

/* math */

#if ARCH_X86
#define MASK_ABS(mask, level)\
            __asm__ volatile(\
                "cltd                   \n\t"\
                "xorl %1, %0            \n\t"\
                "subl %1, %0            \n\t"\
                : "+a" (level), "=&d" (mask)\
            );
#else
#define MASK_ABS(mask, level)\
            mask  = level >> 31;\
            level = (level ^ mask) - mask;
#endif

/* avoid usage of dangerous/inappropriate system functions */
#undef  assert
#define assert					kdAssert
#undef  malloc
#define malloc					kdMalloc
#undef  free
#define free					kdFree
#undef  realloc
#define realloc					kdRealloc
#undef  memcpy
#define memcpy                  kdMemcpy
#undef  memset
#define memset                  kdMemset
#undef  memcmp
#define memcmp                  kdMemcmp
#undef  memchr
#define memchr                  kdMemchr
#undef  memmove
#define memmove                 kdMemmove
#undef  tm
#define tm						KDTm
#undef  time
#define time(t)					kdTime((KDtime*)t)
#undef  gmtime
#define gmtime(t)				kdGmtime_r((const KDtime*)t,0)
#undef  localtime
#define localtime(t)			kdLocaltime_r((const KDtime*)t,0)
#undef  mktime
#define mktime(t)				0
#undef  rand
#define rand					kdRand
#undef  srand
#define srand					kdSrand
#undef  random
#define random					random_is_forbidden_due_to_state_trashing_use_av_lfg_get
#undef  sprintf
#define sprintf                 kdSprintfKHR
#undef  snprintf
#define snprintf                kdSnprintfKHR
#undef	sscanf
#define	sscanf(b,f,...)			kdSscanf((KDchar*)(b),f,__VA_ARGS__)
#undef  strcat
#define strcat(b,s)				kdStrcat((KDchar*)(b),(KDchar*)(s))
#undef  strncat
#define strncat(b,s,l)			kdStrncat((KDchar*)(b),(KDchar*)(s),l)
#undef	strcpy
#define	strcpy(b,s)				kdStrcpy((KDchar*)(b),(const KDchar*)(s))
#undef  strncpy
#define strncpy(b,s,m)			kdStrncpy((KDchar*)(b),(KDchar*)(s),m)
#undef	strlen
#define	strlen(s)				kdStrlen((const KDchar*)(s))
#undef	strchr
#define	strchr(s,c)				kdStrchr((const KDchar*)(s),c)
#undef	strrchr
#define	strrchr(s,c)			kdStrrchr((const KDchar*)(s),c)
#undef	strcmp
#define	strcmp(s1,s2)			kdStrcmp((const KDchar*)(s1),(const KDchar*)(s2))
#undef	strncmp				
#define	strncmp(s1,s2,m)		kdStrncmp((const KDchar*)(s1),(const KDchar*)(s2),m)
#undef  strstr
#define strstr                  kdStrstr
#undef  strspn
#define strspn                  kdStrspn
#undef  strcspn
#define strcspn                 kdStrcspn
#undef  isspace
#define isspace                 kdIsspace
#undef  isdigit
#define isdigit                 kdIsdigit
#undef  isprint
#define isprint                 kdIsprint
#undef  toupper
#define toupper                 kdToupper
#undef  exit
#define exit					kdExit
#undef  printf
#define printf					kdLogMessagefKHR
#undef  fprintf
#define fprintf					kdFprintf
#undef  puts
#define puts					kdPuts
#undef  perror
#define perror					kdLogMessage
#undef	strcasecmp
#define strcasecmp				kdStrcasecmp
#undef	strncasecmp
#define strncasecmp				kdStrncasecmp
#undef	vsnprintf	
#define vsnprintf				kdVsnprintfKHR
#ifndef va_list
#define va_list					KDVaListKHR	
#endif
#ifndef va_start
#define va_start				KD_VA_START_KHR
#endif
#ifndef va_end
#define va_end					KD_VA_END_KHR
#endif

#undef  isatty  
#define isatty                  kdIsatty
#undef  open
#define	open					kdOpen
#undef  close
#define	close					kdClose
#undef	O_RDONLY
#define O_RDONLY				KD_O_RDONLY
#undef	O_WRONLY
#define O_WRONLY				KD_O_WRONLY
#undef	O_RDWR
#define	O_RDWR					KD_O_RDWR
#undef	O_APPEND
#define O_APPEND				KD_O_APPEND
#undef	O_CREAT
#define O_CREAT					KD_O_CREAT
#undef	O_TRUNC
#define O_TRUNC					KD_O_TRUNC
#undef	O_EXCL
#define O_EXCL					KD_O_EXCL
#undef  SEEK_SET
#define SEEK_SET                KD_SEEK_SET
#undef  SEEK_CUR
#define SEEK_CUR                KD_SEEK_CUR
#undef  SEEK_END
#define SEEK_END                KD_SEEK_END
#undef  FILE
#define FILE					KDFile
#undef  qsort
#define	qsort(b,n,s,c)			kdQsort(b,n,s,(KDint (*)(const void *,const void *))c)
#undef	bsearch
#define	bsearch(k,b,n,s,c)		kdBsearch(k,b,n,s,(KDint(*)(const KDvoid*,const KDvoid*))c)
#undef	unlink
#define	unlink(x)		
#undef	strftime
#define	strftime
#undef  strerror
#define strerror(x)             "error"

#undef	strtol
#define	strtol(s,e,b)			kdStrtol((const KDchar*)(s),(KDchar**)(e),b)
#undef  strtoll
#define strtoll(s,e,b)			kdStrtol((const KDchar*)(s),(KDchar**)(e),b)
#undef	strtoul
#define	strtoul(s,e,b)			kdStrtoul((const KDchar*)(s),(KDchar**)(e),b)
#undef  strtod
#define strtod(s,e)             kdStrtod((const KDchar*)(s),(KDchar**)(e))
#undef	atoll
#define atoll(x)				strtol(x,0,0)
#undef  atoi
#define atoi(x)                 strtol(x,0,0)

#if     defined (__APPLE__)
//#include <stdlib.h>
#else
#undef	usleep
#define usleep(t)				kdSleep(t*1000000ULL)
#undef	read			
#define	read					kdRead
#undef	write	
#define	write					kdWrite
#undef	lseek
#define	lseek(f,p,s)			kdLseek(f,p,s)
#endif

#define FF_ALLOC_OR_GOTO(cast, ctx, p, size, label)\
{\
    p = (cast)av_malloc(size);\
    if (p == NULL && (size) != 0) {\
        av_log(ctx, AV_LOG_ERROR, "Cannot allocate memory.\n");\
        goto label;\
    }\
}

#define FF_ALLOCZ_OR_GOTO(cast, ctx, p, size, label)\
{\
    p = (cast)av_mallocz(size);\
    if (p == NULL && (size) != 0) {\
        av_log(ctx, AV_LOG_ERROR, "Cannot allocate memory.\n");\
        goto label;\
    }\
}

#include "libm.h"

/**
 * Return NULL if CONFIG_SMALL is true, otherwise the argument
 * without modification. Used to disable the definition of strings
 * (for example AVCodec long_names).
 */
#if CONFIG_SMALL
#   define NULL_IF_CONFIG_SMALL(x) NULL
#else
#   define NULL_IF_CONFIG_SMALL(x) x
#endif

/**
 * Define a function with only the non-default version specified.
 *
 * On systems with ELF shared libraries, all symbols exported from
 * FFmpeg libraries are tagged with the name and major version of the
 * library to which they belong.  If a function is moved from one
 * library to another, a wrapper must be retained in the original
 * location to preserve binary compatibility.
 *
 * Functions defined with this macro will never be used to resolve
 * symbols by the build-time linker.
 *
 * @param type return type of function
 * @param name name of function
 * @param args argument list of function
 * @param ver  version tag to assign function
 */
#if HAVE_SYMVER_ASM_LABEL
#   define FF_SYMVER(type, name, args, ver)                     \
    type ff_##name args __asm__ (EXTERN_PREFIX #name "@" ver);  \
    type ff_##name args
#elif HAVE_SYMVER_GNU_ASM
#   define FF_SYMVER(type, name, args, ver)                             \
    __asm__ (".symver ff_" #name "," EXTERN_PREFIX #name "@" ver);      \
    type ff_##name args;                                                \
    type ff_##name args
#endif

/**
 * Return NULL if a threading library has not been enabled.
 * Used to disable threading functions in AVCodec definitions
 * when not needed.
 */
#if HAVE_THREADS
#   define ONLY_IF_THREADS_ENABLED(x) x
#else
#   define ONLY_IF_THREADS_ENABLED(x) NULL
#endif

#if HAVE_MMX
/**
 * Empty mmx state.
 * this must be called between any dsp function and float/double code.
 * for example sin(); dsp->idct_put(); emms_c(); cos()
 */
static av_always_inline void emms_c(void)
{
    if(av_get_cpu_flags() & AV_CPU_FLAG_MMX)
        __asm__ volatile ("emms" ::: "memory");
}
#else /* HAVE_MMX */
#define emms_c()
#endif /* HAVE_MMX */

#endif /* AVUTIL_INTERNAL_H */
