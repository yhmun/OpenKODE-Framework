
/* pngread.c - read a PNG file
 *
 * Last changed in libpng 1.5.14 [January 24, 2013]
 * Copyright (c) 1998-2012 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 *
 * This file contains routines that an application calls directly to
 * read a PNG file or stream.
 */

#include "pngpriv.h"

#ifdef PNG_READ_SUPPORTED

/* Create a PNG structure for reading, and allocate any memory needed. */
PNG_FUNCTION(_png_structp,PNGAPI
_png_create_read_struct,(_png_const_charp user__png_ver, _png_voidp error_ptr,
    _png_error_ptr error_fn, _png_error_ptr warn_fn),PNG_ALLOCATED)
{

#ifdef PNG_USER_MEM_SUPPORTED
   return (_png_create_read_struct_2(user__png_ver, error_ptr, error_fn,
       warn_fn, NULL, NULL, NULL));
}

/* Alternate create PNG structure for reading, and allocate any memory
 * needed.
 */
PNG_FUNCTION(_png_structp,PNGAPI
_png_create_read_struct_2,(_png_const_charp user__png_ver, _png_voidp error_ptr,
    _png_error_ptr error_fn, _png_error_ptr warn_fn, _png_voidp mem_ptr,
    _png_malloc_ptr malloc_fn, _png_free_ptr free_fn),PNG_ALLOCATED)
{
#endif /* PNG_USER_MEM_SUPPORTED */

#ifdef PNG_SETJMP_SUPPORTED
   volatile
#endif
   _png_structp _png_ptr;
   volatile int _png_cleanup_needed = 0;

#ifdef PNG_SETJMP_SUPPORTED
#ifdef USE_FAR_KEYWORD
   jmp_buf tmp_jmpbuf;
#endif
#endif

   _png_debug(1, "in _png_create_read_struct");

#ifdef PNG_USER_MEM_SUPPORTED
   _png_ptr = (_png_structp)_png_create_struct_2(PNG_STRUCT_PNG,
       malloc_fn, mem_ptr);
#else
   _png_ptr = (_png_structp)_png_create_struct(PNG_STRUCT_PNG);
#endif
   if (_png_ptr == NULL)
      return (NULL);

   /* Added at libpng-1.2.6 */
#ifdef PNG_USER_LIMITS_SUPPORTED
   _png_ptr->user_width_max = PNG_USER_WIDTH_MAX;
   _png_ptr->user_height_max = PNG_USER_HEIGHT_MAX;

   /* Added at libpng-1.2.43 and 1.4.0 */
   _png_ptr->user_chunk_cache_max = PNG_USER_CHUNK_CACHE_MAX;

   /* Added at libpng-1.2.43 and 1.4.1 */
   _png_ptr->user_chunk_malloc_max = PNG_USER_CHUNK_MALLOC_MAX;
#endif

#ifdef PNG_SETJMP_SUPPORTED
/* Applications that neglect to set up their own setjmp() and then
 * encounter a _png_error() will longjmp here.  Since the jmpbuf is
 * then meaningless we abort instead of returning.
 */
#ifdef USE_FAR_KEYWORD
   if (setjmp(tmp_jmpbuf))
#else
   if (setjmp(_png_jmpbuf(_png_ptr))) /* Sets longjmp to match setjmp */
#endif
      PNG_ABORT();
#ifdef USE_FAR_KEYWORD
   _png_memcpy(_png_jmpbuf(_png_ptr), tmp_jmpbuf, _png_sizeof(jmp_buf));
#endif
#endif /* PNG_SETJMP_SUPPORTED */

#ifdef PNG_USER_MEM_SUPPORTED
   _png_set_mem_fn(_png_ptr, mem_ptr, malloc_fn, free_fn);
#endif

   _png_set_error_fn(_png_ptr, error_ptr, error_fn, warn_fn);

   /* Call the general version checker (shared with read and write code): */
   if (!_png_user_version_check(_png_ptr, user__png_ver))
      _png_cleanup_needed = 1;

   if (!_png_cleanup_needed)
   {
   /* Initialize zbuf - compression buffer */
   _png_ptr->zbuf_size = PNG_ZBUF_SIZE;
   _png_ptr->zbuf = (_png_bytep)_png_malloc_warn(_png_ptr, _png_ptr->zbuf_size);

   if (_png_ptr->zbuf == NULL)
      _png_cleanup_needed = 1;
   }

   _png_ptr->zstream.zalloc = _png_zalloc;
   _png_ptr->zstream.zfree = _png_zfree;
   _png_ptr->zstream.opaque = (voidpf)_png_ptr;

   if (!_png_cleanup_needed)
   {
      switch (inflateInit(&_png_ptr->zstream))
      {
         case Z_OK:
            break; /* Do nothing */

         case Z_MEM_ERROR:
            _png_warning(_png_ptr, "zlib memory error");
            _png_cleanup_needed = 1;
            break;

         case Z_STREAM_ERROR:
            _png_warning(_png_ptr, "zlib stream error");
            _png_cleanup_needed = 1;
            break;

         case Z_VERSION_ERROR:
            _png_warning(_png_ptr, "zlib version error");
            _png_cleanup_needed = 1;
            break;

         default: _png_warning(_png_ptr, "Unknown zlib error");
            _png_cleanup_needed = 1;
      }
   }

   if (_png_cleanup_needed)
   {
      /* Clean up PNG structure and deallocate any memory. */
      _png_free(_png_ptr, _png_ptr->zbuf);
      _png_ptr->zbuf = NULL;
#ifdef PNG_USER_MEM_SUPPORTED
      _png_destroy_struct_2((_png_voidp)_png_ptr,
          (_png_free_ptr)free_fn, (_png_voidp)mem_ptr);
#else
      _png_destroy_struct((_png_voidp)_png_ptr);
#endif
      return (NULL);
   }

   _png_ptr->zstream.next_out = _png_ptr->zbuf;
   _png_ptr->zstream.avail_out = (uInt)_png_ptr->zbuf_size;

   _png_set_read_fn(_png_ptr, NULL, NULL);


   return (_png_ptr);
}


#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
/* Read the information before the actual image data.  This has been
 * changed in v0.90 to allow reading a file that already has the magic
 * bytes read from the stream.  You can tell libpng how many bytes have
 * been read from the beginning of the stream (up to the maximum of 8)
 * via _png_set_sig_bytes(), and we will only check the remaining bytes
 * here.  The application can then have access to the signature bytes we
 * read if it is determined that this isn't a valid PNG file.
 */
void PNGAPI
_png_read_info(_png_structp _png_ptr, _png_infop info_ptr)
{
   _png_debug(1, "in _png_read_info");

   if (_png_ptr == NULL || info_ptr == NULL)
      return;

   /* Read and check the PNG file signature. */
   _png_read_sig(_png_ptr, info_ptr);

   for (;;)
   {
      _png_uint_32 length = _png_read_chunk_header(_png_ptr);
      _png_uint_32 chunk_name = _png_ptr->chunk_name;

      /* This should be a binary subdivision search or a hash for
       * matching the chunk name rather than a linear search.
       */
      if (chunk_name == _png_IDAT)
         if (_png_ptr->mode & PNG_AFTER_IDAT)
            _png_ptr->mode |= PNG_HAVE_CHUNK_AFTER_IDAT;

      if (chunk_name == _png_IHDR)
         _png_handle_IHDR(_png_ptr, info_ptr, length);

      else if (chunk_name == _png_IEND)
         _png_handle_IEND(_png_ptr, info_ptr, length);

#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
      else if (_png_chunk_unknown_handling(_png_ptr, chunk_name) !=
         PNG_HANDLE_CHUNK_AS_DEFAULT)
      {
         if (chunk_name == _png_IDAT)
            _png_ptr->mode |= PNG_HAVE_IDAT;

         _png_handle_unknown(_png_ptr, info_ptr, length);

         if (chunk_name == _png_PLTE)
            _png_ptr->mode |= PNG_HAVE_PLTE;

         else if (chunk_name == _png_IDAT)
         {
            if (!(_png_ptr->mode & PNG_HAVE_IHDR))
               _png_error(_png_ptr, "Missing IHDR before IDAT");

            else if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE &&
                !(_png_ptr->mode & PNG_HAVE_PLTE))
               _png_error(_png_ptr, "Missing PLTE before IDAT");

            break;
         }
      }
#endif
      else if (chunk_name == _png_PLTE)
         _png_handle_PLTE(_png_ptr, info_ptr, length);

      else if (chunk_name == _png_IDAT)
      {
         if (!(_png_ptr->mode & PNG_HAVE_IHDR))
            _png_error(_png_ptr, "Missing IHDR before IDAT");

         else if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE &&
             !(_png_ptr->mode & PNG_HAVE_PLTE))
            _png_error(_png_ptr, "Missing PLTE before IDAT");

         _png_ptr->idat_size = length;
         _png_ptr->mode |= PNG_HAVE_IDAT;
         break;
      }

#ifdef PNG_READ_bKGD_SUPPORTED
      else if (chunk_name == _png_bKGD)
         _png_handle_bKGD(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_cHRM_SUPPORTED
      else if (chunk_name == _png_cHRM)
         _png_handle_cHRM(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_gAMA_SUPPORTED
      else if (chunk_name == _png_gAMA)
         _png_handle_gAMA(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_hIST_SUPPORTED
      else if (chunk_name == _png_hIST)
         _png_handle_hIST(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_oFFs_SUPPORTED
      else if (chunk_name == _png_oFFs)
         _png_handle_oFFs(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_pCAL_SUPPORTED
      else if (chunk_name == _png_pCAL)
         _png_handle_pCAL(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_sCAL_SUPPORTED
      else if (chunk_name == _png_sCAL)
         _png_handle_sCAL(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_pHYs_SUPPORTED
      else if (chunk_name == _png_pHYs)
         _png_handle_pHYs(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_sBIT_SUPPORTED
      else if (chunk_name == _png_sBIT)
         _png_handle_sBIT(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_sRGB_SUPPORTED
      else if (chunk_name == _png_sRGB)
         _png_handle_sRGB(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_iCCP_SUPPORTED
      else if (chunk_name == _png_iCCP)
         _png_handle_iCCP(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_sPLT_SUPPORTED
      else if (chunk_name == _png_sPLT)
         _png_handle_sPLT(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_tEXt_SUPPORTED
      else if (chunk_name == _png_tEXt)
         _png_handle_tEXt(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_tIME_SUPPORTED
      else if (chunk_name == _png_tIME)
         _png_handle_tIME(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_tRNS_SUPPORTED
      else if (chunk_name == _png_tRNS)
         _png_handle_tRNS(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_zTXt_SUPPORTED
      else if (chunk_name == _png_zTXt)
         _png_handle_zTXt(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_iTXt_SUPPORTED
      else if (chunk_name == _png_iTXt)
         _png_handle_iTXt(_png_ptr, info_ptr, length);
#endif

      else
         _png_handle_unknown(_png_ptr, info_ptr, length);
   }
}
#endif /* PNG_SEQUENTIAL_READ_SUPPORTED */

/* Optional call to update the users info_ptr structure */
void PNGAPI
_png_read_update_info(_png_structp _png_ptr, _png_infop info_ptr)
{
   _png_debug(1, "in _png_read_update_info");

   if (_png_ptr == NULL)
      return;

   _png_read_start_row(_png_ptr);

#ifdef PNG_READ_TRANSFORMS_SUPPORTED
   _png_read_transform_info(_png_ptr, info_ptr);
#else
   PNG_UNUSED(info_ptr)
#endif
}

#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
/* Initialize palette, background, etc, after transformations
 * are set, but before any reading takes place.  This allows
 * the user to obtain a gamma-corrected palette, for example.
 * If the user doesn't call this, we will do it ourselves.
 */
void PNGAPI
_png_start_read_image(_png_structp _png_ptr)
{
   _png_debug(1, "in _png_start_read_image");

   if (_png_ptr != NULL)
     _png_read_start_row(_png_ptr);
}
#endif /* PNG_SEQUENTIAL_READ_SUPPORTED */

#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
void PNGAPI
_png_read_row(_png_structp _png_ptr, _png_bytep row, _png_bytep dsp_row)
{
   int ret;

   _png_row_info row_info;

   if (_png_ptr == NULL)
      return;

   _png_debug2(1, "in _png_read_row (row %lu, pass %d)",
       (unsigned long)_png_ptr->row_number, _png_ptr->pass);

   /* _png_read_start_row sets the information (in particular iwidth) for this
    * interlace pass.
    */
   if (!(_png_ptr->flags & PNG_FLAG_ROW_INIT))
      _png_read_start_row(_png_ptr);

   /* 1.5.6: row_info moved out of _png_struct to a local here. */
   row_info.width = _png_ptr->iwidth; /* NOTE: width of current interlaced row */
   row_info.color_type = _png_ptr->color_type;
   row_info.bit_depth = _png_ptr->bit_depth;
   row_info.channels = _png_ptr->channels;
   row_info.pixel_depth = _png_ptr->pixel_depth;
   row_info.rowbytes = PNG_ROWBYTES(row_info.pixel_depth, row_info.width);

   if (_png_ptr->row_number == 0 && _png_ptr->pass == 0)
   {
   /* Check for transforms that have been set but were defined out */
#if defined(PNG_WRITE_INVERT_SUPPORTED) && !defined(PNG_READ_INVERT_SUPPORTED)
   if (_png_ptr->transformations & PNG_INVERT_MONO)
      _png_warning(_png_ptr, "PNG_READ_INVERT_SUPPORTED is not defined");
#endif

#if defined(PNG_WRITE_FILLER_SUPPORTED) && !defined(PNG_READ_FILLER_SUPPORTED)
   if (_png_ptr->transformations & PNG_FILLER)
      _png_warning(_png_ptr, "PNG_READ_FILLER_SUPPORTED is not defined");
#endif

#if defined(PNG_WRITE_PACKSWAP_SUPPORTED) && \
    !defined(PNG_READ_PACKSWAP_SUPPORTED)
   if (_png_ptr->transformations & PNG_PACKSWAP)
      _png_warning(_png_ptr, "PNG_READ_PACKSWAP_SUPPORTED is not defined");
#endif

#if defined(PNG_WRITE_PACK_SUPPORTED) && !defined(PNG_READ_PACK_SUPPORTED)
   if (_png_ptr->transformations & PNG_PACK)
      _png_warning(_png_ptr, "PNG_READ_PACK_SUPPORTED is not defined");
#endif

#if defined(PNG_WRITE_SHIFT_SUPPORTED) && !defined(PNG_READ_SHIFT_SUPPORTED)
   if (_png_ptr->transformations & PNG_SHIFT)
      _png_warning(_png_ptr, "PNG_READ_SHIFT_SUPPORTED is not defined");
#endif

#if defined(PNG_WRITE_BGR_SUPPORTED) && !defined(PNG_READ_BGR_SUPPORTED)
   if (_png_ptr->transformations & PNG_BGR)
      _png_warning(_png_ptr, "PNG_READ_BGR_SUPPORTED is not defined");
#endif

#if defined(PNG_WRITE_SWAP_SUPPORTED) && !defined(PNG_READ_SWAP_SUPPORTED)
   if (_png_ptr->transformations & PNG_SWAP_BYTES)
      _png_warning(_png_ptr, "PNG_READ_SWAP_SUPPORTED is not defined");
#endif
   }

#ifdef PNG_READ_INTERLACING_SUPPORTED
   /* If interlaced and we do not need a new row, combine row and return.
    * Notice that the pixels we have from previous rows have been transformed
    * already; we can only combine like with like (transformed or
    * untransformed) and, because of the libpng API for interlaced images, this
    * means we must transform before de-interlacing.
    */
   if (_png_ptr->interlaced && (_png_ptr->transformations & PNG_INTERLACE))
   {
      switch (_png_ptr->pass)
      {
         case 0:
            if (_png_ptr->row_number & 0x07)
            {
               if (dsp_row != NULL)
                  _png_combine_row(_png_ptr, dsp_row, 1/*display*/);
               _png_read_finish_row(_png_ptr);
               return;
            }
            break;

         case 1:
            if ((_png_ptr->row_number & 0x07) || _png_ptr->width < 5)
            {
               if (dsp_row != NULL)
                  _png_combine_row(_png_ptr, dsp_row, 1/*display*/);

               _png_read_finish_row(_png_ptr);
               return;
            }
            break;

         case 2:
            if ((_png_ptr->row_number & 0x07) != 4)
            {
               if (dsp_row != NULL && (_png_ptr->row_number & 4))
                  _png_combine_row(_png_ptr, dsp_row, 1/*display*/);

               _png_read_finish_row(_png_ptr);
               return;
            }
            break;

         case 3:
            if ((_png_ptr->row_number & 3) || _png_ptr->width < 3)
            {
               if (dsp_row != NULL)
                  _png_combine_row(_png_ptr, dsp_row, 1/*display*/);

               _png_read_finish_row(_png_ptr);
               return;
            }
            break;

         case 4:
            if ((_png_ptr->row_number & 3) != 2)
            {
               if (dsp_row != NULL && (_png_ptr->row_number & 2))
                  _png_combine_row(_png_ptr, dsp_row, 1/*display*/);

               _png_read_finish_row(_png_ptr);
               return;
            }
            break;
         case 5:
            if ((_png_ptr->row_number & 1) || _png_ptr->width < 2)
            {
               if (dsp_row != NULL)
                  _png_combine_row(_png_ptr, dsp_row, 1/*display*/);

               _png_read_finish_row(_png_ptr);
               return;
            }
            break;

         default:
         case 6:
            if (!(_png_ptr->row_number & 1))
            {
               _png_read_finish_row(_png_ptr);
               return;
            }
            break;
      }
   }
#endif

   if (!(_png_ptr->mode & PNG_HAVE_IDAT))
      _png_error(_png_ptr, "Invalid attempt to read row data");

   _png_ptr->zstream.next_out = _png_ptr->row_buf;
   _png_ptr->zstream.avail_out =
       (uInt)(PNG_ROWBYTES(_png_ptr->pixel_depth,
       _png_ptr->iwidth) + 1);

   do
   {
      if (!(_png_ptr->zstream.avail_in))
      {
         while (!_png_ptr->idat_size)
         {
            _png_crc_finish(_png_ptr, 0);

            _png_ptr->idat_size = _png_read_chunk_header(_png_ptr);
            if (_png_ptr->chunk_name != _png_IDAT)
               _png_error(_png_ptr, "Not enough image data");
         }
         _png_ptr->zstream.avail_in = (uInt)_png_ptr->zbuf_size;
         _png_ptr->zstream.next_in = _png_ptr->zbuf;
         if (_png_ptr->zbuf_size > _png_ptr->idat_size)
            _png_ptr->zstream.avail_in = (uInt)_png_ptr->idat_size;
         _png_crc_read(_png_ptr, _png_ptr->zbuf,
             (_png_size_t)_png_ptr->zstream.avail_in);
         _png_ptr->idat_size -= _png_ptr->zstream.avail_in;
      }

      ret = inflate(&_png_ptr->zstream, Z_PARTIAL_FLUSH);

      if (ret == Z_STREAM_END)
      {
         if (_png_ptr->zstream.avail_out || _png_ptr->zstream.avail_in ||
            _png_ptr->idat_size)
            _png_benign_error(_png_ptr, "Extra compressed data");
         _png_ptr->mode |= PNG_AFTER_IDAT;
         _png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;
         break;
      }

      if (ret != Z_OK)
         _png_error(_png_ptr, _png_ptr->zstream.msg ? _png_ptr->zstream.msg :
             "Decompression error");

   } while (_png_ptr->zstream.avail_out);

   if (_png_ptr->row_buf[0] > PNG_FILTER_VALUE_NONE)
   {
      if (_png_ptr->row_buf[0] < PNG_FILTER_VALUE_LAST)
         _png_read_filter_row(_png_ptr, &row_info, _png_ptr->row_buf + 1,
            _png_ptr->prev_row + 1, _png_ptr->row_buf[0]);
      else
         _png_error(_png_ptr, "bad adaptive filter value");
   }

   /* libpng 1.5.6: the following line was copying _png_ptr->rowbytes before
    * 1.5.6, while the buffer really is this big in current versions of libpng
    * it may not be in the future, so this was changed just to copy the
    * interlaced count:
    */
   _png_memcpy(_png_ptr->prev_row, _png_ptr->row_buf, row_info.rowbytes + 1);

#ifdef PNG_MNG_FEATURES_SUPPORTED
   if ((_png_ptr->mng_features_permitted & PNG_FLAG_MNG_FILTER_64) &&
       (_png_ptr->filter_type == PNG_INTRAPIXEL_DIFFERENCING))
   {
      /* Intrapixel differencing */
      _png_do_read_intrapixel(&row_info, _png_ptr->row_buf + 1);
   }
#endif


#ifdef PNG_READ_TRANSFORMS_SUPPORTED
   if (_png_ptr->transformations)
      _png_do_read_transformations(_png_ptr, &row_info);
#endif

   /* The transformed pixel depth should match the depth now in row_info. */
   if (_png_ptr->transformed_pixel_depth == 0)
   {
      _png_ptr->transformed_pixel_depth = row_info.pixel_depth;
      if (row_info.pixel_depth > _png_ptr->maximum_pixel_depth)
         _png_error(_png_ptr, "sequential row overflow");
   }

   else if (_png_ptr->transformed_pixel_depth != row_info.pixel_depth)
      _png_error(_png_ptr, "internal sequential row size calculation error");

#ifdef PNG_READ_INTERLACING_SUPPORTED
   /* Blow up interlaced rows to full size */
   if (_png_ptr->interlaced &&
      (_png_ptr->transformations & PNG_INTERLACE))
   {
      if (_png_ptr->pass < 6)
         _png_do_read_interlace(&row_info, _png_ptr->row_buf + 1, _png_ptr->pass,
            _png_ptr->transformations);

      if (dsp_row != NULL)
         _png_combine_row(_png_ptr, dsp_row, 1/*display*/);

      if (row != NULL)
         _png_combine_row(_png_ptr, row, 0/*row*/);
   }

   else
#endif
   {
      if (row != NULL)
         _png_combine_row(_png_ptr, row, -1/*ignored*/);

      if (dsp_row != NULL)
         _png_combine_row(_png_ptr, dsp_row, -1/*ignored*/);
   }
   _png_read_finish_row(_png_ptr);

   if (_png_ptr->read_row_fn != NULL)
      (*(_png_ptr->read_row_fn))(_png_ptr, _png_ptr->row_number, _png_ptr->pass);
}
#endif /* PNG_SEQUENTIAL_READ_SUPPORTED */

#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
/* Read one or more rows of image data.  If the image is interlaced,
 * and _png_set_interlace_handling() has been called, the rows need to
 * contain the contents of the rows from the previous pass.  If the
 * image has alpha or transparency, and _png_handle_alpha()[*] has been
 * called, the rows contents must be initialized to the contents of the
 * screen.
 *
 * "row" holds the actual image, and pixels are placed in it
 * as they arrive.  If the image is displayed after each pass, it will
 * appear to "sparkle" in.  "display_row" can be used to display a
 * "chunky" progressive image, with finer detail added as it becomes
 * available.  If you do not want this "chunky" display, you may pass
 * NULL for display_row.  If you do not want the sparkle display, and
 * you have not called _png_handle_alpha(), you may pass NULL for rows.
 * If you have called _png_handle_alpha(), and the image has either an
 * alpha channel or a transparency chunk, you must provide a buffer for
 * rows.  In this case, you do not have to provide a display_row buffer
 * also, but you may.  If the image is not interlaced, or if you have
 * not called _png_set_interlace_handling(), the display_row buffer will
 * be ignored, so pass NULL to it.
 *
 * [*] _png_handle_alpha() does not exist yet, as of this version of libpng
 */

void PNGAPI
_png_read_rows(_png_structp _png_ptr, _png_bytepp row,
    _png_bytepp display_row, _png_uint_32 num_rows)
{
   _png_uint_32 i;
   _png_bytepp rp;
   _png_bytepp dp;

   _png_debug(1, "in _png_read_rows");

   if (_png_ptr == NULL)
      return;

   rp = row;
   dp = display_row;
   if (rp != NULL && dp != NULL)
      for (i = 0; i < num_rows; i++)
      {
         _png_bytep rptr = *rp++;
         _png_bytep dptr = *dp++;

         _png_read_row(_png_ptr, rptr, dptr);
      }

   else if (rp != NULL)
      for (i = 0; i < num_rows; i++)
      {
         _png_bytep rptr = *rp;
         _png_read_row(_png_ptr, rptr, NULL);
         rp++;
      }

   else if (dp != NULL)
      for (i = 0; i < num_rows; i++)
      {
         _png_bytep dptr = *dp;
         _png_read_row(_png_ptr, NULL, dptr);
         dp++;
      }
}
#endif /* PNG_SEQUENTIAL_READ_SUPPORTED */

#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
/* Read the entire image.  If the image has an alpha channel or a tRNS
 * chunk, and you have called _png_handle_alpha()[*], you will need to
 * initialize the image to the current image that PNG will be overlaying.
 * We set the num_rows again here, in case it was incorrectly set in
 * _png_read_start_row() by a call to _png_read_update_info() or
 * _png_start_read_image() if _png_set_interlace_handling() wasn't called
 * prior to either of these functions like it should have been.  You can
 * only call this function once.  If you desire to have an image for
 * each pass of a interlaced image, use _png_read_rows() instead.
 *
 * [*] _png_handle_alpha() does not exist yet, as of this version of libpng
 */
void PNGAPI
_png_read_image(_png_structp _png_ptr, _png_bytepp image)
{
   _png_uint_32 i, image_height;
   int pass, j;
   _png_bytepp rp;

   _png_debug(1, "in _png_read_image");

   if (_png_ptr == NULL)
      return;

#ifdef PNG_READ_INTERLACING_SUPPORTED
   if (!(_png_ptr->flags & PNG_FLAG_ROW_INIT))
   {
      pass = _png_set_interlace_handling(_png_ptr);
      /* And make sure transforms are initialized. */
      _png_start_read_image(_png_ptr);
   }
   else
   {
      if (_png_ptr->interlaced && !(_png_ptr->transformations & PNG_INTERLACE))
      {
         /* Caller called _png_start_read_image or _png_read_update_info without
          * first turning on the PNG_INTERLACE transform.  We can fix this here,
          * but the caller should do it!
          */
         _png_warning(_png_ptr, "Interlace handling should be turned on when "
            "using _png_read_image");
         /* Make sure this is set correctly */
         _png_ptr->num_rows = _png_ptr->height;
      }

      /* Obtain the pass number, which also turns on the PNG_INTERLACE flag in
       * the above error case.
       */
      pass = _png_set_interlace_handling(_png_ptr);
   }
#else
   if (_png_ptr->interlaced)
      _png_error(_png_ptr,
          "Cannot read interlaced image -- interlace handler disabled");

   pass = 1;
#endif

   image_height=_png_ptr->height;

   for (j = 0; j < pass; j++)
   {
      rp = image;
      for (i = 0; i < image_height; i++)
      {
         _png_read_row(_png_ptr, *rp, NULL);
         rp++;
      }
   }
}
#endif /* PNG_SEQUENTIAL_READ_SUPPORTED */

#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
/* Read the end of the PNG file.  Will not read past the end of the
 * file, will verify the end is accurate, and will read any comments
 * or time information at the end of the file, if info is not NULL.
 */
void PNGAPI
_png_read_end(_png_structp _png_ptr, _png_infop info_ptr)
{
   _png_debug(1, "in _png_read_end");

   if (_png_ptr == NULL)
      return;

   _png_crc_finish(_png_ptr, 0); /* Finish off CRC from last IDAT chunk */

#ifdef PNG_READ_CHECK_FOR_INVALID_INDEX_SUPPORTED
   /* Report invalid palette index; added at libng-1.5.10 */
   if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE &&
      _png_ptr->num_palette_max > _png_ptr->num_palette)
     _png_benign_error(_png_ptr, "Read palette index exceeding num_palette");
#endif

   do
   {
      _png_uint_32 length = _png_read_chunk_header(_png_ptr);
      _png_uint_32 chunk_name = _png_ptr->chunk_name;

      if (chunk_name == _png_IHDR)
         _png_handle_IHDR(_png_ptr, info_ptr, length);

      else if (chunk_name == _png_IEND)
         _png_handle_IEND(_png_ptr, info_ptr, length);

#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
      else if (_png_chunk_unknown_handling(_png_ptr, chunk_name) !=
         PNG_HANDLE_CHUNK_AS_DEFAULT)
      {
         if (chunk_name == _png_IDAT)
         {
            if ((length > 0) || (_png_ptr->mode & PNG_HAVE_CHUNK_AFTER_IDAT))
               _png_benign_error(_png_ptr, "Too many IDATs found");
         }
         _png_handle_unknown(_png_ptr, info_ptr, length);
         if (chunk_name == _png_PLTE)
            _png_ptr->mode |= PNG_HAVE_PLTE;
      }
#endif

      else if (chunk_name == _png_IDAT)
      {
         /* Zero length IDATs are legal after the last IDAT has been
          * read, but not after other chunks have been read.
          */
         if ((length > 0) || (_png_ptr->mode & PNG_HAVE_CHUNK_AFTER_IDAT))
            _png_benign_error(_png_ptr, "Too many IDATs found");

         _png_crc_finish(_png_ptr, length);
      }
      else if (chunk_name == _png_PLTE)
         _png_handle_PLTE(_png_ptr, info_ptr, length);

#ifdef PNG_READ_bKGD_SUPPORTED
      else if (chunk_name == _png_bKGD)
         _png_handle_bKGD(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_cHRM_SUPPORTED
      else if (chunk_name == _png_cHRM)
         _png_handle_cHRM(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_gAMA_SUPPORTED
      else if (chunk_name == _png_gAMA)
         _png_handle_gAMA(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_hIST_SUPPORTED
      else if (chunk_name == _png_hIST)
         _png_handle_hIST(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_oFFs_SUPPORTED
      else if (chunk_name == _png_oFFs)
         _png_handle_oFFs(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_pCAL_SUPPORTED
      else if (chunk_name == _png_pCAL)
         _png_handle_pCAL(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_sCAL_SUPPORTED
      else if (chunk_name == _png_sCAL)
         _png_handle_sCAL(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_pHYs_SUPPORTED
      else if (chunk_name == _png_pHYs)
         _png_handle_pHYs(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_sBIT_SUPPORTED
      else if (chunk_name == _png_sBIT)
         _png_handle_sBIT(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_sRGB_SUPPORTED
      else if (chunk_name == _png_sRGB)
         _png_handle_sRGB(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_iCCP_SUPPORTED
      else if (chunk_name == _png_iCCP)
         _png_handle_iCCP(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_sPLT_SUPPORTED
      else if (chunk_name == _png_sPLT)
         _png_handle_sPLT(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_tEXt_SUPPORTED
      else if (chunk_name == _png_tEXt)
         _png_handle_tEXt(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_tIME_SUPPORTED
      else if (chunk_name == _png_tIME)
         _png_handle_tIME(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_tRNS_SUPPORTED
      else if (chunk_name == _png_tRNS)
         _png_handle_tRNS(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_zTXt_SUPPORTED
      else if (chunk_name == _png_zTXt)
         _png_handle_zTXt(_png_ptr, info_ptr, length);
#endif

#ifdef PNG_READ_iTXt_SUPPORTED
      else if (chunk_name == _png_iTXt)
         _png_handle_iTXt(_png_ptr, info_ptr, length);
#endif

      else
         _png_handle_unknown(_png_ptr, info_ptr, length);
   } while (!(_png_ptr->mode & PNG_HAVE_IEND));
}
#endif /* PNG_SEQUENTIAL_READ_SUPPORTED */

/* Free all memory used by the read */
void PNGAPI
_png_destroy_read_struct(_png_structpp _png_ptr_ptr, _png_infopp info_ptr_ptr,
    _png_infopp end_info_ptr_ptr)
{
   _png_structp _png_ptr = NULL;
   _png_infop info_ptr = NULL, end_info_ptr = NULL;
#ifdef PNG_USER_MEM_SUPPORTED
   _png_free_ptr free_fn = NULL;
   _png_voidp mem_ptr = NULL;
#endif

   _png_debug(1, "in _png_destroy_read_struct");

   if (_png_ptr_ptr != NULL)
      _png_ptr = *_png_ptr_ptr;
   if (_png_ptr == NULL)
      return;

#ifdef PNG_USER_MEM_SUPPORTED
   free_fn = _png_ptr->free_fn;
   mem_ptr = _png_ptr->mem_ptr;
#endif

   if (info_ptr_ptr != NULL)
      info_ptr = *info_ptr_ptr;

   if (end_info_ptr_ptr != NULL)
      end_info_ptr = *end_info_ptr_ptr;

   _png_read_destroy(_png_ptr, info_ptr, end_info_ptr);

   if (info_ptr != NULL)
   {
#ifdef PNG_TEXT_SUPPORTED
      _png_free_data(_png_ptr, info_ptr, PNG_FREE_TEXT, -1);
#endif

#ifdef PNG_USER_MEM_SUPPORTED
      _png_destroy_struct_2((_png_voidp)info_ptr, (_png_free_ptr)free_fn,
          (_png_voidp)mem_ptr);
#else
      _png_destroy_struct((_png_voidp)info_ptr);
#endif
      *info_ptr_ptr = NULL;
   }

   if (end_info_ptr != NULL)
   {
#ifdef PNG_READ_TEXT_SUPPORTED
      _png_free_data(_png_ptr, end_info_ptr, PNG_FREE_TEXT, -1);
#endif
#ifdef PNG_USER_MEM_SUPPORTED
      _png_destroy_struct_2((_png_voidp)end_info_ptr, (_png_free_ptr)free_fn,
          (_png_voidp)mem_ptr);
#else
      _png_destroy_struct((_png_voidp)end_info_ptr);
#endif
      *end_info_ptr_ptr = NULL;
   }

   if (_png_ptr != NULL)
   {
#ifdef PNG_USER_MEM_SUPPORTED
      _png_destroy_struct_2((_png_voidp)_png_ptr, (_png_free_ptr)free_fn,
          (_png_voidp)mem_ptr);
#else
      _png_destroy_struct((_png_voidp)_png_ptr);
#endif
      *_png_ptr_ptr = NULL;
   }
}

/* Free all memory used by the read (old method) */
void /* PRIVATE */
_png_read_destroy(_png_structp _png_ptr, _png_infop info_ptr,
    _png_infop end_info_ptr)
{
#ifdef PNG_SETJMP_SUPPORTED
   jmp_buf tmp_jmp;
#endif
   _png_error_ptr error_fn;
#ifdef PNG_WARNINGS_SUPPORTED
   _png_error_ptr warning_fn;
#endif
   _png_voidp error_ptr;
#ifdef PNG_USER_MEM_SUPPORTED
   _png_free_ptr free_fn;
#endif

   _png_debug(1, "in _png_read_destroy");

   if (info_ptr != NULL)
      _png_info_destroy(_png_ptr, info_ptr);

   if (end_info_ptr != NULL)
      _png_info_destroy(_png_ptr, end_info_ptr);

#ifdef PNG_READ_GAMMA_SUPPORTED
   _png_destroy_gamma_table(_png_ptr);
#endif

   _png_free(_png_ptr, _png_ptr->zbuf);
   _png_free(_png_ptr, _png_ptr->big_row_buf);
   _png_free(_png_ptr, _png_ptr->big_prev_row);
   _png_free(_png_ptr, _png_ptr->chunkdata);

#ifdef PNG_READ_QUANTIZE_SUPPORTED
   _png_free(_png_ptr, _png_ptr->palette_lookup);
   _png_free(_png_ptr, _png_ptr->quantize_index);
#endif

   if (_png_ptr->free_me & PNG_FREE_PLTE)
      _png_zfree(_png_ptr, _png_ptr->palette);
   _png_ptr->free_me &= ~PNG_FREE_PLTE;

#if defined(PNG_tRNS_SUPPORTED) || \
    defined(PNG_READ_EXPAND_SUPPORTED) || defined(PNG_READ_BACKGROUND_SUPPORTED)
   if (_png_ptr->free_me & PNG_FREE_TRNS)
      _png_free(_png_ptr, _png_ptr->trans_alpha);
   _png_ptr->free_me &= ~PNG_FREE_TRNS;
#endif

#ifdef PNG_READ_hIST_SUPPORTED
   if (_png_ptr->free_me & PNG_FREE_HIST)
      _png_free(_png_ptr, _png_ptr->hist);
   _png_ptr->free_me &= ~PNG_FREE_HIST;
#endif

   inflateEnd(&_png_ptr->zstream);

#ifdef PNG_PROGRESSIVE_READ_SUPPORTED
   _png_free(_png_ptr, _png_ptr->save_buffer);
#endif

   /* Save the important info out of the _png_struct, in case it is
    * being used again.
    */
#ifdef PNG_SETJMP_SUPPORTED
   _png_memcpy(tmp_jmp, _png_ptr->longjmp_buffer, _png_sizeof(jmp_buf));
#endif

   error_fn = _png_ptr->error_fn;
#ifdef PNG_WARNINGS_SUPPORTED
   warning_fn = _png_ptr->warning_fn;
#endif
   error_ptr = _png_ptr->error_ptr;
#ifdef PNG_USER_MEM_SUPPORTED
   free_fn = _png_ptr->free_fn;
#endif

   _png_memset(_png_ptr, 0, _png_sizeof(_png_struct));

   _png_ptr->error_fn = error_fn;
#ifdef PNG_WARNINGS_SUPPORTED
   _png_ptr->warning_fn = warning_fn;
#endif
   _png_ptr->error_ptr = error_ptr;
#ifdef PNG_USER_MEM_SUPPORTED
   _png_ptr->free_fn = free_fn;
#endif

#ifdef PNG_SETJMP_SUPPORTED
   _png_memcpy(_png_ptr->longjmp_buffer, tmp_jmp, _png_sizeof(jmp_buf));
#endif

}

void PNGAPI
_png_set_read_status_fn(_png_structp _png_ptr, _png_read_status_ptr read_row_fn)
{
   if (_png_ptr == NULL)
      return;

   _png_ptr->read_row_fn = read_row_fn;
}


#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
#ifdef PNG_INFO_IMAGE_SUPPORTED
void PNGAPI
_png_read_png(_png_structp _png_ptr, _png_infop info_ptr,
                           int transforms,
                           voidp params)
{
   int row;

   if (_png_ptr == NULL || info_ptr == NULL)
      return;

   /* _png_read_info() gives us all of the information from the
    * PNG file before the first IDAT (image data chunk).
    */
   _png_read_info(_png_ptr, info_ptr);
   if (info_ptr->height > PNG_UINT_32_MAX/_png_sizeof(_png_bytep))
      _png_error(_png_ptr, "Image is too high to process with _png_read_png()");

   /* -------------- image transformations start here ------------------- */

#ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
   /* Tell libpng to strip 16-bit/color files down to 8 bits per color.
    */
   if (transforms & PNG_TRANSFORM_SCALE_16)
   {
     /* Added at libpng-1.5.4. "strip_16" produces the same result that it
      * did in earlier versions, while "scale_16" is now more accurate.
      */
      _png_set_scale_16(_png_ptr);
   }
#endif

#ifdef PNG_READ_STRIP_16_TO_8_SUPPORTED
   /* If both SCALE and STRIP are required pngrtran will effectively cancel the
    * latter by doing SCALE first.  This is ok and allows apps not to check for
    * which is supported to get the right answer.
    */
   if (transforms & PNG_TRANSFORM_STRIP_16)
      _png_set_strip_16(_png_ptr);
#endif

#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
   /* Strip alpha bytes from the input data without combining with
    * the background (not recommended).
    */
   if (transforms & PNG_TRANSFORM_STRIP_ALPHA)
      _png_set_strip_alpha(_png_ptr);
#endif

#if defined(PNG_READ_PACK_SUPPORTED) && !defined(PNG_READ_EXPAND_SUPPORTED)
   /* Extract multiple pixels with bit depths of 1, 2, or 4 from a single
    * byte into separate bytes (useful for paletted and grayscale images).
    */
   if (transforms & PNG_TRANSFORM_PACKING)
      _png_set_packing(_png_ptr);
#endif

#ifdef PNG_READ_PACKSWAP_SUPPORTED
   /* Change the order of packed pixels to least significant bit first
    * (not useful if you are using _png_set_packing).
    */
   if (transforms & PNG_TRANSFORM_PACKSWAP)
      _png_set_packswap(_png_ptr);
#endif

#ifdef PNG_READ_EXPAND_SUPPORTED
   /* Expand paletted colors into true RGB triplets
    * Expand grayscale images to full 8 bits from 1, 2, or 4 bits/pixel
    * Expand paletted or RGB images with transparency to full alpha
    * channels so the data will be available as RGBA quartets.
    */
   if (transforms & PNG_TRANSFORM_EXPAND)
      if ((_png_ptr->bit_depth < 8) ||
          (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE) ||
          (_png_get_valid(_png_ptr, info_ptr, PNG_INFO_tRNS)))
         _png_set_expand(_png_ptr);
#endif

   /* We don't handle background color or gamma transformation or quantizing.
    */

#ifdef PNG_READ_INVERT_SUPPORTED
   /* Invert monochrome files to have 0 as white and 1 as black
    */
   if (transforms & PNG_TRANSFORM_INVERT_MONO)
      _png_set_invert_mono(_png_ptr);
#endif

#ifdef PNG_READ_SHIFT_SUPPORTED
   /* If you want to shift the pixel values from the range [0,255] or
    * [0,65535] to the original [0,7] or [0,31], or whatever range the
    * colors were originally in:
    */
   if ((transforms & PNG_TRANSFORM_SHIFT)
       && _png_get_valid(_png_ptr, info_ptr, PNG_INFO_sBIT))
   {
      _png_color_8p sig_bit;

      _png_get_sBIT(_png_ptr, info_ptr, &sig_bit);
      _png_set_shift(_png_ptr, sig_bit);
   }
#endif

#ifdef PNG_READ_BGR_SUPPORTED
   /* Flip the RGB pixels to BGR (or RGBA to BGRA) */
   if (transforms & PNG_TRANSFORM_BGR)
      _png_set_bgr(_png_ptr);
#endif

#ifdef PNG_READ_SWAP_ALPHA_SUPPORTED
   /* Swap the RGBA or GA data to ARGB or AG (or BGRA to ABGR) */
   if (transforms & PNG_TRANSFORM_SWAP_ALPHA)
      _png_set_swap_alpha(_png_ptr);
#endif

#ifdef PNG_READ_SWAP_SUPPORTED
   /* Swap bytes of 16-bit files to least significant byte first */
   if (transforms & PNG_TRANSFORM_SWAP_ENDIAN)
      _png_set_swap(_png_ptr);
#endif

/* Added at libpng-1.2.41 */
#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
   /* Invert the alpha channel from opacity to transparency */
   if (transforms & PNG_TRANSFORM_INVERT_ALPHA)
      _png_set_invert_alpha(_png_ptr);
#endif

/* Added at libpng-1.2.41 */
#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
   /* Expand grayscale image to RGB */
   if (transforms & PNG_TRANSFORM_GRAY_TO_RGB)
      _png_set_gray_to_rgb(_png_ptr);
#endif

/* Added at libpng-1.5.4 */
#ifdef PNG_READ_EXPAND_16_SUPPORTED
   if (transforms & PNG_TRANSFORM_EXPAND_16)
      _png_set_expand_16(_png_ptr);
#endif

   /* We don't handle adding filler bytes */

   /* We use _png_read_image and rely on that for interlace handling, but we also
    * call _png_read_update_info therefore must turn on interlace handling now:
    */
   (void)_png_set_interlace_handling(_png_ptr);

   /* Optional call to gamma correct and add the background to the palette
    * and update info structure.  REQUIRED if you are expecting libpng to
    * update the palette for you (i.e., you selected such a transform above).
    */
   _png_read_update_info(_png_ptr, info_ptr);

   /* -------------- image transformations end here ------------------- */

   _png_free_data(_png_ptr, info_ptr, PNG_FREE_ROWS, 0);
   if (info_ptr->row_pointers == NULL)
   {
      _png_uint_32 iptr;

      info_ptr->row_pointers = (_png_bytepp)_png_malloc(_png_ptr,
          info_ptr->height * _png_sizeof(_png_bytep));
      for (iptr=0; iptr<info_ptr->height; iptr++)
         info_ptr->row_pointers[iptr] = NULL;

      info_ptr->free_me |= PNG_FREE_ROWS;

      for (row = 0; row < (int)info_ptr->height; row++)
         info_ptr->row_pointers[row] = (_png_bytep)_png_malloc(_png_ptr,
            _png_get_rowbytes(_png_ptr, info_ptr));
   }

   _png_read_image(_png_ptr, info_ptr->row_pointers);
   info_ptr->valid |= PNG_INFO_IDAT;

   /* Read rest of file, and get additional chunks in info_ptr - REQUIRED */
   _png_read_end(_png_ptr, info_ptr);

   PNG_UNUSED(transforms)   /* Quiet compiler warnings */
   PNG_UNUSED(params)

}
#endif /* PNG_INFO_IMAGE_SUPPORTED */
#endif /* PNG_SEQUENTIAL_READ_SUPPORTED */
#endif /* PNG_READ_SUPPORTED */
