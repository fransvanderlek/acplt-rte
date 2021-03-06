/*
*   $Id: ov_config.h,v 1.13 2011-07-15 13:36:46 gustavo Exp $
*
*   Copyright (C) 1998-1999
*   Lehrstuhl fuer Prozessleittechnik,
*   RWTH Aachen, D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package which is licensed as open
*   source under the Artistic License; you can use, redistribute and/or
*   modify it under the terms of that license.
*
*   You should have received a copy of the Artistic License along with
*   this Package; see the file ARTISTIC-LICENSE. If not, write to the
*   Copyright Holder.
*
*   THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
*   WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
*   OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*/
/*
*	History:
*	--------
*	02-Jul-1998 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#ifndef OV_CONFIG_H_INCLUDED
#define OV_CONFIG_H_INCLUDED

/*
*	Plattforms
*	----------
*/
#ifdef __NT__
#define OV_SYSTEM_NT 1
#endif

#ifdef CYGWIN
#define OV_SYSTEM_CYGWIN 1
#endif

#ifndef OV_SYSTEM_HPUX
#define OV_SYSTEM_HPUX 0
#endif

#ifndef OV_SYSTEM_LINUX
#define OV_SYSTEM_LINUX 0
#endif

#ifndef OV_SYSTEM_NT
#define OV_SYSTEM_NT 0
#endif

#ifndef OV_SYSTEM_CYGWIN
#define OV_SYSTEM_CYGWIN 0
#endif

#ifndef OV_SYSTEM_OPENVMS
#define OV_SYSTEM_OPENVMS 0
#endif

#ifndef OV_SYSTEM_SOLARIS
#define OV_SYSTEM_SOLARIS 0
#endif

#ifndef OV_SYSTEM_MC164
#define OV_SYSTEM_MC164 0
#endif

#ifndef OV_SYSTEM_RMOS
#define OV_SYSTEM_RMOS 0
#endif

#if OV_SYSTEM_LINUX + OV_SYSTEM_HPUX + OV_SYSTEM_NT + OV_SYSTEM_CYGWIN + OV_SYSTEM_OPENVMS + OV_SYSTEM_SOLARIS + OV_SYSTEM_MC164 + OV_SYSTEM_RMOS != 1
#error Must define exactly one system!
#endif

#if OV_SYSTEM_LINUX || OV_SYSTEM_HPUX || OV_SYSTEM_SOLARIS || OV_SYSTEM_CYGWIN
#define OV_SYSTEM_UNIX 1
#else
#define OV_SYSTEM_UNIX 0
#endif


/* TLSF Realtime memory allocator, used for database, memstack and heap */
#ifndef TLSF_HEAP
#define TLSF_HEAP 0
#endif

#ifndef OV_RT
#define OV_RT 0
#endif

#define TLSF_STATISTIC 1

/*
*	Compiler
*	--------
*/
#ifdef __GNUC__
#define OV_COMPILER_GCC __GNUC__
#endif

#ifndef OV_COMPILER_GCC
#define OV_COMPILER_GCC 0
#endif

#if defined(__CYGWIN32__) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW__)
#define OV_COMPILER_CYGWIN 1
#endif

#ifndef OV_COMPILER_CYGWIN
#define OV_COMPILER_CYGWIN 0
#endif

#ifdef _MSC_VER
#define OV_COMPILER_MSVC _MSC_VER
#endif

#ifndef OV_COMPILER_MSVC
#define OV_COMPILER_MSVC 0
#endif

#ifdef __BORLANDC__
#define OV_COMPILER_BORLAND __BORLANDC__
#endif

#ifdef __C166__
#define OV_COMPILER_KEIL __C166__
#endif

#ifndef OV_COMPILER_BORLAND
#define OV_COMPILER_BORLAND 0
#endif

#ifdef __DECCXX
#define OV_COMPILER_DECCXX __DECCXX
#endif

#ifndef OV_COMPILER_DECCXX
#define OV_COMPILER_DECCXX 0
#endif

#ifndef OV_COMPILER_KEIL
#define OV_COMPILER_KEIL 0
#endif

#if OV_COMPILER_BORLAND
#pragma warn -sig
#pragma warn -par
#endif

#if OV_COMPILER_KEIL
#ifndef __MODEL__
#define __MODEL__	6	/* HLARGE memory model */
#endif
#pragma warning disable = 47
#pragma warning disable = 138
#endif

/*
*	Definitions based on environment
*	--------------------------------
*/
#if OV_SYSTEM_UNIX
#define OV_DYNAMIC_DATABASE		0
#define OV_MEMSPEC
#define OV_DLLFLNSUFFIX			".so"
#if __GNUC__ >= 4
	/* support on embedded systems is unclear (BFLAT) */
	#define OV_DLLFNCEXPORT	__attribute__ ((visibility ("default")))
	#define OV_DLLVAREXPORT	__attribute__ ((visibility ("default")))
#else
	#define OV_DLLFNCEXPORT
	#define OV_DLLVAREXPORT
#endif
#define OV_DLLVARIMPORT 		extern
#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE                      /* Require IEEE Std. 1003.1 (Needed for sigjmp) */
#endif
#endif

#define OV_INLINE inline

#if OV_SYSTEM_NT
#define OV_DYNAMIC_DATABASE		0
#define OV_MEMSPEC
#define OV_DLLFLNSUFFIX			".dll"
#if OV_COMPILER_MSVC
#define OV_DLLFNCEXPORT 		_declspec(dllexport)
#define OV_DLLVAREXPORT         _declspec(dllexport)
#define OV_DLLVARIMPORT 		_declspec(dllimport)
#else
#if OV_COMPILER_BORLAND || OV_COMPILER_CYGWIN
#define OV_DLLFNCEXPORT 		__declspec(dllexport)
#define OV_DLLVAREXPORT 		__declspec(dllexport)
#define OV_DLLVARIMPORT 		extern __declspec(dllimport)
#else
#define OV_DLLFNCEXPORT         
#define OV_DLLVAREXPORT         
#define OV_DLLVARIMPORT 		extern
#endif
#endif
#endif
#if OV_SYSTEM_OPENVMS
#define OV_DYNAMIC_DATABASE		0
#define OV_MEMSPEC
#define OV_DLLFLNSUFFIX			""
#define OV_DLLFNCEXPORT
#define OV_DLLVAREXPORT
#define OV_DLLVARIMPORT 		extern
#endif

#if OV_SYSTEM_RMOS
#define OV_DYNAMIC_DATABASE		0
#define OV_MEMSPEC
#define OV_DLLFLNSUFFIX			""
#define OV_DLLFNCEXPORT
#define OV_DLLVAREXPORT
#define OV_DLLVARIMPORT 		extern
#endif

#if OV_SYSTEM_MC164
#define OV_DYNAMIC_DATABASE		0
#define OV_MEMSPEC				xhuge
#define OV_DLLFLNSUFFIX			""
#define OV_DLLFNCEXPORT
#define OV_DLLVAREXPORT
#define OV_DLLVARIMPORT 		extern
#endif

/*
*   C-Includes
*   ----------
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#if OV_COMPILER_MSVC
	#if (OV_COMPILER_MSVC > 1500 && OV_COMPILER_MSVC < 1800)
		//1500 is VS 2008
		#include <stdint.h>
	#endif
	#if OV_COMPILER_MSVC >= 1800
		//1800 is VS 2013
		#include <inttypes.h>	//includes stdint.h itself
	#endif
#elif OV_COMPILER_GCC || OV_COMPILER_CYGWIN
	#include <inttypes.h>	//includes stdint.h itself
#endif

#if OV_SYSTEM_NT
#define i386 1
#ifndef WIN32
#define WIN32 1
#endif
#define mc68000 0
#define sparc 0
#define vax 0
#if PLT_USE_WINSOCK2
#include <winsock2.h>
#else
#include <winsock.h>
#endif
#endif

#if OV_COMPILER_MSVC
#include <winbase.h>
#endif
#if OV_SYSTEM_MC164
typedef struct { int quot; int rem; } div_t;
div_t div(int x, int y);
unsigned long strtoul(const char *nptr, char **endptr, int base);
char *strdup(const char *s);
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif

#if OV_SYSTEM_RMOS
#define	offsetof(type, member) ((size_t)&(((type*)0)->member))
#endif

/*
*	Bool datatype
*	-------------
*/
#if OV_SYSTEM_MC164
#define bool_t long
#endif
#ifndef bool_t
#define bool_t int
#endif

/*
*	Enum datatype
*	-------------
*/
#if OV_SYSTEM_MC164
#define enum_t long
#endif
#ifndef enum_t
	//hopefully no one is manipulating an enum binary, so it is ok to be 32 or 64 bit
	#define	enum_t	int
#endif

/*
*	Definitions about static/dynamic libraries
*	------------------------------------------
*/
#ifndef OV_STATIC_LIBRARIES
#define OV_STATIC_LIBRARIES 0
#endif
#ifndef OV_DYNAMIC_LIBRARIES
#define OV_DYNAMIC_LIBRARIES 0
#endif

/*
*	Definitions about dynamic databases
*	-----------------------------------
*/
#ifndef OV_DYNAMIC_DATABASE
#define OV_DYNAMIC_DATABASE 0
#endif

#if OV_SYSTEM_UNIX && !OV_RT
#ifndef USE_SBRK
#define USE_SBRK 1
#endif
#ifndef USE_MMAP
#define USE_MMAP 1
#endif
#else
/* disable memory allocation system calls for RT */
#define USE_SBRK 0
#define USE_MMAP 0
#endif

#if OV_SYSTEM_NT && !OV_RT
#ifndef USE_VIRTALLOC
#define USE_VIRTALLOC 1
#endif
#else
/* disable memory allocation system calls for RT */
#define USE_VIRTALLOC 0
#endif

/*
*	Definitions about mutex
*	-----------------------------------
*/
#ifndef OV_SYNC_PTHREAD
#define OV_SYNC_PTHREAD 0
#endif
#if !OV_SYNC_PTHREAD
#ifndef OV_SYNC_NTMUTEX
#define OV_SYNC_NTMUTEX 0
#endif
#endif
#if OV_SYNC_PTHREAD || OV_SYNC_NTMUTEX
#define OV_SYNC_MUTEX 1
#define TLSF_USE_LOCKS 1
#else
#define OV_SYNC_MUTEX 0
#define TLSF_USE_LOCKS 0
#endif

/*
*   Common definitions
*	------------------
*/
#ifndef NULL
#define NULL (0L)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (1)
#endif




#endif

/*
*	End of file
*/

