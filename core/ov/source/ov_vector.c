/*
*   $Id: ov_vector.c,v 1.10 2008-06-18 15:08:47 martin Exp $
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
*	20-Jan-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: File created.
*	13-Apr-1999 Dirk Meyer <dirk@plt.rwth-aachen.de>: Major revision.
*/

#define OV_COMPILE_LIBOV

#include "ov_vector.h"
#include "ov_database.h"
#include "ov_string.h"
#include "ov_macros.h"

#if OV_SYSTEM_MC164
#define memcpy xmemcpy
#define memset xmemset
#define memcmp xmemcmp
#endif

/*	----------------------------------------------------------------------	*/

OV_DLLFNCEXPORT OV_RESULT ov_vector_setstaticvalue(
	OV_POINTER			pvector,
	const OV_POINTER	pvalue,
	const OV_UINT		veclen,
	const OV_UINT		size,
	const OV_VAR_TYPE	vartype
) {
	/*
	*	check parameters
	*/
	if(!pvector || !pvalue) {
		return OV_ERR_BADPARAM;
	}
	/*
	*	set the value
	*/
	if((vartype & OV_VT_KSMASK) == OV_VT_STRING) {
		return ov_string_setvecvalue((OV_STRING*)pvector,
			(OV_STRING*)pvalue, veclen);
	}
	memcpy(pvector, pvalue, size);
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set the value of a dynamic vector variable
*/
OV_DLLFNCEXPORT OV_RESULT ov_vector_setdynamicvalue(
	OV_GENERIC_VEC		*pvector,
	const OV_POINTER	pvalue,
	const OV_UINT		veclen,
	const OV_UINT		size,
	const OV_VAR_TYPE	vartype
) {
	/*
	*	local variables
	*/
	OV_POINTER	*pnewvalue;
	OV_RESULT	result;
	OV_UINT		tempmaxVecLen	=	0;
	/*
	*	check parameters
	*/
	if(!pvector) {
		return OV_ERR_BADPARAM;
	}
	tempmaxVecLen = ov_vendortree_MaxVectorLength();
	if(tempmaxVecLen && veclen > tempmaxVecLen){
		return OV_ERR_BADVALUE;
	}
	/*
	*	manipulate memory and set the actual vector value
	*/
	if(pvector->veclen && ((vartype & OV_VT_KSMASK) == OV_VT_STRING)) {
		/*
		*	free strings
		*/
		Ov_WarnIfNot(Ov_OK(ov_string_setvecvalue((OV_STRING*)pvector->value,
			NULL, pvector->veclen)));
	}
	if(!veclen) {
		if(pvector->value) {
			ov_database_free(pvector->value);
			pvector->veclen = 0;
			pvector->value = NULL;
		}
		return OV_ERR_OK;
	}
	if(pvector->veclen != veclen) {
		/*
		*	vector length has changed, reallocate memory
		*/
		pnewvalue = ov_database_realloc(pvector->value, size);
		if(!pnewvalue) {
			return OV_ERR_DBOUTOFMEMORY;
		}
		pvector->veclen = veclen;
		pvector->value = pnewvalue;
	}
	if((vartype & OV_VT_KSMASK) == OV_VT_STRING) {
		memset(pvector->value, 0, size);
		result = ov_string_setvecvalue((OV_STRING*)pvector->value,
			(OV_STRING*)pvalue, veclen);
		if(Ov_Fail(result)) {
			Ov_WarnIfNot(Ov_OK(ov_vector_setdynamicvalue(pvector, NULL, 0, 0, 0)));
			return result;
		}
	} else {
		memcpy(pvector->value, pvalue, size);
	}
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Set the vector length of a dynamic vector variable value
*/
OV_DLLFNCEXPORT OV_RESULT ov_vector_setdynamicveclen(
	OV_GENERIC_VEC		*pvector,
	const OV_UINT		veclen,
	const OV_UINT		size,
	const OV_VAR_TYPE	vartype
) {
	/*
	*	local variables
	*/
	OV_UINT		i;
	OV_STRING	*pstring;
	OV_UINT		oldsize;
	OV_POINTER	*pnewvalue;
	OV_UINT		tempMaxVecLen	=	0;
	/*
	*	check parameters
	*/
	if(!pvector) {
		return OV_ERR_BADPARAM;
	}
	tempMaxVecLen = ov_vendortree_MaxVectorLength();
	if(tempMaxVecLen && veclen > tempMaxVecLen){
		return OV_ERR_BADVALUE;
	}
	/*
	*	manipulate memory
	*/
	if((vartype & OV_VT_KSMASK) == OV_VT_STRING) {
		/*
		*	free strings that will be erased
		*/
		if(veclen<pvector->veclen) {
    		for(i=veclen; i<pvector->veclen; i++) {
        	    pstring=((OV_STRING*)pvector->value) + i;
            	Ov_WarnIfNot(Ov_OK(ov_string_setvalue(pstring, NULL)));
        	}
        }
	}
	if(!veclen) {
		if(pvector->value) {
			ov_database_free(pvector->value);
			pvector->veclen = 0;
			pvector->value = NULL;
		}
		return OV_ERR_OK;
	}
	/*
	*	calculate old size
	*/
	oldsize = (size/veclen)*pvector->veclen;
	/*
	*	vector length has changed, reallocate memory
	*/
	if(pvector->veclen != veclen) {
		pnewvalue = ov_database_realloc(pvector->value, size);
		if(!pnewvalue) {
			return OV_ERR_DBOUTOFMEMORY;
		}
		pvector->veclen = veclen;
		pvector->value = pnewvalue;
		/*
		*	set new allocated memory to zero
		*/
		if(size > oldsize) {
			memset(((OV_BYTE*)pvector->value)+oldsize, 0, size-oldsize);
		}
	}
	return OV_ERR_OK;
}
	
/*	----------------------------------------------------------------------	*/

/*
*	Compare two vector variable values, result is greater than, equal to or less than zero
*/
OV_DLLFNCEXPORT OV_INT ov_vector_compare(
	const OV_POINTER	pvalue1,
	const OV_POINTER	pvalue2,
	const OV_UINT		veclen,
	const OV_UINT		size,
	const OV_VAR_TYPE	vartype
) {
	/*
	*	local variables
	*/
	OV_UINT		i;
	OV_STRING	*pstring1, *pstring2;
	OV_INT		result;
	/*
	*	compare values
	*/
	if(pvalue1) {
		if(pvalue2) {
			if((vartype & OV_VT_KSMASK) == OV_VT_STRING) {
				for(i=0, pstring1=(OV_STRING*)pvalue1, pstring2=(OV_STRING*)pvalue2;
					i<veclen; i++, pstring1++, pstring2++
				) {
					result = ov_string_compare(*pstring1, *pstring2);
					if(result) {
						return result;
					}
				}
				return OV_VECCMP_EQUAL;
			}
			return memcmp(pvalue1, pvalue2, size);
		}
		return OV_VECCMP_MORE;
	}
	if(pvalue2) {
		return OV_VECCMP_LESS;
	}
	return OV_VECCMP_EQUAL;
}

/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/

