/*
*	Copyright (C) 2010
*	Chair of Process Control Engineering,
*	Aachen University of Technology.
*	All rights reserved.
*
*	Redistribution and use in source and binary forms, with or without
*	modification, are permitted provided that the following conditions
*	are met:
*	1. Redistributions of source code must retain the above copyright
*	   notice, this list of conditions and the following disclaimer.
*	2. Redistributions in binary form must print or display the above
*	   copyright notice either during startup or must have a means for
*	   the user to view the copyright notice.
*	3. Redistributions in binary form must reproduce the above copyright
*	   notice, this list of conditions and the following disclaimer in
*		the documentation and/or other materials provided with the
*		distribution.
*	4. Neither the name of the Chair of Process Control Engineering nor
*		the name of the Aachen University of Technology may be used to
*		endorse or promote products derived from this software without
*		specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
*	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE CHAIR OF
*	PROCESS CONTROL ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT,
*	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
*	OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
*	AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
*	WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*	POSSIBILITY OF SUCH DAMAGE.
*/
/******************************************************************************
*
*   FILE
*   ----
*   SEL.c
*
*   History
*   -------
*   2010-11-30   File created
*
*******************************************************************************
*
*   This file is generated by the 'fb_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_iec61131stdfb
#define OV_COMPILE_LIBRARY_iec61131stdfb
#endif


#include "iec61131stdfb.h"
#include "stdfb_macros.h"
#include "libov/ov_macros.h"
#include "libov/ov_logfile.h"
#include "helper.h"



#define OV_DEBUG

OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_SEL_G_set(
    OV_INSTPTR_iec61131stdfb_SEL          pobj,
    const OV_BOOL  value
) {
    pobj->v_G = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_SEL_IN0_set(
    OV_INSTPTR_iec61131stdfb_SEL          pobj,
    const OV_ANY*  value
) {
    if((pobj->v_IN0.value.vartype & OV_VT_KSMASK) == (value->value.vartype & OV_VT_KSMASK))
		return ov_variable_setanyvalue(&pobj->v_IN0, value);
	else
	{
		if(iec61131stdfb_isConnected(Ov_PtrUpCast(fb_functionblock, pobj)))
			return OV_ERR_NOACCESS;
		else if((value->value.vartype & OV_VT_KSMASK) == OV_VT_VOID)
			return OV_ERR_BADPARAM;
		else
		{
			iec61131stdfb_freeVec(&pobj->v_IN0);
			iec61131stdfb_freeVec(&pobj->v_IN1);
			iec61131stdfb_freeVec(&pobj->v_OUT);
			pobj->v_IN0.value.vartype = value->value.vartype;
			pobj->v_IN1.value.vartype = value->value.vartype;
			pobj->v_OUT.value.vartype = value->value.vartype;
			return ov_variable_setanyvalue(&pobj->v_IN0, value);
		}
	}
}

OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_SEL_IN1_set(
    OV_INSTPTR_iec61131stdfb_SEL          pobj,
    const OV_ANY*  value
) {
    if((pobj->v_IN1.value.vartype & OV_VT_KSMASK) == (value->value.vartype & OV_VT_KSMASK))
		return ov_variable_setanyvalue(&pobj->v_IN1, value);
	else
	{
		if(iec61131stdfb_isConnected(Ov_PtrUpCast(fb_functionblock, pobj)))
			return OV_ERR_NOACCESS;
		else if((value->value.vartype & OV_VT_KSMASK) == OV_VT_VOID)
			return OV_ERR_BADPARAM;
		else
		{
			iec61131stdfb_freeVec(&pobj->v_IN0);
			iec61131stdfb_freeVec(&pobj->v_IN1);
			iec61131stdfb_freeVec(&pobj->v_OUT);
			pobj->v_IN0.value.vartype = value->value.vartype;
			pobj->v_IN1.value.vartype = value->value.vartype;
			pobj->v_OUT.value.vartype = value->value.vartype;
			return ov_variable_setanyvalue(&pobj->v_IN1, value);
		}
	}
}

OV_DLLFNCEXPORT OV_ANY* iec61131stdfb_SEL_OUT_get(
    OV_INSTPTR_iec61131stdfb_SEL          pobj
) {
    return &pobj->v_OUT;
}


OV_DLLFNCEXPORT void iec61131stdfb_SEL_destructor(OV_INSTPTR_ov_object pobj) {

	OV_INSTPTR_iec61131stdfb_SEL pinst = Ov_StaticPtrCast(iec61131stdfb_SEL, pobj);
	
	iec61131stdfb_freeVec(&pinst->v_IN0);
	iec61131stdfb_freeVec(&pinst->v_IN1);
	iec61131stdfb_freeVec(&pinst->v_OUT);
	fb_functionblock_destructor(pobj);
}

OV_DLLFNCEXPORT OV_RESULT iec61131stdfb_SEL_constructor(OV_INSTPTR_ov_object pobj) {

	OV_RESULT res;
	OV_INSTPTR_iec61131stdfb_SEL pinst = Ov_StaticPtrCast(iec61131stdfb_SEL, pobj);
	
	res = fb_functionblock_constructor(pobj);
	
	if(Ov_OK(res))
	{
		pinst->v_IN1.value.valueunion.val_double = 0;
		pinst->v_IN0.value.valueunion.val_double = 0;
		pinst->v_OUT.value.valueunion.val_double = 0;
		pinst->v_IN1.value.vartype = OV_VT_SINGLE;
		pinst->v_IN0.value.vartype = OV_VT_SINGLE;
		pinst->v_OUT.value.vartype = OV_VT_SINGLE;
		return OV_ERR_OK;
	}
	else
		return res;
}


OV_DLLFNCEXPORT void iec61131stdfb_SEL_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
	
	unsigned int i;
	
    OV_INSTPTR_iec61131stdfb_SEL pinst = Ov_StaticPtrCast(iec61131stdfb_SEL, pfb);
	
	iec61131stdfb_freeVec(&pinst->v_OUT);
	
		
	if(pinst->v_G)
	{
		switch(pinst->v_IN1.value.vartype & OV_VT_KSMASK)
		{
			STDFB_SEL(BYTE, byte, IN1);
			STDFB_SEL(BOOL, bool, IN1);
			STDFB_SEL(INT, int, IN1);
			STDFB_SEL(UINT, uint, IN1);
			STDFB_SEL(SINGLE, single, IN1);
			STDFB_SEL(DOUBLE, double, IN1);
			
			STDFB_SEL_STR(IN1);
			
			STDFB_SEL_TIME(TIME, time, IN1);
			
			STDFB_SEL_TIME(TIME_SPAN, time_span, IN1);
			
			STDFB_SEL_VEC(BYTE, byte, IN1);
			STDFB_SEL_VEC(BOOL, bool, IN1);
			STDFB_SEL_VEC(INT, int, IN1);
			STDFB_SEL_VEC(UINT, uint, IN1);
			STDFB_SEL_VEC(SINGLE, single, IN1);
			STDFB_SEL_VEC(DOUBLE, double, IN1);
			
			STDFB_SEL_STRING_VEC(IN1);
			
			STDFB_SEL_TIME_VEC(TIME,time, IN1);
			
			STDFB_SEL_TIME_VEC(TIME_SPAN, time_span, IN1);
			
			default:
				ov_logfile_error("%s: datatype of IN1 is currently not supported within this block", pinst->v_identifier);
				pinst->v_OUT.value.vartype = OV_VT_BOOL;
				pinst->v_OUT.value.valueunion.val_bool = 0;
			break;
			
		}
		
		/****************** handling states nad timestamp ******************/
	STDFB_STATE(IN1);
	STDFB_TIMESTAMP(IN1);
		
	}
	else
	{
		switch(pinst->v_IN0.value.vartype & OV_VT_KSMASK)
		{
			STDFB_SEL(BYTE, byte, IN0);
			STDFB_SEL(BOOL, bool, IN0);
			STDFB_SEL(INT, int, IN0);
			STDFB_SEL(UINT, uint, IN0);
			STDFB_SEL(SINGLE, single, IN0);
			STDFB_SEL(DOUBLE, double, IN0);
			
			STDFB_SEL_STR(IN0);
			
			STDFB_SEL_TIME(TIME, time, IN0);
			
			STDFB_SEL_TIME(TIME_SPAN, time_span, IN0);
			
			STDFB_SEL_VEC(BYTE, byte, IN0);
			STDFB_SEL_VEC(BOOL, bool, IN0);
			STDFB_SEL_VEC(INT, int, IN0);
			STDFB_SEL_VEC(UINT, uint, IN0);
			STDFB_SEL_VEC(SINGLE, single, IN0);
			STDFB_SEL_VEC(DOUBLE, double, IN0);
			
			STDFB_SEL_STRING_VEC(IN0);
			
			STDFB_SEL_TIME_VEC(TIME,time, IN0);
			
			STDFB_SEL_TIME_VEC(TIME_SPAN, time_span, IN0);
			
			default:
				ov_logfile_error("%s: datatype of IN0 is currently not supported within this block", pinst->v_identifier);
				pinst->v_OUT.value.vartype = OV_VT_BOOL;
				pinst->v_OUT.value.valueunion.val_bool = 0;
			break;
		}
		
		/****************** handling states nad timestamp ******************/
	STDFB_STATE(IN0);
	STDFB_TIMESTAMP(IN0);
		
	}
	
    return;
}
