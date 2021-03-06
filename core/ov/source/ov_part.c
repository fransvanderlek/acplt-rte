/*
*   $Id: ov_part.c,v 1.3 2002-01-23 13:44:14 ansgar Exp $
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
*	09-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*/

#define OV_COMPILE_LIBOV

#include "ov_part.h"
#include "ov_logfile.h"
#include "ov_macros.h"

/*	----------------------------------------------------------------------	*/

/*
*	Load a part into the database
*/
OV_RESULT ov_part_load(
	OV_PART_DEF*			ppartdef,
	OV_INSTPTR_ov_domain	pparent
) {
	/*
	*	local variables
	*/
	OV_RESULT			result;
	OV_INSTPTR_ov_part	ppart;
	OV_INSTPTR_ov_class	ppartclass;
	/*
	*	look for the class object of the part's class
	*/
	ppartclass = ov_class_search(ppartdef->partclassname);
	if(!ppartclass) {
		return OV_ERR_UNKNOWNCLASSDEF;
	}
	/*
	*	create part object
	*/
	result = Ov_CreateObject(ov_part, ppart, pparent, ppartdef->identifier);
	if(Ov_Fail(result)) {
		return result;
	}
	/*
	*	set parameters
	*/
	ppart->v_offset = ppartdef->offset;
	ppart->v_flags = ppartdef->flags;
	/*
	*	link with class of the component
	*/
	Ov_WarnIfNot(Ov_OK(Ov_Link(ov_embedment, ppartclass, ppart)));
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Compare a part with its definition
*/
OV_RESULT ov_part_compare(
	OV_INSTPTR_ov_part	ppart,
	OV_PART_DEF*		ppartdef
) {
	/*
	*	local variables
	*/
	OV_INSTPTR_ov_class	pclass;
	OV_STRING			id, libid;
	/*
	*	compare class of the component
	*/
	pclass = Ov_GetParent(ov_embedment, ppart);
	if(!pclass) {
		goto ERRORMSG;
	}
	libid = Ov_GetParent(ov_containment, pclass)->v_identifier;
	id = (OV_STRING)Ov_HeapMalloc(strlen(libid)+1+strlen(pclass->v_identifier)+1);
	if(!id) {
		return OV_ERR_HEAPOUTOFMEMORY;
	}
	sprintf(id, "%s/%s", libid, pclass->v_identifier);
	if(strcmp(ppartdef->partclassname, id)) {
		Ov_HeapFree(id);
		goto ERRORMSG;
	}
	Ov_HeapFree(id);
	/*
	*	compare part attributes
	*/
	if(strcmp(ppart->v_identifier, ppartdef->identifier)
		|| (ppart->v_offset != ppartdef->offset) || (ppart->v_flags != ppartdef->flags)

	) {
		goto ERRORMSG;
	}
	/*
	*	all is fine
	*/
	return OV_ERR_OK;
	/*
	*	print error message and exit
	*/
ERRORMSG:
	ov_logfile_error("Error loading part \"%s/%s/%s\".",
		Ov_GetParent(ov_containment, Ov_GetParent(ov_containment, ppart))->v_identifier,
		Ov_GetParent(ov_containment, ppart)->v_identifier,
		ppart->v_identifier);
	return OV_ERR_PARTDEFMISMATCH;
}

/*	----------------------------------------------------------------------	*/

/*
*	Test if we can unload a part from the database
*/
OV_BOOL ov_part_canunload(
	OV_INSTPTR_ov_part	ppart
) {
	return TRUE;
}

OV_DLLFNCEXPORT OV_UINT ov_part_flags_get(
	OV_INSTPTR_ov_part	ppart
) {
	return ppart->v_flags;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

