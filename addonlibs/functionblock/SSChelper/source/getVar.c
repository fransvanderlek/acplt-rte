
/******************************************************************************
*
*   FILE
*   ----
*   getVar.c
*
*   History
*   -------
*   2017-08-30   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_SSChelper
#define OV_COMPILE_LIBRARY_SSChelper
#endif


#include "SSChelper.h"
#include "utilities.h"

#include "libov/ov_macros.h"

OV_RESULT SSChelper_getNamedVariable(const OV_INSTPTR_ov_object pTargetObj, const OV_STRING targetVarname, OV_ANY *value){
	OV_RESULT result = OV_ERR_OK;
	OV_ELEMENT element;
	OV_ELEMENT varElement;
	OV_VTBLPTR_ov_object pVtblObj = NULL;

	if(pTargetObj == NULL){
			return OV_ERR_BADPARAM;
	}else if (Ov_CanCastTo(fb_functionchart, pTargetObj)&& !Ov_CanCastTo(ssc_SequentialStateChart, pTargetObj)&&ov_string_compare(targetVarname,"CMD")!=OV_STRCMP_EQUAL){
		//get variable in a functionchart
		result = fb_functionchart_getport(Ov_StaticPtrCast(fb_functionchart, pTargetObj), targetVarname, value);
	}else{
		//get variable in a object
		varElement.elemtype = OV_ET_NONE;
		element.elemtype = OV_ET_OBJECT;
		element.pobj = pTargetObj;

		//search the variable for the set operation
		ov_element_searchpart(&element, &varElement, OV_ET_VARIABLE, targetVarname);
		if(varElement.elemtype == OV_ET_VARIABLE) {
			//port found, use the getter to write the value
			Ov_GetVTablePtr(ov_object, pVtblObj, pTargetObj);
			result = pVtblObj->m_getvar(varElement.pobj, &varElement, value);
		}
	}

	return result;
}

OV_DLLFNCEXPORT void SSChelper_getVar_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_SSChelper_getVar pinst = Ov_StaticPtrCast(SSChelper_getVar, pfb);
    OV_INSTPTR_ov_object target=NULL;

    SSChelper_getObjectAndVarnameFromSetVariable(Ov_PtrUpCast(ov_object,pinst),pinst->v_path,&target,&pinst->v_port);

    SSChelper_getNamedVariable(target,pinst->v_port,&pinst->v_var);
    return;

    return;
}
