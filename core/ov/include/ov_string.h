/*
*   $Id: ov_string.h,v 1.7 2011-01-18 14:27:34 martin Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package 
*   
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*/
/*
*	History:
*	--------
*	20-Jan-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#ifndef OV_STRING_H_INCLUDED
#define OV_STRING_H_INCLUDED

#include "ov_ov.h"
#include "ov_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
*	enumeration of possible memory locations
*/
enum ov_allocator{
	ov_alloc_database,
	ov_alloc_heap,
	ov_alloc_stack
};

/*
*	OV_STRINGCOMPARE:
*	----------------
*	Result of function ov_string_compare(s1, s2)
*/
#define	OV_STRCMP_EQUAL	0		/* s1 = s2 */
#define	OV_STRCMP_MORE		1		/* s1 > s2 */
#define	OV_STRCMP_LESS		(-1)		/* s1 < s2 */

/**
*	Set value of a string in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_setvalue(
	OV_STRING			*pstring,
	const OV_STRING		value
);

/**
*	Set value of a string in selected memory
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_setvalue_allocator(
	OV_STRING			*pstring,
	enum ov_allocator	allocator,
	const OV_STRING		value
);

/**
*	Set value of a string vector in the database
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_setvecvalue(
	OV_STRING			*pstringvec,
	const OV_STRING		*pvalue,
	OV_UINT				veclen
);

/**
*	Compare two strings, result is greater than, equal to or less than zero
*	Please check against OV_STRCMP_EQUAL, OV_STRCMP_MORE and OV_STRCMP_LESS
*	OV_STRCMP_MORE indicates that the first character that does not match has a greater value in string1 than in string2
*	Note: NULL is considered equal to ""
*
*/
OV_DLLFNCEXPORT OV_INT ov_string_compare(
	const OV_STRING		string1,
	const OV_STRING		string2
);

/**
 * Same as ov_string_compare but not case_sensitive
 * upper case comparison is used
 */
OV_DLLFNCEXPORT OV_INT ov_string_comparei(const OV_STRING s1, const OV_STRING s2);


/**
*	Get the length of a string (without the null terminator)
*	results zero if got a NULL pointer
*/
OV_DLLFNCEXPORT OV_UINT ov_string_getlength(
	const OV_STRING		string
);

/**
*	Append a string to an existing one in the database
*	appending a NULL pointer results in a OV_ERR_BADPARAM
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_append(
	OV_STRING			*pstring,
	const OV_STRING		appstring
);

/**
*	Append a string to an existing one in selected memory
*	appending a NULL pointer results in a OV_ERR_BADPARAM
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_append_allocator(
	OV_STRING			*pstring,
	enum ov_allocator	allocator,
	const OV_STRING		appstring
);

/**
*	Formatted print to a string
*	crashes if a used value contains a NULL pointer
*/
OV_DLLFNCEXPORT OV_RESULT ov_string_print(
	OV_STRING		*pstring,
	const OV_STRING	format,
	...
);

OV_DLLFNCEXPORT OV_RESULT ov_string_heap_print(
	OV_STRING		*pstring,
	const OV_STRING	format,
	...
);

OV_DLLFNCEXPORT OV_RESULT ov_string_stack_print(
	OV_STRING		*pstring,
	const OV_STRING	format,
	...
);

/**
*	Test if a string matches a regular expression
*	? is an arbitrary character
*	* is an arbitrary amount of any characters
*/
OV_DLLFNCEXPORT OV_BOOL ov_string_match(
	const OV_STRING		string,
	const OV_STRING		mask
);

/**
*	Convert a string to lower case
*	Note: you must call ov_memstack_lock/unlock() outside of this function!
*/
OV_DLLFNCEXPORT OV_STRING ov_string_tolower(
	const OV_STRING		string
);

/**
*	Convert a string to upper case
*	Note: you must call ov_memstack_lock/unlock() outside of this function!
*/
OV_DLLFNCEXPORT OV_STRING ov_string_toupper(
	const OV_STRING		string
);

/**
*	Helper function of ov_string_match() for sets (subroutine)
*/
int ov_string_match_set(
	const OV_STRING		mask,
	const OV_STRING		chain,
	int					*pm,
	int					*pk
);

/**
*	Helper function of ov_string_match() for jokers (subroutine)
*/
int ov_string_match_joker(
	const OV_STRING		mask,
	const OV_STRING		chain,
	int					*pm,
	int					*pk
);

/**
*	Split a string into the heap
*
*	ov_string_split("a;b;c",";",&len) => len is 3
*	ov_string_split(";",";",&len) => len is 2
*	ov_string_split("a",";",&len) => len is 1
*	ov_string_split(NULL,";",&len) => len is 0
*	---------------
*	OV_STRING *plist = NULL;
*	OV_UINT    i,len;
*	
*	plist = ov_string_split("a;b;c",";",&len);
*	for(i=0; i<len; i++) {
*		fprintf(stderr, "%lu : %s\n", i, plist[i]);
*	}
*	ov_string_freelist(plist);
*/
OV_DLLFNCEXPORT OV_STRING *ov_string_split(
	const OV_STRING		str,
	const OV_STRING		sep,
	OV_UINT             *len
);

/**
 * frees a list in the heap
 * checks for a NULL pointer, but does not prevent double freeing
 */
OV_DLLFNCEXPORT void ov_string_freelist(
	OV_STRING *plist
);

#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif
/*
*	End of file
*/

