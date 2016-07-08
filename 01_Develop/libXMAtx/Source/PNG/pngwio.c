
/* pngwio.c - functions for data output
 *
 * Last changed in libpng 1.5.0 [January 6, 2011]
 * Copyright (c) 1998-2011 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 *
 * This file provides a location for all output.  Users who need
 * special handling are expected to write functions that have the same
 * arguments as these and perform similar functions, but that possibly
 * use different output methods.  Note that you shouldn't change these
 * functions, but rather write replacement functions and then change
 * them at run time with _png_set_write_fn(...).
 */

#include "pngpriv.h"

#ifdef PNG_WRITE_SUPPORTED

/* Write the data to whatever output you are using.  The default routine
 * writes to a file pointer.  Note that this routine sometimes gets called
 * with very small lengths, so you should implement some kind of simple
 * buffering if you are using unbuffered writes.  This should never be asked
 * to write more than 64K on a 16 bit machine.
 */

void /* PRIVATE */
_png_write_data(_png_structp _png_ptr, _png_const_bytep data, _png_size_t length)
{
   /* NOTE: write_data_fn must not change the buffer! */
   if (_png_ptr->write_data_fn != NULL )
      (*(_png_ptr->write_data_fn))(_png_ptr, (_png_bytep)data, length);

   else
      _png_error(_png_ptr, "Call to NULL write function");
}

#ifdef PNG_STDIO_SUPPORTED
/* This is the function that does the actual writing of data.  If you are
 * not writing to a standard C stream, you should create a replacement
 * write_data function and use it at run time with _png_set_write_fn(), rather
 * than changing the library.
 */
#ifndef USE_FAR_KEYWORD
void PNGCBAPI
_png_default_write_data(_png_structp _png_ptr, _png_bytep data, _png_size_t length)
{
   _png_size_t check;

   if (_png_ptr == NULL)
      return;

   check = fwrite(data, 1, length, (_png_FILE_p)(_png_ptr->io_ptr));

   if (check != length)
      _png_error(_png_ptr, "Write Error");
}
#else
/* This is the model-independent version. Since the standard I/O library
 * can't handle far buffers in the medium and small models, we have to copy
 * the data.
 */

#define NEAR_BUF_SIZE 1024
#define MIN(a,b) (a <= b ? a : b)

void PNGCBAPI
_png_default_write_data(_png_structp _png_ptr, _png_bytep data, _png_size_t length)
{
   _png_uint_32 check;
   _png_byte *near_data;  /* Needs to be "_png_byte *" instead of "_png_bytep" */
   _png_FILE_p io_ptr;

   if (_png_ptr == NULL)
      return;

   /* Check if data really is near. If so, use usual code. */
   near_data = (_png_byte *)CVT_PTR_NOCHECK(data);
   io_ptr = (_png_FILE_p)CVT_PTR(_png_ptr->io_ptr);

   if ((_png_bytep)near_data == data)
   {
      check = fwrite(near_data, 1, length, io_ptr);
   }

   else
   {
      _png_byte buf[NEAR_BUF_SIZE];
      _png_size_t written, remaining, err;
      check = 0;
      remaining = length;

      do
      {
         written = MIN(NEAR_BUF_SIZE, remaining);
         _png_memcpy(buf, data, written); /* Copy far buffer to near buffer */
         err = fwrite(buf, 1, written, io_ptr);

         if (err != written)
            break;

         else
            check += err;

         data += written;
         remaining -= written;
      }
      while (remaining != 0);
   }

   if (check != length)
      _png_error(_png_ptr, "Write Error");
}

#endif
#endif

/* This function is called to output any data pending writing (normally
 * to disk).  After _png_flush is called, there should be no data pending
 * writing in any buffers.
 */
#ifdef PNG_WRITE_FLUSH_SUPPORTED
void /* PRIVATE */
_png_flush(_png_structp _png_ptr)
{
   if (_png_ptr->output_flush_fn != NULL)
      (*(_png_ptr->output_flush_fn))(_png_ptr);
}

#  ifdef PNG_STDIO_SUPPORTED
void PNGCBAPI
_png_default_flush(_png_structp _png_ptr)
{
   _png_FILE_p io_ptr;

   if (_png_ptr == NULL)
      return;

   io_ptr = (_png_FILE_p)CVT_PTR((_png_ptr->io_ptr));
   fflush(io_ptr);
}
#  endif
#endif

/* This function allows the application to supply new output functions for
 * libpng if standard C streams aren't being used.
 *
 * This function takes as its arguments:
 * _png_ptr       - pointer to a png output data structure
 * io_ptr        - pointer to user supplied structure containing info about
 *                 the output functions.  May be NULL.
 * write_data_fn - pointer to a new output function that takes as its
 *                 arguments a pointer to a _png_struct, a pointer to
 *                 data to be written, and a 32-bit unsigned int that is
 *                 the number of bytes to be written.  The new write
 *                 function should call _png_error(_png_ptr, "Error msg")
 *                 to exit and output any fatal error messages.  May be
 *                 NULL, in which case libpng's default function will
 *                 be used.
 * flush_data_fn - pointer to a new flush function that takes as its
 *                 arguments a pointer to a _png_struct.  After a call to
 *                 the flush function, there should be no data in any buffers
 *                 or pending transmission.  If the output method doesn't do
 *                 any buffering of output, a function prototype must still be
 *                 supplied although it doesn't have to do anything.  If
 *                 PNG_WRITE_FLUSH_SUPPORTED is not defined at libpng compile
 *                 time, output_flush_fn will be ignored, although it must be
 *                 supplied for compatibility.  May be NULL, in which case
 *                 libpng's default function will be used, if
 *                 PNG_WRITE_FLUSH_SUPPORTED is defined.  This is not
 *                 a good idea if io_ptr does not point to a standard
 *                 *FILE structure.
 */
void PNGAPI
_png_set_write_fn(_png_structp _png_ptr, _png_voidp io_ptr,
    _png_rw_ptr write_data_fn, _png_flush_ptr output_flush_fn)
{
   if (_png_ptr == NULL)
      return;

   _png_ptr->io_ptr = io_ptr;

#ifdef PNG_STDIO_SUPPORTED
   if (write_data_fn != NULL)
      _png_ptr->write_data_fn = write_data_fn;

   else
      _png_ptr->write_data_fn = _png_default_write_data;
#else
   _png_ptr->write_data_fn = write_data_fn;
#endif

#ifdef PNG_WRITE_FLUSH_SUPPORTED
#  ifdef PNG_STDIO_SUPPORTED

   if (output_flush_fn != NULL)
      _png_ptr->output_flush_fn = output_flush_fn;

   else
      _png_ptr->output_flush_fn = _png_default_flush;

#  else
   _png_ptr->output_flush_fn = output_flush_fn;
#  endif
#endif /* PNG_WRITE_FLUSH_SUPPORTED */

   /* It is an error to read while writing a png file */
   if (_png_ptr->read_data_fn != NULL)
   {
      _png_ptr->read_data_fn = NULL;

      _png_warning(_png_ptr,
          "Can't set both read_data_fn and write_data_fn in the"
          " same structure");
   }
}

#ifdef USE_FAR_KEYWORD
#  ifdef _MSC_VER
void *_png_far_to_near(_png_structp _png_ptr, _png_voidp ptr, int check)
{
   void *near_ptr;
   void FAR *far_ptr;
   FP_OFF(near_ptr) = FP_OFF(ptr);
   far_ptr = (void FAR *)near_ptr;

   if (check != 0)
      if (FP_SEG(ptr) != FP_SEG(far_ptr))
         _png_error(_png_ptr, "segment lost in conversion");

   return(near_ptr);
}
#  else
void *_png_far_to_near(_png_structp _png_ptr, _png_voidp ptr, int check)
{
   void *near_ptr;
   void FAR *far_ptr;
   near_ptr = (void FAR *)ptr;
   far_ptr = (void FAR *)near_ptr;

   if (check != 0)
      if (far_ptr != ptr)
         _png_error(_png_ptr, "segment lost in conversion");

   return(near_ptr);
}
#  endif
#endif
#endif /* PNG_WRITE_SUPPORTED */
