
/* pngmem.c - stub functions for memory allocation
 *
 * Last changed in libpng 1.5.13 [September 27, 2012]
 * Copyright (c) 1998-2012 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 *
 * This file provides a location for all memory allocation.  Users who
 * need special memory handling are expected to supply replacement
 * functions for _png_malloc() and _png_free(), and to use
 * _png_create_read_struct_2() and _png_create_write_struct_2() to
 * identify the replacement functions.
 */

#include "pngpriv.h"

#if defined(PNG_READ_SUPPORTED) || defined(PNG_WRITE_SUPPORTED)

/* Borland DOS special memory handler */
#if defined(__TURBOC__) && !defined(_Windows) && !defined(__FLAT__)
/* If you change this, be sure to change the one in png.h also */

/* Allocate memory for a _png_struct.  The malloc and memset can be replaced
   by a single call to calloc() if this is thought to improve performance. */
PNG_FUNCTION(_png_voidp /* PRIVATE */,
_png_create_struct,(int type),PNG_ALLOCATED)
{
#  ifdef PNG_USER_MEM_SUPPORTED
   return (_png_create_struct_2(type, NULL, NULL));
}

/* Alternate version of _png_create_struct, for use with user-defined malloc. */
PNG_FUNCTION(_png_voidp /* PRIVATE */,
_png_create_struct_2,(int type, _png_malloc_ptr malloc_fn, _png_voidp mem_ptr),
   PNG_ALLOCATED)
{
#  endif /* PNG_USER_MEM_SUPPORTED */
   _png_size_t size;
   _png_voidp struct_ptr;

   if (type == PNG_STRUCT_INFO)
      size = _png_sizeof(_png_info);

   else if (type == PNG_STRUCT_PNG)
      size = _png_sizeof(_png_struct);

   else
      return (_png_get_copyright(NULL));

#  ifdef PNG_USER_MEM_SUPPORTED
   if (malloc_fn != NULL)
   {
      _png_struct dummy_struct;
      _png_memset(&dummy_struct, 0, sizeof dummy_struct);
      dummy_struct.mem_ptr=mem_ptr;
      struct_ptr = (*(malloc_fn))(&dummy_struct, (_png_alloc_size_t)size);
   }

   else
#  endif /* PNG_USER_MEM_SUPPORTED */
   struct_ptr = (_png_voidp)farmalloc(size);
   if (struct_ptr != NULL)
      _png_memset(struct_ptr, 0, size);

   return (struct_ptr);
}

/* Free memory allocated by a _png_create_struct() call */
void /* PRIVATE */
_png_destroy_struct(_png_voidp struct_ptr)
{
#  ifdef PNG_USER_MEM_SUPPORTED
   _png_destroy_struct_2(struct_ptr, NULL, NULL);
}

/* Free memory allocated by a _png_create_struct() call */
void /* PRIVATE */
_png_destroy_struct_2(_png_voidp struct_ptr, _png_free_ptr free_fn,
    _png_voidp mem_ptr)
{
#  endif
   if (struct_ptr != NULL)
   {
#  ifdef PNG_USER_MEM_SUPPORTED
      if (free_fn != NULL)
      {
         _png_struct dummy_struct;
         _png_memset(&dummy_struct, 0, sizeof dummy_struct);
         dummy_struct.mem_ptr=mem_ptr;
         (*(free_fn))(&dummy_struct, struct_ptr);
         return;
      }

#  endif /* PNG_USER_MEM_SUPPORTED */
      farfree (struct_ptr);
   }
}

/* Allocate memory.  For reasonable files, size should never exceed
 * 64K.  However, zlib may allocate more then 64K if you don't tell
 * it not to.  See zconf.h and png.h for more information. zlib does
 * need to allocate exactly 64K, so whatever you call here must
 * have the ability to do that.
 *
 * Borland seems to have a problem in DOS mode for exactly 64K.
 * It gives you a segment with an offset of 8 (perhaps to store its
 * memory stuff).  zlib doesn't like this at all, so we have to
 * detect and deal with it.  This code should not be needed in
 * Windows or OS/2 modes, and only in 16 bit mode.  This code has
 * been updated by Alexander Lehmann for version 0.89 to waste less
 * memory.
 *
 * Note that we can't use _png_size_t for the "size" declaration,
 * since on some systems a _png_size_t is a 16-bit quantity, and as a
 * result, we would be truncating potentially larger memory requests
 * (which should cause a fatal error) and introducing major problems.
 */
PNG_FUNCTION(_png_voidp,PNGAPI
_png_calloc,(_png_structp _png_ptr, _png_alloc_size_t size),PNG_ALLOCATED)
{
   _png_voidp ret;

   ret = (_png_malloc(_png_ptr, size));

   if (ret != NULL)
      _png_memset(ret,0,(_png_size_t)size);

   return (ret);
}

PNG_FUNCTION(_png_voidp,PNGAPI
_png_malloc,(_png_structp _png_ptr, _png_alloc_size_t size),PNG_ALLOCATED)
{
   _png_voidp ret;

   if (_png_ptr == NULL || size == 0)
      return (NULL);

#  ifdef PNG_USER_MEM_SUPPORTED
   if (_png_ptr->malloc_fn != NULL)
      ret = ((_png_voidp)(*(_png_ptr->malloc_fn))(_png_ptr, size));

   else
      ret = (_png_malloc_default(_png_ptr, size));

   if (ret == NULL && (_png_ptr->flags&PNG_FLAG_MALLOC_NULL_MEM_OK) == 0)
       _png_error(_png_ptr, "Out of memory");

   return (ret);
}

PNG_FUNCTION(_png_voidp,PNGAPI
_png_malloc_default,(_png_structp _png_ptr, _png_alloc_size_t size),PNG_ALLOCATED)
{
   _png_voidp ret;
#  endif /* PNG_USER_MEM_SUPPORTED */

   if (_png_ptr == NULL || size == 0)
      return (NULL);

#  ifdef PNG_MAX_MALLOC_64K
   if (size > (_png_uint_32)65536L)
   {
      _png_warning(_png_ptr, "Cannot Allocate > 64K");
      ret = NULL;
   }

   else
#  endif

   if (size != (size_t)size)
      ret = NULL;

   else if (size == (_png_uint_32)65536L)
   {
      if (_png_ptr->offset_table == NULL)
      {
         /* Try to see if we need to do any of this fancy stuff */
         ret = farmalloc(size);
         if (ret == NULL || ((_png_size_t)ret & 0xffff))
         {
            int num_blocks;
            _png_uint_32 total_size;
            _png_bytep table;
            int i, mem_level, window_bits;
            _png_byte huge * hptr;
            int window_bits

            if (ret != NULL)
            {
               farfree(ret);
               ret = NULL;
            }

            window_bits =
                _png_ptr->zlib_window_bits >= _png_ptr->zlib_text_window_bits ?
                _png_ptr->zlib_window_bits : _png_ptr->zlib_text_window_bits;

            if (window_bits > 14)
               num_blocks = (int)(1 << (window_bits - 14));

            else
               num_blocks = 1;

            mem_level =
                _png_ptr->zlib_mem_level >= _png_ptr->zlib_text_mem_level ?
                _png_ptr->zlib_mem_level : _png_ptr->zlib_text_mem_level;

            if (mem_level >= 7)
               num_blocks += (int)(1 << (mem_level - 7));

            else
               num_blocks++;

            total_size = ((_png_uint_32)65536L) * (_png_uint_32)num_blocks+16;

            table = farmalloc(total_size);

            if (table == NULL)
            {
#  ifndef PNG_USER_MEM_SUPPORTED
               if ((_png_ptr->flags&PNG_FLAG_MALLOC_NULL_MEM_OK) == 0)
                  _png_error(_png_ptr, "Out Of Memory"); /* Note "O", "M" */

               else
                  _png_warning(_png_ptr, "Out Of Memory");
#  endif
               return (NULL);
            }

            if ((_png_size_t)table & 0xfff0)
            {
#  ifndef PNG_USER_MEM_SUPPORTED
               if ((_png_ptr->flags&PNG_FLAG_MALLOC_NULL_MEM_OK) == 0)
                  _png_error(_png_ptr,
                    "Farmalloc didn't return normalized pointer");

               else
                  _png_warning(_png_ptr,
                    "Farmalloc didn't return normalized pointer");
#  endif
               return (NULL);
            }

            _png_ptr->offset_table = table;
            _png_ptr->offset_table_ptr = farmalloc(num_blocks *
               _png_sizeof(_png_bytep));

            if (_png_ptr->offset_table_ptr == NULL)
            {
#  ifndef PNG_USER_MEM_SUPPORTED
               if ((_png_ptr->flags&PNG_FLAG_MALLOC_NULL_MEM_OK) == 0)
                  _png_error(_png_ptr, "Out Of memory"); /* Note "O", "m" */

               else
                  _png_warning(_png_ptr, "Out Of memory");
#  endif
               return (NULL);
            }

            hptr = (_png_byte huge *)table;
            if ((_png_size_t)hptr & 0xf)
            {
               hptr = (_png_byte huge *)((long)(hptr) & 0xfffffff0L);
               hptr = hptr + 16L;  /* "hptr += 16L" fails on Turbo C++ 3.0 */
            }

            for (i = 0; i < num_blocks; i++)
            {
               _png_ptr->offset_table_ptr[i] = (_png_bytep)hptr;
               hptr = hptr + (_png_uint_32)65536L;  /* "+=" fails on TC++3.0 */
            }

            _png_ptr->offset_table_number = num_blocks;
            _png_ptr->offset_table_count = 0;
            _png_ptr->offset_table_count_free = 0;
         }
      }

      if (_png_ptr->offset_table_count >= _png_ptr->offset_table_number)
      {
#  ifndef PNG_USER_MEM_SUPPORTED
         if ((_png_ptr->flags&PNG_FLAG_MALLOC_NULL_MEM_OK) == 0)
            _png_error(_png_ptr, "Out of Memory"); /* Note "O" and "M" */

         else
            _png_warning(_png_ptr, "Out of Memory");
#  endif
         return (NULL);
      }

      ret = _png_ptr->offset_table_ptr[_png_ptr->offset_table_count++];
   }

   else
      ret = farmalloc(size);

#  ifndef PNG_USER_MEM_SUPPORTED
   if (ret == NULL)
   {
      if ((_png_ptr->flags&PNG_FLAG_MALLOC_NULL_MEM_OK) == 0)
         _png_error(_png_ptr, "Out of memory"); /* Note "o" and "m" */

      else
         _png_warning(_png_ptr, "Out of memory"); /* Note "o" and "m" */
   }
#  endif

   return (ret);
}

/* Free a pointer allocated by _png_malloc().  In the default
 * configuration, _png_ptr is not used, but is passed in case it
 * is needed.  If ptr is NULL, return without taking any action.
 */
void PNGAPI
_png_free(_png_structp _png_ptr, _png_voidp ptr)
{
   if (_png_ptr == NULL || ptr == NULL)
      return;

#  ifdef PNG_USER_MEM_SUPPORTED
   if (_png_ptr->free_fn != NULL)
   {
      (*(_png_ptr->free_fn))(_png_ptr, ptr);
      return;
   }

   else
      _png_free_default(_png_ptr, ptr);
}

void PNGAPI
_png_free_default(_png_structp _png_ptr, _png_voidp ptr)
{
#  endif /* PNG_USER_MEM_SUPPORTED */

   if (_png_ptr == NULL || ptr == NULL)
      return;

   if (_png_ptr->offset_table != NULL)
   {
      int i;

      for (i = 0; i < _png_ptr->offset_table_count; i++)
      {
         if (ptr == _png_ptr->offset_table_ptr[i])
         {
            ptr = NULL;
            _png_ptr->offset_table_count_free++;
            break;
         }
      }
      if (_png_ptr->offset_table_count_free == _png_ptr->offset_table_count)
      {
         farfree(_png_ptr->offset_table);
         farfree(_png_ptr->offset_table_ptr);
         _png_ptr->offset_table = NULL;
         _png_ptr->offset_table_ptr = NULL;
      }
   }

   if (ptr != NULL)
      farfree(ptr);
}

#else /* Not the Borland DOS special memory handler */

/* Allocate memory for a _png_struct or a _png_info.  The malloc and
   memset can be replaced by a single call to calloc() if this is thought
   to improve performance noticably. */
PNG_FUNCTION(_png_voidp /* PRIVATE */,
_png_create_struct,(int type),PNG_ALLOCATED)
{
#  ifdef PNG_USER_MEM_SUPPORTED
   return (_png_create_struct_2(type, NULL, NULL));
}

/* Allocate memory for a _png_struct or a _png_info.  The malloc and
   memset can be replaced by a single call to calloc() if this is thought
   to improve performance noticably. */
PNG_FUNCTION(_png_voidp /* PRIVATE */,
_png_create_struct_2,(int type, _png_malloc_ptr malloc_fn, _png_voidp mem_ptr),
   PNG_ALLOCATED)
{
#  endif /* PNG_USER_MEM_SUPPORTED */
   _png_size_t size;
   _png_voidp struct_ptr;

   if (type == PNG_STRUCT_INFO)
      size = _png_sizeof(_png_info);

   else if (type == PNG_STRUCT_PNG)
      size = _png_sizeof(_png_struct);

   else
      return (NULL);

#  ifdef PNG_USER_MEM_SUPPORTED
   if (malloc_fn != NULL)
   {
      _png_struct dummy_struct;
      _png_structp _png_ptr = &dummy_struct;
      _png_ptr->mem_ptr=mem_ptr;
      struct_ptr = (*(malloc_fn))(_png_ptr, size);

      if (struct_ptr != NULL)
         _png_memset(struct_ptr, 0, size);

      return (struct_ptr);
   }
#  endif /* PNG_USER_MEM_SUPPORTED */

#  if defined(__TURBOC__) && !defined(__FLAT__)
   struct_ptr = (_png_voidp)farmalloc(size);
#  else
#    if defined(_MSC_VER) && defined(MAXSEG_64K)
   struct_ptr = (_png_voidp)halloc(size, 1);
#    else
   struct_ptr = (_png_voidp)malloc(size);
#    endif
#  endif

   if (struct_ptr != NULL)
      _png_memset(struct_ptr, 0, size);

   return (struct_ptr);
}


/* Free memory allocated by a _png_create_struct() call */
void /* PRIVATE */
_png_destroy_struct(_png_voidp struct_ptr)
{
#  ifdef PNG_USER_MEM_SUPPORTED
   _png_destroy_struct_2(struct_ptr, NULL, NULL);
}

/* Free memory allocated by a _png_create_struct() call */
void /* PRIVATE */
_png_destroy_struct_2(_png_voidp struct_ptr, _png_free_ptr free_fn,
    _png_voidp mem_ptr)
{
#  endif /* PNG_USER_MEM_SUPPORTED */
   if (struct_ptr != NULL)
   {
#  ifdef PNG_USER_MEM_SUPPORTED
      if (free_fn != NULL)
      {
         _png_struct dummy_struct;
         _png_structp _png_ptr = &dummy_struct;
         _png_ptr->mem_ptr=mem_ptr;
         (*(free_fn))(_png_ptr, struct_ptr);
         return;
      }
#  endif /* PNG_USER_MEM_SUPPORTED */
#  if defined(__TURBOC__) && !defined(__FLAT__)
      farfree(struct_ptr);

#  else
#    if defined(_MSC_VER) && defined(MAXSEG_64K)
      hfree(struct_ptr);

#    else
      free(struct_ptr);

#    endif
#  endif
   }
}

/* Allocate memory.  For reasonable files, size should never exceed
 * 64K.  However, zlib may allocate more then 64K if you don't tell
 * it not to.  See zconf.h and png.h for more information.  zlib does
 * need to allocate exactly 64K, so whatever you call here must
 * have the ability to do that.
 */

PNG_FUNCTION(_png_voidp,PNGAPI
_png_calloc,(_png_structp _png_ptr, _png_alloc_size_t size),PNG_ALLOCATED)
{
   _png_voidp ret;

   ret = (_png_malloc(_png_ptr, size));

   if (ret != NULL)
      _png_memset(ret,0,(_png_size_t)size);

   return (ret);
}

PNG_FUNCTION(_png_voidp,PNGAPI
_png_malloc,(_png_structp _png_ptr, _png_alloc_size_t size),PNG_ALLOCATED)
{
   _png_voidp ret;

#  ifdef PNG_USER_MEM_SUPPORTED
   if (_png_ptr == NULL || size == 0)
      return (NULL);

   if (_png_ptr->malloc_fn != NULL)
      ret = ((_png_voidp)(*(_png_ptr->malloc_fn))(_png_ptr, (_png_size_t)size));

   else
      ret = (_png_malloc_default(_png_ptr, size));

   if (ret == NULL && (_png_ptr->flags&PNG_FLAG_MALLOC_NULL_MEM_OK) == 0)
       _png_error(_png_ptr, "Out of Memory");

   return (ret);
}

PNG_FUNCTION(_png_voidp,PNGAPI
_png_malloc_default,(_png_structp _png_ptr, _png_alloc_size_t size),PNG_ALLOCATED)
{
   _png_voidp ret;
#  endif /* PNG_USER_MEM_SUPPORTED */

   if (_png_ptr == NULL || size == 0)
      return (NULL);

#  ifdef PNG_MAX_MALLOC_64K
   if (size > (_png_uint_32)65536L)
   {
#    ifndef PNG_USER_MEM_SUPPORTED
      if ((_png_ptr->flags&PNG_FLAG_MALLOC_NULL_MEM_OK) == 0)
         _png_error(_png_ptr, "Cannot Allocate > 64K");

      else
#    endif
         return NULL;
   }
#  endif

   /* Check for overflow */
#  if defined(__TURBOC__) && !defined(__FLAT__)

   if (size != (unsigned long)size)
      ret = NULL;

   else
      ret = farmalloc(size);

#  else
#    if defined(_MSC_VER) && defined(MAXSEG_64K)
   if (size != (unsigned long)size)
      ret = NULL;

   else
      ret = halloc(size, 1);

#    else
   if (size != (size_t)size)
      ret = NULL;

   else
      ret = malloc((size_t)size);
#    endif
#  endif

#  ifndef PNG_USER_MEM_SUPPORTED
   if (ret == NULL && (_png_ptr->flags&PNG_FLAG_MALLOC_NULL_MEM_OK) == 0)
      _png_error(_png_ptr, "Out of Memory");
#  endif

   return (ret);
}

/* Free a pointer allocated by _png_malloc().  If ptr is NULL, return
 * without taking any action.
 */
void PNGAPI
_png_free(_png_structp _png_ptr, _png_voidp ptr)
{
   if (_png_ptr == NULL || ptr == NULL)
      return;

#  ifdef PNG_USER_MEM_SUPPORTED
   if (_png_ptr->free_fn != NULL)
   {
      (*(_png_ptr->free_fn))(_png_ptr, ptr);
      return;
   }

   else
      _png_free_default(_png_ptr, ptr);
}

void PNGAPI
_png_free_default(_png_structp _png_ptr, _png_voidp ptr)
{
   if (_png_ptr == NULL || ptr == NULL)
      return;

#  endif /* PNG_USER_MEM_SUPPORTED */

#  if defined(__TURBOC__) && !defined(__FLAT__)
   farfree(ptr);

#  else
#    if defined(_MSC_VER) && defined(MAXSEG_64K)
   hfree(ptr);

#    else
   free(ptr);

#    endif
#  endif
}
#endif /* Not Borland DOS special memory handler */

/* This function was added at libpng version 1.2.3.  The _png_malloc_warn()
 * function will set up _png_malloc() to issue a _png_warning and return NULL
 * instead of issuing a _png_error, if it fails to allocate the requested
 * memory.
 */
PNG_FUNCTION(_png_voidp,PNGAPI
_png_malloc_warn,(_png_structp _png_ptr, _png_alloc_size_t size),PNG_ALLOCATED)
{
   _png_voidp ptr;
   _png_uint_32 save_flags;
   if (_png_ptr == NULL)
      return (NULL);

   save_flags = _png_ptr->flags;
   _png_ptr->flags|=PNG_FLAG_MALLOC_NULL_MEM_OK;
   ptr = (_png_voidp)_png_malloc((_png_structp)_png_ptr, size);
   _png_ptr->flags=save_flags;
   return(ptr);
}


#ifdef PNG_USER_MEM_SUPPORTED
/* This function is called when the application wants to use another method
 * of allocating and freeing memory.
 */
void PNGAPI
_png_set_mem_fn(_png_structp _png_ptr, _png_voidp mem_ptr, _png_malloc_ptr
  malloc_fn, _png_free_ptr free_fn)
{
   if (_png_ptr != NULL)
   {
      _png_ptr->mem_ptr = mem_ptr;
      _png_ptr->malloc_fn = malloc_fn;
      _png_ptr->free_fn = free_fn;
   }
}

/* This function returns a pointer to the mem_ptr associated with the user
 * functions.  The application should free any memory associated with this
 * pointer before _png_write_destroy and _png_read_destroy are called.
 */
_png_voidp PNGAPI
_png_get_mem_ptr(_png_const_structp _png_ptr)
{
   if (_png_ptr == NULL)
      return (NULL);

   return ((_png_voidp)_png_ptr->mem_ptr);
}
#endif /* PNG_USER_MEM_SUPPORTED */
#endif /* PNG_READ_SUPPORTED || PNG_WRITE_SUPPORTED */
