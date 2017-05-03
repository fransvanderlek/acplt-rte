/******************************************************************************
 *
 *   FILE
 *   ----
 *   nodeStoreFunctions.c
 *
 *   History
 *   -------
 *   2014-10-21   File created
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
#include "libov/ov_result.h"
#include "openaas_helpers.h"



OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSAASIdString_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSAASIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSAASIdType_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSAASIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSPVSLName_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSPVSLName,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSName_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSName,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSExpressionLogic_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_INT  value
) {
    pobj->v_PVSExpressionLogic = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSExpressionSemantic_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_INT  value
) {
    pobj->v_PVSExpressionSemantic = value;
    return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSTimeStamp_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_TIME*  value
) {
    pobj->v_PVSTimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSValue_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_ANY*  value
) {
    return ov_variable_setanyvalue(&pobj->v_PVSValue, value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSUnit_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSUnit,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSIDIdString_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_PVSIDIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSIDIdType_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_PVSIDIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSView_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_INT  value
) {
    pobj->v_PVSView = value;
    return OV_ERR_OK;
}


OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSVisibility_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_INT  value
) {
    pobj->v_PVSVisibility = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSCreate_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_BOOL  value
) {
	AASStatusCode result = AASSTATUSCODE_GOOD;
	pobj->v_PVSCreate = value;
	if (pobj->v_PVSCreate == true){
		IdentificationType tmpAASId;
		IdentificationType_init(&tmpAASId);
		tmpAASId.IdSpec = pobj->v_PVSAASIdString;
		tmpAASId.IdType = pobj->v_PVSAASIdType;

		PropertyValueStatement pvs;
		PropertyValueStatement_init(&pvs);
		pvs.ExpressionSemantic = pobj->v_PVSExpressionSemantic;
		pvs.Visibility = pobj->v_PVSVisibility;
		pvs.ID.IdSpec = pobj->v_PVSIDIdString;
		pvs.ID.IdType = pobj->v_PVSIDIdType;
		pvs.pvsName = pobj->v_PVSName;
		pvs.ExpressionLogic = pobj->v_PVSExpressionLogic;
		pvs.unit = pobj->v_PVSUnit;
		pvs.value = pobj->v_PVSValue;
		pvs.value.time = pobj->v_PVSTimeStamp;
		pvs.view = pobj->v_PVSView;

		result = openaas_modelmanager_createPVS(tmpAASId, pobj->v_PVSPVSLName, pvs);
	}
	pobj->v_PVSCreate = false;
	pobj->v_PVSStatus = result;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_PVSDelete_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_BOOL  value
) {
	AASStatusCode result = AASSTATUSCODE_GOOD;
	pobj->v_PVSDelete = value;
	if (pobj->v_PVSDelete == true){
		IdentificationType tmpAASId;
		tmpAASId.IdSpec = pobj->v_PVSAASIdString;
		tmpAASId.IdType = pobj->v_PVSAASIdType;

		result = openaas_modelmanager_deletePVS(tmpAASId, pobj->v_PVSPVSLName, pobj->v_PVSName);
	}
	pobj->v_PVSDelete = false;
	pobj->v_PVSStatus = result;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_createPVS(IdentificationType aasId, OV_STRING pvslName, PropertyValueStatement pvs) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatementList ppvsl = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatement ppvs = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain,&paas->p_Body), pvslName));
		if(ppvsl){
			ppvs = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatement, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, ppvsl), pvs.pvsName));
			if(!ppvs){
				result = Ov_CreateObject(propertyValueStatement_PropertyValueStatement, ppvs, ppvsl, pvs.pvsName);
				if(Ov_Fail(result)){
					ov_logfile_error("Fatal: could not create PVS object - reason: %s", ov_result_getresulttext(result));
					return openaas_modelmanager_ovresultToAASStatusCode(result);
				}
				ppvs->v_ExpressionLogic = pvs.ExpressionLogic;
				ppvs->v_ExpressionSemantic = pvs.ExpressionSemantic;
				ov_variable_setanyvalue(&(ppvs->v_Value), &pvs.value);
				ov_time_gettime(&ppvs->v_TimeStamp);
				ov_string_setvalue(&(ppvs->v_Unit), pvs.unit);
				ov_string_setvalue(&(ppvs->v_IDIdString), pvs.ID.IdSpec);
				ppvs->v_IDIdType = pvs.ID.IdType;
				ppvs->v_View = pvs.view;
				ppvs->v_Visibility = pvs.Visibility;
			}else{
				return AASSTATUSCODE_BADPVSNAME;
			}
		}else{
			return AASSTATUSCODE_BADPVSLNAME;
		}
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
    return AASSTATUSCODE_GOOD;
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_createPVSTime(IdentificationType aasId, OV_STRING pvslName, PropertyValueStatement pvs) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatementList ppvsl = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatement ppvs = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain,&paas->p_Body), pvslName));
		if(ppvsl){
			result = Ov_CreateObject(propertyValueStatement_PropertyValueStatement, ppvs, ppvsl, pvs.pvsName);
			if(Ov_Fail(result)){
				ov_logfile_error("Fatal: could not create PVS object - reason: %s", ov_result_getresulttext(result));
				return openaas_modelmanager_ovresultToAASStatusCode(result);
			}
			ppvs->v_ExpressionLogic = pvs.ExpressionLogic;
			ppvs->v_ExpressionSemantic = pvs.ExpressionSemantic;
			ov_variable_setanyvalue(&(ppvs->v_Value), &pvs.value);
			ppvs->v_TimeStamp = pvs.value.time;
			ov_string_setvalue(&(ppvs->v_Unit), pvs.unit);
			ov_string_setvalue(&(ppvs->v_IDIdString), pvs.ID.IdSpec);
			ppvs->v_IDIdType = pvs.ID.IdType;
			ppvs->v_View = pvs.view;
			ppvs->v_Visibility = pvs.Visibility;
		}else{
			return AASSTATUSCODE_BADPVSLNAME;
		}
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
    return AASSTATUSCODE_GOOD;
}

OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_deletePVS(IdentificationType aasId, OV_STRING pvslName, OV_STRING pvsName) {
		OV_RESULT result = OV_ERR_OK;
		OV_INSTPTR_openaas_aas paas = NULL;
		OV_INSTPTR_ov_object ptr = NULL;
		OV_INSTPTR_propertyValueStatement_PropertyValueStatementList ppvsl = NULL;
		OV_INSTPTR_propertyValueStatement_PropertyValueStatement ppvs = NULL;
		ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
		if (!ptr)
			return AASSTATUSCODE_BADAASID;
		paas = Ov_StaticPtrCast(openaas_aas, ptr);
		if (paas){
			ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), pvslName));
			if(!ppvsl)
				ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Header), pvslName));
			if(ppvsl){
				ppvs = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatement, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain,ppvsl), pvsName));
				if (ppvs){
					result = Ov_DeleteObject(ppvs);
					if(Ov_Fail(result)){
						ov_logfile_error("Fatal: could not delete PVS object - reason: %s", ov_result_getresulttext(result));
						return openaas_modelmanager_ovresultToAASStatusCode(result);
					}
				}else{
					return AASSTATUSCODE_BADPVSNAME;
				}
			}else{
				return AASSTATUSCODE_BADPVSLNAME;
			}
		}else{
			return AASSTATUSCODE_BADUNEXPECTEDERROR;
		}
    return AASSTATUSCODE_GOOD;
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_getPVS(IdentificationType aasId, OV_STRING pvslName, OV_STRING pvsName, PropertyValueStatement *pvs) {
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatementList ppvsl = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatement ppvs = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), pvslName));
		if(!ppvsl)
			ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Header), pvslName));
		if(ppvsl){
			ppvs = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatement, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain,ppvsl), pvsName));
			if(ppvs){
				ov_string_setvalue(&pvs->pvsName, ppvs->v_identifier);
				pvs->ExpressionLogic = ppvs->v_ExpressionLogic;
				pvs->ExpressionSemantic = ppvs->v_ExpressionSemantic;
				ov_variable_setanyvalue(&pvs->value, &(ppvs->v_Value));
				pvs->value.time = ppvs->v_TimeStamp;
				ov_string_setvalue(&pvs->unit, ppvs->v_Unit);
				ov_string_setvalue(&pvs->ID.IdSpec, ppvs->v_IDIdString);
				pvs->ID.IdType = ppvs->v_IDIdType;
				pvs->view = ppvs->v_View;
				pvs->Visibility = ppvs->v_Visibility;
			}else{
				return AASSTATUSCODE_BADPVSNAME;
			}
		}else{
			return AASSTATUSCODE_BADPVSLNAME;
		}
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}

OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_setPVS(IdentificationType aasId, OV_STRING pvslName, PropertyValueStatement pvs) {
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatementList ppvsl = NULL;
	OV_INSTPTR_propertyValueStatement_PropertyValueStatement ppvs = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), pvslName));
		if(!ppvsl)
			ppvsl = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Header), pvslName));
		if(ppvsl){
			ppvs = Ov_StaticPtrCast(propertyValueStatement_PropertyValueStatement, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain,ppvsl), pvs.pvsName));
			if (ppvs){
				ppvs->v_ExpressionLogic = pvs.ExpressionLogic;
				ppvs->v_ExpressionSemantic = pvs.ExpressionSemantic;
				ov_variable_setanyvalue(&(ppvs->v_Value), &pvs.value);
				ov_time_gettime(&pvs.value.time);
				ov_string_setvalue(&(ppvs->v_Unit), pvs.unit);
				ov_string_setvalue(&(ppvs->v_IDIdString), pvs.ID.IdSpec);
				ppvs->v_IDIdType = pvs.ID.IdType;
				ppvs->v_View = pvs.view;
				ppvs->v_Visibility = pvs.Visibility;
			}else{
				return AASSTATUSCODE_BADPVSNAME;
			}
		}else{
			return AASSTATUSCODE_BADPVSLNAME;
		}
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}
