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
 
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "buffer.h"
#include "error.h"

#define INDENTSPACES 2

#define INITIAL 4096
/*******************************************************************************
	function to allocate memory for a buffer
*******************************************************************************/

void buffer_alloc (
	buffer *buf,
	size_t need)
{
	char *temp;
	
	/***** if no memory alocate *****/

	if (!buf->alloced) {
		buf->alloced = INITIAL;
		if (!(buf->buf = malloc (buf->alloced)))
			ERROR("buffer_alloc");
		
		buf->buf[0] = 0;
	}

	/***** if not enough memory realocate *****/

	while (buf->alloced < buf->used + need) {

		buf->alloced *= 2;
		if (!(temp = realloc (buf->buf, buf->alloced)))
			ERROR("buffer_alloc");
			
		buf->buf = temp;
	}
	
	return;
}

/*******************************************************************************
	function to print to a buffer

	args:
						buf			the buffer to print to
						fmt			the format string
						...			aditianal args that match the format string
	
 returns:
						the number of chars printed to the buffer, not includeing the \0
*******************************************************************************/

int buffer_printf(
	buffer *buf,
	char *format,
	...)
{

	va_list ap;
	int result = 0;
	int need = 0;
	
	int spaces = buf->indent * INDENTSPACES;
	need = 1 + spaces;
	
	/***** alocate for spaces *****/
	
	if (buf->alloced < buf->used + need )
		buffer_alloc(buf, need);
	
	/***** add spaces *****/
	
	int i;
	for (i = 0 ; i < spaces; i++) {
		*(buf->buf + buf->used) = ' ';
		buf->used++;
	}
	*(buf->buf + buf->used) = '\0';

/***** check if there is any memory *****/
	/*
	if (!buf->alloced) {
		va_start (ap, format);
		need = 1 + vsnprintf (NULL, 0, format, ap);
		buffer_alloc(buf, need);
		va_end (ap);
	}
	*/
	/***** try to print to the buffer *****/
	
	va_start (ap, format);
	result = vsnprintf (buf->buf + buf->used,
											buf->alloced - buf->used,
											format,
											ap);
	va_end (ap);
	
	/***** check if there was enough memory *****/
	
	if (buf->alloced < buf->used + result + 2) {
		va_start (ap, format);
		need = 1 + vsnprintf (NULL, 0, format, ap);
		buffer_alloc(buf, need);
		va_end (ap);
		
		/***** reprint *****/
		
		va_start (ap, format);
		result = vsprintf (buf->buf + buf->used,
												format,
												ap);
		va_end (ap);
	}
		
	buf->used += result;

	return result + buf->indent * INDENTSPACES;
}

/*******************************************************************************
	function to print to a buffer with no indent

	args:
						buf			the buffer to print to
						fmt			the format string
						...			aditianal args that match the format string
	
 returns:
						the number of chars printed to the buffer, not includeing the \0
*******************************************************************************/

int buffer_printf_noindent(
	buffer *buf,
	char *format,
	...)
{

	va_list ap;
	size_t result = 0;
	
	/***** try to print to the buffer *****/
	
	va_start (ap, format);
	result = vsnprintf (buf->buf + buf->used,
											buf->alloced - buf->used,
											format,
											ap);
	va_end (ap);
	
	/***** check if there was enough memory *****/
	
	if (buf->alloced < buf->used + result + 2) {
		va_start (ap, format);
		size_t need = 1 + vsnprintf (NULL, 0, format, ap);
		buffer_alloc(buf, need);
		va_end (ap);
		
		/***** reprint *****/
		
		va_start (ap, format);
		result = vsprintf (buf->buf + buf->used,
												format,
												ap);
		va_end (ap);
	}
		
	buf->used += result;
	

	return result;
}

/*******************************************************************************
	function to free a buffer

	args:
						buf			the buffer to free
	
 returns:
						nothing
*******************************************************************************/

void buffer_free(
	buffer *buf)
{
	
	free(buf->buf);
	
	return;
}
