/* cairo - a vector graphics library with display and print output
 *
 * Copyright © 2008 Chris Wilson
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
 * The Initial Developer of the Original Code is Chris Wilson
 *
 * Contributor(s):
 *	Chris Wilson <chris@chris-wilson.co.uk>
 */

#ifndef CAIRO_SCRIPT_H
#define CAIRO_SCRIPT_H

#include "cairo.h"

#if CAIRO_HAS_SCRIPT_SURFACE

CAIRO_BEGIN_DECLS

cairo_public cairo_surface_t *
cairo_script_surface_create (const char		*filename,
			     double width,
			     double height);

cairo_public cairo_surface_t *
cairo_script_surface_create_for_stream (cairo_write_func_t	 write_func,
					void			*closure,
					double width,
					double height);

typedef enum {
    CAIRO_SCRIPT_MODE_BINARY,
    CAIRO_SCRIPT_MODE_ASCII
} cairo_script_mode_t;

cairo_public void
cairo_script_surface_set_mode (cairo_surface_t *surface,
			       cairo_script_mode_t mode);

cairo_public cairo_script_mode_t
cairo_script_surface_get_mode (cairo_surface_t *surface);

CAIRO_END_DECLS

#else  /*CAIRO_HAS_SCRIPT_SURFACE*/
# error Cairo was not compiled with support for the CairoScript backend
#endif /*CAIRO_HAS_SCRIPT_SURFACE*/

#endif /*CAIRO_SCRIPT_H*/
