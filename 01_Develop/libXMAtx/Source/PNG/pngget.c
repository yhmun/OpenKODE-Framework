
/* pngget.c - retrieval of values from info struct
 *
 * Last changed in libpng 1.5.14 [January 24, 2013]
 * Copyright (c) 1998-2011 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 *
 */

#include "pngpriv.h"

#if defined(PNG_READ_SUPPORTED) || defined(PNG_WRITE_SUPPORTED)

_png_uint_32 PNGAPI
_png_get_valid(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_uint_32 flag)
{
   if (_png_ptr != NULL && info_ptr != NULL)
      return(info_ptr->valid & flag);

   return(0);
}

_png_size_t PNGAPI
_png_get_rowbytes(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   if (_png_ptr != NULL && info_ptr != NULL)
      return(info_ptr->rowbytes);

   return(0);
}

#ifdef PNG_INFO_IMAGE_SUPPORTED
_png_bytepp PNGAPI
_png_get_rows(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   if (_png_ptr != NULL && info_ptr != NULL)
      return(info_ptr->row_pointers);

   return(0);
}
#endif

#ifdef PNG_EASY_ACCESS_SUPPORTED
/* Easy access to info, added in libpng-0.99 */
_png_uint_32 PNGAPI
_png_get_image_width(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   if (_png_ptr != NULL && info_ptr != NULL)
      return info_ptr->width;

   return (0);
}

_png_uint_32 PNGAPI
_png_get_image_height(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   if (_png_ptr != NULL && info_ptr != NULL)
      return info_ptr->height;

   return (0);
}

_png_byte PNGAPI
_png_get_bit_depth(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   if (_png_ptr != NULL && info_ptr != NULL)
      return info_ptr->bit_depth;

   return (0);
}

_png_byte PNGAPI
_png_get_color_type(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   if (_png_ptr != NULL && info_ptr != NULL)
      return info_ptr->color_type;

   return (0);
}

_png_byte PNGAPI
_png_get_filter_type(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   if (_png_ptr != NULL && info_ptr != NULL)
      return info_ptr->filter_type;

   return (0);
}

_png_byte PNGAPI
_png_get_interlace_type(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   if (_png_ptr != NULL && info_ptr != NULL)
      return info_ptr->interlace_type;

   return (0);
}

_png_byte PNGAPI
_png_get_compression_type(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   if (_png_ptr != NULL && info_ptr != NULL)
      return info_ptr->compression_type;

   return (0);
}

_png_uint_32 PNGAPI
_png_get_x_pixels_per_meter(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
#ifdef PNG_pHYs_SUPPORTED
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
      {
         _png_debug1(1, "in %s retrieval function",
             "_png_get_x_pixels_per_meter");

         if (info_ptr->phys_unit_type == PNG_RESOLUTION_METER)
            return (info_ptr->x_pixels_per_unit);
      }
#endif

   return (0);
}

_png_uint_32 PNGAPI
_png_get_y_pixels_per_meter(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
#ifdef PNG_pHYs_SUPPORTED
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
   {
      _png_debug1(1, "in %s retrieval function",
          "_png_get_y_pixels_per_meter");

      if (info_ptr->phys_unit_type == PNG_RESOLUTION_METER)
         return (info_ptr->y_pixels_per_unit);
   }
#endif

   return (0);
}

_png_uint_32 PNGAPI
_png_get_pixels_per_meter(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
#ifdef PNG_pHYs_SUPPORTED
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
   {
      _png_debug1(1, "in %s retrieval function", "_png_get_pixels_per_meter");

      if (info_ptr->phys_unit_type == PNG_RESOLUTION_METER &&
          info_ptr->x_pixels_per_unit == info_ptr->y_pixels_per_unit)
         return (info_ptr->x_pixels_per_unit);
   }
#endif

   return (0);
}

#ifdef PNG_FLOATING_POINT_SUPPORTED
float PNGAPI
_png_get_pixel_aspect_ratio(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
#ifdef PNG_READ_pHYs_SUPPORTED
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
   {
      _png_debug1(1, "in %s retrieval function", "_png_get_aspect_ratio");

      if (info_ptr->x_pixels_per_unit != 0)
         return ((float)((float)info_ptr->y_pixels_per_unit
             /(float)info_ptr->x_pixels_per_unit));
   }
#else
   PNG_UNUSED(_png_ptr)
   PNG_UNUSED(info_ptr)
#endif

   return ((float)0.0);
}
#endif

#ifdef PNG_FIXED_POINT_SUPPORTED
_png_fixed_point PNGAPI
_png_get_pixel_aspect_ratio_fixed(_png_const_structp _png_ptr,
    _png_const_infop info_ptr)
{
#ifdef PNG_READ_pHYs_SUPPORTED
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs)
       && info_ptr->x_pixels_per_unit > 0 && info_ptr->y_pixels_per_unit > 0
       && info_ptr->x_pixels_per_unit <= PNG_UINT_31_MAX
       && info_ptr->y_pixels_per_unit <= PNG_UINT_31_MAX)
   {
      _png_fixed_point res;

      _png_debug1(1, "in %s retrieval function", "_png_get_aspect_ratio_fixed");

      /* The following casts work because a PNG 4 byte integer only has a valid
       * range of 0..2^31-1; otherwise the cast might overflow.
       */
      if (_png_muldiv(&res, (_png_int_32)info_ptr->y_pixels_per_unit, PNG_FP_1,
          (_png_int_32)info_ptr->x_pixels_per_unit))
         return res;
   }
#else
   PNG_UNUSED(_png_ptr)
   PNG_UNUSED(info_ptr)
#endif

   return 0;
}
#endif

_png_int_32 PNGAPI
_png_get_x_offset_microns(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
#ifdef PNG_oFFs_SUPPORTED
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_oFFs))
   {
      _png_debug1(1, "in %s retrieval function", "_png_get_x_offset_microns");

      if (info_ptr->offset_unit_type == PNG_OFFSET_MICROMETER)
         return (info_ptr->x_offset);
   }
#endif

   return (0);
}

_png_int_32 PNGAPI
_png_get_y_offset_microns(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
#ifdef PNG_oFFs_SUPPORTED
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_oFFs))
   {
      _png_debug1(1, "in %s retrieval function", "_png_get_y_offset_microns");

      if (info_ptr->offset_unit_type == PNG_OFFSET_MICROMETER)
         return (info_ptr->y_offset);
   }
#endif

   return (0);
}

_png_int_32 PNGAPI
_png_get_x_offset_pixels(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
#ifdef PNG_oFFs_SUPPORTED
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_oFFs))
   {
      _png_debug1(1, "in %s retrieval function", "_png_get_x_offset_pixels");

      if (info_ptr->offset_unit_type == PNG_OFFSET_PIXEL)
         return (info_ptr->x_offset);
   }
#endif

   return (0);
}

_png_int_32 PNGAPI
_png_get_y_offset_pixels(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
#ifdef PNG_oFFs_SUPPORTED
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_oFFs))
   {
      _png_debug1(1, "in %s retrieval function", "_png_get_y_offset_pixels");

      if (info_ptr->offset_unit_type == PNG_OFFSET_PIXEL)
         return (info_ptr->y_offset);
   }
#endif

   return (0);
}

#ifdef PNG_INCH_CONVERSIONS_SUPPORTED
static _png_uint_32
ppi_from_ppm(_png_uint_32 ppm)
{
#if 0
   /* The conversion is *(2.54/100), in binary (32 digits):
    * .00000110100000001001110101001001
    */
   _png_uint_32 t1001, t1101;
   ppm >>= 1;                  /* .1 */
   t1001 = ppm + (ppm >> 3);   /* .1001 */
   t1101 = t1001 + (ppm >> 1); /* .1101 */
   ppm >>= 20;                 /* .000000000000000000001 */
   t1101 += t1101 >> 15;       /* .1101000000000001101 */
   t1001 >>= 11;               /* .000000000001001 */
   t1001 += t1001 >> 12;       /* .000000000001001000000001001 */
   ppm += t1001;               /* .000000000001001000001001001 */
   ppm += t1101;               /* .110100000001001110101001001 */
   return (ppm + 16) >> 5;/* .00000110100000001001110101001001 */
#else
   /* The argument is a PNG unsigned integer, so it is not permitted
    * to be bigger than 2^31.
    */
   _png_fixed_point result;
   if (ppm <= PNG_UINT_31_MAX && _png_muldiv(&result, (_png_int_32)ppm, 127,
       5000))
      return result;

   /* Overflow. */
   return 0;
#endif
}

_png_uint_32 PNGAPI
_png_get_pixels_per_inch(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   return ppi_from_ppm(_png_get_pixels_per_meter(_png_ptr, info_ptr));
}

_png_uint_32 PNGAPI
_png_get_x_pixels_per_inch(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   return ppi_from_ppm(_png_get_x_pixels_per_meter(_png_ptr, info_ptr));
}

_png_uint_32 PNGAPI
_png_get_y_pixels_per_inch(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   return ppi_from_ppm(_png_get_y_pixels_per_meter(_png_ptr, info_ptr));
}

#ifdef PNG_FIXED_POINT_SUPPORTED
static _png_fixed_point
_png_fixed_inches_from_microns(_png_structp _png_ptr, _png_int_32 microns)
{
   /* Convert from metres * 1,000,000 to inches * 100,000, meters to
    * inches is simply *(100/2.54), so we want *(10/2.54) == 500/127.
    * Notice that this can overflow - a warning is output and 0 is
    * returned.
    */
   return _png_muldiv_warn(_png_ptr, microns, 500, 127);
}

_png_fixed_point PNGAPI
_png_get_x_offset_inches_fixed(_png_structp _png_ptr,
    _png_const_infop info_ptr)
{
   return _png_fixed_inches_from_microns(_png_ptr,
       _png_get_x_offset_microns(_png_ptr, info_ptr));
}
#endif

#ifdef PNG_FIXED_POINT_SUPPORTED
_png_fixed_point PNGAPI
_png_get_y_offset_inches_fixed(_png_structp _png_ptr,
    _png_const_infop info_ptr)
{
   return _png_fixed_inches_from_microns(_png_ptr,
       _png_get_y_offset_microns(_png_ptr, info_ptr));
}
#endif

#ifdef PNG_FLOATING_POINT_SUPPORTED
float PNGAPI
_png_get_x_offset_inches(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   /* To avoid the overflow do the conversion directly in floating
    * point.
    */
   return (float)(_png_get_x_offset_microns(_png_ptr, info_ptr) * .00003937);
}
#endif

#ifdef PNG_FLOATING_POINT_SUPPORTED
float PNGAPI
_png_get_y_offset_inches(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   /* To avoid the overflow do the conversion directly in floating
    * point.
    */
   return (float)(_png_get_y_offset_microns(_png_ptr, info_ptr) * .00003937);
}
#endif

#ifdef PNG_pHYs_SUPPORTED
_png_uint_32 PNGAPI
_png_get_pHYs_dpi(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_uint_32 *res_x, _png_uint_32 *res_y, int *unit_type)
{
   _png_uint_32 retval = 0;

   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs))
   {
      _png_debug1(1, "in %s retrieval function", "pHYs");

      if (res_x != NULL)
      {
         *res_x = info_ptr->x_pixels_per_unit;
         retval |= PNG_INFO_pHYs;
      }

      if (res_y != NULL)
      {
         *res_y = info_ptr->y_pixels_per_unit;
         retval |= PNG_INFO_pHYs;
      }

      if (unit_type != NULL)
      {
         *unit_type = (int)info_ptr->phys_unit_type;
         retval |= PNG_INFO_pHYs;

         if (*unit_type == 1)
         {
            if (res_x != NULL) *res_x = (_png_uint_32)(*res_x * .0254 + .50);
            if (res_y != NULL) *res_y = (_png_uint_32)(*res_y * .0254 + .50);
         }
      }
   }

   return (retval);
}
#endif /* PNG_pHYs_SUPPORTED */
#endif  /* PNG_INCH_CONVERSIONS_SUPPORTED */

/* _png_get_channels really belongs in here, too, but it's been around longer */

#endif  /* PNG_EASY_ACCESS_SUPPORTED */

_png_byte PNGAPI
_png_get_channels(_png_const_structp _png_ptr, _png_const_infop info_ptr)
{
   if (_png_ptr != NULL && info_ptr != NULL)
      return(info_ptr->channels);

   return (0);
}

_png_const_bytep PNGAPI
_png_get_signature(_png_const_structp _png_ptr, _png_infop info_ptr)
{
   if (_png_ptr != NULL && info_ptr != NULL)
      return(info_ptr->signature);

   return (NULL);
}

#ifdef PNG_bKGD_SUPPORTED
_png_uint_32 PNGAPI
_png_get_bKGD(_png_const_structp _png_ptr, _png_infop info_ptr,
   _png_color_16p *background)
{
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_bKGD)
       && background != NULL)
   {
      _png_debug1(1, "in %s retrieval function", "bKGD");

      *background = &(info_ptr->background);
      return (PNG_INFO_bKGD);
   }

   return (0);
}
#endif

#ifdef PNG_cHRM_SUPPORTED
/* The XYZ APIs were added in 1.5.5 to take advantage of the code added at the
 * same time to correct the rgb grayscale coefficient defaults obtained from the
 * cHRM chunk in 1.5.4
 */
_png_uint_32 PNGFAPI
_png_get_cHRM_XYZ_fixed(_png_structp _png_ptr, _png_const_infop info_ptr,
    _png_fixed_point *int_red_X, _png_fixed_point *int_red_Y,
    _png_fixed_point *int_red_Z, _png_fixed_point *int_green_X,
    _png_fixed_point *int_green_Y, _png_fixed_point *int_green_Z,
    _png_fixed_point *int_blue_X, _png_fixed_point *int_blue_Y,
    _png_fixed_point *int_blue_Z)
{
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM))
   {
      _png_xy xy;
      _png_XYZ XYZ;

      _png_debug1(1, "in %s retrieval function", "cHRM_XYZ");

      xy.whitex = info_ptr->x_white;
      xy.whitey = info_ptr->y_white;
      xy.redx = info_ptr->x_red;
      xy.redy = info_ptr->y_red;
      xy.greenx = info_ptr->x_green;
      xy.greeny = info_ptr->y_green;
      xy.bluex = info_ptr->x_blue;
      xy.bluey = info_ptr->y_blue;

      /* The *_checked function handles error reporting, so just return 0 if
       * there is a failure here.
       */
      if (_png_XYZ_from_xy_checked(_png_ptr, &XYZ, xy))
      {
         if (int_red_X != NULL)
            *int_red_X = XYZ.redX;
         if (int_red_Y != NULL)
            *int_red_Y = XYZ.redY;
         if (int_red_Z != NULL)
            *int_red_Z = XYZ.redZ;
         if (int_green_X != NULL)
            *int_green_X = XYZ.greenX;
         if (int_green_Y != NULL)
            *int_green_Y = XYZ.greenY;
         if (int_green_Z != NULL)
            *int_green_Z = XYZ.greenZ;
         if (int_blue_X != NULL)
            *int_blue_X = XYZ.blueX;
         if (int_blue_Y != NULL)
            *int_blue_Y = XYZ.blueY;
         if (int_blue_Z != NULL)
            *int_blue_Z = XYZ.blueZ;

         return (PNG_INFO_cHRM);
      }
   }

   return (0);
}

#  ifdef PNG_FLOATING_POINT_SUPPORTED
_png_uint_32 PNGAPI
_png_get_cHRM(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    double *white_x, double *white_y, double *red_x, double *red_y,
    double *green_x, double *green_y, double *blue_x, double *blue_y)
{
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM))
   {
      _png_debug1(1, "in %s retrieval function", "cHRM");

      if (white_x != NULL)
         *white_x = _png_float(_png_ptr, info_ptr->x_white, "cHRM white X");
      if (white_y != NULL)
         *white_y = _png_float(_png_ptr, info_ptr->y_white, "cHRM white Y");
      if (red_x != NULL)
         *red_x = _png_float(_png_ptr, info_ptr->x_red, "cHRM red X");
      if (red_y != NULL)
         *red_y = _png_float(_png_ptr, info_ptr->y_red, "cHRM red Y");
      if (green_x != NULL)
         *green_x = _png_float(_png_ptr, info_ptr->x_green, "cHRM green X");
      if (green_y != NULL)
         *green_y = _png_float(_png_ptr, info_ptr->y_green, "cHRM green Y");
      if (blue_x != NULL)
         *blue_x = _png_float(_png_ptr, info_ptr->x_blue, "cHRM blue X");
      if (blue_y != NULL)
         *blue_y = _png_float(_png_ptr, info_ptr->y_blue, "cHRM blue Y");
      return (PNG_INFO_cHRM);
   }

   return (0);
}

_png_uint_32 PNGAPI
_png_get_cHRM_XYZ(_png_structp _png_ptr, _png_const_infop info_ptr,
   double *red_X, double *red_Y, double *red_Z, double *green_X,
   double *green_Y, double *green_Z, double *blue_X, double *blue_Y,
   double *blue_Z)
{
   _png_XYZ XYZ;

   if (_png_get_cHRM_XYZ_fixed(_png_ptr, info_ptr,
      &XYZ.redX, &XYZ.redY, &XYZ.redZ, &XYZ.greenX, &XYZ.greenY, &XYZ.greenZ,
      &XYZ.blueX, &XYZ.blueY, &XYZ.blueZ) & PNG_INFO_cHRM)
   {
      if (red_X != NULL)
         *red_X = _png_float(_png_ptr, XYZ.redX, "cHRM red X");
      if (red_Y != NULL)
         *red_Y = _png_float(_png_ptr, XYZ.redY, "cHRM red Y");
      if (red_Z != NULL)
         *red_Z = _png_float(_png_ptr, XYZ.redZ, "cHRM red Z");
      if (green_X != NULL)
         *green_X = _png_float(_png_ptr, XYZ.greenX, "cHRM green X");
      if (green_Y != NULL)
         *green_Y = _png_float(_png_ptr, XYZ.greenY, "cHRM green Y");
      if (green_Z != NULL)
         *green_Z = _png_float(_png_ptr, XYZ.greenZ, "cHRM green Z");
      if (blue_X != NULL)
         *blue_X = _png_float(_png_ptr, XYZ.blueX, "cHRM blue X");
      if (blue_Y != NULL)
         *blue_Y = _png_float(_png_ptr, XYZ.blueY, "cHRM blue Y");
      if (blue_Z != NULL)
         *blue_Z = _png_float(_png_ptr, XYZ.blueZ, "cHRM blue Z");
      return (PNG_INFO_cHRM);
   }

   return (0);
}
#  endif

#  ifdef PNG_FIXED_POINT_SUPPORTED
_png_uint_32 PNGAPI
_png_get_cHRM_fixed(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_fixed_point *white_x, _png_fixed_point *white_y, _png_fixed_point *red_x,
    _png_fixed_point *red_y, _png_fixed_point *green_x, _png_fixed_point *green_y,
    _png_fixed_point *blue_x, _png_fixed_point *blue_y)
{
   _png_debug1(1, "in %s retrieval function", "cHRM");

   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM))
   {
      if (white_x != NULL)
         *white_x = info_ptr->x_white;
      if (white_y != NULL)
         *white_y = info_ptr->y_white;
      if (red_x != NULL)
         *red_x = info_ptr->x_red;
      if (red_y != NULL)
         *red_y = info_ptr->y_red;
      if (green_x != NULL)
         *green_x = info_ptr->x_green;
      if (green_y != NULL)
         *green_y = info_ptr->y_green;
      if (blue_x != NULL)
         *blue_x = info_ptr->x_blue;
      if (blue_y != NULL)
         *blue_y = info_ptr->y_blue;
      return (PNG_INFO_cHRM);
   }

   return (0);
}
#  endif
#endif

#ifdef PNG_gAMA_SUPPORTED
_png_uint_32 PNGFAPI
_png_get_gAMA_fixed(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_fixed_point *file_gamma)
{
   _png_debug1(1, "in %s retrieval function", "gAMA");

   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_gAMA)
       && file_gamma != NULL)
   {
      *file_gamma = info_ptr->gamma;
      return (PNG_INFO_gAMA);
   }

   return (0);
}
#  ifdef PNG_FLOATING_POINT_SUPPORTED
_png_uint_32 PNGAPI
_png_get_gAMA(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    double *file_gamma)
{
   _png_fixed_point igamma;
   _png_uint_32 ok = _png_get_gAMA_fixed(_png_ptr, info_ptr, &igamma);

   if (ok)
      *file_gamma = _png_float(_png_ptr, igamma, "_png_get_gAMA");

   return ok;
}

#  endif
#endif

#ifdef PNG_sRGB_SUPPORTED
_png_uint_32 PNGAPI
_png_get_sRGB(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    int *file_srgb_intent)
{
   _png_debug1(1, "in %s retrieval function", "sRGB");

   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_sRGB)
       && file_srgb_intent != NULL)
   {
      *file_srgb_intent = (int)info_ptr->srgb_intent;
      return (PNG_INFO_sRGB);
   }

   return (0);
}
#endif

#ifdef PNG_iCCP_SUPPORTED
_png_uint_32 PNGAPI
_png_get_iCCP(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_charpp name, int *compression_type,
    _png_bytepp profile, _png_uint_32 *proflen)
{
   _png_debug1(1, "in %s retrieval function", "iCCP");

   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_iCCP)
       && name != NULL && compression_type != NULL && profile != NULL &&
		 proflen != NULL)
   {
      *name = info_ptr->iccp_name;
      *profile = info_ptr->iccp_profile;
      /* Compression_type is a dummy so the API won't have to change
       * if we introduce multiple compression types later.
       */
      *proflen = info_ptr->iccp_proflen;
      *compression_type = info_ptr->iccp_compression;
      return (PNG_INFO_iCCP);
   }

   return (0);
}
#endif

#ifdef PNG_sPLT_SUPPORTED
_png_uint_32 PNGAPI
_png_get_sPLT(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_sPLT_tpp spalettes)
{
   if (_png_ptr != NULL && info_ptr != NULL && spalettes != NULL)
   {
      *spalettes = info_ptr->splt_palettes;
      return ((_png_uint_32)info_ptr->splt_palettes_num);
   }

   return (0);
}
#endif

#ifdef PNG_hIST_SUPPORTED
_png_uint_32 PNGAPI
_png_get_hIST(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_uint_16p *hist)
{
   _png_debug1(1, "in %s retrieval function", "hIST");

   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_hIST)
       && hist != NULL)
   {
      *hist = info_ptr->hist;
      return (PNG_INFO_hIST);
   }

   return (0);
}
#endif

_png_uint_32 PNGAPI
_png_get_IHDR(_png_structp _png_ptr, _png_infop info_ptr,
    _png_uint_32 *width, _png_uint_32 *height, int *bit_depth,
    int *color_type, int *interlace_type, int *compression_type,
    int *filter_type)

{
   _png_debug1(1, "in %s retrieval function", "IHDR");

   if (_png_ptr == NULL || info_ptr == NULL || width == NULL ||
       height == NULL || bit_depth == NULL || color_type == NULL)
      return (0);

   *width = info_ptr->width;
   *height = info_ptr->height;
   *bit_depth = info_ptr->bit_depth;
   *color_type = info_ptr->color_type;

   if (compression_type != NULL)
      *compression_type = info_ptr->compression_type;

   if (filter_type != NULL)
      *filter_type = info_ptr->filter_type;

   if (interlace_type != NULL)
      *interlace_type = info_ptr->interlace_type;

   /* This is redundant if we can be sure that the info_ptr values were all
    * assigned in _png_set_IHDR().  We do the check anyhow in case an
    * application has ignored our advice not to mess with the members
    * of info_ptr directly.
    */
   _png_check_IHDR (_png_ptr, info_ptr->width, info_ptr->height,
       info_ptr->bit_depth, info_ptr->color_type, info_ptr->interlace_type,
       info_ptr->compression_type, info_ptr->filter_type);

   return (1);
}

#ifdef PNG_oFFs_SUPPORTED
_png_uint_32 PNGAPI
_png_get_oFFs(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_int_32 *offset_x, _png_int_32 *offset_y, int *unit_type)
{
   _png_debug1(1, "in %s retrieval function", "oFFs");

   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_oFFs)
       && offset_x != NULL && offset_y != NULL && unit_type != NULL)
   {
      *offset_x = info_ptr->x_offset;
      *offset_y = info_ptr->y_offset;
      *unit_type = (int)info_ptr->offset_unit_type;
      return (PNG_INFO_oFFs);
   }

   return (0);
}
#endif

#ifdef PNG_pCAL_SUPPORTED
_png_uint_32 PNGAPI
_png_get_pCAL(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_charp *purpose, _png_int_32 *X0, _png_int_32 *X1, int *type, int *nparams,
    _png_charp *units, _png_charpp *params)
{
   _png_debug1(1, "in %s retrieval function", "pCAL");

   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pCAL)
       && purpose != NULL && X0 != NULL && X1 != NULL && type != NULL &&
       nparams != NULL && units != NULL && params != NULL)
   {
      *purpose = info_ptr->pcal_purpose;
      *X0 = info_ptr->pcal_X0;
      *X1 = info_ptr->pcal_X1;
      *type = (int)info_ptr->pcal_type;
      *nparams = (int)info_ptr->pcal_nparams;
      *units = info_ptr->pcal_units;
      *params = info_ptr->pcal_params;
      return (PNG_INFO_pCAL);
   }

   return (0);
}
#endif

#ifdef PNG_sCAL_SUPPORTED
#  ifdef PNG_FIXED_POINT_SUPPORTED
#    ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
_png_uint_32 PNGAPI
_png_get_sCAL_fixed(_png_structp _png_ptr, _png_const_infop info_ptr,
    int *unit, _png_fixed_point *width, _png_fixed_point *height)
{
   if (_png_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_sCAL))
   {
      *unit = info_ptr->scal_unit;
      /*TODO: make this work without FP support */
      *width = _png_fixed(_png_ptr, atof(info_ptr->scal_s_width), "sCAL width");
      *height = _png_fixed(_png_ptr, atof(info_ptr->scal_s_height),
         "sCAL height");
      return (PNG_INFO_sCAL);
   }

   return(0);
}
#    endif /* FLOATING_ARITHMETIC */
#  endif /* FIXED_POINT */
#  ifdef PNG_FLOATING_POINT_SUPPORTED
_png_uint_32 PNGAPI
_png_get_sCAL(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    int *unit, double *width, double *height)
{
   if (_png_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_sCAL))
   {
      *unit = info_ptr->scal_unit;
      *width = atof(info_ptr->scal_s_width);
      *height = atof(info_ptr->scal_s_height);
      return (PNG_INFO_sCAL);
   }

   return(0);
}
#  endif /* FLOATING POINT */
_png_uint_32 PNGAPI
_png_get_sCAL_s(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    int *unit, _png_charpp width, _png_charpp height)
{
   if (_png_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_sCAL))
   {
      *unit = info_ptr->scal_unit;
      *width = info_ptr->scal_s_width;
      *height = info_ptr->scal_s_height;
      return (PNG_INFO_sCAL);
   }

   return(0);
}
#endif /* sCAL */

#ifdef PNG_pHYs_SUPPORTED
_png_uint_32 PNGAPI
_png_get_pHYs(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_uint_32 *res_x, _png_uint_32 *res_y, int *unit_type)
{
   _png_uint_32 retval = 0;

   _png_debug1(1, "in %s retrieval function", "pHYs");

   if (_png_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_pHYs))
   {
      if (res_x != NULL)
      {
         *res_x = info_ptr->x_pixels_per_unit;
         retval |= PNG_INFO_pHYs;
      }

      if (res_y != NULL)
      {
         *res_y = info_ptr->y_pixels_per_unit;
         retval |= PNG_INFO_pHYs;
      }

      if (unit_type != NULL)
      {
         *unit_type = (int)info_ptr->phys_unit_type;
         retval |= PNG_INFO_pHYs;
      }
   }

   return (retval);
}
#endif /* pHYs */

_png_uint_32 PNGAPI
_png_get_PLTE(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_colorp *palette, int *num_palette)
{
   _png_debug1(1, "in %s retrieval function", "PLTE");

   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_PLTE)
       && palette != NULL)
   {
      *palette = info_ptr->palette;
      *num_palette = info_ptr->num_palette;
      _png_debug1(3, "num_palette = %d", *num_palette);
      return (PNG_INFO_PLTE);
   }

   return (0);
}

#ifdef PNG_sBIT_SUPPORTED
_png_uint_32 PNGAPI
_png_get_sBIT(_png_const_structp _png_ptr, _png_infop info_ptr,
    _png_color_8p *sig_bit)
{
   _png_debug1(1, "in %s retrieval function", "sBIT");

   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_sBIT)
       && sig_bit != NULL)
   {
      *sig_bit = &(info_ptr->sig_bit);
      return (PNG_INFO_sBIT);
   }

   return (0);
}
#endif

#ifdef PNG_TEXT_SUPPORTED
_png_uint_32 PNGAPI
_png_get_text(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_textp *text_ptr, int *num_text)
{
   if (_png_ptr != NULL && info_ptr != NULL && info_ptr->num_text > 0)
   {
      _png_debug1(1, "in 0x%lx retrieval function",
         (unsigned long)_png_ptr->chunk_name);

      if (text_ptr != NULL)
         *text_ptr = info_ptr->text;

      if (num_text != NULL)
         *num_text = info_ptr->num_text;

      return ((_png_uint_32)info_ptr->num_text);
   }

   if (num_text != NULL)
      *num_text = 0;

   return(0);
}
#endif

#ifdef PNG_tIME_SUPPORTED
_png_uint_32 PNGAPI
_png_get_tIME(_png_const_structp _png_ptr, _png_infop info_ptr, _png_timep *mod_time)
{
   _png_debug1(1, "in %s retrieval function", "tIME");

   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_tIME)
       && mod_time != NULL)
   {
      *mod_time = &(info_ptr->mod_time);
      return (PNG_INFO_tIME);
   }

   return (0);
}
#endif

#ifdef PNG_tRNS_SUPPORTED
_png_uint_32 PNGAPI
_png_get_tRNS(_png_const_structp _png_ptr, _png_infop info_ptr,
    _png_bytep *trans_alpha, int *num_trans, _png_color_16p *trans_color)
{
   _png_uint_32 retval = 0;
   if (_png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_tRNS))
   {
      _png_debug1(1, "in %s retrieval function", "tRNS");

      if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      {
         if (trans_alpha != NULL)
         {
            *trans_alpha = info_ptr->trans_alpha;
            retval |= PNG_INFO_tRNS;
         }

         if (trans_color != NULL)
            *trans_color = &(info_ptr->trans_color);
      }

      else /* if (info_ptr->color_type != PNG_COLOR_TYPE_PALETTE) */
      {
         if (trans_color != NULL)
         {
            *trans_color = &(info_ptr->trans_color);
            retval |= PNG_INFO_tRNS;
         }

         if (trans_alpha != NULL)
            *trans_alpha = NULL;
      }

      if (num_trans != NULL)
      {
         *num_trans = info_ptr->num_trans;
         retval |= PNG_INFO_tRNS;
      }
   }

   return (retval);
}
#endif

#ifdef PNG_UNKNOWN_CHUNKS_SUPPORTED
int PNGAPI
_png_get_unknown_chunks(_png_const_structp _png_ptr, _png_const_infop info_ptr,
    _png_unknown_chunkpp unknowns)
{
   if (_png_ptr != NULL && info_ptr != NULL && unknowns != NULL)
   {
      *unknowns = info_ptr->unknown_chunks;
      return info_ptr->unknown_chunks_num;
   }

   return (0);
}
#endif

#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
_png_byte PNGAPI
_png_get_rgb_to_gray_status (_png_const_structp _png_ptr)
{
   return (_png_byte)(_png_ptr ? _png_ptr->rgb_to_gray_status : 0);
}
#endif

#ifdef PNG_USER_CHUNKS_SUPPORTED
_png_voidp PNGAPI
_png_get_user_chunk_ptr(_png_const_structp _png_ptr)
{
   return (_png_ptr ? _png_ptr->user_chunk_ptr : NULL);
}
#endif

_png_size_t PNGAPI
_png_get_compression_buffer_size(_png_const_structp _png_ptr)
{
   return (_png_ptr ? _png_ptr->zbuf_size : 0);
}

#ifdef PNG_SET_USER_LIMITS_SUPPORTED
/* These functions were added to libpng 1.2.6 and were enabled
 * by default in libpng-1.4.0 */
_png_uint_32 PNGAPI
_png_get_user_width_max (_png_const_structp _png_ptr)
{
   return (_png_ptr ? _png_ptr->user_width_max : 0);
}

_png_uint_32 PNGAPI
_png_get_user_height_max (_png_const_structp _png_ptr)
{
   return (_png_ptr ? _png_ptr->user_height_max : 0);
}

/* This function was added to libpng 1.4.0 */
_png_uint_32 PNGAPI
_png_get_chunk_cache_max (_png_const_structp _png_ptr)
{
   return (_png_ptr ? _png_ptr->user_chunk_cache_max : 0);
}

/* This function was added to libpng 1.4.1 */
_png_alloc_size_t PNGAPI
_png_get_chunk_malloc_max (_png_const_structp _png_ptr)
{
   return (_png_ptr ? _png_ptr->user_chunk_malloc_max : 0);
}
#endif /* ?PNG_SET_USER_LIMITS_SUPPORTED */

/* These functions were added to libpng 1.4.0 */
#ifdef PNG_IO_STATE_SUPPORTED
_png_uint_32 PNGAPI
_png_get_io_state (_png_structp _png_ptr)
{
   return _png_ptr->io_state;
}

_png_uint_32 PNGAPI
_png_get_io_chunk_type (_png_const_structp _png_ptr)
{
   return _png_ptr->chunk_name;
}

_png_const_bytep PNGAPI
_png_get_io_chunk_name (_png_structp _png_ptr)
{
   PNG_CSTRING_FROM_CHUNK(_png_ptr->io_chunk_string, _png_ptr->chunk_name);
   return _png_ptr->io_chunk_string;
}
#endif /* ?PNG_IO_STATE_SUPPORTED */

#endif /* PNG_READ_SUPPORTED || PNG_WRITE_SUPPORTED */
