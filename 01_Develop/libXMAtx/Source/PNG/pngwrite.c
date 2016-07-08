
/* pngwrite.c - general routines to write a PNG file
 *
 * Last changed in libpng 1.5.14 [January 24, 2013]
 * Copyright (c) 1998-2013 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 */

#include "pngpriv.h"

#ifdef PNG_WRITE_SUPPORTED

/* Writes all the PNG information.  This is the suggested way to use the
 * library.  If you have a new chunk to add, make a function to write it,
 * and put it in the correct location here.  If you want the chunk written
 * after the image data, put it in _png_write_end().  I strongly encourage
 * you to supply a PNG_INFO_ flag, and check info_ptr->valid before writing
 * the chunk, as that will keep the code from breaking if you want to just
 * write a plain PNG file.  If you have long comments, I suggest writing
 * them in _png_write_end(), and compressing them.
 */
void PNGAPI
_png_write_info_before_PLTE(_png_structp _png_ptr, _png_infop info_ptr)
{
   _png_debug(1, "in _png_write_info_before_PLTE");

   if (_png_ptr == NULL || info_ptr == NULL)
      return;

   if (!(_png_ptr->mode & PNG_WROTE_INFO_BEFORE_PLTE))
   {
   /* Write PNG signature */
   _png_write_sig(_png_ptr);

#ifdef PNG_MNG_FEATURES_SUPPORTED
   if ((_png_ptr->mode&PNG_HAVE_PNG_SIGNATURE) && \
       (_png_ptr->mng_features_permitted))
   {
      _png_warning(_png_ptr, "MNG features are not allowed in a PNG datastream");
      _png_ptr->mng_features_permitted = 0;
   }
#endif

   /* Write IHDR information. */
   _png_write_IHDR(_png_ptr, info_ptr->width, info_ptr->height,
       info_ptr->bit_depth, info_ptr->color_type, info_ptr->compression_type,
       info_ptr->filter_type,
#ifdef PNG_WRITE_INTERLACING_SUPPORTED
       info_ptr->interlace_type);
#else
       0);
#endif
   /* The rest of these check to see if the valid field has the appropriate
    * flag set, and if it does, writes the chunk.
    */
#ifdef PNG_WRITE_gAMA_SUPPORTED
   if (info_ptr->valid & PNG_INFO_gAMA)
      _png_write_gAMA_fixed(_png_ptr, info_ptr->gamma);
#endif
#ifdef PNG_WRITE_sRGB_SUPPORTED
   if (info_ptr->valid & PNG_INFO_sRGB)
      _png_write_sRGB(_png_ptr, (int)info_ptr->srgb_intent);
#endif

#ifdef PNG_WRITE_iCCP_SUPPORTED
   if (info_ptr->valid & PNG_INFO_iCCP)
      _png_write_iCCP(_png_ptr, info_ptr->iccp_name, PNG_COMPRESSION_TYPE_BASE,
          (_png_charp)info_ptr->iccp_profile, (int)info_ptr->iccp_proflen);
#endif
#ifdef PNG_WRITE_sBIT_SUPPORTED
   if (info_ptr->valid & PNG_INFO_sBIT)
      _png_write_sBIT(_png_ptr, &(info_ptr->sig_bit), info_ptr->color_type);
#endif
#ifdef PNG_WRITE_cHRM_SUPPORTED
   if (info_ptr->valid & PNG_INFO_cHRM)
      _png_write_cHRM_fixed(_png_ptr,
          info_ptr->x_white, info_ptr->y_white,
          info_ptr->x_red, info_ptr->y_red,
          info_ptr->x_green, info_ptr->y_green,
          info_ptr->x_blue, info_ptr->y_blue);
#endif

#ifdef PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED
   if (info_ptr->unknown_chunks_num)
   {
      _png_unknown_chunk *up;

      _png_debug(5, "writing extra chunks");

      for (up = info_ptr->unknown_chunks;
           up < info_ptr->unknown_chunks + info_ptr->unknown_chunks_num;
           up++)
      {
         int keep = _png_handle_as_unknown(_png_ptr, up->name);

         if (keep != PNG_HANDLE_CHUNK_NEVER &&
             up->location &&
             !(up->location & PNG_HAVE_PLTE) &&
             !(up->location & PNG_HAVE_IDAT) &&
             !(up->location & PNG_AFTER_IDAT) &&
             ((up->name[3] & 0x20) || keep == PNG_HANDLE_CHUNK_ALWAYS ||
             (_png_ptr->flags & PNG_FLAG_KEEP_UNSAFE_CHUNKS)))
         {
            if (up->size == 0)
               _png_warning(_png_ptr, "Writing zero-length unknown chunk");

            _png_write_chunk(_png_ptr, up->name, up->data, up->size);
         }
      }
   }
#endif
      _png_ptr->mode |= PNG_WROTE_INFO_BEFORE_PLTE;
   }
}

void PNGAPI
_png_write_info(_png_structp _png_ptr, _png_infop info_ptr)
{
#if defined(PNG_WRITE_TEXT_SUPPORTED) || defined(PNG_WRITE_sPLT_SUPPORTED)
   int i;
#endif

   _png_debug(1, "in _png_write_info");

   if (_png_ptr == NULL || info_ptr == NULL)
      return;

   _png_write_info_before_PLTE(_png_ptr, info_ptr);

   if (info_ptr->valid & PNG_INFO_PLTE)
      _png_write_PLTE(_png_ptr, info_ptr->palette,
          (_png_uint_32)info_ptr->num_palette);

   else if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      _png_error(_png_ptr, "Valid palette required for paletted images");

#ifdef PNG_WRITE_tRNS_SUPPORTED
   if (info_ptr->valid & PNG_INFO_tRNS)
   {
#ifdef PNG_WRITE_INVERT_ALPHA_SUPPORTED
      /* Invert the alpha channel (in tRNS) */
      if ((_png_ptr->transformations & PNG_INVERT_ALPHA) &&
          info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      {
         int j;
         for (j = 0; j<(int)info_ptr->num_trans; j++)
            info_ptr->trans_alpha[j] =
               (_png_byte)(255 - info_ptr->trans_alpha[j]);
      }
#endif
      _png_write_tRNS(_png_ptr, info_ptr->trans_alpha, &(info_ptr->trans_color),
          info_ptr->num_trans, info_ptr->color_type);
   }
#endif
#ifdef PNG_WRITE_bKGD_SUPPORTED
   if (info_ptr->valid & PNG_INFO_bKGD)
      _png_write_bKGD(_png_ptr, &(info_ptr->background), info_ptr->color_type);
#endif

#ifdef PNG_WRITE_hIST_SUPPORTED
   if (info_ptr->valid & PNG_INFO_hIST)
      _png_write_hIST(_png_ptr, info_ptr->hist, info_ptr->num_palette);
#endif

#ifdef PNG_WRITE_oFFs_SUPPORTED
   if (info_ptr->valid & PNG_INFO_oFFs)
      _png_write_oFFs(_png_ptr, info_ptr->x_offset, info_ptr->y_offset,
          info_ptr->offset_unit_type);
#endif

#ifdef PNG_WRITE_pCAL_SUPPORTED
   if (info_ptr->valid & PNG_INFO_pCAL)
      _png_write_pCAL(_png_ptr, info_ptr->pcal_purpose, info_ptr->pcal_X0,
          info_ptr->pcal_X1, info_ptr->pcal_type, info_ptr->pcal_nparams,
          info_ptr->pcal_units, info_ptr->pcal_params);
#endif

#ifdef PNG_WRITE_sCAL_SUPPORTED
   if (info_ptr->valid & PNG_INFO_sCAL)
      _png_write_sCAL_s(_png_ptr, (int)info_ptr->scal_unit,
          info_ptr->scal_s_width, info_ptr->scal_s_height);
#endif /* sCAL */

#ifdef PNG_WRITE_pHYs_SUPPORTED
   if (info_ptr->valid & PNG_INFO_pHYs)
      _png_write_pHYs(_png_ptr, info_ptr->x_pixels_per_unit,
          info_ptr->y_pixels_per_unit, info_ptr->phys_unit_type);
#endif /* pHYs */

#ifdef PNG_WRITE_tIME_SUPPORTED
   if (info_ptr->valid & PNG_INFO_tIME)
   {
      _png_write_tIME(_png_ptr, &(info_ptr->mod_time));
      _png_ptr->mode |= PNG_WROTE_tIME;
   }
#endif /* tIME */

#ifdef PNG_WRITE_sPLT_SUPPORTED
   if (info_ptr->valid & PNG_INFO_sPLT)
      for (i = 0; i < (int)info_ptr->splt_palettes_num; i++)
         _png_write_sPLT(_png_ptr, info_ptr->splt_palettes + i);
#endif /* sPLT */

#ifdef PNG_WRITE_TEXT_SUPPORTED
   /* Check to see if we need to write text chunks */
   for (i = 0; i < info_ptr->num_text; i++)
   {
      _png_debug2(2, "Writing header text chunk %d, type %d", i,
          info_ptr->text[i].compression);
      /* An internationalized chunk? */
      if (info_ptr->text[i].compression > 0)
      {
#ifdef PNG_WRITE_iTXt_SUPPORTED
         /* Write international chunk */
         _png_write_iTXt(_png_ptr,
             info_ptr->text[i].compression,
             info_ptr->text[i].key,
             info_ptr->text[i].lang,
             info_ptr->text[i].lang_key,
             info_ptr->text[i].text);
#else
          _png_warning(_png_ptr, "Unable to write international text");
#endif
          /* Mark this chunk as written */
          info_ptr->text[i].compression = PNG_TEXT_COMPRESSION_NONE_WR;
      }

      /* If we want a compressed text chunk */
      else if (info_ptr->text[i].compression == PNG_TEXT_COMPRESSION_zTXt)
      {
#ifdef PNG_WRITE_zTXt_SUPPORTED
         /* Write compressed chunk */
         _png_write_zTXt(_png_ptr, info_ptr->text[i].key,
             info_ptr->text[i].text, 0,
             info_ptr->text[i].compression);
#else
         _png_warning(_png_ptr, "Unable to write compressed text");
#endif
         /* Mark this chunk as written */
         info_ptr->text[i].compression = PNG_TEXT_COMPRESSION_zTXt_WR;
      }

      else if (info_ptr->text[i].compression == PNG_TEXT_COMPRESSION_NONE)
      {
#ifdef PNG_WRITE_tEXt_SUPPORTED
         /* Write uncompressed chunk */
         _png_write_tEXt(_png_ptr, info_ptr->text[i].key,
             info_ptr->text[i].text,
             0);
         /* Mark this chunk as written */
         info_ptr->text[i].compression = PNG_TEXT_COMPRESSION_NONE_WR;
#else
         /* Can't get here */
         _png_warning(_png_ptr, "Unable to write uncompressed text");
#endif
      }
   }
#endif /* tEXt */

#ifdef PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED
   if (info_ptr->unknown_chunks_num)
   {
      _png_unknown_chunk *up;

      _png_debug(5, "writing extra chunks");

      for (up = info_ptr->unknown_chunks;
           up < info_ptr->unknown_chunks + info_ptr->unknown_chunks_num;
           up++)
      {
         int keep = _png_handle_as_unknown(_png_ptr, up->name);
         if (keep != PNG_HANDLE_CHUNK_NEVER &&
             up->location &&
             (up->location & PNG_HAVE_PLTE) &&
             !(up->location & PNG_HAVE_IDAT) &&
             !(up->location & PNG_AFTER_IDAT) &&
             ((up->name[3] & 0x20) || keep == PNG_HANDLE_CHUNK_ALWAYS ||
             (_png_ptr->flags & PNG_FLAG_KEEP_UNSAFE_CHUNKS)))
         {
            _png_write_chunk(_png_ptr, up->name, up->data, up->size);
         }
      }
   }
#endif
}

/* Writes the end of the PNG file.  If you don't want to write comments or
 * time information, you can pass NULL for info.  If you already wrote these
 * in _png_write_info(), do not write them again here.  If you have long
 * comments, I suggest writing them here, and compressing them.
 */
void PNGAPI
_png_write_end(_png_structp _png_ptr, _png_infop info_ptr)
{
   _png_debug(1, "in _png_write_end");

   if (_png_ptr == NULL)
      return;

   if (!(_png_ptr->mode & PNG_HAVE_IDAT))
      _png_error(_png_ptr, "No IDATs written into file");

#ifdef PNG_WRITE_CHECK_FOR_INVALID_INDEX_SUPPORTED
   if (_png_ptr->num_palette_max > _png_ptr->num_palette)
      _png_benign_error(_png_ptr, "Wrote palette index exceeding num_palette");
#endif

   /* See if user wants us to write information chunks */
   if (info_ptr != NULL)
   {
#ifdef PNG_WRITE_TEXT_SUPPORTED
      int i; /* local index variable */
#endif
#ifdef PNG_WRITE_tIME_SUPPORTED
      /* Check to see if user has supplied a time chunk */
      if ((info_ptr->valid & PNG_INFO_tIME) &&
          !(_png_ptr->mode & PNG_WROTE_tIME))
         _png_write_tIME(_png_ptr, &(info_ptr->mod_time));

#endif
#ifdef PNG_WRITE_TEXT_SUPPORTED
      /* Loop through comment chunks */
      for (i = 0; i < info_ptr->num_text; i++)
      {
         _png_debug2(2, "Writing trailer text chunk %d, type %d", i,
            info_ptr->text[i].compression);
         /* An internationalized chunk? */
         if (info_ptr->text[i].compression > 0)
         {
#ifdef PNG_WRITE_iTXt_SUPPORTED
            /* Write international chunk */
            _png_write_iTXt(_png_ptr,
                info_ptr->text[i].compression,
                info_ptr->text[i].key,
                info_ptr->text[i].lang,
                info_ptr->text[i].lang_key,
                info_ptr->text[i].text);
#else
            _png_warning(_png_ptr, "Unable to write international text");
#endif
            /* Mark this chunk as written */
            info_ptr->text[i].compression = PNG_TEXT_COMPRESSION_NONE_WR;
         }

         else if (info_ptr->text[i].compression >= PNG_TEXT_COMPRESSION_zTXt)
         {
#ifdef PNG_WRITE_zTXt_SUPPORTED
            /* Write compressed chunk */
            _png_write_zTXt(_png_ptr, info_ptr->text[i].key,
                info_ptr->text[i].text, 0,
                info_ptr->text[i].compression);
#else
            _png_warning(_png_ptr, "Unable to write compressed text");
#endif
            /* Mark this chunk as written */
            info_ptr->text[i].compression = PNG_TEXT_COMPRESSION_zTXt_WR;
         }

         else if (info_ptr->text[i].compression == PNG_TEXT_COMPRESSION_NONE)
         {
#ifdef PNG_WRITE_tEXt_SUPPORTED
            /* Write uncompressed chunk */
            _png_write_tEXt(_png_ptr, info_ptr->text[i].key,
                info_ptr->text[i].text, 0);
#else
            _png_warning(_png_ptr, "Unable to write uncompressed text");
#endif

            /* Mark this chunk as written */
            info_ptr->text[i].compression = PNG_TEXT_COMPRESSION_NONE_WR;
         }
      }
#endif
#ifdef PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED
   if (info_ptr->unknown_chunks_num)
   {
      _png_unknown_chunk *up;

      _png_debug(5, "writing extra chunks");

      for (up = info_ptr->unknown_chunks;
           up < info_ptr->unknown_chunks + info_ptr->unknown_chunks_num;
           up++)
      {
         int keep = _png_handle_as_unknown(_png_ptr, up->name);
         if (keep != PNG_HANDLE_CHUNK_NEVER &&
             up->location &&
             (up->location & PNG_AFTER_IDAT) &&
             ((up->name[3] & 0x20) || keep == PNG_HANDLE_CHUNK_ALWAYS ||
             (_png_ptr->flags & PNG_FLAG_KEEP_UNSAFE_CHUNKS)))
         {
            _png_write_chunk(_png_ptr, up->name, up->data, up->size);
         }
      }
   }
#endif
   }

   _png_ptr->mode |= PNG_AFTER_IDAT;

   /* Write end of PNG file */
   _png_write_IEND(_png_ptr);
   /* This flush, added in libpng-1.0.8, removed from libpng-1.0.9beta03,
    * and restored again in libpng-1.2.30, may cause some applications that
    * do not set _png_ptr->output_flush_fn to crash.  If your application
    * experiences a problem, please try building libpng with
    * PNG_WRITE_FLUSH_AFTER_IEND_SUPPORTED defined, and report the event to
    * png-mng-implement at lists.sf.net .
    */
#ifdef PNG_WRITE_FLUSH_SUPPORTED
#  ifdef PNG_WRITE_FLUSH_AFTER_IEND_SUPPORTED
   _png_flush(_png_ptr);
#  endif
#endif
}

#ifdef PNG_CONVERT_tIME_SUPPORTED
void PNGAPI
_png_convert_from_struct_tm(_png_timep ptime, PNG_CONST struct KDTm FAR * ttime)
{
   _png_debug(1, "in _png_convert_from_struct_tm");

   ptime->year = (_png_uint_16)(1900 + ttime->tm_year);
   ptime->month = (_png_byte)(ttime->tm_mon + 1);
   ptime->day = (_png_byte)ttime->tm_mday;
   ptime->hour = (_png_byte)ttime->tm_hour;
   ptime->minute = (_png_byte)ttime->tm_min;
   ptime->second = (_png_byte)ttime->tm_sec;
}

void PNGAPI
_png_convert_from_time_t(_png_timep ptime, time_t ttime)
{
   struct KDTm* tbuf;

   _png_debug(1, "in _png_convert_from_time_t");

   tbuf = kdGmtime_r((const KDtime*) &ttime, 0);
   _png_convert_from_struct_tm(ptime, tbuf);
}
#endif

/* Initialize _png_ptr structure, and allocate any memory needed */
PNG_FUNCTION(_png_structp,PNGAPI
_png_create_write_struct,(_png_const_charp user__png_ver, _png_voidp error_ptr,
    _png_error_ptr error_fn, _png_error_ptr warn_fn),PNG_ALLOCATED)
{
#ifdef PNG_USER_MEM_SUPPORTED
   return (_png_create_write_struct_2(user__png_ver, error_ptr, error_fn,
       warn_fn, NULL, NULL, NULL));
}

/* Alternate initialize _png_ptr structure, and allocate any memory needed */
static void _png_reset_filter_heuristics(_png_structp _png_ptr); /* forward decl */

PNG_FUNCTION(_png_structp,PNGAPI
_png_create_write_struct_2,(_png_const_charp user__png_ver, _png_voidp error_ptr,
    _png_error_ptr error_fn, _png_error_ptr warn_fn, _png_voidp mem_ptr,
    _png_malloc_ptr malloc_fn, _png_free_ptr free_fn),PNG_ALLOCATED)
{
#endif /* PNG_USER_MEM_SUPPORTED */
   volatile int _png_cleanup_needed = 0;
#ifdef PNG_SETJMP_SUPPORTED
   volatile
#endif
   _png_structp _png_ptr;
#ifdef PNG_SETJMP_SUPPORTED
#ifdef USE_FAR_KEYWORD
   jmp_buf tmp_jmpbuf;
#endif
#endif

   _png_debug(1, "in _png_create_write_struct");

#ifdef PNG_USER_MEM_SUPPORTED
   _png_ptr = (_png_structp)_png_create_struct_2(PNG_STRUCT_PNG,
       (_png_malloc_ptr)malloc_fn, (_png_voidp)mem_ptr);
#else
   _png_ptr = (_png_structp)_png_create_struct(PNG_STRUCT_PNG);
#endif /* PNG_USER_MEM_SUPPORTED */
   if (_png_ptr == NULL)
      return (NULL);

   /* Added at libpng-1.2.6 */
#ifdef PNG_SET_USER_LIMITS_SUPPORTED
   _png_ptr->user_width_max = PNG_USER_WIDTH_MAX;
   _png_ptr->user_height_max = PNG_USER_HEIGHT_MAX;
#endif

#ifdef PNG_SETJMP_SUPPORTED
/* Applications that neglect to set up their own setjmp() and then
 * encounter a _png_error() will longjmp here.  Since the jmpbuf is
 * then meaningless we abort instead of returning.
 */
#ifdef USE_FAR_KEYWORD
   if (setjmp(tmp_jmpbuf))
#else
   if (setjmp(_png_jmpbuf(_png_ptr))) /* sets longjmp to match setjmp */
#endif
#ifdef USE_FAR_KEYWORD
   _png_memcpy(_png_jmpbuf(_png_ptr), tmp_jmpbuf, _png_sizeof(jmp_buf));
#endif
      PNG_ABORT();
#endif

#ifdef PNG_USER_MEM_SUPPORTED
   _png_set_mem_fn(_png_ptr, mem_ptr, malloc_fn, free_fn);
#endif /* PNG_USER_MEM_SUPPORTED */
   _png_set_error_fn(_png_ptr, error_ptr, error_fn, warn_fn);

   if (!_png_user_version_check(_png_ptr, user__png_ver))
      _png_cleanup_needed = 1;

   /* Initialize zbuf - compression buffer */
   _png_ptr->zbuf_size = PNG_ZBUF_SIZE;

   if (!_png_cleanup_needed)
   {
      _png_ptr->zbuf = (_png_bytep)_png_malloc_warn(_png_ptr,
          _png_ptr->zbuf_size);
      if (_png_ptr->zbuf == NULL)
         _png_cleanup_needed = 1;
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

   _png_set_write_fn(_png_ptr, NULL, NULL, NULL);

#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
   _png_reset_filter_heuristics(_png_ptr);
#endif

   return (_png_ptr);
}


/* Write a few rows of image data.  If the image is interlaced,
 * either you will have to write the 7 sub images, or, if you
 * have called _png_set_interlace_handling(), you will have to
 * "write" the image seven times.
 */
void PNGAPI
_png_write_rows(_png_structp _png_ptr, _png_bytepp row,
    _png_uint_32 num_rows)
{
   _png_uint_32 i; /* row counter */
   _png_bytepp rp; /* row pointer */

   _png_debug(1, "in _png_write_rows");

   if (_png_ptr == NULL)
      return;

   /* Loop through the rows */
   for (i = 0, rp = row; i < num_rows; i++, rp++)
   {
      _png_write_row(_png_ptr, *rp);
   }
}

/* Write the image.  You only need to call this function once, even
 * if you are writing an interlaced image.
 */
void PNGAPI
_png_write_image(_png_structp _png_ptr, _png_bytepp image)
{
   _png_uint_32 i; /* row index */
   int pass, num_pass; /* pass variables */
   _png_bytepp rp; /* points to current row */

   if (_png_ptr == NULL)
      return;

   _png_debug(1, "in _png_write_image");

#ifdef PNG_WRITE_INTERLACING_SUPPORTED
   /* Initialize interlace handling.  If image is not interlaced,
    * this will set pass to 1
    */
   num_pass = _png_set_interlace_handling(_png_ptr);
#else
   num_pass = 1;
#endif
   /* Loop through passes */
   for (pass = 0; pass < num_pass; pass++)
   {
      /* Loop through image */
      for (i = 0, rp = image; i < _png_ptr->height; i++, rp++)
      {
         _png_write_row(_png_ptr, *rp);
      }
   }
}

/* Called by user to write a row of image data */
void PNGAPI
_png_write_row(_png_structp _png_ptr, _png_const_bytep row)
{
   /* 1.5.6: moved from _png_struct to be a local structure: */
   _png_row_info row_info;

   if (_png_ptr == NULL)
      return;

   _png_debug2(1, "in _png_write_row (row %u, pass %d)",
      _png_ptr->row_number, _png_ptr->pass);

   /* Initialize transformations and other stuff if first time */
   if (_png_ptr->row_number == 0 && _png_ptr->pass == 0)
   {
      /* Make sure we wrote the header info */
      if (!(_png_ptr->mode & PNG_WROTE_INFO_BEFORE_PLTE))
         _png_error(_png_ptr,
             "_png_write_info was never called before _png_write_row");

      /* Check for transforms that have been set but were defined out */
#if !defined(PNG_WRITE_INVERT_SUPPORTED) && defined(PNG_READ_INVERT_SUPPORTED)
      if (_png_ptr->transformations & PNG_INVERT_MONO)
         _png_warning(_png_ptr, "PNG_WRITE_INVERT_SUPPORTED is not defined");
#endif

#if !defined(PNG_WRITE_FILLER_SUPPORTED) && defined(PNG_READ_FILLER_SUPPORTED)
      if (_png_ptr->transformations & PNG_FILLER)
         _png_warning(_png_ptr, "PNG_WRITE_FILLER_SUPPORTED is not defined");
#endif
#if !defined(PNG_WRITE_PACKSWAP_SUPPORTED) && \
    defined(PNG_READ_PACKSWAP_SUPPORTED)
      if (_png_ptr->transformations & PNG_PACKSWAP)
         _png_warning(_png_ptr,
             "PNG_WRITE_PACKSWAP_SUPPORTED is not defined");
#endif

#if !defined(PNG_WRITE_PACK_SUPPORTED) && defined(PNG_READ_PACK_SUPPORTED)
      if (_png_ptr->transformations & PNG_PACK)
         _png_warning(_png_ptr, "PNG_WRITE_PACK_SUPPORTED is not defined");
#endif

#if !defined(PNG_WRITE_SHIFT_SUPPORTED) && defined(PNG_READ_SHIFT_SUPPORTED)
      if (_png_ptr->transformations & PNG_SHIFT)
         _png_warning(_png_ptr, "PNG_WRITE_SHIFT_SUPPORTED is not defined");
#endif

#if !defined(PNG_WRITE_BGR_SUPPORTED) && defined(PNG_READ_BGR_SUPPORTED)
      if (_png_ptr->transformations & PNG_BGR)
         _png_warning(_png_ptr, "PNG_WRITE_BGR_SUPPORTED is not defined");
#endif

#if !defined(PNG_WRITE_SWAP_SUPPORTED) && defined(PNG_READ_SWAP_SUPPORTED)
      if (_png_ptr->transformations & PNG_SWAP_BYTES)
         _png_warning(_png_ptr, "PNG_WRITE_SWAP_SUPPORTED is not defined");
#endif

      _png_write_start_row(_png_ptr);
   }

#ifdef PNG_WRITE_INTERLACING_SUPPORTED
   /* If interlaced and not interested in row, return */
   if (_png_ptr->interlaced && (_png_ptr->transformations & PNG_INTERLACE))
   {
      switch (_png_ptr->pass)
      {
         case 0:
            if (_png_ptr->row_number & 0x07)
            {
               _png_write_finish_row(_png_ptr);
               return;
            }
            break;

         case 1:
            if ((_png_ptr->row_number & 0x07) || _png_ptr->width < 5)
            {
               _png_write_finish_row(_png_ptr);
               return;
            }
            break;

         case 2:
            if ((_png_ptr->row_number & 0x07) != 4)
            {
               _png_write_finish_row(_png_ptr);
               return;
            }
            break;

         case 3:
            if ((_png_ptr->row_number & 0x03) || _png_ptr->width < 3)
            {
               _png_write_finish_row(_png_ptr);
               return;
            }
            break;

         case 4:
            if ((_png_ptr->row_number & 0x03) != 2)
            {
               _png_write_finish_row(_png_ptr);
               return;
            }
            break;

         case 5:
            if ((_png_ptr->row_number & 0x01) || _png_ptr->width < 2)
            {
               _png_write_finish_row(_png_ptr);
               return;
            }
            break;

         case 6:
            if (!(_png_ptr->row_number & 0x01))
            {
               _png_write_finish_row(_png_ptr);
               return;
            }
            break;

         default: /* error: ignore it */
            break;
      }
   }
#endif

   /* Set up row info for transformations */
   row_info.color_type = _png_ptr->color_type;
   row_info.width = _png_ptr->usr_width;
   row_info.channels = _png_ptr->usr_channels;
   row_info.bit_depth = _png_ptr->usr_bit_depth;
   row_info.pixel_depth = (_png_byte)(row_info.bit_depth * row_info.channels);
   row_info.rowbytes = PNG_ROWBYTES(row_info.pixel_depth, row_info.width);

   _png_debug1(3, "row_info->color_type = %d", row_info.color_type);
   _png_debug1(3, "row_info->width = %u", row_info.width);
   _png_debug1(3, "row_info->channels = %d", row_info.channels);
   _png_debug1(3, "row_info->bit_depth = %d", row_info.bit_depth);
   _png_debug1(3, "row_info->pixel_depth = %d", row_info.pixel_depth);
   _png_debug1(3, "row_info->rowbytes = %lu", (unsigned long)row_info.rowbytes);

   /* Copy user's row into buffer, leaving room for filter byte. */
   _png_memcpy(_png_ptr->row_buf + 1, row, row_info.rowbytes);

#ifdef PNG_WRITE_INTERLACING_SUPPORTED
   /* Handle interlacing */
   if (_png_ptr->interlaced && _png_ptr->pass < 6 &&
       (_png_ptr->transformations & PNG_INTERLACE))
   {
      _png_do_write_interlace(&row_info, _png_ptr->row_buf + 1, _png_ptr->pass);
      /* This should always get caught above, but still ... */
      if (!(row_info.width))
      {
         _png_write_finish_row(_png_ptr);
         return;
      }
   }
#endif

#ifdef PNG_WRITE_TRANSFORMS_SUPPORTED
   /* Handle other transformations */
   if (_png_ptr->transformations)
      _png_do_write_transformations(_png_ptr, &row_info);
#endif

   /* At this point the row_info pixel depth must match the 'transformed' depth,
    * which is also the output depth.
    */
   if (row_info.pixel_depth != _png_ptr->pixel_depth ||
      row_info.pixel_depth != _png_ptr->transformed_pixel_depth)
      _png_error(_png_ptr, "internal write transform logic error");

#ifdef PNG_MNG_FEATURES_SUPPORTED
   /* Write filter_method 64 (intrapixel differencing) only if
    * 1. Libpng was compiled with PNG_MNG_FEATURES_SUPPORTED and
    * 2. Libpng did not write a PNG signature (this filter_method is only
    *    used in PNG datastreams that are embedded in MNG datastreams) and
    * 3. The application called _png_permit_mng_features with a mask that
    *    included PNG_FLAG_MNG_FILTER_64 and
    * 4. The filter_method is 64 and
    * 5. The color_type is RGB or RGBA
    */
   if ((_png_ptr->mng_features_permitted & PNG_FLAG_MNG_FILTER_64) &&
       (_png_ptr->filter_type == PNG_INTRAPIXEL_DIFFERENCING))
   {
      /* Intrapixel differencing */
      _png_do_write_intrapixel(&row_info, _png_ptr->row_buf + 1);
   }
#endif

/* Added at libpng-1.5.10 */
#ifdef PNG_WRITE_CHECK_FOR_INVALID_INDEX_SUPPORTED
   /* Check for out-of-range palette index */
   if (row_info.color_type == PNG_COLOR_TYPE_PALETTE &&
       _png_ptr->num_palette_max >= 0)
      _png_do_check_palette_indexes(_png_ptr, &row_info);
#endif

   /* Find a filter if necessary, filter the row and write it out. */
   _png_write_find_filter(_png_ptr, &row_info);

   if (_png_ptr->write_row_fn != NULL)
      (*(_png_ptr->write_row_fn))(_png_ptr, _png_ptr->row_number, _png_ptr->pass);
}

#ifdef PNG_WRITE_FLUSH_SUPPORTED
/* Set the automatic flush interval or 0 to turn flushing off */
void PNGAPI
_png_set_flush(_png_structp _png_ptr, int nrows)
{
   _png_debug(1, "in _png_set_flush");

   if (_png_ptr == NULL)
      return;

   _png_ptr->flush_dist = (nrows < 0 ? 0 : nrows);
}

/* Flush the current output buffers now */
void PNGAPI
_png_write_flush(_png_structp _png_ptr)
{
   int wrote_IDAT;

   _png_debug(1, "in _png_write_flush");

   if (_png_ptr == NULL)
      return;

   /* We have already written out all of the data */
   if (_png_ptr->row_number >= _png_ptr->num_rows)
      return;

   do
   {
      int ret;

      /* Compress the data */
      ret = deflate(&_png_ptr->zstream, Z_SYNC_FLUSH);
      wrote_IDAT = 0;

      /* Check for compression errors */
      if (ret != Z_OK)
      {
         if (_png_ptr->zstream.msg != NULL)
            _png_error(_png_ptr, _png_ptr->zstream.msg);

         else
            _png_error(_png_ptr, "zlib error");
      }

      if (!(_png_ptr->zstream.avail_out))
      {
         /* Write the IDAT and reset the zlib output buffer */
         _png_write_IDAT(_png_ptr, _png_ptr->zbuf, _png_ptr->zbuf_size);
         wrote_IDAT = 1;
      }
   } while (wrote_IDAT == 1);

   /* If there is any data left to be output, write it into a new IDAT */
   if (_png_ptr->zbuf_size != _png_ptr->zstream.avail_out)
   {
      /* Write the IDAT and reset the zlib output buffer */
      _png_write_IDAT(_png_ptr, _png_ptr->zbuf,
          _png_ptr->zbuf_size - _png_ptr->zstream.avail_out);
   }
   _png_ptr->flush_rows = 0;
   _png_flush(_png_ptr);
}
#endif /* PNG_WRITE_FLUSH_SUPPORTED */

/* Free all memory used by the write */
void PNGAPI
_png_destroy_write_struct(_png_structpp _png_ptr_ptr, _png_infopp info_ptr_ptr)
{
   _png_structp _png_ptr = NULL;
   _png_infop info_ptr = NULL;
#ifdef PNG_USER_MEM_SUPPORTED
   _png_free_ptr free_fn = NULL;
   _png_voidp mem_ptr = NULL;
#endif

   _png_debug(1, "in _png_destroy_write_struct");

   if (_png_ptr_ptr != NULL)
      _png_ptr = *_png_ptr_ptr;

#ifdef PNG_USER_MEM_SUPPORTED
   if (_png_ptr != NULL)
   {
      free_fn = _png_ptr->free_fn;
      mem_ptr = _png_ptr->mem_ptr;
   }
#endif

   if (info_ptr_ptr != NULL)
      info_ptr = *info_ptr_ptr;

   if (info_ptr != NULL)
   {
      if (_png_ptr != NULL)
      {
         _png_free_data(_png_ptr, info_ptr, PNG_FREE_ALL, -1);

#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
         if (_png_ptr->num_chunk_list)
         {
            _png_free(_png_ptr, _png_ptr->chunk_list);
            _png_ptr->num_chunk_list = 0;
         }
#endif
      }

#ifdef PNG_USER_MEM_SUPPORTED
      _png_destroy_struct_2((_png_voidp)info_ptr, (_png_free_ptr)free_fn,
          (_png_voidp)mem_ptr);
#else
      _png_destroy_struct((_png_voidp)info_ptr);
#endif
      *info_ptr_ptr = NULL;
   }

   if (_png_ptr != NULL)
   {
      _png_write_destroy(_png_ptr);
#ifdef PNG_USER_MEM_SUPPORTED
      _png_destroy_struct_2((_png_voidp)_png_ptr, (_png_free_ptr)free_fn,
          (_png_voidp)mem_ptr);
#else
      _png_destroy_struct((_png_voidp)_png_ptr);
#endif
      *_png_ptr_ptr = NULL;
   }
}


/* Free any memory used in _png_ptr struct (old method) */
void /* PRIVATE */
_png_write_destroy(_png_structp _png_ptr)
{
#ifdef PNG_SETJMP_SUPPORTED
   jmp_buf tmp_jmp; /* Save jump buffer */
#endif
   _png_error_ptr error_fn;
#ifdef PNG_WARNINGS_SUPPORTED
   _png_error_ptr warning_fn;
#endif
   _png_voidp error_ptr;
#ifdef PNG_USER_MEM_SUPPORTED
   _png_free_ptr free_fn;
#endif

   _png_debug(1, "in _png_write_destroy");

   /* Free any memory zlib uses */
   if (_png_ptr->zlib_state != PNG_ZLIB_UNINITIALIZED)
      deflateEnd(&_png_ptr->zstream);

   /* Free our memory.  _png_free checks NULL for us. */
   _png_free(_png_ptr, _png_ptr->zbuf);
   _png_free(_png_ptr, _png_ptr->row_buf);
#ifdef PNG_WRITE_FILTER_SUPPORTED
   _png_free(_png_ptr, _png_ptr->prev_row);
   _png_free(_png_ptr, _png_ptr->sub_row);
   _png_free(_png_ptr, _png_ptr->up_row);
   _png_free(_png_ptr, _png_ptr->avg_row);
   _png_free(_png_ptr, _png_ptr->paeth_row);
#endif

#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
   /* Use this to save a little code space, it doesn't free the filter_costs */
   _png_reset_filter_heuristics(_png_ptr);
   _png_free(_png_ptr, _png_ptr->filter_costs);
   _png_free(_png_ptr, _png_ptr->inv_filter_costs);
#endif

#ifdef PNG_SETJMP_SUPPORTED
   /* Reset structure */
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

/* Allow the application to select one or more row filters to use. */
void PNGAPI
_png_set_filter(_png_structp _png_ptr, int method, int filters)
{
   _png_debug(1, "in _png_set_filter");

   if (_png_ptr == NULL)
      return;

#ifdef PNG_MNG_FEATURES_SUPPORTED
   if ((_png_ptr->mng_features_permitted & PNG_FLAG_MNG_FILTER_64) &&
       (method == PNG_INTRAPIXEL_DIFFERENCING))
      method = PNG_FILTER_TYPE_BASE;

#endif
   if (method == PNG_FILTER_TYPE_BASE)
   {
      switch (filters & (PNG_ALL_FILTERS | 0x07))
      {
#ifdef PNG_WRITE_FILTER_SUPPORTED
         case 5:
         case 6:
         case 7: _png_warning(_png_ptr, "Unknown row filter for method 0");
             /* FALL THROUGH */
#endif /* PNG_WRITE_FILTER_SUPPORTED */
         case PNG_FILTER_VALUE_NONE:
            _png_ptr->do_filter = PNG_FILTER_NONE; break;

#ifdef PNG_WRITE_FILTER_SUPPORTED
         case PNG_FILTER_VALUE_SUB:
            _png_ptr->do_filter = PNG_FILTER_SUB; break;

         case PNG_FILTER_VALUE_UP:
            _png_ptr->do_filter = PNG_FILTER_UP; break;

         case PNG_FILTER_VALUE_AVG:
            _png_ptr->do_filter = PNG_FILTER_AVG; break;

         case PNG_FILTER_VALUE_PAETH:
            _png_ptr->do_filter = PNG_FILTER_PAETH; break;

         default:
            _png_ptr->do_filter = (_png_byte)filters; break;
#else
         default:
            _png_warning(_png_ptr, "Unknown row filter for method 0");
#endif /* PNG_WRITE_FILTER_SUPPORTED */
      }

      /* If we have allocated the row_buf, this means we have already started
       * with the image and we should have allocated all of the filter buffers
       * that have been selected.  If prev_row isn't already allocated, then
       * it is too late to start using the filters that need it, since we
       * will be missing the data in the previous row.  If an application
       * wants to start and stop using particular filters during compression,
       * it should start out with all of the filters, and then add and
       * remove them after the start of compression.
       */
      if (_png_ptr->row_buf != NULL)
      {
#ifdef PNG_WRITE_FILTER_SUPPORTED
         if ((_png_ptr->do_filter & PNG_FILTER_SUB) && _png_ptr->sub_row == NULL)
         {
            _png_ptr->sub_row = (_png_bytep)_png_malloc(_png_ptr,
                (_png_ptr->rowbytes + 1));
            _png_ptr->sub_row[0] = PNG_FILTER_VALUE_SUB;
         }

         if ((_png_ptr->do_filter & PNG_FILTER_UP) && _png_ptr->up_row == NULL)
         {
            if (_png_ptr->prev_row == NULL)
            {
               _png_warning(_png_ptr, "Can't add Up filter after starting");
               _png_ptr->do_filter = (_png_byte)(_png_ptr->do_filter &
                   ~PNG_FILTER_UP);
            }

            else
            {
               _png_ptr->up_row = (_png_bytep)_png_malloc(_png_ptr,
                   (_png_ptr->rowbytes + 1));
               _png_ptr->up_row[0] = PNG_FILTER_VALUE_UP;
            }
         }

         if ((_png_ptr->do_filter & PNG_FILTER_AVG) && _png_ptr->avg_row == NULL)
         {
            if (_png_ptr->prev_row == NULL)
            {
               _png_warning(_png_ptr, "Can't add Average filter after starting");
               _png_ptr->do_filter = (_png_byte)(_png_ptr->do_filter &
                   ~PNG_FILTER_AVG);
            }

            else
            {
               _png_ptr->avg_row = (_png_bytep)_png_malloc(_png_ptr,
                   (_png_ptr->rowbytes + 1));
               _png_ptr->avg_row[0] = PNG_FILTER_VALUE_AVG;
            }
         }

         if ((_png_ptr->do_filter & PNG_FILTER_PAETH) &&
             _png_ptr->paeth_row == NULL)
         {
            if (_png_ptr->prev_row == NULL)
            {
               _png_warning(_png_ptr, "Can't add Paeth filter after starting");
               _png_ptr->do_filter &= (_png_byte)(~PNG_FILTER_PAETH);
            }

            else
            {
               _png_ptr->paeth_row = (_png_bytep)_png_malloc(_png_ptr,
                   (_png_ptr->rowbytes + 1));
               _png_ptr->paeth_row[0] = PNG_FILTER_VALUE_PAETH;
            }
         }

         if (_png_ptr->do_filter == PNG_NO_FILTERS)
#endif /* PNG_WRITE_FILTER_SUPPORTED */
            _png_ptr->do_filter = PNG_FILTER_NONE;
      }
   }
   else
      _png_error(_png_ptr, "Unknown custom filter method");
}

/* This allows us to influence the way in which libpng chooses the "best"
 * filter for the current scanline.  While the "minimum-sum-of-absolute-
 * differences metric is relatively fast and effective, there is some
 * question as to whether it can be improved upon by trying to keep the
 * filtered data going to zlib more consistent, hopefully resulting in
 * better compression.
 */
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED      /* GRR 970116 */
/* Convenience reset API. */
static void
_png_reset_filter_heuristics(_png_structp _png_ptr)
{
   /* Clear out any old values in the 'weights' - this must be done because if
    * the app calls set_filter_heuristics multiple times with different
    * 'num_weights' values we would otherwise potentially have wrong sized
    * arrays.
    */
   _png_ptr->num_prev_filters = 0;
   _png_ptr->heuristic_method = PNG_FILTER_HEURISTIC_UNWEIGHTED;
   if (_png_ptr->prev_filters != NULL)
   {
      _png_bytep old = _png_ptr->prev_filters;
      _png_ptr->prev_filters = NULL;
      _png_free(_png_ptr, old);
   }
   if (_png_ptr->filter_weights != NULL)
   {
      _png_uint_16p old = _png_ptr->filter_weights;
      _png_ptr->filter_weights = NULL;
      _png_free(_png_ptr, old);
   }

   if (_png_ptr->inv_filter_weights != NULL)
   {
      _png_uint_16p old = _png_ptr->inv_filter_weights;
      _png_ptr->inv_filter_weights = NULL;
      _png_free(_png_ptr, old);
   }

   /* Leave the filter_costs - this array is fixed size. */
}

static int
_png_init_filter_heuristics(_png_structp _png_ptr, int heuristic_method,
   int num_weights)
{
   if (_png_ptr == NULL)
      return 0;

   /* Clear out the arrays */
   _png_reset_filter_heuristics(_png_ptr);

   /* Check arguments; the 'reset' function makes the correct settings for the
    * unweighted case, but we must handle the weight case by initializing the
    * arrays for the caller.
    */
   if (heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
   {
      int i;

      if (num_weights > 0)
      {
         _png_ptr->prev_filters = (_png_bytep)_png_malloc(_png_ptr,
             (_png_uint_32)(_png_sizeof(_png_byte) * num_weights));

         /* To make sure that the weighting starts out fairly */
         for (i = 0; i < num_weights; i++)
         {
            _png_ptr->prev_filters[i] = 255;
         }

         _png_ptr->filter_weights = (_png_uint_16p)_png_malloc(_png_ptr,
             (_png_uint_32)(_png_sizeof(_png_uint_16) * num_weights));

         _png_ptr->inv_filter_weights = (_png_uint_16p)_png_malloc(_png_ptr,
             (_png_uint_32)(_png_sizeof(_png_uint_16) * num_weights));

         for (i = 0; i < num_weights; i++)
         {
            _png_ptr->inv_filter_weights[i] =
            _png_ptr->filter_weights[i] = PNG_WEIGHT_FACTOR;
         }

         /* Safe to set this now */
         _png_ptr->num_prev_filters = (_png_byte)num_weights;
      }

      /* If, in the future, there are other filter methods, this would
       * need to be based on _png_ptr->filter.
       */
      if (_png_ptr->filter_costs == NULL)
      {
         _png_ptr->filter_costs = (_png_uint_16p)_png_malloc(_png_ptr,
             (_png_uint_32)(_png_sizeof(_png_uint_16) * PNG_FILTER_VALUE_LAST));

         _png_ptr->inv_filter_costs = (_png_uint_16p)_png_malloc(_png_ptr,
             (_png_uint_32)(_png_sizeof(_png_uint_16) * PNG_FILTER_VALUE_LAST));
      }

      for (i = 0; i < PNG_FILTER_VALUE_LAST; i++)
      {
         _png_ptr->inv_filter_costs[i] =
         _png_ptr->filter_costs[i] = PNG_COST_FACTOR;
      }

      /* All the arrays are inited, safe to set this: */
      _png_ptr->heuristic_method = PNG_FILTER_HEURISTIC_WEIGHTED;

      /* Return the 'ok' code. */
      return 1;
   }
   else if (heuristic_method == PNG_FILTER_HEURISTIC_DEFAULT ||
      heuristic_method == PNG_FILTER_HEURISTIC_UNWEIGHTED)
   {
      return 1;
   }
   else
   {
      _png_warning(_png_ptr, "Unknown filter heuristic method");
      return 0;
   }
}

/* Provide floating and fixed point APIs */
#ifdef PNG_FLOATING_POINT_SUPPORTED
void PNGAPI
_png_set_filter_heuristics(_png_structp _png_ptr, int heuristic_method,
    int num_weights, _png_const_doublep filter_weights,
    _png_const_doublep filter_costs)
{
   _png_debug(1, "in _png_set_filter_heuristics");

   /* The internal API allocates all the arrays and ensures that the elements of
    * those arrays are set to the default value.
    */
   if (!_png_init_filter_heuristics(_png_ptr, heuristic_method, num_weights))
      return;

   /* If using the weighted method copy in the weights. */
   if (heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
   {
      int i;
      for (i = 0; i < num_weights; i++)
      {
         if (filter_weights[i] <= 0.0)
         {
            _png_ptr->inv_filter_weights[i] =
            _png_ptr->filter_weights[i] = PNG_WEIGHT_FACTOR;
         }

         else
         {
            _png_ptr->inv_filter_weights[i] =
                (_png_uint_16)(PNG_WEIGHT_FACTOR*filter_weights[i]+.5);

            _png_ptr->filter_weights[i] =
                (_png_uint_16)(PNG_WEIGHT_FACTOR/filter_weights[i]+.5);
         }
      }

      /* Here is where we set the relative costs of the different filters.  We
       * should take the desired compression level into account when setting
       * the costs, so that Paeth, for instance, has a high relative cost at low
       * compression levels, while it has a lower relative cost at higher
       * compression settings.  The filter types are in order of increasing
       * relative cost, so it would be possible to do this with an algorithm.
       */
      for (i = 0; i < PNG_FILTER_VALUE_LAST; i++) if (filter_costs[i] >= 1.0)
      {
         _png_ptr->inv_filter_costs[i] =
             (_png_uint_16)(PNG_COST_FACTOR / filter_costs[i] + .5);

         _png_ptr->filter_costs[i] =
             (_png_uint_16)(PNG_COST_FACTOR * filter_costs[i] + .5);
      }
   }
}
#endif /* FLOATING_POINT */

#ifdef PNG_FIXED_POINT_SUPPORTED
void PNGAPI
_png_set_filter_heuristics_fixed(_png_structp _png_ptr, int heuristic_method,
    int num_weights, _png_const_fixed_point_p filter_weights,
    _png_const_fixed_point_p filter_costs)
{
   _png_debug(1, "in _png_set_filter_heuristics_fixed");

   /* The internal API allocates all the arrays and ensures that the elements of
    * those arrays are set to the default value.
    */
   if (!_png_init_filter_heuristics(_png_ptr, heuristic_method, num_weights))
      return;

   /* If using the weighted method copy in the weights. */
   if (heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
   {
      int i;
      for (i = 0; i < num_weights; i++)
      {
         if (filter_weights[i] <= 0)
         {
            _png_ptr->inv_filter_weights[i] =
            _png_ptr->filter_weights[i] = PNG_WEIGHT_FACTOR;
         }

         else
         {
            _png_ptr->inv_filter_weights[i] = (_png_uint_16)
               ((PNG_WEIGHT_FACTOR*filter_weights[i]+PNG_FP_HALF)/PNG_FP_1);

            _png_ptr->filter_weights[i] = (_png_uint_16)((PNG_WEIGHT_FACTOR*
               PNG_FP_1+(filter_weights[i]/2))/filter_weights[i]);
         }
      }

      /* Here is where we set the relative costs of the different filters.  We
       * should take the desired compression level into account when setting
       * the costs, so that Paeth, for instance, has a high relative cost at low
       * compression levels, while it has a lower relative cost at higher
       * compression settings.  The filter types are in order of increasing
       * relative cost, so it would be possible to do this with an algorithm.
       */
      for (i = 0; i < PNG_FILTER_VALUE_LAST; i++)
         if (filter_costs[i] >= PNG_FP_1)
      {
         _png_uint_32 tmp;

         /* Use a 32 bit unsigned temporary here because otherwise the
          * intermediate value will be a 32 bit *signed* integer (ANSI rules)
          * and this will get the wrong answer on division.
          */
         tmp = PNG_COST_FACTOR*PNG_FP_1 + (filter_costs[i]/2);
         tmp /= filter_costs[i];

         _png_ptr->inv_filter_costs[i] = (_png_uint_16)tmp;

         tmp = PNG_COST_FACTOR * filter_costs[i] + PNG_FP_HALF;
         tmp /= PNG_FP_1;

         _png_ptr->filter_costs[i] = (_png_uint_16)tmp;
      }
   }
}
#endif /* FIXED_POINT */
#endif /* PNG_WRITE_WEIGHTED_FILTER_SUPPORTED */

void PNGAPI
_png_set_compression_level(_png_structp _png_ptr, int level)
{
   _png_debug(1, "in _png_set_compression_level");

   if (_png_ptr == NULL)
      return;

   _png_ptr->flags |= PNG_FLAG_ZLIB_CUSTOM_LEVEL;
   _png_ptr->zlib_level = level;
}

void PNGAPI
_png_set_compression_mem_level(_png_structp _png_ptr, int mem_level)
{
   _png_debug(1, "in _png_set_compression_mem_level");

   if (_png_ptr == NULL)
      return;

   _png_ptr->flags |= PNG_FLAG_ZLIB_CUSTOM_MEM_LEVEL;
   _png_ptr->zlib_mem_level = mem_level;
}

void PNGAPI
_png_set_compression_strategy(_png_structp _png_ptr, int strategy)
{
   _png_debug(1, "in _png_set_compression_strategy");

   if (_png_ptr == NULL)
      return;

   _png_ptr->flags |= PNG_FLAG_ZLIB_CUSTOM_STRATEGY;
   _png_ptr->zlib_strategy = strategy;
}

/* If PNG_WRITE_OPTIMIZE_CMF_SUPPORTED is defined, libpng will use a
 * smaller value of window_bits if it can do so safely.
 */
void PNGAPI
_png_set_compression_window_bits(_png_structp _png_ptr, int window_bits)
{
   if (_png_ptr == NULL)
      return;

   if (window_bits > 15)
      _png_warning(_png_ptr, "Only compression windows <= 32k supported by PNG");

   else if (window_bits < 8)
      _png_warning(_png_ptr, "Only compression windows >= 256 supported by PNG");

#ifndef WBITS_8_OK
   /* Avoid libpng bug with 256-byte windows */
   if (window_bits == 8)
      {
        _png_warning(_png_ptr, "Compression window is being reset to 512");
        window_bits = 9;
      }

#endif
   _png_ptr->flags |= PNG_FLAG_ZLIB_CUSTOM_WINDOW_BITS;
   _png_ptr->zlib_window_bits = window_bits;
}

void PNGAPI
_png_set_compression_method(_png_structp _png_ptr, int method)
{
   _png_debug(1, "in _png_set_compression_method");

   if (_png_ptr == NULL)
      return;

   if (method != 8)
      _png_warning(_png_ptr, "Only compression method 8 is supported by PNG");

   _png_ptr->flags |= PNG_FLAG_ZLIB_CUSTOM_METHOD;
   _png_ptr->zlib_method = method;
}

/* The following were added to libpng-1.5.4 */
#ifdef PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED
void PNGAPI
_png_set_text_compression_level(_png_structp _png_ptr, int level)
{
   _png_debug(1, "in _png_set_text_compression_level");

   if (_png_ptr == NULL)
      return;

   _png_ptr->flags |= PNG_FLAG_ZTXT_CUSTOM_LEVEL;
   _png_ptr->zlib_text_level = level;
}

void PNGAPI
_png_set_text_compression_mem_level(_png_structp _png_ptr, int mem_level)
{
   _png_debug(1, "in _png_set_text_compression_mem_level");

   if (_png_ptr == NULL)
      return;

   _png_ptr->flags |= PNG_FLAG_ZTXT_CUSTOM_MEM_LEVEL;
   _png_ptr->zlib_text_mem_level = mem_level;
}

void PNGAPI
_png_set_text_compression_strategy(_png_structp _png_ptr, int strategy)
{
   _png_debug(1, "in _png_set_text_compression_strategy");

   if (_png_ptr == NULL)
      return;

   _png_ptr->flags |= PNG_FLAG_ZTXT_CUSTOM_STRATEGY;
   _png_ptr->zlib_text_strategy = strategy;
}

/* If PNG_WRITE_OPTIMIZE_CMF_SUPPORTED is defined, libpng will use a
 * smaller value of window_bits if it can do so safely.
 */
void PNGAPI
_png_set_text_compression_window_bits(_png_structp _png_ptr, int window_bits)
{
   if (_png_ptr == NULL)
      return;

   if (window_bits > 15)
      _png_warning(_png_ptr, "Only compression windows <= 32k supported by PNG");

   else if (window_bits < 8)
      _png_warning(_png_ptr, "Only compression windows >= 256 supported by PNG");

#ifndef WBITS_8_OK
   /* Avoid libpng bug with 256-byte windows */
   if (window_bits == 8)
      {
        _png_warning(_png_ptr, "Text compression window is being reset to 512");
        window_bits = 9;
      }

#endif
   _png_ptr->flags |= PNG_FLAG_ZTXT_CUSTOM_WINDOW_BITS;
   _png_ptr->zlib_text_window_bits = window_bits;
}

void PNGAPI
_png_set_text_compression_method(_png_structp _png_ptr, int method)
{
   _png_debug(1, "in _png_set_text_compression_method");

   if (_png_ptr == NULL)
      return;

   if (method != 8)
      _png_warning(_png_ptr, "Only compression method 8 is supported by PNG");

   _png_ptr->flags |= PNG_FLAG_ZTXT_CUSTOM_METHOD;
   _png_ptr->zlib_text_method = method;
}
#endif /* PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED */
/* end of API added to libpng-1.5.4 */

void PNGAPI
_png_set_write_status_fn(_png_structp _png_ptr, _png_write_status_ptr write_row_fn)
{
   if (_png_ptr == NULL)
      return;

   _png_ptr->write_row_fn = write_row_fn;
}

#ifdef PNG_WRITE_USER_TRANSFORM_SUPPORTED
void PNGAPI
_png_set_write_user_transform_fn(_png_structp _png_ptr, _png_user_transform_ptr
    write_user_transform_fn)
{
   _png_debug(1, "in _png_set_write_user_transform_fn");

   if (_png_ptr == NULL)
      return;

   _png_ptr->transformations |= PNG_USER_TRANSFORM;
   _png_ptr->write_user_transform_fn = write_user_transform_fn;
}
#endif


#ifdef PNG_INFO_IMAGE_SUPPORTED
void PNGAPI
_png_write_png(_png_structp _png_ptr, _png_infop info_ptr,
    int transforms, voidp params)
{
   if (_png_ptr == NULL || info_ptr == NULL)
      return;

   /* Write the file header information. */
   _png_write_info(_png_ptr, info_ptr);

   /* ------ these transformations don't touch the info structure ------- */

#ifdef PNG_WRITE_INVERT_SUPPORTED
   /* Invert monochrome pixels */
   if (transforms & PNG_TRANSFORM_INVERT_MONO)
      _png_set_invert_mono(_png_ptr);
#endif

#ifdef PNG_WRITE_SHIFT_SUPPORTED
   /* Shift the pixels up to a legal bit depth and fill in
    * as appropriate to correctly scale the image.
    */
   if ((transforms & PNG_TRANSFORM_SHIFT)
       && (info_ptr->valid & PNG_INFO_sBIT))
      _png_set_shift(_png_ptr, &info_ptr->sig_bit);
#endif

#ifdef PNG_WRITE_PACK_SUPPORTED
   /* Pack pixels into bytes */
   if (transforms & PNG_TRANSFORM_PACKING)
       _png_set_packing(_png_ptr);
#endif

#ifdef PNG_WRITE_SWAP_ALPHA_SUPPORTED
   /* Swap location of alpha bytes from ARGB to RGBA */
   if (transforms & PNG_TRANSFORM_SWAP_ALPHA)
      _png_set_swap_alpha(_png_ptr);
#endif

#ifdef PNG_WRITE_FILLER_SUPPORTED
   /* Pack XRGB/RGBX/ARGB/RGBA into RGB (4 channels -> 3 channels) */
   if (transforms & PNG_TRANSFORM_STRIP_FILLER_AFTER)
      _png_set_filler(_png_ptr, 0, PNG_FILLER_AFTER);

   else if (transforms & PNG_TRANSFORM_STRIP_FILLER_BEFORE)
      _png_set_filler(_png_ptr, 0, PNG_FILLER_BEFORE);
#endif

#ifdef PNG_WRITE_BGR_SUPPORTED
   /* Flip BGR pixels to RGB */
   if (transforms & PNG_TRANSFORM_BGR)
      _png_set_bgr(_png_ptr);
#endif

#ifdef PNG_WRITE_SWAP_SUPPORTED
   /* Swap bytes of 16-bit files to most significant byte first */
   if (transforms & PNG_TRANSFORM_SWAP_ENDIAN)
      _png_set_swap(_png_ptr);
#endif

#ifdef PNG_WRITE_PACKSWAP_SUPPORTED
   /* Swap bits of 1, 2, 4 bit packed pixel formats */
   if (transforms & PNG_TRANSFORM_PACKSWAP)
      _png_set_packswap(_png_ptr);
#endif

#ifdef PNG_WRITE_INVERT_ALPHA_SUPPORTED
   /* Invert the alpha channel from opacity to transparency */
   if (transforms & PNG_TRANSFORM_INVERT_ALPHA)
      _png_set_invert_alpha(_png_ptr);
#endif

   /* ----------------------- end of transformations ------------------- */

   /* Write the bits */
   if (info_ptr->valid & PNG_INFO_IDAT)
       _png_write_image(_png_ptr, info_ptr->row_pointers);

   /* It is REQUIRED to call this to finish writing the rest of the file */
   _png_write_end(_png_ptr, info_ptr);

   PNG_UNUSED(transforms)   /* Quiet compiler warnings */
   PNG_UNUSED(params)
}
#endif
#endif /* PNG_WRITE_SUPPORTED */
