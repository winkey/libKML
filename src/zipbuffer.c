/***************************************************************************
 *
 *
 *  Fri May 16 06:48:40 2008
 *  Copyright  2008  rush
 *  <rush@<host>>
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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../minizip/zip.h"

#include "buffer.h"
#include "zipbuffer.h"
#include "error.h"


/*******************************************************************************
	function to open the zip file
	
	args:
						name			the filename of the zip archive

	returns:
						pointer to the zip structure
						exit()s on error
*******************************************************************************/

zipFile *zipbuffer_open (
	char *name)
{
	//struct zip *result = NULL;
	zipFile *result = NULL;

	if (!(result = zipOpen(name, APPEND_STATUS_CREATE))) {
	//if (!(result = zip_open(name, ZIP_CREATE, &err))) {
		ERROR("zipbuffer_open");
	}
	
	return result;
}

/*******************************************************************************
	function to add the buffer to the zip file
	
	args:
						name			the filename of the file to add to the zip archive
						zip				pointer to the zip structure
						buf				the buffer to add

	returns:
						nothing
*******************************************************************************/

void zipbuffer_add (
	char *name,
	zipFile zF,
	buffer *buf)
{
#warning fixme i need info
	zip_fileinfo zipfi = {};
	if (zipOpenNewFileInZip(zF, name, &zipfi, NULL, 0, NULL, 0, NULL, Z_DEFLATED,
													 Z_DEFAULT_COMPRESSION))
		ERROR("zipbuffer_add");
	
	
	if (ZEXPORT zipWriteInFileInZip(zF, buf->buf, buf->used)) {
		ERROR("zipbuffer_add");
	}
	
	if (zipCloseFileInZip(zF)) {
		ERROR("zipbuffer_add");
	}
	
	return;
}

/*******************************************************************************
	function to close the zip file

	args:
						zip				pointer to the zip structure

	returns:
						nothing
*******************************************************************************/

void zipbuffer_close (
	zipFile zF)
{
	
	if (zipClose (zF, "created by libKML"))
  	ERROR("zipbuffer_close");
	
	return;
}

