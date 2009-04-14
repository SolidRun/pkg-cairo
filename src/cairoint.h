/* cairo - a vector graphics library with display and print output
 *
 * Copyright © 2002 University of Southern California
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 *
 * The Original Code is the cairo graphics library.
 *
 * The Initial Developer of the Original Code is University of Southern
 * California.
 *
 * Contributor(s):
 *	Carl D. Worth <cworth@cworth.org>
 */

/*
 * These definitions are solely for use by the implementation of Cairo
 * and constitute no kind of standard.  If you need any of these
 * functions, please drop me a note.  Either the library needs new
 * functionality, or there's a way to do what you need using the
 * existing published interfaces. cworth@cworth.org
 */

#ifndef _CAIROINT_H_
#define _CAIROINT_H_

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

#include "cairo.h"

#if __GNUC__ >= 3 && defined(__ELF__)
# define slim_hidden_proto(name)	slim_hidden_proto1(name, INT_##name)
# define slim_hidden_def(name)		slim_hidden_def1(name, INT_##name)
# define slim_hidden_proto1(name, internal)				\
  extern __typeof (name) name						\
	__asm__ (slim_hidden_asmname (internal))			\
	cairo_private;
# define slim_hidden_def1(name, internal)				\
  extern __typeof (name) EXT_##name __asm__(slim_hidden_asmname(name))	\
	__attribute__((__alias__(slim_hidden_asmname(internal))))
# define slim_hidden_ulp		slim_hidden_ulp1(__USER_LABEL_PREFIX__)
# define slim_hidden_ulp1(x)		slim_hidden_ulp2(x)
# define slim_hidden_ulp2(x)		#x
# define slim_hidden_asmname(name)	slim_hidden_asmname1(name)
# define slim_hidden_asmname1(name)	slim_hidden_ulp #name
#else
# define slim_hidden_proto(name)
# define slim_hidden_def(name)
#endif

/* slim_internal.h */
#if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 3)) && defined(__ELF__)
#define cairo_private		__attribute__((__visibility__("hidden")))
#else
#define cairo_private
#endif

/* These macros allow us to deprecate a function by providing an alias
   for the old function name to the new function name. With this
   macro, binary compatibility is preserved. The macro only works on
   some platforms --- tough.

   Meanwhile, new definitions in the public header file break the
   source code so that it will no longer link against the old
   symbols. Instead it will give a descriptive error message
   indicating that the old function has been deprecated by the new
   function.  */
#if __GNUC__ >= 2 && defined(__ELF__)
# define DEPRECATE(old, new)			\
	extern __typeof (new) old		\
	__asm__ ("" #old)			\
	__attribute__((__alias__("" #new)))
#else
# define DEPRECATE(old, new)
#endif

#ifndef __GNUC__
#define __attribute__(x)
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#include "cairo-wideint.h"

typedef int32_t		cairo_fixed_16_16_t;
typedef cairo_int64_t	cairo_fixed_32_32_t;
typedef cairo_int64_t	cairo_fixed_48_16_t;
typedef cairo_int128_t	cairo_fixed_64_64_t;
typedef cairo_int128_t	cairo_fixed_96_32_t;

/* The common 16.16 format gets a shorter name */
typedef cairo_fixed_16_16_t cairo_fixed_t;

#define CAIRO_MAXSHORT SHRT_MAX
#define CAIRO_MINSHORT SHRT_MIN

typedef struct _cairo_point {
    cairo_fixed_t x;
    cairo_fixed_t y;
} cairo_point_t;

typedef struct _cairo_slope
{
    cairo_fixed_t dx;
    cairo_fixed_t dy;
} cairo_slope_t, cairo_distance_t;

typedef struct _cairo_point_double {
    double x;
    double y;
} cairo_point_double_t;

typedef struct _cairo_distance_double {
    double dx;
    double dy;
} cairo_distance_double_t;

typedef struct _cairo_line {
    cairo_point_t p1;
    cairo_point_t p2;
} cairo_line_t, cairo_box_t;

typedef struct _cairo_trapezoid {
    cairo_fixed_t top, bottom;
    cairo_line_t left, right;
} cairo_trapezoid_t;

typedef struct _cairo_rectangle_int {
    short x, y;
    unsigned short width, height;
} cairo_rectangle_t, cairo_glyph_size_t;

/* Sure wish C had a real enum type so that this would be distinct
   from cairo_status_t. Oh well, without that, I'll use this bogus 1000
   offset */
typedef enum cairo_int_status {
    CAIRO_INT_STATUS_DEGENERATE = 1000,
    CAIRO_INT_STATUS_UNSUPPORTED
} cairo_int_status_t;

#define CAIRO_OK(status) ((status) == CAIRO_STATUS_SUCCESS)

typedef enum cairo_path_op {
    CAIRO_PATH_OP_MOVE_TO = 0,
    CAIRO_PATH_OP_LINE_TO = 1,
    CAIRO_PATH_OP_CURVE_TO = 2,
    CAIRO_PATH_OP_CLOSE_PATH = 3
} __attribute__ ((packed)) cairo_path_op_t; /* Don't want 32 bits if we can avoid it. */

typedef enum cairo_direction {
    CAIRO_DIRECTION_FORWARD,
    CAIRO_DIRECTION_REVERSE
} cairo_direction_t;

#define CAIRO_PATH_BUF_SZ 64

typedef struct _cairo_path_op_buf {
    int num_ops;
    cairo_path_op_t op[CAIRO_PATH_BUF_SZ];

    struct _cairo_path_op_buf *next, *prev;
} cairo_path_op_buf_t;

typedef struct _cairo_path_arg_buf {
    int num_points;
    cairo_point_t points[CAIRO_PATH_BUF_SZ];

    struct _cairo_path_arg_buf *next, *prev;
} cairo_path_arg_buf_t;

typedef struct _cairo_path {
    cairo_path_op_buf_t *op_head;
    cairo_path_op_buf_t *op_tail;

    cairo_path_arg_buf_t *arg_head;
    cairo_path_arg_buf_t *arg_tail;

    cairo_point_t last_move_point;
    cairo_point_t current_point;
    int has_current_point;
} cairo_path_t;

typedef struct _cairo_edge {
    cairo_line_t edge;
    int clockWise;

    cairo_fixed_16_16_t current_x;
} cairo_edge_t;

typedef struct _cairo_polygon {
    int num_edges;
    int edges_size;
    cairo_edge_t *edges;

    cairo_point_t first_point;
    cairo_point_t current_point;
    int has_current_point;

    int closed;
} cairo_polygon_t;

typedef struct _cairo_spline {
    cairo_point_t a, b, c, d;

    cairo_slope_t initial_slope;
    cairo_slope_t final_slope;

    int num_points;
    int points_size;
    cairo_point_t *points;
} cairo_spline_t;

typedef struct _cairo_pen_vertex {
    cairo_point_t point;

    cairo_slope_t slope_ccw;
    cairo_slope_t slope_cw;
} cairo_pen_vertex_t;

typedef struct _cairo_pen {
    double radius;
    double tolerance;

    cairo_pen_vertex_t *vertices;
    int num_vertices;
} cairo_pen_t;

typedef struct _cairo_color cairo_color_t;
typedef struct _cairo_image_surface cairo_image_surface_t;

/* cairo_array.c structures and functions */ 

typedef struct _cairo_array cairo_array_t;
struct _cairo_array {
    int size;
    int num_elements;
    int element_size;
    char *elements;
};

cairo_private void
_cairo_array_init (cairo_array_t *array, int element_size);

cairo_private void
_cairo_array_fini (cairo_array_t *array);

cairo_private cairo_status_t
_cairo_array_grow_by (cairo_array_t *array, int additional);

cairo_private void
_cairo_array_truncate (cairo_array_t *array, int length);

cairo_private void *
_cairo_array_append (cairo_array_t *array,
		     const void *elements, int num_elements);

cairo_private void *
_cairo_array_index (cairo_array_t *array, int index);

cairo_private void
_cairo_array_copy_element (cairo_array_t *array, int index, void *dst);

cairo_private int
_cairo_array_num_elements (cairo_array_t *array);

/* cairo_cache.c structures and functions */ 

typedef struct _cairo_cache_backend {

    unsigned long	(*hash)			(void *cache,
						 void *key);

    int			(*keys_equal)		(void *cache,
						 void *k1, 
						 void *k2);
    
    cairo_status_t	(*create_entry)		(void *cache,
						 void *key,
						 void **entry_return);

    void		(*destroy_entry)	(void *cache,
						 void *entry);

    void		(*destroy_cache)	(void *cache);

} cairo_cache_backend_t;

/* 
 * The cairo_cache system makes the following assumptions about
 * entries in its cache:
 *
 *  - a pointer to an entry can be cast to a cairo_cache_entry_base_t.
 *  - a pointer to an entry can also be cast to the "key type".
 *
 * The practical effect of this is that your entries must be laid
 * out this way:
 *
 *    struct my_entry { 
 *      cairo_cache_entry_base_t;
 *      my_key_value_1;
 *      my_key_value_2;
 *      ...
 *      my_value;
 *    };
 */

typedef struct {
    unsigned long memory;
    unsigned long hashcode;
} cairo_cache_entry_base_t;

typedef struct {
    unsigned long high_water_mark;
    unsigned long size;
    unsigned long rehash;
} cairo_cache_arrangement_t;

#undef CAIRO_MEASURE_CACHE_PERFORMANCE

typedef struct {
    unsigned long refcount;
    const cairo_cache_backend_t *backend;
    const cairo_cache_arrangement_t *arrangement;
    cairo_cache_entry_base_t **entries;

    unsigned long max_memory;
    unsigned long used_memory;
    unsigned long live_entries;

#ifdef CAIRO_MEASURE_CACHE_PERFORMANCE
    unsigned long hits;
    unsigned long misses;
    unsigned long probes;
#endif
} cairo_cache_t;

cairo_private cairo_status_t
_cairo_cache_init (cairo_cache_t *cache,
		   const cairo_cache_backend_t *backend,
		   unsigned long max_memory);

cairo_private void
_cairo_cache_reference (cairo_cache_t *cache);

cairo_private void
_cairo_cache_destroy (cairo_cache_t *cache);

cairo_private cairo_status_t
_cairo_cache_lookup (cairo_cache_t *cache,
		     void          *key,
		     void         **entry_return,
		     int           *created_entry);

cairo_private cairo_status_t
_cairo_cache_remove (cairo_cache_t *cache,
		     void          *key);

cairo_private void *
_cairo_cache_random_entry (cairo_cache_t *cache,
			   int (*predicate) (void*));

cairo_private unsigned long
_cairo_hash_string (const char *c);

#define CAIRO_IMAGE_GLYPH_CACHE_MEMORY_DEFAULT 0x100000
#define CAIRO_XLIB_GLYPH_CACHE_MEMORY_DEFAULT 0x100000

typedef struct {
  double matrix[2][2];
} cairo_font_scale_t;

struct _cairo_font_backend;

/*
 * A cairo_unscaled_font_t is just an opaque handle we use in the
 * glyph cache.
 */
typedef struct {
    int refcount;
    const struct _cairo_font_backend *backend;
} cairo_unscaled_font_t;

struct _cairo_font {
    int refcount;
    cairo_font_scale_t scale;	/* font space => device space */
    const struct _cairo_font_backend *backend;
};

/* cairo_font.c is responsible for a global glyph cache: 
 *  
 *   - glyph entries: [[[base], cairo_unscaled_font_t, scale, flags, index],
 *                     image, size, extents]
 *
 * Surfaces may build their own glyph caches if they have surface-specific
 * glyph resources to maintain; those caches can feed off of the global
 * caches if need be (eg. cairo_xlib_surface.c does this).
 */

typedef struct {
    cairo_cache_entry_base_t base;
    cairo_unscaled_font_t *unscaled;
    cairo_font_scale_t scale;
    int flags;
    unsigned long index;
} cairo_glyph_cache_key_t;

typedef struct {
    cairo_glyph_cache_key_t key;
    cairo_image_surface_t *image;
    cairo_glyph_size_t size;    
    cairo_text_extents_t extents;
} cairo_image_glyph_cache_entry_t;

cairo_private void
_cairo_lock_global_image_glyph_cache (void);

cairo_private void
_cairo_unlock_global_image_glyph_cache (void);

cairo_private cairo_cache_t *
_cairo_get_global_image_glyph_cache (void);

/* Some glyph cache functions you can reuse. */

cairo_private unsigned long
_cairo_glyph_cache_hash (void *cache, void *key);

cairo_private int
_cairo_glyph_cache_keys_equal (void *cache,
			       void *k1,
			       void *k2);

/* the font backend interface */

typedef struct _cairo_font_backend {
    cairo_status_t (*create)         (const char		*family,
				      cairo_font_slant_t	slant,
				      cairo_font_weight_t	weight,
				      cairo_font_scale_t	*scale,
				      cairo_font_t              **font);
				    
    void (*destroy_font)              (void			*font);
    void (*destroy_unscaled_font)     (void			*font);

    cairo_status_t (*font_extents)   (void			*font,
				      cairo_font_extents_t	*extents);

    cairo_status_t (*text_to_glyphs) (void                      *font,
				      const unsigned char	*utf8,
				      cairo_glyph_t		**glyphs, 
				      int			*num_glyphs);

    cairo_status_t (*glyph_extents)  (void			*font,
				      cairo_glyph_t		*glyphs, 
				      int			num_glyphs,
				      cairo_text_extents_t	*extents);

    cairo_status_t (*glyph_bbox)    (void			*font,
				     const cairo_glyph_t	*glyphs,
				     int			num_glyphs,
				     cairo_box_t		*bbox);
  
    cairo_status_t (*show_glyphs)    (void			*font,
				      cairo_operator_t		operator,
				      cairo_pattern_t		*pattern,
				      cairo_surface_t     	*surface,
				      int                       source_x,
				      int                       source_y,
				      int			dest_x,
				      int			dest_y,
				      unsigned int		width,
				      unsigned int		height,
				      const cairo_glyph_t	*glyphs,
				      int			num_glyphs);
  
    cairo_status_t (*glyph_path)     (void			*font,
				      cairo_glyph_t		*glyphs, 
				      int			num_glyphs,
				      cairo_path_t		*path);
    void (*get_glyph_cache_key)      (void                      *font,
				      cairo_glyph_cache_key_t   *key);

    cairo_status_t (*create_glyph) (cairo_image_glyph_cache_entry_t *entry);

} cairo_font_backend_t;

/* concrete font backends */
#ifdef CAIRO_HAS_FT_FONT

extern const cairo_private struct _cairo_font_backend cairo_ft_font_backend;

#endif

#ifdef CAIRO_HAS_WIN32_FONT

extern const cairo_private struct _cairo_font_backend cairo_win32_font_backend;

#endif

#ifdef CAIRO_HAS_ATSUI_FONT

extern const cairo_private struct _cairo_font_backend cairo_atsui_font_backend;

#endif

typedef struct _cairo_surface_backend {
    cairo_surface_t *
    (*create_similar)		(void			*surface,
				 cairo_format_t		format,
				 int                    drawable,
				 int			width,
				 int			height);

    void
    (*destroy)			(void			*surface);

    double
    (*pixels_per_inch)		(void			*surface);

    cairo_status_t
    (* acquire_source_image)    (void                    *abstract_surface,
				 cairo_image_surface_t  **image_out,
				 void                   **image_extra);

    void
    (* release_source_image)    (void                   *abstract_surface,
				 cairo_image_surface_t  *image,
				 void                   *image_extra);

    cairo_status_t
    (*acquire_dest_image)       (void                   *abstract_surface,
				 cairo_rectangle_t       *interest_rect,
				 cairo_image_surface_t  **image_out,
				 cairo_rectangle_t       *image_rect,
				 void                   **image_extra);

    void
    (*release_dest_image)       (void                   *abstract_surface,
				 cairo_rectangle_t      *interest_rect,
				 cairo_image_surface_t  *image,
				 cairo_rectangle_t      *image_rect,
				 void                   *image_extra);
	
    cairo_status_t
    (*clone_similar)            (void                   *surface,
				 cairo_surface_t        *src,
				 cairo_surface_t       **clone_out);
				 
    /* XXX: dst should be the first argument for consistency */
    cairo_int_status_t
    (*composite)		(cairo_operator_t	operator,
				 cairo_pattern_t       	*src,
				 cairo_pattern_t	*mask,
				 void			*dst,
				 int			src_x,
				 int			src_y,
				 int			mask_x,
				 int			mask_y,
				 int			dst_x,
				 int			dst_y,
				 unsigned int		width,
				 unsigned int		height);

    cairo_int_status_t
    (*fill_rectangles)		(void			*surface,
				 cairo_operator_t	operator,
				 const cairo_color_t	*color,
				 cairo_rectangle_t	*rects,
				 int			num_rects);

    /* XXX: dst should be the first argument for consistency */
    cairo_int_status_t
    (*composite_trapezoids)	(cairo_operator_t	operator,
				 cairo_pattern_t	*pattern,
				 void			*dst,
				 int			src_x,
				 int			src_y,
				 int			dst_x,
				 int			dst_y,
				 unsigned int		width,
				 unsigned int		height,
				 cairo_trapezoid_t	*traps,
				 int			num_traps);

    cairo_int_status_t
    (*copy_page)		(void			*surface);

    cairo_int_status_t
    (*show_page)		(void			*surface);

    cairo_int_status_t
    (*set_clip_region)		(void			*surface,
				 pixman_region16_t	*region);
    /* 
     * This is an optional entry to let the surface manage its own glyph
     * resources. If null, the font will be asked to render against this
     * surface, using image surfaces as glyphs. 
     */    
    cairo_status_t 
    (*show_glyphs)		(cairo_font_t		        *font,
				 cairo_operator_t		operator,
				 cairo_pattern_t		*pattern,
				 void				*surface,
				 int				source_x,
				 int				source_y,
				 int				dest_x,
				 int				dest_y,
				 unsigned int			width,
				 unsigned int			height,
				 const cairo_glyph_t		*glyphs,
				 int				num_glyphs);
} cairo_surface_backend_t;

struct _cairo_matrix {
    double m[3][2];
};

typedef struct _cairo_format_masks {
    int bpp;
    unsigned long alpha_mask;
    unsigned long red_mask;
    unsigned long green_mask;
    unsigned long blue_mask;
} cairo_format_masks_t;

struct _cairo_surface {
    const cairo_surface_backend_t *backend;

    unsigned int ref_count;

    cairo_matrix_t matrix;
    cairo_filter_t filter;
    int repeat;
};

struct _cairo_image_surface {
    cairo_surface_t base;

    /* libic-specific fields */
    cairo_format_t format;
    char *data;
    int owns_data;

    int width;
    int height;
    int stride;
    int depth;

    pixman_image_t *pixman_image;
};

/* XXX: Right now, the cairo_color structure puts unpremultiplied
   color in the doubles and premultiplied color in the shorts. Yes,
   this is crazy insane, (but at least we don't export this
   madness). I'm still working on a cleaner API, but in the meantime,
   at least this does prevent precision loss in color when changing
   alpha. */
struct _cairo_color {
    double red;
    double green;
    double blue;
    double alpha;

    unsigned short red_short;
    unsigned short green_short;
    unsigned short blue_short;
    unsigned short alpha_short;
};

#define CAIRO_EXTEND_DEFAULT CAIRO_EXTEND_NONE
#define CAIRO_FILTER_DEFAULT CAIRO_FILTER_BEST

typedef enum {
    CAIRO_PATTERN_SOLID,
    CAIRO_PATTERN_SURFACE,
    CAIRO_PATTERN_LINEAR,
    CAIRO_PATTERN_RADIAL
} cairo_pattern_type_t;

typedef struct _cairo_color_stop {
    cairo_fixed_t offset;
    cairo_color_t color;
} cairo_color_stop_t;

struct _cairo_pattern {
    cairo_pattern_type_t type;
    unsigned int	 ref_count;
    cairo_matrix_t	 matrix;
    cairo_filter_t	 filter;
    cairo_extend_t	 extend;
    double		 alpha;
};

typedef struct _cairo_solid_pattern {
    cairo_pattern_t base;
    
    double red, green, blue;
} cairo_solid_pattern_t;

typedef struct _cairo_surface_pattern {
    cairo_pattern_t base;
    
    cairo_surface_t *surface;
} cairo_surface_pattern_t;

typedef struct _cairo_gradient_pattern {
    cairo_pattern_t base;
    
    cairo_color_stop_t *stops;
    int		       n_stops;
} cairo_gradient_pattern_t;

typedef struct _cairo_linear_pattern {
    cairo_gradient_pattern_t base;

    cairo_point_double_t point0;
    cairo_point_double_t point1;
} cairo_linear_pattern_t;

typedef struct _cairo_radial_pattern {
    cairo_gradient_pattern_t base;

    cairo_point_double_t center0;
    cairo_point_double_t center1;
    double		 radius0;
    double		 radius1;
} cairo_radial_pattern_t;

typedef union {
    cairo_gradient_pattern_t base;

    cairo_linear_pattern_t linear;
    cairo_radial_pattern_t radial;
} cairo_gradient_pattern_union_t;

typedef union {
    cairo_pattern_t base;
    
    cairo_solid_pattern_t	   solid;
    cairo_surface_pattern_t	   surface;
    cairo_gradient_pattern_union_t gradient;
} cairo_pattern_union_t;

typedef struct _cairo_surface_attributes {
    cairo_matrix_t matrix;
    cairo_extend_t extend;
    cairo_filter_t filter;
    int		   x_offset;
    int		   y_offset;
    cairo_bool_t   acquired;
    void	   *extra;
} cairo_surface_attributes_t;

typedef struct _cairo_traps {
    cairo_trapezoid_t *traps;
    int num_traps;
    int traps_size;
    cairo_box_t extents;
} cairo_traps_t;

#define CAIRO_FONT_SLANT_DEFAULT   CAIRO_FONT_SLANT_NORMAL
#define CAIRO_FONT_WEIGHT_DEFAULT  CAIRO_FONT_WEIGHT_NORMAL

#if defined (CAIRO_HAS_WIN32_FONT)

#define CAIRO_FONT_FAMILY_DEFAULT "Arial"
#define CAIRO_FONT_BACKEND_DEFAULT &cairo_win32_font_backend

#elif defined (CAIRO_HAS_ATSUI_FONT)

#define CAIRO_FONT_FAMILY_DEFAULT  "Monaco"
#define CAIRO_FONT_BACKEND_DEFAULT &cairo_atsui_font_backend

#elif defined (CAIRO_HAS_FT_FONT)

#define CAIRO_FONT_FAMILY_DEFAULT  "serif"
#define CAIRO_FONT_BACKEND_DEFAULT &cairo_ft_font_backend

#endif

#define CAIRO_GSTATE_OPERATOR_DEFAULT	CAIRO_OPERATOR_OVER
#define CAIRO_GSTATE_TOLERANCE_DEFAULT	0.1
#define CAIRO_GSTATE_FILL_RULE_DEFAULT	CAIRO_FILL_RULE_WINDING
#define CAIRO_GSTATE_LINE_WIDTH_DEFAULT	2.0
#define CAIRO_GSTATE_LINE_CAP_DEFAULT	CAIRO_LINE_CAP_BUTT
#define CAIRO_GSTATE_LINE_JOIN_DEFAULT	CAIRO_LINE_JOIN_MITER
#define CAIRO_GSTATE_MITER_LIMIT_DEFAULT	10.0
#define CAIRO_GSTATE_PIXELS_PER_INCH_DEFAULT	96.0

/* Need a name distinct from the cairo_clip function */
typedef struct _cairo_clip_rec {
    cairo_rectangle_t rect;
    pixman_region16_t *region;
    cairo_surface_t *surface;
} cairo_clip_rec_t;

typedef struct _cairo_gstate {
    cairo_operator_t operator;
    
    double tolerance;

    /* stroke style */
    double line_width;
    cairo_line_cap_t line_cap;
    cairo_line_join_t line_join;
    double miter_limit;

    cairo_fill_rule_t fill_rule;

    double *dash;
    int num_dashes;
    double dash_offset;

    char *font_family; /* NULL means CAIRO_FONT_FAMILY_DEFAULT; */
    cairo_font_slant_t font_slant; 
    cairo_font_weight_t font_weight;

    cairo_font_t *font;		/* Specific to the current CTM */

    cairo_surface_t *surface;

    cairo_pattern_t *pattern;
    double alpha;

    cairo_clip_rec_t clip;

    double pixels_per_inch;

    cairo_matrix_t font_matrix;

    cairo_matrix_t ctm;
    cairo_matrix_t ctm_inverse;

    cairo_path_t path;

    cairo_pen_t pen_regular;

    struct _cairo_gstate *next;
} cairo_gstate_t;

struct _cairo {
    unsigned int ref_count;
    cairo_gstate_t *gstate;
    cairo_status_t status;
};

typedef struct _cairo_stroke_face {
    cairo_point_t ccw;
    cairo_point_t point;
    cairo_point_t cw;
    cairo_slope_t dev_vector;
    cairo_point_double_t usr_vector;
} cairo_stroke_face_t;

/* cairo.c */
cairo_private void
_cairo_restrict_value (double *value, double min, double max);

/* cairo_fixed.c */
cairo_private cairo_fixed_t
_cairo_fixed_from_int (int i);

cairo_private cairo_fixed_t
_cairo_fixed_from_double (double d);

cairo_private cairo_fixed_t
_cairo_fixed_from_26_6 (uint32_t i);

cairo_private double
_cairo_fixed_to_double (cairo_fixed_t f);

cairo_private int
_cairo_fixed_is_integer (cairo_fixed_t f);

cairo_private int
_cairo_fixed_integer_part (cairo_fixed_t f);

cairo_private int
_cairo_fixed_integer_floor (cairo_fixed_t f);

cairo_private int
_cairo_fixed_integer_ceil (cairo_fixed_t f);


/* cairo_gstate.c */
cairo_private cairo_gstate_t *
_cairo_gstate_create (void);

cairo_private cairo_status_t
_cairo_gstate_init (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_init_copy (cairo_gstate_t *gstate, cairo_gstate_t *other);

cairo_private void
_cairo_gstate_fini (cairo_gstate_t *gstate);

cairo_private void
_cairo_gstate_destroy (cairo_gstate_t *gstate);

cairo_private cairo_gstate_t *
_cairo_gstate_clone (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_copy (cairo_gstate_t *dest, cairo_gstate_t *src);

cairo_private cairo_status_t
_cairo_gstate_begin_group (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_end_group (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_set_target_surface (cairo_gstate_t *gstate, cairo_surface_t *surface);

cairo_private cairo_surface_t *
_cairo_gstate_current_target_surface (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_set_pattern (cairo_gstate_t *gstate, cairo_pattern_t *pattern);

cairo_private cairo_pattern_t *
_cairo_gstate_current_pattern (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_set_operator (cairo_gstate_t *gstate, cairo_operator_t operator);

cairo_private cairo_operator_t
_cairo_gstate_current_operator (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_set_rgb_color (cairo_gstate_t *gstate, double red, double green, double blue);

cairo_private cairo_status_t
_cairo_gstate_current_rgb_color (cairo_gstate_t *gstate,
				 double *red,
				 double *green,
				 double *blue);

cairo_private cairo_status_t
_cairo_gstate_set_tolerance (cairo_gstate_t *gstate, double tolerance);

cairo_private double
_cairo_gstate_current_tolerance (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_set_alpha (cairo_gstate_t *gstate, double alpha);

cairo_private double
_cairo_gstate_current_alpha (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_set_fill_rule (cairo_gstate_t *gstate, cairo_fill_rule_t fill_rule);

cairo_private cairo_fill_rule_t
_cairo_gstate_current_fill_rule (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_set_line_width (cairo_gstate_t *gstate, double width);

cairo_private double
_cairo_gstate_current_line_width (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_set_line_cap (cairo_gstate_t *gstate, cairo_line_cap_t line_cap);

cairo_private cairo_line_cap_t
_cairo_gstate_current_line_cap (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_set_line_join (cairo_gstate_t *gstate, cairo_line_join_t line_join);

cairo_private cairo_line_join_t
_cairo_gstate_current_line_join (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_set_dash (cairo_gstate_t *gstate, double *dash, int num_dashes, double offset);

cairo_private cairo_status_t
_cairo_gstate_set_miter_limit (cairo_gstate_t *gstate, double limit);

cairo_private double
_cairo_gstate_current_miter_limit (cairo_gstate_t *gstate);

cairo_private void
_cairo_gstate_current_matrix (cairo_gstate_t *gstate, cairo_matrix_t *matrix);

cairo_private cairo_status_t
_cairo_gstate_translate (cairo_gstate_t *gstate, double tx, double ty);

cairo_private cairo_status_t
_cairo_gstate_scale (cairo_gstate_t *gstate, double sx, double sy);

cairo_private cairo_status_t
_cairo_gstate_rotate (cairo_gstate_t *gstate, double angle);

cairo_private cairo_status_t
_cairo_gstate_concat_matrix (cairo_gstate_t *gstate,
			     cairo_matrix_t *matrix);

cairo_private cairo_status_t
_cairo_gstate_set_matrix (cairo_gstate_t *gstate,
			  cairo_matrix_t *matrix);

cairo_private cairo_status_t
_cairo_gstate_default_matrix (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_identity_matrix (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_transform_point (cairo_gstate_t *gstate, double *x, double *y);

cairo_private cairo_status_t
_cairo_gstate_transform_distance (cairo_gstate_t *gstate, double *dx, double *dy);

cairo_private cairo_status_t
_cairo_gstate_inverse_transform_point (cairo_gstate_t *gstate, double *x, double *y);

cairo_private cairo_status_t
_cairo_gstate_inverse_transform_distance (cairo_gstate_t *gstate, double *dx, double *dy);

cairo_private cairo_status_t
_cairo_gstate_new_path (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_move_to (cairo_gstate_t *gstate, double x, double y);

cairo_private cairo_status_t
_cairo_gstate_line_to (cairo_gstate_t *gstate, double x, double y);

cairo_private cairo_status_t
_cairo_gstate_curve_to (cairo_gstate_t *gstate,
			double x1, double y1,
			double x2, double y2,
			double x3, double y3);

cairo_private cairo_status_t
_cairo_gstate_arc (cairo_gstate_t *gstate,
		   double xc, double yc,
		   double radius,
		   double angle1, double angle2);

cairo_private cairo_status_t
_cairo_gstate_arc_negative (cairo_gstate_t *gstate,
			    double xc, double yc,
			    double radius,
			    double angle1, double angle2);

cairo_private cairo_status_t
_cairo_gstate_rel_move_to (cairo_gstate_t *gstate, double dx, double dy);

cairo_private cairo_status_t
_cairo_gstate_rel_line_to (cairo_gstate_t *gstate, double dx, double dy);

cairo_private cairo_status_t
_cairo_gstate_rel_curve_to (cairo_gstate_t *gstate,
			    double dx1, double dy1,
			    double dx2, double dy2,
			    double dx3, double dy3);

/* XXX: NYI
cairo_private cairo_status_t
_cairo_gstate_stroke_path (cairo_gstate_t *gstate);
*/

cairo_private cairo_status_t
_cairo_gstate_close_path (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_current_point (cairo_gstate_t *gstate, double *x, double *y);

cairo_private cairo_status_t
_cairo_gstate_interpret_path (cairo_gstate_t		*gstate,
			      cairo_move_to_func_t	*move_to,
			      cairo_line_to_func_t	*line_to,
			      cairo_curve_to_func_t	*curve_to,
			      cairo_close_path_func_t	*close_path,
			      void			*closure);

cairo_private cairo_status_t
_cairo_gstate_stroke (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_fill (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_copy_page (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_show_page (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_stroke_extents (cairo_gstate_t *gstate,
                              double *x1, double *y1,
			      double *x2, double *y2);

cairo_private cairo_status_t
_cairo_gstate_fill_extents (cairo_gstate_t *gstate,
                            double *x1, double *y1,
			    double *x2, double *y2);

cairo_private cairo_status_t
_cairo_gstate_in_stroke (cairo_gstate_t	*gstate,
			 double		x,
			 double		y,
			 cairo_bool_t	*inside_ret);

cairo_private cairo_status_t
_cairo_gstate_in_fill (cairo_gstate_t	*gstate,
		       double		x,
		       double		y,
		       cairo_bool_t	*inside_ret);

cairo_private cairo_status_t
_cairo_gstate_init_clip (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_clip (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_restore_external_state (cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_gstate_show_surface (cairo_gstate_t	*gstate,
			    cairo_surface_t	*surface,
			    int			width,
			    int			height);

cairo_private cairo_status_t
_cairo_gstate_select_font (cairo_gstate_t *gstate, 
			   const char *family, 
			   cairo_font_slant_t slant, 
			   cairo_font_weight_t weight);

cairo_private cairo_status_t
_cairo_gstate_scale_font (cairo_gstate_t *gstate, 
			  double scale);

cairo_private void
_cairo_gstate_current_font_scale (cairo_gstate_t     *gstate,
				  cairo_font_scale_t *sc);
    
cairo_private cairo_status_t
_cairo_gstate_transform_font (cairo_gstate_t *gstate, 
			      cairo_matrix_t *matrix);

cairo_private cairo_status_t
_cairo_gstate_current_font (cairo_gstate_t *gstate, 
			    cairo_font_t **font);

cairo_private void
_cairo_gstate_set_font_transform (cairo_gstate_t *gstate, 
				  cairo_matrix_t *matrix);

cairo_private void
_cairo_gstate_current_font_transform (cairo_gstate_t *gstate,
				      cairo_matrix_t *matrix);

cairo_private cairo_status_t
_cairo_gstate_current_font_extents (cairo_gstate_t *gstate, 
				    cairo_font_extents_t *extents);

cairo_private cairo_status_t
_cairo_gstate_set_font (cairo_gstate_t *gstate, 
			cairo_font_t *font);

cairo_private cairo_status_t
_cairo_gstate_text_to_glyphs (cairo_gstate_t *font,
			      const unsigned char *utf8, 
			      cairo_glyph_t **glyphs, 
			      int *num_glyphs);

cairo_private cairo_status_t
_cairo_gstate_glyph_extents (cairo_gstate_t *gstate,
			     cairo_glyph_t *glyphs, 
			     int num_glyphs,
			     cairo_text_extents_t *extents);

cairo_private cairo_status_t
_cairo_gstate_show_glyphs (cairo_gstate_t *gstate, 
			   cairo_glyph_t *glyphs, 
			   int num_glyphs);

cairo_private cairo_status_t
_cairo_gstate_glyph_path (cairo_gstate_t *gstate, 
			  cairo_glyph_t *glyphs, 
			  int num_glyphs);


/* cairo_color.c */
cairo_private void
_cairo_color_init (cairo_color_t *color);

cairo_private void
_cairo_color_fini (cairo_color_t *color);

cairo_private void
_cairo_color_set_rgb (cairo_color_t *color, double red, double green, double blue);

cairo_private void
_cairo_color_get_rgb (const cairo_color_t *color,
		      double *red, double *green, double *blue);

cairo_private void
_cairo_color_set_alpha (cairo_color_t *color, double alpha);

/* cairo_font.c */

cairo_private cairo_status_t
_cairo_font_create (const char           *family, 
		    cairo_font_slant_t   slant, 
		    cairo_font_weight_t  weight,
		    cairo_font_scale_t   *sc,
		    cairo_font_t         **font);

cairo_private void
_cairo_font_init (cairo_font_t 		     *font, 
		  cairo_font_scale_t 	     *scale,
		  const cairo_font_backend_t *backend);

cairo_private void
_cairo_unscaled_font_init (cairo_unscaled_font_t	    *font, 
			   const struct _cairo_font_backend *backend);

cairo_private void
_cairo_unscaled_font_reference (cairo_unscaled_font_t *font);

cairo_private void
_cairo_unscaled_font_destroy (cairo_unscaled_font_t *font);

cairo_private cairo_status_t
_cairo_font_font_extents (cairo_font_t 	       *font, 
			  cairo_font_extents_t *extents);

cairo_private cairo_status_t
_cairo_font_text_to_glyphs (cairo_font_t	 *font,
			    const unsigned char  *utf8, 
			    cairo_glyph_t 	**glyphs, 
			    int 		 *num_glyphs);

cairo_private cairo_status_t
_cairo_font_glyph_extents (cairo_font_t	        *font,
			   cairo_glyph_t 	*glyphs,
			   int 			num_glyphs,
			   cairo_text_extents_t *extents);

cairo_private cairo_status_t
_cairo_font_glyph_bbox (cairo_font_t          *font,
			cairo_glyph_t         *glyphs,
			int                   num_glyphs,
			cairo_box_t	      *bbox);

cairo_private cairo_status_t
_cairo_font_show_glyphs (cairo_font_t		*font,
			 cairo_operator_t	operator,
			 cairo_pattern_t	*source,
			 cairo_surface_t	*surface,
			 int			source_x,
			 int			source_y,
			 int			dest_x,
			 int			dest_y,
			 unsigned int		widht,
			 unsigned int		height,
			 cairo_glyph_t		*glyphs,
			 int			num_glyphs);

cairo_private cairo_status_t
_cairo_font_glyph_path (cairo_font_t        *font,
			cairo_glyph_t       *glyphs, 
			int                 num_glyphs,
			cairo_path_t        *path);

cairo_private cairo_status_t
_cairo_font_glyph_path (cairo_font_t        *font,
			cairo_glyph_t       *glyphs, 
			int                 num_glyphs,
			cairo_path_t        *path);

cairo_private void
_cairo_font_get_glyph_cache_key (cairo_font_t            *font,
				 cairo_glyph_cache_key_t *key);

/* cairo_hull.c */
cairo_private cairo_status_t
_cairo_hull_compute (cairo_pen_vertex_t *vertices, int *num_vertices);

/* cairo_path.c */
cairo_private void
_cairo_path_init (cairo_path_t *path);

cairo_private cairo_status_t
_cairo_path_init_copy (cairo_path_t *path, cairo_path_t *other);

cairo_private void
_cairo_path_fini (cairo_path_t *path);

cairo_private cairo_status_t
_cairo_path_move_to (cairo_path_t *path, cairo_point_t *point);

cairo_private cairo_status_t
_cairo_path_rel_move_to (cairo_path_t *path, cairo_slope_t *slope);

cairo_private cairo_status_t
_cairo_path_line_to (cairo_path_t *path, cairo_point_t *point);

cairo_private cairo_status_t
_cairo_path_rel_line_to (cairo_path_t *path, cairo_slope_t *slope);

cairo_private cairo_status_t
_cairo_path_curve_to (cairo_path_t *path,
		      cairo_point_t *p0,
		      cairo_point_t *p1,
		      cairo_point_t *p2);

cairo_private cairo_status_t
_cairo_path_rel_curve_to (cairo_path_t *path,
			  cairo_slope_t *s0,
			  cairo_slope_t *s1,
			  cairo_slope_t *s2);

cairo_private cairo_status_t
_cairo_path_close_path (cairo_path_t *path);

cairo_private cairo_status_t
_cairo_path_current_point (cairo_path_t *path, cairo_point_t *point);

typedef cairo_status_t (cairo_path_move_to_func_t) (void *closure,
						    cairo_point_t *point);

typedef cairo_status_t (cairo_path_line_to_func_t) (void *closure,
						    cairo_point_t *point);

typedef cairo_status_t (cairo_path_curve_to_func_t) (void *closure,
						     cairo_point_t *p0,
						     cairo_point_t *p1,
						     cairo_point_t *p2);

typedef cairo_status_t (cairo_path_close_path_func_t) (void *closure);

cairo_private cairo_status_t
_cairo_path_interpret (cairo_path_t			*path,
		       cairo_direction_t		dir,
		       cairo_path_move_to_func_t	*move_to,
		       cairo_path_line_to_func_t	*line_to,
		       cairo_path_curve_to_func_t	*curve_to,
		       cairo_path_close_path_func_t	*close_path,
		       void				*closure);

cairo_private cairo_status_t
_cairo_path_bounds (cairo_path_t *path, double *x1, double *y1, double *x2, double *y2);

/* cairo_path_fill.c */
cairo_private cairo_status_t
_cairo_path_fill_to_traps (cairo_path_t *path, cairo_gstate_t *gstate, cairo_traps_t *traps);

/* cairo_path_stroke.c */
cairo_private cairo_status_t
_cairo_path_stroke_to_traps (cairo_path_t *path, cairo_gstate_t *gstate, cairo_traps_t *traps);

/* cairo_surface.c */
cairo_private cairo_surface_t *
_cairo_surface_create_similar_scratch (cairo_surface_t	*other,
				       cairo_format_t	format,
				       int		drawable,
				       int		width,
				       int		height);

cairo_private cairo_surface_t *
_cairo_surface_create_similar_solid (cairo_surface_t	*other,
				     cairo_format_t	format,
				     int		width,
				     int		height,
				     cairo_color_t	*color);

cairo_private void
_cairo_surface_init (cairo_surface_t			*surface,
		     const cairo_surface_backend_t	*backend);

cairo_private cairo_status_t
_cairo_surface_fill_rectangle (cairo_surface_t	*surface,
			       cairo_operator_t	operator,
			       cairo_color_t	*color,
			       int		x,
			       int		y,
			       int		width,
			       int		height);

cairo_private cairo_status_t
_cairo_surface_composite (cairo_operator_t	operator,
			  cairo_pattern_t	*src,
			  cairo_pattern_t	*mask,
			  cairo_surface_t	*dst,
			  int			src_x,
			  int			src_y,
			  int			mask_x,
			  int			mask_y,
			  int			dst_x,
			  int			dst_y,
			  unsigned int		width,
			  unsigned int		height);

cairo_private cairo_status_t
_cairo_surface_fill_rectangles (cairo_surface_t		*surface,
				cairo_operator_t	operator,
				const cairo_color_t	*color,
				cairo_rectangle_t	*rects,
				int			num_rects);

cairo_private cairo_status_t
_cairo_surface_composite_trapezoids (cairo_operator_t	operator,
				     cairo_pattern_t	*pattern,
				     cairo_surface_t	*dst,
				     int		src_x,
				     int		src_y,
				     int		dst_x,
				     int		dst_y,
				     unsigned int	width,
				     unsigned int	height,
				     cairo_trapezoid_t	*traps,
				     int		ntraps);

cairo_private cairo_status_t
_cairo_surface_copy_page (cairo_surface_t *surface);

cairo_private cairo_status_t
_cairo_surface_show_page (cairo_surface_t *surface);

cairo_private double
_cairo_surface_pixels_per_inch (cairo_surface_t *surface);

cairo_private cairo_status_t
_cairo_surface_acquire_source_image (cairo_surface_t         *surface,
				     cairo_image_surface_t  **image_out,
				     void                   **image_extra);

cairo_private void
_cairo_surface_release_source_image (cairo_surface_t        *surface,
				     cairo_image_surface_t  *image,
				     void                   *image_extra);

cairo_private cairo_status_t
_cairo_surface_acquire_dest_image (cairo_surface_t         *surface,
				   cairo_rectangle_t       *interest_rect,
				   cairo_image_surface_t  **image_out,
				   cairo_rectangle_t       *image_rect,
				   void                   **image_extra);

cairo_private void
_cairo_surface_release_dest_image (cairo_surface_t        *surface,
				   cairo_rectangle_t      *interest_rect,
				   cairo_image_surface_t  *image,
				   cairo_rectangle_t      *image_rect,
				   void                   *image_extra);
    
cairo_private cairo_status_t
_cairo_surface_clone_similar (cairo_surface_t  *surface,
			      cairo_surface_t  *src,
			      cairo_surface_t **clone_out);

cairo_private cairo_status_t
_cairo_surface_set_clip_region (cairo_surface_t *surface, pixman_region16_t *region);

/* cairo_image_surface.c */

cairo_private cairo_image_surface_t *
_cairo_image_surface_create_with_masks (char			*data,
					cairo_format_masks_t	*format,
					int			width,
					int			height,
					int			stride);

cairo_private void
_cairo_image_surface_assume_ownership_of_data (cairo_image_surface_t *surface);

cairo_private cairo_status_t
_cairo_image_surface_set_matrix (cairo_image_surface_t	*surface,
				 cairo_matrix_t		*matrix);

cairo_private cairo_status_t
_cairo_image_surface_set_filter (cairo_image_surface_t	*surface,
				 cairo_filter_t		filter);

cairo_private cairo_status_t
_cairo_image_surface_set_repeat (cairo_image_surface_t	*surface,
				 int			repeat);

cairo_private cairo_int_status_t
_cairo_image_surface_set_clip_region (cairo_image_surface_t *surface,
				      pixman_region16_t *region);

cairo_private int
_cairo_surface_is_image (cairo_surface_t *surface);

/* cairo_pen.c */
cairo_private cairo_status_t
_cairo_pen_init (cairo_pen_t *pen, double radius, cairo_gstate_t *gstate);

cairo_private cairo_status_t
_cairo_pen_init_empty (cairo_pen_t *pen);

cairo_private cairo_status_t
_cairo_pen_init_copy (cairo_pen_t *pen, cairo_pen_t *other);

cairo_private void
_cairo_pen_fini (cairo_pen_t *pen);

cairo_private cairo_status_t
_cairo_pen_add_points (cairo_pen_t *pen, cairo_point_t *point, int num_points);

cairo_private cairo_status_t
_cairo_pen_add_points_for_slopes (cairo_pen_t *pen,
				  cairo_point_t *a,
				  cairo_point_t *b,
				  cairo_point_t *c,
				  cairo_point_t *d);

cairo_private cairo_status_t
_cairo_pen_find_active_cw_vertex_index (cairo_pen_t *pen,
					cairo_slope_t *slope,
					int *active);

cairo_private cairo_status_t
_cairo_pen_find_active_ccw_vertex_index (cairo_pen_t *pen,
					 cairo_slope_t *slope,
					 int *active);

cairo_private cairo_status_t
_cairo_pen_stroke_spline (cairo_pen_t *pen,
			  cairo_spline_t *spline,
			  double tolerance,
			  cairo_traps_t *traps);

/* cairo_polygon.c */
cairo_private void
_cairo_polygon_init (cairo_polygon_t *polygon);

cairo_private void
_cairo_polygon_fini (cairo_polygon_t *polygon);

cairo_private cairo_status_t
_cairo_polygon_add_edge (cairo_polygon_t *polygon, cairo_point_t *p1, cairo_point_t *p2);

cairo_private cairo_status_t
_cairo_polygon_move_to (cairo_polygon_t *polygon, cairo_point_t *point);

cairo_private cairo_status_t
_cairo_polygon_line_to (cairo_polygon_t *polygon, cairo_point_t *point);

cairo_private cairo_status_t
_cairo_polygon_close (cairo_polygon_t *polygon);

/* cairo_spline.c */
cairo_private cairo_int_status_t
_cairo_spline_init (cairo_spline_t *spline,
		    cairo_point_t *a,
		    cairo_point_t *b,
		    cairo_point_t *c,
		    cairo_point_t *d);

cairo_private cairo_status_t
_cairo_spline_decompose (cairo_spline_t *spline, double tolerance);

cairo_private void
_cairo_spline_fini (cairo_spline_t *spline);

/* cairo_matrix.c */
cairo_private void
_cairo_matrix_init (cairo_matrix_t *matrix);

cairo_private void
_cairo_matrix_fini (cairo_matrix_t *matrix);

cairo_private cairo_status_t
_cairo_matrix_set_translate (cairo_matrix_t *matrix,
			     double tx, double ty);

cairo_private cairo_status_t
_cairo_matrix_set_scale (cairo_matrix_t *matrix,
			 double sx, double sy);

cairo_private cairo_status_t
_cairo_matrix_set_rotate (cairo_matrix_t *matrix,
			  double angle);

cairo_private cairo_status_t
_cairo_matrix_transform_bounding_box (cairo_matrix_t *matrix,
				      double *x, double *y,
				      double *width, double *height);

cairo_private cairo_status_t
_cairo_matrix_compute_determinant (cairo_matrix_t *matrix, double *det);

cairo_private cairo_status_t
_cairo_matrix_compute_eigen_values (cairo_matrix_t *matrix, double *lambda1, double *lambda2);

cairo_private cairo_status_t
_cairo_matrix_compute_scale_factors (cairo_matrix_t *matrix, double *sx, double *sy, int x_major);

cairo_private cairo_bool_t
_cairo_matrix_is_integer_translation(cairo_matrix_t *matrix, int *itx, int *ity);

/* cairo_traps.c */
cairo_private void
_cairo_traps_init (cairo_traps_t *traps);

cairo_private void
_cairo_traps_fini (cairo_traps_t *traps);

cairo_private cairo_status_t
_cairo_traps_tessellate_triangle (cairo_traps_t *traps, cairo_point_t t[3]);

cairo_private cairo_status_t
_cairo_traps_tessellate_rectangle (cairo_traps_t *traps, cairo_point_t q[4]);

cairo_private cairo_status_t
_cairo_traps_tessellate_polygon (cairo_traps_t *traps,
				 cairo_polygon_t *poly,
				 cairo_fill_rule_t fill_rule);

cairo_private int
_cairo_traps_contain (cairo_traps_t *traps, double x, double y);

cairo_private void
_cairo_traps_extents (cairo_traps_t *traps, cairo_box_t *extents);

/* cairo_slope.c */
cairo_private void
_cairo_slope_init (cairo_slope_t *slope, cairo_point_t *a, cairo_point_t *b);

cairo_private int
_cairo_slope_compare (cairo_slope_t *a, cairo_slope_t *b);

cairo_private int
_cairo_slope_clockwise (cairo_slope_t *a, cairo_slope_t *b);

cairo_private int
_cairo_slope_counter_clockwise (cairo_slope_t *a, cairo_slope_t *b);

/* cairo_pattern.c */

cairo_private cairo_status_t
_cairo_pattern_init_copy (cairo_pattern_t *pattern, cairo_pattern_t *other);

cairo_private void
_cairo_pattern_init_solid (cairo_solid_pattern_t *pattern,
			   double red, double green, double blue);

cairo_private void
_cairo_pattern_init_for_surface (cairo_surface_pattern_t *pattern,
				 cairo_surface_t *surface);

cairo_private void
_cairo_pattern_init_linear (cairo_linear_pattern_t *pattern,
			    double x0, double y0, double x1, double y1);

cairo_private void
_cairo_pattern_init_radial (cairo_radial_pattern_t *pattern,
			    double cx0, double cy0, double radius0,
			    double cx1, double cy1, double radius1);

cairo_private void
_cairo_pattern_fini (cairo_pattern_t *pattern);

cairo_private cairo_pattern_t *
_cairo_pattern_create_solid (double red, double green, double blue);

cairo_private cairo_status_t
_cairo_pattern_get_rgb (cairo_pattern_t *pattern,
			double *red, double *green, double *blue);

cairo_private void
_cairo_pattern_set_alpha (cairo_pattern_t *pattern, double alpha);

cairo_private void
_cairo_pattern_transform (cairo_pattern_t *pattern,
			  cairo_matrix_t *ctm_inverse);

cairo_private cairo_bool_t 
_cairo_pattern_is_opaque (cairo_pattern_t *pattern);

cairo_private cairo_int_status_t
_cairo_pattern_acquire_surface (cairo_pattern_t		   *pattern,
				cairo_surface_t		   *dst,
				int			   x,
				int			   y,
				unsigned int		   width,
				unsigned int		   height,
				cairo_surface_t		   **surface_out,
				cairo_surface_attributes_t *attributes);

cairo_private void
_cairo_pattern_release_surface (cairo_surface_t		   *dst,
				cairo_surface_t		   *surface,
				cairo_surface_attributes_t *attributes);

cairo_private cairo_int_status_t
_cairo_pattern_acquire_surfaces (cairo_pattern_t	    *src,
				 cairo_pattern_t	    *mask,
				 cairo_surface_t	    *dst,
				 int			    src_x,
				 int			    src_y,
				 int			    mask_x,
				 int			    mask_y,
				 unsigned int		    width,
				 unsigned int		    height,
				 cairo_surface_t	    **src_out,
				 cairo_surface_t	    **mask_out,
				 cairo_surface_attributes_t *src_attributes,
				 cairo_surface_attributes_t *mask_attributes);


/* cairo_unicode.c */

cairo_private cairo_status_t
_cairo_utf8_to_ucs4 (const char  *str,
		     int          len,
		     uint32_t   **result,
		     int         *items_written);

cairo_private cairo_status_t
_cairo_utf8_to_utf16 (const char  *str,
		      int          len,
		      uint16_t   **result,
		      int         *items_written);

/* Avoid unnecessary PLT entries.  */

slim_hidden_proto(cairo_close_path)
slim_hidden_proto(cairo_matrix_copy)
slim_hidden_proto(cairo_matrix_invert)
slim_hidden_proto(cairo_matrix_multiply)
slim_hidden_proto(cairo_matrix_scale)
slim_hidden_proto(cairo_matrix_set_affine)
slim_hidden_proto(cairo_matrix_set_identity)
slim_hidden_proto(cairo_matrix_transform_distance)
slim_hidden_proto(cairo_matrix_transform_point)
slim_hidden_proto(cairo_move_to)
slim_hidden_proto(cairo_rel_line_to)
slim_hidden_proto(cairo_restore)
slim_hidden_proto(cairo_save)
slim_hidden_proto(cairo_set_target_surface)
slim_hidden_proto(cairo_surface_create_for_image)
slim_hidden_proto(cairo_surface_destroy)
slim_hidden_proto(cairo_surface_get_matrix)
slim_hidden_proto(cairo_surface_set_matrix)
slim_hidden_proto(cairo_surface_set_repeat)

#endif
