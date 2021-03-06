/*
*   $Id: ov_logfile.c,v 1.6 2001-07-09 12:50:00 ansgar Exp $
*
*   Copyright (C) 1998
*   Lehrstuhl fuer Prozessleittechnik,
*   D-52056 Aachen, Germany.
*   All rights reserved.
*
*   Author: Dirk Meyer <dirk@plt.rwth-aachen.de>
*
*   This file is part of the ACPLT/OV Package 
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
*	14-May-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	07-Jun-2001 J.Nagelmann <nagelmann@ltsoft.de>: Changes for Sun Solaris.
*/

#define OV_COMPILE_LIBOV

#include "ov_logfile.h"
#include "ov_time.h"
#include "ov_macros.h"

#include <stdarg.h>

#if OV_SYSTEM_MC164
#define memset xmemset
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Constants
*/
#if OV_SYSTEM_NT
#define OV_REGISTRY_KEY		"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\"
#define OV_REGISTRY_NAME1	"EventMessageFile"
#define OV_REGISTRY_VALUE1	"%SystemRoot%\\system32\\libov.dll"
#define OV_REGISTRY_NAME2	"TypesSupported"
#define OV_REGISTRY_VALUE2	EVENTLOG_INFORMATION_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_ERROR_TYPE
#endif

/*	----------------------------------------------------------------------	*/

/*
*	Global variables
*/
static char			msg[1024];

/**
 * bufidx identifies the next free entry or is MAXMSGS if not initialised yet
 */
static OV_UINT		bufidx = OV_LOGFILE_MAXMSGS;
static OV_STRING	msgbuf[OV_LOGFILE_MAXMSGS];
static OV_TIME		timebuf[OV_LOGFILE_MAXMSGS];
static OV_STRING	id = NULL;

#if !OV_SYSTEM_MC164
static FILE			*logfile = NULL;
#endif

#if OV_SYSTEM_NT
static FILE 		ntlog;
static HANDLE		eventsource = NULL;
#endif

/*	----------------------------------------------------------------------	*/

/**
*	Open/create a logfile
*	@param ident will be "ACPLT/OV" if NULL
*	@param filename
*	@param mode will be passed to fopen, for example "w"
*/
OV_DLLFNCEXPORT OV_RESULT ov_logfile_open(
	const OV_STRING	ident,
	OV_STRING		filename,
	OV_STRING		mode
) {
	id = ident;
#if !OV_SYSTEM_MC164
	/*
	*	test if logfile already open
	*/
	if(logfile) {
		ov_logfile_close();
	}
	logfile = stdout;
	/*
	*	open a new logfile
	*/
	logfile = fopen(filename, mode);
	if(!logfile) {
		return OV_ERR_CANTOPENFILE;
	}
#endif
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/**
*	Close the logfile
*/
OV_DLLFNCEXPORT void ov_logfile_close(void) {
#if !OV_SYSTEM_MC164
	if(logfile) {
		if((logfile != stdout) && (logfile != stderr)
#if OV_SYSTEM_NT
			&& (logfile != &ntlog)
#endif
		) {
			fclose(logfile);
		}
		logfile = NULL;
#if OV_SYSTEM_NT
		if(eventsource) {
			DeregisterEventSource(eventsource);
			eventsource = NULL;
		}
#endif
	}
#endif
}

/*	----------------------------------------------------------------------	*/

/**
*	Log to stdout
*	@param ident will be "ACPLT/OV" if NULL
*/
OV_DLLFNCEXPORT void ov_logfile_logtostdout(
	const OV_STRING	ident
) {
	id = ident;
#if !OV_SYSTEM_MC164
	if(logfile) {
		ov_logfile_close();
	}
	logfile = stdout;
#endif
}

/*	----------------------------------------------------------------------	*/

/**
*	Log to stderr
*	@param ident will be "ACPLT/OV" if NULL
*/
OV_DLLFNCEXPORT void ov_logfile_logtostderr(
	const OV_STRING	ident
) {
	id = ident;
#if !OV_SYSTEM_MC164
	if(logfile) {
		ov_logfile_close();
	}
	logfile = stderr;
#endif
}

/*	----------------------------------------------------------------------	*/

/**
*	Log to the NT logger (Windows NT only)
*	@param ident will be "ACPLT/OV" if NULL
*/
#if OV_SYSTEM_NT
OV_DLLFNCEXPORT void ov_logfile_logtontlog(
	const OV_STRING	ident
) {
	/*
	*	local variables
	*/
	HKEY 		hkey;
	DWORD		value2 = OV_REGISTRY_VALUE2;
	OV_STRING	key;
	OV_UINT		size;
	/*
	*	register the message file in the registry
	*/
	size = strlen(OV_REGISTRY_KEY)+((id)?(strlen(id)):(strlen("ACPLT/OV")))+1;
	key = (OV_STRING)Ov_HeapMalloc(size);
	if(key) {
		sprintf(key, "%s%s", OV_REGISTRY_KEY, ((id)?(id):("ACPLT/OV")));
		if(RegCreateKey(HKEY_LOCAL_MACHINE, key, &hkey) == ERROR_SUCCESS) {
			RegSetValueEx(hkey, OV_REGISTRY_NAME1, 0, REG_EXPAND_SZ,
				(LPBYTE)OV_REGISTRY_VALUE1, strlen(OV_REGISTRY_VALUE1)+1);
			RegSetValueEx(hkey, OV_REGISTRY_NAME2, 0, REG_DWORD,
				(LPBYTE)&value2, sizeof(DWORD));
			RegCloseKey(hkey);
		}
		Ov_HeapFree(key);
	}
	/*
	*	create event source
	*/
	id = ident;
	if(logfile) {
		ov_logfile_close();
	}
	logfile = &ntlog;
	eventsource = RegisterEventSource(0, ((id)?(id):("ACPLT/OV")));
	if(!eventsource) {
		ov_logfile_close();
	}
}
#endif

/*	----------------------------------------------------------------------	*/

/**
*	Print text to logfile
*/
OV_DLLFNCEXPORT void ov_logfile_print(
	OV_MSG_TYPE		msgtype,
	const OV_STRING	msg
) {
	/*
	*	local variables
	*/
	OV_STRING	ident;
	OV_STRING	typetext;
	OV_STRING	output;
	/*
	*	create full message
	*/
	if(id) {
		ident = id;
	} else {
		ident = "ACPLT/OV";
	}
	switch(msgtype) {
	case OV_MT_INFO:
		typetext = "Info";
		break;
	case OV_MT_DEBUG:
		typetext = "Debug";
		break;
	case OV_MT_WARNING:
		typetext = "Warning";
		break;
	case OV_MT_ERROR:
		typetext = "Error";
		break;
	case OV_MT_ALERT:
		typetext = "Alert";
		break;
	default:
		typetext = "(unknown message type)";
		break;
	}
	output = (OV_STRING)Ov_HeapMalloc(strlen(ident)+strlen(typetext)+strlen(msg)+5);
	if(!output) {
		return;
	}
	sprintf(output, "[%s %s] %s", ident, typetext, msg);
	/*
	*	enter message into the buffer
	*/
	if(bufidx == OV_LOGFILE_MAXMSGS) {
		/*
		*	initialize the buffer
		*/
		memset(msgbuf, 0, sizeof(msgbuf));
		bufidx = 0;
	}
	msgbuf[bufidx] = (OV_STRING)Ov_HeapRealloc(msgbuf[bufidx], strlen(output)+1);
	strcpy(msgbuf[bufidx], output);
	ov_time_gettime(&timebuf[bufidx]);
	bufidx++;
	if(bufidx == OV_LOGFILE_MAXMSGS) {
		//we are wrapped around and have to start the index again
		bufidx = 0;
	}
#if OV_SYSTEM_NT
	if(logfile == &ntlog) {
		LPCTSTR	messages[1];
		messages[0] = (LPCTSTR)output;
		/*
		*	report the message to the NT Logger
		*/
		switch(msgtype) {
		case OV_MT_WARNING:
			ReportEvent(eventsource, EVENTLOG_WARNING_TYPE,
				0, msgtype, 0, 1, 0, messages, 0);
			Ov_HeapFree(output);
			return;
		case OV_MT_ERROR:
			ReportEvent(eventsource, EVENTLOG_ERROR_TYPE,
				0, msgtype, 0, 1, 0, messages, 0);
			Ov_HeapFree(output);
			return;
		case OV_MT_INFO:
		case OV_MT_DEBUG:
		case OV_MT_ALERT:
		default:
			ReportEvent(eventsource, EVENTLOG_INFORMATION_TYPE,
				0, msgtype, 0, 1, 0, messages, 0);
			Ov_HeapFree(output);
			return;
		}
	}
#endif
#if !OV_SYSTEM_MC164
	/*
	*	print the message to the log file
	*/
	if(logfile) {
		fprintf(logfile, "%s\n", output);
		/*
		 *	added to handle output for eclipse consoles on windows and file output on linux
		 * 	workaround for the bug https://bugs.eclipse.org/bugs/show_bug.cgi?id=173732
		 *	we need to see whether this has performance issues
		*/
		fflush(logfile);
	}
#endif
	Ov_HeapFree(output);
}

/*	----------------------------------------------------------------------	*/

/**
*	Print info to logfile
*/
OV_DLLFNCEXPORT void ov_logfile_info(
	const OV_STRING	format,
	...
) {
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_UNIX && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	va_end(args);
	ov_logfile_print(OV_MT_INFO, msg);
}

/*	----------------------------------------------------------------------	*/

/**
*	Print debug info to logfile
*/
OV_DLLFNCEXPORT void ov_logfile_debug(
	const OV_STRING	format,
	...
) {
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_UNIX  && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	va_end(args);
	ov_logfile_print(OV_MT_DEBUG, msg);
}

/*	----------------------------------------------------------------------	*/

/**
*	Print warning to logfile
*/
OV_DLLFNCEXPORT void ov_logfile_warning(
	const OV_STRING	format,
	...
) {
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_UNIX  && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	va_end(args);
	ov_logfile_print(OV_MT_WARNING, msg);
}

/*	----------------------------------------------------------------------	*/

/**
*	Print error to logfile
*/
OV_DLLFNCEXPORT void ov_logfile_error(
	const OV_STRING	format,
	...
) {
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_UNIX  && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	va_end(args);
	ov_logfile_print(OV_MT_ERROR, msg);
}

/*	----------------------------------------------------------------------	*/

/**
*	Print alert to logfile
*/
OV_DLLFNCEXPORT void ov_logfile_alert(
	const OV_STRING	format,
	...
) {
	/*
	*	local variables
	*/
	va_list	args;
	/*
	*	print text to logfile
	*/
	va_start(args, format);
#if OV_SYSTEM_UNIX  && !OV_SYSTEM_SOLARIS
	vsnprintf(msg, sizeof(msg), format, args);
#else
	vsprintf(msg, format, args);
#endif
	va_end(args);
	ov_logfile_print(OV_MT_ALERT, msg);
}

/*	----------------------------------------------------------------------	*/

/**
*	Get messages from the logfile
*	Note: you must call ov_memstack_lock() and ov_memstack_unlock() outside
*	is able to scan from oldest to newest messages or vice versa (by switching to/from times)
*	@param from
*	@param to
*	@param max_no_messages
*	@param messages pointer to an STRING array. Could be NULL if not interesting
*	@param times pointer to an TIME array. Could be NULL if not interesting
*	@param no_messages number of results in the messages and/or times arrays
*/
OV_DLLFNCEXPORT OV_RESULT ov_logfile_getmessages(
	OV_TIME		*from,
	OV_TIME		*to,
	OV_UINT		max_no_messages,
	OV_STRING	**messages,
	OV_TIME		**times,
	OV_UINT		*no_messages
) {
	/*
	*	local variables
	*/
	OV_UINT		i, j, k, count;
	OV_BOOL		mark[OV_LOGFILE_MAXMSGS];
	OV_INT		step;
	OV_UINT		start;

	if(bufidx == OV_LOGFILE_MAXMSGS) {
		//no logfile used till now
		*no_messages = 0;
		if(messages) {
			*messages = NULL;
		}
		if(times) {
			*times = NULL;
		}
		return OV_ERR_OK;
	}

	/*
	*	determine whether to scan from oldest to newst messages
	*	or vice versa
	*/
	step = ov_time_compare(to, from);
	if(step == OV_TIMECMP_EQUAL) {
		step = OV_TIMECMP_AFTER;
	}
	if(step == OV_TIMECMP_AFTER) {
		start = bufidx;
	} else {
		if(bufidx) {
			start = bufidx-1;
		} else {
			start = OV_LOGFILE_MAXMSGS-1;
		}
	}
	/*
	*	count and mark the messages
	*/
	for(i=0, k=start, count=0; (i<OV_LOGFILE_MAXMSGS) && (count<max_no_messages); i++) {
		mark[k] = FALSE;
		if(msgbuf[k]) {
			if((step*ov_time_compare(&timebuf[k], from) >= 0)
				&& (step*ov_time_compare(&timebuf[k], to) <= 0)
			) {
				mark[k] = TRUE;
				count++;
			}
		}
		if(step == OV_TIMECMP_AFTER) {
			if(k == OV_LOGFILE_MAXMSGS-1) {
				k = 0;
			} else {
				k++;
			}
		} else {
			if(k == 0) {
				k = OV_LOGFILE_MAXMSGS-1;
			} else {
				k--;
			}
		}
	}
	/*
	*	allocate memory for the result
	*/
	if(messages) {
		*messages = (OV_STRING*)ov_memstack_alloc(count*sizeof(OV_STRING));
		if(!*messages) {
			return OV_ERR_HEAPOUTOFMEMORY;
		}
	}
	if(times) {
		*times = (OV_TIME*)ov_memstack_alloc(count*sizeof(OV_TIME));
		if(!*times) {
			return OV_ERR_HEAPOUTOFMEMORY;
		}
	}
	/*
	*	enter the results
	*/
	*no_messages = count;
	for(i=0, j=0, k=start; (i<OV_LOGFILE_MAXMSGS) && (j<count); i++) {
		if(mark[k]) {
			if(messages) {
				(*messages)[j] = msgbuf[k];
			}
			if(times) {
				(*times)[j] = timebuf[k];
			}
			j++;
		}
		if(step > 0) {
			if(k == OV_LOGFILE_MAXMSGS-1) {
				k = 0;
			} else {
				k++;
			}
		} else {
			if(k == 0) {
				k = OV_LOGFILE_MAXMSGS-1;
			} else {
				k--;
			}
		}
	}
	return OV_ERR_OK;
}

/**
 *	frees the heap memory used by the logfile array
 */
OV_DLLFNCEXPORT void ov_logfile_free() {
	OV_UINT runindex = 0;
	if(bufidx == OV_LOGFILE_MAXMSGS) {
		//no logfile used till now
		return;
	}
	//free all pointers. Will be initialized on first usage in ov_logfile_print, so setting to NULL not needed.
	for(runindex = 0;runindex < OV_LOGFILE_MAXMSGS;runindex++){
		Ov_HeapFree(msgbuf[runindex]);
	}
	bufidx = OV_LOGFILE_MAXMSGS;
	return;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

