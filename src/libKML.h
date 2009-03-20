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

enum {
	clampToGround,
	relativeToGround,
	absolute
} KML_altitudeModeEnum;

#ifndef MAKING_KML_C

typedef void KMZ;
typedef void KML;

#endif

/*****************************************************************************//**
 function to create a new kmz
 
 @param	kmzfile			the full path of the kmz file
 
 @return	pointer to the KMZ struct
*******************************************************************************/

KMZ *KMZ_new(
	char *kmzfile);

/*****************************************************************************//**
 function to create a new kml
 
 @param kmz					pointer to the kmz to add it to
 @param kmlfile			the full path of the kml file
 
 @return	pointer to the KML struct
*******************************************************************************/

KML *KML_new(
	KMZ *kmz,
	char *kmlfile);

/*****************************************************************************//**
 function to free a kml struct
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_free(
	KML *kml);

/*****************************************************************************//**
 function to free a kmz struct
 
 @param kmz				pointer to the kmz struct
 
 @return	nothing
*******************************************************************************/

void KMZ_free(
	KMZ *kmz);

/*****************************************************************************//**
 function to write a kmz to disk
 
 @param kmz				pointer to the kmz struct
 
 @return	nothing
*******************************************************************************/

void KMZ_write(
	KMZ *kmz);

/*****************************************************************************//**
 function to write a kml to disk
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_write(
	KML *kml);

/*****************************************************************************//**
 function to add a kml header to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_header (
	KML *kml);

/*****************************************************************************//**
 function to add a kml footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_footer (
	KML *kml);

/*****************************************************************************//**
 function to add a name to a kml

 @param kml				pointer to the kml struct
 @param name			the name of the item

 @return	nothing
*******************************************************************************/

void KML_name(
	KML *kml,
	char *name);

/*****************************************************************************//**
 function to add a description to a kml

 @param kml				pointer to the kml struct
 @param desc			the description of the item

 @return	nothing
*******************************************************************************/

void KML_desc(
	KML *kml,
	char *desc);

/*****************************************************************************//**
 function to add a kml placemark header to a kml
 
 @param kml				pointer to the kml struct
 @param name			the name of the placemark or NULL for none
 @param desc			the description of a placemark or NULL for none
 @param styleid		the style id for the placemark
 
 @return	nothing
*******************************************************************************/

void KML_placemark_header (
	KML *kml,
	char *name,
	char *desc,
	char *styleid);

/*****************************************************************************//**
 function to add a placemark footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_placemark_footer (
	KML *kml);

/*****************************************************************************//**
 function to add a icon header to a kml
 
 args:
								kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_icon_header (
		KML *kml);

/*****************************************************************************//**
 function to add a icon footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_icon_footer (
	KML *kml);

/*****************************************************************************//**
 function to add a linestring header to a kml
 
 @param kml						pointer to the kml struct
 @param extrude				extruded? 0/1
 @param tessellate		tessellate? 0/1
 @param altitudeMode	altitude mode
 
 @return	nothing
*******************************************************************************/

void KML_linestring_header (
	KML *kml,
	int extrude,
	int tessellate,
	int altitudeMode);

/*****************************************************************************//**
 function to add a linestring footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_linestring_footer (
	KML *kml);

/*****************************************************************************//**
 function to add a polygon header to a kml
 
 @param kml						pointer to the kml struct
 @param extrude				extruded? 0/1
 @param tessellate		tessellate? 0/1
 @param altitudeMode	altitude mode
 
 @return	nothing
*******************************************************************************/

void KML_polygon_header (
	KML *kml,
	int extrude,
	int tessellate,
	int altitudeMode);

/*****************************************************************************//**
 function to add a polygon footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_polygon_footer (
	KML *kml);

/*****************************************************************************//**
 function to add a outerBoundaryIs header to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_outerboundry_header (
	KML *kml);

/*****************************************************************************//**
 function to add a outerBoundaryIs footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_outerboundry_footer (
	KML *kml);

/*****************************************************************************//**
 function to add a innerBoundaryIs header to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_innerboundry_header (
	KML *kml);

/*****************************************************************************//**
 function to add a innerBoundaryIs footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_innerboundry_footer (
	KML *kml);

/*****************************************************************************//**
 function to add a linearring header to a kml
 
 @param kml						pointer to the kml struct
 @param extrude				extruded? 0/1
 @param tessellate		tessellate? 0/1
 @param altitudeMode	altitude mode
 
 @return	nothing
*******************************************************************************/

void KML_linearring_header (
	KML *kml,
	int extrude,
	int tessellate,
	int altitudeMode);

/*****************************************************************************//**
 function to add a linearring footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_linearring_footer (
	KML *kml);

/*****************************************************************************//**
 function to add coordinates of a point to a kml
 
 @param kml					pointer to the kml struct
 @param precision		number of places to the right of the point to print
 @param x						x coord of the point
 @param y						y coord of the point
 @param z						z coord of the point NULL if 2d
	
 @return	nothing
*******************************************************************************/

void KML_coordinates (
	KML *kml,
	int precision,												
	double *x,
	double *y,
	double *z);

/*****************************************************************************//**
 function to add a style header to a kml
 
 @param kml					pointer to the kml struct
 @param id					the style id

 @return	nothing
*******************************************************************************/

void KML_style_header (
	KML *kml,
	char *id);

/*****************************************************************************//**
 function to add a style footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_style_footer (
	KML *kml);

/*****************************************************************************//**
 function to add a linestyle to a kml
 
 @param kml				pointer to the kml struct
 @param rgb				rgb value for the style
 @param alpha			the alpha value for the style
 @param width			the line width
 
 @return	nothing
*******************************************************************************/

void KML_linestyle (
	KML *kml,
	char *rgb,
	char *alpha,
	int width);

/*****************************************************************************//**
 function to add a polystyle to a kml
 
 @param kml				pointer to the kml struct
 @param rgb				rgb value for the fill style
 @param alpha			the alpha value for the fill style
 
 @return	nothing
*******************************************************************************/

void KML_polystyle (
	KML *kml,
	char *rgb,
	char *alpha);

/*****************************************************************************//**
 function to add a iconstyle to a kml
 
 @param kml				pointer to the kml struct
 @param rgb				rgb value for the style
 @param alpha			the alpha value for the style
 @param scale			scale value for the style
 @param heading		deg to rotate the icon
 @param icon 			url of the icon to use or NULL

 @return	nothing
*******************************************************************************/

void KML_iconstyle (
	KML *kml,
	char *rgb,
	char *alpha,
	float scale,
	float heading,
	char *icon);

/*****************************************************************************//**
 function to add a networklink to a kml
 
 @param kml				pointer to the kml struct
 @param url				url to link to 
 
 @return	nothing
*******************************************************************************/

void KML_networklink (
	KML *kml,
	char *url);

/*****************************************************************************//**
 function to add a TimeStamp header to a kml
 
 @param pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_timestamp_header (
	KML *kml);

/*****************************************************************************//**
 function to add a TimeStamp footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_timestamp_footer (
	KML *kml);

/*****************************************************************************//**
 function to add a TimeSpan header to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_timespan_header (
	KML *kml);

/*****************************************************************************//**
 function to add a TimeSpan footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_timespan_footer (
	KML *kml);

/*****************************************************************************//**
 function to add a begin header to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_begin_header (
	KML *kml);

/*****************************************************************************//**
 function to add a begin footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_begin_footer (
	KML *kml);

/*****************************************************************************//**
 function to add a end header to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_end_header (
	KML *kml);

/*****************************************************************************//**
 function to add a end footer to a kml
 
 @param kml				pointer to the kml struct
 
 @return	nothing
*******************************************************************************/

void KML_end_footer (
	KML *kml);

/*****************************************************************************//**
 function to add a time to a kml
 
 @param kml				pointer to the kml struct
 @param year
 @param month
 @param day
 @param hour
 @param min
 @param sec
 
 @return	nothing
*******************************************************************************/

void KML_time (
	KML *kml,
	int *year,
	int *month,
	int *day,
	int *hour,
	int *min,
	int *sec);

#endif /* _LIBKML_H */

/*****************************************************************************//**
 function to add a style url to a kml
 
 @param kml				pointer to the kml struct
 @param styleid		the style id for the placemark
 
 @return	nothing
*******************************************************************************/

void KML_style_url (
	KML *kml,
	char *styleid);
