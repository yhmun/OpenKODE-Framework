
/* pngrutil.c - utilities to read a PNG file
 *
 * Last changed in libpng 1.5.14 [January 24, 2013]
 * Copyright (c) 1998-2013 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 *
 * This file contains routines that are only called from within
 * libpng itself during the course of reading an image.
 */

#include "pngpriv.h"

#ifdef PNG_READ_SUPPORTED

#define _png_strtod(p,a,b) strtod(a,b)

_png_uint_32 PNGAPI
_png_get_uint_31(_png_structp _png_ptr, _png_const_bytep buf)
{
   _png_uint_32 uval = _png_get_uint_32(buf);

   if (uval > PNG_UINT_31_MAX)
      _png_error(_png_ptr, "PNG unsigned integer out of range");

   return (uval);
}

#if defined(PNG_READ_gAMA_SUPPORTED) || defined(PNG_READ_cHRM_SUPPORTED)
/* The following is a variation on the above for use with the fixed
 * point values used for gAMA and cHRM.  Instead of _png_error it
 * issues a warning and returns (-1) - an invalid value because both
 * gAMA and cHRM use *unsigned* integers for fixed point values.
 */
#define PNG_FIXED_ERROR (-1)

static _png_fixed_point /* PRIVATE */
_png_get_fixed_point(_png_structp _png_ptr, _png_const_bytep buf)
{
   _png_uint_32 uval = _png_get_uint_32(buf);

   if (uval <= PNG_UINT_31_MAX)
      return (_png_fixed_point)uval; /* known to be in range */

   /* The caller can turn off the warning by passing NULL. */
   if (_png_ptr != NULL)
      _png_warning(_png_ptr, "PNG fixed point integer out of range");

   return PNG_FIXED_ERROR;
}
#endif

#ifdef PNG_READ_INT_FUNCTIONS_SUPPORTED
/* NOTE: the read macros will obscure these definitions, so that if
 * PNG_USE_READ_MACROS is set the library will not use them internally,
 * but the APIs will still be available externally.
 *
 * The parentheses around "PNGAPI function_name" in the following three
 * functions are necessary because they allow the macros to co-exist with
 * these (unused but exported) functions.
 */

/* Grab an unsigned 32-bit integer from a buffer in big-endian format. */
_png_uint_32 (PNGAPI
_png_get_uint_32)(_png_const_bytep buf)
{
   _png_uint_32 uval =
       ((_png_uint_32)(*(buf    )) << 24) +
       ((_png_uint_32)(*(buf + 1)) << 16) +
       ((_png_uint_32)(*(buf + 2)) <<  8) +
       ((_png_uint_32)(*(buf + 3))      ) ;

   return uval;
}

/* Grab a signed 32-bit integer from a buffer in big-endian format.  The
 * data is stored in the PNG file in two's complement format and there
 * is no guarantee that a '_png_int_32' is exactly 32 bits, therefore
 * the following code does a two's complement to native conversion.
 */
_png_int_32 (PNGAPI
_png_get_int_32)(_png_const_bytep buf)
{
   _png_uint_32 uval = _png_get_uint_32(buf);
   if ((uval & 0x80000000) == 0) /* non-negative */
      return uval;

   uval = (uval ^ 0xffffffff) + 1;  /* 2's complement: -x = ~x+1 */
   return -(_png_int_32)uval;
}

/* Grab an unsigned 16-bit integer from a buffer in big-endian format. */
_png_uint_16 (PNGAPI
_png_get_uint_16)(_png_const_bytep buf)
{
   /* ANSI-C requires an int value to accomodate at least 16 bits so this
    * works and allows the compiler not to worry about possible narrowing
    * on 32 bit systems.  (Pre-ANSI systems did not make integers smaller
    * than 16 bits either.)
    */
   unsigned int val =
       ((unsigned int)(*buf) << 8) +
       ((unsigned int)(*(buf + 1)));

   return (_png_uint_16)val;
}

#endif /* PNG_READ_INT_FUNCTIONS_SUPPORTED */

/* Read and check the PNG file signature */
void /* PRIVATE */
_png_read_sig(_png_structp _png_ptr, _png_infop info_ptr)
{
   _png_size_t num_checked, num_to_check;

   /* Exit if the user application does not expect a signature. */
   if (_png_ptr->sig_bytes >= 8)
      return;

   num_checked = _png_ptr->sig_bytes;
   num_to_check = 8 - num_checked;

#ifdef PNG_IO_STATE_SUPPORTED
   _png_ptr->io_state = PNG_IO_READING | PNG_IO_SIGNATURE;
#endif

   /* The signature must be serialized in a single I/O call. */
   _png_read_data(_png_ptr, &(info_ptr->signature[num_checked]), num_to_check);
   _png_ptr->sig_bytes = 8;

   if (_png_sig_cmp(info_ptr->signature, num_checked, num_to_check))
   {
      if (num_checked < 4 &&
          _png_sig_cmp(info_ptr->signature, num_checked, num_to_check - 4))
         _png_error(_png_ptr, "Not a PNG file");
      else
         _png_error(_png_ptr, "PNG file corrupted by ASCII conversion");
   }
   if (num_checked < 3)
      _png_ptr->mode |= PNG_HAVE_PNG_SIGNATURE;
}

/* Read the chunk header (length + type name).
 * Put the type name into _png_ptr->chunk_name, and return the length.
 */
_png_uint_32 /* PRIVATE */
_png_read_chunk_header(_png_structp _png_ptr)
{
   _png_byte buf[8];
   _png_uint_32 length;

#ifdef PNG_IO_STATE_SUPPORTED
   _png_ptr->io_state = PNG_IO_READING | PNG_IO_CHUNK_HDR;
#endif

   /* Read the length and the chunk name.
    * This must be performed in a single I/O call.
    */
   _png_read_data(_png_ptr, buf, 8);
   length = _png_get_uint_31(_png_ptr, buf);

   /* Put the chunk name into _png_ptr->chunk_name. */
   _png_ptr->chunk_name = PNG_CHUNK_FROM_STRING(buf+4);

   _png_debug2(0, "Reading %lx chunk, length = %lu",
       (unsigned long)_png_ptr->chunk_name, (unsigned long)length);

   /* Reset the crc and run it over the chunk name. */
   _png_reset_crc(_png_ptr);
   _png_calculate_crc(_png_ptr, buf + 4, 4);

   /* Check to see if chunk name is valid. */
   _png_check_chunk_name(_png_ptr, _png_ptr->chunk_name);

#ifdef PNG_IO_STATE_SUPPORTED
   _png_ptr->io_state = PNG_IO_READING | PNG_IO_CHUNK_DATA;
#endif

   return length;
}

/* Read data, and (optionally) run it through the CRC. */
void /* PRIVATE */
_png_crc_read(_png_structp _png_ptr, _png_bytep buf, _png_size_t length)
{
   if (_png_ptr == NULL)
      return;

   _png_read_data(_png_ptr, buf, length);
   _png_calculate_crc(_png_ptr, buf, length);
}

/* Optionally skip data and then check the CRC.  Depending on whether we
 * are reading a ancillary or critical chunk, and how the program has set
 * things up, we may calculate the CRC on the data and print a message.
 * Returns '1' if there was a CRC error, '0' otherwise.
 */
int /* PRIVATE */
_png_crc_finish(_png_structp _png_ptr, _png_uint_32 skip)
{
   _png_size_t i;
   _png_size_t istop = _png_ptr->zbuf_size;

   for (i = (_png_size_t)skip; i > istop; i -= istop)
   {
      _png_crc_read(_png_ptr, _png_ptr->zbuf, _png_ptr->zbuf_size);
   }

   if (i)
   {
      _png_crc_read(_png_ptr, _png_ptr->zbuf, i);
   }

   if (_png_crc_error(_png_ptr))
   {
      if (PNG_CHUNK_ANCILLIARY(_png_ptr->chunk_name) ?
          !(_png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_NOWARN) :
          (_png_ptr->flags & PNG_FLAG_CRC_CRITICAL_USE))
      {
         _png_chunk_warning(_png_ptr, "CRC error");
      }

      else
      {
         _png_chunk_benign_error(_png_ptr, "CRC error");
         return (0);
      }

      return (1);
   }

   return (0);
}

/* Compare the CRC stored in the PNG file with that calculated by libpng from
 * the data it has read thus far.
 */
int /* PRIVATE */
_png_crc_error(_png_structp _png_ptr)
{
   _png_byte crc_bytes[4];
   _png_uint_32 crc;
   int need_crc = 1;

   if (PNG_CHUNK_ANCILLIARY(_png_ptr->chunk_name))
   {
      if ((_png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_MASK) ==
          (PNG_FLAG_CRC_ANCILLARY_USE | PNG_FLAG_CRC_ANCILLARY_NOWARN))
         need_crc = 0;
   }

   else /* critical */
   {
      if (_png_ptr->flags & PNG_FLAG_CRC_CRITICAL_IGNORE)
         need_crc = 0;
   }

#ifdef PNG_IO_STATE_SUPPORTED
   _png_ptr->io_state = PNG_IO_READING | PNG_IO_CHUNK_CRC;
#endif

   /* The chunk CRC must be serialized in a single I/O call. */
   _png_read_data(_png_ptr, crc_bytes, 4);

   if (need_crc)
   {
      crc = _png_get_uint_32(crc_bytes);
      return ((int)(crc != _png_ptr->crc));
   }

   else
      return (0);
}

#ifdef PNG_READ_COMPRESSED_TEXT_SUPPORTED
static _png_size_t
_png_inflate(_png_structp _png_ptr, _png_bytep data, _png_size_t size,
    _png_bytep output, _png_size_t output_size)
{
   _png_size_t count = 0;

   /* zlib can't necessarily handle more than 65535 bytes at once (i.e. it can't
    * even necessarily handle 65536 bytes) because the type uInt is "16 bits or
    * more".  Consequently it is necessary to chunk the input to zlib.  This
    * code uses ZLIB_IO_MAX, from pngpriv.h, as the maximum (the maximum value
    * that can be stored in a uInt.)  It is possible to set ZLIB_IO_MAX to a
    * lower value in pngpriv.h and this may sometimes have a performance
    * advantage, because it forces access of the input data to be separated from
    * at least some of the use by some period of time.
    */
   _png_ptr->zstream.next_in = data;
   /* avail_in is set below from 'size' */
   _png_ptr->zstream.avail_in = 0;

   while (1)
   {
      int ret, avail;

      /* The setting of 'avail_in' used to be outside the loop; by setting it
       * inside it is possible to chunk the input to zlib and simply rely on
       * zlib to advance the 'next_in' pointer.  This allows arbitrary amounts o
       * data to be passed through zlib at the unavoidable cost of requiring a
       * window save (memcpy of up to 32768 output bytes) every ZLIB_IO_MAX
       * input bytes.
       */
      if (_png_ptr->zstream.avail_in == 0 && size > 0)
      {
         if (size <= ZLIB_IO_MAX)
         {
            /* The value is less than ZLIB_IO_MAX so the cast is safe: */
            _png_ptr->zstream.avail_in = (uInt)size;
            size = 0;
         }

         else
         {
            _png_ptr->zstream.avail_in = ZLIB_IO_MAX;
            size -= ZLIB_IO_MAX;
         }
      }

      /* Reset the output buffer each time round - we empty it
       * after every inflate call.
       */
      _png_ptr->zstream.next_out = _png_ptr->zbuf;
      _png_ptr->zstream.avail_out = _png_ptr->zbuf_size;

      ret = inflate(&_png_ptr->zstream, Z_NO_FLUSH);
      avail = _png_ptr->zbuf_size - _png_ptr->zstream.avail_out;

      /* First copy/count any new output - but only if we didn't
       * get an error code.
       */
      if ((ret == Z_OK || ret == Z_STREAM_END) && avail > 0)
      {
         _png_size_t space = avail; /* > 0, see above */

         if (output != 0 && output_size > count)
         {
            _png_size_t copy = output_size - count;

            if (space < copy)
               copy = space;

            _png_memcpy(output + count, _png_ptr->zbuf, copy);
         }
         count += space;
      }

      if (ret == Z_OK)
         continue;

      /* Termination conditions - always reset the zstream, it
       * must be left in inflateInit state.
       */
      _png_ptr->zstream.avail_in = 0;
      inflateReset(&_png_ptr->zstream);

      if (ret == Z_STREAM_END)
         return count; /* NOTE: may be zero. */

      /* Now handle the error codes - the API always returns 0
       * and the error message is dumped into the uncompressed
       * buffer if available.
       */
#     ifdef PNG_WARNINGS_SUPPORTED
      {
         _png_const_charp msg;

         if (_png_ptr->zstream.msg != 0)
            msg = _png_ptr->zstream.msg;

         else switch (ret)
         {
            case Z_BUF_ERROR:
               msg = "Buffer error in compressed datastream";
               break;

            case Z_DATA_ERROR:
               msg = "Data error in compressed datastream";
               break;

            default:
               msg = "Incomplete compressed datastream";
               break;
         }

         _png_chunk_warning(_png_ptr, msg);
      }
#     endif

      /* 0 means an error - notice that this code simply ignores
       * zero length compressed chunks as a result.
       */
      return 0;
   }
}

/*
 * Decompress trailing data in a chunk.  The assumption is that chunkdata
 * points at an allocated area holding the contents of a chunk with a
 * trailing compressed part.  What we get back is an allocated area
 * holding the original prefix part and an uncompressed version of the
 * trailing part (the malloc area passed in is freed).
 */
void /* PRIVATE */
_png_decompress_chunk(_png_structp _png_ptr, int comp_type,
    _png_size_t chunklength,
    _png_size_t prefix_size, _png_size_t *newlength)
{
   /* The caller should guarantee this */
   if (prefix_size > chunklength)
   {
      /* The recovery is to delete the chunk. */
      _png_warning(_png_ptr, "invalid chunklength");
      prefix_size = 0; /* To delete everything */
   }

   else if (comp_type == PNG_COMPRESSION_TYPE_BASE)
   {
      _png_size_t expanded_size = _png_inflate(_png_ptr,
          (_png_bytep)(_png_ptr->chunkdata + prefix_size),
          chunklength - prefix_size,
          0,            /* output */
          0);           /* output size */

      /* Now check the limits on this chunk - if the limit fails the
       * compressed data will be removed, the prefix will remain.
       */
      if (prefix_size >= (~(_png_size_t)0) - 1 ||
         expanded_size >= (~(_png_size_t)0) - 1 - prefix_size
#ifdef PNG_USER_LIMITS_SUPPORTED
         || (_png_ptr->user_chunk_malloc_max &&
          (prefix_size + expanded_size >= _png_ptr->user_chunk_malloc_max - 1))
#else
         || ((PNG_USER_CHUNK_MALLOC_MAX > 0) &&
          prefix_size + expanded_size >= PNG_USER_CHUNK_MALLOC_MAX - 1)
#endif
         )
         _png_warning(_png_ptr, "Exceeded size limit while expanding chunk");

      /* If the size is zero either there was an error and a message
       * has already been output (warning) or the size really is zero
       * and we have nothing to do - the code will exit through the
       * error case below.
       */
      else if (expanded_size > 0)
      {
         /* Success (maybe) - really uncompress the chunk. */
         _png_size_t new_size = 0;
         _png_charp text = (_png_charp)_png_malloc_warn(_png_ptr,
             prefix_size + expanded_size + 1);

         if (text != NULL)
         {
            _png_memcpy(text, _png_ptr->chunkdata, prefix_size);
            new_size = _png_inflate(_png_ptr,
                (_png_bytep)(_png_ptr->chunkdata + prefix_size),
                chunklength - prefix_size,
                (_png_bytep)(text + prefix_size), expanded_size);
            text[prefix_size + expanded_size] = 0; /* just in case */

            if (new_size == expanded_size)
            {
               _png_free(_png_ptr, _png_ptr->chunkdata);
               _png_ptr->chunkdata = text;
               *newlength = prefix_size + expanded_size;
               return; /* The success return! */
            }

            _png_warning(_png_ptr, "_png_inflate logic error");
            _png_free(_png_ptr, text);
         }

         else
            _png_warning(_png_ptr, "Not enough memory to decompress chunk");
      }
   }

   else /* if (comp_type != PNG_COMPRESSION_TYPE_BASE) */
   {
      PNG_WARNING_PARAMETERS(p)
      _png_warning_parameter_signed(p, 1, PNG_NUMBER_FORMAT_d, comp_type);
      _png_formatted_warning(_png_ptr, p, "Unknown compression type @1");

      /* The recovery is to simply drop the data. */
   }

   /* Generic error return - leave the prefix, delete the compressed
    * data, reallocate the chunkdata to remove the potentially large
    * amount of compressed data.
    */
   {
      _png_charp text = (_png_charp)_png_malloc_warn(_png_ptr, prefix_size + 1);

      if (text != NULL)
      {
         if (prefix_size > 0)
            _png_memcpy(text, _png_ptr->chunkdata, prefix_size);

         _png_free(_png_ptr, _png_ptr->chunkdata);
         _png_ptr->chunkdata = text;

         /* This is an extra zero in the 'uncompressed' part. */
         *(_png_ptr->chunkdata + prefix_size) = 0x00;
      }
      /* Ignore a malloc error here - it is safe. */
   }

   *newlength = prefix_size;
}
#endif /* PNG_READ_COMPRESSED_TEXT_SUPPORTED */

/* Read and check the IDHR chunk */
void /* PRIVATE */
_png_handle_IHDR(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_byte buf[13];
   _png_uint_32 width, height;
   int bit_depth, color_type, compression_type, filter_type;
   int interlace_type;

   _png_debug(1, "in _png_handle_IHDR");

   if (_png_ptr->mode & PNG_HAVE_IHDR)
      _png_error(_png_ptr, "Out of place IHDR");

   /* Check the length */
   if (length != 13)
      _png_error(_png_ptr, "Invalid IHDR chunk");

   _png_ptr->mode |= PNG_HAVE_IHDR;

   _png_crc_read(_png_ptr, buf, 13);
   _png_crc_finish(_png_ptr, 0);

   width = _png_get_uint_31(_png_ptr, buf);
   height = _png_get_uint_31(_png_ptr, buf + 4);
   bit_depth = buf[8];
   color_type = buf[9];
   compression_type = buf[10];
   filter_type = buf[11];
   interlace_type = buf[12];

   /* Set internal variables */
   _png_ptr->width = width;
   _png_ptr->height = height;
   _png_ptr->bit_depth = (_png_byte)bit_depth;
   _png_ptr->interlaced = (_png_byte)interlace_type;
   _png_ptr->color_type = (_png_byte)color_type;
#ifdef PNG_MNG_FEATURES_SUPPORTED
   _png_ptr->filter_type = (_png_byte)filter_type;
#endif
   _png_ptr->compression_type = (_png_byte)compression_type;

   /* Find number of channels */
   switch (_png_ptr->color_type)
   {
      default: /* invalid, _png_set_IHDR calls _png_error */
      case PNG_COLOR_TYPE_GRAY:
      case PNG_COLOR_TYPE_PALETTE:
         _png_ptr->channels = 1;
         break;

      case PNG_COLOR_TYPE_RGB:
         _png_ptr->channels = 3;
         break;

      case PNG_COLOR_TYPE_GRAY_ALPHA:
         _png_ptr->channels = 2;
         break;

      case PNG_COLOR_TYPE_RGB_ALPHA:
         _png_ptr->channels = 4;
         break;
   }

   /* Set up other useful info */
   _png_ptr->pixel_depth = (_png_byte)(_png_ptr->bit_depth *
   _png_ptr->channels);
   _png_ptr->rowbytes = PNG_ROWBYTES(_png_ptr->pixel_depth, _png_ptr->width);
   _png_debug1(3, "bit_depth = %d", _png_ptr->bit_depth);
   _png_debug1(3, "channels = %d", _png_ptr->channels);
   _png_debug1(3, "rowbytes = %lu", (unsigned long)_png_ptr->rowbytes);
   _png_set_IHDR(_png_ptr, info_ptr, width, height, bit_depth,
       color_type, interlace_type, compression_type, filter_type);
}

/* Read and check the palette */
void /* PRIVATE */
_png_handle_PLTE(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_color palette[PNG_MAX_PALETTE_LENGTH];
   int num, i;
#ifdef PNG_POINTER_INDEXING_SUPPORTED
   _png_colorp pal_ptr;
#endif

   _png_debug(1, "in _png_handle_PLTE");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before PLTE");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid PLTE after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (_png_ptr->mode & PNG_HAVE_PLTE)
      _png_error(_png_ptr, "Duplicate PLTE chunk");

   _png_ptr->mode |= PNG_HAVE_PLTE;

   if (!(_png_ptr->color_type&PNG_COLOR_MASK_COLOR))
   {
      _png_warning(_png_ptr,
          "Ignoring PLTE chunk in grayscale PNG");
      _png_crc_finish(_png_ptr, length);
      return;
   }

#ifndef PNG_READ_OPT_PLTE_SUPPORTED
   if (_png_ptr->color_type != PNG_COLOR_TYPE_PALETTE)
   {
      _png_crc_finish(_png_ptr, length);
      return;
   }
#endif

   if (length > 3*PNG_MAX_PALETTE_LENGTH || length % 3)
   {
      if (_png_ptr->color_type != PNG_COLOR_TYPE_PALETTE)
      {
         _png_warning(_png_ptr, "Invalid palette chunk");
         _png_crc_finish(_png_ptr, length);
         return;
      }

      else
      {
         _png_error(_png_ptr, "Invalid palette chunk");
      }
   }

   num = (int)length / 3;

#ifdef PNG_POINTER_INDEXING_SUPPORTED
   for (i = 0, pal_ptr = palette; i < num; i++, pal_ptr++)
   {
      _png_byte buf[3];

      _png_crc_read(_png_ptr, buf, 3);
      pal_ptr->red = buf[0];
      pal_ptr->green = buf[1];
      pal_ptr->blue = buf[2];
   }
#else
   for (i = 0; i < num; i++)
   {
      _png_byte buf[3];

      _png_crc_read(_png_ptr, buf, 3);
      /* Don't depend upon _png_color being any order */
      palette[i].red = buf[0];
      palette[i].green = buf[1];
      palette[i].blue = buf[2];
   }
#endif

   /* If we actually need the PLTE chunk (ie for a paletted image), we do
    * whatever the normal CRC configuration tells us.  However, if we
    * have an RGB image, the PLTE can be considered ancillary, so
    * we will act as though it is.
    */
#ifndef PNG_READ_OPT_PLTE_SUPPORTED
   if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
#endif
   {
      _png_crc_finish(_png_ptr, 0);
   }

#ifndef PNG_READ_OPT_PLTE_SUPPORTED
   else if (_png_crc_error(_png_ptr))  /* Only if we have a CRC error */
   {
      /* If we don't want to use the data from an ancillary chunk,
       * we have two options: an error abort, or a warning and we
       * ignore the data in this chunk (which should be OK, since
       * it's considered ancillary for a RGB or RGBA image).
       */
      if (!(_png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_USE))
      {
         if (_png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_NOWARN)
         {
            _png_chunk_benign_error(_png_ptr, "CRC error");
         }

         else
         {
            _png_chunk_warning(_png_ptr, "CRC error");
            return;
         }
      }

      /* Otherwise, we (optionally) emit a warning and use the chunk. */
      else if (!(_png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_NOWARN))
      {
         _png_chunk_warning(_png_ptr, "CRC error");
      }
   }
#endif

   _png_set_PLTE(_png_ptr, info_ptr, palette, num);

#ifdef PNG_READ_tRNS_SUPPORTED
   if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
   {
      if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_tRNS))
      {
         if (_png_ptr->num_trans > (_png_uint_16)num)
         {
            _png_warning(_png_ptr, "Truncating incorrect tRNS chunk length");
            _png_ptr->num_trans = (_png_uint_16)num;
         }

         if (info_ptr->num_trans > (_png_uint_16)num)
         {
            _png_warning(_png_ptr, "Truncating incorrect info tRNS chunk length");
            info_ptr->num_trans = (_png_uint_16)num;
         }
      }
   }
#endif

}

void /* PRIVATE */
_png_handle_IEND(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_debug(1, "in _png_handle_IEND");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR) || !(_png_ptr->mode & PNG_HAVE_IDAT))
   {
      _png_error(_png_ptr, "No image in file");
   }

   _png_ptr->mode |= (PNG_AFTER_IDAT | PNG_HAVE_IEND);

   if (length != 0)
   {
      _png_warning(_png_ptr, "Incorrect IEND chunk length");
   }

   _png_crc_finish(_png_ptr, length);

   PNG_UNUSED(info_ptr) /* Quiet compiler warnings about unused info_ptr */
}

#ifdef PNG_READ_gAMA_SUPPORTED
void /* PRIVATE */
_png_handle_gAMA(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_fixed_point igamma;
   _png_byte buf[4];

   _png_debug(1, "in _png_handle_gAMA");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before gAMA");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid gAMA after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (_png_ptr->mode & PNG_HAVE_PLTE)
      /* Should be an error, but we can cope with it */
      _png_warning(_png_ptr, "Out of place gAMA chunk");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_gAMA)
#ifdef PNG_READ_sRGB_SUPPORTED
       && !(info_ptr->valid & PNG_INFO_sRGB)
#endif
       )
   {
      _png_warning(_png_ptr, "Duplicate gAMA chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   if (length != 4)
   {
      _png_warning(_png_ptr, "Incorrect gAMA chunk length");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   _png_crc_read(_png_ptr, buf, 4);

   if (_png_crc_finish(_png_ptr, 0))
      return;

   igamma = _png_get_fixed_point(NULL, buf);

   /* Check for zero gamma or an error. */
   if (igamma <= 0)
   {
      _png_warning(_png_ptr,
          "Ignoring gAMA chunk with out of range gamma");

      return;
   }

#  ifdef PNG_READ_sRGB_SUPPORTED
   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_sRGB))
   {
      if (PNG_OUT_OF_RANGE(igamma, 45500, 500))
      {
         PNG_WARNING_PARAMETERS(p)
         _png_warning_parameter_signed(p, 1, PNG_NUMBER_FORMAT_fixed, igamma);
         _png_formatted_warning(_png_ptr, p,
             "Ignoring incorrect gAMA value @1 when sRGB is also present");
         return;
      }
   }
#  endif /* PNG_READ_sRGB_SUPPORTED */

#  ifdef PNG_READ_GAMMA_SUPPORTED
   /* Gamma correction on read is supported. */
   _png_ptr->gamma = igamma;
#  endif
   /* And set the 'info' structure members. */
   _png_set_gAMA_fixed(_png_ptr, info_ptr, igamma);
}
#endif

#ifdef PNG_READ_sBIT_SUPPORTED
void /* PRIVATE */
_png_handle_sBIT(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_size_t truelen;
   _png_byte buf[4];

   _png_debug(1, "in _png_handle_sBIT");

   buf[0] = buf[1] = buf[2] = buf[3] = 0;

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before sBIT");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid sBIT after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (_png_ptr->mode & PNG_HAVE_PLTE)
   {
      /* Should be an error, but we can cope with it */
      _png_warning(_png_ptr, "Out of place sBIT chunk");
   }

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_sBIT))
   {
      _png_warning(_png_ptr, "Duplicate sBIT chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      truelen = 3;

   else
      truelen = (_png_size_t)_png_ptr->channels;

   if (length != truelen || length > 4)
   {
      _png_warning(_png_ptr, "Incorrect sBIT chunk length");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   _png_crc_read(_png_ptr, buf, truelen);

   if (_png_crc_finish(_png_ptr, 0))
      return;

   if (_png_ptr->color_type & PNG_COLOR_MASK_COLOR)
   {
      _png_ptr->sig_bit.red = buf[0];
      _png_ptr->sig_bit.green = buf[1];
      _png_ptr->sig_bit.blue = buf[2];
      _png_ptr->sig_bit.alpha = buf[3];
   }

   else
   {
      _png_ptr->sig_bit.gray = buf[0];
      _png_ptr->sig_bit.red = buf[0];
      _png_ptr->sig_bit.green = buf[0];
      _png_ptr->sig_bit.blue = buf[0];
      _png_ptr->sig_bit.alpha = buf[1];
   }

   _png_set_sBIT(_png_ptr, info_ptr, &(_png_ptr->sig_bit));
}
#endif

#ifdef PNG_READ_cHRM_SUPPORTED
void /* PRIVATE */
_png_handle_cHRM(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_byte buf[32];
   _png_fixed_point x_white, y_white, x_red, y_red, x_green, y_green, x_blue,
      y_blue;

   _png_debug(1, "in _png_handle_cHRM");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before cHRM");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid cHRM after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (_png_ptr->mode & PNG_HAVE_PLTE)
      /* Should be an error, but we can cope with it */
      _png_warning(_png_ptr, "Out of place cHRM chunk");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM)
#  ifdef PNG_READ_sRGB_SUPPORTED
       && !(info_ptr->valid & PNG_INFO_sRGB)
#  endif
      )
   {
      _png_warning(_png_ptr, "Duplicate cHRM chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   if (length != 32)
   {
      _png_warning(_png_ptr, "Incorrect cHRM chunk length");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   _png_crc_read(_png_ptr, buf, 32);

   if (_png_crc_finish(_png_ptr, 0))
      return;

   x_white = _png_get_fixed_point(NULL, buf);
   y_white = _png_get_fixed_point(NULL, buf + 4);
   x_red   = _png_get_fixed_point(NULL, buf + 8);
   y_red   = _png_get_fixed_point(NULL, buf + 12);
   x_green = _png_get_fixed_point(NULL, buf + 16);
   y_green = _png_get_fixed_point(NULL, buf + 20);
   x_blue  = _png_get_fixed_point(NULL, buf + 24);
   y_blue  = _png_get_fixed_point(NULL, buf + 28);

   if (x_white == PNG_FIXED_ERROR ||
       y_white == PNG_FIXED_ERROR ||
       x_red   == PNG_FIXED_ERROR ||
       y_red   == PNG_FIXED_ERROR ||
       x_green == PNG_FIXED_ERROR ||
       y_green == PNG_FIXED_ERROR ||
       x_blue  == PNG_FIXED_ERROR ||
       y_blue  == PNG_FIXED_ERROR)
   {
      _png_warning(_png_ptr, "Ignoring cHRM chunk with negative chromaticities");
      return;
   }

#ifdef PNG_READ_sRGB_SUPPORTED
   if ((info_ptr != NULL) && (info_ptr->valid & PNG_INFO_sRGB))
   {
      if (PNG_OUT_OF_RANGE(x_white, 31270,  1000) ||
          PNG_OUT_OF_RANGE(y_white, 32900,  1000) ||
          PNG_OUT_OF_RANGE(x_red,   64000,  1000) ||
          PNG_OUT_OF_RANGE(y_red,   33000,  1000) ||
          PNG_OUT_OF_RANGE(x_green, 30000,  1000) ||
          PNG_OUT_OF_RANGE(y_green, 60000,  1000) ||
          PNG_OUT_OF_RANGE(x_blue,  15000,  1000) ||
          PNG_OUT_OF_RANGE(y_blue,   6000,  1000))
      {
         PNG_WARNING_PARAMETERS(p)

         _png_warning_parameter_signed(p, 1, PNG_NUMBER_FORMAT_fixed, x_white);
         _png_warning_parameter_signed(p, 2, PNG_NUMBER_FORMAT_fixed, y_white);
         _png_warning_parameter_signed(p, 3, PNG_NUMBER_FORMAT_fixed, x_red);
         _png_warning_parameter_signed(p, 4, PNG_NUMBER_FORMAT_fixed, y_red);
         _png_warning_parameter_signed(p, 5, PNG_NUMBER_FORMAT_fixed, x_green);
         _png_warning_parameter_signed(p, 6, PNG_NUMBER_FORMAT_fixed, y_green);
         _png_warning_parameter_signed(p, 7, PNG_NUMBER_FORMAT_fixed, x_blue);
         _png_warning_parameter_signed(p, 8, PNG_NUMBER_FORMAT_fixed, y_blue);

         _png_formatted_warning(_png_ptr, p,
             "Ignoring incorrect cHRM white(@1,@2) r(@3,@4)g(@5,@6)b(@7,@8) "
             "when sRGB is also present");
      }
      return;
   }
#endif /* PNG_READ_sRGB_SUPPORTED */

#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
   /* Store the _white values as default coefficients for the rgb to gray
    * operation if it is supported.  Check if the transform is already set to
    * avoid destroying the transform values.
    */
   if (!_png_ptr->rgb_to_gray_coefficients_set)
   {
      /* _png_set_background has not been called and we haven't seen an sRGB
       * chunk yet.  Find the XYZ of the three end points.
       */
      _png_XYZ XYZ;
      _png_xy xy;

      xy.redx = x_red;
      xy.redy = y_red;
      xy.greenx = x_green;
      xy.greeny = y_green;
      xy.bluex = x_blue;
      xy.bluey = y_blue;
      xy.whitex = x_white;
      xy.whitey = y_white;

      if (_png_XYZ_from_xy_checked(_png_ptr, &XYZ, xy))
      {
         /* The success case, because XYZ_from_xy normalises to a reference
          * white Y of 1.0 we just need to scale the numbers.  This should
          * always work just fine. It is an internal error if this overflows.
          */
         {
            _png_fixed_point r, g, b;
            if (_png_muldiv(&r, XYZ.redY, 32768, PNG_FP_1) &&
               r >= 0 && r <= 32768 &&
               _png_muldiv(&g, XYZ.greenY, 32768, PNG_FP_1) &&
               g >= 0 && g <= 32768 &&
               _png_muldiv(&b, XYZ.blueY, 32768, PNG_FP_1) &&
               b >= 0 && b <= 32768 &&
               r+g+b <= 32769)
            {
               /* We allow 0 coefficients here.  r+g+b may be 32769 if two or
                * all of the coefficients were rounded up.  Handle this by
                * reducing the *largest* coefficient by 1; this matches the
                * approach used for the default coefficients in pngrtran.c
                */
               int add = 0;

               if (r+g+b > 32768)
                  add = -1;
               else if (r+g+b < 32768)
                  add = 1;

               if (add != 0)
               {
                  if (g >= r && g >= b)
                     g += add;
                  else if (r >= g && r >= b)
                     r += add;
                  else
                     b += add;
               }

               /* Check for an internal error. */
               if (r+g+b != 32768)
                  _png_error(_png_ptr,
                     "internal error handling cHRM coefficients");

               _png_ptr->rgb_to_gray_red_coeff   = (_png_uint_16)r;
               _png_ptr->rgb_to_gray_green_coeff = (_png_uint_16)g;
            }

            /* This is a _png_error at present even though it could be ignored -
             * it should never happen, but it is important that if it does, the
             * bug is fixed.
             */
            else
               _png_error(_png_ptr, "internal error handling cHRM->XYZ");
         }
      }
   }
#endif

   _png_set_cHRM_fixed(_png_ptr, info_ptr, x_white, y_white, x_red, y_red,
      x_green, y_green, x_blue, y_blue);
}
#endif

#ifdef PNG_READ_sRGB_SUPPORTED
void /* PRIVATE */
_png_handle_sRGB(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   int intent;
   _png_byte buf[1];

   _png_debug(1, "in _png_handle_sRGB");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before sRGB");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid sRGB after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (_png_ptr->mode & PNG_HAVE_PLTE)
      /* Should be an error, but we can cope with it */
      _png_warning(_png_ptr, "Out of place sRGB chunk");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_sRGB))
   {
      _png_warning(_png_ptr, "Duplicate sRGB chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   if (length != 1)
   {
      _png_warning(_png_ptr, "Incorrect sRGB chunk length");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   _png_crc_read(_png_ptr, buf, 1);

   if (_png_crc_finish(_png_ptr, 0))
      return;

   intent = buf[0];

   /* Check for bad intent */
   if (intent >= PNG_sRGB_INTENT_LAST)
   {
      _png_warning(_png_ptr, "Unknown sRGB intent");
      return;
   }

#if defined(PNG_READ_gAMA_SUPPORTED) && defined(PNG_READ_GAMMA_SUPPORTED)
   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_gAMA))
   {
      if (PNG_OUT_OF_RANGE(info_ptr->gamma, 45500, 500))
      {
         PNG_WARNING_PARAMETERS(p)

         _png_warning_parameter_signed(p, 1, PNG_NUMBER_FORMAT_fixed,
            info_ptr->gamma);

         _png_formatted_warning(_png_ptr, p,
             "Ignoring incorrect gAMA value @1 when sRGB is also present");
      }
   }
#endif /* PNG_READ_gAMA_SUPPORTED */

#ifdef PNG_READ_cHRM_SUPPORTED
   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM))
      if (PNG_OUT_OF_RANGE(info_ptr->x_white, 31270,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->y_white, 32900,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->x_red,   64000,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->y_red,   33000,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->x_green, 30000,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->y_green, 60000,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->x_blue,  15000,  1000) ||
          PNG_OUT_OF_RANGE(info_ptr->y_blue,   6000,  1000))
      {
         _png_warning(_png_ptr,
             "Ignoring incorrect cHRM value when sRGB is also present");
      }
#endif /* PNG_READ_cHRM_SUPPORTED */

   /* This is recorded for use when handling the cHRM chunk above.  An sRGB
    * chunk unconditionally overwrites the coefficients for grayscale conversion
    * too.
    */
   _png_ptr->is_sRGB = 1;

#  ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
      /* Don't overwrite user supplied values: */
      if (!_png_ptr->rgb_to_gray_coefficients_set)
      {
         /* These numbers come from the sRGB specification (or, since one has to
          * pay much money to get a copy, the wikipedia sRGB page) the
          * chromaticity values quoted have been inverted to get the reverse
          * transformation from RGB to XYZ and the 'Y' coefficients scaled by
          * 32768 (then rounded).
          *
          * sRGB and ITU Rec-709 both truncate the values for the D65 white
          * point to four digits and, even though it actually stores five
          * digits, the PNG spec gives the truncated value.
          *
          * This means that when the chromaticities are converted back to XYZ
          * end points we end up with (6968,23435,2366), which, as described in
          * pngrtran.c, would overflow.  If the five digit precision and up is
          * used we get, instead:
          *
          *    6968*R + 23435*G + 2365*B
          *
          * (Notice that this rounds the blue coefficient down, rather than the
          * choice used in pngrtran.c which is to round the green one down.)
          */
         _png_ptr->rgb_to_gray_red_coeff   =  6968; /* 0.212639005871510 */
         _png_ptr->rgb_to_gray_green_coeff = 23434; /* 0.715168678767756 */
         /* _png_ptr->rgb_to_gray_blue_coeff  =  2366; 0.072192315360734	*/

         /* The following keeps the cHRM chunk from destroying the
          * coefficients again in the event that it follows the sRGB chunk.
          */
         _png_ptr->rgb_to_gray_coefficients_set = 1;
      }
#  endif

   _png_set_sRGB_gAMA_and_cHRM(_png_ptr, info_ptr, intent);
}
#endif /* PNG_READ_sRGB_SUPPORTED */

#ifdef PNG_READ_iCCP_SUPPORTED
void /* PRIVATE */
_png_handle_iCCP(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
/* Note: this does not properly handle chunks that are > 64K under DOS */
{
   _png_byte compression_type;
   _png_bytep pC;
   _png_charp profile;
   _png_uint_32 skip = 0;
   _png_uint_32 profile_size;
   _png_alloc_size_t profile_length;
   _png_size_t slength, prefix_length, data_length;

   _png_debug(1, "in _png_handle_iCCP");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before iCCP");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid iCCP after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (_png_ptr->mode & PNG_HAVE_PLTE)
      /* Should be an error, but we can cope with it */
      _png_warning(_png_ptr, "Out of place iCCP chunk");

   if ((_png_ptr->mode & PNG_HAVE_iCCP) || (info_ptr != NULL &&
      (info_ptr->valid & (PNG_INFO_iCCP|PNG_INFO_sRGB))))
   {
      _png_warning(_png_ptr, "Duplicate iCCP chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   _png_ptr->mode |= PNG_HAVE_iCCP;

#ifdef PNG_MAX_MALLOC_64K
   if (length > (_png_uint_32)65535L)
   {
      _png_warning(_png_ptr, "iCCP chunk too large to fit in memory");
      skip = length - (_png_uint_32)65535L;
      length = (_png_uint_32)65535L;
   }
#endif

   _png_free(_png_ptr, _png_ptr->chunkdata);
   _png_ptr->chunkdata = (_png_charp)_png_malloc(_png_ptr, length + 1);
   slength = length;
   _png_crc_read(_png_ptr, (_png_bytep)_png_ptr->chunkdata, slength);

   if (_png_crc_finish(_png_ptr, skip))
   {
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   _png_ptr->chunkdata[slength] = 0x00;

   for (profile = _png_ptr->chunkdata; *profile; profile++)
      /* Empty loop to find end of name */ ;

   ++profile;

   /* There should be at least one zero (the compression type byte)
    * following the separator, and we should be on it
    */
   if (profile >= _png_ptr->chunkdata + slength - 1)
   {
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      _png_warning(_png_ptr, "Malformed iCCP chunk");
      return;
   }

   /* Compression_type should always be zero */
   compression_type = *profile++;

   if (compression_type)
   {
      _png_warning(_png_ptr, "Ignoring nonzero compression type in iCCP chunk");
      compression_type = 0x00;  /* Reset it to zero (libpng-1.0.6 through 1.0.8
                                 wrote nonzero) */
   }

   prefix_length = profile - _png_ptr->chunkdata;
   _png_decompress_chunk(_png_ptr, compression_type,
       slength, prefix_length, &data_length);

   profile_length = data_length - prefix_length;

   if (prefix_length > data_length || profile_length < 4)
   {
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      _png_warning(_png_ptr, "Profile size field missing from iCCP chunk");
      return;
   }

   /* Check the profile_size recorded in the first 32 bits of the ICC profile */
   pC = (_png_bytep)(_png_ptr->chunkdata + prefix_length);
   profile_size = ((*(pC    )) << 24) |
                  ((*(pC + 1)) << 16) |
                  ((*(pC + 2)) <<  8) |
                  ((*(pC + 3))      );

   /* NOTE: the following guarantees that 'profile_length' fits into 32 bits,
    * because profile_size is a 32 bit value.
    */
   if (profile_size < profile_length)
      profile_length = profile_size;

   /* And the following guarantees that profile_size == profile_length. */
   if (profile_size > profile_length)
   {
      PNG_WARNING_PARAMETERS(p)

      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;

      _png_warning_parameter_unsigned(p, 1, PNG_NUMBER_FORMAT_u, profile_size);
      _png_warning_parameter_unsigned(p, 2, PNG_NUMBER_FORMAT_u, profile_length);
      _png_formatted_warning(_png_ptr, p,
         "Ignoring iCCP chunk with declared size = @1 and actual length = @2");
      return;
   }

   _png_set_iCCP(_png_ptr, info_ptr, _png_ptr->chunkdata,
       compression_type, (_png_bytep)_png_ptr->chunkdata + prefix_length,
       profile_size);
   _png_free(_png_ptr, _png_ptr->chunkdata);
   _png_ptr->chunkdata = NULL;
}
#endif /* PNG_READ_iCCP_SUPPORTED */

#ifdef PNG_READ_sPLT_SUPPORTED
void /* PRIVATE */
_png_handle_sPLT(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
/* Note: this does not properly handle chunks that are > 64K under DOS */
{
   _png_bytep entry_start;
   _png_sPLT_t new_palette;
   _png_sPLT_entryp pp;
   _png_uint_32 data_length;
   int entry_size, i;
   _png_uint_32 skip = 0;
   _png_size_t slength;
   _png_uint_32 dl;
   _png_size_t max_dl;

   _png_debug(1, "in _png_handle_sPLT");

#ifdef PNG_USER_LIMITS_SUPPORTED

   if (_png_ptr->user_chunk_cache_max != 0)
   {
      if (_png_ptr->user_chunk_cache_max == 1)
      {
         _png_crc_finish(_png_ptr, length);
         return;
      }

      if (--_png_ptr->user_chunk_cache_max == 1)
      {
         _png_warning(_png_ptr, "No space in chunk cache for sPLT");
         _png_crc_finish(_png_ptr, length);
         return;
      }
   }
#endif

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before sPLT");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid sPLT after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

#ifdef PNG_MAX_MALLOC_64K
   if (length > (_png_uint_32)65535L)
   {
      _png_warning(_png_ptr, "sPLT chunk too large to fit in memory");
      skip = length - (_png_uint_32)65535L;
      length = (_png_uint_32)65535L;
   }
#endif

   _png_free(_png_ptr, _png_ptr->chunkdata);
   _png_ptr->chunkdata = (_png_charp)_png_malloc(_png_ptr, length + 1);

   /* WARNING: this may break if size_t is less than 32 bits; it is assumed
    * that the PNG_MAX_MALLOC_64K test is enabled in this case, but this is a
    * potential breakage point if the types in pngconf.h aren't exactly right.
    */
   slength = length;
   _png_crc_read(_png_ptr, (_png_bytep)_png_ptr->chunkdata, slength);

   if (_png_crc_finish(_png_ptr, skip))
   {
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   _png_ptr->chunkdata[slength] = 0x00;

   for (entry_start = (_png_bytep)_png_ptr->chunkdata; *entry_start;
       entry_start++)
      /* Empty loop to find end of name */ ;

   ++entry_start;

   /* A sample depth should follow the separator, and we should be on it  */
   if (entry_start > (_png_bytep)_png_ptr->chunkdata + slength - 2)
   {
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      _png_warning(_png_ptr, "malformed sPLT chunk");
      return;
   }

   new_palette.depth = *entry_start++;
   entry_size = (new_palette.depth == 8 ? 6 : 10);
   /* This must fit in a _png_uint_32 because it is derived from the original
    * chunk data length (and use 'length', not 'slength' here for clarity -
    * they are guaranteed to be the same, see the tests above.)
    */
   data_length = length - (_png_uint_32)(entry_start -
      (_png_bytep)_png_ptr->chunkdata);

   /* Integrity-check the data length */
   if (data_length % entry_size)
   {
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      _png_warning(_png_ptr, "sPLT chunk has bad length");
      return;
   }

   dl = (_png_int_32)(data_length / entry_size);
   max_dl = PNG_SIZE_MAX / _png_sizeof(_png_sPLT_entry);

   if (dl > max_dl)
   {
       _png_warning(_png_ptr, "sPLT chunk too long");
       return;
   }

   new_palette.nentries = (_png_int_32)(data_length / entry_size);

   new_palette.entries = (_png_sPLT_entryp)_png_malloc_warn(
       _png_ptr, new_palette.nentries * _png_sizeof(_png_sPLT_entry));

   if (new_palette.entries == NULL)
   {
       _png_warning(_png_ptr, "sPLT chunk requires too much memory");
       return;
   }

#ifdef PNG_POINTER_INDEXING_SUPPORTED
   for (i = 0; i < new_palette.nentries; i++)
   {
      pp = new_palette.entries + i;

      if (new_palette.depth == 8)
      {
         pp->red = *entry_start++;
         pp->green = *entry_start++;
         pp->blue = *entry_start++;
         pp->alpha = *entry_start++;
      }

      else
      {
         pp->red   = _png_get_uint_16(entry_start); entry_start += 2;
         pp->green = _png_get_uint_16(entry_start); entry_start += 2;
         pp->blue  = _png_get_uint_16(entry_start); entry_start += 2;
         pp->alpha = _png_get_uint_16(entry_start); entry_start += 2;
      }

      pp->frequency = _png_get_uint_16(entry_start); entry_start += 2;
   }
#else
   pp = new_palette.entries;

   for (i = 0; i < new_palette.nentries; i++)
   {

      if (new_palette.depth == 8)
      {
         pp[i].red   = *entry_start++;
         pp[i].green = *entry_start++;
         pp[i].blue  = *entry_start++;
         pp[i].alpha = *entry_start++;
      }

      else
      {
         pp[i].red   = _png_get_uint_16(entry_start); entry_start += 2;
         pp[i].green = _png_get_uint_16(entry_start); entry_start += 2;
         pp[i].blue  = _png_get_uint_16(entry_start); entry_start += 2;
         pp[i].alpha = _png_get_uint_16(entry_start); entry_start += 2;
      }

      pp[i].frequency = _png_get_uint_16(entry_start); entry_start += 2;
   }
#endif

   /* Discard all chunk data except the name and stash that */
   new_palette.name = _png_ptr->chunkdata;

   _png_set_sPLT(_png_ptr, info_ptr, &new_palette, 1);

   _png_free(_png_ptr, _png_ptr->chunkdata);
   _png_ptr->chunkdata = NULL;
   _png_free(_png_ptr, new_palette.entries);
}
#endif /* PNG_READ_sPLT_SUPPORTED */

#ifdef PNG_READ_tRNS_SUPPORTED
void /* PRIVATE */
_png_handle_tRNS(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_byte readbuf[PNG_MAX_PALETTE_LENGTH];

   _png_debug(1, "in _png_handle_tRNS");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before tRNS");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid tRNS after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_tRNS))
   {
      _png_warning(_png_ptr, "Duplicate tRNS chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   if (_png_ptr->color_type == PNG_COLOR_TYPE_GRAY)
   {
      _png_byte buf[2];

      if (length != 2)
      {
         _png_warning(_png_ptr, "Incorrect tRNS chunk length");
         _png_crc_finish(_png_ptr, length);
         return;
      }

      _png_crc_read(_png_ptr, buf, 2);
      _png_ptr->num_trans = 1;
      _png_ptr->trans_color.gray = _png_get_uint_16(buf);
   }

   else if (_png_ptr->color_type == PNG_COLOR_TYPE_RGB)
   {
      _png_byte buf[6];

      if (length != 6)
      {
         _png_warning(_png_ptr, "Incorrect tRNS chunk length");
         _png_crc_finish(_png_ptr, length);
         return;
      }

      _png_crc_read(_png_ptr, buf, (_png_size_t)length);
      _png_ptr->num_trans = 1;
      _png_ptr->trans_color.red = _png_get_uint_16(buf);
      _png_ptr->trans_color.green = _png_get_uint_16(buf + 2);
      _png_ptr->trans_color.blue = _png_get_uint_16(buf + 4);
   }

   else if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
   {
      if (!(_png_ptr->mode & PNG_HAVE_PLTE))
      {
         /* Should be an error, but we can cope with it. */
         _png_warning(_png_ptr, "Missing PLTE before tRNS");
      }

      if (length > (_png_uint_32)_png_ptr->num_palette ||
          length > PNG_MAX_PALETTE_LENGTH)
      {
         _png_warning(_png_ptr, "Incorrect tRNS chunk length");
         _png_crc_finish(_png_ptr, length);
         return;
      }

      if (length == 0)
      {
         _png_warning(_png_ptr, "Zero length tRNS chunk");
         _png_crc_finish(_png_ptr, length);
         return;
      }

      _png_crc_read(_png_ptr, readbuf, (_png_size_t)length);
      _png_ptr->num_trans = (_png_uint_16)length;
   }

   else
   {
      _png_warning(_png_ptr, "tRNS chunk not allowed with alpha channel");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   if (_png_crc_finish(_png_ptr, 0))
   {
      _png_ptr->num_trans = 0;
      return;
   }

   _png_set_tRNS(_png_ptr, info_ptr, readbuf, _png_ptr->num_trans,
       &(_png_ptr->trans_color));
}
#endif

#ifdef PNG_READ_bKGD_SUPPORTED
void /* PRIVATE */
_png_handle_bKGD(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_size_t truelen;
   _png_byte buf[6];
   _png_color_16 background;

   _png_debug(1, "in _png_handle_bKGD");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before bKGD");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid bKGD after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE &&
       !(_png_ptr->mode & PNG_HAVE_PLTE))
   {
      _png_warning(_png_ptr, "Missing PLTE before bKGD");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_bKGD))
   {
      _png_warning(_png_ptr, "Duplicate bKGD chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      truelen = 1;

   else if (_png_ptr->color_type & PNG_COLOR_MASK_COLOR)
      truelen = 6;

   else
      truelen = 2;

   if (length != truelen)
   {
      _png_warning(_png_ptr, "Incorrect bKGD chunk length");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   _png_crc_read(_png_ptr, buf, truelen);

   if (_png_crc_finish(_png_ptr, 0))
      return;

   /* We convert the index value into RGB components so that we can allow
    * arbitrary RGB values for background when we have transparency, and
    * so it is easy to determine the RGB values of the background color
    * from the info_ptr struct.
    */
   if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
   {
      background.index = buf[0];

      if (info_ptr && info_ptr->num_palette)
      {
         if (buf[0] >= info_ptr->num_palette)
         {
            _png_warning(_png_ptr, "Incorrect bKGD chunk index value");
            return;
         }

         background.red = (_png_uint_16)_png_ptr->palette[buf[0]].red;
         background.green = (_png_uint_16)_png_ptr->palette[buf[0]].green;
         background.blue = (_png_uint_16)_png_ptr->palette[buf[0]].blue;
      }

      else
         background.red = background.green = background.blue = 0;

      background.gray = 0;
   }

   else if (!(_png_ptr->color_type & PNG_COLOR_MASK_COLOR)) /* GRAY */
   {
      background.index = 0;
      background.red =
      background.green =
      background.blue =
      background.gray = _png_get_uint_16(buf);
   }

   else
   {
      background.index = 0;
      background.red = _png_get_uint_16(buf);
      background.green = _png_get_uint_16(buf + 2);
      background.blue = _png_get_uint_16(buf + 4);
      background.gray = 0;
   }

   _png_set_bKGD(_png_ptr, info_ptr, &background);
}
#endif

#ifdef PNG_READ_hIST_SUPPORTED
void /* PRIVATE */
_png_handle_hIST(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   unsigned int num, i;
   _png_uint_16 readbuf[PNG_MAX_PALETTE_LENGTH];

   _png_debug(1, "in _png_handle_hIST");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before hIST");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid hIST after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (!(_png_ptr->mode & PNG_HAVE_PLTE))
   {
      _png_warning(_png_ptr, "Missing PLTE before hIST");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_hIST))
   {
      _png_warning(_png_ptr, "Duplicate hIST chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   if (length > 2*PNG_MAX_PALETTE_LENGTH ||
       length != (unsigned int) (2*_png_ptr->num_palette))
   {
      _png_warning(_png_ptr, "Incorrect hIST chunk length");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   num = length / 2 ;

   for (i = 0; i < num; i++)
   {
      _png_byte buf[2];

      _png_crc_read(_png_ptr, buf, 2);
      readbuf[i] = _png_get_uint_16(buf);
   }

   if (_png_crc_finish(_png_ptr, 0))
      return;

   _png_set_hIST(_png_ptr, info_ptr, readbuf);
}
#endif

#ifdef PNG_READ_pHYs_SUPPORTED
void /* PRIVATE */
_png_handle_pHYs(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_byte buf[9];
   _png_uint_32 res_x, res_y;
   int unit_type;

   _png_debug(1, "in _png_handle_pHYs");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before pHYs");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid pHYs after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
   {
      _png_warning(_png_ptr, "Duplicate pHYs chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   if (length != 9)
   {
      _png_warning(_png_ptr, "Incorrect pHYs chunk length");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   _png_crc_read(_png_ptr, buf, 9);

   if (_png_crc_finish(_png_ptr, 0))
      return;

   res_x = _png_get_uint_32(buf);
   res_y = _png_get_uint_32(buf + 4);
   unit_type = buf[8];
   _png_set_pHYs(_png_ptr, info_ptr, res_x, res_y, unit_type);
}
#endif

#ifdef PNG_READ_oFFs_SUPPORTED
void /* PRIVATE */
_png_handle_oFFs(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_byte buf[9];
   _png_int_32 offset_x, offset_y;
   int unit_type;

   _png_debug(1, "in _png_handle_oFFs");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before oFFs");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid oFFs after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_oFFs))
   {
      _png_warning(_png_ptr, "Duplicate oFFs chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   if (length != 9)
   {
      _png_warning(_png_ptr, "Incorrect oFFs chunk length");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   _png_crc_read(_png_ptr, buf, 9);

   if (_png_crc_finish(_png_ptr, 0))
      return;

   offset_x = _png_get_int_32(buf);
   offset_y = _png_get_int_32(buf + 4);
   unit_type = buf[8];
   _png_set_oFFs(_png_ptr, info_ptr, offset_x, offset_y, unit_type);
}
#endif

#ifdef PNG_READ_pCAL_SUPPORTED
/* Read the pCAL chunk (described in the PNG Extensions document) */
void /* PRIVATE */
_png_handle_pCAL(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_int_32 X0, X1;
   _png_byte type, nparams;
   _png_charp buf, units, endptr;
   _png_charpp params;
   _png_size_t slength;
   int i;

   _png_debug(1, "in _png_handle_pCAL");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before pCAL");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid pCAL after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_pCAL))
   {
      _png_warning(_png_ptr, "Duplicate pCAL chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   _png_debug1(2, "Allocating and reading pCAL chunk data (%u bytes)",
       length + 1);
   _png_free(_png_ptr, _png_ptr->chunkdata);
   _png_ptr->chunkdata = (_png_charp)_png_malloc_warn(_png_ptr, length + 1);

   if (_png_ptr->chunkdata == NULL)
   {
      _png_warning(_png_ptr, "No memory for pCAL purpose");
      return;
   }

   slength = length;
   _png_crc_read(_png_ptr, (_png_bytep)_png_ptr->chunkdata, slength);

   if (_png_crc_finish(_png_ptr, 0))
   {
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   _png_ptr->chunkdata[slength] = 0x00; /* Null terminate the last string */

   _png_debug(3, "Finding end of pCAL purpose string");
   for (buf = _png_ptr->chunkdata; *buf; buf++)
      /* Empty loop */ ;

   endptr = _png_ptr->chunkdata + slength;

   /* We need to have at least 12 bytes after the purpose string
    * in order to get the parameter information.
    */
   if (endptr <= buf + 12)
   {
      _png_warning(_png_ptr, "Invalid pCAL data");
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   _png_debug(3, "Reading pCAL X0, X1, type, nparams, and units");
   X0 = _png_get_int_32((_png_bytep)buf+1);
   X1 = _png_get_int_32((_png_bytep)buf+5);
   type = buf[9];
   nparams = buf[10];
   units = buf + 11;

   _png_debug(3, "Checking pCAL equation type and number of parameters");
   /* Check that we have the right number of parameters for known
    * equation types.
    */
   if ((type == PNG_EQUATION_LINEAR && nparams != 2) ||
       (type == PNG_EQUATION_BASE_E && nparams != 3) ||
       (type == PNG_EQUATION_ARBITRARY && nparams != 3) ||
       (type == PNG_EQUATION_HYPERBOLIC && nparams != 4))
   {
      _png_warning(_png_ptr, "Invalid pCAL parameters for equation type");
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   else if (type >= PNG_EQUATION_LAST)
   {
      _png_warning(_png_ptr, "Unrecognized equation type for pCAL chunk");
   }

   for (buf = units; *buf; buf++)
      /* Empty loop to move past the units string. */ ;

   _png_debug(3, "Allocating pCAL parameters array");

   params = (_png_charpp)_png_malloc_warn(_png_ptr,
       (_png_size_t)(nparams * _png_sizeof(_png_charp)));

   if (params == NULL)
   {
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      _png_warning(_png_ptr, "No memory for pCAL params");
      return;
   }

   /* Get pointers to the start of each parameter string. */
   for (i = 0; i < (int)nparams; i++)
   {
      buf++; /* Skip the null string terminator from previous parameter. */

      _png_debug1(3, "Reading pCAL parameter %d", i);

      for (params[i] = buf; buf <= endptr && *buf != 0x00; buf++)
         /* Empty loop to move past each parameter string */ ;

      /* Make sure we haven't run out of data yet */
      if (buf > endptr)
      {
         _png_warning(_png_ptr, "Invalid pCAL data");
         _png_free(_png_ptr, _png_ptr->chunkdata);
         _png_ptr->chunkdata = NULL;
         _png_free(_png_ptr, params);
         return;
      }
   }

   _png_set_pCAL(_png_ptr, info_ptr, _png_ptr->chunkdata, X0, X1, type, nparams,
      units, params);

   _png_free(_png_ptr, _png_ptr->chunkdata);
   _png_ptr->chunkdata = NULL;
   _png_free(_png_ptr, params);
}
#endif

#ifdef PNG_READ_sCAL_SUPPORTED
/* Read the sCAL chunk */
void /* PRIVATE */
_png_handle_sCAL(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_size_t slength, i;
   int state;

   _png_debug(1, "in _png_handle_sCAL");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before sCAL");

   else if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      _png_warning(_png_ptr, "Invalid sCAL after IDAT");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_sCAL))
   {
      _png_warning(_png_ptr, "Duplicate sCAL chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   /* Need unit type, width, \0, height: minimum 4 bytes */
   else if (length < 4)
   {
      _png_warning(_png_ptr, "sCAL chunk too short");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   _png_debug1(2, "Allocating and reading sCAL chunk data (%u bytes)",
      length + 1);

   _png_ptr->chunkdata = (_png_charp)_png_malloc_warn(_png_ptr, length + 1);

   if (_png_ptr->chunkdata == NULL)
   {
      _png_warning(_png_ptr, "Out of memory while processing sCAL chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   slength = length;
   _png_crc_read(_png_ptr, (_png_bytep)_png_ptr->chunkdata, slength);
   _png_ptr->chunkdata[slength] = 0x00; /* Null terminate the last string */

   if (_png_crc_finish(_png_ptr, 0))
   {
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   /* Validate the unit. */
   if (_png_ptr->chunkdata[0] != 1 && _png_ptr->chunkdata[0] != 2)
   {
      _png_warning(_png_ptr, "Invalid sCAL ignored: invalid unit");
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   /* Validate the ASCII numbers, need two ASCII numbers separated by
    * a '\0' and they need to fit exactly in the chunk data.
    */
   i = 1;
   state = 0;

   if (!_png_check_fp_number(_png_ptr->chunkdata, slength, &state, &i) ||
       i >= slength || _png_ptr->chunkdata[i++] != 0)
      _png_warning(_png_ptr, "Invalid sCAL chunk ignored: bad width format");

   else if (!PNG_FP_IS_POSITIVE(state))
      _png_warning(_png_ptr, "Invalid sCAL chunk ignored: non-positive width");

   else
   {
      _png_size_t heighti = i;

      state = 0;
      if (!_png_check_fp_number(_png_ptr->chunkdata, slength, &state, &i) ||
          i != slength)
         _png_warning(_png_ptr, "Invalid sCAL chunk ignored: bad height format");

      else if (!PNG_FP_IS_POSITIVE(state))
         _png_warning(_png_ptr,
            "Invalid sCAL chunk ignored: non-positive height");

      else
         /* This is the (only) success case. */
         _png_set_sCAL_s(_png_ptr, info_ptr, _png_ptr->chunkdata[0],
            _png_ptr->chunkdata+1, _png_ptr->chunkdata+heighti);
   }

   /* Clean up - just free the temporarily allocated buffer. */
   _png_free(_png_ptr, _png_ptr->chunkdata);
   _png_ptr->chunkdata = NULL;
}
#endif

#ifdef PNG_READ_tIME_SUPPORTED
void /* PRIVATE */
_png_handle_tIME(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_byte buf[7];
   _png_time mod_time;

   _png_debug(1, "in _png_handle_tIME");

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Out of place tIME chunk");

   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_tIME))
   {
      _png_warning(_png_ptr, "Duplicate tIME chunk");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   if (_png_ptr->mode & PNG_HAVE_IDAT)
      _png_ptr->mode |= PNG_AFTER_IDAT;

   if (length != 7)
   {
      _png_warning(_png_ptr, "Incorrect tIME chunk length");
      _png_crc_finish(_png_ptr, length);
      return;
   }

   _png_crc_read(_png_ptr, buf, 7);

   if (_png_crc_finish(_png_ptr, 0))
      return;

   mod_time.second = buf[6];
   mod_time.minute = buf[5];
   mod_time.hour = buf[4];
   mod_time.day = buf[3];
   mod_time.month = buf[2];
   mod_time.year = _png_get_uint_16(buf);

   _png_set_tIME(_png_ptr, info_ptr, &mod_time);
}
#endif

#ifdef PNG_READ_tEXt_SUPPORTED
/* Note: this does not properly handle chunks that are > 64K under DOS */
void /* PRIVATE */
_png_handle_tEXt(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_textp text_ptr;
   _png_charp key;
   _png_charp text;
   _png_uint_32 skip = 0;
   _png_size_t slength;
   int ret;

   _png_debug(1, "in _png_handle_tEXt");

#ifdef PNG_USER_LIMITS_SUPPORTED
   if (_png_ptr->user_chunk_cache_max != 0)
   {
      if (_png_ptr->user_chunk_cache_max == 1)
      {
         _png_crc_finish(_png_ptr, length);
         return;
      }

      if (--_png_ptr->user_chunk_cache_max == 1)
      {
         _png_warning(_png_ptr, "No space in chunk cache for tEXt");
         _png_crc_finish(_png_ptr, length);
         return;
      }
   }
#endif

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before tEXt");

   if (_png_ptr->mode & PNG_HAVE_IDAT)
      _png_ptr->mode |= PNG_AFTER_IDAT;

#ifdef PNG_MAX_MALLOC_64K
   if (length > (_png_uint_32)65535L)
   {
      _png_warning(_png_ptr, "tEXt chunk too large to fit in memory");
      skip = length - (_png_uint_32)65535L;
      length = (_png_uint_32)65535L;
   }
#endif

   _png_free(_png_ptr, _png_ptr->chunkdata);

   _png_ptr->chunkdata = (_png_charp)_png_malloc_warn(_png_ptr, length + 1);

   if (_png_ptr->chunkdata == NULL)
   {
     _png_warning(_png_ptr, "No memory to process text chunk");
     return;
   }

   slength = length;
   _png_crc_read(_png_ptr, (_png_bytep)_png_ptr->chunkdata, slength);

   if (_png_crc_finish(_png_ptr, skip))
   {
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   key = _png_ptr->chunkdata;

   key[slength] = 0x00;

   for (text = key; *text; text++)
      /* Empty loop to find end of key */ ;

   if (text != key + slength)
      text++;

   text_ptr = (_png_textp)_png_malloc_warn(_png_ptr,
       _png_sizeof(_png_text));

   if (text_ptr == NULL)
   {
      _png_warning(_png_ptr, "Not enough memory to process text chunk");
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   text_ptr->compression = PNG_TEXT_COMPRESSION_NONE;
   text_ptr->key = key;
   text_ptr->lang = NULL;
   text_ptr->lang_key = NULL;
   text_ptr->itxt_length = 0;
   text_ptr->text = text;
   text_ptr->text_length = _png_strlen(text);

   ret = _png_set_text_2(_png_ptr, info_ptr, text_ptr, 1);

   _png_free(_png_ptr, _png_ptr->chunkdata);
   _png_ptr->chunkdata = NULL;
   _png_free(_png_ptr, text_ptr);

   if (ret)
      _png_warning(_png_ptr, "Insufficient memory to process text chunk");
}
#endif

#ifdef PNG_READ_zTXt_SUPPORTED
/* Note: this does not correctly handle chunks that are > 64K under DOS */
void /* PRIVATE */
_png_handle_zTXt(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_textp text_ptr;
   _png_charp text;
   int comp_type;
   int ret;
   _png_size_t slength, prefix_len, data_len;

   _png_debug(1, "in _png_handle_zTXt");

#ifdef PNG_USER_LIMITS_SUPPORTED
   if (_png_ptr->user_chunk_cache_max != 0)
   {
      if (_png_ptr->user_chunk_cache_max == 1)
      {
         _png_crc_finish(_png_ptr, length);
         return;
      }

      if (--_png_ptr->user_chunk_cache_max == 1)
      {
         _png_warning(_png_ptr, "No space in chunk cache for zTXt");
         _png_crc_finish(_png_ptr, length);
         return;
      }
   }
#endif

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before zTXt");

   if (_png_ptr->mode & PNG_HAVE_IDAT)
      _png_ptr->mode |= PNG_AFTER_IDAT;

#ifdef PNG_MAX_MALLOC_64K
   /* We will no doubt have problems with chunks even half this size, but
    * there is no hard and fast rule to tell us where to stop.
    */
   if (length > (_png_uint_32)65535L)
   {
      _png_warning(_png_ptr, "zTXt chunk too large to fit in memory");
      _png_crc_finish(_png_ptr, length);
      return;
   }
#endif

   _png_free(_png_ptr, _png_ptr->chunkdata);
   _png_ptr->chunkdata = (_png_charp)_png_malloc_warn(_png_ptr, length + 1);

   if (_png_ptr->chunkdata == NULL)
   {
      _png_warning(_png_ptr, "Out of memory processing zTXt chunk");
      return;
   }

   slength = length;
   _png_crc_read(_png_ptr, (_png_bytep)_png_ptr->chunkdata, slength);

   if (_png_crc_finish(_png_ptr, 0))
   {
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   _png_ptr->chunkdata[slength] = 0x00;

   for (text = _png_ptr->chunkdata; *text; text++)
      /* Empty loop */ ;

   /* zTXt must have some text after the chunkdataword */
   if (text >= _png_ptr->chunkdata + slength - 2)
   {
      _png_warning(_png_ptr, "Truncated zTXt chunk");
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   else
   {
       comp_type = *(++text);

       if (comp_type != PNG_TEXT_COMPRESSION_zTXt)
       {
          _png_warning(_png_ptr, "Unknown compression type in zTXt chunk");
          comp_type = PNG_TEXT_COMPRESSION_zTXt;
       }

       text++;        /* Skip the compression_method byte */
   }

   prefix_len = text - _png_ptr->chunkdata;

   _png_decompress_chunk(_png_ptr, comp_type,
       (_png_size_t)length, prefix_len, &data_len);

   text_ptr = (_png_textp)_png_malloc_warn(_png_ptr,
       _png_sizeof(_png_text));

   if (text_ptr == NULL)
   {
      _png_warning(_png_ptr, "Not enough memory to process zTXt chunk");
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   text_ptr->compression = comp_type;
   text_ptr->key = _png_ptr->chunkdata;
   text_ptr->lang = NULL;
   text_ptr->lang_key = NULL;
   text_ptr->itxt_length = 0;
   text_ptr->text = _png_ptr->chunkdata + prefix_len;
   text_ptr->text_length = data_len;

   ret = _png_set_text_2(_png_ptr, info_ptr, text_ptr, 1);

   _png_free(_png_ptr, text_ptr);
   _png_free(_png_ptr, _png_ptr->chunkdata);
   _png_ptr->chunkdata = NULL;

   if (ret)
      _png_error(_png_ptr, "Insufficient memory to store zTXt chunk");
}
#endif

#ifdef PNG_READ_iTXt_SUPPORTED
/* Note: this does not correctly handle chunks that are > 64K under DOS */
void /* PRIVATE */
_png_handle_iTXt(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_textp text_ptr;
   _png_charp key, lang, text, lang_key;
   int comp_flag;
   int comp_type;
   int ret;
   _png_size_t slength, prefix_len, data_len;

   _png_debug(1, "in _png_handle_iTXt");

#ifdef PNG_USER_LIMITS_SUPPORTED
   if (_png_ptr->user_chunk_cache_max != 0)
   {
      if (_png_ptr->user_chunk_cache_max == 1)
      {
         _png_crc_finish(_png_ptr, length);
         return;
      }

      if (--_png_ptr->user_chunk_cache_max == 1)
      {
         _png_warning(_png_ptr, "No space in chunk cache for iTXt");
         _png_crc_finish(_png_ptr, length);
         return;
      }
   }
#endif

   if (!(_png_ptr->mode & PNG_HAVE_IHDR))
      _png_error(_png_ptr, "Missing IHDR before iTXt");

   if (_png_ptr->mode & PNG_HAVE_IDAT)
      _png_ptr->mode |= PNG_AFTER_IDAT;

#ifdef PNG_MAX_MALLOC_64K
   /* We will no doubt have problems with chunks even half this size, but
    * there is no hard and fast rule to tell us where to stop.
    */
   if (length > (_png_uint_32)65535L)
   {
      _png_warning(_png_ptr, "iTXt chunk too large to fit in memory");
      _png_crc_finish(_png_ptr, length);
      return;
   }
#endif

   _png_free(_png_ptr, _png_ptr->chunkdata);
   _png_ptr->chunkdata = (_png_charp)_png_malloc_warn(_png_ptr, length + 1);

   if (_png_ptr->chunkdata == NULL)
   {
      _png_warning(_png_ptr, "No memory to process iTXt chunk");
      return;
   }

   slength = length;
   _png_crc_read(_png_ptr, (_png_bytep)_png_ptr->chunkdata, slength);

   if (_png_crc_finish(_png_ptr, 0))
   {
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   _png_ptr->chunkdata[slength] = 0x00;

   for (lang = _png_ptr->chunkdata; *lang; lang++)
      /* Empty loop */ ;

   lang++;        /* Skip NUL separator */

   /* iTXt must have a language tag (possibly empty), two compression bytes,
    * translated keyword (possibly empty), and possibly some text after the
    * keyword
    */

   if (lang >= _png_ptr->chunkdata + slength - 3)
   {
      _png_warning(_png_ptr, "Truncated iTXt chunk");
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   comp_flag = *lang++;
   comp_type = *lang++;

   /* 1.5.14: The spec says "for uncompressed text decoders shall ignore [the
    * compression type]".  The compression flag shall be 0 (no compression) or
    * 1 (compressed with method 0 - deflate.)
    */
   if (comp_flag != 0 && comp_flag != 1)
   {
      _png_warning(_png_ptr, "invalid iTXt compression flag");
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   if (comp_flag/*compressed*/ && comp_type != 0)
   {
      _png_warning(_png_ptr, "unknown iTXt compression type");
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   for (lang_key = lang; *lang_key; lang_key++)
      /* Empty loop */ ;

   lang_key++;        /* Skip NUL separator */

   if (lang_key >= _png_ptr->chunkdata + slength)
   {
      _png_warning(_png_ptr, "Truncated iTXt chunk");
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   for (text = lang_key; *text; text++)
      /* Empty loop */ ;

   text++;        /* Skip NUL separator */

   if (text >= _png_ptr->chunkdata + slength)
   {
      _png_warning(_png_ptr, "Malformed iTXt chunk");
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   prefix_len = text - _png_ptr->chunkdata;

   key=_png_ptr->chunkdata;

   if (comp_flag/*compressed*/)
      _png_decompress_chunk(_png_ptr, comp_type,
          (size_t)length, prefix_len, &data_len);

   else
      data_len = _png_strlen(_png_ptr->chunkdata + prefix_len);

   text_ptr = (_png_textp)_png_malloc_warn(_png_ptr,
       _png_sizeof(_png_text));

   if (text_ptr == NULL)
   {
      _png_warning(_png_ptr, "Not enough memory to process iTXt chunk");
      _png_free(_png_ptr, _png_ptr->chunkdata);
      _png_ptr->chunkdata = NULL;
      return;
   }

   text_ptr->compression =
      (comp_flag ? PNG_ITXT_COMPRESSION_zTXt : PNG_ITXT_COMPRESSION_NONE);
   text_ptr->lang_key = _png_ptr->chunkdata + (lang_key - key);
   text_ptr->lang = _png_ptr->chunkdata + (lang - key);
   text_ptr->itxt_length = data_len;
   text_ptr->text_length = 0;
   text_ptr->key = _png_ptr->chunkdata;
   text_ptr->text = _png_ptr->chunkdata + prefix_len;

   ret = _png_set_text_2(_png_ptr, info_ptr, text_ptr, 1);

   _png_free(_png_ptr, text_ptr);
   _png_free(_png_ptr, _png_ptr->chunkdata);
   _png_ptr->chunkdata = NULL;

   if (ret)
      _png_error(_png_ptr, "Insufficient memory to store iTXt chunk");
}
#endif

/* This function is called when we haven't found a handler for a
 * chunk.  If there isn't a problem with the chunk itself (ie bad
 * chunk name, CRC, or a critical chunk), the chunk is silently ignored
 * -- unless the PNG_FLAG_UNKNOWN_CHUNKS_SUPPORTED flag is on in which
 * case it will be saved away to be written out later.
 */
void /* PRIVATE */
_png_handle_unknown(_png_structp _png_ptr, _png_infop info_ptr, _png_uint_32 length)
{
   _png_uint_32 skip = 0;

   _png_debug(1, "in _png_handle_unknown");

#ifdef PNG_USER_LIMITS_SUPPORTED
   if (_png_ptr->user_chunk_cache_max != 0)
   {
      if (_png_ptr->user_chunk_cache_max == 1)
      {
         _png_crc_finish(_png_ptr, length);
         return;
      }

      if (--_png_ptr->user_chunk_cache_max == 1)
      {
         _png_warning(_png_ptr, "No space in chunk cache for unknown chunk");
         _png_crc_finish(_png_ptr, length);
         return;
      }
   }
#endif

   if (_png_ptr->mode & PNG_HAVE_IDAT)
   {
      if (_png_ptr->chunk_name != _png_IDAT)
         _png_ptr->mode |= PNG_AFTER_IDAT;
   }

   if (PNG_CHUNK_CRITICAL(_png_ptr->chunk_name))
   {
#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
      if (_png_chunk_unknown_handling(_png_ptr, _png_ptr->chunk_name) !=
          PNG_HANDLE_CHUNK_ALWAYS
#ifdef PNG_READ_USER_CHUNKS_SUPPORTED
          && _png_ptr->read_user_chunk_fn == NULL
#endif
          )
#endif
         _png_chunk_error(_png_ptr, "unknown critical chunk");
   }

#ifdef PNG_READ_UNKNOWN_CHUNKS_SUPPORTED
   if ((_png_ptr->flags & PNG_FLAG_KEEP_UNKNOWN_CHUNKS)
#ifdef PNG_READ_USER_CHUNKS_SUPPORTED
       || (_png_ptr->read_user_chunk_fn != NULL)
#endif
       )
   {
#ifdef PNG_MAX_MALLOC_64K
      if (length > 65535)
      {
         _png_warning(_png_ptr, "unknown chunk too large to fit in memory");
         skip = length - 65535;
         length = 65535;
      }
#endif

      /* TODO: this code is very close to the unknown handling in pngpread.c,
       * maybe it can be put into a common utility routine?
       * _png_struct::unknown_chunk is just used as a temporary variable, along
       * with the data into which the chunk is read.  These can be eliminated.
       */
      PNG_CSTRING_FROM_CHUNK(_png_ptr->unknown_chunk.name, _png_ptr->chunk_name);
      _png_ptr->unknown_chunk.size = (_png_size_t)length;

      if (length == 0)
         _png_ptr->unknown_chunk.data = NULL;

      else
      {
         _png_ptr->unknown_chunk.data = (_png_bytep)_png_malloc(_png_ptr, length);
         _png_crc_read(_png_ptr, _png_ptr->unknown_chunk.data, length);
      }

#ifdef PNG_READ_USER_CHUNKS_SUPPORTED
      if (_png_ptr->read_user_chunk_fn != NULL)
      {
         /* Callback to user unknown chunk handler */
         int ret;

         ret = (*(_png_ptr->read_user_chunk_fn))
             (_png_ptr, &_png_ptr->unknown_chunk);

         if (ret < 0)
            _png_chunk_error(_png_ptr, "error in user chunk");

         if (ret == 0)
         {
            if (PNG_CHUNK_CRITICAL(_png_ptr->chunk_name))
            {
#ifdef PNG_HANDLE_AS_UNKNOWN_SUPPORTED
               if (_png_chunk_unknown_handling(_png_ptr, _png_ptr->chunk_name) !=
                   PNG_HANDLE_CHUNK_ALWAYS)
#endif
                  _png_chunk_error(_png_ptr, "unknown critical chunk");
            }

            _png_set_unknown_chunks(_png_ptr, info_ptr,
                &_png_ptr->unknown_chunk, 1);
         }
      }

      else
#endif
         _png_set_unknown_chunks(_png_ptr, info_ptr, &_png_ptr->unknown_chunk, 1);

      _png_free(_png_ptr, _png_ptr->unknown_chunk.data);
      _png_ptr->unknown_chunk.data = NULL;
   }

   else
#endif
      skip = length;

   _png_crc_finish(_png_ptr, skip);

#ifndef PNG_READ_USER_CHUNKS_SUPPORTED
   PNG_UNUSED(info_ptr) /* Quiet compiler warnings about unused info_ptr */
#endif
}

/* This function is called to verify that a chunk name is valid.
 * This function can't have the "critical chunk check" incorporated
 * into it, since in the future we will need to be able to call user
 * functions to handle unknown critical chunks after we check that
 * the chunk name itself is valid.
 */

/* Bit hacking: the test for an invalid byte in the 4 byte chunk name is:
 *
 * ((c) < 65 || (c) > 122 || ((c) > 90 && (c) < 97))
 */

void /* PRIVATE */
_png_check_chunk_name(_png_structp _png_ptr, _png_uint_32 chunk_name)
{
   int i;

   _png_debug(1, "in _png_check_chunk_name");

   for (i=1; i<=4; ++i)
   {
      int c = chunk_name & 0xff;

      if (c < 65 || c > 122 || (c > 90 && c < 97))
         _png_chunk_error(_png_ptr, "invalid chunk type");

      chunk_name >>= 8;
   }
}

/* Combines the row recently read in with the existing pixels in the row.  This
 * routine takes care of alpha and transparency if requested.  This routine also
 * handles the two methods of progressive display of interlaced images,
 * depending on the 'display' value; if 'display' is true then the whole row
 * (dp) is filled from the start by replicating the available pixels.  If
 * 'display' is false only those pixels present in the pass are filled in.
 */
void /* PRIVATE */
_png_combine_row(_png_structp _png_ptr, _png_bytep dp, int display)
{
   unsigned int pixel_depth = _png_ptr->transformed_pixel_depth;
   _png_const_bytep sp = _png_ptr->row_buf + 1;
   _png_uint_32 row_width = _png_ptr->width;
   unsigned int pass = _png_ptr->pass;
   _png_bytep end_ptr = 0;
   _png_byte end_byte = 0;
   unsigned int end_mask;

   _png_debug(1, "in _png_combine_row");

   /* Added in 1.5.6: it should not be possible to enter this routine until at
    * least one row has been read from the PNG data and transformed.
    */
   if (pixel_depth == 0)
      _png_error(_png_ptr, "internal row logic error");

   /* Added in 1.5.4: the pixel depth should match the information returned by
    * any call to _png_read_update_info at this point.  Do not continue if we got
    * this wrong.
    */
   if (_png_ptr->info_rowbytes != 0 && _png_ptr->info_rowbytes !=
          PNG_ROWBYTES(pixel_depth, row_width))
      _png_error(_png_ptr, "internal row size calculation error");

   /* Don't expect this to ever happen: */
   if (row_width == 0)
      _png_error(_png_ptr, "internal row width error");

   /* Preserve the last byte in cases where only part of it will be overwritten,
    * the multiply below may overflow, we don't care because ANSI-C guarantees
    * we get the low bits.
    */
   end_mask = (pixel_depth * row_width) & 7;
   if (end_mask != 0)
   {
      /* end_ptr == NULL is a flag to say do nothing */
      end_ptr = dp + PNG_ROWBYTES(pixel_depth, row_width) - 1;
      end_byte = *end_ptr;
#     ifdef PNG_READ_PACKSWAP_SUPPORTED
         if (_png_ptr->transformations & PNG_PACKSWAP) /* little-endian byte */
            end_mask = 0xff << end_mask;

         else /* big-endian byte */
#     endif
         end_mask = 0xff >> end_mask;
      /* end_mask is now the bits to *keep* from the destination row */
   }

   /* For non-interlaced images this reduces to a _png_memcpy(). A _png_memcpy()
    * will also happen if interlacing isn't supported or if the application
    * does not call _png_set_interlace_handling().  In the latter cases the
    * caller just gets a sequence of the unexpanded rows from each interlace
    * pass.
    */
#ifdef PNG_READ_INTERLACING_SUPPORTED
   if (_png_ptr->interlaced && (_png_ptr->transformations & PNG_INTERLACE) &&
      pass < 6 && (display == 0 ||
      /* The following copies everything for 'display' on passes 0, 2 and 4. */
      (display == 1 && (pass & 1) != 0)))
   {
      /* Narrow images may have no bits in a pass; the caller should handle
       * this, but this test is cheap:
       */
      if (row_width <= PNG_PASS_START_COL(pass))
         return;

      if (pixel_depth < 8)
      {
         /* For pixel depths up to 4 bpp the 8-pixel mask can be expanded to fit
          * into 32 bits, then a single loop over the bytes using the four byte
          * values in the 32-bit mask can be used.  For the 'display' option the
          * expanded mask may also not require any masking within a byte.  To
          * make this work the PACKSWAP option must be taken into account - it
          * simply requires the pixels to be reversed in each byte.
          *
          * The 'regular' case requires a mask for each of the first 6 passes,
          * the 'display' case does a copy for the even passes in the range
          * 0..6.  This has already been handled in the test above.
          *
          * The masks are arranged as four bytes with the first byte to use in
          * the lowest bits (little-endian) regardless of the order (PACKSWAP or
          * not) of the pixels in each byte.
          *
          * NOTE: the whole of this logic depends on the caller of this function
          * only calling it on rows appropriate to the pass.  This function only
          * understands the 'x' logic; the 'y' logic is handled by the caller.
          *
          * The following defines allow generation of compile time constant bit
          * masks for each pixel depth and each possibility of swapped or not
          * swapped bytes.  Pass 'p' is in the range 0..6; 'x', a pixel index,
          * is in the range 0..7; and the result is 1 if the pixel is to be
          * copied in the pass, 0 if not.  'S' is for the sparkle method, 'B'
          * for the block method.
          *
          * With some compilers a compile time expression of the general form:
          *
          *    (shift >= 32) ? (a >> (shift-32)) : (b >> shift)
          *
          * Produces warnings with values of 'shift' in the range 33 to 63
          * because the right hand side of the ?: expression is evaluated by
          * the compiler even though it isn't used.  Microsoft Visual C (various
          * versions) and the Intel C compiler are known to do this.  To avoid
          * this the following macros are used in 1.5.6.  This is a temporary
          * solution to avoid destabilizing the code during the release process.
          */
#        if PNG_USE_COMPILE_TIME_MASKS
#           define PNG_LSR(x,s) ((x)>>((s) & 0x1f))
#           define PNG_LSL(x,s) ((x)<<((s) & 0x1f))
#        else
#           define PNG_LSR(x,s) ((x)>>(s))
#           define PNG_LSL(x,s) ((x)<<(s))
#        endif
#        define S_COPY(p,x) (((p)<4 ? PNG_LSR(0x80088822,(3-(p))*8+(7-(x))) :\
           PNG_LSR(0xaa55ff00,(7-(p))*8+(7-(x)))) & 1)
#        define B_COPY(p,x) (((p)<4 ? PNG_LSR(0xff0fff33,(3-(p))*8+(7-(x))) :\
           PNG_LSR(0xff55ff00,(7-(p))*8+(7-(x)))) & 1)

         /* Return a mask for pass 'p' pixel 'x' at depth 'd'.  The mask is
          * little endian - the first pixel is at bit 0 - however the extra
          * parameter 's' can be set to cause the mask position to be swapped
          * within each byte, to match the PNG format.  This is done by XOR of
          * the shift with 7, 6 or 4 for bit depths 1, 2 and 4.
          */
#        define PIXEL_MASK(p,x,d,s) \
            (PNG_LSL(((PNG_LSL(1U,(d)))-1),(((x)*(d))^((s)?8-(d):0))))

         /* Hence generate the appropriate 'block' or 'sparkle' pixel copy mask.
          */
#        define S_MASKx(p,x,d,s) (S_COPY(p,x)?PIXEL_MASK(p,x,d,s):0)
#        define B_MASKx(p,x,d,s) (B_COPY(p,x)?PIXEL_MASK(p,x,d,s):0)

         /* Combine 8 of these to get the full mask.  For the 1-bpp and 2-bpp
          * cases the result needs replicating, for the 4-bpp case the above
          * generates a full 32 bits.
          */
#        define MASK_EXPAND(m,d) ((m)*((d)==1?0x01010101:((d)==2?0x00010001:1)))

#        define S_MASK(p,d,s) MASK_EXPAND(S_MASKx(p,0,d,s) + S_MASKx(p,1,d,s) +\
            S_MASKx(p,2,d,s) + S_MASKx(p,3,d,s) + S_MASKx(p,4,d,s) +\
            S_MASKx(p,5,d,s) + S_MASKx(p,6,d,s) + S_MASKx(p,7,d,s), d)

#        define B_MASK(p,d,s) MASK_EXPAND(B_MASKx(p,0,d,s) + B_MASKx(p,1,d,s) +\
            B_MASKx(p,2,d,s) + B_MASKx(p,3,d,s) + B_MASKx(p,4,d,s) +\
            B_MASKx(p,5,d,s) + B_MASKx(p,6,d,s) + B_MASKx(p,7,d,s), d)

#if PNG_USE_COMPILE_TIME_MASKS
         /* Utility macros to construct all the masks for a depth/swap
          * combination.  The 's' parameter says whether the format is PNG
          * (big endian bytes) or not.  Only the three odd-numbered passes are
          * required for the display/block algorithm.
          */
#        define S_MASKS(d,s) { S_MASK(0,d,s), S_MASK(1,d,s), S_MASK(2,d,s),\
            S_MASK(3,d,s), S_MASK(4,d,s), S_MASK(5,d,s) }

#        define B_MASKS(d,s) { B_MASK(1,d,s), S_MASK(3,d,s), S_MASK(5,d,s) }

#        define DEPTH_INDEX(d) ((d)==1?0:((d)==2?1:2))

         /* Hence the pre-compiled masks indexed by PACKSWAP (or not), depth and
          * then pass:
          */
         static PNG_CONST _png_uint_32 row_mask[2/*PACKSWAP*/][3/*depth*/][6] =
         {
            /* Little-endian byte masks for PACKSWAP */
            { S_MASKS(1,0), S_MASKS(2,0), S_MASKS(4,0) },
            /* Normal (big-endian byte) masks - PNG format */
            { S_MASKS(1,1), S_MASKS(2,1), S_MASKS(4,1) }
         };

         /* display_mask has only three entries for the odd passes, so index by
          * pass>>1.
          */
         static PNG_CONST _png_uint_32 display_mask[2][3][3] =
         {
            /* Little-endian byte masks for PACKSWAP */
            { B_MASKS(1,0), B_MASKS(2,0), B_MASKS(4,0) },
            /* Normal (big-endian byte) masks - PNG format */
            { B_MASKS(1,1), B_MASKS(2,1), B_MASKS(4,1) }
         };

#        define MASK(pass,depth,display,png)\
            ((display)?display_mask[png][DEPTH_INDEX(depth)][pass>>1]:\
               row_mask[png][DEPTH_INDEX(depth)][pass])

#else /* !PNG_USE_COMPILE_TIME_MASKS */
         /* This is the runtime alternative: it seems unlikely that this will
          * ever be either smaller or faster than the compile time approach.
          */
#        define MASK(pass,depth,display,png)\
            ((display)?B_MASK(pass,depth,png):S_MASK(pass,depth,png))
#endif /* !PNG_USE_COMPILE_TIME_MASKS */

         /* Use the appropriate mask to copy the required bits.  In some cases
          * the byte mask will be 0 or 0xff, optimize these cases.  row_width is
          * the number of pixels, but the code copies bytes, so it is necessary
          * to special case the end.
          */
         _png_uint_32 pixels_per_byte = 8 / pixel_depth;
         _png_uint_32 mask;

#        ifdef PNG_READ_PACKSWAP_SUPPORTED
            if (_png_ptr->transformations & PNG_PACKSWAP)
               mask = MASK(pass, pixel_depth, display, 0);

            else
#        endif
            mask = MASK(pass, pixel_depth, display, 1);

         for (;;)
         {
            _png_uint_32 m;

            /* It doesn't matter in the following if _png_uint_32 has more than
             * 32 bits because the high bits always match those in m<<24; it is,
             * however, essential to use OR here, not +, because of this.
             */
            m = mask;
            mask = (m >> 8) | (m << 24); /* rotate right to good compilers */
            m &= 0xff;

            if (m != 0) /* something to copy */
            {
               if (m != 0xff)
                  *dp = (_png_byte)((*dp & ~m) | (*sp & m));
               else
                  *dp = *sp;
            }

            /* NOTE: this may overwrite the last byte with garbage if the image
             * is not an exact number of bytes wide; libpng has always done
             * this.
             */
            if (row_width <= pixels_per_byte)
               break; /* May need to restore part of the last byte */

            row_width -= pixels_per_byte;
            ++dp;
            ++sp;
         }
      }

      else /* pixel_depth >= 8 */
      {
         unsigned int bytes_to_copy, bytes_to_jump;

         /* Validate the depth - it must be a multiple of 8 */
         if (pixel_depth & 7)
            _png_error(_png_ptr, "invalid user transform pixel depth");

         pixel_depth >>= 3; /* now in bytes */
         row_width *= pixel_depth;

         /* Regardless of pass number the Adam 7 interlace always results in a
          * fixed number of pixels to copy then to skip.  There may be a
          * different number of pixels to skip at the start though.
          */
         {
            unsigned int offset = PNG_PASS_START_COL(pass) * pixel_depth;

            row_width -= offset;
            dp += offset;
            sp += offset;
         }

         /* Work out the bytes to copy. */
         if (display)
         {
            /* When doing the 'block' algorithm the pixel in the pass gets
             * replicated to adjacent pixels.  This is why the even (0,2,4,6)
             * passes are skipped above - the entire expanded row is copied.
             */
            bytes_to_copy = (1<<((6-pass)>>1)) * pixel_depth;

            /* But don't allow this number to exceed the actual row width. */
            if (bytes_to_copy > row_width)
               bytes_to_copy = row_width;
         }

         else /* normal row; Adam7 only ever gives us one pixel to copy. */
            bytes_to_copy = pixel_depth;

         /* In Adam7 there is a constant offset between where the pixels go. */
         bytes_to_jump = PNG_PASS_COL_OFFSET(pass) * pixel_depth;

         /* And simply copy these bytes.  Some optimization is possible here,
          * depending on the value of 'bytes_to_copy'.  Special case the low
          * byte counts, which we know to be frequent.
          *
          * Notice that these cases all 'return' rather than 'break' - this
          * avoids an unnecessary test on whether to restore the last byte
          * below.
          */
         switch (bytes_to_copy)
         {
            case 1:
               for (;;)
               {
                  *dp = *sp;

                  if (row_width <= bytes_to_jump)
                     return;

                  dp += bytes_to_jump;
                  sp += bytes_to_jump;
                  row_width -= bytes_to_jump;
               }

            case 2:
               /* There is a possibility of a partial copy at the end here; this
                * slows the code down somewhat.
                */
               do
               {
                  dp[0] = sp[0], dp[1] = sp[1];

                  if (row_width <= bytes_to_jump)
                     return;

                  sp += bytes_to_jump;
                  dp += bytes_to_jump;
                  row_width -= bytes_to_jump;
               }
               while (row_width > 1);

               /* And there can only be one byte left at this point: */
               *dp = *sp;
               return;

            case 3:
               /* This can only be the RGB case, so each copy is exactly one
                * pixel and it is not necessary to check for a partial copy.
                */
               for(;;)
               {
                  dp[0] = sp[0], dp[1] = sp[1], dp[2] = sp[2];

                  if (row_width <= bytes_to_jump)
                     return;

                  sp += bytes_to_jump;
                  dp += bytes_to_jump;
                  row_width -= bytes_to_jump;
               }

            default:
#if PNG_ALIGN_TYPE != PNG_ALIGN_NONE
               /* Check for double byte alignment and, if possible, use a
                * 16-bit copy.  Don't attempt this for narrow images - ones that
                * are less than an interlace panel wide.  Don't attempt it for
                * wide bytes_to_copy either - use the _png_memcpy there.
                */
               if (bytes_to_copy < 16 /*else use _png_memcpy*/ &&
                  _png_isaligned(dp, _png_uint_16) &&
                  _png_isaligned(sp, _png_uint_16) &&
                  bytes_to_copy % sizeof (_png_uint_16) == 0 &&
                  bytes_to_jump % sizeof (_png_uint_16) == 0)
               {
                  /* Everything is aligned for _png_uint_16 copies, but try for
                   * _png_uint_32 first.
                   */
                  if (_png_isaligned(dp, _png_uint_32) &&
                     _png_isaligned(sp, _png_uint_32) &&
                     bytes_to_copy % sizeof (_png_uint_32) == 0 &&
                     bytes_to_jump % sizeof (_png_uint_32) == 0)
                  {
                     _png_uint_32p dp32 = (_png_uint_32p)dp;
                     _png_const_uint_32p sp32 = (_png_const_uint_32p)sp;
                     unsigned int skip = (bytes_to_jump-bytes_to_copy) /
                        sizeof (_png_uint_32);

                     do
                     {
                        size_t c = bytes_to_copy;
                        do
                        {
                           *dp32++ = *sp32++;
                           c -= sizeof (_png_uint_32);
                        }
                        while (c > 0);

                        if (row_width <= bytes_to_jump)
                           return;

                        dp32 += skip;
                        sp32 += skip;
                        row_width -= bytes_to_jump;
                     }
                     while (bytes_to_copy <= row_width);

                     /* Get to here when the row_width truncates the final copy.
                      * There will be 1-3 bytes left to copy, so don't try the
                      * 16-bit loop below.
                      */
                     dp = (_png_bytep)dp32;
                     sp = (_png_const_bytep)sp32;
                     do
                        *dp++ = *sp++;
                     while (--row_width > 0);
                     return;
                  }

                  /* Else do it in 16-bit quantities, but only if the size is
                   * not too large.
                   */
                  else
                  {
                     _png_uint_16p dp16 = (_png_uint_16p)dp;
                     _png_const_uint_16p sp16 = (_png_const_uint_16p)sp;
                     unsigned int skip = (bytes_to_jump-bytes_to_copy) /
                        sizeof (_png_uint_16);

                     do
                     {
                        size_t c = bytes_to_copy;
                        do
                        {
                           *dp16++ = *sp16++;
                           c -= sizeof (_png_uint_16);
                        }
                        while (c > 0);

                        if (row_width <= bytes_to_jump)
                           return;

                        dp16 += skip;
                        sp16 += skip;
                        row_width -= bytes_to_jump;
                     }
                     while (bytes_to_copy <= row_width);

                     /* End of row - 1 byte left, bytes_to_copy > row_width: */
                     dp = (_png_bytep)dp16;
                     sp = (_png_const_bytep)sp16;
                     do
                        *dp++ = *sp++;
                     while (--row_width > 0);
                     return;
                  }
               }
#endif /* PNG_ALIGN_ code */

               /* The true default - use a _png_memcpy: */
               for (;;)
               {
                  _png_memcpy(dp, sp, bytes_to_copy);

                  if (row_width <= bytes_to_jump)
                     return;

                  sp += bytes_to_jump;
                  dp += bytes_to_jump;
                  row_width -= bytes_to_jump;
                  if (bytes_to_copy > row_width)
                     bytes_to_copy = row_width;
               }
         }

         /* NOT REACHED*/
      } /* pixel_depth >= 8 */

      /* Here if pixel_depth < 8 to check 'end_ptr' below. */
   }
   else
#endif

   /* If here then the switch above wasn't used so just _png_memcpy the whole row
    * from the temporary row buffer (notice that this overwrites the end of the
    * destination row if it is a partial byte.)
    */
   _png_memcpy(dp, sp, PNG_ROWBYTES(pixel_depth, row_width));

   /* Restore the overwritten bits from the last byte if necessary. */
   if (end_ptr != NULL)
      *end_ptr = (_png_byte)((end_byte & end_mask) | (*end_ptr & ~end_mask));
}

#ifdef PNG_READ_INTERLACING_SUPPORTED
void /* PRIVATE */
_png_do_read_interlace(_png_row_infop row_info, _png_bytep row, int pass,
   _png_uint_32 transformations /* Because these may affect the byte layout */)
{
   /* Arrays to facilitate easy interlacing - use pass (0 - 6) as index */
   /* Offset to next interlace block */
   static PNG_CONST int _png_pass_inc[7] = {8, 8, 4, 4, 2, 2, 1};

   _png_debug(1, "in _png_do_read_interlace");
   if (row != NULL && row_info != NULL)
   {
      _png_uint_32 final_width;

      final_width = row_info->width * _png_pass_inc[pass];

      switch (row_info->pixel_depth)
      {
         case 1:
         {
            _png_bytep sp = row + (_png_size_t)((row_info->width - 1) >> 3);
            _png_bytep dp = row + (_png_size_t)((final_width - 1) >> 3);
            int sshift, dshift;
            int s_start, s_end, s_inc;
            int jstop = _png_pass_inc[pass];
            _png_byte v;
            _png_uint_32 i;
            int j;

#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if (transformations & PNG_PACKSWAP)
            {
                sshift = (int)((row_info->width + 7) & 0x07);
                dshift = (int)((final_width + 7) & 0x07);
                s_start = 7;
                s_end = 0;
                s_inc = -1;
            }

            else
#endif
            {
                sshift = 7 - (int)((row_info->width + 7) & 0x07);
                dshift = 7 - (int)((final_width + 7) & 0x07);
                s_start = 0;
                s_end = 7;
                s_inc = 1;
            }

            for (i = 0; i < row_info->width; i++)
            {
               v = (_png_byte)((*sp >> sshift) & 0x01);
               for (j = 0; j < jstop; j++)
               {
                  *dp &= (_png_byte)((0x7f7f >> (7 - dshift)) & 0xff);
                  *dp |= (_png_byte)(v << dshift);

                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }

                  else
                     dshift += s_inc;
               }

               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }

               else
                  sshift += s_inc;
            }
            break;
         }

         case 2:
         {
            _png_bytep sp = row + (_png_uint_32)((row_info->width - 1) >> 2);
            _png_bytep dp = row + (_png_uint_32)((final_width - 1) >> 2);
            int sshift, dshift;
            int s_start, s_end, s_inc;
            int jstop = _png_pass_inc[pass];
            _png_uint_32 i;

#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if (transformations & PNG_PACKSWAP)
            {
               sshift = (int)(((row_info->width + 3) & 0x03) << 1);
               dshift = (int)(((final_width + 3) & 0x03) << 1);
               s_start = 6;
               s_end = 0;
               s_inc = -2;
            }

            else
#endif
            {
               sshift = (int)((3 - ((row_info->width + 3) & 0x03)) << 1);
               dshift = (int)((3 - ((final_width + 3) & 0x03)) << 1);
               s_start = 0;
               s_end = 6;
               s_inc = 2;
            }

            for (i = 0; i < row_info->width; i++)
            {
               _png_byte v;
               int j;

               v = (_png_byte)((*sp >> sshift) & 0x03);
               for (j = 0; j < jstop; j++)
               {
                  *dp &= (_png_byte)((0x3f3f >> (6 - dshift)) & 0xff);
                  *dp |= (_png_byte)(v << dshift);

                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }

                  else
                     dshift += s_inc;
               }

               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }

               else
                  sshift += s_inc;
            }
            break;
         }

         case 4:
         {
            _png_bytep sp = row + (_png_size_t)((row_info->width - 1) >> 1);
            _png_bytep dp = row + (_png_size_t)((final_width - 1) >> 1);
            int sshift, dshift;
            int s_start, s_end, s_inc;
            _png_uint_32 i;
            int jstop = _png_pass_inc[pass];

#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if (transformations & PNG_PACKSWAP)
            {
               sshift = (int)(((row_info->width + 1) & 0x01) << 2);
               dshift = (int)(((final_width + 1) & 0x01) << 2);
               s_start = 4;
               s_end = 0;
               s_inc = -4;
            }

            else
#endif
            {
               sshift = (int)((1 - ((row_info->width + 1) & 0x01)) << 2);
               dshift = (int)((1 - ((final_width + 1) & 0x01)) << 2);
               s_start = 0;
               s_end = 4;
               s_inc = 4;
            }

            for (i = 0; i < row_info->width; i++)
            {
               _png_byte v = (_png_byte)((*sp >> sshift) & 0x0f);
               int j;

               for (j = 0; j < jstop; j++)
               {
                  *dp &= (_png_byte)((0xf0f >> (4 - dshift)) & 0xff);
                  *dp |= (_png_byte)(v << dshift);

                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }

                  else
                     dshift += s_inc;
               }

               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }

               else
                  sshift += s_inc;
            }
            break;
         }

         default:
         {
            _png_size_t pixel_bytes = (row_info->pixel_depth >> 3);

            _png_bytep sp = row + (_png_size_t)(row_info->width - 1)
                * pixel_bytes;

            _png_bytep dp = row + (_png_size_t)(final_width - 1) * pixel_bytes;

            int jstop = _png_pass_inc[pass];
            _png_uint_32 i;

            for (i = 0; i < row_info->width; i++)
            {
               _png_byte v[8];
               int j;

               _png_memcpy(v, sp, pixel_bytes);

               for (j = 0; j < jstop; j++)
               {
                  _png_memcpy(dp, v, pixel_bytes);
                  dp -= pixel_bytes;
               }

               sp -= pixel_bytes;
            }
            break;
         }
      }

      row_info->width = final_width;
      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, final_width);
   }
#ifndef PNG_READ_PACKSWAP_SUPPORTED
   PNG_UNUSED(transformations)  /* Silence compiler warning */
#endif
}
#endif /* PNG_READ_INTERLACING_SUPPORTED */

static void
_png_read_filter_row_sub(_png_row_infop row_info, _png_bytep row,
   _png_const_bytep prev_row)
{
   _png_size_t i;
   _png_size_t istop = row_info->rowbytes;
   unsigned int bpp = (row_info->pixel_depth + 7) >> 3;
   _png_bytep rp = row + bpp;

   PNG_UNUSED(prev_row)

   for (i = bpp; i < istop; i++)
   {
      *rp = (_png_byte)(((int)(*rp) + (int)(*(rp-bpp))) & 0xff);
      rp++;
   }
}

static void
_png_read_filter_row_up(_png_row_infop row_info, _png_bytep row,
   _png_const_bytep prev_row)
{
   _png_size_t i;
   _png_size_t istop = row_info->rowbytes;
   _png_bytep rp = row;
   _png_const_bytep pp = prev_row;

   for (i = 0; i < istop; i++)
   {
      *rp = (_png_byte)(((int)(*rp) + (int)(*pp++)) & 0xff);
      rp++;
   }
}

static void
_png_read_filter_row_avg(_png_row_infop row_info, _png_bytep row,
   _png_const_bytep prev_row)
{
   _png_size_t i;
   _png_bytep rp = row;
   _png_const_bytep pp = prev_row;
   unsigned int bpp = (row_info->pixel_depth + 7) >> 3;
   _png_size_t istop = row_info->rowbytes - bpp;

   for (i = 0; i < bpp; i++)
   {
      *rp = (_png_byte)(((int)(*rp) +
         ((int)(*pp++) / 2 )) & 0xff);

      rp++;
   }

   for (i = 0; i < istop; i++)
   {
      *rp = (_png_byte)(((int)(*rp) +
         (int)(*pp++ + *(rp-bpp)) / 2 ) & 0xff);

      rp++;
   }
}

static void
_png_read_filter_row_paeth_1byte_pixel(_png_row_infop row_info, _png_bytep row,
   _png_const_bytep prev_row)
{
   _png_bytep rp_end = row + row_info->rowbytes;
   int a, c;

   /* First pixel/byte */
   c = *prev_row++;
   a = *row + c;
   *row++ = (_png_byte)a;

   /* Remainder */
   while (row < rp_end)
   {
      int b, pa, pb, pc, p;

      a &= 0xff; /* From previous iteration or start */
      b = *prev_row++;

      p = b - c;
      pc = a - c;

#     ifdef PNG_USE_ABS
         pa = abs(p);
         pb = abs(pc);
         pc = abs(p + pc);
#     else
         pa = p < 0 ? -p : p;
         pb = pc < 0 ? -pc : pc;
         pc = (p + pc) < 0 ? -(p + pc) : p + pc;
#     endif

      /* Find the best predictor, the least of pa, pb, pc favoring the earlier
       * ones in the case of a tie.
       */
      if (pb < pa) pa = pb, a = b;
      if (pc < pa) a = c;

      /* Calculate the current pixel in a, and move the previous row pixel to c
       * for the next time round the loop
       */
      c = b;
      a += *row;
      *row++ = (_png_byte)a;
   }
}

static void
_png_read_filter_row_paeth_multibyte_pixel(_png_row_infop row_info, _png_bytep row,
   _png_const_bytep prev_row)
{
   int bpp = (row_info->pixel_depth + 7) >> 3;
   _png_bytep rp_end = row + bpp;

   /* Process the first pixel in the row completely (this is the same as 'up'
    * because there is only one candidate predictor for the first row).
    */
   while (row < rp_end)
   {
      int a = *row + *prev_row++;
      *row++ = (_png_byte)a;
   }

   /* Remainder */
   rp_end += row_info->rowbytes - bpp;

   while (row < rp_end)
   {
      int a, b, c, pa, pb, pc, p;

      c = *(prev_row - bpp);
      a = *(row - bpp);
      b = *prev_row++;

      p = b - c;
      pc = a - c;

#     ifdef PNG_USE_ABS
         pa = abs(p);
         pb = abs(pc);
         pc = abs(p + pc);
#     else
         pa = p < 0 ? -p : p;
         pb = pc < 0 ? -pc : pc;
         pc = (p + pc) < 0 ? -(p + pc) : p + pc;
#     endif

      if (pb < pa) pa = pb, a = b;
      if (pc < pa) a = c;

      c = b;
      a += *row;
      *row++ = (_png_byte)a;
   }
}

static void
_png_init_filter_functions(_png_structp pp)
{
   unsigned int bpp = (pp->pixel_depth + 7) >> 3;

   pp->read_filter[PNG_FILTER_VALUE_SUB-1] = _png_read_filter_row_sub;
   pp->read_filter[PNG_FILTER_VALUE_UP-1] = _png_read_filter_row_up;
   pp->read_filter[PNG_FILTER_VALUE_AVG-1] = _png_read_filter_row_avg;
   if (bpp == 1)
      pp->read_filter[PNG_FILTER_VALUE_PAETH-1] =
         _png_read_filter_row_paeth_1byte_pixel;
   else
      pp->read_filter[PNG_FILTER_VALUE_PAETH-1] =
         _png_read_filter_row_paeth_multibyte_pixel;

#ifdef PNG_FILTER_OPTIMIZATIONS
   /* To use this define PNG_FILTER_OPTIMIZATIONS as the name of a function to
    * call to install hardware optimizations for the above functions; simply
    * replace whatever elements of the pp->read_filter[] array with a hardware
    * specific (or, for that matter, generic) optimization.
    *
    * To see an example of this examine what configure.ac does when
    * --enable-arm-neon is specified on the command line.
    */
   PNG_FILTER_OPTIMIZATIONS(pp, bpp);
#endif
}

void /* PRIVATE */
_png_read_filter_row(_png_structp pp, _png_row_infop row_info, _png_bytep row,
   _png_const_bytep prev_row, int filter)
{
   if (pp->read_filter[0] == NULL)
      _png_init_filter_functions(pp);
   if (filter > PNG_FILTER_VALUE_NONE && filter < PNG_FILTER_VALUE_LAST)
      pp->read_filter[filter-1](row_info, row, prev_row);
}

#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
void /* PRIVATE */
_png_read_finish_row(_png_structp _png_ptr)
{
#ifdef PNG_READ_INTERLACING_SUPPORTED
   /* Arrays to facilitate easy interlacing - use pass (0 - 6) as index */

   /* Start of interlace block */
   static PNG_CONST _png_byte _png_pass_start[7] = {0, 4, 0, 2, 0, 1, 0};

   /* Offset to next interlace block */
   static PNG_CONST _png_byte _png_pass_inc[7] = {8, 8, 4, 4, 2, 2, 1};

   /* Start of interlace block in the y direction */
   static PNG_CONST _png_byte _png_pass_ystart[7] = {0, 0, 4, 0, 2, 0, 1};

   /* Offset to next interlace block in the y direction */
   static PNG_CONST _png_byte _png_pass_yinc[7] = {8, 8, 8, 4, 4, 2, 2};
#endif /* PNG_READ_INTERLACING_SUPPORTED */

   _png_debug(1, "in _png_read_finish_row");
   _png_ptr->row_number++;
   if (_png_ptr->row_number < _png_ptr->num_rows)
      return;

#ifdef PNG_READ_INTERLACING_SUPPORTED
   if (_png_ptr->interlaced)
   {
      _png_ptr->row_number = 0;

      /* TO DO: don't do this if prev_row isn't needed (requires
       * read-ahead of the next row's filter byte.
       */
      _png_memset(_png_ptr->prev_row, 0, _png_ptr->rowbytes + 1);

      do
      {
         _png_ptr->pass++;

         if (_png_ptr->pass >= 7)
            break;

         _png_ptr->iwidth = (_png_ptr->width +
            _png_pass_inc[_png_ptr->pass] - 1 -
            _png_pass_start[_png_ptr->pass]) /
            _png_pass_inc[_png_ptr->pass];

         if (!(_png_ptr->transformations & PNG_INTERLACE))
         {
            _png_ptr->num_rows = (_png_ptr->height +
                _png_pass_yinc[_png_ptr->pass] - 1 -
                _png_pass_ystart[_png_ptr->pass]) /
                _png_pass_yinc[_png_ptr->pass];
         }

         else  /* if (_png_ptr->transformations & PNG_INTERLACE) */
            break; /* libpng deinterlacing sees every row */

      } while (_png_ptr->num_rows == 0 || _png_ptr->iwidth == 0);

      if (_png_ptr->pass < 7)
         return;
   }
#endif /* PNG_READ_INTERLACING_SUPPORTED */

   if (!(_png_ptr->flags & PNG_FLAG_ZLIB_FINISHED))
   {
      char extra;
      int ret;

      _png_ptr->zstream.next_out = (Byte *)&extra;
      _png_ptr->zstream.avail_out = (uInt)1;

      for (;;)
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

            _png_crc_read(_png_ptr, _png_ptr->zbuf, _png_ptr->zstream.avail_in);
            _png_ptr->idat_size -= _png_ptr->zstream.avail_in;
         }

         ret = inflate(&_png_ptr->zstream, Z_PARTIAL_FLUSH);

         if (ret == Z_STREAM_END)
         {
            if (!(_png_ptr->zstream.avail_out) || _png_ptr->zstream.avail_in ||
                _png_ptr->idat_size)
               _png_warning(_png_ptr, "Extra compressed data");

            _png_ptr->mode |= PNG_AFTER_IDAT;
            _png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;
            break;
         }

         if (ret != Z_OK)
            _png_error(_png_ptr, _png_ptr->zstream.msg ? _png_ptr->zstream.msg :
                "Decompression Error");

         if (!(_png_ptr->zstream.avail_out))
         {
            _png_warning(_png_ptr, "Extra compressed data");
            _png_ptr->mode |= PNG_AFTER_IDAT;
            _png_ptr->flags |= PNG_FLAG_ZLIB_FINISHED;
            break;
         }

      }
      _png_ptr->zstream.avail_out = 0;
   }

   if (_png_ptr->idat_size || _png_ptr->zstream.avail_in)
      _png_warning(_png_ptr, "Extra compression data");

   inflateReset(&_png_ptr->zstream);

   _png_ptr->mode |= PNG_AFTER_IDAT;
}
#endif /* PNG_SEQUENTIAL_READ_SUPPORTED */

void /* PRIVATE */
_png_read_start_row(_png_structp _png_ptr)
{
#ifdef PNG_READ_INTERLACING_SUPPORTED
   /* Arrays to facilitate easy interlacing - use pass (0 - 6) as index */

   /* Start of interlace block */
   static PNG_CONST _png_byte _png_pass_start[7] = {0, 4, 0, 2, 0, 1, 0};

   /* Offset to next interlace block */
   static PNG_CONST _png_byte _png_pass_inc[7] = {8, 8, 4, 4, 2, 2, 1};

   /* Start of interlace block in the y direction */
   static PNG_CONST _png_byte _png_pass_ystart[7] = {0, 0, 4, 0, 2, 0, 1};

   /* Offset to next interlace block in the y direction */
   static PNG_CONST _png_byte _png_pass_yinc[7] = {8, 8, 8, 4, 4, 2, 2};
#endif

   int max_pixel_depth;
   _png_size_t row_bytes;

   _png_debug(1, "in _png_read_start_row");
   _png_ptr->zstream.avail_in = 0;
#ifdef PNG_READ_TRANSFORMS_SUPPORTED
   _png_init_read_transformations(_png_ptr);
#endif
#ifdef PNG_READ_INTERLACING_SUPPORTED
   if (_png_ptr->interlaced)
   {
      if (!(_png_ptr->transformations & PNG_INTERLACE))
         _png_ptr->num_rows = (_png_ptr->height + _png_pass_yinc[0] - 1 -
             _png_pass_ystart[0]) / _png_pass_yinc[0];

      else
         _png_ptr->num_rows = _png_ptr->height;

      _png_ptr->iwidth = (_png_ptr->width +
          _png_pass_inc[_png_ptr->pass] - 1 -
          _png_pass_start[_png_ptr->pass]) /
          _png_pass_inc[_png_ptr->pass];
   }

   else
#endif /* PNG_READ_INTERLACING_SUPPORTED */
   {
      _png_ptr->num_rows = _png_ptr->height;
      _png_ptr->iwidth = _png_ptr->width;
   }

   max_pixel_depth = _png_ptr->pixel_depth;

   /* WARNING: * _png_read_transform_info (pngrtran.c) performs a simpliar set of
    * calculations to calculate the final pixel depth, then
    * _png_do_read_transforms actually does the transforms.  This means that the
    * code which effectively calculates this value is actually repeated in three
    * separate places.  They must all match.  Innocent changes to the order of
    * transformations can and will break libpng in a way that causes memory
    * overwrites.
    *
    * TODO: fix this.
    */
#ifdef PNG_READ_PACK_SUPPORTED
   if ((_png_ptr->transformations & PNG_PACK) && _png_ptr->bit_depth < 8)
      max_pixel_depth = 8;
#endif

#ifdef PNG_READ_EXPAND_SUPPORTED
   if (_png_ptr->transformations & PNG_EXPAND)
   {
      if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      {
         if (_png_ptr->num_trans)
            max_pixel_depth = 32;

         else
            max_pixel_depth = 24;
      }

      else if (_png_ptr->color_type == PNG_COLOR_TYPE_GRAY)
      {
         if (max_pixel_depth < 8)
            max_pixel_depth = 8;

         if (_png_ptr->num_trans)
            max_pixel_depth *= 2;
      }

      else if (_png_ptr->color_type == PNG_COLOR_TYPE_RGB)
      {
         if (_png_ptr->num_trans)
         {
            max_pixel_depth *= 4;
            max_pixel_depth /= 3;
         }
      }
   }
#endif

#ifdef PNG_READ_EXPAND_16_SUPPORTED
   if (_png_ptr->transformations & PNG_EXPAND_16)
   {
#     ifdef PNG_READ_EXPAND_SUPPORTED
         /* In fact it is an error if it isn't supported, but checking is
          * the safe way.
          */
         if (_png_ptr->transformations & PNG_EXPAND)
         {
            if (_png_ptr->bit_depth < 16)
               max_pixel_depth *= 2;
         }
         else
#     endif
         _png_ptr->transformations &= ~PNG_EXPAND_16;
   }
#endif

#ifdef PNG_READ_FILLER_SUPPORTED
   if (_png_ptr->transformations & (PNG_FILLER))
   {
      if (_png_ptr->color_type == PNG_COLOR_TYPE_GRAY)
      {
         if (max_pixel_depth <= 8)
            max_pixel_depth = 16;

         else
            max_pixel_depth = 32;
      }

      else if (_png_ptr->color_type == PNG_COLOR_TYPE_RGB ||
         _png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      {
         if (max_pixel_depth <= 32)
            max_pixel_depth = 32;

         else
            max_pixel_depth = 64;
      }
   }
#endif

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
   if (_png_ptr->transformations & PNG_GRAY_TO_RGB)
   {
      if (
#ifdef PNG_READ_EXPAND_SUPPORTED
          (_png_ptr->num_trans && (_png_ptr->transformations & PNG_EXPAND)) ||
#endif
#ifdef PNG_READ_FILLER_SUPPORTED
          (_png_ptr->transformations & (PNG_FILLER)) ||
#endif
          _png_ptr->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      {
         if (max_pixel_depth <= 16)
            max_pixel_depth = 32;

         else
            max_pixel_depth = 64;
      }

      else
      {
         if (max_pixel_depth <= 8)
         {
            if (_png_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
               max_pixel_depth = 32;

            else
               max_pixel_depth = 24;
         }

         else if (_png_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
            max_pixel_depth = 64;

         else
            max_pixel_depth = 48;
      }
   }
#endif

#if defined(PNG_READ_USER_TRANSFORM_SUPPORTED) && \
defined(PNG_USER_TRANSFORM_PTR_SUPPORTED)
   if (_png_ptr->transformations & PNG_USER_TRANSFORM)
   {
      int user_pixel_depth = _png_ptr->user_transform_depth *
         _png_ptr->user_transform_channels;

      if (user_pixel_depth > max_pixel_depth)
         max_pixel_depth = user_pixel_depth;
   }
#endif

   /* This value is stored in _png_struct and double checked in the row read
    * code.
    */
   _png_ptr->maximum_pixel_depth = (_png_byte)max_pixel_depth;
   _png_ptr->transformed_pixel_depth = 0; /* calculated on demand */

   /* Align the width on the next larger 8 pixels.  Mainly used
    * for interlacing
    */
   row_bytes = ((_png_ptr->width + 7) & ~((_png_uint_32)7));
   /* Calculate the maximum bytes needed, adding a byte and a pixel
    * for safety's sake
    */
   row_bytes = PNG_ROWBYTES(max_pixel_depth, row_bytes) +
       1 + ((max_pixel_depth + 7) >> 3);

#ifdef PNG_MAX_MALLOC_64K
   if (row_bytes > (_png_uint_32)65536L)
      _png_error(_png_ptr, "This image requires a row greater than 64KB");
#endif

   if (row_bytes + 48 > _png_ptr->old_big_row_buf_size)
   {
     _png_free(_png_ptr, _png_ptr->big_row_buf);
     _png_free(_png_ptr, _png_ptr->big_prev_row);

     if (_png_ptr->interlaced)
        _png_ptr->big_row_buf = (_png_bytep)_png_calloc(_png_ptr,
            row_bytes + 48);

     else
        _png_ptr->big_row_buf = (_png_bytep)_png_malloc(_png_ptr, row_bytes + 48);

     _png_ptr->big_prev_row = (_png_bytep)_png_malloc(_png_ptr, row_bytes + 48);

#ifdef PNG_ALIGNED_MEMORY_SUPPORTED
     /* Use 16-byte aligned memory for row_buf with at least 16 bytes
      * of padding before and after row_buf; treat prev_row similarly.
      * NOTE: the alignment is to the start of the pixels, one beyond the start
      * of the buffer, because of the filter byte.  Prior to libpng 1.5.6 this
      * was incorrect; the filter byte was aligned, which had the exact
      * opposite effect of that intended.
      */
     {
        _png_bytep temp = _png_ptr->big_row_buf + 32;
        int extra = (int)((temp - (_png_bytep)0) & 0x0f);
        _png_ptr->row_buf = temp - extra - 1/*filter byte*/;

        temp = _png_ptr->big_prev_row + 32;
        extra = (int)((temp - (_png_bytep)0) & 0x0f);
        _png_ptr->prev_row = temp - extra - 1/*filter byte*/;
     }

#else
     /* Use 31 bytes of padding before and 17 bytes after row_buf. */
     _png_ptr->row_buf = _png_ptr->big_row_buf + 31;
     _png_ptr->prev_row = _png_ptr->big_prev_row + 31;
#endif
     _png_ptr->old_big_row_buf_size = row_bytes + 48;
   }

#ifdef PNG_MAX_MALLOC_64K
   if (_png_ptr->rowbytes > 65535)
      _png_error(_png_ptr, "This image requires a row greater than 64KB");

#endif
   if (_png_ptr->rowbytes > (PNG_SIZE_MAX - 1))
      _png_error(_png_ptr, "Row has too many bytes to allocate in memory");

   _png_memset(_png_ptr->prev_row, 0, _png_ptr->rowbytes + 1);

   _png_debug1(3, "width = %u,", _png_ptr->width);
   _png_debug1(3, "height = %u,", _png_ptr->height);
   _png_debug1(3, "iwidth = %u,", _png_ptr->iwidth);
   _png_debug1(3, "num_rows = %u,", _png_ptr->num_rows);
   _png_debug1(3, "rowbytes = %lu,", (unsigned long)_png_ptr->rowbytes);
   _png_debug1(3, "irowbytes = %lu",
       (unsigned long)PNG_ROWBYTES(_png_ptr->pixel_depth, _png_ptr->iwidth) + 1);

   _png_ptr->flags |= PNG_FLAG_ROW_INIT;
}
#endif /* PNG_READ_SUPPORTED */
