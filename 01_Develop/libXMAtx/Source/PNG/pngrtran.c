
/* pngrtran.c - transforms the data in a row for PNG readers
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
 * This file contains functions optionally called by an application
 * in order to tell libpng how to handle data when reading a PNG.
 * Transformations that are used in both reading and writing are
 * in pngtrans.c.
 */

#include "pngpriv.h"

#ifdef PNG_READ_SUPPORTED

/* Set the action on getting a CRC error for an ancillary or critical chunk. */
void PNGAPI
_png_set_crc_action(_png_structp _png_ptr, int crit_action, int ancil_action)
{
   _png_debug(1, "in _png_set_crc_action");

   if (_png_ptr == NULL)
      return;

   /* Tell libpng how we react to CRC errors in critical chunks */
   switch (crit_action)
   {
      case PNG_CRC_NO_CHANGE:                        /* Leave setting as is */
         break;

      case PNG_CRC_WARN_USE:                               /* Warn/use data */
         _png_ptr->flags &= ~PNG_FLAG_CRC_CRITICAL_MASK;
         _png_ptr->flags |= PNG_FLAG_CRC_CRITICAL_USE;
         break;

      case PNG_CRC_QUIET_USE:                             /* Quiet/use data */
         _png_ptr->flags &= ~PNG_FLAG_CRC_CRITICAL_MASK;
         _png_ptr->flags |= PNG_FLAG_CRC_CRITICAL_USE |
                           PNG_FLAG_CRC_CRITICAL_IGNORE;
         break;

      case PNG_CRC_WARN_DISCARD:    /* Not a valid action for critical data */
         _png_warning(_png_ptr,
            "Can't discard critical data on CRC error");
      case PNG_CRC_ERROR_QUIT:                                /* Error/quit */

      case PNG_CRC_DEFAULT:
      default:
         _png_ptr->flags &= ~PNG_FLAG_CRC_CRITICAL_MASK;
         break;
   }

   /* Tell libpng how we react to CRC errors in ancillary chunks */
   switch (ancil_action)
   {
      case PNG_CRC_NO_CHANGE:                       /* Leave setting as is */
         break;

      case PNG_CRC_WARN_USE:                              /* Warn/use data */
         _png_ptr->flags &= ~PNG_FLAG_CRC_ANCILLARY_MASK;
         _png_ptr->flags |= PNG_FLAG_CRC_ANCILLARY_USE;
         break;

      case PNG_CRC_QUIET_USE:                            /* Quiet/use data */
         _png_ptr->flags &= ~PNG_FLAG_CRC_ANCILLARY_MASK;
         _png_ptr->flags |= PNG_FLAG_CRC_ANCILLARY_USE |
                           PNG_FLAG_CRC_ANCILLARY_NOWARN;
         break;

      case PNG_CRC_ERROR_QUIT:                               /* Error/quit */
         _png_ptr->flags &= ~PNG_FLAG_CRC_ANCILLARY_MASK;
         _png_ptr->flags |= PNG_FLAG_CRC_ANCILLARY_NOWARN;
         break;

      case PNG_CRC_WARN_DISCARD:                      /* Warn/discard data */

      case PNG_CRC_DEFAULT:
      default:
         _png_ptr->flags &= ~PNG_FLAG_CRC_ANCILLARY_MASK;
         break;
   }
}

#ifdef PNG_READ_BACKGROUND_SUPPORTED
/* Handle alpha and tRNS via a background color */
void PNGFAPI
_png_set_background_fixed(_png_structp _png_ptr,
    _png_const_color_16p background_color, int background_gamma_code,
    int need_expand, _png_fixed_point background_gamma)
{
   _png_debug(1, "in _png_set_background_fixed");

   if (_png_ptr == NULL)
      return;

   if (background_gamma_code == PNG_BACKGROUND_GAMMA_UNKNOWN)
   {
      _png_warning(_png_ptr, "Application must supply a known background gamma");
      return;
   }

   _png_ptr->transformations |= PNG_COMPOSE | PNG_STRIP_ALPHA;
   _png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
   _png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;

   _png_memcpy(&(_png_ptr->background), background_color,
      _png_sizeof(_png_color_16));
   _png_ptr->background_gamma = background_gamma;
   _png_ptr->background_gamma_type = (_png_byte)(background_gamma_code);
   if (need_expand)
      _png_ptr->transformations |= PNG_BACKGROUND_EXPAND;
   else
      _png_ptr->transformations &= ~PNG_BACKGROUND_EXPAND;
}

#  ifdef PNG_FLOATING_POINT_SUPPORTED
void PNGAPI
_png_set_background(_png_structp _png_ptr,
    _png_const_color_16p background_color, int background_gamma_code,
    int need_expand, double background_gamma)
{
   _png_set_background_fixed(_png_ptr, background_color, background_gamma_code,
      need_expand, _png_fixed(_png_ptr, background_gamma, "_png_set_background"));
}
#  endif  /* FLOATING_POINT */
#endif /* READ_BACKGROUND */

/* Scale 16-bit depth files to 8-bit depth.  If both of these are set then the
 * one that pngrtran does first (scale) happens.  This is necessary to allow the
 * TRANSFORM and API behavior to be somewhat consistent, and it's simpler.
 */
#ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
void PNGAPI
_png_set_scale_16(_png_structp _png_ptr)
{
   _png_debug(1, "in _png_set_scale_16");

   if (_png_ptr == NULL)
      return;

   _png_ptr->transformations |= PNG_SCALE_16_TO_8;
}
#endif

#ifdef PNG_READ_STRIP_16_TO_8_SUPPORTED
/* Chop 16-bit depth files to 8-bit depth */
void PNGAPI
_png_set_strip_16(_png_structp _png_ptr)
{
   _png_debug(1, "in _png_set_strip_16");

   if (_png_ptr == NULL)
      return;

   _png_ptr->transformations |= PNG_16_TO_8;
}
#endif

#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
void PNGAPI
_png_set_strip_alpha(_png_structp _png_ptr)
{
   _png_debug(1, "in _png_set_strip_alpha");

   if (_png_ptr == NULL)
      return;

   _png_ptr->transformations |= PNG_STRIP_ALPHA;
}
#endif

#if defined(PNG_READ_ALPHA_MODE_SUPPORTED) || defined(PNG_READ_GAMMA_SUPPORTED)
static _png_fixed_point
translate_gamma_flags(_png_structp _png_ptr, _png_fixed_point output_gamma,
   int is_screen)
{
   /* Check for flag values.  The main reason for having the old Mac value as a
    * flag is that it is pretty near impossible to work out what the correct
    * value is from Apple documentation - a working Mac system is needed to
    * discover the value!
    */
   if (output_gamma == PNG_DEFAULT_sRGB ||
      output_gamma == PNG_FP_1 / PNG_DEFAULT_sRGB)
   {
      /* If there is no sRGB support this just sets the gamma to the standard
       * sRGB value.  (This is a side effect of using this function!)
       */
#     ifdef PNG_READ_sRGB_SUPPORTED
         _png_ptr->flags |= PNG_FLAG_ASSUME_sRGB;
#     endif
      if (is_screen)
         output_gamma = PNG_GAMMA_sRGB;
      else
         output_gamma = PNG_GAMMA_sRGB_INVERSE;
   }

   else if (output_gamma == PNG_GAMMA_MAC_18 ||
      output_gamma == PNG_FP_1 / PNG_GAMMA_MAC_18)
   {
      if (is_screen)
         output_gamma = PNG_GAMMA_MAC_OLD;
      else
         output_gamma = PNG_GAMMA_MAC_INVERSE;
   }

   return output_gamma;
}

#  ifdef PNG_FLOATING_POINT_SUPPORTED
static _png_fixed_point
convert_gamma_value(_png_structp _png_ptr, double output_gamma)
{
   /* The following silently ignores cases where fixed point (times 100,000)
    * gamma values are passed to the floating point API.  This is safe and it
    * means the fixed point constants work just fine with the floating point
    * API.  The alternative would just lead to undetected errors and spurious
    * bug reports.  Negative values fail inside the _fixed API unless they
    * correspond to the flag values.
    */
   if (output_gamma > 0 && output_gamma < 128)
      output_gamma *= PNG_FP_1;

   /* This preserves -1 and -2 exactly: */
   output_gamma = floor(output_gamma + .5);

   if (output_gamma > PNG_FP_MAX || output_gamma < PNG_FP_MIN)
      _png_fixed_error(_png_ptr, "gamma value");

   return (_png_fixed_point)output_gamma;
}
#  endif
#endif /* READ_ALPHA_MODE || READ_GAMMA */

#ifdef PNG_READ_ALPHA_MODE_SUPPORTED
void PNGFAPI
_png_set_alpha_mode_fixed(_png_structp _png_ptr, int mode,
   _png_fixed_point output_gamma)
{
   int compose = 0;
   _png_fixed_point file_gamma;

   _png_debug(1, "in _png_set_alpha_mode");

   if (_png_ptr == NULL)
      return;

   output_gamma = translate_gamma_flags(_png_ptr, output_gamma, 1/*screen*/);

   /* Validate the value to ensure it is in a reasonable range. The value
    * is expected to be 1 or greater, but this range test allows for some
    * viewing correction values.  The intent is to weed out users of this API
    * who use the inverse of the gamma value accidentally!  Since some of these
    * values are reasonable this may have to be changed.
    */
   if (output_gamma < 70000 || output_gamma > 300000)
      _png_error(_png_ptr, "output gamma out of expected range");

   /* The default file gamma is the inverse of the output gamma; the output
    * gamma may be changed below so get the file value first:
    */
   file_gamma = _png_reciprocal(output_gamma);

   /* There are really 8 possibilities here, composed of any combination
    * of:
    *
    *    premultiply the color channels
    *    do not encode non-opaque pixels
    *    encode the alpha as well as the color channels
    *
    * The differences disappear if the input/output ('screen') gamma is 1.0,
    * because then the encoding is a no-op and there is only the choice of
    * premultiplying the color channels or not.
    *
    * _png_set_alpha_mode and _png_set_background interact because both use
    * _png_compose to do the work.  Calling both is only useful when
    * _png_set_alpha_mode is used to set the default mode - PNG_ALPHA_PNG - along
    * with a default gamma value.  Otherwise PNG_COMPOSE must not be set.
    */
   switch (mode)
   {
      case PNG_ALPHA_PNG:        /* default: png standard */
         /* No compose, but it may be set by _png_set_background! */
         _png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
         _png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
         break;

      case PNG_ALPHA_ASSOCIATED: /* color channels premultiplied */
         compose = 1;
         _png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
         _png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
         /* The output is linear: */
         output_gamma = PNG_FP_1;
         break;

      case PNG_ALPHA_OPTIMIZED:  /* associated, non-opaque pixels linear */
         compose = 1;
         _png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
         _png_ptr->flags |= PNG_FLAG_OPTIMIZE_ALPHA;
         /* output_gamma records the encoding of opaque pixels! */
         break;

      case PNG_ALPHA_BROKEN:     /* associated, non-linear, alpha encoded */
         compose = 1;
         _png_ptr->transformations |= PNG_ENCODE_ALPHA;
         _png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
         break;

      default:
         _png_error(_png_ptr, "invalid alpha mode");
   }

   /* Only set the default gamma if the file gamma has not been set (this has
    * the side effect that the gamma in a second call to _png_set_alpha_mode will
    * be ignored.)
    */
   if (_png_ptr->gamma == 0)
      _png_ptr->gamma = file_gamma;

   /* But always set the output gamma: */
   _png_ptr->screen_gamma = output_gamma;

   /* Finally, if pre-multiplying, set the background fields to achieve the
    * desired result.
    */
   if (compose)
   {
      /* And obtain alpha pre-multiplication by composing on black: */
      _png_memset(&_png_ptr->background, 0, sizeof _png_ptr->background);
      _png_ptr->background_gamma = _png_ptr->gamma; /* just in case */
      _png_ptr->background_gamma_type = PNG_BACKGROUND_GAMMA_FILE;
      _png_ptr->transformations &= ~PNG_BACKGROUND_EXPAND;

      if (_png_ptr->transformations & PNG_COMPOSE)
         _png_error(_png_ptr,
            "conflicting calls to set alpha mode and background");

      _png_ptr->transformations |= PNG_COMPOSE;
   }

   /* New API, make sure apps call the correct initializers: */
   _png_ptr->flags |= PNG_FLAG_DETECT_UNINITIALIZED;
}

#  ifdef PNG_FLOATING_POINT_SUPPORTED
void PNGAPI
_png_set_alpha_mode(_png_structp _png_ptr, int mode, double output_gamma)
{
   _png_set_alpha_mode_fixed(_png_ptr, mode, convert_gamma_value(_png_ptr,
      output_gamma));
}
#  endif
#endif

#ifdef PNG_READ_QUANTIZE_SUPPORTED
/* Dither file to 8-bit.  Supply a palette, the current number
 * of elements in the palette, the maximum number of elements
 * allowed, and a histogram if possible.  If the current number
 * of colors is greater then the maximum number, the palette will be
 * modified to fit in the maximum number.  "full_quantize" indicates
 * whether we need a quantizing cube set up for RGB images, or if we
 * simply are reducing the number of colors in a paletted image.
 */

typedef struct _png_dsort_struct
{
   struct _png_dsort_struct FAR * next;
   _png_byte left;
   _png_byte right;
} _png_dsort;
typedef _png_dsort FAR *       _png_dsortp;
typedef _png_dsort FAR * FAR * _png_dsortpp;

void PNGAPI
_png_set_quantize(_png_structp _png_ptr, _png_colorp palette,
    int num_palette, int maximum_colors, _png_const_uint_16p histogram,
    int full_quantize)
{
   _png_debug(1, "in _png_set_quantize");

   if (_png_ptr == NULL)
      return;

   _png_ptr->transformations |= PNG_QUANTIZE;

   if (!full_quantize)
   {
      int i;

      _png_ptr->quantize_index = (_png_bytep)_png_malloc(_png_ptr,
          (_png_uint_32)(num_palette * _png_sizeof(_png_byte)));
      for (i = 0; i < num_palette; i++)
         _png_ptr->quantize_index[i] = (_png_byte)i;
   }

   if (num_palette > maximum_colors)
   {
      if (histogram != NULL)
      {
         /* This is easy enough, just throw out the least used colors.
          * Perhaps not the best solution, but good enough.
          */

         int i;

         /* Initialize an array to sort colors */
         _png_ptr->quantize_sort = (_png_bytep)_png_malloc(_png_ptr,
             (_png_uint_32)(num_palette * _png_sizeof(_png_byte)));

         /* Initialize the quantize_sort array */
         for (i = 0; i < num_palette; i++)
            _png_ptr->quantize_sort[i] = (_png_byte)i;

         /* Find the least used palette entries by starting a
          * bubble sort, and running it until we have sorted
          * out enough colors.  Note that we don't care about
          * sorting all the colors, just finding which are
          * least used.
          */

         for (i = num_palette - 1; i >= maximum_colors; i--)
         {
            int done; /* To stop early if the list is pre-sorted */
            int j;

            done = 1;
            for (j = 0; j < i; j++)
            {
               if (histogram[_png_ptr->quantize_sort[j]]
                   < histogram[_png_ptr->quantize_sort[j + 1]])
               {
                  _png_byte t;

                  t = _png_ptr->quantize_sort[j];
                  _png_ptr->quantize_sort[j] = _png_ptr->quantize_sort[j + 1];
                  _png_ptr->quantize_sort[j + 1] = t;
                  done = 0;
               }
            }

            if (done)
               break;
         }

         /* Swap the palette around, and set up a table, if necessary */
         if (full_quantize)
         {
            int j = num_palette;

            /* Put all the useful colors within the max, but don't
             * move the others.
             */
            for (i = 0; i < maximum_colors; i++)
            {
               if ((int)_png_ptr->quantize_sort[i] >= maximum_colors)
               {
                  do
                     j--;
                  while ((int)_png_ptr->quantize_sort[j] >= maximum_colors);

                  palette[i] = palette[j];
               }
            }
         }
         else
         {
            int j = num_palette;

            /* Move all the used colors inside the max limit, and
             * develop a translation table.
             */
            for (i = 0; i < maximum_colors; i++)
            {
               /* Only move the colors we need to */
               if ((int)_png_ptr->quantize_sort[i] >= maximum_colors)
               {
                  _png_color tmp_color;

                  do
                     j--;
                  while ((int)_png_ptr->quantize_sort[j] >= maximum_colors);

                  tmp_color = palette[j];
                  palette[j] = palette[i];
                  palette[i] = tmp_color;
                  /* Indicate where the color went */
                  _png_ptr->quantize_index[j] = (_png_byte)i;
                  _png_ptr->quantize_index[i] = (_png_byte)j;
               }
            }

            /* Find closest color for those colors we are not using */
            for (i = 0; i < num_palette; i++)
            {
               if ((int)_png_ptr->quantize_index[i] >= maximum_colors)
               {
                  int min_d, k, min_k, d_index;

                  /* Find the closest color to one we threw out */
                  d_index = _png_ptr->quantize_index[i];
                  min_d = PNG_COLOR_DIST(palette[d_index], palette[0]);
                  for (k = 1, min_k = 0; k < maximum_colors; k++)
                  {
                     int d;

                     d = PNG_COLOR_DIST(palette[d_index], palette[k]);

                     if (d < min_d)
                     {
                        min_d = d;
                        min_k = k;
                     }
                  }
                  /* Point to closest color */
                  _png_ptr->quantize_index[i] = (_png_byte)min_k;
               }
            }
         }
         _png_free(_png_ptr, _png_ptr->quantize_sort);
         _png_ptr->quantize_sort = NULL;
      }
      else
      {
         /* This is much harder to do simply (and quickly).  Perhaps
          * we need to go through a median cut routine, but those
          * don't always behave themselves with only a few colors
          * as input.  So we will just find the closest two colors,
          * and throw out one of them (chosen somewhat randomly).
          * [We don't understand this at all, so if someone wants to
          *  work on improving it, be our guest - AED, GRP]
          */
         int i;
         int max_d;
         int num_new_palette;
         _png_dsortp t;
         _png_dsortpp hash;

         t = NULL;

         /* Initialize palette index arrays */
         _png_ptr->index_to_palette = (_png_bytep)_png_malloc(_png_ptr,
             (_png_uint_32)(num_palette * _png_sizeof(_png_byte)));
         _png_ptr->palette_to_index = (_png_bytep)_png_malloc(_png_ptr,
             (_png_uint_32)(num_palette * _png_sizeof(_png_byte)));

         /* Initialize the sort array */
         for (i = 0; i < num_palette; i++)
         {
            _png_ptr->index_to_palette[i] = (_png_byte)i;
            _png_ptr->palette_to_index[i] = (_png_byte)i;
         }

         hash = (_png_dsortpp)_png_calloc(_png_ptr, (_png_uint_32)(769 *
             _png_sizeof(_png_dsortp)));

         num_new_palette = num_palette;

         /* Initial wild guess at how far apart the farthest pixel
          * pair we will be eliminating will be.  Larger
          * numbers mean more areas will be allocated, Smaller
          * numbers run the risk of not saving enough data, and
          * having to do this all over again.
          *
          * I have not done extensive checking on this number.
          */
         max_d = 96;

         while (num_new_palette > maximum_colors)
         {
            for (i = 0; i < num_new_palette - 1; i++)
            {
               int j;

               for (j = i + 1; j < num_new_palette; j++)
               {
                  int d;

                  d = PNG_COLOR_DIST(palette[i], palette[j]);

                  if (d <= max_d)
                  {

                     t = (_png_dsortp)_png_malloc_warn(_png_ptr,
                         (_png_uint_32)(_png_sizeof(_png_dsort)));

                     if (t == NULL)
                         break;

                     t->next = hash[d];
                     t->left = (_png_byte)i;
                     t->right = (_png_byte)j;
                     hash[d] = t;
                  }
               }
               if (t == NULL)
                  break;
            }

            if (t != NULL)
            for (i = 0; i <= max_d; i++)
            {
               if (hash[i] != NULL)
               {
                  _png_dsortp p;

                  for (p = hash[i]; p; p = p->next)
                  {
                     if ((int)_png_ptr->index_to_palette[p->left]
                         < num_new_palette &&
                         (int)_png_ptr->index_to_palette[p->right]
                         < num_new_palette)
                     {
                        int j, next_j;

                        if (num_new_palette & 0x01)
                        {
                           j = p->left;
                           next_j = p->right;
                        }
                        else
                        {
                           j = p->right;
                           next_j = p->left;
                        }

                        num_new_palette--;
                        palette[_png_ptr->index_to_palette[j]]
                            = palette[num_new_palette];
                        if (!full_quantize)
                        {
                           int k;

                           for (k = 0; k < num_palette; k++)
                           {
                              if (_png_ptr->quantize_index[k] ==
                                  _png_ptr->index_to_palette[j])
                                 _png_ptr->quantize_index[k] =
                                     _png_ptr->index_to_palette[next_j];

                              if ((int)_png_ptr->quantize_index[k] ==
                                  num_new_palette)
                                 _png_ptr->quantize_index[k] =
                                     _png_ptr->index_to_palette[j];
                           }
                        }

                        _png_ptr->index_to_palette[_png_ptr->palette_to_index
                            [num_new_palette]] = _png_ptr->index_to_palette[j];

                        _png_ptr->palette_to_index[_png_ptr->index_to_palette[j]]
                            = _png_ptr->palette_to_index[num_new_palette];

                        _png_ptr->index_to_palette[j] =
                            (_png_byte)num_new_palette;

                        _png_ptr->palette_to_index[num_new_palette] =
                            (_png_byte)j;
                     }
                     if (num_new_palette <= maximum_colors)
                        break;
                  }
                  if (num_new_palette <= maximum_colors)
                     break;
               }
            }

            for (i = 0; i < 769; i++)
            {
               if (hash[i] != NULL)
               {
                  _png_dsortp p = hash[i];
                  while (p)
                  {
                     t = p->next;
                     _png_free(_png_ptr, p);
                     p = t;
                  }
               }
               hash[i] = 0;
            }
            max_d += 96;
         }
         _png_free(_png_ptr, hash);
         _png_free(_png_ptr, _png_ptr->palette_to_index);
         _png_free(_png_ptr, _png_ptr->index_to_palette);
         _png_ptr->palette_to_index = NULL;
         _png_ptr->index_to_palette = NULL;
      }
      num_palette = maximum_colors;
   }
   if (_png_ptr->palette == NULL)
   {
      _png_ptr->palette = palette;
   }
   _png_ptr->num_palette = (_png_uint_16)num_palette;

   if (full_quantize)
   {
      int i;
      _png_bytep distance;
      int total_bits = PNG_QUANTIZE_RED_BITS + PNG_QUANTIZE_GREEN_BITS +
          PNG_QUANTIZE_BLUE_BITS;
      int num_red = (1 << PNG_QUANTIZE_RED_BITS);
      int num_green = (1 << PNG_QUANTIZE_GREEN_BITS);
      int num_blue = (1 << PNG_QUANTIZE_BLUE_BITS);
      _png_size_t num_entries = ((_png_size_t)1 << total_bits);

      _png_ptr->palette_lookup = (_png_bytep)_png_calloc(_png_ptr,
          (_png_uint_32)(num_entries * _png_sizeof(_png_byte)));

      distance = (_png_bytep)_png_malloc(_png_ptr, (_png_uint_32)(num_entries *
          _png_sizeof(_png_byte)));

      _png_memset(distance, 0xff, num_entries * _png_sizeof(_png_byte));

      for (i = 0; i < num_palette; i++)
      {
         int ir, ig, ib;
         int r = (palette[i].red >> (8 - PNG_QUANTIZE_RED_BITS));
         int g = (palette[i].green >> (8 - PNG_QUANTIZE_GREEN_BITS));
         int b = (palette[i].blue >> (8 - PNG_QUANTIZE_BLUE_BITS));

         for (ir = 0; ir < num_red; ir++)
         {
            /* int dr = abs(ir - r); */
            int dr = ((ir > r) ? ir - r : r - ir);
            int index_r = (ir << (PNG_QUANTIZE_BLUE_BITS +
                PNG_QUANTIZE_GREEN_BITS));

            for (ig = 0; ig < num_green; ig++)
            {
               /* int dg = abs(ig - g); */
               int dg = ((ig > g) ? ig - g : g - ig);
               int dt = dr + dg;
               int dm = ((dr > dg) ? dr : dg);
               int index_g = index_r | (ig << PNG_QUANTIZE_BLUE_BITS);

               for (ib = 0; ib < num_blue; ib++)
               {
                  int d_index = index_g | ib;
                  /* int db = abs(ib - b); */
                  int db = ((ib > b) ? ib - b : b - ib);
                  int dmax = ((dm > db) ? dm : db);
                  int d = dmax + dt + db;

                  if (d < (int)distance[d_index])
                  {
                     distance[d_index] = (_png_byte)d;
                     _png_ptr->palette_lookup[d_index] = (_png_byte)i;
                  }
               }
            }
         }
      }

      _png_free(_png_ptr, distance);
   }
}
#endif /* PNG_READ_QUANTIZE_SUPPORTED */

#ifdef PNG_READ_GAMMA_SUPPORTED
void PNGFAPI
_png_set_gamma_fixed(_png_structp _png_ptr, _png_fixed_point scrn_gamma,
   _png_fixed_point file_gamma)
{
   _png_debug(1, "in _png_set_gamma_fixed");

   if (_png_ptr == NULL)
      return;

   /* New in libpng-1.5.4 - reserve particular negative values as flags. */
   scrn_gamma = translate_gamma_flags(_png_ptr, scrn_gamma, 1/*screen*/);
   file_gamma = translate_gamma_flags(_png_ptr, file_gamma, 0/*file*/);

#if PNG_LIBPNG_VER >= 10600
   /* Checking the gamma values for being >0 was added in 1.5.4 along with the
    * premultiplied alpha support; this actually hides an undocumented feature
    * of the previous implementation which allowed gamma processing to be
    * disabled in background handling.  There is no evidence (so far) that this
    * was being used; however, _png_set_background itself accepted and must still
    * accept '0' for the gamma value it takes, because it isn't always used.
    *
    * Since this is an API change (albeit a very minor one that removes an
    * undocumented API feature) it will not be made until libpng-1.6.0.
    */
   if (file_gamma <= 0)
      _png_error(_png_ptr, "invalid file gamma in _png_set_gamma");

   if (scrn_gamma <= 0)
      _png_error(_png_ptr, "invalid screen gamma in _png_set_gamma");
#endif

   /* Set the gamma values unconditionally - this overrides the value in the PNG
    * file if a gAMA chunk was present.  _png_set_alpha_mode provides a
    * different, easier, way to default the file gamma.
    */
   _png_ptr->gamma = file_gamma;
   _png_ptr->screen_gamma = scrn_gamma;
}

#  ifdef PNG_FLOATING_POINT_SUPPORTED
void PNGAPI
_png_set_gamma(_png_structp _png_ptr, double scrn_gamma, double file_gamma)
{
   _png_set_gamma_fixed(_png_ptr, convert_gamma_value(_png_ptr, scrn_gamma),
      convert_gamma_value(_png_ptr, file_gamma));
}
#  endif /* FLOATING_POINT_SUPPORTED */
#endif /* READ_GAMMA */

#ifdef PNG_READ_EXPAND_SUPPORTED
/* Expand paletted images to RGB, expand grayscale images of
 * less than 8-bit depth to 8-bit depth, and expand tRNS chunks
 * to alpha channels.
 */
void PNGAPI
_png_set_expand(_png_structp _png_ptr)
{
   _png_debug(1, "in _png_set_expand");

   if (_png_ptr == NULL)
      return;

   _png_ptr->transformations |= (PNG_EXPAND | PNG_EXPAND_tRNS);
   _png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}

/* GRR 19990627:  the following three functions currently are identical
 *  to _png_set_expand().  However, it is entirely reasonable that someone
 *  might wish to expand an indexed image to RGB but *not* expand a single,
 *  fully transparent palette entry to a full alpha channel--perhaps instead
 *  convert tRNS to the grayscale/RGB format (16-bit RGB value), or replace
 *  the transparent color with a particular RGB value, or drop tRNS entirely.
 *  IOW, a future version of the library may make the transformations flag
 *  a bit more fine-grained, with separate bits for each of these three
 *  functions.
 *
 *  More to the point, these functions make it obvious what libpng will be
 *  doing, whereas "expand" can (and does) mean any number of things.
 *
 *  GRP 20060307: In libpng-1.2.9, _png_set_gray_1_2_4_to_8() was modified
 *  to expand only the sample depth but not to expand the tRNS to alpha
 *  and its name was changed to _png_set_expand_gray_1_2_4_to_8().
 */

/* Expand paletted images to RGB. */
void PNGAPI
_png_set_palette_to_rgb(_png_structp _png_ptr)
{
   _png_debug(1, "in _png_set_palette_to_rgb");

   if (_png_ptr == NULL)
      return;

   _png_ptr->transformations |= (PNG_EXPAND | PNG_EXPAND_tRNS);
   _png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}

/* Expand grayscale images of less than 8-bit depth to 8 bits. */
void PNGAPI
_png_set_expand_gray_1_2_4_to_8(_png_structp _png_ptr)
{
   _png_debug(1, "in _png_set_expand_gray_1_2_4_to_8");

   if (_png_ptr == NULL)
      return;

   _png_ptr->transformations |= PNG_EXPAND;
   _png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}



/* Expand tRNS chunks to alpha channels. */
void PNGAPI
_png_set_tRNS_to_alpha(_png_structp _png_ptr)
{
   _png_debug(1, "in _png_set_tRNS_to_alpha");

   _png_ptr->transformations |= (PNG_EXPAND | PNG_EXPAND_tRNS);
   _png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
}
#endif /* defined(PNG_READ_EXPAND_SUPPORTED) */

#ifdef PNG_READ_EXPAND_16_SUPPORTED
/* Expand to 16-bit channels, expand the tRNS chunk too (because otherwise
 * it may not work correctly.)
 */
void PNGAPI
_png_set_expand_16(_png_structp _png_ptr)
{
   _png_debug(1, "in _png_set_expand_16");

   if (_png_ptr == NULL)
      return;

   _png_ptr->transformations |= (PNG_EXPAND_16 | PNG_EXPAND | PNG_EXPAND_tRNS);
   _png_ptr->flags &= ~PNG_FLAG_ROW_INIT;

   /* New API, make sure apps call the correct initializers: */
   _png_ptr->flags |= PNG_FLAG_DETECT_UNINITIALIZED;
}
#endif

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
void PNGAPI
_png_set_gray_to_rgb(_png_structp _png_ptr)
{
   _png_debug(1, "in _png_set_gray_to_rgb");

   if (_png_ptr != NULL)
   {
      /* Because rgb must be 8 bits or more: */
      _png_set_expand_gray_1_2_4_to_8(_png_ptr);
      _png_ptr->transformations |= PNG_GRAY_TO_RGB;
      _png_ptr->flags &= ~PNG_FLAG_ROW_INIT;
   }
}
#endif

#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
void PNGFAPI
_png_set_rgb_to_gray_fixed(_png_structp _png_ptr, int error_action,
    _png_fixed_point red, _png_fixed_point green)
{
   _png_debug(1, "in _png_set_rgb_to_gray");

   if (_png_ptr == NULL)
      return;

   switch(error_action)
   {
      case PNG_ERROR_ACTION_NONE:
         _png_ptr->transformations |= PNG_RGB_TO_GRAY;
         break;

      case PNG_ERROR_ACTION_WARN:
         _png_ptr->transformations |= PNG_RGB_TO_GRAY_WARN;
         break;

      case PNG_ERROR_ACTION_ERROR:
         _png_ptr->transformations |= PNG_RGB_TO_GRAY_ERR;
         break;

      default:
         _png_error(_png_ptr, "invalid error action to rgb_to_gray");
         break;
   }
   if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
#ifdef PNG_READ_EXPAND_SUPPORTED
      _png_ptr->transformations |= PNG_EXPAND;
#else
   {
      _png_warning(_png_ptr,
        "Cannot do RGB_TO_GRAY without EXPAND_SUPPORTED");

      _png_ptr->transformations &= ~PNG_RGB_TO_GRAY;
   }
#endif
   {
      if (red >= 0 && green >= 0 && red + green <= PNG_FP_1)
      {
         _png_uint_16 red_int, green_int;

         /* NOTE: this calculation does not round, but this behavior is retained
          * for consistency, the inaccuracy is very small.  The code here always
          * overwrites the coefficients, regardless of whether they have been
          * defaulted or set already.
          */
         red_int = (_png_uint_16)(((_png_uint_32)red*32768)/100000);
         green_int = (_png_uint_16)(((_png_uint_32)green*32768)/100000);

         _png_ptr->rgb_to_gray_red_coeff   = red_int;
         _png_ptr->rgb_to_gray_green_coeff = green_int;
         _png_ptr->rgb_to_gray_coefficients_set = 1;
      }

      else
      {
         if (red >= 0 && green >= 0)
            _png_warning(_png_ptr,
               "ignoring out of range rgb_to_gray coefficients");

         /* Use the defaults, from the cHRM chunk if set, else the historical
          * values which are close to the sRGB/HDTV/ITU-Rec 709 values.  See
          * _png_do_rgb_to_gray for more discussion of the values.  In this case
          * the coefficients are not marked as 'set' and are not overwritten if
          * something has already provided a default.
          */
         if (_png_ptr->rgb_to_gray_red_coeff == 0 &&
            _png_ptr->rgb_to_gray_green_coeff == 0)
         {
            _png_ptr->rgb_to_gray_red_coeff   = 6968;
            _png_ptr->rgb_to_gray_green_coeff = 23434;
            /* _png_ptr->rgb_to_gray_blue_coeff  = 2366; */
         }
      }
   }
}

#ifdef PNG_FLOATING_POINT_SUPPORTED
/* Convert a RGB image to a grayscale of the same width.  This allows us,
 * for example, to convert a 24 bpp RGB image into an 8 bpp grayscale image.
 */

void PNGAPI
_png_set_rgb_to_gray(_png_structp _png_ptr, int error_action, double red,
   double green)
{
   if (_png_ptr == NULL)
      return;

   _png_set_rgb_to_gray_fixed(_png_ptr, error_action,
      _png_fixed(_png_ptr, red, "rgb to gray red coefficient"),
      _png_fixed(_png_ptr, green, "rgb to gray green coefficient"));
}
#endif /* FLOATING POINT */

#endif

#if defined(PNG_READ_USER_TRANSFORM_SUPPORTED) || \
    defined(PNG_WRITE_USER_TRANSFORM_SUPPORTED)
void PNGAPI
_png_set_read_user_transform_fn(_png_structp _png_ptr, _png_user_transform_ptr
    read_user_transform_fn)
{
   _png_debug(1, "in _png_set_read_user_transform_fn");

   if (_png_ptr == NULL)
      return;

#ifdef PNG_READ_USER_TRANSFORM_SUPPORTED
   _png_ptr->transformations |= PNG_USER_TRANSFORM;
   _png_ptr->read_user_transform_fn = read_user_transform_fn;
#endif
}
#endif

#ifdef PNG_READ_TRANSFORMS_SUPPORTED
#ifdef PNG_READ_GAMMA_SUPPORTED
/* In the case of gamma transformations only do transformations on images where
 * the [file] gamma and screen_gamma are not close reciprocals, otherwise it
 * slows things down slightly, and also needlessly introduces small errors.
 */
static int /* PRIVATE */
_png_gamma_threshold(_png_fixed_point screen_gamma, _png_fixed_point file_gamma)
{
   /* PNG_GAMMA_THRESHOLD is the threshold for performing gamma
    * correction as a difference of the overall transform from 1.0
    *
    * We want to compare the threshold with s*f - 1, if we get
    * overflow here it is because of wacky gamma values so we
    * turn on processing anyway.
    */
   _png_fixed_point gtest;
   return !_png_muldiv(&gtest, screen_gamma, file_gamma, PNG_FP_1) ||
       _png_gamma_significant(gtest);
}
#endif

/* Initialize everything needed for the read.  This includes modifying
 * the palette.
 */

/*For the moment '_png_init_palette_transformations' and
 * '_png_init_rgb_transformations' only do some flag canceling optimizations.
 * The intent is that these two routines should have palette or rgb operations
 * extracted from '_png_init_read_transformations'.
 */
static void /* PRIVATE */
_png_init_palette_transformations(_png_structp _png_ptr)
{
   /* Called to handle the (input) palette case.  In _png_do_read_transformations
    * the first step is to expand the palette if requested, so this code must
    * take care to only make changes that are invariant with respect to the
    * palette expansion, or only do them if there is no expansion.
    *
    * STRIP_ALPHA has already been handled in the caller (by setting num_trans
    * to 0.)
    */
   int input_has_alpha = 0;
   int input_has_transparency = 0;

   if (_png_ptr->num_trans > 0)
   {
      int i;

      /* Ignore if all the entries are opaque (unlikely!) */
      for (i=0; i<_png_ptr->num_trans; ++i)
         if (_png_ptr->trans_alpha[i] == 255)
            continue;
         else if (_png_ptr->trans_alpha[i] == 0)
            input_has_transparency = 1;
         else
            input_has_alpha = 1;
   }

   /* If no alpha we can optimize. */
   if (!input_has_alpha)
   {
      /* Any alpha means background and associative alpha processing is
       * required, however if the alpha is 0 or 1 throughout OPTIIMIZE_ALPHA
       * and ENCODE_ALPHA are irrelevant.
       */
      _png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
      _png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;

      if (!input_has_transparency)
         _png_ptr->transformations &= ~(PNG_COMPOSE | PNG_BACKGROUND_EXPAND);
   }

#if defined(PNG_READ_EXPAND_SUPPORTED) && defined(PNG_READ_BACKGROUND_SUPPORTED)
   /* _png_set_background handling - deals with the complexity of whether the
    * background color is in the file format or the screen format in the case
    * where an 'expand' will happen.
    */

   /* The following code cannot be entered in the alpha pre-multiplication case
    * because PNG_BACKGROUND_EXPAND is cancelled below.
    */
   if ((_png_ptr->transformations & PNG_BACKGROUND_EXPAND) &&
       (_png_ptr->transformations & PNG_EXPAND))
   {
      {
         _png_ptr->background.red   =
             _png_ptr->palette[_png_ptr->background.index].red;
         _png_ptr->background.green =
             _png_ptr->palette[_png_ptr->background.index].green;
         _png_ptr->background.blue  =
             _png_ptr->palette[_png_ptr->background.index].blue;

#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
        if (_png_ptr->transformations & PNG_INVERT_ALPHA)
        {
           if (!(_png_ptr->transformations & PNG_EXPAND_tRNS))
           {
              /* Invert the alpha channel (in tRNS) unless the pixels are
               * going to be expanded, in which case leave it for later
               */
              int i, istop = _png_ptr->num_trans;

              for (i=0; i<istop; i++)
                 _png_ptr->trans_alpha[i] = (_png_byte)(255 -
                    _png_ptr->trans_alpha[i]);
           }
        }
#endif /* PNG_READ_INVERT_ALPHA_SUPPORTED */
      }
   } /* background expand and (therefore) no alpha association. */
#endif /* PNG_READ_EXPAND_SUPPORTED && PNG_READ_BACKGROUND_SUPPORTED */
}

static void /* PRIVATE */
_png_init_rgb_transformations(_png_structp _png_ptr)
{
   /* Added to libpng-1.5.4: check the color type to determine whether there
    * is any alpha or transparency in the image and simply cancel the
    * background and alpha mode stuff if there isn't.
    */
   int input_has_alpha = (_png_ptr->color_type & PNG_COLOR_MASK_ALPHA) != 0;
   int input_has_transparency = _png_ptr->num_trans > 0;

   /* If no alpha we can optimize. */
   if (!input_has_alpha)
   {
      /* Any alpha means background and associative alpha processing is
       * required, however if the alpha is 0 or 1 throughout OPTIIMIZE_ALPHA
       * and ENCODE_ALPHA are irrelevant.
       */
#     ifdef PNG_READ_ALPHA_MODE_SUPPORTED
         _png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
         _png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
#     endif

      if (!input_has_transparency)
         _png_ptr->transformations &= ~(PNG_COMPOSE | PNG_BACKGROUND_EXPAND);
   }

#if defined(PNG_READ_EXPAND_SUPPORTED) && defined(PNG_READ_BACKGROUND_SUPPORTED)
   /* _png_set_background handling - deals with the complexity of whether the
    * background color is in the file format or the screen format in the case
    * where an 'expand' will happen.
    */

   /* The following code cannot be entered in the alpha pre-multiplication case
    * because PNG_BACKGROUND_EXPAND is cancelled below.
    */
   if ((_png_ptr->transformations & PNG_BACKGROUND_EXPAND) &&
       (_png_ptr->transformations & PNG_EXPAND) &&
       !(_png_ptr->color_type & PNG_COLOR_MASK_COLOR))
       /* i.e., GRAY or GRAY_ALPHA */
   {
      {
         /* Expand background and tRNS chunks */
         int gray = _png_ptr->background.gray;
         int trans_gray = _png_ptr->trans_color.gray;

         switch (_png_ptr->bit_depth)
         {
            case 1:
               gray *= 0xff;
               trans_gray *= 0xff;
               break;

            case 2:
               gray *= 0x55;
               trans_gray *= 0x55;
               break;

            case 4:
               gray *= 0x11;
               trans_gray *= 0x11;
               break;

            default:

            case 8:
               /* FALL THROUGH (already 8 bits) */

            case 16:
               /* Already a full 16 bits */
               break;
         }

         _png_ptr->background.red = _png_ptr->background.green =
            _png_ptr->background.blue = (_png_uint_16)gray;

         if (!(_png_ptr->transformations & PNG_EXPAND_tRNS))
         {
            _png_ptr->trans_color.red = _png_ptr->trans_color.green =
               _png_ptr->trans_color.blue = (_png_uint_16)trans_gray;
         }
      }
   } /* background expand and (therefore) no alpha association. */
#endif /* PNG_READ_EXPAND_SUPPORTED && PNG_READ_BACKGROUND_SUPPORTED */
}

void /* PRIVATE */
_png_init_read_transformations(_png_structp _png_ptr)
{
   _png_debug(1, "in _png_init_read_transformations");

   /* This internal function is called from _png_read_start_row in pngrutil.c
    * and it is called before the 'rowbytes' calculation is done, so the code
    * in here can change or update the transformations flags.
    *
    * First do updates that do not depend on the details of the PNG image data
    * being processed.
    */

#ifdef PNG_READ_GAMMA_SUPPORTED
   /* Prior to 1.5.4 these tests were performed from _png_set_gamma, 1.5.4 adds
    * _png_set_alpha_mode and this is another source for a default file gamma so
    * the test needs to be performed later - here.  In addition prior to 1.5.4
    * the tests were repeated for the PALETTE color type here - this is no
    * longer necessary (and doesn't seem to have been necessary before.)
    */
   {
      /* The following temporary indicates if overall gamma correction is
       * required.
       */
      int gamma_correction = 0;

      if (_png_ptr->gamma != 0) /* has been set */
      {
         if (_png_ptr->screen_gamma != 0) /* screen set too */
            gamma_correction = _png_gamma_threshold(_png_ptr->gamma,
               _png_ptr->screen_gamma);

         else
            /* Assume the output matches the input; a long time default behavior
             * of libpng, although the standard has nothing to say about this.
             */
            _png_ptr->screen_gamma = _png_reciprocal(_png_ptr->gamma);
      }

      else if (_png_ptr->screen_gamma != 0)
         /* The converse - assume the file matches the screen, note that this
          * perhaps undesireable default can (from 1.5.4) be changed by calling
          * _png_set_alpha_mode (even if the alpha handling mode isn't required
          * or isn't changed from the default.)
          */
         _png_ptr->gamma = _png_reciprocal(_png_ptr->screen_gamma);

      else /* neither are set */
         /* Just in case the following prevents any processing - file and screen
          * are both assumed to be linear and there is no way to introduce a
          * third gamma value other than _png_set_background with 'UNIQUE', and,
          * prior to 1.5.4
          */
         _png_ptr->screen_gamma = _png_ptr->gamma = PNG_FP_1;

      /* Now turn the gamma transformation on or off as appropriate.  Notice
       * that PNG_GAMMA just refers to the file->screen correction.  Alpha
       * composition may independently cause gamma correction because it needs
       * linear data (e.g. if the file has a gAMA chunk but the screen gamma
       * hasn't been specified.)  In any case this flag may get turned off in
       * the code immediately below if the transform can be handled outside the
       * row loop.
       */
      if (gamma_correction)
         _png_ptr->transformations |= PNG_GAMMA;

      else
         _png_ptr->transformations &= ~PNG_GAMMA;
   }
#endif

   /* Certain transformations have the effect of preventing other
    * transformations that happen afterward in _png_do_read_transformations,
    * resolve the interdependencies here.  From the code of
    * _png_do_read_transformations the order is:
    *
    *  1) PNG_EXPAND (including PNG_EXPAND_tRNS)
    *  2) PNG_STRIP_ALPHA (if no compose)
    *  3) PNG_RGB_TO_GRAY
    *  4) PNG_GRAY_TO_RGB iff !PNG_BACKGROUND_IS_GRAY
    *  5) PNG_COMPOSE
    *  6) PNG_GAMMA
    *  7) PNG_STRIP_ALPHA (if compose)
    *  8) PNG_ENCODE_ALPHA
    *  9) PNG_SCALE_16_TO_8
    * 10) PNG_16_TO_8
    * 11) PNG_QUANTIZE (converts to palette)
    * 12) PNG_EXPAND_16
    * 13) PNG_GRAY_TO_RGB iff PNG_BACKGROUND_IS_GRAY
    * 14) PNG_INVERT_MONO
    * 15) PNG_SHIFT
    * 16) PNG_PACK
    * 17) PNG_BGR
    * 18) PNG_PACKSWAP
    * 19) PNG_FILLER (includes PNG_ADD_ALPHA)
    * 20) PNG_INVERT_ALPHA
    * 21) PNG_SWAP_ALPHA
    * 22) PNG_SWAP_BYTES
    * 23) PNG_USER_TRANSFORM [must be last]
    */
#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
   if ((_png_ptr->transformations & PNG_STRIP_ALPHA) &&
      !(_png_ptr->transformations & PNG_COMPOSE))
   {
      /* Stripping the alpha channel happens immediately after the 'expand'
       * transformations, before all other transformation, so it cancels out
       * the alpha handling.  It has the side effect negating the effect of
       * PNG_EXPAND_tRNS too:
       */
      _png_ptr->transformations &= ~(PNG_BACKGROUND_EXPAND | PNG_ENCODE_ALPHA |
         PNG_EXPAND_tRNS);
      _png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;

      /* Kill the tRNS chunk itself too.  Prior to 1.5.4 this did not happen
       * so transparency information would remain just so long as it wasn't
       * expanded.  This produces unexpected API changes if the set of things
       * that do PNG_EXPAND_tRNS changes (perfectly possible given the
       * documentation - which says ask for what you want, accept what you
       * get.)  This makes the behavior consistent from 1.5.4:
       */
      _png_ptr->num_trans = 0;
   }
#endif /* STRIP_ALPHA supported, no COMPOSE */

#ifdef PNG_READ_ALPHA_MODE_SUPPORTED
   /* If the screen gamma is about 1.0 then the OPTIMIZE_ALPHA and ENCODE_ALPHA
    * settings will have no effect.
    */
   if (!_png_gamma_significant(_png_ptr->screen_gamma))
   {
      _png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
      _png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
   }
#endif

#if defined(PNG_READ_EXPAND_SUPPORTED) && \
   defined(PNG_READ_BACKGROUND_SUPPORTED) && \
   defined(PNG_READ_GRAY_TO_RGB_SUPPORTED)
   /* Detect gray background and attempt to enable optimization for
    * gray --> RGB case.
    *
    * Note:  if PNG_BACKGROUND_EXPAND is set and color_type is either RGB or
    * RGB_ALPHA (in which case need_expand is superfluous anyway), the
    * background color might actually be gray yet not be flagged as such.
    * This is not a problem for the current code, which uses
    * PNG_BACKGROUND_IS_GRAY only to decide when to do the
    * _png_do_gray_to_rgb() transformation.
    *
    * TODO: this code needs to be revised to avoid the complexity and
    * interdependencies.  The color type of the background should be recorded in
    * _png_set_background, along with the bit depth, then the code has a record
    * of exactly what color space the background is currently in.
    */
   if (_png_ptr->transformations & PNG_BACKGROUND_EXPAND)
   {
      /* PNG_BACKGROUND_EXPAND: the background is in the file color space, so if
       * the file was grayscale the background value is gray.
       */
      if (!(_png_ptr->color_type & PNG_COLOR_MASK_COLOR))
         _png_ptr->mode |= PNG_BACKGROUND_IS_GRAY;
   }

   else if (_png_ptr->transformations & PNG_COMPOSE)
   {
      /* PNG_COMPOSE: _png_set_background was called with need_expand false,
       * so the color is in the color space of the output or _png_set_alpha_mode
       * was called and the color is black.  Ignore RGB_TO_GRAY because that
       * happens before GRAY_TO_RGB.
       */
      if (_png_ptr->transformations & PNG_GRAY_TO_RGB)
      {
         if (_png_ptr->background.red == _png_ptr->background.green &&
             _png_ptr->background.red == _png_ptr->background.blue)
         {
            _png_ptr->mode |= PNG_BACKGROUND_IS_GRAY;
            _png_ptr->background.gray = _png_ptr->background.red;
         }
      }
   }
#endif /* PNG_READ_GRAY_TO_RGB_SUPPORTED (etc) */

   /* For indexed PNG data (PNG_COLOR_TYPE_PALETTE) many of the transformations
    * can be performed directly on the palette, and some (such as rgb to gray)
    * can be optimized inside the palette.  This is particularly true of the
    * composite (background and alpha) stuff, which can be pretty much all done
    * in the palette even if the result is expanded to RGB or gray afterward.
    *
    * NOTE: this is Not Yet Implemented, the code behaves as in 1.5.1 and
    * earlier and the palette stuff is actually handled on the first row.  This
    * leads to the reported bug that the palette returned by _png_get_PLTE is not
    * updated.
    */
   if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      _png_init_palette_transformations(_png_ptr);

   else
      _png_init_rgb_transformations(_png_ptr);

#if defined(PNG_READ_BACKGROUND_SUPPORTED) && \
   defined(PNG_READ_EXPAND_16_SUPPORTED)
   if ((_png_ptr->transformations & PNG_EXPAND_16) &&
      (_png_ptr->transformations & PNG_COMPOSE) &&
      !(_png_ptr->transformations & PNG_BACKGROUND_EXPAND) &&
      _png_ptr->bit_depth != 16)
   {
      /* TODO: fix this.  Because the expand_16 operation is after the compose
       * handling the background color must be 8, not 16, bits deep, but the
       * application will supply a 16-bit value so reduce it here.
       *
       * The PNG_BACKGROUND_EXPAND code above does not expand to 16 bits at
       * present, so that case is ok (until do_expand_16 is moved.)
       *
       * NOTE: this discards the low 16 bits of the user supplied background
       * color, but until expand_16 works properly there is no choice!
       */
#     define CHOP(x) (x)=((_png_uint_16)(((_png_uint_32)(x)*255+32895) >> 16))
      CHOP(_png_ptr->background.red);
      CHOP(_png_ptr->background.green);
      CHOP(_png_ptr->background.blue);
      CHOP(_png_ptr->background.gray);
#     undef CHOP
   }
#endif /* PNG_READ_BACKGROUND_SUPPORTED && PNG_READ_EXPAND_16_SUPPORTED */

#if defined(PNG_READ_BACKGROUND_SUPPORTED) && \
   (defined(PNG_READ_SCALE_16_TO_8_SUPPORTED) || \
   defined(PNG_READ_STRIP_16_TO_8_SUPPORTED))
   if ((_png_ptr->transformations & (PNG_16_TO_8|PNG_SCALE_16_TO_8)) &&
      (_png_ptr->transformations & PNG_COMPOSE) &&
      !(_png_ptr->transformations & PNG_BACKGROUND_EXPAND) &&
      _png_ptr->bit_depth == 16)
   {
      /* On the other hand, if a 16-bit file is to be reduced to 8-bits per
       * component this will also happen after PNG_COMPOSE and so the background
       * color must be pre-expanded here.
       *
       * TODO: fix this too.
       */
      _png_ptr->background.red = (_png_uint_16)(_png_ptr->background.red * 257);
      _png_ptr->background.green =
         (_png_uint_16)(_png_ptr->background.green * 257);
      _png_ptr->background.blue = (_png_uint_16)(_png_ptr->background.blue * 257);
      _png_ptr->background.gray = (_png_uint_16)(_png_ptr->background.gray * 257);
   }
#endif

   /* NOTE: below 'PNG_READ_ALPHA_MODE_SUPPORTED' is presumed to also enable the
    * background support (see the comments in scripts/pnglibconf.dfa), this
    * allows pre-multiplication of the alpha channel to be implemented as
    * compositing on black.  This is probably sub-optimal and has been done in
    * 1.5.4 betas simply to enable external critique and testing (i.e. to
    * implement the new API quickly, without lots of internal changes.)
    */

#ifdef PNG_READ_GAMMA_SUPPORTED
#  ifdef PNG_READ_BACKGROUND_SUPPORTED
      /* Includes ALPHA_MODE */
      _png_ptr->background_1 = _png_ptr->background;
#  endif

   /* This needs to change - in the palette image case a whole set of tables are
    * built when it would be quicker to just calculate the correct value for
    * each palette entry directly.  Also, the test is too tricky - why check
    * PNG_RGB_TO_GRAY if PNG_GAMMA is not set?  The answer seems to be that
    * PNG_GAMMA is cancelled even if the gamma is known?  The test excludes the
    * PNG_COMPOSE case, so apparently if there is no *overall* gamma correction
    * the gamma tables will not be built even if composition is required on a
    * gamma encoded value.
    *
    * In 1.5.4 this is addressed below by an additional check on the individual
    * file gamma - if it is not 1.0 both RGB_TO_GRAY and COMPOSE need the
    * tables.
    */
   if ((_png_ptr->transformations & PNG_GAMMA)
      || ((_png_ptr->transformations & PNG_RGB_TO_GRAY)
         && (_png_gamma_significant(_png_ptr->gamma) ||
            _png_gamma_significant(_png_ptr->screen_gamma)))
      || ((_png_ptr->transformations & PNG_COMPOSE)
         && (_png_gamma_significant(_png_ptr->gamma)
            || _png_gamma_significant(_png_ptr->screen_gamma)
#  ifdef PNG_READ_BACKGROUND_SUPPORTED
            || (_png_ptr->background_gamma_type == PNG_BACKGROUND_GAMMA_UNIQUE
               && _png_gamma_significant(_png_ptr->background_gamma))
#  endif
      )) || ((_png_ptr->transformations & PNG_ENCODE_ALPHA)
         && _png_gamma_significant(_png_ptr->screen_gamma))
      )
   {
      _png_build_gamma_table(_png_ptr, _png_ptr->bit_depth);

#ifdef PNG_READ_BACKGROUND_SUPPORTED
      if (_png_ptr->transformations & PNG_COMPOSE)
      {
         /* Issue a warning about this combination: because RGB_TO_GRAY is
          * optimized to do the gamma transform if present yet do_background has
          * to do the same thing if both options are set a
          * double-gamma-correction happens.  This is true in all versions of
          * libpng to date.
          */
         if (_png_ptr->transformations & PNG_RGB_TO_GRAY)
            _png_warning(_png_ptr,
               "libpng does not support gamma+background+rgb_to_gray");

         if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
         {
            /* We don't get to here unless there is a tRNS chunk with non-opaque
             * entries - see the checking code at the start of this function.
             */
            _png_color back, back_1;
            _png_colorp palette = _png_ptr->palette;
            int num_palette = _png_ptr->num_palette;
            int i;
            if (_png_ptr->background_gamma_type == PNG_BACKGROUND_GAMMA_FILE)
            {

               back.red = _png_ptr->gamma_table[_png_ptr->background.red];
               back.green = _png_ptr->gamma_table[_png_ptr->background.green];
               back.blue = _png_ptr->gamma_table[_png_ptr->background.blue];

               back_1.red = _png_ptr->gamma_to_1[_png_ptr->background.red];
               back_1.green = _png_ptr->gamma_to_1[_png_ptr->background.green];
               back_1.blue = _png_ptr->gamma_to_1[_png_ptr->background.blue];
            }
            else
            {
               _png_fixed_point g, gs;

               switch (_png_ptr->background_gamma_type)
               {
                  case PNG_BACKGROUND_GAMMA_SCREEN:
                     g = (_png_ptr->screen_gamma);
                     gs = PNG_FP_1;
                     break;

                  case PNG_BACKGROUND_GAMMA_FILE:
                     g = _png_reciprocal(_png_ptr->gamma);
                     gs = _png_reciprocal2(_png_ptr->gamma,
                        _png_ptr->screen_gamma);
                     break;

                  case PNG_BACKGROUND_GAMMA_UNIQUE:
                     g = _png_reciprocal(_png_ptr->background_gamma);
                     gs = _png_reciprocal2(_png_ptr->background_gamma,
                        _png_ptr->screen_gamma);
                     break;
                  default:
                     g = PNG_FP_1;    /* back_1 */
                     gs = PNG_FP_1;   /* back */
                     break;
               }

               if (_png_gamma_significant(gs))
               {
                  back.red = _png_gamma_8bit_correct(_png_ptr->background.red,
                      gs);
                  back.green = _png_gamma_8bit_correct(_png_ptr->background.green,
                      gs);
                  back.blue = _png_gamma_8bit_correct(_png_ptr->background.blue,
                      gs);
               }

               else
               {
                  back.red   = (_png_byte)_png_ptr->background.red;
                  back.green = (_png_byte)_png_ptr->background.green;
                  back.blue  = (_png_byte)_png_ptr->background.blue;
               }

               if (_png_gamma_significant(g))
               {
                  back_1.red = _png_gamma_8bit_correct(_png_ptr->background.red,
                     g);
                  back_1.green = _png_gamma_8bit_correct(
                     _png_ptr->background.green, g);
                  back_1.blue = _png_gamma_8bit_correct(_png_ptr->background.blue,
                     g);
               }

               else
               {
                  back_1.red   = (_png_byte)_png_ptr->background.red;
                  back_1.green = (_png_byte)_png_ptr->background.green;
                  back_1.blue  = (_png_byte)_png_ptr->background.blue;
               }
            }

            for (i = 0; i < num_palette; i++)
            {
               if (i < (int)_png_ptr->num_trans &&
                   _png_ptr->trans_alpha[i] != 0xff)
               {
                  if (_png_ptr->trans_alpha[i] == 0)
                  {
                     palette[i] = back;
                  }
                  else /* if (_png_ptr->trans_alpha[i] != 0xff) */
                  {
                     _png_byte v, w;

                     v = _png_ptr->gamma_to_1[palette[i].red];
                     _png_composite(w, v, _png_ptr->trans_alpha[i], back_1.red);
                     palette[i].red = _png_ptr->gamma_from_1[w];

                     v = _png_ptr->gamma_to_1[palette[i].green];
                     _png_composite(w, v, _png_ptr->trans_alpha[i], back_1.green);
                     palette[i].green = _png_ptr->gamma_from_1[w];

                     v = _png_ptr->gamma_to_1[palette[i].blue];
                     _png_composite(w, v, _png_ptr->trans_alpha[i], back_1.blue);
                     palette[i].blue = _png_ptr->gamma_from_1[w];
                  }
               }
               else
               {
                  palette[i].red = _png_ptr->gamma_table[palette[i].red];
                  palette[i].green = _png_ptr->gamma_table[palette[i].green];
                  palette[i].blue = _png_ptr->gamma_table[palette[i].blue];
               }
            }

            /* Prevent the transformations being done again.
             *
             * NOTE: this is highly dubious; it removes the transformations in
             * place.  This seems inconsistent with the general treatment of the
             * transformations elsewhere.
             */
            _png_ptr->transformations &= ~(PNG_COMPOSE | PNG_GAMMA);
         } /* color_type == PNG_COLOR_TYPE_PALETTE */

         /* if (_png_ptr->background_gamma_type!=PNG_BACKGROUND_GAMMA_UNKNOWN) */
         else /* color_type != PNG_COLOR_TYPE_PALETTE */
         {
            int gs_sig, g_sig;
            _png_fixed_point g = PNG_FP_1;  /* Correction to linear */
            _png_fixed_point gs = PNG_FP_1; /* Correction to screen */

            switch (_png_ptr->background_gamma_type)
            {
               case PNG_BACKGROUND_GAMMA_SCREEN:
                  g = _png_ptr->screen_gamma;
                  /* gs = PNG_FP_1; */
                  break;

               case PNG_BACKGROUND_GAMMA_FILE:
                  g = _png_reciprocal(_png_ptr->gamma);
                  gs = _png_reciprocal2(_png_ptr->gamma, _png_ptr->screen_gamma);
                  break;

               case PNG_BACKGROUND_GAMMA_UNIQUE:
                  g = _png_reciprocal(_png_ptr->background_gamma);
                  gs = _png_reciprocal2(_png_ptr->background_gamma,
                      _png_ptr->screen_gamma);
                  break;

               default:
                  _png_error(_png_ptr, "invalid background gamma type");
            }

            g_sig = _png_gamma_significant(g);
            gs_sig = _png_gamma_significant(gs);

            if (g_sig)
               _png_ptr->background_1.gray = _png_gamma_correct(_png_ptr,
                   _png_ptr->background.gray, g);

            if (gs_sig)
               _png_ptr->background.gray = _png_gamma_correct(_png_ptr,
                   _png_ptr->background.gray, gs);

            if ((_png_ptr->background.red != _png_ptr->background.green) ||
                (_png_ptr->background.red != _png_ptr->background.blue) ||
                (_png_ptr->background.red != _png_ptr->background.gray))
            {
               /* RGB or RGBA with color background */
               if (g_sig)
               {
                  _png_ptr->background_1.red = _png_gamma_correct(_png_ptr,
                      _png_ptr->background.red, g);

                  _png_ptr->background_1.green = _png_gamma_correct(_png_ptr,
                      _png_ptr->background.green, g);

                  _png_ptr->background_1.blue = _png_gamma_correct(_png_ptr,
                      _png_ptr->background.blue, g);
               }

               if (gs_sig)
               {
                  _png_ptr->background.red = _png_gamma_correct(_png_ptr,
                      _png_ptr->background.red, gs);

                  _png_ptr->background.green = _png_gamma_correct(_png_ptr,
                      _png_ptr->background.green, gs);

                  _png_ptr->background.blue = _png_gamma_correct(_png_ptr,
                      _png_ptr->background.blue, gs);
               }
            }

            else
            {
               /* GRAY, GRAY ALPHA, RGB, or RGBA with gray background */
               _png_ptr->background_1.red = _png_ptr->background_1.green
                   = _png_ptr->background_1.blue = _png_ptr->background_1.gray;

               _png_ptr->background.red = _png_ptr->background.green
                   = _png_ptr->background.blue = _png_ptr->background.gray;
            }

            /* The background is now in screen gamma: */
            _png_ptr->background_gamma_type = PNG_BACKGROUND_GAMMA_SCREEN;
         } /* color_type != PNG_COLOR_TYPE_PALETTE */
      }/* _png_ptr->transformations & PNG_BACKGROUND */

      else
      /* Transformation does not include PNG_BACKGROUND */
#endif /* PNG_READ_BACKGROUND_SUPPORTED */
      if (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE
#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
         /* RGB_TO_GRAY needs to have non-gamma-corrected values! */
         && ((_png_ptr->transformations & PNG_EXPAND) == 0 ||
         (_png_ptr->transformations & PNG_RGB_TO_GRAY) == 0)
#endif
         )
      {
         _png_colorp palette = _png_ptr->palette;
         int num_palette = _png_ptr->num_palette;
         int i;

         /* NOTE: there are other transformations that should probably be in
          * here too.
          */
         for (i = 0; i < num_palette; i++)
         {
            palette[i].red = _png_ptr->gamma_table[palette[i].red];
            palette[i].green = _png_ptr->gamma_table[palette[i].green];
            palette[i].blue = _png_ptr->gamma_table[palette[i].blue];
         }

         /* Done the gamma correction. */
         _png_ptr->transformations &= ~PNG_GAMMA;
      } /* color_type == PALETTE && !PNG_BACKGROUND transformation */
   }
#ifdef PNG_READ_BACKGROUND_SUPPORTED
   else
#endif
#endif /* PNG_READ_GAMMA_SUPPORTED */

#ifdef PNG_READ_BACKGROUND_SUPPORTED
   /* No GAMMA transformation (see the hanging else 4 lines above) */
   if ((_png_ptr->transformations & PNG_COMPOSE) &&
       (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE))
   {
      int i;
      int istop = (int)_png_ptr->num_trans;
      _png_color back;
      _png_colorp palette = _png_ptr->palette;

      back.red   = (_png_byte)_png_ptr->background.red;
      back.green = (_png_byte)_png_ptr->background.green;
      back.blue  = (_png_byte)_png_ptr->background.blue;

      for (i = 0; i < istop; i++)
      {
         if (_png_ptr->trans_alpha[i] == 0)
         {
            palette[i] = back;
         }

         else if (_png_ptr->trans_alpha[i] != 0xff)
         {
            /* The _png_composite() macro is defined in png.h */
            _png_composite(palette[i].red, palette[i].red,
                _png_ptr->trans_alpha[i], back.red);

            _png_composite(palette[i].green, palette[i].green,
                _png_ptr->trans_alpha[i], back.green);

            _png_composite(palette[i].blue, palette[i].blue,
                _png_ptr->trans_alpha[i], back.blue);
         }
      }

      _png_ptr->transformations &= ~PNG_COMPOSE;
   }
#endif /* PNG_READ_BACKGROUND_SUPPORTED */

#ifdef PNG_READ_SHIFT_SUPPORTED
   if ((_png_ptr->transformations & PNG_SHIFT) &&
      !(_png_ptr->transformations & PNG_EXPAND) &&
       (_png_ptr->color_type == PNG_COLOR_TYPE_PALETTE))
   {
      int i;
      int istop = _png_ptr->num_palette;
      int shift = 8 - _png_ptr->sig_bit.red;

      _png_ptr->transformations &= ~PNG_SHIFT;

      /* significant bits can be in the range 1 to 7 for a meaninful result, if
       * the number of significant bits is 0 then no shift is done (this is an
       * error condition which is silently ignored.)
       */
      if (shift > 0 && shift < 8) for (i=0; i<istop; ++i)
      {
         int component = _png_ptr->palette[i].red;

         component >>= shift;
         _png_ptr->palette[i].red = (_png_byte)component;
      }

      shift = 8 - _png_ptr->sig_bit.green;
      if (shift > 0 && shift < 8) for (i=0; i<istop; ++i)
      {
         int component = _png_ptr->palette[i].green;

         component >>= shift;
         _png_ptr->palette[i].green = (_png_byte)component;
      }

      shift = 8 - _png_ptr->sig_bit.blue;
      if (shift > 0 && shift < 8) for (i=0; i<istop; ++i)
      {
         int component = _png_ptr->palette[i].blue;

         component >>= shift;
         _png_ptr->palette[i].blue = (_png_byte)component;
      }
   }
#endif  /* PNG_READ_SHIFT_SUPPORTED */
}

/* Modify the info structure to reflect the transformations.  The
 * info should be updated so a PNG file could be written with it,
 * assuming the transformations result in valid PNG data.
 */
void /* PRIVATE */
_png_read_transform_info(_png_structp _png_ptr, _png_infop info_ptr)
{
   _png_debug(1, "in _png_read_transform_info");

#ifdef PNG_READ_EXPAND_SUPPORTED
   if (_png_ptr->transformations & PNG_EXPAND)
   {
      if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      {
         /* This check must match what actually happens in
          * _png_do_expand_palette; if it ever checks the tRNS chunk to see if
          * it is all opaque we must do the same (at present it does not.)
          */
         if (_png_ptr->num_trans > 0)
            info_ptr->color_type = PNG_COLOR_TYPE_RGB_ALPHA;

         else
            info_ptr->color_type = PNG_COLOR_TYPE_RGB;

         info_ptr->bit_depth = 8;
         info_ptr->num_trans = 0;
      }
      else
      {
         if (_png_ptr->num_trans)
         {
            if (_png_ptr->transformations & PNG_EXPAND_tRNS)
               info_ptr->color_type |= PNG_COLOR_MASK_ALPHA;
         }
         if (info_ptr->bit_depth < 8)
            info_ptr->bit_depth = 8;

         info_ptr->num_trans = 0;
      }
   }
#endif

#if defined(PNG_READ_BACKGROUND_SUPPORTED) ||\
   defined(PNG_READ_ALPHA_MODE_SUPPORTED)
   /* The following is almost certainly wrong unless the background value is in
    * the screen space!
    */
   if (_png_ptr->transformations & PNG_COMPOSE)
      info_ptr->background = _png_ptr->background;
#endif

#ifdef PNG_READ_GAMMA_SUPPORTED
   /* The following used to be conditional on PNG_GAMMA (prior to 1.5.4),
    * however it seems that the code in _png_init_read_transformations, which has
    * been called before this from _png_read_update_info->_png_read_start_row
    * sometimes does the gamma transform and cancels the flag.
    */
   info_ptr->gamma = _png_ptr->gamma;
#endif

   if (info_ptr->bit_depth == 16)
   {
#  ifdef PNG_READ_16BIT_SUPPORTED
#     ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
         if (_png_ptr->transformations & PNG_SCALE_16_TO_8)
            info_ptr->bit_depth = 8;
#     endif

#     ifdef PNG_READ_STRIP_16_TO_8_SUPPORTED
         if (_png_ptr->transformations & PNG_16_TO_8)
            info_ptr->bit_depth = 8;
#     endif

#  else
      /* No 16 bit support: force chopping 16-bit input down to 8, in this case
       * the app program can chose if both APIs are available by setting the
       * correct scaling to use.
       */
#     ifdef PNG_READ_STRIP_16_TO_8_SUPPORTED
         /* For compatibility with previous versions use the strip method by
          * default.  This code works because if PNG_SCALE_16_TO_8 is already
          * set the code below will do that in preference to the chop.
          */
         _png_ptr->transformations |= PNG_16_TO_8;
         info_ptr->bit_depth = 8;
#     else

#        ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
            _png_ptr->transformations |= PNG_SCALE_16_TO_8;
            info_ptr->bit_depth = 8;
#        else

            CONFIGURATION ERROR: you must enable at least one 16 to 8 method
#        endif
#    endif
#endif /* !READ_16BIT_SUPPORTED */
   }

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
   if (_png_ptr->transformations & PNG_GRAY_TO_RGB)
      info_ptr->color_type = (_png_byte)(info_ptr->color_type |
         PNG_COLOR_MASK_COLOR);
#endif

#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
   if (_png_ptr->transformations & PNG_RGB_TO_GRAY)
      info_ptr->color_type = (_png_byte)(info_ptr->color_type &
         ~PNG_COLOR_MASK_COLOR);
#endif

#ifdef PNG_READ_QUANTIZE_SUPPORTED
   if (_png_ptr->transformations & PNG_QUANTIZE)
   {
      if (((info_ptr->color_type == PNG_COLOR_TYPE_RGB) ||
          (info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)) &&
          _png_ptr->palette_lookup && info_ptr->bit_depth == 8)
      {
         info_ptr->color_type = PNG_COLOR_TYPE_PALETTE;
      }
   }
#endif

#ifdef PNG_READ_EXPAND_16_SUPPORTED
   if (_png_ptr->transformations & PNG_EXPAND_16 && info_ptr->bit_depth == 8 &&
      info_ptr->color_type != PNG_COLOR_TYPE_PALETTE)
   {
      info_ptr->bit_depth = 16;
   }
#endif

#ifdef PNG_READ_PACK_SUPPORTED
   if ((_png_ptr->transformations & PNG_PACK) && (info_ptr->bit_depth < 8))
      info_ptr->bit_depth = 8;
#endif

   if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      info_ptr->channels = 1;

   else if (info_ptr->color_type & PNG_COLOR_MASK_COLOR)
      info_ptr->channels = 3;

   else
      info_ptr->channels = 1;

#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
   if (_png_ptr->transformations & PNG_STRIP_ALPHA)
   {
      info_ptr->color_type = (_png_byte)(info_ptr->color_type &
         ~PNG_COLOR_MASK_ALPHA);
      info_ptr->num_trans = 0;
   }
#endif

   if (info_ptr->color_type & PNG_COLOR_MASK_ALPHA)
      info_ptr->channels++;

#ifdef PNG_READ_FILLER_SUPPORTED
   /* STRIP_ALPHA and FILLER allowed:  MASK_ALPHA bit stripped above */
   if ((_png_ptr->transformations & PNG_FILLER) &&
       ((info_ptr->color_type == PNG_COLOR_TYPE_RGB) ||
       (info_ptr->color_type == PNG_COLOR_TYPE_GRAY)))
   {
      info_ptr->channels++;
      /* If adding a true alpha channel not just filler */
      if (_png_ptr->transformations & PNG_ADD_ALPHA)
         info_ptr->color_type |= PNG_COLOR_MASK_ALPHA;
   }
#endif

#if defined(PNG_USER_TRANSFORM_PTR_SUPPORTED) && \
defined(PNG_READ_USER_TRANSFORM_SUPPORTED)
   if (_png_ptr->transformations & PNG_USER_TRANSFORM)
   {
      if (info_ptr->bit_depth < _png_ptr->user_transform_depth)
         info_ptr->bit_depth = _png_ptr->user_transform_depth;

      if (info_ptr->channels < _png_ptr->user_transform_channels)
         info_ptr->channels = _png_ptr->user_transform_channels;
   }
#endif

   info_ptr->pixel_depth = (_png_byte)(info_ptr->channels *
       info_ptr->bit_depth);

   info_ptr->rowbytes = PNG_ROWBYTES(info_ptr->pixel_depth, info_ptr->width);

   /* Adding in 1.5.4: cache the above value in _png_struct so that we can later
    * check in _png_rowbytes that the user buffer won't get overwritten.  Note
    * that the field is not always set - if _png_read_update_info isn't called
    * the application has to either not do any transforms or get the calculation
    * right itself.
    */
   _png_ptr->info_rowbytes = info_ptr->rowbytes;

#ifndef PNG_READ_EXPAND_SUPPORTED
   if (_png_ptr)
      return;
#endif
}

/* Transform the row.  The order of transformations is significant,
 * and is very touchy.  If you add a transformation, take care to
 * decide how it fits in with the other transformations here.
 */
void /* PRIVATE */
_png_do_read_transformations(_png_structp _png_ptr, _png_row_infop row_info)
{
   _png_debug(1, "in _png_do_read_transformations");

   if (_png_ptr->row_buf == NULL)
   {
      /* Prior to 1.5.4 this output row/pass where the NULL pointer is, but this
       * error is incredibly rare and incredibly easy to debug without this
       * information.
       */
      _png_error(_png_ptr, "NULL row buffer");
   }

   /* The following is debugging; prior to 1.5.4 the code was never compiled in;
    * in 1.5.4 PNG_FLAG_DETECT_UNINITIALIZED was added and the macro
    * PNG_WARN_UNINITIALIZED_ROW removed.  In 1.5 the new flag is set only for
    * selected new APIs to ensure that there is no API change.
    */
   if ((_png_ptr->flags & PNG_FLAG_DETECT_UNINITIALIZED) != 0 &&
      !(_png_ptr->flags & PNG_FLAG_ROW_INIT))
   {
      /* Application has failed to call either _png_read_start_image() or
       * _png_read_update_info() after setting transforms that expand pixels.
       * This check added to libpng-1.2.19 (but not enabled until 1.5.4).
       */
      _png_error(_png_ptr, "Uninitialized row");
   }

#ifdef PNG_READ_EXPAND_SUPPORTED
   if (_png_ptr->transformations & PNG_EXPAND)
   {
      if (row_info->color_type == PNG_COLOR_TYPE_PALETTE)
      {
         _png_do_expand_palette(row_info, _png_ptr->row_buf + 1,
             _png_ptr->palette, _png_ptr->trans_alpha, _png_ptr->num_trans);
      }

      else
      {
         if (_png_ptr->num_trans &&
             (_png_ptr->transformations & PNG_EXPAND_tRNS))
            _png_do_expand(row_info, _png_ptr->row_buf + 1,
                &(_png_ptr->trans_color));

         else
            _png_do_expand(row_info, _png_ptr->row_buf + 1,
                NULL);
      }
   }
#endif

#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
   if ((_png_ptr->transformations & PNG_STRIP_ALPHA) &&
      !(_png_ptr->transformations & PNG_COMPOSE) &&
      (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA ||
      row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
      _png_do_strip_channel(row_info, _png_ptr->row_buf + 1,
         0 /* at_start == false, because SWAP_ALPHA happens later */);
#endif

#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
   if (_png_ptr->transformations & PNG_RGB_TO_GRAY)
   {
      int rgb_error =
          _png_do_rgb_to_gray(_png_ptr, row_info,
              _png_ptr->row_buf + 1);

      if (rgb_error)
      {
         _png_ptr->rgb_to_gray_status=1;
         if ((_png_ptr->transformations & PNG_RGB_TO_GRAY) ==
             PNG_RGB_TO_GRAY_WARN)
            _png_warning(_png_ptr, "_png_do_rgb_to_gray found nongray pixel");

         if ((_png_ptr->transformations & PNG_RGB_TO_GRAY) ==
             PNG_RGB_TO_GRAY_ERR)
            _png_error(_png_ptr, "_png_do_rgb_to_gray found nongray pixel");
      }
   }
#endif

/* From Andreas Dilger e-mail to png-implement, 26 March 1998:
 *
 *   In most cases, the "simple transparency" should be done prior to doing
 *   gray-to-RGB, or you will have to test 3x as many bytes to check if a
 *   pixel is transparent.  You would also need to make sure that the
 *   transparency information is upgraded to RGB.
 *
 *   To summarize, the current flow is:
 *   - Gray + simple transparency -> compare 1 or 2 gray bytes and composite
 *                                   with background "in place" if transparent,
 *                                   convert to RGB if necessary
 *   - Gray + alpha -> composite with gray background and remove alpha bytes,
 *                                   convert to RGB if necessary
 *
 *   To support RGB backgrounds for gray images we need:
 *   - Gray + simple transparency -> convert to RGB + simple transparency,
 *                                   compare 3 or 6 bytes and composite with
 *                                   background "in place" if transparent
 *                                   (3x compare/pixel compared to doing
 *                                   composite with gray bkgrnd)
 *   - Gray + alpha -> convert to RGB + alpha, composite with background and
 *                                   remove alpha bytes (3x float
 *                                   operations/pixel compared with composite
 *                                   on gray background)
 *
 *  Greg's change will do this.  The reason it wasn't done before is for
 *  performance, as this increases the per-pixel operations.  If we would check
 *  in advance if the background was gray or RGB, and position the gray-to-RGB
 *  transform appropriately, then it would save a lot of work/time.
 */

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
   /* If gray -> RGB, do so now only if background is non-gray; else do later
    * for performance reasons
    */
   if ((_png_ptr->transformations & PNG_GRAY_TO_RGB) &&
       !(_png_ptr->mode & PNG_BACKGROUND_IS_GRAY))
      _png_do_gray_to_rgb(row_info, _png_ptr->row_buf + 1);
#endif

#if (defined PNG_READ_BACKGROUND_SUPPORTED) ||\
   (defined PNG_READ_ALPHA_MODE_SUPPORTED)
   if (_png_ptr->transformations & PNG_COMPOSE)
      _png_do_compose(row_info, _png_ptr->row_buf + 1, _png_ptr);
#endif

#ifdef PNG_READ_GAMMA_SUPPORTED
   if ((_png_ptr->transformations & PNG_GAMMA) &&
#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
      /* Because RGB_TO_GRAY does the gamma transform. */
      !(_png_ptr->transformations & PNG_RGB_TO_GRAY) &&
#endif
#if (defined PNG_READ_BACKGROUND_SUPPORTED) ||\
   (defined PNG_READ_ALPHA_MODE_SUPPORTED)
      /* Because PNG_COMPOSE does the gamma transform if there is something to
       * do (if there is an alpha channel or transparency.)
       */
       !((_png_ptr->transformations & PNG_COMPOSE) &&
       ((_png_ptr->num_trans != 0) ||
       (_png_ptr->color_type & PNG_COLOR_MASK_ALPHA))) &&
#endif
      /* Because _png_init_read_transformations transforms the palette, unless
       * RGB_TO_GRAY will do the transform.
       */
       (_png_ptr->color_type != PNG_COLOR_TYPE_PALETTE))
      _png_do_gamma(row_info, _png_ptr->row_buf + 1, _png_ptr);
#endif

#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
   if ((_png_ptr->transformations & PNG_STRIP_ALPHA) &&
      (_png_ptr->transformations & PNG_COMPOSE) &&
      (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA ||
      row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
      _png_do_strip_channel(row_info, _png_ptr->row_buf + 1,
         0 /* at_start == false, because SWAP_ALPHA happens later */);
#endif

#ifdef PNG_READ_ALPHA_MODE_SUPPORTED
   if ((_png_ptr->transformations & PNG_ENCODE_ALPHA) &&
      (row_info->color_type & PNG_COLOR_MASK_ALPHA))
      _png_do_encode_alpha(row_info, _png_ptr->row_buf + 1, _png_ptr);
#endif

#ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
   if (_png_ptr->transformations & PNG_SCALE_16_TO_8)
      _png_do_scale_16_to_8(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_STRIP_16_TO_8_SUPPORTED
   /* There is no harm in doing both of these because only one has any effect,
    * by putting the 'scale' option first if the app asks for scale (either by
    * calling the API or in a TRANSFORM flag) this is what happens.
    */
   if (_png_ptr->transformations & PNG_16_TO_8)
      _png_do_chop(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_QUANTIZE_SUPPORTED
   if (_png_ptr->transformations & PNG_QUANTIZE)
   {
      _png_do_quantize(row_info, _png_ptr->row_buf + 1,
          _png_ptr->palette_lookup, _png_ptr->quantize_index);

      if (row_info->rowbytes == 0)
         _png_error(_png_ptr, "_png_do_quantize returned rowbytes=0");
   }
#endif /* PNG_READ_QUANTIZE_SUPPORTED */

#ifdef PNG_READ_EXPAND_16_SUPPORTED
   /* Do the expansion now, after all the arithmetic has been done.  Notice
    * that previous transformations can handle the PNG_EXPAND_16 flag if this
    * is efficient (particularly true in the case of gamma correction, where
    * better accuracy results faster!)
    */
   if (_png_ptr->transformations & PNG_EXPAND_16)
      _png_do_expand_16(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
   /* NOTE: moved here in 1.5.4 (from much later in this list.) */
   if ((_png_ptr->transformations & PNG_GRAY_TO_RGB) &&
       (_png_ptr->mode & PNG_BACKGROUND_IS_GRAY))
      _png_do_gray_to_rgb(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_INVERT_SUPPORTED
   if (_png_ptr->transformations & PNG_INVERT_MONO)
      _png_do_invert(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_SHIFT_SUPPORTED
   if (_png_ptr->transformations & PNG_SHIFT)
      _png_do_unshift(row_info, _png_ptr->row_buf + 1,
          &(_png_ptr->shift));
#endif

#ifdef PNG_READ_PACK_SUPPORTED
   if (_png_ptr->transformations & PNG_PACK)
      _png_do_unpack(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_CHECK_FOR_INVALID_INDEX_SUPPORTED
   /* Added at libpng-1.5.10 */
   if (row_info->color_type == PNG_COLOR_TYPE_PALETTE &&
       _png_ptr->num_palette_max >= 0)
      _png_do_check_palette_indexes(_png_ptr, row_info);
#endif

#ifdef PNG_READ_BGR_SUPPORTED
   if (_png_ptr->transformations & PNG_BGR)
      _png_do_bgr(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_PACKSWAP_SUPPORTED
   if (_png_ptr->transformations & PNG_PACKSWAP)
      _png_do_packswap(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_FILLER_SUPPORTED
   if (_png_ptr->transformations & PNG_FILLER)
      _png_do_read_filler(row_info, _png_ptr->row_buf + 1,
          (_png_uint_32)_png_ptr->filler, _png_ptr->flags);
#endif

#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
   if (_png_ptr->transformations & PNG_INVERT_ALPHA)
      _png_do_read_invert_alpha(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_SWAP_ALPHA_SUPPORTED
   if (_png_ptr->transformations & PNG_SWAP_ALPHA)
      _png_do_read_swap_alpha(row_info, _png_ptr->row_buf + 1);
#endif

#ifdef PNG_READ_16BIT_SUPPORTED
#ifdef PNG_READ_SWAP_SUPPORTED
   if (_png_ptr->transformations & PNG_SWAP_BYTES)
      _png_do_swap(row_info, _png_ptr->row_buf + 1);
#endif
#endif

#ifdef PNG_READ_USER_TRANSFORM_SUPPORTED
   if (_png_ptr->transformations & PNG_USER_TRANSFORM)
    {
      if (_png_ptr->read_user_transform_fn != NULL)
         (*(_png_ptr->read_user_transform_fn)) /* User read transform function */
             (_png_ptr,     /* _png_ptr */
             row_info,     /* row_info: */
                /*  _png_uint_32 width;       width of row */
                /*  _png_size_t rowbytes;     number of bytes in row */
                /*  _png_byte color_type;     color type of pixels */
                /*  _png_byte bit_depth;      bit depth of samples */
                /*  _png_byte channels;       number of channels (1-4) */
                /*  _png_byte pixel_depth;    bits per pixel (depth*channels) */
             _png_ptr->row_buf + 1);    /* start of pixel data for row */
#ifdef PNG_USER_TRANSFORM_PTR_SUPPORTED
      if (_png_ptr->user_transform_depth)
         row_info->bit_depth = _png_ptr->user_transform_depth;

      if (_png_ptr->user_transform_channels)
         row_info->channels = _png_ptr->user_transform_channels;
#endif
      row_info->pixel_depth = (_png_byte)(row_info->bit_depth *
          row_info->channels);

      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_info->width);
   }
#endif
}

#ifdef PNG_READ_PACK_SUPPORTED
/* Unpack pixels of 1, 2, or 4 bits per pixel into 1 byte per pixel,
 * without changing the actual values.  Thus, if you had a row with
 * a bit depth of 1, you would end up with bytes that only contained
 * the numbers 0 or 1.  If you would rather they contain 0 and 255, use
 * _png_do_shift() after this.
 */
void /* PRIVATE */
_png_do_unpack(_png_row_infop row_info, _png_bytep row)
{
   _png_debug(1, "in _png_do_unpack");

   if (row_info->bit_depth < 8)
   {
      _png_uint_32 i;
      _png_uint_32 row_width=row_info->width;

      switch (row_info->bit_depth)
      {
         case 1:
         {
            _png_bytep sp = row + (_png_size_t)((row_width - 1) >> 3);
            _png_bytep dp = row + (_png_size_t)row_width - 1;
            _png_uint_32 shift = 7 - (int)((row_width + 7) & 0x07);
            for (i = 0; i < row_width; i++)
            {
               *dp = (_png_byte)((*sp >> shift) & 0x01);

               if (shift == 7)
               {
                  shift = 0;
                  sp--;
               }

               else
                  shift++;

               dp--;
            }
            break;
         }

         case 2:
         {

            _png_bytep sp = row + (_png_size_t)((row_width - 1) >> 2);
            _png_bytep dp = row + (_png_size_t)row_width - 1;
            _png_uint_32 shift = (int)((3 - ((row_width + 3) & 0x03)) << 1);
            for (i = 0; i < row_width; i++)
            {
               *dp = (_png_byte)((*sp >> shift) & 0x03);

               if (shift == 6)
               {
                  shift = 0;
                  sp--;
               }

               else
                  shift += 2;

               dp--;
            }
            break;
         }

         case 4:
         {
            _png_bytep sp = row + (_png_size_t)((row_width - 1) >> 1);
            _png_bytep dp = row + (_png_size_t)row_width - 1;
            _png_uint_32 shift = (int)((1 - ((row_width + 1) & 0x01)) << 2);
            for (i = 0; i < row_width; i++)
            {
               *dp = (_png_byte)((*sp >> shift) & 0x0f);

               if (shift == 4)
               {
                  shift = 0;
                  sp--;
               }

               else
                  shift = 4;

               dp--;
            }
            break;
         }

         default:
            break;
      }
      row_info->bit_depth = 8;
      row_info->pixel_depth = (_png_byte)(8 * row_info->channels);
      row_info->rowbytes = row_width * row_info->channels;
   }
}
#endif

#ifdef PNG_READ_SHIFT_SUPPORTED
/* Reverse the effects of _png_do_shift.  This routine merely shifts the
 * pixels back to their significant bits values.  Thus, if you have
 * a row of bit depth 8, but only 5 are significant, this will shift
 * the values back to 0 through 31.
 */
void /* PRIVATE */
_png_do_unshift(_png_row_infop row_info, _png_bytep row,
    _png_const_color_8p sig_bits)
{
   int color_type;

   _png_debug(1, "in _png_do_unshift");

   /* The palette case has already been handled in the _init routine. */
   color_type = row_info->color_type;

   if (color_type != PNG_COLOR_TYPE_PALETTE)
   {
      int shift[4];
      int channels = 0;
      int bit_depth = row_info->bit_depth;

      if (color_type & PNG_COLOR_MASK_COLOR)
      {
         shift[channels++] = bit_depth - sig_bits->red;
         shift[channels++] = bit_depth - sig_bits->green;
         shift[channels++] = bit_depth - sig_bits->blue;
      }

      else
      {
         shift[channels++] = bit_depth - sig_bits->gray;
      }

      if (color_type & PNG_COLOR_MASK_ALPHA)
      {
         shift[channels++] = bit_depth - sig_bits->alpha;
      }

      {
         int c, have_shift;

         for (c = have_shift = 0; c < channels; ++c)
         {
            /* A shift of more than the bit depth is an error condition but it
             * gets ignored here.
             */
            if (shift[c] <= 0 || shift[c] >= bit_depth)
               shift[c] = 0;

            else
               have_shift = 1;
         }

         if (!have_shift)
            return;
      }

      switch (bit_depth)
      {
         default:
         /* Must be 1bpp gray: should not be here! */
            /* NOTREACHED */
            break;

         case 2:
         /* Must be 2bpp gray */
         /* assert(channels == 1 && shift[0] == 1) */
         {
            _png_bytep bp = row;
            _png_bytep bp_end = bp + row_info->rowbytes;

            while (bp < bp_end)
            {
               int b = (*bp >> 1) & 0x55;
               *bp++ = (_png_byte)b;
            }
            break;
         }

         case 4:
         /* Must be 4bpp gray */
         /* assert(channels == 1) */
         {
            _png_bytep bp = row;
            _png_bytep bp_end = bp + row_info->rowbytes;
            int gray_shift = shift[0];
            int mask =  0xf >> gray_shift;

            mask |= mask << 4;

            while (bp < bp_end)
            {
               int b = (*bp >> gray_shift) & mask;
               *bp++ = (_png_byte)b;
            }
            break;
         }

         case 8:
         /* Single byte components, G, GA, RGB, RGBA */
         {
            _png_bytep bp = row;
            _png_bytep bp_end = bp + row_info->rowbytes;
            int channel = 0;

            while (bp < bp_end)
            {
               int b = *bp >> shift[channel];
               if (++channel >= channels)
                  channel = 0;
               *bp++ = (_png_byte)b;
            }
            break;
         }

#ifdef PNG_READ_16BIT_SUPPORTED
         case 16:
         /* Double byte components, G, GA, RGB, RGBA */
         {
            _png_bytep bp = row;
            _png_bytep bp_end = bp + row_info->rowbytes;
            int channel = 0;

            while (bp < bp_end)
            {
               int value = (bp[0] << 8) + bp[1];

               value >>= shift[channel];
               if (++channel >= channels)
                  channel = 0;
               *bp++ = (_png_byte)(value >> 8);
               *bp++ = (_png_byte)(value & 0xff);
            }
            break;
         }
#endif
      }
   }
}
#endif

#ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
/* Scale rows of bit depth 16 down to 8 accurately */
void /* PRIVATE */
_png_do_scale_16_to_8(_png_row_infop row_info, _png_bytep row)
{
   _png_debug(1, "in _png_do_scale_16_to_8");

   if (row_info->bit_depth == 16)
   {
      _png_bytep sp = row; /* source */
      _png_bytep dp = row; /* destination */
      _png_bytep ep = sp + row_info->rowbytes; /* end+1 */

      while (sp < ep)
      {
         /* The input is an array of 16 bit components, these must be scaled to
          * 8 bits each.  For a 16 bit value V the required value (from the PNG
          * specification) is:
          *
          *    (V * 255) / 65535
          *
          * This reduces to round(V / 257), or floor((V + 128.5)/257)
          *
          * Represent V as the two byte value vhi.vlo.  Make a guess that the
          * result is the top byte of V, vhi, then the correction to this value
          * is:
          *
          *    error = floor(((V-vhi.vhi) + 128.5) / 257)
          *          = floor(((vlo-vhi) + 128.5) / 257)
          *
          * This can be approximated using integer arithmetic (and a signed
          * shift):
          *
          *    error = (vlo-vhi+128) >> 8;
          *
          * The approximate differs from the exact answer only when (vlo-vhi) is
          * 128; it then gives a correction of +1 when the exact correction is
          * 0.  This gives 128 errors.  The exact answer (correct for all 16 bit
          * input values) is:
          *
          *    error = (vlo-vhi+128)*65535 >> 24;
          *
          * An alternative arithmetic calculation which also gives no errors is:
          *
          *    (V * 255 + 32895) >> 16
          */

         _png_int_32 tmp = *sp++; /* must be signed! */
         tmp += (((int)*sp++ - tmp + 128) * 65535) >> 24;
         *dp++ = (_png_byte)tmp;
      }

      row_info->bit_depth = 8;
      row_info->pixel_depth = (_png_byte)(8 * row_info->channels);
      row_info->rowbytes = row_info->width * row_info->channels;
   }
}
#endif

#ifdef PNG_READ_STRIP_16_TO_8_SUPPORTED
void /* PRIVATE */
/* Simply discard the low byte.  This was the default behavior prior
 * to libpng-1.5.4.
 */
_png_do_chop(_png_row_infop row_info, _png_bytep row)
{
   _png_debug(1, "in _png_do_chop");

   if (row_info->bit_depth == 16)
   {
      _png_bytep sp = row; /* source */
      _png_bytep dp = row; /* destination */
      _png_bytep ep = sp + row_info->rowbytes; /* end+1 */

      while (sp < ep)
      {
         *dp++ = *sp;
         sp += 2; /* skip low byte */
      }

      row_info->bit_depth = 8;
      row_info->pixel_depth = (_png_byte)(8 * row_info->channels);
      row_info->rowbytes = row_info->width * row_info->channels;
   }
}
#endif

#ifdef PNG_READ_SWAP_ALPHA_SUPPORTED
void /* PRIVATE */
_png_do_read_swap_alpha(_png_row_infop row_info, _png_bytep row)
{
   _png_debug(1, "in _png_do_read_swap_alpha");

   {
      _png_uint_32 row_width = row_info->width;
      if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
      {
         /* This converts from RGBA to ARGB */
         if (row_info->bit_depth == 8)
         {
            _png_bytep sp = row + row_info->rowbytes;
            _png_bytep dp = sp;
            _png_byte save;
            _png_uint_32 i;

            for (i = 0; i < row_width; i++)
            {
               save = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = save;
            }
         }

#ifdef PNG_READ_16BIT_SUPPORTED
         /* This converts from RRGGBBAA to AARRGGBB */
         else
         {
            _png_bytep sp = row + row_info->rowbytes;
            _png_bytep dp = sp;
            _png_byte save[2];
            _png_uint_32 i;

            for (i = 0; i < row_width; i++)
            {
               save[0] = *(--sp);
               save[1] = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = save[0];
               *(--dp) = save[1];
            }
         }
#endif
      }

      else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      {
         /* This converts from GA to AG */
         if (row_info->bit_depth == 8)
         {
            _png_bytep sp = row + row_info->rowbytes;
            _png_bytep dp = sp;
            _png_byte save;
            _png_uint_32 i;

            for (i = 0; i < row_width; i++)
            {
               save = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = save;
            }
         }

#ifdef PNG_READ_16BIT_SUPPORTED
         /* This converts from GGAA to AAGG */
         else
         {
            _png_bytep sp = row + row_info->rowbytes;
            _png_bytep dp = sp;
            _png_byte save[2];
            _png_uint_32 i;

            for (i = 0; i < row_width; i++)
            {
               save[0] = *(--sp);
               save[1] = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = save[0];
               *(--dp) = save[1];
            }
         }
#endif
      }
   }
}
#endif

#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
void /* PRIVATE */
_png_do_read_invert_alpha(_png_row_infop row_info, _png_bytep row)
{
   _png_uint_32 row_width;
   _png_debug(1, "in _png_do_read_invert_alpha");

   row_width = row_info->width;
   if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
   {
      if (row_info->bit_depth == 8)
      {
         /* This inverts the alpha channel in RGBA */
         _png_bytep sp = row + row_info->rowbytes;
         _png_bytep dp = sp;
         _png_uint_32 i;

         for (i = 0; i < row_width; i++)
         {
            *(--dp) = (_png_byte)(255 - *(--sp));

/*          This does nothing:
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            We can replace it with:
*/
            sp-=3;
            dp=sp;
         }
      }

#ifdef PNG_READ_16BIT_SUPPORTED
      /* This inverts the alpha channel in RRGGBBAA */
      else
      {
         _png_bytep sp = row + row_info->rowbytes;
         _png_bytep dp = sp;
         _png_uint_32 i;

         for (i = 0; i < row_width; i++)
         {
            *(--dp) = (_png_byte)(255 - *(--sp));
            *(--dp) = (_png_byte)(255 - *(--sp));

/*          This does nothing:
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
            We can replace it with:
*/
            sp-=6;
            dp=sp;
         }
      }
#endif
   }
   else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
   {
      if (row_info->bit_depth == 8)
      {
         /* This inverts the alpha channel in GA */
         _png_bytep sp = row + row_info->rowbytes;
         _png_bytep dp = sp;
         _png_uint_32 i;

         for (i = 0; i < row_width; i++)
         {
            *(--dp) = (_png_byte)(255 - *(--sp));
            *(--dp) = *(--sp);
         }
      }

#ifdef PNG_READ_16BIT_SUPPORTED
      else
      {
         /* This inverts the alpha channel in GGAA */
         _png_bytep sp  = row + row_info->rowbytes;
         _png_bytep dp = sp;
         _png_uint_32 i;

         for (i = 0; i < row_width; i++)
         {
            *(--dp) = (_png_byte)(255 - *(--sp));
            *(--dp) = (_png_byte)(255 - *(--sp));
/*
            *(--dp) = *(--sp);
            *(--dp) = *(--sp);
*/
            sp-=2;
            dp=sp;
         }
      }
#endif
   }
}
#endif

#ifdef PNG_READ_FILLER_SUPPORTED
/* Add filler channel if we have RGB color */
void /* PRIVATE */
_png_do_read_filler(_png_row_infop row_info, _png_bytep row,
    _png_uint_32 filler, _png_uint_32 flags)
{
   _png_uint_32 i;
   _png_uint_32 row_width = row_info->width;

#ifdef PNG_READ_16BIT_SUPPORTED
   _png_byte hi_filler = (_png_byte)((filler>>8) & 0xff);
#endif
   _png_byte lo_filler = (_png_byte)(filler & 0xff);

   _png_debug(1, "in _png_do_read_filler");

   if (
       row_info->color_type == PNG_COLOR_TYPE_GRAY)
   {
      if (row_info->bit_depth == 8)
      {
         if (flags & PNG_FLAG_FILLER_AFTER)
         {
            /* This changes the data from G to GX */
            _png_bytep sp = row + (_png_size_t)row_width;
            _png_bytep dp =  sp + (_png_size_t)row_width;
            for (i = 1; i < row_width; i++)
            {
               *(--dp) = lo_filler;
               *(--dp) = *(--sp);
            }
            *(--dp) = lo_filler;
            row_info->channels = 2;
            row_info->pixel_depth = 16;
            row_info->rowbytes = row_width * 2;
         }

         else
         {
            /* This changes the data from G to XG */
            _png_bytep sp = row + (_png_size_t)row_width;
            _png_bytep dp = sp  + (_png_size_t)row_width;
            for (i = 0; i < row_width; i++)
            {
               *(--dp) = *(--sp);
               *(--dp) = lo_filler;
            }
            row_info->channels = 2;
            row_info->pixel_depth = 16;
            row_info->rowbytes = row_width * 2;
         }
      }

#ifdef PNG_READ_16BIT_SUPPORTED
      else if (row_info->bit_depth == 16)
      {
         if (flags & PNG_FLAG_FILLER_AFTER)
         {
            /* This changes the data from GG to GGXX */
            _png_bytep sp = row + (_png_size_t)row_width * 2;
            _png_bytep dp = sp  + (_png_size_t)row_width * 2;
            for (i = 1; i < row_width; i++)
            {
               *(--dp) = hi_filler;
               *(--dp) = lo_filler;
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
            }
            *(--dp) = hi_filler;
            *(--dp) = lo_filler;
            row_info->channels = 2;
            row_info->pixel_depth = 32;
            row_info->rowbytes = row_width * 4;
         }

         else
         {
            /* This changes the data from GG to XXGG */
            _png_bytep sp = row + (_png_size_t)row_width * 2;
            _png_bytep dp = sp  + (_png_size_t)row_width * 2;
            for (i = 0; i < row_width; i++)
            {
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = hi_filler;
               *(--dp) = lo_filler;
            }
            row_info->channels = 2;
            row_info->pixel_depth = 32;
            row_info->rowbytes = row_width * 4;
         }
      }
#endif
   } /* COLOR_TYPE == GRAY */
   else if (row_info->color_type == PNG_COLOR_TYPE_RGB)
   {
      if (row_info->bit_depth == 8)
      {
         if (flags & PNG_FLAG_FILLER_AFTER)
         {
            /* This changes the data from RGB to RGBX */
            _png_bytep sp = row + (_png_size_t)row_width * 3;
            _png_bytep dp = sp  + (_png_size_t)row_width;
            for (i = 1; i < row_width; i++)
            {
               *(--dp) = lo_filler;
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
            }
            *(--dp) = lo_filler;
            row_info->channels = 4;
            row_info->pixel_depth = 32;
            row_info->rowbytes = row_width * 4;
         }

         else
         {
            /* This changes the data from RGB to XRGB */
            _png_bytep sp = row + (_png_size_t)row_width * 3;
            _png_bytep dp = sp + (_png_size_t)row_width;
            for (i = 0; i < row_width; i++)
            {
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = lo_filler;
            }
            row_info->channels = 4;
            row_info->pixel_depth = 32;
            row_info->rowbytes = row_width * 4;
         }
      }

#ifdef PNG_READ_16BIT_SUPPORTED
      else if (row_info->bit_depth == 16)
      {
         if (flags & PNG_FLAG_FILLER_AFTER)
         {
            /* This changes the data from RRGGBB to RRGGBBXX */
            _png_bytep sp = row + (_png_size_t)row_width * 6;
            _png_bytep dp = sp  + (_png_size_t)row_width * 2;
            for (i = 1; i < row_width; i++)
            {
               *(--dp) = hi_filler;
               *(--dp) = lo_filler;
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
            }
            *(--dp) = hi_filler;
            *(--dp) = lo_filler;
            row_info->channels = 4;
            row_info->pixel_depth = 64;
            row_info->rowbytes = row_width * 8;
         }

         else
         {
            /* This changes the data from RRGGBB to XXRRGGBB */
            _png_bytep sp = row + (_png_size_t)row_width * 6;
            _png_bytep dp = sp  + (_png_size_t)row_width * 2;
            for (i = 0; i < row_width; i++)
            {
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = *(--sp);
               *(--dp) = hi_filler;
               *(--dp) = lo_filler;
            }

            row_info->channels = 4;
            row_info->pixel_depth = 64;
            row_info->rowbytes = row_width * 8;
         }
      }
#endif
   } /* COLOR_TYPE == RGB */
}
#endif

#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
/* Expand grayscale files to RGB, with or without alpha */
void /* PRIVATE */
_png_do_gray_to_rgb(_png_row_infop row_info, _png_bytep row)
{
   _png_uint_32 i;
   _png_uint_32 row_width = row_info->width;

   _png_debug(1, "in _png_do_gray_to_rgb");

   if (row_info->bit_depth >= 8 &&
       !(row_info->color_type & PNG_COLOR_MASK_COLOR))
   {
      if (row_info->color_type == PNG_COLOR_TYPE_GRAY)
      {
         if (row_info->bit_depth == 8)
         {
            /* This changes G to RGB */
            _png_bytep sp = row + (_png_size_t)row_width - 1;
            _png_bytep dp = sp  + (_png_size_t)row_width * 2;
            for (i = 0; i < row_width; i++)
            {
               *(dp--) = *sp;
               *(dp--) = *sp;
               *(dp--) = *(sp--);
            }
         }

         else
         {
            /* This changes GG to RRGGBB */
            _png_bytep sp = row + (_png_size_t)row_width * 2 - 1;
            _png_bytep dp = sp  + (_png_size_t)row_width * 4;
            for (i = 0; i < row_width; i++)
            {
               *(dp--) = *sp;
               *(dp--) = *(sp - 1);
               *(dp--) = *sp;
               *(dp--) = *(sp - 1);
               *(dp--) = *(sp--);
               *(dp--) = *(sp--);
            }
         }
      }

      else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      {
         if (row_info->bit_depth == 8)
         {
            /* This changes GA to RGBA */
            _png_bytep sp = row + (_png_size_t)row_width * 2 - 1;
            _png_bytep dp = sp  + (_png_size_t)row_width * 2;
            for (i = 0; i < row_width; i++)
            {
               *(dp--) = *(sp--);
               *(dp--) = *sp;
               *(dp--) = *sp;
               *(dp--) = *(sp--);
            }
         }

         else
         {
            /* This changes GGAA to RRGGBBAA */
            _png_bytep sp = row + (_png_size_t)row_width * 4 - 1;
            _png_bytep dp = sp  + (_png_size_t)row_width * 4;
            for (i = 0; i < row_width; i++)
            {
               *(dp--) = *(sp--);
               *(dp--) = *(sp--);
               *(dp--) = *sp;
               *(dp--) = *(sp - 1);
               *(dp--) = *sp;
               *(dp--) = *(sp - 1);
               *(dp--) = *(sp--);
               *(dp--) = *(sp--);
            }
         }
      }
      row_info->channels = (_png_byte)(row_info->channels + 2);
      row_info->color_type |= PNG_COLOR_MASK_COLOR;
      row_info->pixel_depth = (_png_byte)(row_info->channels *
          row_info->bit_depth);
      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
   }
}
#endif

#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
/* Reduce RGB files to grayscale, with or without alpha
 * using the equation given in Poynton's ColorFAQ of 1998-01-04 at
 * <http://www.inforamp.net/~poynton/>  (THIS LINK IS DEAD June 2008 but
 * versions dated 1998 through November 2002 have been archived at
 * http://web.archive.org/web/20000816232553/http://www.inforamp.net/
 * ~poynton/notes/colour_and_gamma/ColorFAQ.txt )
 * Charles Poynton poynton at poynton.com
 *
 *     Y = 0.212671 * R + 0.715160 * G + 0.072169 * B
 *
 *  which can be expressed with integers as
 *
 *     Y = (6969 * R + 23434 * G + 2365 * B)/32768
 *
 * Poynton's current link (as of January 2003 through July 2011):
 * <http://www.poynton.com/notes/colour_and_gamma/>
 * has changed the numbers slightly:
 *
 *     Y = 0.2126*R + 0.7152*G + 0.0722*B
 *
 *  which can be expressed with integers as
 *
 *     Y = (6966 * R + 23436 * G + 2366 * B)/32768
 *
 *  Historically, however, libpng uses numbers derived from the ITU-R Rec 709
 *  end point chromaticities and the D65 white point.  Depending on the
 *  precision used for the D65 white point this produces a variety of different
 *  numbers, however if the four decimal place value used in ITU-R Rec 709 is
 *  used (0.3127,0.3290) the Y calculation would be:
 *
 *     Y = (6968 * R + 23435 * G + 2366 * B)/32768
 *
 *  While this is correct the rounding results in an overflow for white, because
 *  the sum of the rounded coefficients is 32769, not 32768.  Consequently
 *  libpng uses, instead, the closest non-overflowing approximation:
 *
 *     Y = (6968 * R + 23434 * G + 2366 * B)/32768
 *
 *  Starting with libpng-1.5.5, if the image being converted has a cHRM chunk
 *  (including an sRGB chunk) then the chromaticities are used to calculate the
 *  coefficients.  See the chunk handling in pngrutil.c for more information.
 *
 *  In all cases the calculation is to be done in a linear colorspace.  If no
 *  gamma information is available to correct the encoding of the original RGB
 *  values this results in an implicit assumption that the original PNG RGB
 *  values were linear.
 *
 *  Other integer coefficents can be used via _png_set_rgb_to_gray().  Because
 *  the API takes just red and green coefficients the blue coefficient is
 *  calculated to make the sum 32768.  This will result in different rounding
 *  to that used above.
 */
int /* PRIVATE */
_png_do_rgb_to_gray(_png_structp _png_ptr, _png_row_infop row_info, _png_bytep row)

{
   int rgb_error = 0;

   _png_debug(1, "in _png_do_rgb_to_gray");

   if (!(row_info->color_type & PNG_COLOR_MASK_PALETTE) &&
       (row_info->color_type & PNG_COLOR_MASK_COLOR))
   {
      PNG_CONST _png_uint_32 rc = _png_ptr->rgb_to_gray_red_coeff;
      PNG_CONST _png_uint_32 gc = _png_ptr->rgb_to_gray_green_coeff;
      PNG_CONST _png_uint_32 bc = 32768 - rc - gc;
      PNG_CONST _png_uint_32 row_width = row_info->width;
      PNG_CONST int have_alpha =
         (row_info->color_type & PNG_COLOR_MASK_ALPHA) != 0;

      if (row_info->bit_depth == 8)
      {
#if defined(PNG_READ_GAMMA_SUPPORTED) || defined(PNG_READ_BACKGROUND_SUPPORTED)
         /* Notice that gamma to/from 1 are not necessarily inverses (if
          * there is an overall gamma correction).  Prior to 1.5.5 this code
          * checked the linearized values for equality; this doesn't match
          * the documentation, the original values must be checked.
          */
         if (_png_ptr->gamma_from_1 != NULL && _png_ptr->gamma_to_1 != NULL)
         {
            _png_bytep sp = row;
            _png_bytep dp = row;
            _png_uint_32 i;

            for (i = 0; i < row_width; i++)
            {
               _png_byte red   = *(sp++);
               _png_byte green = *(sp++);
               _png_byte blue  = *(sp++);

               if (red != green || red != blue)
               {
                  red = _png_ptr->gamma_to_1[red];
                  green = _png_ptr->gamma_to_1[green];
                  blue = _png_ptr->gamma_to_1[blue];

                  rgb_error |= 1;
                  *(dp++) = _png_ptr->gamma_from_1[
                      (rc*red + gc*green + bc*blue + 16384)>>15];
               }

               else
               {
                  /* If there is no overall correction the table will not be
                   * set.
                   */
                  if (_png_ptr->gamma_table != NULL)
                     red = _png_ptr->gamma_table[red];

                  *(dp++) = red;
               }

               if (have_alpha)
                  *(dp++) = *(sp++);
            }
         }
         else
#endif
         {
            _png_bytep sp = row;
            _png_bytep dp = row;
            _png_uint_32 i;

            for (i = 0; i < row_width; i++)
            {
               _png_byte red   = *(sp++);
               _png_byte green = *(sp++);
               _png_byte blue  = *(sp++);

               if (red != green || red != blue)
               {
                  rgb_error |= 1;
                  /* NOTE: this is the historical approach which simply
                   * truncates the results.
                   */
                  *(dp++) = (_png_byte)((rc*red + gc*green + bc*blue)>>15);
               }

               else
                  *(dp++) = red;

               if (have_alpha)
                  *(dp++) = *(sp++);
            }
         }
      }

      else /* RGB bit_depth == 16 */
      {
#if defined(PNG_READ_GAMMA_SUPPORTED) || defined(PNG_READ_BACKGROUND_SUPPORTED)
         if (_png_ptr->gamma_16_to_1 != NULL && _png_ptr->gamma_16_from_1 != NULL)
         {
            _png_bytep sp = row;
            _png_bytep dp = row;
            _png_uint_32 i;

            for (i = 0; i < row_width; i++)
            {
               _png_uint_16 red, green, blue, w;

               red   = (_png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;
               green = (_png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;
               blue  = (_png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;

               if (red == green && red == blue)
               {
                  if (_png_ptr->gamma_16_table != NULL)
                     w = _png_ptr->gamma_16_table[(red&0xff)
                         >> _png_ptr->gamma_shift][red>>8];

                  else
                     w = red;
               }

               else
               {
                  _png_uint_16 red_1   = _png_ptr->gamma_16_to_1[(red&0xff)
                      >> _png_ptr->gamma_shift][red>>8];
                  _png_uint_16 green_1 =
                      _png_ptr->gamma_16_to_1[(green&0xff) >>
                      _png_ptr->gamma_shift][green>>8];
                  _png_uint_16 blue_1  = _png_ptr->gamma_16_to_1[(blue&0xff)
                      >> _png_ptr->gamma_shift][blue>>8];
                  _png_uint_16 gray16  = (_png_uint_16)((rc*red_1 + gc*green_1
                      + bc*blue_1 + 16384)>>15);
                  w = _png_ptr->gamma_16_from_1[(gray16&0xff) >>
                      _png_ptr->gamma_shift][gray16 >> 8];
                  rgb_error |= 1;
               }

               *(dp++) = (_png_byte)((w>>8) & 0xff);
               *(dp++) = (_png_byte)(w & 0xff);

               if (have_alpha)
               {
                  *(dp++) = *(sp++);
                  *(dp++) = *(sp++);
               }
            }
         }
         else
#endif
         {
            _png_bytep sp = row;
            _png_bytep dp = row;
            _png_uint_32 i;

            for (i = 0; i < row_width; i++)
            {
               _png_uint_16 red, green, blue, gray16;

               red   = (_png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;
               green = (_png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;
               blue  = (_png_uint_16)(((*(sp))<<8) | *(sp + 1)); sp += 2;

               if (red != green || red != blue)
                  rgb_error |= 1;

               /* From 1.5.5 in the 16 bit case do the accurate conversion even
                * in the 'fast' case - this is because this is where the code
                * ends up when handling linear 16 bit data.
                */
               gray16  = (_png_uint_16)((rc*red + gc*green + bc*blue + 16384) >>
                  15);
               *(dp++) = (_png_byte)((gray16>>8) & 0xff);
               *(dp++) = (_png_byte)(gray16 & 0xff);

               if (have_alpha)
               {
                  *(dp++) = *(sp++);
                  *(dp++) = *(sp++);
               }
            }
         }
      }

      row_info->channels = (_png_byte)(row_info->channels - 2);
      row_info->color_type = (_png_byte)(row_info->color_type &
          ~PNG_COLOR_MASK_COLOR);
      row_info->pixel_depth = (_png_byte)(row_info->channels *
          row_info->bit_depth);
      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
   }
   return rgb_error;
}
#endif
#endif /* PNG_READ_TRANSFORMS_SUPPORTED */

#ifdef PNG_BUILD_GRAYSCALE_PALETTE_SUPPORTED
/* Build a grayscale palette.  Palette is assumed to be 1 << bit_depth
 * large of _png_color.  This lets grayscale images be treated as
 * paletted.  Most useful for gamma correction and simplification
 * of code.  This API is not used internally.
 */
void PNGAPI
_png_build_grayscale_palette(int bit_depth, _png_colorp palette)
{
   int num_palette;
   int color_inc;
   int i;
   int v;

   _png_debug(1, "in _png_do_build_grayscale_palette");

   if (palette == NULL)
      return;

   switch (bit_depth)
   {
      case 1:
         num_palette = 2;
         color_inc = 0xff;
         break;

      case 2:
         num_palette = 4;
         color_inc = 0x55;
         break;

      case 4:
         num_palette = 16;
         color_inc = 0x11;
         break;

      case 8:
         num_palette = 256;
         color_inc = 1;
         break;

      default:
         num_palette = 0;
         color_inc = 0;
         break;
   }

   for (i = 0, v = 0; i < num_palette; i++, v += color_inc)
   {
      palette[i].red = (_png_byte)v;
      palette[i].green = (_png_byte)v;
      palette[i].blue = (_png_byte)v;
   }
}
#endif


#ifdef PNG_READ_TRANSFORMS_SUPPORTED
#if (defined PNG_READ_BACKGROUND_SUPPORTED) ||\
   (defined PNG_READ_ALPHA_MODE_SUPPORTED)
/* Replace any alpha or transparency with the supplied background color.
 * "background" is already in the screen gamma, while "background_1" is
 * at a gamma of 1.0.  Paletted files have already been taken care of.
 */
void /* PRIVATE */
_png_do_compose(_png_row_infop row_info, _png_bytep row, _png_structp _png_ptr)
{
#ifdef PNG_READ_GAMMA_SUPPORTED
   _png_const_bytep gamma_table = _png_ptr->gamma_table;
   _png_const_bytep gamma_from_1 = _png_ptr->gamma_from_1;
   _png_const_bytep gamma_to_1 = _png_ptr->gamma_to_1;
   _png_const_uint_16pp gamma_16 = _png_ptr->gamma_16_table;
   _png_const_uint_16pp gamma_16_from_1 = _png_ptr->gamma_16_from_1;
   _png_const_uint_16pp gamma_16_to_1 = _png_ptr->gamma_16_to_1;
   int gamma_shift = _png_ptr->gamma_shift;
#endif

   _png_bytep sp;
   _png_uint_32 i;
   _png_uint_32 row_width = row_info->width;
   int optimize = (_png_ptr->flags & PNG_FLAG_OPTIMIZE_ALPHA) != 0;
   int shift;

   _png_debug(1, "in _png_do_compose");

   {
      switch (row_info->color_type)
      {
         case PNG_COLOR_TYPE_GRAY:
         {
            switch (row_info->bit_depth)
            {
               case 1:
               {
                  sp = row;
                  shift = 7;
                  for (i = 0; i < row_width; i++)
                  {
                     if ((_png_uint_16)((*sp >> shift) & 0x01)
                        == _png_ptr->trans_color.gray)
                     {
                        *sp &= (_png_byte)((0x7f7f >> (7 - shift)) & 0xff);
                        *sp |= (_png_byte)(_png_ptr->background.gray << shift);
                     }

                     if (!shift)
                     {
                        shift = 7;
                        sp++;
                     }

                     else
                        shift--;
                  }
                  break;
               }

               case 2:
               {
#ifdef PNG_READ_GAMMA_SUPPORTED
                  if (gamma_table != NULL)
                  {
                     sp = row;
                     shift = 6;
                     for (i = 0; i < row_width; i++)
                     {
                        if ((_png_uint_16)((*sp >> shift) & 0x03)
                            == _png_ptr->trans_color.gray)
                        {
                           *sp &= (_png_byte)((0x3f3f >> (6 - shift)) & 0xff);
                           *sp |= (_png_byte)(_png_ptr->background.gray << shift);
                        }

                        else
                        {
                           _png_byte p = (_png_byte)((*sp >> shift) & 0x03);
                           _png_byte g = (_png_byte)((gamma_table [p | (p << 2) |
                               (p << 4) | (p << 6)] >> 6) & 0x03);
                           *sp &= (_png_byte)((0x3f3f >> (6 - shift)) & 0xff);
                           *sp |= (_png_byte)(g << shift);
                        }

                        if (!shift)
                        {
                           shift = 6;
                           sp++;
                        }

                        else
                           shift -= 2;
                     }
                  }

                  else
#endif
                  {
                     sp = row;
                     shift = 6;
                     for (i = 0; i < row_width; i++)
                     {
                        if ((_png_uint_16)((*sp >> shift) & 0x03)
                            == _png_ptr->trans_color.gray)
                        {
                           *sp &= (_png_byte)((0x3f3f >> (6 - shift)) & 0xff);
                           *sp |= (_png_byte)(_png_ptr->background.gray << shift);
                        }

                        if (!shift)
                        {
                           shift = 6;
                           sp++;
                        }

                        else
                           shift -= 2;
                     }
                  }
                  break;
               }

               case 4:
               {
#ifdef PNG_READ_GAMMA_SUPPORTED
                  if (gamma_table != NULL)
                  {
                     sp = row;
                     shift = 4;
                     for (i = 0; i < row_width; i++)
                     {
                        if ((_png_uint_16)((*sp >> shift) & 0x0f)
                            == _png_ptr->trans_color.gray)
                        {
                           *sp &= (_png_byte)((0xf0f >> (4 - shift)) & 0xff);
                           *sp |= (_png_byte)(_png_ptr->background.gray << shift);
                        }

                        else
                        {
                           _png_byte p = (_png_byte)((*sp >> shift) & 0x0f);
                           _png_byte g = (_png_byte)((gamma_table[p |
                               (p << 4)] >> 4) & 0x0f);
                           *sp &= (_png_byte)((0xf0f >> (4 - shift)) & 0xff);
                           *sp |= (_png_byte)(g << shift);
                        }

                        if (!shift)
                        {
                           shift = 4;
                           sp++;
                        }

                        else
                           shift -= 4;
                     }
                  }

                  else
#endif
                  {
                     sp = row;
                     shift = 4;
                     for (i = 0; i < row_width; i++)
                     {
                        if ((_png_uint_16)((*sp >> shift) & 0x0f)
                            == _png_ptr->trans_color.gray)
                        {
                           *sp &= (_png_byte)((0xf0f >> (4 - shift)) & 0xff);
                           *sp |= (_png_byte)(_png_ptr->background.gray << shift);
                        }

                        if (!shift)
                        {
                           shift = 4;
                           sp++;
                        }

                        else
                           shift -= 4;
                     }
                  }
                  break;
               }

               case 8:
               {
#ifdef PNG_READ_GAMMA_SUPPORTED
                  if (gamma_table != NULL)
                  {
                     sp = row;
                     for (i = 0; i < row_width; i++, sp++)
                     {
                        if (*sp == _png_ptr->trans_color.gray)
                           *sp = (_png_byte)_png_ptr->background.gray;

                        else
                           *sp = gamma_table[*sp];
                     }
                  }
                  else
#endif
                  {
                     sp = row;
                     for (i = 0; i < row_width; i++, sp++)
                     {
                        if (*sp == _png_ptr->trans_color.gray)
                           *sp = (_png_byte)_png_ptr->background.gray;
                     }
                  }
                  break;
               }

               case 16:
               {
#ifdef PNG_READ_GAMMA_SUPPORTED
                  if (gamma_16 != NULL)
                  {
                     sp = row;
                     for (i = 0; i < row_width; i++, sp += 2)
                     {
                        _png_uint_16 v;

                        v = (_png_uint_16)(((*sp) << 8) + *(sp + 1));

                        if (v == _png_ptr->trans_color.gray)
                        {
                           /* Background is already in screen gamma */
                           *sp = (_png_byte)((_png_ptr->background.gray >> 8)
                                & 0xff);
                           *(sp + 1) = (_png_byte)(_png_ptr->background.gray
                                & 0xff);
                        }

                        else
                        {
                           v = gamma_16[*(sp + 1) >> gamma_shift][*sp];
                           *sp = (_png_byte)((v >> 8) & 0xff);
                           *(sp + 1) = (_png_byte)(v & 0xff);
                        }
                     }
                  }
                  else
#endif
                  {
                     sp = row;
                     for (i = 0; i < row_width; i++, sp += 2)
                     {
                        _png_uint_16 v;

                        v = (_png_uint_16)(((*sp) << 8) + *(sp + 1));

                        if (v == _png_ptr->trans_color.gray)
                        {
                           *sp = (_png_byte)((_png_ptr->background.gray >> 8)
                                & 0xff);
                           *(sp + 1) = (_png_byte)(_png_ptr->background.gray
                                & 0xff);
                        }
                     }
                  }
                  break;
               }

               default:
                  break;
            }
            break;
         }

         case PNG_COLOR_TYPE_RGB:
         {
            if (row_info->bit_depth == 8)
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_table != NULL)
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 3)
                  {
                     if (*sp == _png_ptr->trans_color.red &&
                         *(sp + 1) == _png_ptr->trans_color.green &&
                         *(sp + 2) == _png_ptr->trans_color.blue)
                     {
                        *sp = (_png_byte)_png_ptr->background.red;
                        *(sp + 1) = (_png_byte)_png_ptr->background.green;
                        *(sp + 2) = (_png_byte)_png_ptr->background.blue;
                     }

                     else
                     {
                        *sp = gamma_table[*sp];
                        *(sp + 1) = gamma_table[*(sp + 1)];
                        *(sp + 2) = gamma_table[*(sp + 2)];
                     }
                  }
               }
               else
#endif
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 3)
                  {
                     if (*sp == _png_ptr->trans_color.red &&
                         *(sp + 1) == _png_ptr->trans_color.green &&
                         *(sp + 2) == _png_ptr->trans_color.blue)
                     {
                        *sp = (_png_byte)_png_ptr->background.red;
                        *(sp + 1) = (_png_byte)_png_ptr->background.green;
                        *(sp + 2) = (_png_byte)_png_ptr->background.blue;
                     }
                  }
               }
            }
            else /* if (row_info->bit_depth == 16) */
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_16 != NULL)
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 6)
                  {
                     _png_uint_16 r = (_png_uint_16)(((*sp) << 8) + *(sp + 1));

                     _png_uint_16 g = (_png_uint_16)(((*(sp + 2)) << 8)
                         + *(sp + 3));

                     _png_uint_16 b = (_png_uint_16)(((*(sp + 4)) << 8)
                         + *(sp + 5));

                     if (r == _png_ptr->trans_color.red &&
                         g == _png_ptr->trans_color.green &&
                         b == _png_ptr->trans_color.blue)
                     {
                        /* Background is already in screen gamma */
                        *sp = (_png_byte)((_png_ptr->background.red >> 8) & 0xff);
                        *(sp + 1) = (_png_byte)(_png_ptr->background.red & 0xff);
                        *(sp + 2) = (_png_byte)((_png_ptr->background.green >> 8)
                                & 0xff);
                        *(sp + 3) = (_png_byte)(_png_ptr->background.green
                                & 0xff);
                        *(sp + 4) = (_png_byte)((_png_ptr->background.blue >> 8)
                                & 0xff);
                        *(sp + 5) = (_png_byte)(_png_ptr->background.blue & 0xff);
                     }

                     else
                     {
                        _png_uint_16 v = gamma_16[*(sp + 1) >> gamma_shift][*sp];
                        *sp = (_png_byte)((v >> 8) & 0xff);
                        *(sp + 1) = (_png_byte)(v & 0xff);

                        v = gamma_16[*(sp + 3) >> gamma_shift][*(sp + 2)];
                        *(sp + 2) = (_png_byte)((v >> 8) & 0xff);
                        *(sp + 3) = (_png_byte)(v & 0xff);

                        v = gamma_16[*(sp + 5) >> gamma_shift][*(sp + 4)];
                        *(sp + 4) = (_png_byte)((v >> 8) & 0xff);
                        *(sp + 5) = (_png_byte)(v & 0xff);
                     }
                  }
               }

               else
#endif
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 6)
                  {
                     _png_uint_16 r = (_png_uint_16)(((*sp) << 8) + *(sp + 1));

                     _png_uint_16 g = (_png_uint_16)(((*(sp + 2)) << 8)
                         + *(sp + 3));

                     _png_uint_16 b = (_png_uint_16)(((*(sp + 4)) << 8)
                         + *(sp + 5));

                     if (r == _png_ptr->trans_color.red &&
                         g == _png_ptr->trans_color.green &&
                         b == _png_ptr->trans_color.blue)
                     {
                        *sp = (_png_byte)((_png_ptr->background.red >> 8) & 0xff);
                        *(sp + 1) = (_png_byte)(_png_ptr->background.red & 0xff);
                        *(sp + 2) = (_png_byte)((_png_ptr->background.green >> 8)
                                & 0xff);
                        *(sp + 3) = (_png_byte)(_png_ptr->background.green
                                & 0xff);
                        *(sp + 4) = (_png_byte)((_png_ptr->background.blue >> 8)
                                & 0xff);
                        *(sp + 5) = (_png_byte)(_png_ptr->background.blue & 0xff);
                     }
                  }
               }
            }
            break;
         }

         case PNG_COLOR_TYPE_GRAY_ALPHA:
         {
            if (row_info->bit_depth == 8)
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_to_1 != NULL && gamma_from_1 != NULL &&
                   gamma_table != NULL)
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 2)
                  {
                     _png_uint_16 a = *(sp + 1);

                     if (a == 0xff)
                        *sp = gamma_table[*sp];

                     else if (a == 0)
                     {
                        /* Background is already in screen gamma */
                        *sp = (_png_byte)_png_ptr->background.gray;
                     }

                     else
                     {
                        _png_byte v, w;

                        v = gamma_to_1[*sp];
                        _png_composite(w, v, a, _png_ptr->background_1.gray);
                        if (!optimize)
                           w = gamma_from_1[w];
                        *sp = w;
                     }
                  }
               }
               else
#endif
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 2)
                  {
                     _png_byte a = *(sp + 1);

                     if (a == 0)
                        *sp = (_png_byte)_png_ptr->background.gray;

                     else if (a < 0xff)
                        _png_composite(*sp, *sp, a, _png_ptr->background.gray);
                  }
               }
            }
            else /* if (_png_ptr->bit_depth == 16) */
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_16 != NULL && gamma_16_from_1 != NULL &&
                   gamma_16_to_1 != NULL)
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 4)
                  {
                     _png_uint_16 a = (_png_uint_16)(((*(sp + 2)) << 8)
                         + *(sp + 3));

                     if (a == (_png_uint_16)0xffff)
                     {
                        _png_uint_16 v;

                        v = gamma_16[*(sp + 1) >> gamma_shift][*sp];
                        *sp = (_png_byte)((v >> 8) & 0xff);
                        *(sp + 1) = (_png_byte)(v & 0xff);
                     }

                     else if (a == 0)
                     {
                        /* Background is already in screen gamma */
                        *sp = (_png_byte)((_png_ptr->background.gray >> 8)
                                & 0xff);
                        *(sp + 1) = (_png_byte)(_png_ptr->background.gray & 0xff);
                     }

                     else
                     {
                        _png_uint_16 g, v, w;

                        g = gamma_16_to_1[*(sp + 1) >> gamma_shift][*sp];
                        _png_composite_16(v, g, a, _png_ptr->background_1.gray);
                        if (optimize)
                           w = v;
                        else
                           w = gamma_16_from_1[(v&0xff) >> gamma_shift][v >> 8];
                        *sp = (_png_byte)((w >> 8) & 0xff);
                        *(sp + 1) = (_png_byte)(w & 0xff);
                     }
                  }
               }
               else
#endif
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 4)
                  {
                     _png_uint_16 a = (_png_uint_16)(((*(sp + 2)) << 8)
                         + *(sp + 3));

                     if (a == 0)
                     {
                        *sp = (_png_byte)((_png_ptr->background.gray >> 8)
                                & 0xff);
                        *(sp + 1) = (_png_byte)(_png_ptr->background.gray & 0xff);
                     }

                     else if (a < 0xffff)
                     {
                        _png_uint_16 g, v;

                        g = (_png_uint_16)(((*sp) << 8) + *(sp + 1));
                        _png_composite_16(v, g, a, _png_ptr->background.gray);
                        *sp = (_png_byte)((v >> 8) & 0xff);
                        *(sp + 1) = (_png_byte)(v & 0xff);
                     }
                  }
               }
            }
            break;
         }

         case PNG_COLOR_TYPE_RGB_ALPHA:
         {
            if (row_info->bit_depth == 8)
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_to_1 != NULL && gamma_from_1 != NULL &&
                   gamma_table != NULL)
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 4)
                  {
                     _png_byte a = *(sp + 3);

                     if (a == 0xff)
                     {
                        *sp = gamma_table[*sp];
                        *(sp + 1) = gamma_table[*(sp + 1)];
                        *(sp + 2) = gamma_table[*(sp + 2)];
                     }

                     else if (a == 0)
                     {
                        /* Background is already in screen gamma */
                        *sp = (_png_byte)_png_ptr->background.red;
                        *(sp + 1) = (_png_byte)_png_ptr->background.green;
                        *(sp + 2) = (_png_byte)_png_ptr->background.blue;
                     }

                     else
                     {
                        _png_byte v, w;

                        v = gamma_to_1[*sp];
                        _png_composite(w, v, a, _png_ptr->background_1.red);
                        if (!optimize) w = gamma_from_1[w];
                        *sp = w;

                        v = gamma_to_1[*(sp + 1)];
                        _png_composite(w, v, a, _png_ptr->background_1.green);
                        if (!optimize) w = gamma_from_1[w];
                        *(sp + 1) = w;

                        v = gamma_to_1[*(sp + 2)];
                        _png_composite(w, v, a, _png_ptr->background_1.blue);
                        if (!optimize) w = gamma_from_1[w];
                        *(sp + 2) = w;
                     }
                  }
               }
               else
#endif
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 4)
                  {
                     _png_byte a = *(sp + 3);

                     if (a == 0)
                     {
                        *sp = (_png_byte)_png_ptr->background.red;
                        *(sp + 1) = (_png_byte)_png_ptr->background.green;
                        *(sp + 2) = (_png_byte)_png_ptr->background.blue;
                     }

                     else if (a < 0xff)
                     {
                        _png_composite(*sp, *sp, a, _png_ptr->background.red);

                        _png_composite(*(sp + 1), *(sp + 1), a,
                            _png_ptr->background.green);

                        _png_composite(*(sp + 2), *(sp + 2), a,
                            _png_ptr->background.blue);
                     }
                  }
               }
            }
            else /* if (row_info->bit_depth == 16) */
            {
#ifdef PNG_READ_GAMMA_SUPPORTED
               if (gamma_16 != NULL && gamma_16_from_1 != NULL &&
                   gamma_16_to_1 != NULL)
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 8)
                  {
                     _png_uint_16 a = (_png_uint_16)(((_png_uint_16)(*(sp + 6))
                         << 8) + (_png_uint_16)(*(sp + 7)));

                     if (a == (_png_uint_16)0xffff)
                     {
                        _png_uint_16 v;

                        v = gamma_16[*(sp + 1) >> gamma_shift][*sp];
                        *sp = (_png_byte)((v >> 8) & 0xff);
                        *(sp + 1) = (_png_byte)(v & 0xff);

                        v = gamma_16[*(sp + 3) >> gamma_shift][*(sp + 2)];
                        *(sp + 2) = (_png_byte)((v >> 8) & 0xff);
                        *(sp + 3) = (_png_byte)(v & 0xff);

                        v = gamma_16[*(sp + 5) >> gamma_shift][*(sp + 4)];
                        *(sp + 4) = (_png_byte)((v >> 8) & 0xff);
                        *(sp + 5) = (_png_byte)(v & 0xff);
                     }

                     else if (a == 0)
                     {
                        /* Background is already in screen gamma */
                        *sp = (_png_byte)((_png_ptr->background.red >> 8) & 0xff);
                        *(sp + 1) = (_png_byte)(_png_ptr->background.red & 0xff);
                        *(sp + 2) = (_png_byte)((_png_ptr->background.green >> 8)
                                & 0xff);
                        *(sp + 3) = (_png_byte)(_png_ptr->background.green
                                & 0xff);
                        *(sp + 4) = (_png_byte)((_png_ptr->background.blue >> 8)
                                & 0xff);
                        *(sp + 5) = (_png_byte)(_png_ptr->background.blue & 0xff);
                     }

                     else
                     {
                        _png_uint_16 v, w;

                        v = gamma_16_to_1[*(sp + 1) >> gamma_shift][*sp];
                        _png_composite_16(w, v, a, _png_ptr->background_1.red);
                        if (!optimize)
                           w = gamma_16_from_1[((w&0xff) >> gamma_shift)]
                               [w >> 8];
                        *sp = (_png_byte)((w >> 8) & 0xff);
                        *(sp + 1) = (_png_byte)(w & 0xff);

                        v = gamma_16_to_1[*(sp + 3) >> gamma_shift][*(sp + 2)];
                        _png_composite_16(w, v, a, _png_ptr->background_1.green);
                        if (!optimize)
                           w = gamma_16_from_1[((w&0xff) >> gamma_shift)]
                               [w >> 8];

                        *(sp + 2) = (_png_byte)((w >> 8) & 0xff);
                        *(sp + 3) = (_png_byte)(w & 0xff);

                        v = gamma_16_to_1[*(sp + 5) >> gamma_shift][*(sp + 4)];
                        _png_composite_16(w, v, a, _png_ptr->background_1.blue);
                        if (!optimize)
                           w = gamma_16_from_1[((w&0xff) >> gamma_shift)]
                               [w >> 8];

                        *(sp + 4) = (_png_byte)((w >> 8) & 0xff);
                        *(sp + 5) = (_png_byte)(w & 0xff);
                     }
                  }
               }

               else
#endif
               {
                  sp = row;
                  for (i = 0; i < row_width; i++, sp += 8)
                  {
                     _png_uint_16 a = (_png_uint_16)(((_png_uint_16)(*(sp + 6))
                         << 8) + (_png_uint_16)(*(sp + 7)));

                     if (a == 0)
                     {
                        *sp = (_png_byte)((_png_ptr->background.red >> 8) & 0xff);
                        *(sp + 1) = (_png_byte)(_png_ptr->background.red & 0xff);
                        *(sp + 2) = (_png_byte)((_png_ptr->background.green >> 8)
                                & 0xff);
                        *(sp + 3) = (_png_byte)(_png_ptr->background.green
                                & 0xff);
                        *(sp + 4) = (_png_byte)((_png_ptr->background.blue >> 8)
                                & 0xff);
                        *(sp + 5) = (_png_byte)(_png_ptr->background.blue & 0xff);
                     }

                     else if (a < 0xffff)
                     {
                        _png_uint_16 v;

                        _png_uint_16 r = (_png_uint_16)(((*sp) << 8) + *(sp + 1));
                        _png_uint_16 g = (_png_uint_16)(((*(sp + 2)) << 8)
                            + *(sp + 3));
                        _png_uint_16 b = (_png_uint_16)(((*(sp + 4)) << 8)
                            + *(sp + 5));

                        _png_composite_16(v, r, a, _png_ptr->background.red);
                        *sp = (_png_byte)((v >> 8) & 0xff);
                        *(sp + 1) = (_png_byte)(v & 0xff);

                        _png_composite_16(v, g, a, _png_ptr->background.green);
                        *(sp + 2) = (_png_byte)((v >> 8) & 0xff);
                        *(sp + 3) = (_png_byte)(v & 0xff);

                        _png_composite_16(v, b, a, _png_ptr->background.blue);
                        *(sp + 4) = (_png_byte)((v >> 8) & 0xff);
                        *(sp + 5) = (_png_byte)(v & 0xff);
                     }
                  }
               }
            }
            break;
         }

         default:
            break;
      }
   }
}
#endif /* PNG_READ_BACKGROUND_SUPPORTED || PNG_READ_ALPHA_MODE_SUPPORTED */

#ifdef PNG_READ_GAMMA_SUPPORTED
/* Gamma correct the image, avoiding the alpha channel.  Make sure
 * you do this after you deal with the transparency issue on grayscale
 * or RGB images. If your bit depth is 8, use gamma_table, if it
 * is 16, use gamma_16_table and gamma_shift.  Build these with
 * build_gamma_table().
 */
void /* PRIVATE */
_png_do_gamma(_png_row_infop row_info, _png_bytep row, _png_structp _png_ptr)
{
   _png_const_bytep gamma_table = _png_ptr->gamma_table;
   _png_const_uint_16pp gamma_16_table = _png_ptr->gamma_16_table;
   int gamma_shift = _png_ptr->gamma_shift;

   _png_bytep sp;
   _png_uint_32 i;
   _png_uint_32 row_width=row_info->width;

   _png_debug(1, "in _png_do_gamma");

   if (((row_info->bit_depth <= 8 && gamma_table != NULL) ||
       (row_info->bit_depth == 16 && gamma_16_table != NULL)))
   {
      switch (row_info->color_type)
      {
         case PNG_COLOR_TYPE_RGB:
         {
            if (row_info->bit_depth == 8)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  *sp = gamma_table[*sp];
                  sp++;
                  *sp = gamma_table[*sp];
                  sp++;
                  *sp = gamma_table[*sp];
                  sp++;
               }
            }

            else /* if (row_info->bit_depth == 16) */
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  _png_uint_16 v;

                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (_png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (_png_byte)(v & 0xff);
                  sp += 2;

                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (_png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (_png_byte)(v & 0xff);
                  sp += 2;

                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (_png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (_png_byte)(v & 0xff);
                  sp += 2;
               }
            }
            break;
         }

         case PNG_COLOR_TYPE_RGB_ALPHA:
         {
            if (row_info->bit_depth == 8)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  *sp = gamma_table[*sp];
                  sp++;

                  *sp = gamma_table[*sp];
                  sp++;

                  *sp = gamma_table[*sp];
                  sp++;

                  sp++;
               }
            }

            else /* if (row_info->bit_depth == 16) */
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  _png_uint_16 v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (_png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (_png_byte)(v & 0xff);
                  sp += 2;

                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (_png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (_png_byte)(v & 0xff);
                  sp += 2;

                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (_png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (_png_byte)(v & 0xff);
                  sp += 4;
               }
            }
            break;
         }

         case PNG_COLOR_TYPE_GRAY_ALPHA:
         {
            if (row_info->bit_depth == 8)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  *sp = gamma_table[*sp];
                  sp += 2;
               }
            }

            else /* if (row_info->bit_depth == 16) */
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  _png_uint_16 v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (_png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (_png_byte)(v & 0xff);
                  sp += 4;
               }
            }
            break;
         }

         case PNG_COLOR_TYPE_GRAY:
         {
            if (row_info->bit_depth == 2)
            {
               sp = row;
               for (i = 0; i < row_width; i += 4)
               {
                  int a = *sp & 0xc0;
                  int b = *sp & 0x30;
                  int c = *sp & 0x0c;
                  int d = *sp & 0x03;

                  *sp = (_png_byte)(
                      ((((int)gamma_table[a|(a>>2)|(a>>4)|(a>>6)])   ) & 0xc0)|
                      ((((int)gamma_table[(b<<2)|b|(b>>2)|(b>>4)])>>2) & 0x30)|
                      ((((int)gamma_table[(c<<4)|(c<<2)|c|(c>>2)])>>4) & 0x0c)|
                      ((((int)gamma_table[(d<<6)|(d<<4)|(d<<2)|d])>>6) ));
                  sp++;
               }
            }

            if (row_info->bit_depth == 4)
            {
               sp = row;
               for (i = 0; i < row_width; i += 2)
               {
                  int msb = *sp & 0xf0;
                  int lsb = *sp & 0x0f;

                  *sp = (_png_byte)((((int)gamma_table[msb | (msb >> 4)]) & 0xf0)
                      | (((int)gamma_table[(lsb << 4) | lsb]) >> 4));
                  sp++;
               }
            }

            else if (row_info->bit_depth == 8)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  *sp = gamma_table[*sp];
                  sp++;
               }
            }

            else if (row_info->bit_depth == 16)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  _png_uint_16 v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (_png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (_png_byte)(v & 0xff);
                  sp += 2;
               }
            }
            break;
         }

         default:
            break;
      }
   }
}
#endif

#ifdef PNG_READ_ALPHA_MODE_SUPPORTED
/* Encode the alpha channel to the output gamma (the input channel is always
 * linear.)  Called only with color types that have an alpha channel.  Needs the
 * from_1 tables.
 */
void /* PRIVATE */
_png_do_encode_alpha(_png_row_infop row_info, _png_bytep row, _png_structp _png_ptr)
{
   _png_uint_32 row_width = row_info->width;

   _png_debug(1, "in _png_do_encode_alpha");

   if (row_info->color_type & PNG_COLOR_MASK_ALPHA)
   {
      if (row_info->bit_depth == 8)
      {
         PNG_CONST _png_bytep table = _png_ptr->gamma_from_1;

         if (table != NULL)
         {
            PNG_CONST int step =
               (row_info->color_type & PNG_COLOR_MASK_COLOR) ? 4 : 2;

            /* The alpha channel is the last component: */
            row += step - 1;

            for (; row_width > 0; --row_width, row += step)
               *row = table[*row];

            return;
         }
      }

      else if (row_info->bit_depth == 16)
      {
         PNG_CONST _png_uint_16pp table = _png_ptr->gamma_16_from_1;
         PNG_CONST int gamma_shift = _png_ptr->gamma_shift;

         if (table != NULL)
         {
            PNG_CONST int step =
               (row_info->color_type & PNG_COLOR_MASK_COLOR) ? 8 : 4;

            /* The alpha channel is the last component: */
            row += step - 2;

            for (; row_width > 0; --row_width, row += step)
            {
               _png_uint_16 v;

               v = table[*(row + 1) >> gamma_shift][*row];
               *row = (_png_byte)((v >> 8) & 0xff);
               *(row + 1) = (_png_byte)(v & 0xff);
            }

            return;
         }
      }
   }

   /* Only get to here if called with a weird row_info; no harm has been done,
    * so just issue a warning.
    */
   _png_warning(_png_ptr, "_png_do_encode_alpha: unexpected call");
}
#endif

#ifdef PNG_READ_EXPAND_SUPPORTED
/* Expands a palette row to an RGB or RGBA row depending
 * upon whether you supply trans and num_trans.
 */
void /* PRIVATE */
_png_do_expand_palette(_png_row_infop row_info, _png_bytep row,
   _png_const_colorp palette, _png_const_bytep trans_alpha, int num_trans)
{
   int shift, value;
   _png_bytep sp, dp;
   _png_uint_32 i;
   _png_uint_32 row_width=row_info->width;

   _png_debug(1, "in _png_do_expand_palette");

   if (row_info->color_type == PNG_COLOR_TYPE_PALETTE)
   {
      if (row_info->bit_depth < 8)
      {
         switch (row_info->bit_depth)
         {
            case 1:
            {
               sp = row + (_png_size_t)((row_width - 1) >> 3);
               dp = row + (_png_size_t)row_width - 1;
               shift = 7 - (int)((row_width + 7) & 0x07);
               for (i = 0; i < row_width; i++)
               {
                  if ((*sp >> shift) & 0x01)
                     *dp = 1;

                  else
                     *dp = 0;

                  if (shift == 7)
                  {
                     shift = 0;
                     sp--;
                  }

                  else
                     shift++;

                  dp--;
               }
               break;
            }

            case 2:
            {
               sp = row + (_png_size_t)((row_width - 1) >> 2);
               dp = row + (_png_size_t)row_width - 1;
               shift = (int)((3 - ((row_width + 3) & 0x03)) << 1);
               for (i = 0; i < row_width; i++)
               {
                  value = (*sp >> shift) & 0x03;
                  *dp = (_png_byte)value;
                  if (shift == 6)
                  {
                     shift = 0;
                     sp--;
                  }

                  else
                     shift += 2;

                  dp--;
               }
               break;
            }

            case 4:
            {
               sp = row + (_png_size_t)((row_width - 1) >> 1);
               dp = row + (_png_size_t)row_width - 1;
               shift = (int)((row_width & 0x01) << 2);
               for (i = 0; i < row_width; i++)
               {
                  value = (*sp >> shift) & 0x0f;
                  *dp = (_png_byte)value;
                  if (shift == 4)
                  {
                     shift = 0;
                     sp--;
                  }

                  else
                     shift += 4;

                  dp--;
               }
               break;
            }

            default:
               break;
         }
         row_info->bit_depth = 8;
         row_info->pixel_depth = 8;
         row_info->rowbytes = row_width;
      }

      if (row_info->bit_depth == 8)
      {
         {
            if (num_trans > 0)
            {
               sp = row + (_png_size_t)row_width - 1;
               dp = row + (_png_size_t)(row_width << 2) - 1;

               for (i = 0; i < row_width; i++)
               {
                  if ((int)(*sp) >= num_trans)
                     *dp-- = 0xff;

                  else
                     *dp-- = trans_alpha[*sp];

                  *dp-- = palette[*sp].blue;
                  *dp-- = palette[*sp].green;
                  *dp-- = palette[*sp].red;
                  sp--;
               }
               row_info->bit_depth = 8;
               row_info->pixel_depth = 32;
               row_info->rowbytes = row_width * 4;
               row_info->color_type = 6;
               row_info->channels = 4;
            }

            else
            {
               sp = row + (_png_size_t)row_width - 1;
               dp = row + (_png_size_t)(row_width * 3) - 1;

               for (i = 0; i < row_width; i++)
               {
                  *dp-- = palette[*sp].blue;
                  *dp-- = palette[*sp].green;
                  *dp-- = palette[*sp].red;
                  sp--;
               }

               row_info->bit_depth = 8;
               row_info->pixel_depth = 24;
               row_info->rowbytes = row_width * 3;
               row_info->color_type = 2;
               row_info->channels = 3;
            }
         }
      }
   }
}

/* If the bit depth < 8, it is expanded to 8.  Also, if the already
 * expanded transparency value is supplied, an alpha channel is built.
 */
void /* PRIVATE */
_png_do_expand(_png_row_infop row_info, _png_bytep row,
    _png_const_color_16p trans_color)
{
   int shift, value;
   _png_bytep sp, dp;
   _png_uint_32 i;
   _png_uint_32 row_width=row_info->width;

   _png_debug(1, "in _png_do_expand");

   {
      if (row_info->color_type == PNG_COLOR_TYPE_GRAY)
      {
         _png_uint_16 gray = (_png_uint_16)(trans_color ? trans_color->gray : 0);

         if (row_info->bit_depth < 8)
         {
            switch (row_info->bit_depth)
            {
               case 1:
               {
                  gray = (_png_uint_16)((gray & 0x01) * 0xff);
                  sp = row + (_png_size_t)((row_width - 1) >> 3);
                  dp = row + (_png_size_t)row_width - 1;
                  shift = 7 - (int)((row_width + 7) & 0x07);
                  for (i = 0; i < row_width; i++)
                  {
                     if ((*sp >> shift) & 0x01)
                        *dp = 0xff;

                     else
                        *dp = 0;

                     if (shift == 7)
                     {
                        shift = 0;
                        sp--;
                     }

                     else
                        shift++;

                     dp--;
                  }
                  break;
               }

               case 2:
               {
                  gray = (_png_uint_16)((gray & 0x03) * 0x55);
                  sp = row + (_png_size_t)((row_width - 1) >> 2);
                  dp = row + (_png_size_t)row_width - 1;
                  shift = (int)((3 - ((row_width + 3) & 0x03)) << 1);
                  for (i = 0; i < row_width; i++)
                  {
                     value = (*sp >> shift) & 0x03;
                     *dp = (_png_byte)(value | (value << 2) | (value << 4) |
                        (value << 6));
                     if (shift == 6)
                     {
                        shift = 0;
                        sp--;
                     }

                     else
                        shift += 2;

                     dp--;
                  }
                  break;
               }

               case 4:
               {
                  gray = (_png_uint_16)((gray & 0x0f) * 0x11);
                  sp = row + (_png_size_t)((row_width - 1) >> 1);
                  dp = row + (_png_size_t)row_width - 1;
                  shift = (int)((1 - ((row_width + 1) & 0x01)) << 2);
                  for (i = 0; i < row_width; i++)
                  {
                     value = (*sp >> shift) & 0x0f;
                     *dp = (_png_byte)(value | (value << 4));
                     if (shift == 4)
                     {
                        shift = 0;
                        sp--;
                     }

                     else
                        shift = 4;

                     dp--;
                  }
                  break;
               }

               default:
                  break;
            }

            row_info->bit_depth = 8;
            row_info->pixel_depth = 8;
            row_info->rowbytes = row_width;
         }

         if (trans_color != NULL)
         {
            if (row_info->bit_depth == 8)
            {
               /* NOTE: prior to libpng 1.5.14 this cleared out the top bits of
                * 'gray', however if those are set it is an error.
                */
               sp = row + (_png_size_t)row_width - 1;
               dp = row + (_png_size_t)(row_width << 1) - 1;

               for (i = 0; i < row_width; i++)
               {
                  if (*sp == gray)
                     *dp-- = 0;

                  else
                     *dp-- = 0xff;

                  *dp-- = *sp--;
               }
            }

            else if (row_info->bit_depth == 16)
            {
               _png_byte gray_high = (_png_byte)((gray >> 8) & 0xff);
               _png_byte gray_low = (_png_byte)(gray & 0xff);
               sp = row + row_info->rowbytes - 1;
               dp = row + (row_info->rowbytes << 1) - 1;
               for (i = 0; i < row_width; i++)
               {
                  if (*(sp - 1) == gray_high && *(sp) == gray_low)
                  {
                     *dp-- = 0;
                     *dp-- = 0;
                  }

                  else
                  {
                     *dp-- = 0xff;
                     *dp-- = 0xff;
                  }

                  *dp-- = *sp--;
                  *dp-- = *sp--;
               }
            }

            row_info->color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
            row_info->channels = 2;
            row_info->pixel_depth = (_png_byte)(row_info->bit_depth << 1);
            row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth,
               row_width);
         }
      }
      else if (row_info->color_type == PNG_COLOR_TYPE_RGB && trans_color)
      {
         if (row_info->bit_depth == 8)
         {
            _png_byte red = (_png_byte)(trans_color->red & 0xff);
            _png_byte green = (_png_byte)(trans_color->green & 0xff);
            _png_byte blue = (_png_byte)(trans_color->blue & 0xff);
            sp = row + (_png_size_t)row_info->rowbytes - 1;
            dp = row + (_png_size_t)(row_width << 2) - 1;
            for (i = 0; i < row_width; i++)
            {
               if (*(sp - 2) == red && *(sp - 1) == green && *(sp) == blue)
                  *dp-- = 0;

               else
                  *dp-- = 0xff;

               *dp-- = *sp--;
               *dp-- = *sp--;
               *dp-- = *sp--;
            }
         }
         else if (row_info->bit_depth == 16)
         {
            _png_byte red_high = (_png_byte)((trans_color->red >> 8) & 0xff);
            _png_byte green_high = (_png_byte)((trans_color->green >> 8) & 0xff);
            _png_byte blue_high = (_png_byte)((trans_color->blue >> 8) & 0xff);
            _png_byte red_low = (_png_byte)(trans_color->red & 0xff);
            _png_byte green_low = (_png_byte)(trans_color->green & 0xff);
            _png_byte blue_low = (_png_byte)(trans_color->blue & 0xff);
            sp = row + row_info->rowbytes - 1;
            dp = row + (_png_size_t)(row_width << 3) - 1;
            for (i = 0; i < row_width; i++)
            {
               if (*(sp - 5) == red_high &&
                   *(sp - 4) == red_low &&
                   *(sp - 3) == green_high &&
                   *(sp - 2) == green_low &&
                   *(sp - 1) == blue_high &&
                   *(sp    ) == blue_low)
               {
                  *dp-- = 0;
                  *dp-- = 0;
               }

               else
               {
                  *dp-- = 0xff;
                  *dp-- = 0xff;
               }

               *dp-- = *sp--;
               *dp-- = *sp--;
               *dp-- = *sp--;
               *dp-- = *sp--;
               *dp-- = *sp--;
               *dp-- = *sp--;
            }
         }
         row_info->color_type = PNG_COLOR_TYPE_RGB_ALPHA;
         row_info->channels = 4;
         row_info->pixel_depth = (_png_byte)(row_info->bit_depth << 2);
         row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
      }
   }
}
#endif

#ifdef PNG_READ_EXPAND_16_SUPPORTED
/* If the bit depth is 8 and the color type is not a palette type expand the
 * whole row to 16 bits.  Has no effect otherwise.
 */
void /* PRIVATE */
_png_do_expand_16(_png_row_infop row_info, _png_bytep row)
{
   if (row_info->bit_depth == 8 &&
      row_info->color_type != PNG_COLOR_TYPE_PALETTE)
   {
      /* The row have a sequence of bytes containing [0..255] and we need
       * to turn it into another row containing [0..65535], to do this we
       * calculate:
       *
       *  (input / 255) * 65535
       *
       *  Which happens to be exactly input * 257 and this can be achieved
       *  simply by byte replication in place (copying backwards).
       */
      _png_byte *sp = row + row_info->rowbytes; /* source, last byte + 1 */
      _png_byte *dp = sp + row_info->rowbytes;  /* destination, end + 1 */
      while (dp > sp)
         dp[-2] = dp[-1] = *--sp, dp -= 2;

      row_info->rowbytes *= 2;
      row_info->bit_depth = 16;
      row_info->pixel_depth = (_png_byte)(row_info->channels * 16);
   }
}
#endif

#ifdef PNG_READ_QUANTIZE_SUPPORTED
void /* PRIVATE */
_png_do_quantize(_png_row_infop row_info, _png_bytep row,
    _png_const_bytep palette_lookup, _png_const_bytep quantize_lookup)
{
   _png_bytep sp, dp;
   _png_uint_32 i;
   _png_uint_32 row_width=row_info->width;

   _png_debug(1, "in _png_do_quantize");

   if (row_info->bit_depth == 8)
   {
      if (row_info->color_type == PNG_COLOR_TYPE_RGB && palette_lookup)
      {
         int r, g, b, p;
         sp = row;
         dp = row;
         for (i = 0; i < row_width; i++)
         {
            r = *sp++;
            g = *sp++;
            b = *sp++;

            /* This looks real messy, but the compiler will reduce
             * it down to a reasonable formula.  For example, with
             * 5 bits per color, we get:
             * p = (((r >> 3) & 0x1f) << 10) |
             *    (((g >> 3) & 0x1f) << 5) |
             *    ((b >> 3) & 0x1f);
             */
            p = (((r >> (8 - PNG_QUANTIZE_RED_BITS)) &
                ((1 << PNG_QUANTIZE_RED_BITS) - 1)) <<
                (PNG_QUANTIZE_GREEN_BITS + PNG_QUANTIZE_BLUE_BITS)) |
                (((g >> (8 - PNG_QUANTIZE_GREEN_BITS)) &
                ((1 << PNG_QUANTIZE_GREEN_BITS) - 1)) <<
                (PNG_QUANTIZE_BLUE_BITS)) |
                ((b >> (8 - PNG_QUANTIZE_BLUE_BITS)) &
                ((1 << PNG_QUANTIZE_BLUE_BITS) - 1));

            *dp++ = palette_lookup[p];
         }

         row_info->color_type = PNG_COLOR_TYPE_PALETTE;
         row_info->channels = 1;
         row_info->pixel_depth = row_info->bit_depth;
         row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
      }

      else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA &&
         palette_lookup != NULL)
      {
         int r, g, b, p;
         sp = row;
         dp = row;
         for (i = 0; i < row_width; i++)
         {
            r = *sp++;
            g = *sp++;
            b = *sp++;
            sp++;

            p = (((r >> (8 - PNG_QUANTIZE_RED_BITS)) &
                ((1 << PNG_QUANTIZE_RED_BITS) - 1)) <<
                (PNG_QUANTIZE_GREEN_BITS + PNG_QUANTIZE_BLUE_BITS)) |
                (((g >> (8 - PNG_QUANTIZE_GREEN_BITS)) &
                ((1 << PNG_QUANTIZE_GREEN_BITS) - 1)) <<
                (PNG_QUANTIZE_BLUE_BITS)) |
                ((b >> (8 - PNG_QUANTIZE_BLUE_BITS)) &
                ((1 << PNG_QUANTIZE_BLUE_BITS) - 1));

            *dp++ = palette_lookup[p];
         }

         row_info->color_type = PNG_COLOR_TYPE_PALETTE;
         row_info->channels = 1;
         row_info->pixel_depth = row_info->bit_depth;
         row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
      }

      else if (row_info->color_type == PNG_COLOR_TYPE_PALETTE &&
         quantize_lookup)
      {
         sp = row;

         for (i = 0; i < row_width; i++, sp++)
         {
            *sp = quantize_lookup[*sp];
         }
      }
   }
}
#endif /* PNG_READ_QUANTIZE_SUPPORTED */
#endif /* PNG_READ_TRANSFORMS_SUPPORTED */

#ifdef PNG_MNG_FEATURES_SUPPORTED
/* Undoes intrapixel differencing  */
void /* PRIVATE */
_png_do_read_intrapixel(_png_row_infop row_info, _png_bytep row)
{
   _png_debug(1, "in _png_do_read_intrapixel");

   if (
       (row_info->color_type & PNG_COLOR_MASK_COLOR))
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
            *(rp) = (_png_byte)((256 + *rp + *(rp + 1)) & 0xff);
            *(rp+2) = (_png_byte)((256 + *(rp + 2) + *(rp + 1)) & 0xff);
         }
      }
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
            _png_uint_32 red  = (s0 + s1 + 65536) & 0xffff;
            _png_uint_32 blue = (s2 + s1 + 65536) & 0xffff;
            *(rp    ) = (_png_byte)((red >> 8) & 0xff);
            *(rp + 1) = (_png_byte)(red & 0xff);
            *(rp + 4) = (_png_byte)((blue >> 8) & 0xff);
            *(rp + 5) = (_png_byte)(blue & 0xff);
         }
      }
   }
}
#endif /* PNG_MNG_FEATURES_SUPPORTED */
#endif /* PNG_READ_SUPPORTED */
