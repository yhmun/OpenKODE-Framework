
/* pngwtran.c - transforms the data in a row for PNG writers
 *
 * Last changed in libpng 1.5.13 [September 27, 2012]
 * Copyright (c) 1998-2012 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 */

#include "pngpriv.h"

#ifdef PNG_WRITE_SUPPORTED

#ifdef PNG_WRITE_TRANSFORMS_SUPPORTED
/* Transform the data according to the user's wishes.  The order of
 * transformations is significant.
 */
void /* PRIVATE */
_png_do_write_transformations(_png_structp _png_ptr, _png_row_infop row_info)
{
   _png_debug(1, "in _png_do_write_transformations");

   if (_png_ptr == NULL)
      return;

#ifdef PNG_WRITE_USER_TRANSFORM_SUPPORTED
   if (_png_ptr->transformations & PNG_USER_TRANSFORM)
      if (_png_ptr->write_user_transform_fn != NULL)
         (*(_png_ptr->write_user_transform_fn)) /* User write transform
                                                 function */
             (_png_ptr,  /* _png_ptr */
             row_info,  /* row_info: */
                /*  _png_uint_32 width;       width of row */
                /*  _png_size_t rowbytes;     number of bytes in row */
                /*  _png_byte color_type;     color type of pixels */
                /*  _png_byte bit_depth;      bit depth of samples */
                /*  _png_byte channels;       number of channels (1-4) */
                /*  _png_byte pixel_depth;    bits per pixel (depth*channels) */
             _png_ptr->row_buf + 1);      /* start of pixel data for row */
#endif

#ifdef PNG_WRITE_FILLER_SUPPORTED
   if (_png_ptr->transformations & PNG_FILLER)
   {
      if (_png_ptr->color_type & (PNG_COLOR_MASK_ALPHA|PNG_COLOR_MASK_PALETTE))
      {
         /* GA, RGBA or palette; in any of these cases libpng will not do the
          * the correct thing (whatever that might be).
          */
         _png_warning(_png_ptr, "incorrect _png_set_filler call ignored");
         _png_ptr->transformations &= ~PNG_FILLER;
      }

      else
         _png_do_strip_channel(row_info, _png_ptr->row_buf + 1,
            !(_png_ptr->flags & PNG_FLAG_FILLER_AFTER));
   }
#endif

#ifdef PNG_WRITE_PACKSWAP_SUPPORTED
   if (_png_ptr->transformations & PNG_PACKSWAP)
      _png_do_packswap(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_WRITE_PACK_SUPPORTED
   if (_png_ptr->transformations & PNG_PACK)
      _png_do_pack(row_info, _png_ptr->row_buf + 1,
          (_png_uint_32)_png_ptr->bit_depth);
#endif

#ifdef PNG_WRITE_SWAP_SUPPORTED
   if (_png_ptr->transformations & PNG_SWAP_BYTES)
      _png_do_swap(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_WRITE_SHIFT_SUPPORTED
   if (_png_ptr->transformations & PNG_SHIFT)
      _png_do_shift(row_info, _png_ptr->row_buf + 1,
          &(_png_ptr->shift));
#endif

#ifdef PNG_WRITE_SWAP_ALPHA_SUPPORTED
   if (_png_ptr->transformations & PNG_SWAP_ALPHA)
      _png_do_write_swap_alpha(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_WRITE_INVERT_ALPHA_SUPPORTED
   if (_png_ptr->transformations & PNG_INVERT_ALPHA)
      _png_do_write_invert_alpha(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_WRITE_BGR_SUPPORTED
   if (_png_ptr->transformations & PNG_BGR)
      _png_do_bgr(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_WRITE_INVERT_SUPPORTED
   if (_png_ptr->transformations & PNG_INVERT_MONO)
      _png_do_invert(row_info, _png_ptr->row_buf + 1);
#endif
}

#ifdef PNG_WRITE_PACK_SUPPORTED
/* Pack pixels into bytes.  Pass the true bit depth in bit_depth.  The
 * row_info bit depth should be 8 (one pixel per byte).  The channels
 * should be 1 (this only happens on grayscale and paletted images).
 */
void /* PRIVATE */
_png_do_pack(_png_row_infop row_info, _png_bytep row, _png_uint_32 bit_depth)
{
   _png_debug(1, "in _png_do_pack");

   if (row_info->bit_depth == 8 &&
      row_info->channels == 1)
   {
      switch ((int)bit_depth)
      {
         case 1:
         {
            _png_bytep sp, dp;
            int mask, v;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            sp = row;
            dp = row;
            mask = 0x80;
            v = 0;

            for (i = 0; i < row_width; i++)
            {
               if (*sp != 0)
                  v |= mask;

               sp++;

               if (mask > 1)
                  mask >>= 1;

               else
               {
                  mask = 0x80;
                  *dp = (_png_byte)v;
                  dp++;
                  v = 0;
               }
            }

            if (mask != 0x80)
               *dp = (_png_byte)v;

            break;
         }

         case 2:
         {
            _png_bytep sp, dp;
            int shift, v;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            sp = row;
            dp = row;
            shift = 6;
            v = 0;

            for (i = 0; i < row_width; i++)
            {
               _png_byte value;

               value = (_png_byte)(*sp & 0x03);
               v |= (value << shift);

               if (shift == 0)
               {
                  shift = 6;
                  *dp = (_png_byte)v;
                  dp++;
                  v = 0;
               }

               else
                  shift -= 2;

               sp++;
            }

            if (shift != 6)
               *dp = (_png_byte)v;

            break;
         }

         case 4:
         {
            _png_bytep sp, dp;
            int shift, v;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            sp = row;
            dp = row;
            shift = 4;
            v = 0;

            for (i = 0; i < row_width; i++)
            {
               _png_byte value;

               value = (_png_byte)(*sp & 0x0f);
               v |= (value << shift);

               if (shift == 0)
               {
                  shift = 4;
                  *dp = (_png_byte)v;
                  dp++;
                  v = 0;
               }

               else
                  shift -= 4;

               sp++;
            }

            if (shift != 4)
               *dp = (_png_byte)v;

            break;
         }

         default:
            break;
      }

      row_info->bit_depth = (_png_byte)bit_depth;
      row_info->pixel_depth = (_png_byte)(bit_depth * row_info->channels);
      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth,
          row_info->width);
   }
}
#endif

#ifdef PNG_WRITE_SHIFT_SUPPORTED
/* Shift pixel values to take advantage of whole range.  Pass the
 * true number of bits in bit_depth.  The row should be packed
 * according to row_info->bit_depth.  Thus, if you had a row of
 * bit depth 4, but the pixels only had values from 0 to 7, you
 * would pass 3 as bit_depth, and this routine would translate the
 * data to 0 to 15.
 */
void /* PRIVATE */
_png_do_shift(_png_row_infop row_info, _png_bytep row,
    _png_const_color_8p bit_depth)
{
   _png_debug(1, "in _png_do_shift");

   if (row_info->color_type != PNG_COLOR_TYPE_PALETTE)
   {
      int shift_start[4], shift_dec[4];
      int channels = 0;

      if (row_info->color_type & PNG_COLOR_MASK_COLOR)
      {
         shift_start[channels] = row_info->bit_depth - bit_depth->red;
         shift_dec[channels] = bit_depth->red;
         channels++;

         shift_start[channels] = row_info->bit_depth - bit_depth->green;
         shift_dec[channels] = bit_depth->green;
         channels++;

         shift_start[channels] = row_info->bit_depth - bit_depth->blue;
         shift_dec[channels] = bit_depth->blue;
         channels++;
      }

      else
      {
         shift_start[channels] = row_info->bit_depth - bit_depth->gray;
         shift_dec[channels] = bit_depth->gray;
         channels++;
      }

      if (row_info->color_type & PNG_COLOR_MASK_ALPHA)
      {
         shift_start[channels] = row_info->bit_depth - bit_depth->alpha;
         shift_dec[channels] = bit_depth->alpha;
         channels++;
      }

      /* With low row depths, could only be grayscale, so one channel */
      if (row_info->bit_depth < 8)
      {
         _png_bytep bp = row;
         _png_size_t i;
         _png_byte mask;
         _png_size_t row_bytes = row_info->rowbytes;

         if (bit_depth->gray == 1 && row_info->bit_depth == 2)
            mask = 0x55;

         else if (row_info->bit_depth == 4 && bit_depth->gray == 3)
            mask = 0x11;

         else
            mask = 0xff;

         for (i = 0; i < row_bytes; i++, bp++)
         {
            _png_uint_16 v;
            int j;

            v = *bp;
            *bp = 0;

            for (j = shift_start[0]; j > -shift_dec[0]; j -= shift_dec[0])
            {
               if (j > 0)
                  *bp |= (_png_byte)((v << j) & 0xff);

               else
                  *bp |= (_png_byte)((v >> (-j)) & mask);
            }
         }
      }

      else if (row_info->bit_depth == 8)
      {
         _png_bytep bp = row;
         _png_uint_32 i;
         _png_uint_32 istop = channels * row_info->width;

         for (i = 0; i < istop; i++, bp++)
         {

            _png_uint_16 v;
            int j;
            int c = (int)(i%channels);

            v = *bp;
            *bp = 0;

            for (j = shift_start[c]; j > -shift_dec[c]; j -= shift_dec[c])
            {
               if (j > 0)
                  *bp |= (_png_byte)((v << j) & 0xff);

               else
                  *bp |= (_png_byte)((v >> (-j)) & 0xff);
            }
         }
      }

      else
      {
         _png_bytep bp;
         _png_uint_32 i;
         _png_uint_32 istop = channels * row_info->width;

         for (bp = row, i = 0; i < istop; i++)
         {
            int c = (int)(i%channels);
            _png_uint_16 value, v;
            int j;

            v = (_png_uint_16)(((_png_uint_16)(*bp) << 8) + *(bp + 1));
            value = 0;

            for (j = shift_start[c]; j > -shift_dec[c]; j -= shift_dec[c])
            {
               if (j > 0)
                  value |= (_png_uint_16)((v << j) & (_png_uint_16)0xffff);

               else
                  value |= (_png_uint_16)((v >> (-j)) & (_png_uint_16)0xffff);
            }
            *bp++ = (_png_byte)(value >> 8);
            *bp++ = (_png_byte)(value & 0xff);
         }
      }
   }
}
#endif

#ifdef PNG_WRITE_SWAP_ALPHA_SUPPORTED
void /* PRIVATE */
_png_do_write_swap_alpha(_png_row_infop row_info, _png_bytep row)
{
   _png_debug(1, "in _png_do_write_swap_alpha");

   {
      if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
      {
         if (row_info->bit_depth == 8)
         {
            /* This converts from ARGB to RGBA */
            _png_bytep sp, dp;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               _png_byte save = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = save;
            }
         }

#ifdef PNG_WRITE_16BIT_SUPPORTED
         else
         {
            /* This converts from AARRGGBB to RRGGBBAA */
            _png_bytep sp, dp;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               _png_byte save[2];
               save[0] = *(sp++);
               save[1] = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = save[0];
               *(dp++) = save[1];
            }
         }
#endif /* PNG_WRITE_16BIT_SUPPORTED */
      }

      else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      {
         if (row_info->bit_depth == 8)
         {
            /* This converts from AG to GA */
            _png_bytep sp, dp;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               _png_byte save = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = save;
            }
         }

#ifdef PNG_WRITE_16BIT_SUPPORTED
         else
         {
            /* This converts from AAGG to GGAA */
            _png_bytep sp, dp;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               _png_byte save[2];
               save[0] = *(sp++);
               save[1] = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = save[0];
               *(dp++) = save[1];
            }
         }
#endif /* PNG_WRITE_16BIT_SUPPORTED */
      }
   }
}
#endif

#ifdef PNG_WRITE_INVERT_ALPHA_SUPPORTED
void /* PRIVATE */
_png_do_write_invert_alpha(_png_row_infop row_info, _png_bytep row)
{
   _png_debug(1, "in _png_do_write_invert_alpha");

   {
      if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
      {
         if (row_info->bit_depth == 8)
         {
            /* This inverts the alpha channel in RGBA */
            _png_bytep sp, dp;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               /* Does nothing
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               */
               sp+=3; dp = sp;
               *(dp++) = (_png_byte)(255 - *(sp++));
            }
         }

#ifdef PNG_WRITE_16BIT_SUPPORTED
         else
         {
            /* This inverts the alpha channel in RRGGBBAA */
            _png_bytep sp, dp;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               /* Does nothing
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               */
               sp+=6; dp = sp;
               *(dp++) = (_png_byte)(255 - *(sp++));
               *(dp++) = (_png_byte)(255 - *(sp++));
            }
         }
#endif /* PNG_WRITE_16BIT_SUPPORTED */
      }

      else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      {
         if (row_info->bit_depth == 8)
         {
            /* This inverts the alpha channel in GA */
            _png_bytep sp, dp;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               *(dp++) = *(sp++);
               *(dp++) = (_png_byte)(255 - *(sp++));
            }
         }

#ifdef PNG_WRITE_16BIT_SUPPORTED
         else
         {
            /* This inverts the alpha channel in GGAA */
            _png_bytep sp, dp;
            _png_uint_32 i;
            _png_uint_32 row_width = row_info->width;

            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               /* Does nothing
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               */
               sp+=2; dp = sp;
               *(dp++) = (_png_byte)(255 - *(sp++));
               *(dp++) = (_png_byte)(255 - *(sp++));
            }
         }
#endif /* PNG_WRITE_16BIT_SUPPORTED */
      }
   }
}
#endif
#endif /* PNG_WRITE_TRANSFORMS_SUPPORTED */

#ifdef PNG_MNG_FEATURES_SUPPORTED
/* Undoes intrapixel differencing  */
void /* PRIVATE */
_png_do_write_intrapixel(_png_row_infop row_info, _png_bytep row)
{
   _png_debug(1, "in _png_do_write_intrapixel");

   if ((row_info->color_type & PNG_COLOR_MASK_COLOR))
   {
      int bytes_per_pixel;
      _png_uint_32 row_width = row_info->width;
      if (row_info->bit_depth == 8)
      {
         _png_bytep rp;
         _png_uint_32 i;

         if (row_info->color_type == PNG_COLOR_TYPE_RGB)
            bytes_per_pixel = 3;

         else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
            bytes_per_pixel = 4;

         else
            return;

         for (i = 0, rp = row; i < row_width; i++, rp += bytes_per_pixel)
         {
            *(rp)     = (_png_byte)((*rp       - *(rp + 1)) & 0xff);
            *(rp + 2) = (_png_byte)((*(rp + 2) - *(rp + 1)) & 0xff);
         }
      }

#ifdef PNG_WRITE_16BIT_SUPPORTED
      else if (row_info->bit_depth == 16)
      {
         _png_bytep rp;
         _png_uint_32 i;

         if (row_info->color_type == PNG_COLOR_TYPE_RGB)
            bytes_per_pixel = 6;

         else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
            bytes_per_pixel = 8;

         else
            return;

         for (i = 0, rp = row; i < row_width; i++, rp += bytes_per_pixel)
         {
            _png_uint_32 s0   = (*(rp    ) << 8) | *(rp + 1);
            _png_uint_32 s1   = (*(rp + 2) << 8) | *(rp + 3);
            _png_uint_32 s2   = (*(rp + 4) << 8) | *(rp + 5);
            _png_uint_32 red  = (_png_uint_32)((s0 - s1) & 0xffffL);
            _png_uint_32 blue = (_png_uint_32)((s2 - s1) & 0xffffL);
            *(rp    ) = (_png_byte)((red >> 8) & 0xff);
            *(rp + 1) = (_png_byte)(red & 0xff);
            *(rp + 4) = (_png_byte)((blue >> 8) & 0xff);
            *(rp + 5) = (_png_byte)(blue & 0xff);
         }
      }
#endif /* PNG_WRITE_16BIT_SUPPORTED */
   }
}
#endif /* PNG_MNG_FEATURES_SUPPORTED */
#endif /* PNG_WRITE_SUPPORTED */
