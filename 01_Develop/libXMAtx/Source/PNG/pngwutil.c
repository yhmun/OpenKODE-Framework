
/* pngwutil.c - utilities to write a PNG file
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

#ifdef PNG_WRITE_INT_FUNCTIONS_SUPPORTED
/* Place a 32-bit number into a buffer in PNG byte order.  We work
 * with unsigned numbers for convenience, although one supported
 * ancillary chunk uses signed (two's complement) numbers.
 */
void PNGAPI
_png_save_uint_32(_png_bytep buf, _png_uint_32 i)
{
   buf[0] = (_png_byte)((i >> 24) & 0xff);
   buf[1] = (_png_byte)((i >> 16) & 0xff);
   buf[2] = (_png_byte)((i >> 8) & 0xff);
   buf[3] = (_png_byte)(i & 0xff);
}

#ifdef PNG_SAVE_INT_32_SUPPORTED
/* The _png_save_int_32 function assumes integers are stored in two's
 * complement format.  If this isn't the case, then this routine needs to
 * be modified to write data in two's complement format.  Note that,
 * the following works correctly even if _png_int_32 has more than 32 bits
 * (compare the more complex code required on read for sign extention.)
 */
void PNGAPI
_png_save_int_32(_png_bytep buf, _png_int_32 i)
{
   buf[0] = (_png_byte)((i >> 24) & 0xff);
   buf[1] = (_png_byte)((i >> 16) & 0xff);
   buf[2] = (_png_byte)((i >> 8) & 0xff);
   buf[3] = (_png_byte)(i & 0xff);
}
#endif

/* Place a 16-bit number into a buffer in PNG byte order.
 * The parameter is declared unsigned int, not _png_uint_16,
 * just to avoid potential problems on pre-ANSI C compilers.
 */
void PNGAPI
_png_save_uint_16(_png_bytep buf, unsigned int i)
{
   buf[0] = (_png_byte)((i >> 8) & 0xff);
   buf[1] = (_png_byte)(i & 0xff);
}
#endif

/* Simple function to write the signature.  If we have already written
 * the magic bytes of the signature, or more likely, the PNG stream is
 * being embedded into another stream and doesn't need its own signature,
 * we should call _png_set_sig_bytes() to tell libpng how many of the
 * bytes have already been written.
 */
void PNGAPI
_png_write_sig(_png_structp _png_ptr)
{
   _png_byte _png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};

#ifdef PNG_IO_STATE_SUPPORTED
   /* Inform the I/O callback that the signature is being written */
   _png_ptr->io_state = PNG_IO_WRITING | PNG_IO_SIGNATURE;
#endif

   /* Write the rest of the 8 byte signature */
   _png_write_data(_png_ptr, &_png_signature[_png_ptr->sig_bytes],
      (_png_size_t)(8 - _png_ptr->sig_bytes));

   if (_png_ptr->sig_bytes < 3)
      _png_ptr->mode |= PNG_HAVE_PNG_SIGNATURE;
}

/* Write the start of a PNG chunk.  The type is the chunk type.
 * The total_length is the sum of the lengths of all the data you will be
 * passing in _png_write_chunk_data().
 */
static void
_png_write_chunk_header(_png_structp _png_ptr, _png_uint_32 chunk_name,
    _png_uint_32 length)
{
   _png_byte buf[8];

#if defined(PNG_DEBUG) && (PNG_DEBUG > 0)
   PNG_CSTRING_FROM_CHUNK(buf, chunk_name);
   _png_debug2(0, "Writing %s chunk, length = %lu", buf, (unsigned long)length);
#endif

   if (_png_ptr == NULL)
      return;

#ifdef PNG_IO_STATE_SUPPORTED
   /* Inform the I/O callback that the chunk header is being written.
    * PNG_IO_CHUNK_HDR requires a single I/O call.
    */
   _png_ptr->io_state = PNG_IO_WRITING | PNG_IO_CHUNK_HDR;
#endif

   /* Write the length and the chunk name */
   _png_save_uint_32(buf, length);
   _png_save_uint_32(buf + 4, chunk_name);
   _png_write_data(_png_ptr, buf, 8);

   /* Put the chunk name into _png_ptr->chunk_name */
   _png_ptr->chunk_name = chunk_name;

   /* Reset the crc and run it over the chunk name */
   _png_reset_crc(_png_ptr);

   _png_calculate_crc(_png_ptr, buf + 4, 4);

#ifdef PNG_IO_STATE_SUPPORTED
   /* Inform the I/O callback that chunk data will (possibly) be written.
    * PNG_IO_CHUNK_DATA does NOT require a specific number of I/O calls.
    */
   _png_ptr->io_state = PNG_IO_WRITING | PNG_IO_CHUNK_DATA;
#endif
}

void PNGAPI
_png_write_chunk_start(_png_structp _png_ptr, _png_const_bytep chunk_string,
    _png_uint_32 length)
{
   _png_write_chunk_header(_png_ptr, PNG_CHUNK_FROM_STRING(chunk_string), length);
}

/* Write the data of a PNG chunk started with _png_write_chunk_header().
 * Note that multiple calls to this function are allowed, and that the
 * sum of the lengths from these calls *must* add up to the total_length
 * given to _png_write_chunk_header().
 */
void PNGAPI
_png_write_chunk_data(_png_structp _png_ptr, _png_const_bytep data,
    _png_size_t length)
{
   /* Write the data, and run the CRC over it */
   if (_png_ptr == NULL)
      return;

   if (data != NULL && length > 0)
   {
      _png_write_data(_png_ptr, data, length);

      /* Update the CRC after writing the data,
       * in case that the user I/O routine alters it.
       */
      _png_calculate_crc(_png_ptr, data, length);
   }
}

/* Finish a chunk started with _png_write_chunk_header(). */
void PNGAPI
_png_write_chunk_end(_png_structp _png_ptr)
{
   _png_byte buf[4];

   if (_png_ptr == NULL) return;

#ifdef PNG_IO_STATE_SUPPORTED
   /* Inform the I/O callback that the chunk CRC is being written.
    * PNG_IO_CHUNK_CRC requires a single I/O function call.
    */
   _png_ptr->io_state = PNG_IO_WRITING | PNG_IO_CHUNK_CRC;
#endif

   /* Write the crc in a single operation */
   _png_save_uint_32(buf, _png_ptr->crc);

   _png_write_data(_png_ptr, buf, (_png_size_t)4);
}

/* Write a PNG chunk all at once.  The type is an array of ASCII characters
 * representing the chunk name.  The array must be at least 4 bytes in
 * length, and does not need to be null terminated.  To be safe, pass the
 * pre-defined chunk names here, and if you need a new one, define it
 * where the others are defined.  The length is the length of the data.
 * All the data must be present.  If that is not possible, use the
 * _png_write_chunk_start(), _png_write_chunk_data(), and _png_write_chunk_end()
 * functions instead.
 */
static void
_png_write_complete_chunk(_png_structp _png_ptr, _png_uint_32 chunk_name,
   _png_const_bytep data, _png_size_t length)
{
   if (_png_ptr == NULL)
      return;

   /* On 64 bit architectures 'length' may not fit in a _png_uint_32. */
   if (length > PNG_UINT_32_MAX)
      _png_error(_png_ptr, "length exceeds PNG maxima");

   _png_write_chunk_header(_png_ptr, chunk_name, (_png_uint_32)length);
   _png_write_chunk_data(_png_ptr, data, length);
   _png_write_chunk_end(_png_ptr);
}

/* This is the API that calls the internal function above. */
void PNGAPI
_png_write_chunk(_png_structp _png_ptr, _png_const_bytep chunk_string,
   _png_const_bytep data, _png_size_t length)
{
   _png_write_complete_chunk(_png_ptr, PNG_CHUNK_FROM_STRING(chunk_string), data,
      length);
}

/* Initialize the compressor for the appropriate type of compression. */
static void
_png_zlib_claim(_png_structp _png_ptr, _png_uint_32 state)
{
   if (!(_png_ptr->zlib_state & PNG_ZLIB_IN_USE))
   {
      /* If already initialized for 'state' do not re-init. */
      if (_png_ptr->zlib_state != state)
      {
         int ret = Z_OK;
         _png_const_charp who = "-";

         /* If actually initialized for another state do a deflateEnd. */
         if (_png_ptr->zlib_state != PNG_ZLIB_UNINITIALIZED)
         {
            ret = deflateEnd(&_png_ptr->zstream);
            who = "end";
            _png_ptr->zlib_state = PNG_ZLIB_UNINITIALIZED;
         }

         /* zlib itself detects an incomplete state on deflateEnd */
         if (ret == Z_OK) switch (state)
         {
#           ifdef PNG_WRITE_COMPRESSED_TEXT_SUPPORTED
               case PNG_ZLIB_FOR_TEXT:
                  ret = deflateInit2(&_png_ptr->zstream,
                     _png_ptr->zlib_text_level, _png_ptr->zlib_text_method,
                     _png_ptr->zlib_text_window_bits,
                     _png_ptr->zlib_text_mem_level, _png_ptr->zlib_text_strategy);
                  who = "text";
                  break;
#           endif

            case PNG_ZLIB_FOR_IDAT:
               ret = deflateInit2(&_png_ptr->zstream, _png_ptr->zlib_level,
                   _png_ptr->zlib_method, _png_ptr->zlib_window_bits,
                   _png_ptr->zlib_mem_level, _png_ptr->zlib_strategy);
               who = "IDAT";
               break;

            default:
               _png_error(_png_ptr, "invalid zlib state");
         }

         if (ret == Z_OK)
            _png_ptr->zlib_state = state;

         else /* an error in deflateEnd or deflateInit2 */
         {
            size_t pos = 0;
            char msg[64];

            pos = _png_safecat(msg, sizeof msg, pos,
               "zlib failed to initialize compressor (");
            pos = _png_safecat(msg, sizeof msg, pos, who);

            switch (ret)
            {
               case Z_VERSION_ERROR:
                  pos = _png_safecat(msg, sizeof msg, pos, ") version error");
                  break;

               case Z_STREAM_ERROR:
                  pos = _png_safecat(msg, sizeof msg, pos, ") stream error");
                  break;

               case Z_MEM_ERROR:
                  pos = _png_safecat(msg, sizeof msg, pos, ") memory error");
                  break;

               default:
                  pos = _png_safecat(msg, sizeof msg, pos, ") unknown error");
                  break;
            }

            _png_error(_png_ptr, msg);
         }
      }

      /* Here on success, claim the zstream: */
      _png_ptr->zlib_state |= PNG_ZLIB_IN_USE;
   }

   else
      _png_error(_png_ptr, "zstream already in use (internal error)");
}

/* The opposite: release the stream.  It is also reset, this API will warn on
 * error but will not fail.
 */
static void
_png_zlib_release(_png_structp _png_ptr)
{
   if (_png_ptr->zlib_state & PNG_ZLIB_IN_USE)
   {
      int ret = deflateReset(&_png_ptr->zstream);

      _png_ptr->zlib_state &= ~PNG_ZLIB_IN_USE;

      if (ret != Z_OK)
      {
         _png_const_charp err;
         PNG_WARNING_PARAMETERS(p)

         switch (ret)
         {
            case Z_VERSION_ERROR:
               err = "version";
               break;

            case Z_STREAM_ERROR:
               err = "stream";
               break;

            case Z_MEM_ERROR:
               err = "memory";
               break;

            default:
               err = "unknown";
               break;
         }

         _png_warning_parameter_signed(p, 1, PNG_NUMBER_FORMAT_d, ret);
         _png_warning_parameter(p, 2, err);

         if (_png_ptr->zstream.msg)
            err = _png_ptr->zstream.msg;
         else
            err = "[no zlib message]";

         _png_warning_parameter(p, 3, err);

         _png_formatted_warning(_png_ptr, p,
            "zlib failed to reset compressor: @1(@2): @3");
      }
   }

   else
      _png_warning(_png_ptr, "zstream not in use (internal error)");
}

#ifdef PNG_WRITE_COMPRESSED_TEXT_SUPPORTED
/* This pair of functions encapsulates the operation of (a) compressing a
 * text string, and (b) issuing it later as a series of chunk data writes.
 * The compression_state structure is shared context for these functions
 * set up by the caller in order to make the whole mess thread-safe.
 */

typedef struct
{
   _png_const_bytep input;   /* The uncompressed input data */
   _png_size_t input_len;    /* Its length */
   int num_output_ptr;      /* Number of output pointers used */
   int max_output_ptr;      /* Size of output_ptr */
   _png_bytep *output_ptr;   /* Array of pointers to output */
} compression_state;

/* Compress given text into storage in the _png_ptr structure */
static int /* PRIVATE */
_png_text_compress(_png_structp _png_ptr,
    _png_const_charp text, _png_size_t text_len, int compression,
    compression_state *comp)
{
   int ret;

   comp->num_output_ptr = 0;
   comp->max_output_ptr = 0;
   comp->output_ptr = NULL;
   comp->input = NULL;
   comp->input_len = text_len;

   /* We may just want to pass the text right through */
   if (compression == PNG_TEXT_COMPRESSION_NONE)
   {
      comp->input = (_png_const_bytep)text;
      return((int)text_len);
   }

   if (compression >= PNG_TEXT_COMPRESSION_LAST)
   {
      PNG_WARNING_PARAMETERS(p)

      _png_warning_parameter_signed(p, 1, PNG_NUMBER_FORMAT_d,
         compression);
      _png_formatted_warning(_png_ptr, p, "Unknown compression type @1");
   }

   /* We can't write the chunk until we find out how much data we have,
    * which means we need to run the compressor first and save the
    * output.  This shouldn't be a problem, as the vast majority of
    * comments should be reasonable, but we will set up an array of
    * malloc'd pointers to be sure.
    *
    * If we knew the application was well behaved, we could simplify this
    * greatly by assuming we can always malloc an output buffer large
    * enough to hold the compressed text ((1001 * text_len / 1000) + 12)
    * and malloc this directly.  The only time this would be a bad idea is
    * if we can't malloc more than 64K and we have 64K of random input
    * data, or if the input string is incredibly large (although this
    * wouldn't cause a failure, just a slowdown due to swapping).
    */
   _png_zlib_claim(_png_ptr, PNG_ZLIB_FOR_TEXT);

   /* Set up the compression buffers */
   /* TODO: the following cast hides a potential overflow problem. */
   _png_ptr->zstream.avail_in = (uInt)text_len;

   /* NOTE: assume zlib doesn't overwrite the input */
   _png_ptr->zstream.next_in = (Bytef *)text;
   _png_ptr->zstream.avail_out = _png_ptr->zbuf_size;
   _png_ptr->zstream.next_out = _png_ptr->zbuf;

   /* This is the same compression loop as in _png_write_row() */
   do
   {
      /* Compress the data */
      ret = deflate(&_png_ptr->zstream, Z_NO_FLUSH);

      if (ret != Z_OK)
      {
         /* Error */
         if (_png_ptr->zstream.msg != NULL)
            _png_error(_png_ptr, _png_ptr->zstream.msg);

         else
            _png_error(_png_ptr, "zlib error");
      }

      /* Check to see if we need more room */
      if (!(_png_ptr->zstream.avail_out))
      {
         /* Make sure the output array has room */
         if (comp->num_output_ptr >= comp->max_output_ptr)
         {
            int old_max;

            old_max = comp->max_output_ptr;
            comp->max_output_ptr = comp->num_output_ptr + 4;
            if (comp->output_ptr != NULL)
            {
               _png_bytepp old_ptr;

               old_ptr = comp->output_ptr;

               comp->output_ptr = (_png_bytepp)_png_malloc(_png_ptr,
                   (comp->max_output_ptr * _png_sizeof(_png_bytep)));

               _png_memcpy(comp->output_ptr, old_ptr, old_max
                   * _png_sizeof(_png_bytep));

               _png_free(_png_ptr, old_ptr);
            }
            else
               comp->output_ptr = (_png_bytepp)_png_malloc(_png_ptr,
                   (comp->max_output_ptr * _png_sizeof(_png_bytep)));
         }

         /* Save the data */
         comp->output_ptr[comp->num_output_ptr] =
             (_png_bytep)_png_malloc(_png_ptr, _png_ptr->zbuf_size);

         _png_memcpy(comp->output_ptr[comp->num_output_ptr], _png_ptr->zbuf,
             _png_ptr->zbuf_size);

         comp->num_output_ptr++;

         /* and reset the buffer */
         _png_ptr->zstream.avail_out = (uInt)_png_ptr->zbuf_size;
         _png_ptr->zstream.next_out = _png_ptr->zbuf;
      }
   /* Continue until we don't have any more to compress */
   } while (_png_ptr->zstream.avail_in);

   /* Finish the compression */
   do
   {
      /* Tell zlib we are finished */
      ret = deflate(&_png_ptr->zstream, Z_FINISH);

      if (ret == Z_OK)
      {
         /* Check to see if we need more room */
         if (!(_png_ptr->zstream.avail_out))
         {
            /* Check to make sure our output array has room */
            if (comp->num_output_ptr >= comp->max_output_ptr)
            {
               int old_max;

               old_max = comp->max_output_ptr;
               comp->max_output_ptr = comp->num_output_ptr + 4;
               if (comp->output_ptr != NULL)
               {
                  _png_bytepp old_ptr;

                  old_ptr = comp->output_ptr;

                  /* This could be optimized to realloc() */
                  comp->output_ptr = (_png_bytepp)_png_malloc(_png_ptr,
                      (_png_alloc_size_t)(comp->max_output_ptr *
                      _png_sizeof(_png_charp)));

                  _png_memcpy(comp->output_ptr, old_ptr,
                      old_max * _png_sizeof(_png_charp));

                  _png_free(_png_ptr, old_ptr);
               }

               else
                  comp->output_ptr = (_png_bytepp)_png_malloc(_png_ptr,
                      (_png_alloc_size_t)(comp->max_output_ptr *
                      _png_sizeof(_png_charp)));
            }

            /* Save the data */
            comp->output_ptr[comp->num_output_ptr] =
                (_png_bytep)_png_malloc(_png_ptr,
                (_png_alloc_size_t)_png_ptr->zbuf_size);

            _png_memcpy(comp->output_ptr[comp->num_output_ptr], _png_ptr->zbuf,
                _png_ptr->zbuf_size);

            comp->num_output_ptr++;

            /* and reset the buffer pointers */
            _png_ptr->zstream.avail_out = (uInt)_png_ptr->zbuf_size;
            _png_ptr->zstream.next_out = _png_ptr->zbuf;
         }
      }
      else if (ret != Z_STREAM_END)
      {
         /* We got an error */
         if (_png_ptr->zstream.msg != NULL)
            _png_error(_png_ptr, _png_ptr->zstream.msg);

         else
            _png_error(_png_ptr, "zlib error");
      }
   } while (ret != Z_STREAM_END);

   /* Text length is number of buffers plus last buffer */
   text_len = _png_ptr->zbuf_size * comp->num_output_ptr;

   if (_png_ptr->zstream.avail_out < _png_ptr->zbuf_size)
      text_len += _png_ptr->zbuf_size - (_png_size_t)_png_ptr->zstream.avail_out;

   return((int)text_len);
}

/* Ship the compressed text out via chunk writes */
static void /* PRIVATE */
_png_write_compressed_data_out(_png_structp _png_ptr, compression_state *comp,
   _png_size_t data_len)
{
   int i;

   /* Handle the no-compression case */
   if (comp->input)
   {
      _png_write_chunk_data(_png_ptr, comp->input, data_len);

      return;
   }

#ifdef PNG_WRITE_OPTIMIZE_CMF_SUPPORTED
   /* The zbuf_size test is because the code below doesn't work if zbuf_size is
    * '1'; simply skip it to avoid memory overwrite.
    */
   if (data_len >= 2 && comp->input_len < 16384 && _png_ptr->zbuf_size > 1)
   {
      unsigned int z_cmf;  /* zlib compression method and flags */

      /* Optimize the CMF field in the zlib stream.  This hack of the zlib
       * stream is compliant to the stream specification.
       */

      if (comp->num_output_ptr)
        z_cmf = comp->output_ptr[0][0];
      else
        z_cmf = _png_ptr->zbuf[0];

      if ((z_cmf & 0x0f) == 8 && (z_cmf & 0xf0) <= 0x70)
      {
         unsigned int z_cinfo;
         unsigned int half_z_window_size;
         _png_size_t uncompressed_text_size = comp->input_len;

         z_cinfo = z_cmf >> 4;
         half_z_window_size = 1 << (z_cinfo + 7);

         while (uncompressed_text_size <= half_z_window_size &&
             half_z_window_size >= 256)
         {
            z_cinfo--;
            half_z_window_size >>= 1;
         }

         z_cmf = (z_cmf & 0x0f) | (z_cinfo << 4);

         if (comp->num_output_ptr)
         {

           if (comp->output_ptr[0][0] != z_cmf)
           {
              int tmp;

              comp->output_ptr[0][0] = (_png_byte)z_cmf;
              tmp = comp->output_ptr[0][1] & 0xe0;
              tmp += 0x1f - ((z_cmf << 8) + tmp) % 0x1f;
              comp->output_ptr[0][1] = (_png_byte)tmp;
           }
         }
         else
         {
            int tmp;

            _png_ptr->zbuf[0] = (_png_byte)z_cmf;
            tmp = _png_ptr->zbuf[1] & 0xe0;
            tmp += 0x1f - ((z_cmf << 8) + tmp) % 0x1f;
            _png_ptr->zbuf[1] = (_png_byte)tmp;
         }
      }

      else
         _png_error(_png_ptr,
             "Invalid zlib compression method or flags in non-IDAT chunk");
   }
#endif /* PNG_WRITE_OPTIMIZE_CMF_SUPPORTED */

   /* Write saved output buffers, if any */
   for (i = 0; i < comp->num_output_ptr; i++)
   {
      _png_write_chunk_data(_png_ptr, comp->output_ptr[i],
          (_png_size_t)_png_ptr->zbuf_size);

      _png_free(_png_ptr, comp->output_ptr[i]);
   }

   if (comp->max_output_ptr != 0)
      _png_free(_png_ptr, comp->output_ptr);

   /* Write anything left in zbuf */
   if (_png_ptr->zstream.avail_out < (_png_uint_32)_png_ptr->zbuf_size)
      _png_write_chunk_data(_png_ptr, _png_ptr->zbuf,
          (_png_size_t)(_png_ptr->zbuf_size - _png_ptr->zstream.avail_out));

   /* Reset zlib for another zTXt/iTXt or image data */
   _png_zlib_release(_png_ptr);
}
#endif /* PNG_WRITE_COMPRESSED_TEXT_SUPPORTED */

/* Write the IHDR chunk, and update the _png_struct with the necessary
 * information.  Note that the rest of this code depends upon this
 * information being correct.
 */
void /* PRIVATE */
_png_write_IHDR(_png_structp _png_ptr, _png_uint_32 width, _png_uint_32 height,
    int bit_depth, int color_type, int compression_type, int filter_type,
    int interlace_type)
{
   _png_byte buf[13]; /* Buffer to store the IHDR info */

   _png_debug(1, "in _png_write_IHDR");

   /* Check that we have valid input data from the application info */
   switch (color_type)
   {
      case PNG_COLOR_TYPE_GRAY:
         switch (bit_depth)
         {
            case 1:
            case 2:
            case 4:
            case 8:
#ifdef PNG_WRITE_16BIT_SUPPORTED
            case 16:
#endif
               _png_ptr->channels = 1; break;

            default:
               _png_error(_png_ptr,
                   "Invalid bit depth for grayscale image");
         }
         break;

      case PNG_COLOR_TYPE_RGB:
#ifdef PNG_WRITE_16BIT_SUPPORTED
         if (bit_depth != 8 && bit_depth != 16)
#else
         if (bit_depth != 8)
#endif
            _png_error(_png_ptr, "Invalid bit depth for RGB image");

         _png_ptr->channels = 3;
         break;

      case PNG_COLOR_TYPE_PALETTE:
         switch (bit_depth)
         {
            case 1:
            case 2:
            case 4:
            case 8:
               _png_ptr->channels = 1;
               break;

            default:
               _png_error(_png_ptr, "Invalid bit depth for paletted image");
         }
         break;

      case PNG_COLOR_TYPE_GRAY_ALPHA:
         if (bit_depth != 8 && bit_depth != 16)
            _png_error(_png_ptr, "Invalid bit depth for grayscale+alpha image");

         _png_ptr->channels = 2;
         break;

      case PNG_COLOR_TYPE_RGB_ALPHA:
#ifdef PNG_WRITE_16BIT_SUPPORTED
         if (bit_depth != 8 && bit_depth != 16)
#else
         if (bit_depth != 8)
#endif
            _png_error(_png_ptr, "Invalid bit depth for RGBA image");

         _png_ptr->channels = 4;
         break;

      default:
         _png_error(_png_ptr, "Invalid image color type specified");
   }

   if (compression_type != PNG_COMPRESSION_TYPE_BASE)
   {
      _png_warning(_png_ptr, "Invalid compression type specified");
      compression_type = PNG_COMPRESSION_TYPE_BASE;
   }

   /* Write filter_method 64 (intrapixel differencing) only if
    * 1. Libpng was compiled with PNG_MNG_FEATURES_SUPPORTED and
    * 2. Libpng did not write a PNG signature (this filter_method is only
    *    used in PNG datastreams that are embedded in MNG datastreams) and
    * 3. The application called _png_permit_mng_features with a mask that
    *    included PNG_FLAG_MNG_FILTER_64 and
    * 4. The filter_method is 64 and
    * 5. The color_type is RGB or RGBA
    */
   if (
#ifdef PNG_MNG_FEATURES_SUPPORTED
       !((_png_ptr->mng_features_permitted & PNG_FLAG_MNG_FILTER_64) &&
       ((_png_ptr->mode&PNG_HAVE_PNG_SIGNATURE) == 0) &&
       (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_RGB_ALPHA) &&
       (filter_type == PNG_INTRAPIXEL_DIFFERENCING)) &&
#endif
       filter_type != PNG_FILTER_TYPE_BASE)
   {
      _png_warning(_png_ptr, "Invalid filter type specified");
      filter_type = PNG_FILTER_TYPE_BASE;
   }

#ifdef PNG_WRITE_INTERLACING_SUPPORTED
   if (interlace_type != PNG_INTERLACE_NONE &&
       interlace_type != PNG_INTERLACE_ADAM7)
   {
      _png_warning(_png_ptr, "Invalid interlace type specified");
      interlace_type = PNG_INTERLACE_ADAM7;
   }
#else
   interlace_type=PNG_INTERLACE_NONE;
#endif

   /* Save the relevent information */
   _png_ptr->bit_depth = (_png_byte)bit_depth;
   _png_ptr->color_type = (_png_byte)color_type;
   _png_ptr->interlaced = (_png_byte)interlace_type;
#ifdef PNG_MNG_FEATURES_SUPPORTED
   _png_ptr->filter_type = (_png_byte)filter_type;
#endif
   _png_ptr->compression_type = (_png_byte)compression_type;
   _png_ptr->width = width;
   _png_ptr->height = height;

   _png_ptr->pixel_depth = (_png_byte)(bit_depth * _png_ptr->channels);
   _png_ptr->rowbytes = PNG_ROWBYTES(_png_ptr->pixel_depth, width);
   /* Set the usr info, so any transformations can modify it */
   _png_ptr->usr_width = _png_ptr->width;
   _png_ptr->usr_bit_depth = _png_ptr->bit_depth;
   _png_ptr->usr_channels = _png_ptr->channels;

   /* Pack the header information into the buffer */
   _png_save_uint_32(buf, width);
   _png_save_uint_32(buf + 4, height);
   buf[8] = (_png_byte)bit_depth;
   buf[9] = (_png_byte)color_type;
   buf[10] = (_png_byte)compression_type;
   buf[11] = (_png_byte)filter_type;
   buf[12] = (_png_byte)interlace_type;

   /* Write the chunk */
   _png_write_complete_chunk(_png_ptr, _png_IHDR, buf, (_png_size_t)13);

   /* Initialize zlib with PNG info */
   _png_ptr->zstream.zalloc = _png_zalloc;
   _png_ptr->zstream.zfree = _png_zfree;
   _png_ptr->zstream.opaque = (voidpf)_png_ptr;

   if (!(_png_ptr->do_filter))
   {
      if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE ||
          _png_ptr->bit_depth < 8)
         _png_ptr->do_filter = PNG_FILTER_NONE;

      else
         _png_ptr->do_filter = PNG_ALL_FILTERS;
   }

   if (!(_png_ptr->flags & PNG_FLAG_ZLIB_CUSTOM_STRATEGY))
   {
      if (_png_ptr->do_filter != PNG_FILTER_NONE)
         _png_ptr->zlib_strategy = Z_FILTERED;

      else
         _png_ptr->zlib_strategy = Z_DEFAULT_STRATEGY;
   }

   if (!(_png_ptr->flags & PNG_FLAG_ZLIB_CUSTOM_LEVEL))
      _png_ptr->zlib_level = Z_DEFAULT_COMPRESSION;

   if (!(_png_ptr->flags & PNG_FLAG_ZLIB_CUSTOM_MEM_LEVEL))
      _png_ptr->zlib_mem_level = 8;

   if (!(_png_ptr->flags & PNG_FLAG_ZLIB_CUSTOM_WINDOW_BITS))
      _png_ptr->zlib_window_bits = 15;

   if (!(_png_ptr->flags & PNG_FLAG_ZLIB_CUSTOM_METHOD))
      _png_ptr->zlib_method = 8;

#ifdef PNG_WRITE_COMPRESSED_TEXT_SUPPORTED
#ifdef PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED
   if (!(_png_ptr->flags & PNG_FLAG_ZTXT_CUSTOM_STRATEGY))
      _png_ptr->zlib_text_strategy = Z_DEFAULT_STRATEGY;

   if (!(_png_ptr->flags & PNG_FLAG_ZTXT_CUSTOM_LEVEL))
      _png_ptr->zlib_text_level = _png_ptr->zlib_level;

   if (!(_png_ptr->flags & PNG_FLAG_ZTXT_CUSTOM_MEM_LEVEL))
      _png_ptr->zlib_text_mem_level = _png_ptr->zlib_mem_level;

   if (!(_png_ptr->flags & PNG_FLAG_ZTXT_CUSTOM_WINDOW_BITS))
      _png_ptr->zlib_text_window_bits = _png_ptr->zlib_window_bits;

   if (!(_png_ptr->flags & PNG_FLAG_ZTXT_CUSTOM_METHOD))
      _png_ptr->zlib_text_method = _png_ptr->zlib_method;
#else
   _png_ptr->zlib_text_strategy = Z_DEFAULT_STRATEGY;
   _png_ptr->zlib_text_level = _png_ptr->zlib_level;
   _png_ptr->zlib_text_mem_level = _png_ptr->zlib_mem_level;
   _png_ptr->zlib_text_window_bits = _png_ptr->zlib_window_bits;
   _png_ptr->zlib_text_method = _png_ptr->zlib_method;
#endif /* PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED */
#endif /* PNG_WRITE_COMPRESSED_TEXT_SUPPORTED */

   /* Record that the compressor has not yet been initialized. */
   _png_ptr->zlib_state = PNG_ZLIB_UNINITIALIZED;

   _png_ptr->mode = PNG_HAVE_IHDR; /* not READY_FOR_ZTXT */
}

/* Write the palette.  We are careful not to trust _png_color to be in the
 * correct order for PNG, so people can redefine it to any convenient
 * structure.
 */
void /* PRIVATE */
_png_write_PLTE(_png_structp _png_ptr, _png_const_colorp palette,
    _png_uint_32 num_pal)
{
   _png_uint_32 i;
   _png_const_colorp pal_ptr;
   _png_byte buf[3];

   _png_debug(1, "in _png_write_PLTE");

   if ((
#ifdef PNG_MNG_FEATURES_SUPPORTED
       !(_png_ptr->mng_features_permitted & PNG_FLAG_MNG_EMPTY_PLTE) &&
#endif
       num_pal == 0) || num_pal > 256)
   {
      if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      {
         _png_error(_png_ptr, "Invalid number of colors in palette");
      }

      else
      {
         _png_warning(_png_ptr, "Invalid number of colors in palette");
         return;
      }
   }

   if (!(_png_ptr->color_type&PNG_COLOR_MASK_COLOR))
   {
      _png_warning(_png_ptr,
          "Ignoring request to write a PLTE chunk in grayscale PNG");

      return;
   }

   _png_ptr->num_palette = (_png_uint_16)num_pal;
   _png_debug1(3, "num_palette = %d", _png_ptr->num_palette);

   _png_write_chunk_header(_png_ptr, _png_PLTE, (_png_uint_32)(num_pal * 3));
#ifdef PNG_POINTER_INDEXING_SUPPORTED

   for (i = 0, pal_ptr = palette; i < num_pal; i++, pal_ptr++)
   {
      buf[0] = pal_ptr->red;
      buf[1] = pal_ptr->green;
      buf[2] = pal_ptr->blue;
      _png_write_chunk_data(_png_ptr, buf, (_png_size_t)3);
   }

#else
   /* This is a little slower but some buggy compilers need to do this
    * instead
    */
   pal_ptr=palette;

   for (i = 0; i < num_pal; i++)
   {
      buf[0] = pal_ptr[i].red;
      buf[1] = pal_ptr[i].green;
      buf[2] = pal_ptr[i].blue;
      _png_write_chunk_data(_png_ptr, buf, (_png_size_t)3);
   }

#endif
   _png_write_chunk_end(_png_ptr);
   _png_ptr->mode |= PNG_HAVE_PLTE;
}

/* Write an IDAT chunk */
void /* PRIVATE */
_png_write_IDAT(_png_structp _png_ptr, _png_bytep data, _png_size_t length)
{
   _png_debug(1, "in _png_write_IDAT");

#ifdef PNG_WRITE_OPTIMIZE_CMF_SUPPORTED
   if (!(_png_ptr->mode & PNG_HAVE_IDAT) &&
       _png_ptr->compression_type == PNG_COMPRESSION_TYPE_BASE)
   {
      /* Optimize the CMF field in the zlib stream.  This hack of the zlib
       * stream is compliant to the stream specification.
       */
      unsigned int z_cmf = data[0];  /* zlib compression method and flags */

      if ((z_cmf & 0x0f) == 8 && (z_cmf & 0xf0) <= 0x70)
      {
         /* Avoid memory underflows and multiplication overflows.
          *
          * The conditions below are practically always satisfied;
          * however, they still must be checked.
          */
         if (length >= 2 &&
             _png_ptr->height < 16384 && _png_ptr->width < 16384)
         {
            /* Compute the maximum possible length of the datastream */

            /* Number of pixels, plus for each row a filter byte
             * and possibly a padding byte, so increase the maximum
             * size to account for these.
             */
            unsigned int z_cinfo;
            unsigned int half_z_window_size;
            _png_uint_32 uncompressed_idat_size = _png_ptr->height *
                ((_png_ptr->width *
                _png_ptr->channels * _png_ptr->bit_depth + 15) >> 3);

            /* If it's interlaced, each block of 8 rows is sent as up to
             * 14 rows, i.e., 6 additional rows, each with a filter byte
             * and possibly a padding byte
             */
            if (_png_ptr->interlaced)
               uncompressed_idat_size += ((_png_ptr->height + 7)/8) *
                   (_png_ptr->bit_depth < 8 ? 12 : 6);

            z_cinfo = z_cmf >> 4;
            half_z_window_size = 1 << (z_cinfo + 7);

            while (uncompressed_idat_size <= half_z_window_size &&
                half_z_window_size >= 256)
            {
               z_cinfo--;
               half_z_window_size >>= 1;
            }

            z_cmf = (z_cmf & 0x0f) | (z_cinfo << 4);

            if (data[0] != z_cmf)
            {
               int tmp;
               data[0] = (_png_byte)z_cmf;
               tmp = data[1] & 0xe0;
               tmp += 0x1f - ((z_cmf << 8) + tmp) % 0x1f;
               data[1] = (_png_byte)tmp;
            }
         }
      }

      else
         _png_error(_png_ptr,
             "Invalid zlib compression method or flags in IDAT");
   }
#endif /* PNG_WRITE_OPTIMIZE_CMF_SUPPORTED */

   _png_write_complete_chunk(_png_ptr, _png_IDAT, data, length);
   _png_ptr->mode |= PNG_HAVE_IDAT;

   /* Prior to 1.5.4 this code was replicated in every caller (except at the
    * end, where it isn't technically necessary).  Since this function has
    * flushed the data we can safely reset the zlib output buffer here.
    */
   _png_ptr->zstream.next_out = _png_ptr->zbuf;
   _png_ptr->zstream.avail_out = (uInt)_png_ptr->zbuf_size;
}

/* Write an IEND chunk */
void /* PRIVATE */
_png_write_IEND(_png_structp _png_ptr)
{
   _png_debug(1, "in _png_write_IEND");

   _png_write_complete_chunk(_png_ptr, _png_IEND, NULL, (_png_size_t)0);
   _png_ptr->mode |= PNG_HAVE_IEND;
}

#ifdef PNG_WRITE_gAMA_SUPPORTED
/* Write a gAMA chunk */
void /* PRIVATE */
_png_write_gAMA_fixed(_png_structp _png_ptr, _png_fixed_point file_gamma)
{
   _png_byte buf[4];

   _png_debug(1, "in _png_write_gAMA");

   /* file_gamma is saved in 1/100,000ths */
   _png_save_uint_32(buf, (_png_uint_32)file_gamma);
   _png_write_complete_chunk(_png_ptr, _png_gAMA, buf, (_png_size_t)4);
}
#endif

#ifdef PNG_WRITE_sRGB_SUPPORTED
/* Write a sRGB chunk */
void /* PRIVATE */
_png_write_sRGB(_png_structp _png_ptr, int srgb_intent)
{
   _png_byte buf[1];

   _png_debug(1, "in _png_write_sRGB");

   if (srgb_intent >= PNG_sRGB_INTENT_LAST)
      _png_warning(_png_ptr,
          "Invalid sRGB rendering intent specified");

   buf[0]=(_png_byte)srgb_intent;
   _png_write_complete_chunk(_png_ptr, _png_sRGB, buf, (_png_size_t)1);
}
#endif

#ifdef PNG_WRITE_iCCP_SUPPORTED
/* Write an iCCP chunk */
void /* PRIVATE */
_png_write_iCCP(_png_structp _png_ptr, _png_const_charp name, int compression_type,
    _png_const_charp profile, int profile_len)
{
   _png_size_t name_len;
   _png_charp new_name;
   compression_state comp;
   int embedded_profile_len = 0;

   _png_debug(1, "in _png_write_iCCP");

   comp.num_output_ptr = 0;
   comp.max_output_ptr = 0;
   comp.output_ptr = NULL;
   comp.input = NULL;
   comp.input_len = 0;

   if ((name_len = _png_check_keyword(_png_ptr, name, &new_name)) == 0)
      return;

   if (compression_type != PNG_COMPRESSION_TYPE_BASE)
      _png_warning(_png_ptr, "Unknown compression type in iCCP chunk");

   if (profile == NULL)
      profile_len = 0;

   if (profile_len > 3)
      embedded_profile_len =
          ((*( (_png_const_bytep)profile    ))<<24) |
          ((*( (_png_const_bytep)profile + 1))<<16) |
          ((*( (_png_const_bytep)profile + 2))<< 8) |
          ((*( (_png_const_bytep)profile + 3))    );

   if (embedded_profile_len < 0)
   {
      _png_warning(_png_ptr,
          "Embedded profile length in iCCP chunk is negative");

      _png_free(_png_ptr, new_name);
      return;
   }

   if (profile_len < embedded_profile_len)
   {
      _png_warning(_png_ptr,
          "Embedded profile length too large in iCCP chunk");

      _png_free(_png_ptr, new_name);
      return;
   }

   if (profile_len > embedded_profile_len)
   {
      _png_warning(_png_ptr,
          "Truncating profile to actual length in iCCP chunk");

      profile_len = embedded_profile_len;
   }

   if (profile_len)
      profile_len = _png_text_compress(_png_ptr, profile,
          (_png_size_t)profile_len, PNG_COMPRESSION_TYPE_BASE, &comp);

   /* Make sure we include the NULL after the name and the compression type */
   _png_write_chunk_header(_png_ptr, _png_iCCP,
       (_png_uint_32)(name_len + profile_len + 2));

   new_name[name_len + 1] = 0x00;

   _png_write_chunk_data(_png_ptr, (_png_bytep)new_name,
       (_png_size_t)(name_len + 2));

   if (profile_len)
   {
      _png_write_compressed_data_out(_png_ptr, &comp, profile_len);
   }

   _png_write_chunk_end(_png_ptr);
   _png_free(_png_ptr, new_name);
}
#endif

#ifdef PNG_WRITE_sPLT_SUPPORTED
/* Write a sPLT chunk */
void /* PRIVATE */
_png_write_sPLT(_png_structp _png_ptr, _png_const_sPLT_tp spalette)
{
   _png_size_t name_len;
   _png_charp new_name;
   _png_byte entrybuf[10];
   _png_size_t entry_size = (spalette->depth == 8 ? 6 : 10);
   _png_size_t palette_size = entry_size * spalette->nentries;
   _png_sPLT_entryp ep;
#ifndef PNG_POINTER_INDEXING_SUPPORTED
   int i;
#endif

   _png_debug(1, "in _png_write_sPLT");

   if ((name_len = _png_check_keyword(_png_ptr,spalette->name, &new_name))==0)
      return;

   /* Make sure we include the NULL after the name */
   _png_write_chunk_header(_png_ptr, _png_sPLT,
       (_png_uint_32)(name_len + 2 + palette_size));

   _png_write_chunk_data(_png_ptr, (_png_bytep)new_name,
       (_png_size_t)(name_len + 1));

   _png_write_chunk_data(_png_ptr, &spalette->depth, (_png_size_t)1);

   /* Loop through each palette entry, writing appropriately */
#ifdef PNG_POINTER_INDEXING_SUPPORTED
   for (ep = spalette->entries; ep<spalette->entries + spalette->nentries; ep++)
   {
      if (spalette->depth == 8)
      {
         entrybuf[0] = (_png_byte)ep->red;
         entrybuf[1] = (_png_byte)ep->green;
         entrybuf[2] = (_png_byte)ep->blue;
         entrybuf[3] = (_png_byte)ep->alpha;
         _png_save_uint_16(entrybuf + 4, ep->frequency);
      }

      else
      {
         _png_save_uint_16(entrybuf + 0, ep->red);
         _png_save_uint_16(entrybuf + 2, ep->green);
         _png_save_uint_16(entrybuf + 4, ep->blue);
         _png_save_uint_16(entrybuf + 6, ep->alpha);
         _png_save_uint_16(entrybuf + 8, ep->frequency);
      }

      _png_write_chunk_data(_png_ptr, entrybuf, (_png_size_t)entry_size);
   }
#else
   ep=spalette->entries;
   for (i = 0; i>spalette->nentries; i++)
   {
      if (spalette->depth == 8)
      {
         entrybuf[0] = (_png_byte)ep[i].red;
         entrybuf[1] = (_png_byte)ep[i].green;
         entrybuf[2] = (_png_byte)ep[i].blue;
         entrybuf[3] = (_png_byte)ep[i].alpha;
         _png_save_uint_16(entrybuf + 4, ep[i].frequency);
      }

      else
      {
         _png_save_uint_16(entrybuf + 0, ep[i].red);
         _png_save_uint_16(entrybuf + 2, ep[i].green);
         _png_save_uint_16(entrybuf + 4, ep[i].blue);
         _png_save_uint_16(entrybuf + 6, ep[i].alpha);
         _png_save_uint_16(entrybuf + 8, ep[i].frequency);
      }

      _png_write_chunk_data(_png_ptr, entrybuf, (_png_size_t)entry_size);
   }
#endif

   _png_write_chunk_end(_png_ptr);
   _png_free(_png_ptr, new_name);
}
#endif

#ifdef PNG_WRITE_sBIT_SUPPORTED
/* Write the sBIT chunk */
void /* PRIVATE */
_png_write_sBIT(_png_structp _png_ptr, _png_const_color_8p sbit, int color_type)
{
   _png_byte buf[4];
   _png_size_t size;

   _png_debug(1, "in _png_write_sBIT");

   /* Make sure we don't depend upon the order of PNG_COLOR_8 */
   if (color_type & PNG_COLOR_MASK_COLOR)
   {
      _png_byte maxbits;

      maxbits = (_png_byte)(color_type==PNG_COLOR_TYPE_PALETTE ? 8 :
          _png_ptr->usr_bit_depth);

      if (sbit->red == 0 || sbit->red > maxbits ||
          sbit->green == 0 || sbit->green > maxbits ||
          sbit->blue == 0 || sbit->blue > maxbits)
      {
         _png_warning(_png_ptr, "Invalid sBIT depth specified");
         return;
      }

      buf[0] = sbit->red;
      buf[1] = sbit->green;
      buf[2] = sbit->blue;
      size = 3;
   }

   else
   {
      if (sbit->gray == 0 || sbit->gray > _png_ptr->usr_bit_depth)
      {
         _png_warning(_png_ptr, "Invalid sBIT depth specified");
         return;
      }

      buf[0] = sbit->gray;
      size = 1;
   }

   if (color_type & PNG_COLOR_MASK_ALPHA)
   {
      if (sbit->alpha == 0 || sbit->alpha > _png_ptr->usr_bit_depth)
      {
         _png_warning(_png_ptr, "Invalid sBIT depth specified");
         return;
      }

      buf[size++] = sbit->alpha;
   }

   _png_write_complete_chunk(_png_ptr, _png_sBIT, buf, size);
}
#endif

#ifdef PNG_WRITE_cHRM_SUPPORTED
/* Write the cHRM chunk */
void /* PRIVATE */
_png_write_cHRM_fixed(_png_structp _png_ptr, _png_fixed_point white_x,
    _png_fixed_point white_y, _png_fixed_point red_x, _png_fixed_point red_y,
    _png_fixed_point green_x, _png_fixed_point green_y, _png_fixed_point blue_x,
    _png_fixed_point blue_y)
{
   _png_byte buf[32];

   _png_debug(1, "in _png_write_cHRM");

   /* Each value is saved in 1/100,000ths */
#ifdef PNG_CHECK_cHRM_SUPPORTED
   if (_png_check_cHRM_fixed(_png_ptr, white_x, white_y, red_x, red_y,
       green_x, green_y, blue_x, blue_y))
#endif
   {
      _png_save_uint_32(buf, (_png_uint_32)white_x);
      _png_save_uint_32(buf + 4, (_png_uint_32)white_y);

      _png_save_uint_32(buf + 8, (_png_uint_32)red_x);
      _png_save_uint_32(buf + 12, (_png_uint_32)red_y);

      _png_save_uint_32(buf + 16, (_png_uint_32)green_x);
      _png_save_uint_32(buf + 20, (_png_uint_32)green_y);

      _png_save_uint_32(buf + 24, (_png_uint_32)blue_x);
      _png_save_uint_32(buf + 28, (_png_uint_32)blue_y);

      _png_write_complete_chunk(_png_ptr, _png_cHRM, buf, (_png_size_t)32);
   }
}
#endif

#ifdef PNG_WRITE_tRNS_SUPPORTED
/* Write the tRNS chunk */
void /* PRIVATE */
_png_write_tRNS(_png_structp _png_ptr, _png_const_bytep trans_alpha,
    _png_const_color_16p tran, int num_trans, int color_type)
{
   _png_byte buf[6];

   _png_debug(1, "in _png_write_tRNS");

   if (color_type == PNG_COLOR_TYPE_PALETTE)
   {
      if (num_trans <= 0 || num_trans > (int)_png_ptr->num_palette)
      {
         _png_warning(_png_ptr, "Invalid number of transparent colors specified");
         return;
      }

      /* Write the chunk out as it is */
      _png_write_complete_chunk(_png_ptr, _png_tRNS, trans_alpha,
         (_png_size_t)num_trans);
   }

   else if (color_type == PNG_COLOR_TYPE_GRAY)
   {
      /* One 16 bit value */
      if (tran->gray >= (1 << _png_ptr->bit_depth))
      {
         _png_warning(_png_ptr,
             "Ignoring attempt to write tRNS chunk out-of-range for bit_depth");

         return;
      }

      _png_save_uint_16(buf, tran->gray);
      _png_write_complete_chunk(_png_ptr, _png_tRNS, buf, (_png_size_t)2);
   }

   else if (color_type == PNG_COLOR_TYPE_RGB)
   {
      /* Three 16 bit values */
      _png_save_uint_16(buf, tran->red);
      _png_save_uint_16(buf + 2, tran->green);
      _png_save_uint_16(buf + 4, tran->blue);
#ifdef PNG_WRITE_16BIT_SUPPORTED
      if (_png_ptr->bit_depth == 8 && (buf[0] | buf[2] | buf[4]))
#else
      if (buf[0] | buf[2] | buf[4])
#endif
      {
         _png_warning(_png_ptr,
           "Ignoring attempt to write 16-bit tRNS chunk when bit_depth is 8");
         return;
      }

      _png_write_complete_chunk(_png_ptr, _png_tRNS, buf, (_png_size_t)6);
   }

   else
   {
      _png_warning(_png_ptr, "Can't write tRNS with an alpha channel");
   }
}
#endif

#ifdef PNG_WRITE_bKGD_SUPPORTED
/* Write the background chunk */
void /* PRIVATE */
_png_write_bKGD(_png_structp _png_ptr, _png_const_color_16p back, int color_type)
{
   _png_byte buf[6];

   _png_debug(1, "in _png_write_bKGD");

   if (color_type == PNG_COLOR_TYPE_PALETTE)
   {
      if (
#ifdef PNG_MNG_FEATURES_SUPPORTED
          (_png_ptr->num_palette ||
          (!(_png_ptr->mng_features_permitted & PNG_FLAG_MNG_EMPTY_PLTE))) &&
#endif
         back->index >= _png_ptr->num_palette)
      {
         _png_warning(_png_ptr, "Invalid background palette index");
         return;
      }

      buf[0] = back->index;
      _png_write_complete_chunk(_png_ptr, _png_bKGD, buf, (_png_size_t)1);
   }

   else if (color_type & PNG_COLOR_MASK_COLOR)
   {
      _png_save_uint_16(buf, back->red);
      _png_save_uint_16(buf + 2, back->green);
      _png_save_uint_16(buf + 4, back->blue);
#ifdef PNG_WRITE_16BIT_SUPPORTED
      if (_png_ptr->bit_depth == 8 && (buf[0] | buf[2] | buf[4]))
#else
      if (buf[0] | buf[2] | buf[4])
#endif
      {
         _png_warning(_png_ptr,
             "Ignoring attempt to write 16-bit bKGD chunk when bit_depth is 8");

         return;
      }

      _png_write_complete_chunk(_png_ptr, _png_bKGD, buf, (_png_size_t)6);
   }

   else
   {
      if (back->gray >= (1 << _png_ptr->bit_depth))
      {
         _png_warning(_png_ptr,
             "Ignoring attempt to write bKGD chunk out-of-range for bit_depth");

         return;
      }

      _png_save_uint_16(buf, back->gray);
      _png_write_complete_chunk(_png_ptr, _png_bKGD, buf, (_png_size_t)2);
   }
}
#endif

#ifdef PNG_WRITE_hIST_SUPPORTED
/* Write the histogram */
void /* PRIVATE */
_png_write_hIST(_png_structp _png_ptr, _png_const_uint_16p hist, int num_hist)
{
   int i;
   _png_byte buf[3];

   _png_debug(1, "in _png_write_hIST");

   if (num_hist > (int)_png_ptr->num_palette)
   {
      _png_debug2(3, "num_hist = %d, num_palette = %d", num_hist,
          _png_ptr->num_palette);

      _png_warning(_png_ptr, "Invalid number of histogram entries specified");
      return;
   }

   _png_write_chunk_header(_png_ptr, _png_hIST, (_png_uint_32)(num_hist * 2));

   for (i = 0; i < num_hist; i++)
   {
      _png_save_uint_16(buf, hist[i]);
      _png_write_chunk_data(_png_ptr, buf, (_png_size_t)2);
   }

   _png_write_chunk_end(_png_ptr);
}
#endif

#if defined(PNG_WRITE_TEXT_SUPPORTED) || defined(PNG_WRITE_pCAL_SUPPORTED) || \
    defined(PNG_WRITE_iCCP_SUPPORTED) || defined(PNG_WRITE_sPLT_SUPPORTED)
/* Check that the tEXt or zTXt keyword is valid per PNG 1.0 specification,
 * and if invalid, correct the keyword rather than discarding the entire
 * chunk.  The PNG 1.0 specification requires keywords 1-79 characters in
 * length, forbids leading or trailing whitespace, multiple internal spaces,
 * and the non-break space (0x80) from ISO 8859-1.  Returns keyword length.
 *
 * The new_key is allocated to hold the corrected keyword and must be freed
 * by the calling routine.  This avoids problems with trying to write to
 * static keywords without having to have duplicate copies of the strings.
 */
_png_size_t /* PRIVATE */
_png_check_keyword(_png_structp _png_ptr, _png_const_charp key, _png_charpp new_key)
{
   _png_size_t key_len;
   _png_const_charp ikp;
   _png_charp kp, dp;
   int kflag;
   int kwarn=0;

   _png_debug(1, "in _png_check_keyword");

   *new_key = NULL;

   if (key == NULL || (key_len = _png_strlen(key)) == 0)
   {
      _png_warning(_png_ptr, "zero length keyword");
      return ((_png_size_t)0);
   }

   _png_debug1(2, "Keyword to be checked is '%s'", key);

   *new_key = (_png_charp)_png_malloc_warn(_png_ptr, (_png_uint_32)(key_len + 2));

   if (*new_key == NULL)
   {
      _png_warning(_png_ptr, "Out of memory while procesing keyword");
      return ((_png_size_t)0);
   }

   /* Replace non-printing characters with a blank and print a warning */
   for (ikp = key, dp = *new_key; *ikp != '\0'; ikp++, dp++)
   {
      if ((_png_byte)*ikp < 0x20 ||
         ((_png_byte)*ikp > 0x7E && (_png_byte)*ikp < 0xA1))
      {
         PNG_WARNING_PARAMETERS(p)

         _png_warning_parameter_unsigned(p, 1, PNG_NUMBER_FORMAT_02x,
            (_png_byte)*ikp);
         _png_formatted_warning(_png_ptr, p, "invalid keyword character 0x@1");
         *dp = ' ';
      }

      else
      {
         *dp = *ikp;
      }
   }
   *dp = '\0';

   /* Remove any trailing white space. */
   kp = *new_key + key_len - 1;
   if (*kp == ' ')
   {
      _png_warning(_png_ptr, "trailing spaces removed from keyword");

      while (*kp == ' ')
      {
         *(kp--) = '\0';
         key_len--;
      }
   }

   /* Remove any leading white space. */
   kp = *new_key;
   if (*kp == ' ')
   {
      _png_warning(_png_ptr, "leading spaces removed from keyword");

      while (*kp == ' ')
      {
         kp++;
         key_len--;
      }
   }

   _png_debug1(2, "Checking for multiple internal spaces in '%s'", kp);

   /* Remove multiple internal spaces. */
   for (kflag = 0, dp = *new_key; *kp != '\0'; kp++)
   {
      if (*kp == ' ' && kflag == 0)
      {
         *(dp++) = *kp;
         kflag = 1;
      }

      else if (*kp == ' ')
      {
         key_len--;
         kwarn = 1;
      }

      else
      {
         *(dp++) = *kp;
         kflag = 0;
      }
   }
   *dp = '\0';
   if (kwarn)
      _png_warning(_png_ptr, "extra interior spaces removed from keyword");

   if (key_len == 0)
   {
      _png_free(_png_ptr, *new_key);
      _png_warning(_png_ptr, "Zero length keyword");
   }

   if (key_len > 79)
   {
      _png_warning(_png_ptr, "keyword length must be 1 - 79 characters");
      (*new_key)[79] = '\0';
      key_len = 79;
   }

   return (key_len);
}
#endif

#ifdef PNG_WRITE_tEXt_SUPPORTED
/* Write a tEXt chunk */
void /* PRIVATE */
_png_write_tEXt(_png_structp _png_ptr, _png_const_charp key, _png_const_charp text,
    _png_size_t text_len)
{
   _png_size_t key_len;
   _png_charp new_key;

   _png_debug(1, "in _png_write_tEXt");

   if ((key_len = _png_check_keyword(_png_ptr, key, &new_key))==0)
      return;

   if (text == NULL || *text == '\0')
      text_len = 0;

   else
      text_len = _png_strlen(text);

   /* Make sure we include the 0 after the key */
   _png_write_chunk_header(_png_ptr, _png_tEXt,
       (_png_uint_32)(key_len + text_len + 1));
   /*
    * We leave it to the application to meet PNG-1.0 requirements on the
    * contents of the text.  PNG-1.0 through PNG-1.2 discourage the use of
    * any non-Latin-1 characters except for NEWLINE.  ISO PNG will forbid them.
    * The NUL character is forbidden by PNG-1.0 through PNG-1.2 and ISO PNG.
    */
   _png_write_chunk_data(_png_ptr, (_png_bytep)new_key,
       (_png_size_t)(key_len + 1));

   if (text_len)
      _png_write_chunk_data(_png_ptr, (_png_const_bytep)text,
          (_png_size_t)text_len);

   _png_write_chunk_end(_png_ptr);
   _png_free(_png_ptr, new_key);
}
#endif

#ifdef PNG_WRITE_zTXt_SUPPORTED
/* Write a compressed text chunk */
void /* PRIVATE */
_png_write_zTXt(_png_structp _png_ptr, _png_const_charp key, _png_const_charp text,
    _png_size_t text_len, int compression)
{
   _png_size_t key_len;
   _png_byte buf;
   _png_charp new_key;
   compression_state comp;

   _png_debug(1, "in _png_write_zTXt");

   comp.num_output_ptr = 0;
   comp.max_output_ptr = 0;
   comp.output_ptr = NULL;
   comp.input = NULL;
   comp.input_len = 0;

   if ((key_len = _png_check_keyword(_png_ptr, key, &new_key)) == 0)
   {
      _png_free(_png_ptr, new_key);
      return;
   }

   if (text == NULL || *text == '\0' || compression==PNG_TEXT_COMPRESSION_NONE)
   {
      _png_write_tEXt(_png_ptr, new_key, text, (_png_size_t)0);
      _png_free(_png_ptr, new_key);
      return;
   }

   text_len = _png_strlen(text);

   /* Compute the compressed data; do it now for the length */
   text_len = _png_text_compress(_png_ptr, text, text_len, compression,
       &comp);

   /* Write start of chunk */
   _png_write_chunk_header(_png_ptr, _png_zTXt,
       (_png_uint_32)(key_len+text_len + 2));

   /* Write key */
   _png_write_chunk_data(_png_ptr, (_png_bytep)new_key,
       (_png_size_t)(key_len + 1));

   _png_free(_png_ptr, new_key);

   buf = (_png_byte)compression;

   /* Write compression */
   _png_write_chunk_data(_png_ptr, &buf, (_png_size_t)1);

   /* Write the compressed data */
   _png_write_compressed_data_out(_png_ptr, &comp, text_len);

   /* Close the chunk */
   _png_write_chunk_end(_png_ptr);
}
#endif

#ifdef PNG_WRITE_iTXt_SUPPORTED
/* Write an iTXt chunk */
void /* PRIVATE */
_png_write_iTXt(_png_structp _png_ptr, int compression, _png_const_charp key,
    _png_const_charp lang, _png_const_charp lang_key, _png_const_charp text)
{
   _png_size_t lang_len, key_len, lang_key_len, text_len;
   _png_charp new_lang;
   _png_charp new_key = NULL;
   _png_byte cbuf[2];
   compression_state comp;

   _png_debug(1, "in _png_write_iTXt");

   comp.num_output_ptr = 0;
   comp.max_output_ptr = 0;
   comp.output_ptr = NULL;
   comp.input = NULL;

   if ((key_len = _png_check_keyword(_png_ptr, key, &new_key)) == 0)
      return;

   if ((lang_len = _png_check_keyword(_png_ptr, lang, &new_lang)) == 0)
   {
      _png_warning(_png_ptr, "Empty language field in iTXt chunk");
      new_lang = NULL;
      lang_len = 0;
   }

   if (lang_key == NULL)
      lang_key_len = 0;

   else
      lang_key_len = _png_strlen(lang_key);

   if (text == NULL)
      text_len = 0;

   else
      text_len = _png_strlen(text);

   /* Compute the compressed data; do it now for the length */
   text_len = _png_text_compress(_png_ptr, text, text_len, compression - 2,
       &comp);


   /* Make sure we include the compression flag, the compression byte,
    * and the NULs after the key, lang, and lang_key parts
    */

   _png_write_chunk_header(_png_ptr, _png_iTXt, (_png_uint_32)(
        5 /* comp byte, comp flag, terminators for key, lang and lang_key */
        + key_len
        + lang_len
        + lang_key_len
        + text_len));

   /* We leave it to the application to meet PNG-1.0 requirements on the
    * contents of the text.  PNG-1.0 through PNG-1.2 discourage the use of
    * any non-Latin-1 characters except for NEWLINE.  ISO PNG will forbid them.
    * The NUL character is forbidden by PNG-1.0 through PNG-1.2 and ISO PNG.
    */
   _png_write_chunk_data(_png_ptr, (_png_bytep)new_key, (_png_size_t)(key_len + 1));

   /* Set the compression flag */
   if (compression == PNG_ITXT_COMPRESSION_NONE ||
       compression == PNG_TEXT_COMPRESSION_NONE)
      cbuf[0] = 0;

   else /* compression == PNG_ITXT_COMPRESSION_zTXt */
      cbuf[0] = 1;

   /* Set the compression method */
   cbuf[1] = 0;

   _png_write_chunk_data(_png_ptr, cbuf, (_png_size_t)2);

   cbuf[0] = 0;
   _png_write_chunk_data(_png_ptr, (new_lang ? (_png_const_bytep)new_lang : cbuf),
       (_png_size_t)(lang_len + 1));

   _png_write_chunk_data(_png_ptr, (lang_key ? (_png_const_bytep)lang_key : cbuf),
       (_png_size_t)(lang_key_len + 1));

   _png_write_compressed_data_out(_png_ptr, &comp, text_len);

   _png_write_chunk_end(_png_ptr);

   _png_free(_png_ptr, new_key);
   _png_free(_png_ptr, new_lang);
}
#endif

#ifdef PNG_WRITE_oFFs_SUPPORTED
/* Write the oFFs chunk */
void /* PRIVATE */
_png_write_oFFs(_png_structp _png_ptr, _png_int_32 x_offset, _png_int_32 y_offset,
    int unit_type)
{
   _png_byte buf[9];

   _png_debug(1, "in _png_write_oFFs");

   if (unit_type >= PNG_OFFSET_LAST)
      _png_warning(_png_ptr, "Unrecognized unit type for oFFs chunk");

   _png_save_int_32(buf, x_offset);
   _png_save_int_32(buf + 4, y_offset);
   buf[8] = (_png_byte)unit_type;

   _png_write_complete_chunk(_png_ptr, _png_oFFs, buf, (_png_size_t)9);
}
#endif
#ifdef PNG_WRITE_pCAL_SUPPORTED
/* Write the pCAL chunk (described in the PNG extensions document) */
void /* PRIVATE */
_png_write_pCAL(_png_structp _png_ptr, _png_charp purpose, _png_int_32 X0,
    _png_int_32 X1, int type, int nparams, _png_const_charp units,
    _png_charpp params)
{
   _png_size_t purpose_len, units_len, total_len;
   _png_size_tp params_len;
   _png_byte buf[10];
   _png_charp new_purpose;
   int i;

   _png_debug1(1, "in _png_write_pCAL (%d parameters)", nparams);

   if (type >= PNG_EQUATION_LAST)
      _png_warning(_png_ptr, "Unrecognized equation type for pCAL chunk");

   purpose_len = _png_check_keyword(_png_ptr, purpose, &new_purpose) + 1;
   _png_debug1(3, "pCAL purpose length = %d", (int)purpose_len);
   units_len = _png_strlen(units) + (nparams == 0 ? 0 : 1);
   _png_debug1(3, "pCAL units length = %d", (int)units_len);
   total_len = purpose_len + units_len + 10;

   params_len = (_png_size_tp)_png_malloc(_png_ptr,
       (_png_alloc_size_t)(nparams * _png_sizeof(_png_size_t)));

   /* Find the length of each parameter, making sure we don't count the
    * null terminator for the last parameter.
    */
   for (i = 0; i < nparams; i++)
   {
      params_len[i] = _png_strlen(params[i]) + (i == nparams - 1 ? 0 : 1);
      _png_debug2(3, "pCAL parameter %d length = %lu", i,
          (unsigned long)params_len[i]);
      total_len += params_len[i];
   }

   _png_debug1(3, "pCAL total length = %d", (int)total_len);
   _png_write_chunk_header(_png_ptr, _png_pCAL, (_png_uint_32)total_len);
   _png_write_chunk_data(_png_ptr, (_png_const_bytep)new_purpose, purpose_len);
   _png_save_int_32(buf, X0);
   _png_save_int_32(buf + 4, X1);
   buf[8] = (_png_byte)type;
   buf[9] = (_png_byte)nparams;
   _png_write_chunk_data(_png_ptr, buf, (_png_size_t)10);
   _png_write_chunk_data(_png_ptr, (_png_const_bytep)units, (_png_size_t)units_len);

   _png_free(_png_ptr, new_purpose);

   for (i = 0; i < nparams; i++)
   {
      _png_write_chunk_data(_png_ptr, (_png_const_bytep)params[i], params_len[i]);
   }

   _png_free(_png_ptr, params_len);
   _png_write_chunk_end(_png_ptr);
}
#endif

#ifdef PNG_WRITE_sCAL_SUPPORTED
/* Write the sCAL chunk */
void /* PRIVATE */
_png_write_sCAL_s(_png_structp _png_ptr, int unit, _png_const_charp width,
    _png_const_charp height)
{
   _png_byte buf[64];
   _png_size_t wlen, hlen, total_len;

   _png_debug(1, "in _png_write_sCAL_s");

   wlen = _png_strlen(width);
   hlen = _png_strlen(height);
   total_len = wlen + hlen + 2;

   if (total_len > 64)
   {
      _png_warning(_png_ptr, "Can't write sCAL (buffer too small)");
      return;
   }

   buf[0] = (_png_byte)unit;
   _png_memcpy(buf + 1, width, wlen + 1);      /* Append the '\0' here */
   _png_memcpy(buf + wlen + 2, height, hlen);  /* Do NOT append the '\0' here */

   _png_debug1(3, "sCAL total length = %u", (unsigned int)total_len);
   _png_write_complete_chunk(_png_ptr, _png_sCAL, buf, total_len);
}
#endif

#ifdef PNG_WRITE_pHYs_SUPPORTED
/* Write the pHYs chunk */
void /* PRIVATE */
_png_write_pHYs(_png_structp _png_ptr, _png_uint_32 x_pixels_per_unit,
    _png_uint_32 y_pixels_per_unit,
    int unit_type)
{
   _png_byte buf[9];

   _png_debug(1, "in _png_write_pHYs");

   if (unit_type >= PNG_RESOLUTION_LAST)
      _png_warning(_png_ptr, "Unrecognized unit type for pHYs chunk");

   _png_save_uint_32(buf, x_pixels_per_unit);
   _png_save_uint_32(buf + 4, y_pixels_per_unit);
   buf[8] = (_png_byte)unit_type;

   _png_write_complete_chunk(_png_ptr, _png_pHYs, buf, (_png_size_t)9);
}
#endif

#ifdef PNG_WRITE_tIME_SUPPORTED
/* Write the tIME chunk.  Use either _png_convert_from_struct_tm()
 * or _png_convert_from_time_t(), or fill in the structure yourself.
 */
void /* PRIVATE */
_png_write_tIME(_png_structp _png_ptr, _png_const_timep mod_time)
{
   _png_byte buf[7];

   _png_debug(1, "in _png_write_tIME");

   if (mod_time->month  > 12 || mod_time->month  < 1 ||
       mod_time->day    > 31 || mod_time->day    < 1 ||
       mod_time->hour   > 23 || mod_time->second > 60)
   {
      _png_warning(_png_ptr, "Invalid time specified for tIME chunk");
      return;
   }

   _png_save_uint_16(buf, mod_time->year);
   buf[2] = mod_time->month;
   buf[3] = mod_time->day;
   buf[4] = mod_time->hour;
   buf[5] = mod_time->minute;
   buf[6] = mod_time->second;

   _png_write_complete_chunk(_png_ptr, _png_tIME, buf, (_png_size_t)7);
}
#endif

/* Initializes the row writing capability of libpng */
void /* PRIVATE */
_png_write_start_row(_png_structp _png_ptr)
{
#ifdef PNG_WRITE_INTERLACING_SUPPORTED
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

   _png_alloc_size_t buf_size;
   int usr_pixel_depth;

   _png_debug(1, "in _png_write_start_row");

   usr_pixel_depth = _png_ptr->usr_channels * _png_ptr->usr_bit_depth;
   buf_size = PNG_ROWBYTES(usr_pixel_depth, _png_ptr->width) + 1;

   /* 1.5.6: added to allow checking in the row write code. */
   _png_ptr->transformed_pixel_depth = _png_ptr->pixel_depth;
   _png_ptr->maximum_pixel_depth = (_png_byte)usr_pixel_depth;

   /* Set up row buffer */
   _png_ptr->row_buf = (_png_bytep)_png_malloc(_png_ptr, buf_size);

   _png_ptr->row_buf[0] = PNG_FILTER_VALUE_NONE;

#ifdef PNG_WRITE_FILTER_SUPPORTED
   /* Set up filtering buffer, if using this filter */
   if (_png_ptr->do_filter & PNG_FILTER_SUB)
   {
      _png_ptr->sub_row = (_png_bytep)_png_malloc(_png_ptr, _png_ptr->rowbytes + 1);

      _png_ptr->sub_row[0] = PNG_FILTER_VALUE_SUB;
   }

   /* We only need to keep the previous row if we are using one of these. */
   if (_png_ptr->do_filter & (PNG_FILTER_AVG | PNG_FILTER_UP | PNG_FILTER_PAETH))
   {
      /* Set up previous row buffer */
      _png_ptr->prev_row = (_png_bytep)_png_calloc(_png_ptr, buf_size);

      if (_png_ptr->do_filter & PNG_FILTER_UP)
      {
         _png_ptr->up_row = (_png_bytep)_png_malloc(_png_ptr,
            _png_ptr->rowbytes + 1);

         _png_ptr->up_row[0] = PNG_FILTER_VALUE_UP;
      }

      if (_png_ptr->do_filter & PNG_FILTER_AVG)
      {
         _png_ptr->avg_row = (_png_bytep)_png_malloc(_png_ptr,
             _png_ptr->rowbytes + 1);

         _png_ptr->avg_row[0] = PNG_FILTER_VALUE_AVG;
      }

      if (_png_ptr->do_filter & PNG_FILTER_PAETH)
      {
         _png_ptr->paeth_row = (_png_bytep)_png_malloc(_png_ptr,
             _png_ptr->rowbytes + 1);

         _png_ptr->paeth_row[0] = PNG_FILTER_VALUE_PAETH;
      }
   }
#endif /* PNG_WRITE_FILTER_SUPPORTED */

#ifdef PNG_WRITE_INTERLACING_SUPPORTED
   /* If interlaced, we need to set up width and height of pass */
   if (_png_ptr->interlaced)
   {
      if (!(_png_ptr->transformations & PNG_INTERLACE))
      {
         _png_ptr->num_rows = (_png_ptr->height + _png_pass_yinc[0] - 1 -
             _png_pass_ystart[0]) / _png_pass_yinc[0];

         _png_ptr->usr_width = (_png_ptr->width + _png_pass_inc[0] - 1 -
             _png_pass_start[0]) / _png_pass_inc[0];
      }

      else
      {
         _png_ptr->num_rows = _png_ptr->height;
         _png_ptr->usr_width = _png_ptr->width;
      }
   }

   else
#endif
   {
      _png_ptr->num_rows = _png_ptr->height;
      _png_ptr->usr_width = _png_ptr->width;
   }

   _png_zlib_claim(_png_ptr, PNG_ZLIB_FOR_IDAT);
   _png_ptr->zstream.avail_out = (uInt)_png_ptr->zbuf_size;
   _png_ptr->zstream.next_out = _png_ptr->zbuf;
}

/* Internal use only.  Called when finished processing a row of data. */
void /* PRIVATE */
_png_write_finish_row(_png_structp _png_ptr)
{
#ifdef PNG_WRITE_INTERLACING_SUPPORTED
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

   int ret;

   _png_debug(1, "in _png_write_finish_row");

   /* Next row */
   _png_ptr->row_number++;

   /* See if we are done */
   if (_png_ptr->row_number < _png_ptr->num_rows)
      return;

#ifdef PNG_WRITE_INTERLACING_SUPPORTED
   /* If interlaced, go to next pass */
   if (_png_ptr->interlaced)
   {
      _png_ptr->row_number = 0;
      if (_png_ptr->transformations & PNG_INTERLACE)
      {
         _png_ptr->pass++;
      }

      else
      {
         /* Loop until we find a non-zero width or height pass */
         do
         {
            _png_ptr->pass++;

            if (_png_ptr->pass >= 7)
               break;

            _png_ptr->usr_width = (_png_ptr->width +
                _png_pass_inc[_png_ptr->pass] - 1 -
                _png_pass_start[_png_ptr->pass]) /
                _png_pass_inc[_png_ptr->pass];

            _png_ptr->num_rows = (_png_ptr->height +
                _png_pass_yinc[_png_ptr->pass] - 1 -
                _png_pass_ystart[_png_ptr->pass]) /
                _png_pass_yinc[_png_ptr->pass];

            if (_png_ptr->transformations & PNG_INTERLACE)
               break;

         } while (_png_ptr->usr_width == 0 || _png_ptr->num_rows == 0);

      }

      /* Reset the row above the image for the next pass */
      if (_png_ptr->pass < 7)
      {
         if (_png_ptr->prev_row != NULL)
            _png_memset(_png_ptr->prev_row, 0,
                (_png_size_t)(PNG_ROWBYTES(_png_ptr->usr_channels*
                _png_ptr->usr_bit_depth, _png_ptr->width)) + 1);

         return;
      }
   }
#endif

   /* If we get here, we've just written the last row, so we need
      to flush the compressor */
   do
   {
      /* Tell the compressor we are done */
      ret = deflate(&_png_ptr->zstream, Z_FINISH);

      /* Check for an error */
      if (ret == Z_OK)
      {
         /* Check to see if we need more room */
         if (!(_png_ptr->zstream.avail_out))
         {
            _png_write_IDAT(_png_ptr, _png_ptr->zbuf, _png_ptr->zbuf_size);
            _png_ptr->zstream.next_out = _png_ptr->zbuf;
            _png_ptr->zstream.avail_out = (uInt)_png_ptr->zbuf_size;
         }
      }

      else if (ret != Z_STREAM_END)
      {
         if (_png_ptr->zstream.msg != NULL)
            _png_error(_png_ptr, _png_ptr->zstream.msg);

         else
            _png_error(_png_ptr, "zlib error");
      }
   } while (ret != Z_STREAM_END);

   /* Write any extra space */
   if (_png_ptr->zstream.avail_out < _png_ptr->zbuf_size)
   {
      _png_write_IDAT(_png_ptr, _png_ptr->zbuf, _png_ptr->zbuf_size -
          _png_ptr->zstream.avail_out);
   }

   _png_zlib_release(_png_ptr);
   _png_ptr->zstream.data_type = Z_BINARY;
}

#ifdef PNG_WRITE_INTERLACING_SUPPORTED
/* Pick out the correct pixels for the interlace pass.
 * The basic idea here is to go through the row with a source
 * pointer and a destination pointer (sp and dp), and copy the
 * correct pixels for the pass.  As the row gets compacted,
 * sp will always be >= dp, so we should never overwrite anything.
 * See the default: case for the easiest code to understand.
 */
void /* PRIVATE */
_png_do_write_interlace(_png_row_infop row_info, _png_bytep row, int pass)
{
   /* Arrays to facilitate easy interlacing - use pass (0 - 6) as index */

   /* Start of interlace block */
   static PNG_CONST _png_byte _png_pass_start[7] = {0, 4, 0, 2, 0, 1, 0};

   /* Offset to next interlace block */
   static PNG_CONST _png_byte  _png_pass_inc[7] = {8, 8, 4, 4, 2, 2, 1};

   _png_debug(1, "in _png_do_write_interlace");

   /* We don't have to do anything on the last pass (6) */
   if (pass < 6)
   {
      /* Each pixel depth is handled separately */
      switch (row_info->pixel_depth)
      {
         case 1:
         {
            _png_bytep sp;
            _png_bytep dp;
            int shift;
            int d;
            int value;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            dp = row;
            d = 0;
            shift = 7;

            for (i = _png_pass_start[pass]; i < row_width;
               i += _png_pass_inc[pass])
            {
               sp = row + (_png_size_t)(i >> 3);
               value = (int)(*sp >> (7 - (int)(i & 0x07))) & 0x01;
               d |= (value << shift);

               if (shift == 0)
               {
                  shift = 7;
                  *dp++ = (_png_byte)d;
                  d = 0;
               }

               else
                  shift--;

            }
            if (shift != 7)
               *dp = (_png_byte)d;

            break;
         }

         case 2:
         {
            _png_bytep sp;
            _png_bytep dp;
            int shift;
            int d;
            int value;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            dp = row;
            shift = 6;
            d = 0;

            for (i = _png_pass_start[pass]; i < row_width;
               i += _png_pass_inc[pass])
            {
               sp = row + (_png_size_t)(i >> 2);
               value = (*sp >> ((3 - (int)(i & 0x03)) << 1)) & 0x03;
               d |= (value << shift);

               if (shift == 0)
               {
                  shift = 6;
                  *dp++ = (_png_byte)d;
                  d = 0;
               }

               else
                  shift -= 2;
            }
            if (shift != 6)
               *dp = (_png_byte)d;

            break;
         }

         case 4:
         {
            _png_bytep sp;
            _png_bytep dp;
            int shift;
            int d;
            int value;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            dp = row;
            shift = 4;
            d = 0;
            for (i = _png_pass_start[pass]; i < row_width;
                i += _png_pass_inc[pass])
            {
               sp = row + (_png_size_t)(i >> 1);
               value = (*sp >> ((1 - (int)(i & 0x01)) << 2)) & 0x0f;
               d |= (value << shift);

               if (shift == 0)
               {
                  shift = 4;
                  *dp++ = (_png_byte)d;
                  d = 0;
               }

               else
                  shift -= 4;
            }
            if (shift != 4)
               *dp = (_png_byte)d;

            break;
         }

         default:
         {
            _png_bytep sp;
            _png_bytep dp;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;
            _png_size_t pixel_bytes;

            /* Start at the beginning */
            dp = row;

            /* Find out how many bytes each pixel takes up */
            pixel_bytes = (row_info->pixel_depth >> 3);

            /* Loop through the row, only looking at the pixels that matter */
            for (i = _png_pass_start[pass]; i < row_width;
               i += _png_pass_inc[pass])
            {
               /* Find out where the original pixel is */
               sp = row + (_png_size_t)i * pixel_bytes;

               /* Move the pixel */
               if (dp != sp)
                  _png_memcpy(dp, sp, pixel_bytes);

               /* Next pixel */
               dp += pixel_bytes;
            }
            break;
         }
      }
      /* Set new row width */
      row_info->width = (row_info->width +
          _png_pass_inc[pass] - 1 -
          _png_pass_start[pass]) /
          _png_pass_inc[pass];

      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth,
          row_info->width);
   }
}
#endif

/* This filters the row, chooses which filter to use, if it has not already
 * been specified by the application, and then writes the row out with the
 * chosen filter.
 */
static void _png_write_filtered_row(_png_structp _png_ptr, _png_bytep filtered_row,
   _png_size_t row_bytes);

#define PNG_MAXSUM (((_png_uint_32)(-1)) >> 1)
#define PNG_HISHIFT 10
#define PNG_LOMASK ((_png_uint_32)0xffffL)
#define PNG_HIMASK ((_png_uint_32)(~PNG_LOMASK >> PNG_HISHIFT))
void /* PRIVATE */
_png_write_find_filter(_png_structp _png_ptr, _png_row_infop row_info)
{
   _png_bytep best_row;
#ifdef PNG_WRITE_FILTER_SUPPORTED
   _png_bytep prev_row, row_buf;
   _png_uint_32 mins, bpp;
   _png_byte filter_to_do = _png_ptr->do_filter;
   _png_size_t row_bytes = row_info->rowbytes;
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
   int num_p_filters = _png_ptr->num_prev_filters;
#endif

   _png_debug(1, "in _png_write_find_filter");

#ifndef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
  if (_png_ptr->row_number == 0 && filter_to_do == PNG_ALL_FILTERS)
  {
     /* These will never be selected so we need not test them. */
     filter_to_do &= ~(PNG_FILTER_UP | PNG_FILTER_PAETH);
  }
#endif

   /* Find out how many bytes offset each pixel is */
   bpp = (row_info->pixel_depth + 7) >> 3;

   prev_row = _png_ptr->prev_row;
#endif
   best_row = _png_ptr->row_buf;
#ifdef PNG_WRITE_FILTER_SUPPORTED
   row_buf = best_row;
   mins = PNG_MAXSUM;

   /* The prediction method we use is to find which method provides the
    * smallest value when summing the absolute values of the distances
    * from zero, using anything >= 128 as negative numbers.  This is known
    * as the "minimum sum of absolute differences" heuristic.  Other
    * heuristics are the "weighted minimum sum of absolute differences"
    * (experimental and can in theory improve compression), and the "zlib
    * predictive" method (not implemented yet), which does test compressions
    * of lines using different filter methods, and then chooses the
    * (series of) filter(s) that give minimum compressed data size (VERY
    * computationally expensive).
    *
    * GRR 980525:  consider also
    *
    *   (1) minimum sum of absolute differences from running average (i.e.,
    *       keep running sum of non-absolute differences & count of bytes)
    *       [track dispersion, too?  restart average if dispersion too large?]
    *
    *  (1b) minimum sum of absolute differences from sliding average, probably
    *       with window size <= deflate window (usually 32K)
    *
    *   (2) minimum sum of squared differences from zero or running average
    *       (i.e., ~ root-mean-square approach)
    */


   /* We don't need to test the 'no filter' case if this is the only filter
    * that has been chosen, as it doesn't actually do anything to the data.
    */
   if ((filter_to_do & PNG_FILTER_NONE) && filter_to_do != PNG_FILTER_NONE)
   {
      _png_bytep rp;
      _png_uint_32 sum = 0;
      _png_size_t i;
      int v;

      for (i = 0, rp = row_buf + 1; i < row_bytes; i++, rp++)
      {
         v = *rp;
         sum += (v < 128) ? v : 256 - v;
      }

#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (_png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         _png_uint_32 sumhi, sumlo;
         int j;
         sumlo = sum & PNG_LOMASK;
         sumhi = (sum >> PNG_HISHIFT) & PNG_HIMASK; /* Gives us some footroom */

         /* Reduce the sum if we match any of the previous rows */
         for (j = 0; j < num_p_filters; j++)
         {
            if (_png_ptr->prev_filters[j] == PNG_FILTER_VALUE_NONE)
            {
               sumlo = (sumlo * _png_ptr->filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;

               sumhi = (sumhi * _png_ptr->filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;
            }
         }

         /* Factor in the cost of this filter (this is here for completeness,
          * but it makes no sense to have a "cost" for the NONE filter, as
          * it has the minimum possible computational cost - none).
          */
         sumlo = (sumlo * _png_ptr->filter_costs[PNG_FILTER_VALUE_NONE]) >>
             PNG_COST_SHIFT;

         sumhi = (sumhi * _png_ptr->filter_costs[PNG_FILTER_VALUE_NONE]) >>
             PNG_COST_SHIFT;

         if (sumhi > PNG_HIMASK)
            sum = PNG_MAXSUM;

         else
            sum = (sumhi << PNG_HISHIFT) + sumlo;
      }
#endif
      mins = sum;
   }

   /* Sub filter */
   if (filter_to_do == PNG_FILTER_SUB)
   /* It's the only filter so no testing is needed */
   {
      _png_bytep rp, lp, dp;
      _png_size_t i;

      for (i = 0, rp = row_buf + 1, dp = _png_ptr->sub_row + 1; i < bpp;
           i++, rp++, dp++)
      {
         *dp = *rp;
      }

      for (lp = row_buf + 1; i < row_bytes;
         i++, rp++, lp++, dp++)
      {
         *dp = (_png_byte)(((int)*rp - (int)*lp) & 0xff);
      }

      best_row = _png_ptr->sub_row;
   }

   else if (filter_to_do & PNG_FILTER_SUB)
   {
      _png_bytep rp, dp, lp;
      _png_uint_32 sum = 0, lmins = mins;
      _png_size_t i;
      int v;

#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      /* We temporarily increase the "minimum sum" by the factor we
       * would reduce the sum of this filter, so that we can do the
       * early exit comparison without scaling the sum each time.
       */
      if (_png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         _png_uint_32 lmhi, lmlo;
         lmlo = lmins & PNG_LOMASK;
         lmhi = (lmins >> PNG_HISHIFT) & PNG_HIMASK;

         for (j = 0; j < num_p_filters; j++)
         {
            if (_png_ptr->prev_filters[j] == PNG_FILTER_VALUE_SUB)
            {
               lmlo = (lmlo * _png_ptr->inv_filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;

               lmhi = (lmhi * _png_ptr->inv_filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;
            }
         }

         lmlo = (lmlo * _png_ptr->inv_filter_costs[PNG_FILTER_VALUE_SUB]) >>
             PNG_COST_SHIFT;

         lmhi = (lmhi * _png_ptr->inv_filter_costs[PNG_FILTER_VALUE_SUB]) >>
             PNG_COST_SHIFT;

         if (lmhi > PNG_HIMASK)
            lmins = PNG_MAXSUM;

         else
            lmins = (lmhi << PNG_HISHIFT) + lmlo;
      }
#endif

      for (i = 0, rp = row_buf + 1, dp = _png_ptr->sub_row + 1; i < bpp;
           i++, rp++, dp++)
      {
         v = *dp = *rp;

         sum += (v < 128) ? v : 256 - v;
      }

      for (lp = row_buf + 1; i < row_bytes;
         i++, rp++, lp++, dp++)
      {
         v = *dp = (_png_byte)(((int)*rp - (int)*lp) & 0xff);

         sum += (v < 128) ? v : 256 - v;

         if (sum > lmins)  /* We are already worse, don't continue. */
            break;
      }

#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (_png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         _png_uint_32 sumhi, sumlo;
         sumlo = sum & PNG_LOMASK;
         sumhi = (sum >> PNG_HISHIFT) & PNG_HIMASK;

         for (j = 0; j < num_p_filters; j++)
         {
            if (_png_ptr->prev_filters[j] == PNG_FILTER_VALUE_SUB)
            {
               sumlo = (sumlo * _png_ptr->inv_filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;

               sumhi = (sumhi * _png_ptr->inv_filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;
            }
         }

         sumlo = (sumlo * _png_ptr->inv_filter_costs[PNG_FILTER_VALUE_SUB]) >>
             PNG_COST_SHIFT;

         sumhi = (sumhi * _png_ptr->inv_filter_costs[PNG_FILTER_VALUE_SUB]) >>
             PNG_COST_SHIFT;

         if (sumhi > PNG_HIMASK)
            sum = PNG_MAXSUM;

         else
            sum = (sumhi << PNG_HISHIFT) + sumlo;
      }
#endif

      if (sum < mins)
      {
         mins = sum;
         best_row = _png_ptr->sub_row;
      }
   }

   /* Up filter */
   if (filter_to_do == PNG_FILTER_UP)
   {
      _png_bytep rp, dp, pp;
      _png_size_t i;

      for (i = 0, rp = row_buf + 1, dp = _png_ptr->up_row + 1,
          pp = prev_row + 1; i < row_bytes;
          i++, rp++, pp++, dp++)
      {
         *dp = (_png_byte)(((int)*rp - (int)*pp) & 0xff);
      }

      best_row = _png_ptr->up_row;
   }

   else if (filter_to_do & PNG_FILTER_UP)
   {
      _png_bytep rp, dp, pp;
      _png_uint_32 sum = 0, lmins = mins;
      _png_size_t i;
      int v;


#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (_png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         _png_uint_32 lmhi, lmlo;
         lmlo = lmins & PNG_LOMASK;
         lmhi = (lmins >> PNG_HISHIFT) & PNG_HIMASK;

         for (j = 0; j < num_p_filters; j++)
         {
            if (_png_ptr->prev_filters[j] == PNG_FILTER_VALUE_UP)
            {
               lmlo = (lmlo * _png_ptr->inv_filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;

               lmhi = (lmhi * _png_ptr->inv_filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;
            }
         }

         lmlo = (lmlo * _png_ptr->inv_filter_costs[PNG_FILTER_VALUE_UP]) >>
             PNG_COST_SHIFT;

         lmhi = (lmhi * _png_ptr->inv_filter_costs[PNG_FILTER_VALUE_UP]) >>
             PNG_COST_SHIFT;

         if (lmhi > PNG_HIMASK)
            lmins = PNG_MAXSUM;

         else
            lmins = (lmhi << PNG_HISHIFT) + lmlo;
      }
#endif

      for (i = 0, rp = row_buf + 1, dp = _png_ptr->up_row + 1,
          pp = prev_row + 1; i < row_bytes; i++)
      {
         v = *dp++ = (_png_byte)(((int)*rp++ - (int)*pp++) & 0xff);

         sum += (v < 128) ? v : 256 - v;

         if (sum > lmins)  /* We are already worse, don't continue. */
            break;
      }

#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (_png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         _png_uint_32 sumhi, sumlo;
         sumlo = sum & PNG_LOMASK;
         sumhi = (sum >> PNG_HISHIFT) & PNG_HIMASK;

         for (j = 0; j < num_p_filters; j++)
         {
            if (_png_ptr->prev_filters[j] == PNG_FILTER_VALUE_UP)
            {
               sumlo = (sumlo * _png_ptr->filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;

               sumhi = (sumhi * _png_ptr->filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;
            }
         }

         sumlo = (sumlo * _png_ptr->filter_costs[PNG_FILTER_VALUE_UP]) >>
             PNG_COST_SHIFT;

         sumhi = (sumhi * _png_ptr->filter_costs[PNG_FILTER_VALUE_UP]) >>
             PNG_COST_SHIFT;

         if (sumhi > PNG_HIMASK)
            sum = PNG_MAXSUM;

         else
            sum = (sumhi << PNG_HISHIFT) + sumlo;
      }
#endif

      if (sum < mins)
      {
         mins = sum;
         best_row = _png_ptr->up_row;
      }
   }

   /* Avg filter */
   if (filter_to_do == PNG_FILTER_AVG)
   {
      _png_bytep rp, dp, pp, lp;
      _png_uint_32 i;

      for (i = 0, rp = row_buf + 1, dp = _png_ptr->avg_row + 1,
           pp = prev_row + 1; i < bpp; i++)
      {
         *dp++ = (_png_byte)(((int)*rp++ - ((int)*pp++ / 2)) & 0xff);
      }

      for (lp = row_buf + 1; i < row_bytes; i++)
      {
         *dp++ = (_png_byte)(((int)*rp++ - (((int)*pp++ + (int)*lp++) / 2))
                 & 0xff);
      }
      best_row = _png_ptr->avg_row;
   }

   else if (filter_to_do & PNG_FILTER_AVG)
   {
      _png_bytep rp, dp, pp, lp;
      _png_uint_32 sum = 0, lmins = mins;
      _png_size_t i;
      int v;

#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (_png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         _png_uint_32 lmhi, lmlo;
         lmlo = lmins & PNG_LOMASK;
         lmhi = (lmins >> PNG_HISHIFT) & PNG_HIMASK;

         for (j = 0; j < num_p_filters; j++)
         {
            if (_png_ptr->prev_filters[j] == PNG_FILTER_VALUE_AVG)
            {
               lmlo = (lmlo * _png_ptr->inv_filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;

               lmhi = (lmhi * _png_ptr->inv_filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;
            }
         }

         lmlo = (lmlo * _png_ptr->inv_filter_costs[PNG_FILTER_VALUE_AVG]) >>
             PNG_COST_SHIFT;

         lmhi = (lmhi * _png_ptr->inv_filter_costs[PNG_FILTER_VALUE_AVG]) >>
             PNG_COST_SHIFT;

         if (lmhi > PNG_HIMASK)
            lmins = PNG_MAXSUM;

         else
            lmins = (lmhi << PNG_HISHIFT) + lmlo;
      }
#endif

      for (i = 0, rp = row_buf + 1, dp = _png_ptr->avg_row + 1,
           pp = prev_row + 1; i < bpp; i++)
      {
         v = *dp++ = (_png_byte)(((int)*rp++ - ((int)*pp++ / 2)) & 0xff);

         sum += (v < 128) ? v : 256 - v;
      }

      for (lp = row_buf + 1; i < row_bytes; i++)
      {
         v = *dp++ =
             (_png_byte)(((int)*rp++ - (((int)*pp++ + (int)*lp++) / 2)) & 0xff);

         sum += (v < 128) ? v : 256 - v;

         if (sum > lmins)  /* We are already worse, don't continue. */
            break;
      }

#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (_png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         _png_uint_32 sumhi, sumlo;
         sumlo = sum & PNG_LOMASK;
         sumhi = (sum >> PNG_HISHIFT) & PNG_HIMASK;

         for (j = 0; j < num_p_filters; j++)
         {
            if (_png_ptr->prev_filters[j] == PNG_FILTER_VALUE_NONE)
            {
               sumlo = (sumlo * _png_ptr->filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;

               sumhi = (sumhi * _png_ptr->filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;
            }
         }

         sumlo = (sumlo * _png_ptr->filter_costs[PNG_FILTER_VALUE_AVG]) >>
             PNG_COST_SHIFT;

         sumhi = (sumhi * _png_ptr->filter_costs[PNG_FILTER_VALUE_AVG]) >>
             PNG_COST_SHIFT;

         if (sumhi > PNG_HIMASK)
            sum = PNG_MAXSUM;

         else
            sum = (sumhi << PNG_HISHIFT) + sumlo;
      }
#endif

      if (sum < mins)
      {
         mins = sum;
         best_row = _png_ptr->avg_row;
      }
   }

   /* Paeth filter */
   if (filter_to_do == PNG_FILTER_PAETH)
   {
      _png_bytep rp, dp, pp, cp, lp;
      _png_size_t i;

      for (i = 0, rp = row_buf + 1, dp = _png_ptr->paeth_row + 1,
          pp = prev_row + 1; i < bpp; i++)
      {
         *dp++ = (_png_byte)(((int)*rp++ - (int)*pp++) & 0xff);
      }

      for (lp = row_buf + 1, cp = prev_row + 1; i < row_bytes; i++)
      {
         int a, b, c, pa, pb, pc, p;

         b = *pp++;
         c = *cp++;
         a = *lp++;

         p = b - c;
         pc = a - c;

#ifdef PNG_USE_ABS
         pa = abs(p);
         pb = abs(pc);
         pc = abs(p + pc);
#else
         pa = p < 0 ? -p : p;
         pb = pc < 0 ? -pc : pc;
         pc = (p + pc) < 0 ? -(p + pc) : p + pc;
#endif

         p = (pa <= pb && pa <=pc) ? a : (pb <= pc) ? b : c;

         *dp++ = (_png_byte)(((int)*rp++ - p) & 0xff);
      }
      best_row = _png_ptr->paeth_row;
   }

   else if (filter_to_do & PNG_FILTER_PAETH)
   {
      _png_bytep rp, dp, pp, cp, lp;
      _png_uint_32 sum = 0, lmins = mins;
      _png_size_t i;
      int v;

#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (_png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         _png_uint_32 lmhi, lmlo;
         lmlo = lmins & PNG_LOMASK;
         lmhi = (lmins >> PNG_HISHIFT) & PNG_HIMASK;

         for (j = 0; j < num_p_filters; j++)
         {
            if (_png_ptr->prev_filters[j] == PNG_FILTER_VALUE_PAETH)
            {
               lmlo = (lmlo * _png_ptr->inv_filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;

               lmhi = (lmhi * _png_ptr->inv_filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;
            }
         }

         lmlo = (lmlo * _png_ptr->inv_filter_costs[PNG_FILTER_VALUE_PAETH]) >>
             PNG_COST_SHIFT;

         lmhi = (lmhi * _png_ptr->inv_filter_costs[PNG_FILTER_VALUE_PAETH]) >>
             PNG_COST_SHIFT;

         if (lmhi > PNG_HIMASK)
            lmins = PNG_MAXSUM;

         else
            lmins = (lmhi << PNG_HISHIFT) + lmlo;
      }
#endif

      for (i = 0, rp = row_buf + 1, dp = _png_ptr->paeth_row + 1,
          pp = prev_row + 1; i < bpp; i++)
      {
         v = *dp++ = (_png_byte)(((int)*rp++ - (int)*pp++) & 0xff);

         sum += (v < 128) ? v : 256 - v;
      }

      for (lp = row_buf + 1, cp = prev_row + 1; i < row_bytes; i++)
      {
         int a, b, c, pa, pb, pc, p;

         b = *pp++;
         c = *cp++;
         a = *lp++;

#ifndef PNG_SLOW_PAETH
         p = b - c;
         pc = a - c;
#ifdef PNG_USE_ABS
         pa = abs(p);
         pb = abs(pc);
         pc = abs(p + pc);
#else
         pa = p < 0 ? -p : p;
         pb = pc < 0 ? -pc : pc;
         pc = (p + pc) < 0 ? -(p + pc) : p + pc;
#endif
         p = (pa <= pb && pa <=pc) ? a : (pb <= pc) ? b : c;
#else /* PNG_SLOW_PAETH */
         p = a + b - c;
         pa = abs(p - a);
         pb = abs(p - b);
         pc = abs(p - c);

         if (pa <= pb && pa <= pc)
            p = a;

         else if (pb <= pc)
            p = b;

         else
            p = c;
#endif /* PNG_SLOW_PAETH */

         v = *dp++ = (_png_byte)(((int)*rp++ - p) & 0xff);

         sum += (v < 128) ? v : 256 - v;

         if (sum > lmins)  /* We are already worse, don't continue. */
            break;
      }

#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (_png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         _png_uint_32 sumhi, sumlo;
         sumlo = sum & PNG_LOMASK;
         sumhi = (sum >> PNG_HISHIFT) & PNG_HIMASK;

         for (j = 0; j < num_p_filters; j++)
         {
            if (_png_ptr->prev_filters[j] == PNG_FILTER_VALUE_PAETH)
            {
               sumlo = (sumlo * _png_ptr->filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;

               sumhi = (sumhi * _png_ptr->filter_weights[j]) >>
                   PNG_WEIGHT_SHIFT;
            }
         }

         sumlo = (sumlo * _png_ptr->filter_costs[PNG_FILTER_VALUE_PAETH]) >>
             PNG_COST_SHIFT;

         sumhi = (sumhi * _png_ptr->filter_costs[PNG_FILTER_VALUE_PAETH]) >>
             PNG_COST_SHIFT;

         if (sumhi > PNG_HIMASK)
            sum = PNG_MAXSUM;

         else
            sum = (sumhi << PNG_HISHIFT) + sumlo;
      }
#endif

      if (sum < mins)
      {
         best_row = _png_ptr->paeth_row;
      }
   }
#endif /* PNG_WRITE_FILTER_SUPPORTED */

   /* Do the actual writing of the filtered row data from the chosen filter. */
   _png_write_filtered_row(_png_ptr, best_row, row_info->rowbytes+1);

#ifdef PNG_WRITE_FILTER_SUPPORTED
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
   /* Save the type of filter we picked this time for future calculations */
   if (_png_ptr->num_prev_filters > 0)
   {
      int j;

      for (j = 1; j < num_p_filters; j++)
      {
         _png_ptr->prev_filters[j] = _png_ptr->prev_filters[j - 1];
      }

      _png_ptr->prev_filters[j] = best_row[0];
   }
#endif
#endif /* PNG_WRITE_FILTER_SUPPORTED */
}


/* Do the actual writing of a previously filtered row. */
static void
_png_write_filtered_row(_png_structp _png_ptr, _png_bytep filtered_row,
   _png_size_t avail/*includes filter byte*/)
{
   _png_debug(1, "in _png_write_filtered_row");

   _png_debug1(2, "filter = %d", filtered_row[0]);
   /* Set up the zlib input buffer */

   _png_ptr->zstream.next_in = filtered_row;
   _png_ptr->zstream.avail_in = 0;
   /* Repeat until we have compressed all the data */
   do
   {
      int ret; /* Return of zlib */

      /* Record the number of bytes available - zlib supports at least 65535
       * bytes at one step, depending on the size of the zlib type 'uInt', the
       * maximum size zlib can write at once is ZLIB_IO_MAX (from pngpriv.h).
       * Use this because on 16 bit systems 'rowbytes' can be up to 65536 (i.e.
       * one more than 16 bits) and, in this case 'rowbytes+1' can overflow a
       * uInt.  ZLIB_IO_MAX can be safely reduced to cause zlib to be called
       * with smaller chunks of data.
       */
      if (_png_ptr->zstream.avail_in == 0)
      {
         if (avail > ZLIB_IO_MAX)
         {
            _png_ptr->zstream.avail_in  = ZLIB_IO_MAX;
            avail -= ZLIB_IO_MAX;
         }

         else
         {
            /* So this will fit in the available uInt space: */
            _png_ptr->zstream.avail_in = (uInt)avail;
            avail = 0;
         }
      }

      /* Compress the data */
      ret = deflate(&_png_ptr->zstream, Z_NO_FLUSH);

      /* Check for compression errors */
      if (ret != Z_OK)
      {
         if (_png_ptr->zstream.msg != NULL)
            _png_error(_png_ptr, _png_ptr->zstream.msg);

         else
            _png_error(_png_ptr, "zlib error");
      }

      /* See if it is time to write another IDAT */
      if (!(_png_ptr->zstream.avail_out))
      {
         /* Write the IDAT and reset the zlib output buffer */
         _png_write_IDAT(_png_ptr, _png_ptr->zbuf, _png_ptr->zbuf_size);
      }
   /* Repeat until all data has been compressed */
   } while (avail > 0 || _png_ptr->zstream.avail_in > 0);

   /* Swap the current and previous rows */
   if (_png_ptr->prev_row != NULL)
   {
      _png_bytep tptr;

      tptr = _png_ptr->prev_row;
      _png_ptr->prev_row = _png_ptr->row_buf;
      _png_ptr->row_buf = tptr;
   }

   /* Finish row - updates counters and flushes zlib if last row */
   _png_write_finish_row(_png_ptr);

#ifdef PNG_WRITE_FLUSH_SUPPORTED
   _png_ptr->flush_rows++;

   if (_png_ptr->flush_dist > 0 &&
       _png_ptr->flush_rows >= _png_ptr->flush_dist)
   {
      _png_write_flush(_png_ptr);
   }
#endif
}
#endif /* PNG_WRITE_SUPPORTED */
