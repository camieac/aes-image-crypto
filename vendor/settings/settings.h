/*
 *    settings version 1.0.0
 *
 *    ANSI C implementation for managing application settings.
 *
 *    settings.h
 *
 *    Copyright (c) 2009 Per Ola Kristensson.
 *
 *    Per Ola Kristensson <pok21@cam.ac.uk>
 *    Inference Group, Department of Physics
 *    University of Cambridge
 *    Cavendish Laboratory
 *    JJ Thomson Avenue
 *    CB3 0HE Cambridge
 *    United Kingdom
 *
 *    settings is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    settings is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with settings.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "strmap.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct Settings Settings;

/*
 * This callback function is called once per key-value when enumerating
 * all keys inside a section.
 *
 * Parameters:
 *
 * key: A pointer to a null-terminated C string. The string must not
 * be modified by the client.
 *
 * value: A pointer to a null-terminated C string. The string must
 * not be modified by the client.
 *
 * obj: A pointer to a client-specific object. This parameter may be
 * null.
 *
 * Return value: None.
 */
typedef void(*settings_section_enum_func)(const char *key, const char *value, const void *obj);

/*
 * Creates a settings object.
 *
 * Return value: A pointer to a settings object,
 * or null if a new settings object could not be allocated.
 */
Settings * settings_new();

/*
 * Releases all memory held by a settings object.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 * If the supplied settings object has been previously released, the
 * behaviour of this function is undefined.
 *
 * Return value: None.
 */
void settings_delete(Settings *settings);

/*
 * Constructs a settings object by loading settings in textual form
 * from the given stream.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 *
 * stream: A pointer to a stream. This parameter cannot be null.
 *
 * Return value: A pointer to a settings object,
 * or null if an error occurred.
 */
Settings * settings_open(FILE *stream);

/*
 * Saves the current settings object in textual form to the given stream.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 *
 * stream: A pointer to a stream. This parameter cannot be null.
 *
 * Return value: 1 if the operation succeeded, 0 otherwise.
 */
int settings_save(const Settings *settings, FILE *stream);

/*
 * Returns the value associated with the supplied key in the
 * provided section.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 *
 * section: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * key: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * out_buf: A pointer to an output buffer which will contain the value,
 * if it exists and fits into the buffer.
 *
 * n_out_buf: The size of the output buffer in bytes.
 *
 * Return value: If out_buf is set to null and n_out_buf is set to 0 the return
 * value will be the number of bytes required to store the value (if it exists)
 * and its null-terminator. For all other parameter configurations the return value
 * is 1 if an associated value was found and completely copied into the output buffer,
 * 0 otherwise.
 */
int settings_get(const Settings *settings, const char *section, const char *key, char *out_buf, unsigned int n_out_buf);

/*
 * Returns the integer value associated with the supplied key in the
 * provided section.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 *
 * section: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * key: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * Return value: The integer value associated to the provided section and
 * key, or 0 if no such value exists.
 */
int settings_get_int(const Settings *settings, const char *section, const char *key);

/*
 * Returns the long integer value associated with the supplied key in the
 * provided section.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 *
 * section: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * key: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * Return value: The long integer value associated to the provided section and
 * key, or 0 if no such value exists.
 */
long settings_get_long(const Settings *settings, const char *section, const char *key);

/*
 * Returns the double value associated with the supplied key in the
 * provided section.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 *
 * section: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * key: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * Return value: The double value associated to the provided section and
 * key, or 0 if no such value exists.
 */
double settings_get_double(const Settings *settings, const char *section, const char *key);

/*
 * Returns the integer tuple associated with the supplied key in the
 * provided section.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 *
 * section: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * key: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * out: A pointer to an output buffer.
 *
 * n_out: The maximum number of elements the output buffer can hold.
 *
 * Return value: 1 if the entire tuple was copied into the output buffer,
 * 0 otherwise.
 */
int settings_get_int_tuple(const Settings *settings, const char *section, const char *key, int *out, unsigned int n_out);

/*
 * Returns the long tuple associated with the supplied key in the
 * provided section.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 *
 * section: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * key: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * out: A pointer to an output buffer.
 *
 * n_out: The maximum number of elements the output buffer can hold.
 *
 * Return value: 1 if the entire tuple was copied into the output buffer,
 * 0 otherwise.
 */
long settings_get_long_tuple(const Settings *settings, const char *section, const char *key, long *out, unsigned int n_out);

/*
 * Returns the double tuple associated with the supplied key in the
 * provided section.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 *
 * section: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * key: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * out: A pointer to an output buffer.
 *
 * n_out: The maximum number of elements the output buffer can hold.
 *
 * Return value: 1 if the entire tuple was copied into the output buffer,
 * 0 otherwise.
 */
double settings_get_double_tuple(const Settings *settings, const char *section, const char *key, double *out, unsigned int n_out);

/*
 * Associates a value with the supplied key in the provided section.
 * If the key is already associated with a value, the previous value
 * is replaced.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 *
 * section: A pointer to a null-terminated C string. This parameter cannot
 * be null. The string must have a string length > 0. The string will
 * be copied.
 *
 * key: A pointer to a null-terminated C string. This parameter
 * cannot be null. The string must have a string length > 0. The
 * string will be copied.
 *
 * value: A pointer to a null-terminated C string. This parameter
 * cannot be null. The string must have a string length > 0. The
 * string will be copied.
 *
 * Return value: 1 if the association succeeded, 0 otherwise.
 */
int settings_set(Settings *setting, const char *section, const char *key, const char *value);

/*
 * Returns the number of associations between keys and values that exist
 * in the provided section.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 *
 * section: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * Return value: The number of associations between keys and values in
 * the provided section.
 */
int settings_section_get_count(const Settings *settings, const char *section);

/*
 * Enumerates all associations between keys and values in the provided
 * section.
 *
 * Parameters:
 *
 * settings: A pointer to a settings object. This parameter cannot be null.
 *
 * section: A pointer to a null-terminated C string. This parameter cannot
 * be null.
 *
 * enum_func: A pointer to a callback function that will be
 * called by this procedure once for every key associated
 * with a value. This parameter cannot be null.
 *
 * obj: A pointer to a client-specific object. This parameter will be
 * passed back to the client's callback function. This parameter can
 * be null.
 *
 * Return value: 1 if enumeration completed, 0 otherwise.
 */
int settings_section_enum(const Settings *settings, const char *section, settings_section_enum_func enum_func, const void *obj);

#ifdef __cplusplus
}
#endif

#endif
