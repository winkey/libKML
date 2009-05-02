/***************************************************************************
 *  kml.c
 *
 *  Thursday May 15 21:46:00 2008
 *  Copyright  2008  winkey
 *  <rush@winkey.org>
 ****************************************************************************/
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

#define MAKING_KML_C


#include <stdio.h>
#include <stdlib.h>
#include "../minizip/zip.h"
#include <time.h>
#include "buffer.h"
#include "zipbuffer.h"
#include "error.h"
#include "libDataStruct/DLList.h"


/*******************************************************************************
 kml info storage struct
 
 members:
										kmlfile			the full path of the kml file
										kmzfile			the full path of the kmz file
										buf					the buffer struct
*******************************************************************************/

typedef struct {
	char kmlfile[800];
	buffer buf;
	char fmt2d[100];
	char fmt3d[100];
} KML;

typedef struct {
	char *kmzfile;
	DLList kmls;
} KMZ;

#include "libKML.h"

/*******************************************************************************
 function to create a new kmz
 
 args:
								kmzfile			the full path of the kmz file
 
 returns:				pointer to the KMZ struct
*******************************************************************************/

KMZ *KMZ_new(
	char *kmzfile)
{
	KMZ *result = NULL;
	
	if (!(result = calloc(sizeof(KMZ), 1)))
		ERROR("KMZ_new");
	
	result->kmzfile = kmzfile;
	
	return result;
}


/*******************************************************************************
 function to create a new kml
 
 args:
								kmz					pointer to the kmz to add it to
								kmlfile			the full path of the kml file
								printprec		the precision to print coordantes at
 
 returns:				pointer to the KML struct
*******************************************************************************/

KML *KML_new(
	KMZ *kmz,
	char *kmlfile,
	int printprec)
{
	KML *result = NULL;
	
	if (!(result = calloc(sizeof(KML), 1)))
		ERROR("KML_new");
	
	strncpy(result->kmlfile, kmlfile, sizeof(result->kmlfile));
	
	snprintf(result->fmt2d, sizeof(result->fmt2d), "%%.%ilg,%%.%ilg ",
					 printprec, printprec);
	snprintf(result->fmt3d, sizeof(result->fmt3d), "%%.%ilg,%%.%ilg,%%.%ilg ",
					 printprec, printprec, printprec);
	
	if (kmz)
		DLList_append(&kmz->kmls, result);
	
	return result;	
}

/*******************************************************************************
 function to free a kml struct
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_free(
	KML *kml)
{
	
	buffer_free (&(kml->buf));
	free(kml);
	
	return;
}

/*******************************************************************************
	dllist iterate function to free a kmz
*******************************************************************************/

void *kmz_free_iterate(
	DLList *list,
	DLList_node *node,
	void *data,
	void *extra)
{
	KML *kml = data;
	
	KML_free (kml);
	
	return NULL;
}

/*******************************************************************************
 function to free a kmz struct
 
 args:
								kmz				pointer to the kmz struct
 
 returns:
								nothing
*******************************************************************************/

void KMZ_free(
	KMZ *kmz)
{
	
	DLList_delete_all(&kmz->kmls, (DLList_data_free_func) KML_free);
	
	free(kmz);
	
	return;
}
/*******************************************************************************
	dllist iterate function to write a kmz
*******************************************************************************/

void *kmz_write_iterate(
	DLList *list,
	DLList_node *node,
	void *data,
	void *extra)
{
	KML *kml = data;
	zipFile zf = extra;
	zipbuffer_add(kml->kmlfile, zf, &(kml->buf));
	
	return NULL;
}

/*******************************************************************************
 function to write a kmz to disk
 
 args:
								kmz				pointer to the kmz struct
 
 returns:
								nothing
*******************************************************************************/

void KMZ_write(
	KMZ *kmz)
{
	
	zipFile zf = zipbuffer_open(kmz->kmzfile);
	
	DLList_iterate(&kmz->kmls, kmz_write_iterate, zf);
	
	zipbuffer_close(zf);
	
	return;
}

/*******************************************************************************
 function to write a kml to disk
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_write(
	KML *kml)
{
	
	FILE *fp;
	
	if (!(fp = fopen(kml->kmlfile, "w")))
		ERROR("KML_write");
		
	fputs(kml->buf.buf, fp);
	fclose(fp);
	
	return;
}

/*******************************************************************************
 function to add a kml header to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_header (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n");
	buffer_printf(buf, "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
	buffer_printf(buf, "<Document>\n");
	buf->indent++;
 return;
}

/*******************************************************************************
 function to add a kml footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	buf->indent--;
	buffer_printf(buf, "</Document>\n");
	buffer_printf(buf, "</kml>\n");
	
	return;
}

/*****************************************************************************//**
 function to add a name to a kml

 @param kml				pointer to the kml struct
 @param name			the name of the item

 @return	nothing
*******************************************************************************/

void KML_name(
	KML *kml,
	char *name)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "  <name>%s</name>\n", name);
	
	return;
}

/*****************************************************************************//**
 function to add a description to a kml

 @param kml				pointer to the kml struct
 @param desc			the description of the item

 @return	nothing
*******************************************************************************/

void KML_desc(
	KML *kml,
	char *desc)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<description>");
		buffer_printf(buf, "%s", desc);
	buffer_printf(buf, "</description>\n");
	
	return;
}

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
	char *styleid)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<Placemark>\n");
	if (name)
		buffer_printf(buf, "  <name>%s</name>\n", name);
	if (desc) {
		buffer_printf(buf, "  <description>");
		buffer_printf(buf, "%s", desc);
		buffer_printf(buf, "  </description>\n");
	}
	if (styleid)
		buffer_printf(buf, "  <styleUrl>#%s</styleUrl>\n", styleid);
	
	buf->indent++;
	
	return;
}

/*******************************************************************************
 function to add a placemark footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_placemark_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buf->indent--;
  buffer_printf(buf, "</Placemark>\n");
	
	return;
}

/*******************************************************************************
 function to add a icon header to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_icon_header (
		KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<Point>\n");

	buffer_printf(buf, "  <coordinates>");
	buf->indent++;
	
	return;
}

/*******************************************************************************
 function to add a icon footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_icon_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buf->indent--;
	buffer_printf_noindent(buf, "</coordinates>\n");
	buffer_printf(buf, "</Point>\n");

	return;
}

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
	int tessellate,
	int altitudeMode)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<LineString>\n");
	if (extrude)
		buffer_printf(buf, "  <extrude>1</extrude>\n");
	if (tessellate)		
		buffer_printf(buf, "  <tessellate>1</tessellate>\n");
	
	switch (altitudeMode) {
	
		case relativeToGround:
			buffer_printf(buf, "  <altitudeMode>relativeToGround</altitudeMode>\n");
			break;
		
		case absolute:
			buffer_printf(buf, "  <altitudeMode>absolute</altitudeMode>\n");
			break;
		
		case clampToGround:
		default:
			break;
	}
	
	buffer_printf(buf, "  <coordinates>");
	buf->indent++;

	return;
}

/*******************************************************************************
 function to add a linestring footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_linestring_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buf->indent--;
	buffer_printf_noindent(buf, "</coordinates>\n");
	buffer_printf(buf, "</LineString>\n");

	return;
}

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
	int tessellate,
	int altitudeMode)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<Polygon>\n");
		if (extrude)
		buffer_printf(buf, "  <extrude>1</extrude>\n");
	if (tessellate)		
		buffer_printf(buf, "  <tessellate>1</tessellate>\n");

	
	switch (altitudeMode) {
	
		case relativeToGround:
			buffer_printf(buf, "  <altitudeMode>relativeToGround</altitudeMode>\n");
			break;
		
		case absolute:
			buffer_printf(buf, "  <altitudeMode>absolute</altitudeMode>\n");
			break;
		
		case clampToGround:
		default:
			break;
	}
	buf->indent++;
	
	return;
}

/*******************************************************************************
 function to add a polygon footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_polygon_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buf->indent--;
	buffer_printf(buf, "</Polygon>\n");
	
	return;
}

/*******************************************************************************
 function to add a outerBoundaryIs header to a kml
 
 args:
								kml				pointer to the kml struct
								extrude			extruded? 0/1
								tessellate	tessellate? 0/1
 
 returns:
								nothing
*******************************************************************************/

void KML_outerboundry_header (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<outerBoundaryIs>\n");
	buf->indent++;
	
	return;
}

/*******************************************************************************
 function to add a outerBoundaryIs footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_outerboundry_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buf->indent--;
	buffer_printf(buf, "</outerBoundaryIs>\n");
	
	return;
}

/*******************************************************************************
 function to add a innerBoundaryIs header to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_innerboundry_header (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<innerBoundaryIs>\n");
	buf->indent++;
	
	return;
}

/*******************************************************************************
 function to add a innerBoundaryIs footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_innerboundry_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buf->indent--;
	buffer_printf(buf, "</innerBoundaryIs>\n");
	
	return;
}

/*******************************************************************************
 function to add a linearring header to a kml
 
 args:
								kml					pointer to the kml struct
								extrude			extruded? 0/1
								tessellate	tessellate? 0/1
 
 returns:
								nothing
*******************************************************************************/

void KML_linearring_header (
	KML *kml,
	int extrude,
	int tessellate,
	int altitudeMode)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<LinearRing>\n");;
	if (extrude)
		buffer_printf(buf, "  <extrude>1</extrude>\n");
	if (tessellate)		
		buffer_printf(buf, "  <tessellate>1</tessellate>\n");
	
	switch (altitudeMode) {
	
		case relativeToGround:
			buffer_printf(buf, "  <altitudeMode>relativeToGround</altitudeMode>\n");
			break;
		
		case absolute:
			buffer_printf(buf, "  <altitudeMode>absolute</altitudeMode>\n");
			break;
		
		case clampToGround:
		default:
			break;
	}
	
	buffer_printf(buf, "  <coordinates>");
	buf->indent++;
	
	return;
}

/*******************************************************************************
 function to add a linearring footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_linearring_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buf->indent--;
	buffer_printf_noindent(buf, "</coordinates>\n");
	buffer_printf(buf, "</LinearRing>\n");

	return;
}

/*******************************************************************************
 function to add coordinates of a 3d point to a kml
 
 args:
								kml					pointer to the kml struct
								x						x coord of the point
								y						y coord of the point
								z						z coord of the point
	
 returns:
								nothing
*******************************************************************************/

void KML_coordinates_3d (
	KML *kml,
	double *x,
	double *y,
	double *z)
{
	buffer *buf = &(kml->buf);

	buffer_printf_noindent(buf, kml->fmt3d, *x, *y, *z);
	
	return;
}
/*******************************************************************************
 function to add coordinates of a 2d point to a kml
 
 args:
								kml					pointer to the kml struct
								x						x coord of the point
								y						y coord of the point
	
 returns:
								nothing
*******************************************************************************/

void KML_coordinates_2d (
	KML *kml,
	double *x,
	double *y)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf_noindent(buf, kml->fmt2d, *x, *y);
	
	return;
}

/*******************************************************************************
 function to add a style header to a kml
 
 args:
								kml					pointer to the kml struct
								id					the style id

 returns:
								nothing
*******************************************************************************/

void KML_style_header (
	KML *kml,
	char *id)
{
	buffer *buf = &(kml->buf);
	
	if (id)
		buffer_printf(buf, "<Style id=\"%s\">\n", id);
	else
		buffer_printf(buf, "<Style>\n", id);
	buf->indent++;
	
	return;
}

/*******************************************************************************
 function to add a style footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_style_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buf->indent--;
	buffer_printf(buf, "</Style>\n");
	
	return;
}

/*******************************************************************************
 function to add a linestyle to a kml
 
 args:
								kml				pointer to the kml struct
								rgb				rgb value for the style
								alpha			the alpha value for the style
								width			the line width
 
 returns:
								nothing
*******************************************************************************/

void KML_linestyle (
	KML *kml,
	char *rgb,
	char *alpha,
	int width)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<LineStyle>\n");
	buffer_printf(buf, "  <color>%c%c%c%c%c%c%c%c</color>\n", alpha[0], alpha[1],
								rgb[4], rgb[5], rgb[2], rgb[3], rgb[0], rgb[1]);
	buffer_printf(buf, "  <width>%i</width>\n", width);
	buffer_printf(buf, "</LineStyle>\n");
	
	return;
}

/*******************************************************************************
 function to add a polystyle to a kml
 
 args:
								kml			pointer to the kml struct
								rgb			rgb value for the fill style
								alpha		the alpha value for the fill style
 
 returns:
								nothing
*******************************************************************************/

void KML_polystyle (
	KML *kml,
	char *rgb,
	char *alpha)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<PolyStyle>\n");
	buffer_printf(buf, "  <color>%c%c%c%c%c%c%c%c</color>\n", alpha[0], alpha[1],
								rgb[4], rgb[5], rgb[2],
								rgb[3], rgb[0], rgb[1]);
	buffer_printf(buf, "</PolyStyle>\n");
	
	return;
}

/*******************************************************************************
 function to add a iconstyle to a kml
 
 args:
								kml				pointer to the kml struct
								rgb				rgb value for the style
								alpha			the alpha value for the style
								scale			scale value for the style or 1
								heading		deg to rotate the icon or 0
								dx				hotspox x fraction or 0.5
								dy				hotspox y fraction or 0.5
								icon 			url of the icon to use or NULL
 
 returns:
								nothing
*******************************************************************************/

void KML_iconstyle (
	KML *kml,
	char *rgb,
	char *alpha,
	float scale,
	float heading,
	float dx,
	float dy,
	char *icon)
{
	
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<IconStyle>\n");
	buffer_printf(buf, "  <color>%c%c%c%c%c%c%c%c</color>\n", alpha[0], alpha[1],
								rgb[4], rgb[5], rgb[2],
								rgb[3], rgb[0], rgb[1]);
	
	if (scale != 1.0)
		buffer_printf(buf, "  <scale>%f</scale>\n", scale);
	
	if (heading != 0)
		buffer_printf(buf, "  <heading>%f</heading>\n", heading);
	
	if (dx != 0.5 || dy != 0.5)
		buffer_printf(buf, "  <hotSpot x=\"%f\" y=\"%f\" xunits=\"%s\" yunits=\"%s\"/>\n",
									dx, dy, "fraction", "fraction");
	
	if (icon) {
		buffer_printf(buf, "  <Icon>\n");
		buffer_printf(buf, "    <href>%s</href>\n", icon);
		buffer_printf(buf, "  </Icon>\n");
	}
	
	buffer_printf(buf, "</IconStyle>\n");
	
	return;
}

/*******************************************************************************
 function to add a networklink to a kml
 
 args:
								kml				pointer to the kml struct
								url				url to link to 
 
 returns:
								nothing
*******************************************************************************/

void KML_networklink (
	KML *kml,
	char *url)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<NetworkLink>\n");
	buffer_printf(buf, "  <Link>\n");
	buffer_printf(buf, "    <href>%s</href>\n", url);
	buffer_printf(buf, "  </Link>\n");
	buffer_printf(buf, "</NetworkLink>\n");
	
	return;
}

/*******************************************************************************
 function to add a TimeStamp header to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_timestamp_header (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<TimeStamp>\n");
	buf->indent++;
	buffer_printf(buf, "<when>\n");
	buf->indent++;
	
	return;
}

/*******************************************************************************
 function to add a TimeStamp footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_timestamp_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buf->indent--;
	buffer_printf(buf, "</when>\n");
	buf->indent--;
	buffer_printf(buf, "</TimeStamp>\n");
	
	return;
}

/*******************************************************************************
 function to add a TimeSpan header to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_timespan_header (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<TimeSpan>\n");
	buf->indent++;
	
	return;
}

/*******************************************************************************
 function to add a TimeSpan footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_timespan_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buf->indent--;
	buffer_printf(buf, "</TimeSpan>\n");
	
	return;
}

/*******************************************************************************
 function to add a begin header to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_begin_header (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<begin>");
	
	return;
}

/*******************************************************************************
 function to add a begin footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_begin_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf_noindent(buf, "</begin>\n");
	
	return;
}

/*******************************************************************************
 function to add a end header to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_end_header (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "<end>");
	
	return;
}

/*******************************************************************************
 function to add a end footer to a kml
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_end_footer (
	KML *kml)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf_noindent(buf, "</end>\n");
	
	return;
}

/*******************************************************************************
 function to add a time to a kml
 
  args:
								kml				pointer to the kml struct
								year			pointer to the year eg 2001
								month			pointer to the mo
								day
								hour
								min
								sec
 
 returns:
								nothing
*******************************************************************************/

void KML_time (
	KML *kml,
	int *year,
	int *month,
	int *day,
	int *hour,
	int *min,
	int *sec)
{
	buffer *buf = &(kml->buf);
	
	if (year)
		buffer_printf_noindent(buf, "%i", *year);
	else
		goto end;
	
	if (month)
		buffer_printf_noindent(buf, "-%02i", *month);
	else
		goto end;

	if (day)
		buffer_printf_noindent(buf, "-%02i", *day);
	else
		goto end;
	
	if (hour)
		buffer_printf_noindent(buf, "T%02i", *hour);
	else
		goto end;
	
	if (min)
		buffer_printf_noindent(buf, ":%02i", *min);
	else
		goto end;
	
	if (sec)
		buffer_printf_noindent(buf, ":%02iZ", *sec);
	else
		goto end;
	
end:
	
	
	return;
}

/*******************************************************************************
 function to add a style url to a kml
 
 @param kml				pointer to the kml struct
 @param styleurl	the style url for the placemark or NULL for none
 @param styleid		the style id for the placemark
 
 @return	nothing
*******************************************************************************/

void KML_style_url (
	KML *kml,
	char *styleurl,
	char *styleid)
{
	buffer *buf = &(kml->buf);
	
	if (styleurl)
		buffer_printf(buf, "<styleUrl>%s#%s</styleUrl>\n", styleurl, styleid);
	else
		buffer_printf(buf, "<styleUrl>#%s</styleUrl>\n", styleid);
	
	return;
}
