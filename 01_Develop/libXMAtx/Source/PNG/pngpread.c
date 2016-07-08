
/* pngpread.c - read a png file in push mode
 *
 * Last changed in libpng 1.5.11 [June 14, 2012]
 * Copyright (c) 1998-2012 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 */

#include "pngpriv.h"

#ifdef PNG_PROGRESSIVE_READ_SUPPORTED

/* Push model modes */
#define PNG_READ_SIG_MODE   0
#define PNG_READ_CHUNK_MODE 1
#define PNG_READ_IDAT_MODE  2
#define PNG_SKIP_MODE       3
#define PNG_READ_tEXt_MODE  4
#define PNG_READ_zTXt_MODE  5
#define PNG_READ_DONE_MODE  6
#define PNG_READ_iTXt_MODE  7
#define PNG_ERROR_MODE      8

void PNGAPI
_png_process_data(_png_structp _png_ptr, _png_infop info_ptr,
    _png_bytep buffer, _png_size_t buffer_size)
{
   if (_png_ptr == NULL || info_ptr == NULL)
      return;

   _png_push_restore_buffer(_png_ptr, buffer, buffer_size);

   while (_png_ptr->buffer_size)
   {
      _png_process_some_data(_png_ptr, info_ptr);
   }
}

_png_size_t PNGAPI
_png_process_data_pause(_png_structp _png_ptr, int save)
{
   if (_png_ptr != NULL)
   {
      /* It's easiest for the caller if we do the save, then the caller doesn't
       * have to supply the same data again:
       */
      if (save)
         _png_push_save_buffer(_png_ptr);
      else
      {
         /* This includes any pending saved bytes: */
         _png_size_t remaining = _png_ptr->buffer_size;
         _png_ptr->buffer_size = 0;

         /* So subtract the saved buffer size, unless all the data
          * is actually 'saved', in which case we just return 0
          */
         if (_png_ptr->save_buffer_size < remaining)
            return remaining - _png_ptr->save_buffer_size;
      }
   }

   return 0;
}

_png_uint_32 PNGAPI
_png_process_data_skip(_png_structp _png_ptr)
{
   _png_uint_32 remaining = 0;

   if (_png_ptr != NULL && _png_ptr->process_mode == PNG_SKIP_MODE &&
      _png_ptr->skip_length > 0)
   {
      /* At the end of _png_process_data the buffer size must be 0 (see the loop
       * above) so we can detect a broken call here:
       */
      if (_png_ptr->buffer_size != 0)
         _png_error(_png_ptr,
            "_png_process_data_skip called inside _png_process_data");

      /* If is impossible for there to be a saved buffer at this point -
       * otherwise we could not be in SKIP mode.  This will also happen if
       * _png_process_skip is called inside _png_process_data (but only very
       * rarely.)
       */
      if (_png_ptr->save_buffer_size != 0)
         _png_error(_png_ptr, "_png_process_data_skip called with saved data");

      remaining = _png_ptr->skip_length;
      _png_ptr->skip_length = 0;
      _png_ptr->process_mode = PNG_READ_CHUNK_MODE;
   }

   return remaining;
}

/* What we do with the incoming data depends on what we were previously
 * doing before we ran out of data...
 */
void /* PRIVATE */
_png_process_some_data(_png_structp _png_ptr, _png_infop info_ptr)
{
   if (_png_ptr == NULL)
      return;

   switch (_png_ptr->process_mode)
   {
      case PNG_READ_SIG_MODE:
      {
         _png_push_read_sig(_png_ptr, info_ptr);
         break;
      }

      case PNG_READ_CHUNK_MODE:
      {
         _png_push_read_chunk(_png_ptr, info_ptr);
         break;
      }

      case PNG_READ_IDAT_MODE:
      {
         _png_push_read_IDAT(_png_ptr);
         break;
      }

      case PNG_SKIP_MODE:
      {
         _png_push_crc_finish(_png_ptr);
         break;
      }

      default:
      {
         _png_ptr->buffer_size = 0;
         break;
      }
   }
}

/* Read any remaining signature bytes from the stream and compare them with
 * the correct PNG signature.  It is possible that this routine is called
 * with bytes already read from the signature, either because they have been
 * checked by the calling application, or because of multiple calls to this
 * routine.
 */
void /* PRIVATE */
_png_push_read_sig(_png_structp _png_ptr, _png_infop info_ptr)
{
   _png_size_t num_checked = _png_ptr->sig_bytes,
       num_to_check = 8 - num_checked;

   if (_png_ptr->buffer_size < num_to_check)
   {
      num_to_check = _png_ptr->buffer_size;
   }

   _png_push_fill_buffer(_png_ptr, &(info_ptr->signature[num_checked]),
       num_to_check);
   _png_ptr->sig_bytes = (_png_byte)(_png_ptr->sig_bytes + num_to_check);

   if (_png_sig_cmp(info_ptr->signature, num_checked, num_to_check))
   {
      if (num_checked < 4 &&
          _png_sig_cmp(info_ptr->signature, num_checked, num_to_check - 4))
         _png_error(_png_ptr, "Not a PNG file");

      else
         _png_error(_png_ptr, "PNG file corrupted by ASCII conversion");
   }

   else
   {
      if (_png_ptr->sig_bytes >= 8)
      {
         _png_ptr->process_mode = PNG_READ_CHUNK_MODE;
      }
   }
}

void /* PRIVATE */
_png_push_read_chunk(_png_structp _png_ptr, _png_infop info_ptr)
{
   _png_uint_32 chunk_name;

   /* First we make sure we have enough data for the 4 byte chunk name
    * and the 4 byte chunk length before proceeding with decoding the
    * chunk data.  To fully decode each of these chunks, we also make
    * sure we have enough data in the buffer for the 4 byte CRC at the
    * end of every chunk (except IDAT, which is handled separately).
    */
   if (!(_png_ptr->mode & PNG_HAVE_CHUNK_HEADER))
   {
      _png_byte chunk_length[4];
      _png_byte chunk_tag[4];

      if (_png_ptr->buffer_size < 8)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_push_fill_buffer(_png_ptr, chunk_length, 4);
      _png_ptr->push_length = _png_get_uint_31(_png_ptr, chunk_length);
      _png_reset_crc(_png_ptr);
      _png_crc_read(_png_ptr, chunk_tag, 4);
      _png_ptr->chunk_name = PNG_CHUNK_FROM_STRING(chunk_tag);
      _png_check_chunk_name(_png_ptr, _png_ptr->chunk_name);
      _png_ptr->mode |= PNG_HAVE_CHUNK_HEADER;
   }

   chunk_name = _png_ptr->chunk_name;

   if (chunk_name == _png_IDAT)
   {
      /* This is here above the if/else case statement below because if the
       * unknown handling marks 'IDAT' as unknown then the IDAT handling case is
       * completely skipped.
       *
       * TODO: there must be a better way of doing this.
       */
      if (_png_ptr->mode & PNG_AFTER_IDAT)
         _png_ptr->mode |= PNG_HAVE_CHUNK_AFTER_IDAT;
   }

   if (chunk_name == _png_IHDR)
   {
      if (_png_ptr->push_length != 13)
         _png_error(_png_ptr, "Invalid IHDR length");

      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_IHDR(_png_ptr, info_ptr, _png_ptr->push_length);
   }

   else if (chunk_name == _png_IEND)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_IEND(_png_ptr, info_ptr, _png_ptr->push_length);

      _png_ptr->process_mode = PNG_READ_DONE_MODE;
      _png_push_have_end(_png_ptr, info_ptr);
   }

#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
   else if (_png_chunk_unknown_handling(_png_ptr, chunk_name))
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      if (chunk_name == _png_IDAT)
         _png_ptr->mode |= PNG_HAVE_IDAT;

      _png_handle_unknown(_png_ptr, info_ptr, _png_ptr->push_length);

      if (chunk_name == _png_PLTE)
         _png_ptr->mode |= PNG_HAVE_PLTE;

      else if (chunk_name == _png_IDAT)
      {
         if (!(_png_ptr->mode & PNG_HAVE_IHDR))
            _png_error(_png_ptr, "Missing IHDR before IDAT");

         else if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE &&
             !(_png_ptr->mode & PNG_HAVE_PLTE))
            _png_error(_png_ptr, "Missing PLTE before IDAT");
      }
   }
#endif

   else if (chunk_name == _png_PLTE)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }
      _png_handle_PLTE(_png_ptr, info_ptr, _png_ptr->push_length);
   }

   else if (chunk_name == _png_IDAT)
   {
      /* If we reach an IDAT chunk, this means we have read all of the
       * header chunks, and we can start reading the image (or if this
       * is called after the image has been read - we have an error).
       */

      if (!(_png_ptr->mode & PNG_HAVE_IHDR))
         _png_error(_png_ptr, "Missing IHDR before IDAT");

      else if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE &&
          !(_png_ptr->mode & PNG_HAVE_PLTE))
         _png_error(_png_ptr, "Missing PLTE before IDAT");

      if (_png_ptr->mode & PNG_HAVE_IDAT)
      {
         if (!(_png_ptr->mode & PNG_HAVE_CHUNK_AFTER_IDAT))
            if (_png_ptr->push_length == 0)
               return;

         if (_png_ptr->mode & PNG_AFTER_IDAT)
            _png_benign_error(_png_ptr, "Too many IDATs found");
      }

      _png_ptr->idat_size = _png_ptr->push_length;
      _png_ptr->mode |= PNG_HAVE_IDAT;
      _png_ptr->process_mode = PNG_READ_IDAT_MODE;
      _png_push_have_info(_png_ptr, info_ptr);
      _png_ptr->zstream.avail_out =
          (uInt) PNG_ROWBYTES(_png_ptr->pixel_depth,
          _png_ptr->iwidth) + 1;
      _png_ptr->zstream.next_out = _png_ptr->row_buf;
      return;
   }

#ifdef PNG_READ_gAMA_SUPPORTED
   else if (_png_ptr->chunk_name == _png_gAMA)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_gAMA(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_sBIT_SUPPORTED
   else if (_png_ptr->chunk_name == _png_sBIT)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_sBIT(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_cHRM_SUPPORTED
   else if (_png_ptr->chunk_name == _png_cHRM)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_cHRM(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_sRGB_SUPPORTED
   else if (chunk_name == _png_sRGB)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_sRGB(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_iCCP_SUPPORTED
   else if (_png_ptr->chunk_name == _png_iCCP)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_iCCP(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_sPLT_SUPPORTED
   else if (chunk_name == _png_sPLT)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_sPLT(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_tRNS_SUPPORTED
   else if (chunk_name == _png_tRNS)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_tRNS(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_bKGD_SUPPORTED
   else if (chunk_name == _png_bKGD)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_bKGD(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_hIST_SUPPORTED
   else if (chunk_name == _png_hIST)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_hIST(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_pHYs_SUPPORTED
   else if (chunk_name == _png_pHYs)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_pHYs(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_oFFs_SUPPORTED
   else if (chunk_name == _png_oFFs)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_oFFs(_png_ptr, info_ptr, _png_ptr->push_length);
   }
#endif

#ifdef PNG_READ_pCAL_SUPPORTED
   else if (chunk_name == _png_pCAL)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_pCAL(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_sCAL_SUPPORTED
   else if (chunk_name == _png_sCAL)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_sCAL(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_tIME_SUPPORTED
   else if (chunk_name == _png_tIME)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_tIME(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_tEXt_SUPPORTED
   else if (chunk_name == _png_tEXt)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_tEXt(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_zTXt_SUPPORTED
   else if (chunk_name == _png_zTXt)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_zTXt(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif
#ifdef PNG_READ_iTXt_SUPPORTED
   else if (chunk_name == _png_iTXt)
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_handle_iTXt(_png_ptr, info_ptr, _png_ptr->push_length);
   }

#endif

   else
   {
      if (_png_ptr->push_length + 4 > _png_ptr->buffer_size)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }
      _png_handle_unknown(_png_ptr, info_ptr, _png_ptr->push_length);
   }

   _png_ptr->mode &= ~PNG_HAVE_CHUNK_HEADER;
}

void /* PRIVATE */
_png_push_crc_skip(_png_structp _png_ptr, _png_uint_32 skip)
{
   _png_ptr->process_mode = PNG_SKIP_MODE;
   _png_ptr->skip_length = skip;
}

void /* PRIVATE */
_png_push_crc_finish(_png_structp _png_ptr)
{
   if (_png_ptr->skip_length && _png_ptr->save_buffer_size)
   {
      _png_size_t save_size = _png_ptr->save_buffer_size;
      _png_uint_32 skip_length = _png_ptr->skip_length;

      /* We want the smaller of 'skip_length' and 'save_buffer_size', but
       * they are of different types and we don't know which variable has the
       * fewest bits.  Carefully select the smaller and cast it to the type of
       * the larger - this cannot overflow.  Do not cast in the following test
       * - it will break on either 16 or 64 bit platforms.
       */
      if (skip_length < save_size)
         save_size = (_png_size_t)skip_length;

      else
         skip_length = (_png_uint_32)save_size;

      _png_calculate_crc(_png_ptr, _png_ptr->save_buffer_ptr, save_size);

      _png_ptr->skip_length -= skip_length;
      _png_ptr->buffer_size -= save_size;
      _png_ptr->save_buffer_size -= save_size;
      _png_ptr->save_buffer_ptr += save_size;
   }

   if (_png_ptr->skip_length && _png_ptr->current_buffer_size)
   {
      _png_size_t save_size = _png_ptr->current_buffer_size;
      _png_uint_32 skip_length = _png_ptr->skip_length;

      /* We want the smaller of 'skip_length' and 'current_buffer_size', here,
       * the same problem exists as above and the same solution.
       */
      if (skip_length < save_size)
         save_size = (_png_size_t)skip_length;

      else
         skip_length = (_png_uint_32)save_size;

      _png_calculate_crc(_png_ptr, _png_ptr->current_buffer_ptr, save_size);

      _png_ptr->skip_length -= skip_length;
      _png_ptr->buffer_size -= save_size;
      _png_ptr->current_buffer_size -= save_size;
      _png_ptr->current_buffer_ptr += save_size;
   }

   if (!_png_ptr->skip_length)
   {
      if (_png_ptr->buffer_size < 4)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_crc_finish(_png_ptr, 0);
      _png_ptr->process_mode = PNG_READ_CHUNK_MODE;
   }
}

void PNGCBAPI
_png_push_fill_buffer(_png_structp _png_ptr, _png_bytep buffer, _png_size_t length)
{
   _png_bytep ptr;

   if (_png_ptr == NULL)
      return;

   ptr = buffer;

   if (_png_ptr->save_buffer_size)
   {
      _png_size_t save_size;

      if (length < _png_ptr->save_buffer_size)
         save_size = length;

      else
         save_size = _png_ptr->save_buffer_size;

      _png_memcpy(ptr, _png_ptr->save_buffer_ptr, save_size);
      length -= save_size;
      ptr += save_size;
      _png_ptr->buffer_size -= save_size;
      _png_ptr->save_buffer_size -= save_size;
      _png_ptr->save_buffer_ptr += save_size;
   }

   if (length && _png_ptr->current_buffer_size)
   {
      _png_size_t save_size;

      if (length < _png_ptr->current_buffer_size)
         save_size = length;

      else
         save_size = _png_ptr->current_buffer_size;

      _png_memcpy(ptr, _png_ptr->current_buffer_ptr, save_size);
      _png_ptr->buffer_size -= save_size;
      _png_ptr->current_buffer_size -= save_size;
      _png_ptr->current_buffer_ptr += save_size;
   }
}

void /* PRIVATE */
_png_push_save_buffer(_png_structp _png_ptr)
{
   if (_png_ptr->save_buffer_size)
   {
      if (_png_ptr->save_buffer_ptr != _png_ptr->save_buffer)
      {
         _png_size_t i, istop;
         _png_bytep sp;
         _png_bytep dp;

         istop = _png_ptr->save_buffer_size;

         for (i = 0, sp = _png_ptr->save_buffer_ptr, dp = _png_ptr->save_buffer;
             i < istop; i++, sp++, dp++)
         {
            *dp = *sp;
         }
      }
   }

   if (_png_ptr->save_buffer_size + _png_ptr->current_buffer_size >
       _png_ptr->save_buffer_max)
   {
      _png_size_t new_max;
      _png_bytep old_buffer;

      if (_png_ptr->save_buffer_size > PNG_SIZE_MAX -
          (_png_ptr->current_buffer_size + 256))
      {
         _png_error(_png_ptr, "Potential overflow of save_buffer");
      }

      new_max = _png_ptr->save_buffer_size + _png_ptr->current_buffer_size + 256;
      old_buffer = _png_ptr->save_buffer;
      _png_ptr->save_buffer = (_png_bytep)_png_malloc_warn(_png_ptr, new_max);

      if (_png_ptr->save_buffer == NULL)
      {
         _png_free(_png_ptr, old_buffer);
         _png_error(_png_ptr, "Insufficient memory for save_buffer");
      }

      _png_memcpy(_png_ptr->save_buffer, old_buffer, _png_ptr->save_buffer_size);
      _png_free(_png_ptr, old_buffer);
      _png_ptr->save_buffer_max = new_max;
   }

   if (_png_ptr->current_buffer_size)
   {
      _png_memcpy(_png_ptr->save_buffer + _png_ptr->save_buffer_size,
         _png_ptr->current_buffer_ptr, _png_ptr->current_buffer_size);
      _png_ptr->save_buffer_size += _png_ptr->current_buffer_size;
      _png_ptr->current_buffer_size = 0;
   }

   _png_ptr->save_buffer_ptr = _png_ptr->save_buffer;
   _png_ptr->buffer_size = 0;
}

void /* PRIVATE */
_png_push_restore_buffer(_png_structp _png_ptr, _png_bytep buffer,
   _png_size_t buffer_length)
{
   _png_ptr->current_buffer = buffer;
   _png_ptr->current_buffer_size = buffer_length;
   _png_ptr->buffer_size = buffer_length + _png_ptr->save_buffer_size;
   _png_ptr->current_buffer_ptr = _png_ptr->current_buffer;
}

void /* PRIVATE */
_png_push_read_IDAT(_png_structp _png_ptr)
{
   if (!(_png_ptr->mode & PNG_HAVE_CHUNK_HEADER))
   {
      _png_byte chunk_length[4];
      _png_byte chunk_tag[4];

      /* TODO: this code can be commoned up with the same code in push_read */
      if (_png_ptr->buffer_size < 8)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_push_fill_buffer(_png_ptr, chunk_length, 4);
      _png_ptr->push_length = _png_get_uint_31(_png_ptr, chunk_length);
      _png_reset_crc(_png_ptr);
      _png_crc_read(_png_ptr, chunk_tag, 4);
      _png_ptr->chunk_name = PNG_CHUNK_FROM_STRING(chunk_tag);
      _png_ptr->mode |= PNG_HAVE_CHUNK_HEADER;

      if (_png_ptr->chunk_name != _png_IDAT)
      {
         _png_ptr->process_mode = PNG_READ_CHUNK_MODE;

         if (!(_png_ptr->flags & PNG_FLAG_ZLIB_FINISHED))
            _png_error(_png_ptr, "Not enough compressed data");

         return;
      }

      _png_ptr->idat_size = _png_ptr->push_length;
   }

   if (_png_ptr->idat_size && _png_ptr->save_buffer_size)
   {
      _png_size_t save_size = _png_ptr->save_buffer_size;
      _png_uint_32 idat_size = _png_ptr->idat_size;

      /* We want the smaller of 'idat_size' and 'current_buffer_size', but they
       * are of different types and we don't know which variable has the fewest
       * bits.  Carefully select the smaller and cast it to the type of the
       * larger - this cannot overflow.  Do not cast in the following test - it
       * will break on either 16 or 64 bit platforms.
       */
      if (idat_size < save_size)
         save_size = (_png_size_t)idat_size;

      else
         idat_size = (_png_uint_32)save_size;

      _png_calculate_crc(_png_ptr, _png_ptr->save_buffer_ptr, save_size);

      _png_process_IDAT_data(_png_ptr, _png_ptr->save_buffer_ptr, save_size);

      _png_ptr->idat_size -= idat_size;
      _png_ptr->buffer_size -= save_size;
      _png_ptr->save_buffer_size -= save_size;
      _png_ptr->save_buffer_ptr += save_size;
   }

   if (_png_ptr->idat_size && _png_ptr->current_buffer_size)
   {
      _png_size_t save_size = _png_ptr->current_buffer_size;
      _png_uint_32 idat_size = _png_ptr->idat_size;

      /* We want the smaller of 'idat_size' and 'current_buffer_size', but they
       * are of different types and we don't know which variable has the fewest
       * bits.  Carefully select the smaller and cast it to the type of the
       * larger - this cannot overflow.
       */
      if (idat_size < save_size)
         save_size = (_png_size_t)idat_size;

      else
         idat_size = (_png_uint_32)save_size;

      _png_calculate_crc(_png_ptr, _png_ptr->current_buffer_ptr, save_size);

      _png_process_IDAT_data(_png_ptr, _png_ptr->current_buffer_ptr, save_size);

      _png_ptr->idat_size -= idat_size;
      _png_ptr->buffer_size -= save_size;
      _png_ptr->current_buffer_size -= save_size;
      _png_ptr->current_buffer_ptr += save_size;
   }

   if (!_png_ptr->idat_size)
   {
      if (_png_ptr->buffer_size < 4)
      {
         _png_push_save_buffer(_png_ptr);
         return;
      }

      _png_crc_finish(_png_ptr, 0);
      _png_ptr->mode &= ~PNG_HAVE_CHUNK_HEADER;
      _png_ptr->mode |= PNG_AFTER_IDAT;
   }
}

void /* PRIVATE */
_png_process_IDAT_data(_png_structp _png_ptr, _png_bytep buffer,
   _png_size_t buffer_length)
{
   /* The caller checks for a non-zero buffer length. */
   if (!(buffer_length > 0) || buffer == NULL)
      _png_error(_png_ptr, "No IDAT data (internal error)");

   /* This routine must process all the data it has been given
    * before returning, calling the row callback as required to
    * handle the uncompressed results.
    */
   _png_ptr->zstream.next_in = buffer;
   _png_ptr->zstream.avail_in = (uInt)buffer_length;

   /* Keep going until the decompressed data is all processed
    * or the stream marked as finished.
    */
   while (_png_ptr->zstream.avail_in > 0 &&
          !(_png_ptr->flags & PNG_FLAG_ZLIB_FINISHED))
   {
      int ret;

      /* We have data for zlib, but we must check that zlib
       * has someplace to put the results.  It doesn't matter
       * if we don't expect any results -- it may be the input
       * data is just the LZ end code.
       */
      if (!(_png_ptr->zstream.avail_out > 0))
      {
         _png_ptr->zstream.avail_out =
             (uInt) PNG_ROWBYTES(_png_ptr->pixel_depth,
             _png_ptr->iwidth) + 1;

         _png_ptr->zstream.next_out = _png_ptr->row_buf;
      }

      /* Using Z_SYNC_FLUSH here means that an unterminated
       * LZ stream (a stream with a missing end code) can still
       * be handled, otherwise (Z_NO_FLUSH) a future zlib
       * implementation might defer output and therefore
       * change the current behavior (see comments in inflate.c
       * for why this doesn't happen at present with zlib 1.2.5).
       */
      ret = inflate(&_png_ptr->zstream, Z_SYNC_FLUSH);

      /* Check for any failure before proceeding. */
      if (ret != Z_OK && ret != Z_STREAM_END)
      {
         /* Terminate the decompression. */
         _png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;

         /* This may be a truncated stream (missing or
          * damaged end code).  Treat that as a warning.
          */
         if (_png_ptr->row_number >= _png_ptr->num_rows ||
             _png_ptr->pass > 6)
            _png_warning(_png_ptr, "Truncated compressed data in IDAT");

         else
            _png_error(_png_ptr, "Decompression error in IDAT");

         /* Skip the check on unprocessed input */
         return;
      }

      /* Did inflate output any data? */
      if (_png_ptr->zstream.next_out != _png_ptr->row_buf)
      {
         /* Is this unexpected data after the last row?
          * If it is, artificially terminate the LZ output
          * here.
          */
         if (_png_ptr->row_number >= _png_ptr->num_rows ||
             _png_ptr->pass > 6)
         {
            /* Extra data. */
            _png_warning(_png_ptr, "Extra compressed data in IDAT");
            _png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;

            /* Do no more processing; skip the unprocessed
             * input check below.
             */
            return;
         }

         /* Do we have a complete row? */
         if (_png_ptr->zstream.avail_out == 0)
            _png_push_process_row(_png_ptr);
      }

      /* And check for the end of the stream. */
      if (ret == Z_STREAM_END)
         _png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;
   }

   /* All the data should have been processed, if anything
    * is left at this point we have bytes of IDAT data
    * after the zlib end code.
    */
   if (_png_ptr->zstream.avail_in > 0)
      _png_warning(_png_ptr, "Extra compression data in IDAT");
}

void /* PRIVATE */
_png_push_process_row(_png_structp _png_ptr)
{
   /* 1.5.6: row_info moved out of _png_struct to a local here. */
   _png_row_info row_info;

   row_info.width = _png_ptr->iwidth; /* NOTE: width of current interlaced row */
   row_info.color_type = _png_ptr->color_type;
   row_info.bit_depth = _png_ptr->bit_depth;
   row_info.channels = _png_ptr->channels;
   row_info.pixel_depth = _png_ptr->pixel_depth;
   row_info.rowbytes = PNG_ROWBYTES(row_info.pixel_depth, row_info.width);

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
    * interlaced row count:
    */
   _png_memcpy(_png_ptr->prev_row, _png_ptr->row_buf, row_info.rowbytes + 1);

#ifdef PNG_READ_TRANSFORMS_SUPPORTED
   if (_png_ptr->transformations)
      _png_do_read_transformations(_png_ptr, &row_info);
#endif

   /* The transformed pixel depth should match the depth now in row_info. */
   if (_png_ptr->transformed_pixel_depth == 0)
   {
      _png_ptr->transformed_pixel_depth = row_info.pixel_depth;
      if (row_info.pixel_depth > _png_ptr->maximum_pixel_depth)
         _png_error(_png_ptr, "progressive row overflow");
   }

   else if (_png_ptr->transformed_pixel_depth != row_info.pixel_depth)
      _png_error(_png_ptr, "internal progressive row size calculation error");


#ifdef PNG_READ_INTERLACING_SUPPORTED
   /* Blow up interlaced rows to full size */
   if (_png_ptr->interlaced && (_png_ptr->transformations & PNG_INTERLACE))
   {
      if (_png_ptr->pass < 6)
         _png_do_read_interlace(&row_info, _png_ptr->row_buf + 1, _png_ptr->pass,
            _png_ptr->transformations);

    switch (_png_ptr->pass)
    {
         case 0:
         {
            int i;
            for (i = 0; i < 8 && _png_ptr->pass == 0; i++)
            {
               _png_push_have_row(_png_ptr, _png_ptr->row_buf + 1);
               _png_read_push_finish_row(_png_ptr); /* Updates _png_ptr->pass */
            }

            if (_png_ptr->pass == 2) /* Pass 1 might be empty */
            {
               for (i = 0; i < 4 && _png_ptr->pass == 2; i++)
               {
                  _png_push_have_row(_png_ptr, NULL);
                  _png_read_push_finish_row(_png_ptr);
               }
            }

            if (_png_ptr->pass == 4 && _png_ptr->height <= 4)
            {
               for (i = 0; i < 2 && _png_ptr->pass == 4; i++)
               {
                  _png_push_have_row(_png_ptr, NULL);
                  _png_read_push_finish_row(_png_ptr);
               }
            }

            if (_png_ptr->pass == 6 && _png_ptr->height <= 4)
            {
                _png_push_have_row(_png_ptr, NULL);
                _png_read_push_finish_row(_png_ptr);
            }

            break;
         }

         case 1:
         {
            int i;
            for (i = 0; i < 8 && _png_ptr->pass == 1; i++)
            {
               _png_push_have_row(_png_ptr, _png_ptr->row_buf + 1);
               _png_read_push_finish_row(_png_ptr);
            }

            if (_png_ptr->pass == 2) /* Skip top 4 generated rows */
            {
               for (i = 0; i < 4 && _png_ptr->pass == 2; i++)
               {
                  _png_push_have_row(_png_ptr, NULL);
                  _png_read_push_finish_row(_png_ptr);
               }
            }

            break;
         }

         case 2:
         {
            int i;

            for (i = 0; i < 4 && _png_ptr->pass == 2; i++)
            {
               _png_push_have_row(_png_ptr, _png_ptr->row_buf + 1);
               _png_read_push_finish_row(_png_ptr);
            }

            for (i = 0; i < 4 && _png_ptr->pass == 2; i++)
            {
               _png_push_have_row(_png_ptr, NULL);
               _png_read_push_finish_row(_png_ptr);
            }

            if (_png_ptr->pass == 4) /* Pass 3 might be empty */
            {
               for (i = 0; i < 2 && _png_ptr->pass == 4; i++)
               {
                  _png_push_have_row(_png_ptr, NULL);
                  _png_read_push_finish_row(_png_ptr);
               }
            }

            break;
         }

         case 3:
         {
            int i;

            for (i = 0; i < 4 && _png_ptr->pass == 3; i++)
            {
               _png_push_have_row(_png_ptr, _png_ptr->row_buf + 1);
               _png_read_push_finish_row(_png_ptr);
            }

            if (_png_ptr->pass == 4) /* Skip top two generated rows */
            {
               for (i = 0; i < 2 && _png_ptr->pass == 4; i++)
               {
                  _png_push_have_row(_png_ptr, NULL);
                  _png_read_push_finish_row(_png_ptr);
               }
            }

            break;
         }

         case 4:
         {
            int i;

            for (i = 0; i < 2 && _png_ptr->pass == 4; i++)
            {
               _png_push_have_row(_png_ptr, _png_ptr->row_buf + 1);
               _png_read_push_finish_row(_png_ptr);
            }

            for (i = 0; i < 2 && _png_ptr->pass == 4; i++)
            {
               _png_push_have_row(_png_ptr, NULL);
               _png_read_push_finish_row(_png_ptr);
            }

            if (_png_ptr->pass == 6) /* Pass 5 might be empty */
            {
               _png_push_have_row(_png_ptr, NULL);
               _png_read_push_finish_row(_png_ptr);
            }

            break;
         }

         case 5:
         {
            int i;

            for (i = 0; i < 2 && _png_ptr->pass == 5; i++)
            {
               _png_push_have_row(_png_ptr, _png_ptr->row_buf + 1);
               _png_read_push_finish_row(_png_ptr);
            }

            if (_png_ptr->pass == 6) /* Skip top generated row */
            {
               _png_push_have_row(_png_ptr, NULL);
               _png_read_push_finish_row(_png_ptr);
            }

            break;
         }

         default:
         case 6:
         {
            _png_push_have_row(_png_ptr, _png_ptr->row_buf + 1);
            _png_read_push_finish_row(_png_ptr);

            if (_png_ptr->pass != 6)
               break;

            _png_push_have_row(_png_ptr, NULL);
            _png_read_push_finish_row(_png_ptr);
         }
      }
   }
   else
#endif
   {
      _png_push_have_row(_png_ptr, _png_ptr->row_buf + 1);
      _png_read_push_finish_row(_png_ptr);
   }
}

void /* PRIVATE */
_png_read_push_finish_row(_png_structp _png_ptr)
{
#ifdef PNG_READ_INTERLACING_SUPPORTED
   /* Arrays to facilitate easy interlacing - use pass (0 - 6) as index */

   /* Start of interlace block */
   static PNG_CONST _png_byte FARDATA _png_pass_start[] = {0, 4, 0, 2, 0, 1, 0};

   /* Offset to next interlace block */
   static PNG_CONST _png_byte FARDATA _png_pass_inc[] = {8, 8, 4, 4, 2, 2, 1};

   /* Start of interlace block in the y direction */
   static PNG_CONST _png_byte FARDATA _png_pass_ystart[] = {0, 0, 4, 0, 2, 0, 1};

   /* Offset to next interlace block in the y direction */
   static PNG_CONST _png_byte FARDATA _png_pass_yinc[] = {8, 8, 8, 4, 4, 2, 2};

   /* Height of interlace block.  This is not currently used - if you need
    * it, uncomment it here and in png.h
   static PNG_CONST _png_byte FARDATA _png_pass_height[] = {8, 8, 4, 4, 2, 2, 1};
   */
#endif

   _png_ptr->row_number++;
   if (_png_ptr->row_number < _png_ptr->num_rows)
      return;

#ifdef PNG_READ_INTERLACING_SUPPORTED
   if (_png_ptr->interlaced)
   {
      _png_ptr->row_number = 0;
      _png_memset(_png_ptr->prev_row, 0, _png_ptr->rowbytes + 1);

      do
      {
         _png_ptr->pass++;
         if ((_png_ptr->pass == 1 && _png_ptr->width < 5) ||
             (_png_ptr->pass == 3 && _png_ptr->width < 3) ||
             (_png_ptr->pass == 5 && _png_ptr->width < 2))
            _png_ptr->pass++;

         if (_png_ptr->pass > 7)
            _png_ptr->pass--;

         if (_png_ptr->pass >= 7)
            break;

         _png_ptr->iwidth = (_png_ptr->width +
             _png_pass_inc[_png_ptr->pass] - 1 -
             _png_pass_start[_png_ptr->pass]) /
             _png_pass_inc[_png_ptr->pass];

         if (_png_ptr->transformations & PNG_INTERLACE)
            break;

         _png_ptr->num_rows = (_png_ptr->height +
             _png_pass_yinc[_png_ptr->pass] - 1 -
             _png_pass_ystart[_png_ptr->pass]) /
             _png_pass_yinc[_png_ptr->pass];

      } while (_png_ptr->iwidth == 0 || _png_ptr->num_rows == 0);
   }
#endif /* PNG_READ_INTERLACING_SUPPORTED */
}

void /* PRIVATE */
_png_push_have_info(_png_structp _png_ptr, _png_infop info_ptr)
{
   if (_png_ptr->info_fn != NULL)
      (*(_png_ptr->info_fn))(_png_ptr, info_ptr);
}

void /* PRIVATE */
_png_push_have_end(_png_structp _png_ptr, _png_infop info_ptr)
{
   if (_png_ptr->end_fn != NULL)
      (*(_png_ptr->end_fn))(_png_ptr, info_ptr);
}

void /* PRIVATE */
_png_push_have_row(_png_structp _png_ptr, _png_bytep row)
{
   if (_png_ptr->row_fn != NULL)
      (*(_png_ptr->row_fn))(_png_ptr, row, _png_ptr->row_number,
         (int)_png_ptr->pass);
}

#ifdef PNG_READ_INTERLACING_SUPPORTED
void PNGAPI
_png_progressive_combine_row (_png_structp _png_ptr, _png_bytep old_row,
    _png_const_bytep new_row)
{
   if (_png_ptr == NULL)
      return;

   /* new_row is a flag here - if it is NULL then the app callback was called
    * from an empty row (see the calls to _png_struct::row_fn below), otherwise
    * it must be _png_ptr->row_buf+1
    */
   if (new_row != NULL)
      _png_combine_row(_png_ptr, old_row, 1/*display*/);
}
#endif /* PNG_READ_INTERLACING_SUPPORTED */

void PNGAPI
_png_set_progressive_read_fn(_png_structp _png_ptr, _png_voidp progressive_ptr,
    _png_progressive_info_ptr info_fn, _png_progressive_row_ptr row_fn,
    _png_progressive_end_ptr end_fn)
{
   if (_png_ptr == NULL)
      return;

   _png_ptr->info_fn = info_fn;
   _png_ptr->row_fn = row_fn;
   _png_ptr->end_fn = end_fn;

   _png_set_read_fn(_png_ptr, progressive_ptr, _png_push_fill_buffer);
}

_png_voidp PNGAPI
_png_get_progressive_ptr(_png_const_structp _png_ptr)
{
   if (_png_ptr == NULL)
      return (NULL);

   return _png_ptr->io_ptr;
}
#endif /* PNG_PROGRESSIVE_READ_SUPPORTED */
