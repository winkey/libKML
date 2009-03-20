/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */
 
#ifndef _LIBKML_H
#define _LIBKML_H

/*******************************************************************************
 kml info storage struct
 
 members:
										kmlfile			the full path of the kml file
										kmzfile			the full path of the kmz file
										buf					the buffer struct
*******************************************************************************/

#ifndef MAKING_KML_C

typedef void KML;

#endif

/*******************************************************************************
 function to create a new kml
 
 args:
								kmlfile			the full path of the kml file
								kmzfile			the full path of the kmz file
 
 returns:				pointer to the KML struct
*******************************************************************************/

KML *KML_new(
	char *kmlfile,
	char *kmzfile);

/*******************************************************************************
 function to free a kml struct
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_free(
	KML *kml);

/*******************************************************************************
 function to write a kml to disk
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_write(
	KML *kml);

/*******************************************************************************
 function to add a kml header to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_header (
	KML *kml);

/*******************************************************************************
 function to add a kml footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_footer (
	KML *kml);

/*******************************************************************************
 function to add a kml placemark header to a kml
 
 args:
								kml				pointer to the kml struct
								name			the name of the placemark or NULL for none
								desc			the description of a placemark or NULL for none
								styleid		the style id for the placemark
 
 returns:
								nothing
*******************************************************************************/

void KML_placemark_header (
	KML *kml,
	char *name,
	char *desc,
	char *styleid);

/*******************************************************************************
 function to add a placemark footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_placemark_footer (
	KML *kml);

/*******************************************************************************
 function to add a linestring header to a kml
 
 args:
								kml					pointer to the kml struct
								extrude			extruded? 0/1
								tessellate	tessellate? 0/1
 
 returns:
								nothing
*******************************************************************************/

void KML_linestring_header (
	KML *kml,
	int extrude,
	int tessellate);

/*******************************************************************************
 function to add a linestring footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_linestring_footer (
	KML *kml);

/*******************************************************************************
 function to add a polygon header to a kml
 
 args:
								kml				pointer to the kml struct
								extrude			extruded? 0/1
								tessellate	tessellate? 0/1
 
 returns:
								nothing
*******************************************************************************/

void KML_polygon_header (
	KML *kml,
	int extrude,
	int tessellate);

/*******************************************************************************
 function to add a polygon footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_polygon_footer (
	KML *kml);

/*******************************************************************************
 function to add coordinates of a point to a kml
 
 args:
								kml					pointer to the kml struct
								precision		number of places to the right of the point to print
								x						x coord of the point
								y						y coord of the point
								z						z coord of the point NULL if 2d
	
 returns:
								nothing
*******************************************************************************/

void KML_coordinates (
	KML *kml,
	int precision,												
	double *x,
	double *y,
	double *z);

/*******************************************************************************
 function to add a linestyle to a kml
 
 args:
								kml				pointer to the kml struct
								id				the style id
								rgb				rgb value for the style
								alpha			the alpha value for the style
								width			the line width
 
 returns:
								nothing
*******************************************************************************/

void KML_linestyle (
	KML *kml,
	char *id,
	char *rgb,
	char *alpha,
	int width);

/*******************************************************************************
 function to add a polystyle to a kml
 
 args:
								kml					pointer to the kml struct
								id					the style id
								linergb			rgb value for the line style
								linealpha		the alpha value for the line style
								width				the line width
								polyrgb			rgb value for the fill style
								polyalpha		the alpha value for the fill style
 
 returns:
								nothing
*******************************************************************************/

void KML_polystyle (
	KML *kml,
	char *id,
	char *linergb,
	char *linealpha,
	int width,
	char *polyrgb,
	char *polyalpha);




#endif /* _LIBKML_H */

