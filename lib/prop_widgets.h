/* Dia -- a diagram creation/manipulation program -*- c -*-
 * Copyright (C) 1998 Alexander Larsson
 *
 * Property system for dia objects/shapes.
 * Copyright (C) 2000 James Henstridge
 * Copyright (C) 2001 Cyrille Chepelov
 * Major restructuration done in August 2001 by C. Chepelov
 *
 * Property types for "widget" types (static, buttons, notebooks, frames, etc.)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#ifndef PROP_WIDGETS_H
#define PROP_WIDGETS_H

#include "properties.h"
#include "dia_xml.h"
#include "text.h"


typedef struct {
  Property common;
} StaticProperty;

typedef struct {
  Property common;
} ButtonProperty;

typedef struct {
  Property common;
  gboolean collapsible;
  gboolean collapsed;
} FrameProperty;

typedef struct {
  Property common;
} MulticolProperty;

typedef struct {
  Property common;
} NotebookProperty;

typedef struct {
  Property common;
  gint selected;
  GPtrArray *lines;
  gint w_selected;
} ListProperty;

void prop_widgets_register(void);

#endif
