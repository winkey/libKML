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
#include <zip.h>
#include "buffer.h"
#include "zipbuffer.h"
#include "error.h"

/*******************************************************************************
 kml info storage struct
 
 members:
										kmlfile			the full path of the kml file
										kmzfile			the full path of the kmz file
										buf					the buffer struct
*******************************************************************************/

typedef struct {
	char *kmlfile;
	char *kmzfile;
	buffer buf;
} KML;

#include "libKML.h"


/*******************************************************************************
 function to create a new kml
 
 args:
								kmlfile			the full path of the kml file
								kmzfile			the full path of the kmz file
 
 returns:				pointer to the KML struct
*******************************************************************************/

KML *KML_new(
	char *kmlfile,
	char *kmzfile)
{
	KML *result = NULL;
	
	if (!(result = calloc(sizeof(KML), 1)))
		ERROR("KML_new");
	
	result->kmlfile = kmlfile;
	result->kmzfile = kmzfile;
	
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
 function to write a kml to disk
 
 args:
								kml				pointer to the kml struct
 
 returns:
								nothing
*******************************************************************************/

void KML_write(
	KML *kml)
{
	if (kml->kmzfile) {
		struct zip *za;
		
		za = zipbuffer_open(kml->kmzfile);
		zipbuffer_add(kml->kmlfile, za, &(kml->buf));
		zipbuffer_close(za);
	}
	else {
		FILE *fp;
		
		if (!(fp = fopen(kml->kmlfile, "w")))
			ERROR("KML_write");
		
		fputs(kml->buf.buf, fp);
		fclose(fp);
	}
	
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
	
	buffer_printf(buf, "</Document>\n");
	buffer_printf(buf, "</kml>\n");
	
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
	
	buffer_printf(buf, "  <Placemark>\n");
	if (name)
		buffer_printf(buf, "    <name>%s</name>\n", name);
	if (desc) {
		buffer_printf(buf, "    <description>");
		buffer_printf(buf, "%s", desc);
		buffer_printf(buf, "    </description>\n");
	}
	buffer_printf(buf, "    <styleUrl>#%s</styleUrl>\n", styleid);
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
	
  buffer_printf(buf, "  </Placemark>\n");
	
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
	int tessellate)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "    <LineString>\n");
	if (extrude)
		buffer_printf(buf, "      <extrude>1</extrude>\n");
	if (tessellate)		
		buffer_printf(buf, "      <tessellate>1</tessellate>\n");
	buffer_printf(buf, "      <coordinates>");

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
	
	
	buffer_printf(buf, "</coordinates>\n");
	buffer_printf(buf, "    </LineString>\n");

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
	int tessellate)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "    <Polygon>\n");
		if (extrude)
		buffer_printf(buf, "      <extrude>1</extrude>\n");
	if (tessellate)		
		buffer_printf(buf, "      <tessellate>1</tessellate>\n");
	buffer_printf(buf, "      <outerBoundaryIs>\n");
	buffer_printf(buf, "        <LinearRing>\n");
	buffer_printf(buf, "          <coordinates>");

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
	
	
	buffer_printf(buf, "</coordinates>\n");
	buffer_printf(buf, "        </LinearRing>\n");
	buffer_printf(buf, "      </outerBoundaryIs>\n");
	buffer_printf(buf, "    </Polygon>\n");
	
	return;
}

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
	double *z)
{
	buffer *buf = &(kml->buf);
	char fmt[40];
	
	if (!z) {
		snprintf(fmt, sizeof(fmt), "%%.%ilg,%%.%ilg ", precision, precision);
		buffer_printf(buf, fmt, *x, *y);
	}
	else {
		snprintf(fmt, sizeof(fmt), "%%.%ilg,%%.%ilg,%%.%ilg ",
						 precision, precision, precision);
		buffer_printf(buf, fmt, *x, *y, *z);
	}
	
	return;
}
	

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
	int width)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "    <Style id=\"%s\">\n", id);
	buffer_printf(buf, "  	  <LineStyle>\n");
	buffer_printf(buf, "        <color>%s%c%c%c%c%c%c</color>\n", alpha,
								rgb[4], rgb[5], rgb[2], rgb[3], rgb[0], rgb[1]);
	buffer_printf(buf, "        <width>%i</width>\n", width);
	buffer_printf(buf, "      </LineStyle>\n");
	buffer_printf(buf, "    </Style>\n");
	
	return;
}

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
	char *polyalpha)
{
	buffer *buf = &(kml->buf);
	
	buffer_printf(buf, "    <Style id=\"%s\">\n", id);
	buffer_printf(buf, "  	  <LineStyle>\n");
	buffer_printf(buf, "        <color>%s%c%c%c%c%c%c</color>\n", linealpha,
								linergb[4], linergb[5], linergb[2],
								linergb[3], linergb[0], linergb[1]);
	buffer_printf(buf, "        <width>%i</width>\n", width);
	buffer_printf(buf, "      </LineStyle>\n");
	buffer_printf(buf, "  	  <PolyStyle>\n");
	buffer_printf(buf, "        <color>%s%c%c%c%c%c%c</color>\n", polyalpha,
								polyrgb[4], polyrgb[5], polyrgb[2],
								polyrgb[3], polyrgb[0], polyrgb[1]);
	buffer_printf(buf, "      </LineStyle>\n");
	buffer_printf(buf, "    </Style>\n");
	
	return;
}

