#ifndef FFMPEG_TYPE_H
#define FFMPEG_TYPE_H

#include <stdlib.h>
#include <cstdlib>
#include <kode.h>

typedef signed char         int8_t;
typedef khronos_int16_t		int16_t;
typedef khronos_int32_t		int32_t;
typedef khronos_int64_t		int64_t;

typedef khronos_uint8_t		uint8_t;
typedef khronos_uint16_t	uint16_t;
typedef khronos_uint32_t	uint32_t;
typedef khronos_uint64_t	uint64_t;

#if defined (SHP)
typedef int 				ptrdiff_t;
typedef KDint				time_t;
#endif

#if defined (Linux)
typedef int 				ptrdiff_t;
#include <time.h>
#endif

#if   defined (__APPLE__) 
#include <inttypes.h>
#define HAVE_INTTYPE
#elif defined ( _WIN32 )
#define PRI64_PREFIX		"%l64"
#endif

#ifndef HAVE_INTTYPE

#ifndef PRI64_PREFIX
#define PRI64_PREFIX		"%ll"
#endif

#define PRId8				"%hhd"
#define PRIi8				"%hhi"
#define PRIu8				"%hhu"
#define PRIx8				"%hhx"

#define PRId16				"%hd"
#define PRIi16				"%hi"
#define PRIu16				"%hu"
#define PRIx16				"%hx"

#define PRId32				"%ld"
#define PRIi32				"%li"
#define PRIu32				"%lu"
#define PRIx32				"%lx"

#define PRId64				PRI64_PREFIX "d"
#define PRIi64				PRI64_PREFIX "i"
#define PRIu64				PRI64_PREFIX "u"
#define PRIx64				PRI64_PREFIX "x"
#define PRIX64				PRI64_PREFIX "X"

#define SCNd64				PRI64_PREFIX "d"
#endif

#ifndef INT64_C
#define INT64_C(val)		val##LL
#endif

#ifndef UINT64_C
#define UINT64_C(val)		val##ULL
#endif

#endif /* FFMPEG_DEFINE_H */
