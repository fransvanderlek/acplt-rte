
/******************************************************************************
*
*   FILE
*   ----
*   SwitchForLCEGenerator.c
*
*   History
*   -------
*   2017-03-29   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_openaas
#define OV_COMPILE_LIBRARY_openaas
#endif


#include "openaas.h"
#include "libov/ov_macros.h"
#include "openaas_helpers.h"


OV_DLLFNCEXPORT OV_RESULT openaas_SwitchForLCEGenerator_AASIdString1_set(
    OV_INSTPTR_openaas_SwitchForLCEGenerator          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_AASIdString1,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_SwitchForLCEGenerator_AASIdType1_set(
    OV_INSTPTR_openaas_SwitchForLCEGenerator          pobj,
    const OV_UINT  value
) {
    pobj->v_AASIdType1 = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_SwitchForLCEGenerator_PVSLName1_set(
    OV_INSTPTR_openaas_SwitchForLCEGenerator          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSLName1,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_SwitchForLCEGenerator_PVSName1_set(
    OV_INSTPTR_openaas_SwitchForLCEGenerator          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSName1,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_SwitchForLCEGenerator_AASIdString2_set(
    OV_INSTPTR_openaas_SwitchForLCEGenerator          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_AASIdString2,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_SwitchForLCEGenerator_AASIdType2_set(
    OV_INSTPTR_openaas_SwitchForLCEGenerator          pobj,
    const OV_UINT  value
) {
    pobj->v_AASIdType2 = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_SwitchForLCEGenerator_PVSLName2_set(
    OV_INSTPTR_openaas_SwitchForLCEGenerator          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSLName2,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_SwitchForLCEGenerator_PVSName2_set(
    OV_INSTPTR_openaas_SwitchForLCEGenerator          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSName2,value);
}

OV_DLLFNCEXPORT void openaas_SwitchForLCEGenerator_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_openaas_SwitchForLCEGenerator pinst = Ov_StaticPtrCast(openaas_SwitchForLCEGenerator, pfb);
    pinst->v_Error = 0;
	ov_string_setvalue(&pinst->v_AASIDStringOut, "");
	pinst->v_AASIDTypeOut = 0;
	IdentificationType aasId1;
	IdentificationType_init(&aasId1);
	ov_string_setvalue(&aasId1.IdSpec, pinst->v_AASIdString1);
	aasId1.IdType = pinst->v_AASIdType1;

	PropertyValueStatement pvs1;
	PropertyValueStatement_init(&pvs1);

	IdentificationType aasId2;
	IdentificationType_init(&aasId2);
	ov_string_setvalue(&aasId2.IdSpec, pinst->v_AASIdString2);
	aasId2.IdType = pinst->v_AASIdType2;

	PropertyValueStatement pvs2;
	PropertyValueStatement_init(&pvs2);

	OV_BOOL aasID1Plugged = FALSE;
	OV_BOOL aasID2Plugged = FALSE;
	pinst->v_Error |= openaas_modelmanager_getPVS(aasId1, pinst->v_PVSLName1, pinst->v_PVSName1, &pvs1);
	if ((pvs1.value.value.vartype & OV_VT_KSMASK) == OV_VT_BOOL){
		if (pvs1.value.value.valueunion.val_bool == TRUE){
			aasID1Plugged = TRUE;
		}
	}
	pinst->v_Error |= openaas_modelmanager_getPVS(aasId2, pinst->v_PVSLName2, pinst->v_PVSName2, &pvs2);
	if ((pvs2.value.value.vartype & OV_VT_KSMASK) == OV_VT_BOOL){
		if (pvs2.value.value.valueunion.val_bool == TRUE){
			aasID2Plugged = TRUE;
		}
	}

	if (aasID1Plugged && aasID2Plugged)
		pinst->v_Error |= 99;
	else{
		if (aasID1Plugged == TRUE){
			ov_string_setvalue(&pinst->v_AASIDStringOut, aasId1.IdSpec);
			pinst->v_AASIDTypeOut = aasId1.IdType;
		}else if (aasID2Plugged == TRUE){
			ov_string_setvalue(&pinst->v_AASIDStringOut, aasId2.IdSpec);
			pinst->v_AASIDTypeOut = aasId2.IdType;
		}
	}

	PropertyValueStatement_deleteMembers(&pvs1);
	PropertyValueStatement_deleteMembers(&pvs2);
	IdentificationType_deleteMembers(&aasId1);
	IdentificationType_deleteMembers(&aasId2);
    return;
}

