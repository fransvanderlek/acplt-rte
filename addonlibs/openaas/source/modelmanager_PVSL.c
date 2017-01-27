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


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_createPVSL(IdentificationType aasId, OV_STRING pvslName, IdentificationType carrier) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_openaas_PropertyValueStatementList ppvsl = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		ppvsl = Ov_StaticPtrCast(openaas_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), pvslName));
		if(!ppvsl){
			result = Ov_CreateObject(openaas_PropertyValueStatementList, ppvsl, Ov_StaticPtrCast(ov_domain, &paas->p_Body), pvslName);
			if(Ov_Fail(result)){
				ov_logfile_error("Fatal: could not create PVSL object - reason: %s", ov_result_getresulttext(result));
				return openaas_modelmanager_ovresultToAASStatusCode(result);
			}
			ov_string_setvalue(&(ppvsl->v_CarrierString), carrier.IdSpec);
			ppvsl->v_CarrierType = carrier.IdType;
		}else{
			return AASSTATUSCODE_BADPVSLNAME;
		}
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_deletePVSL(IdentificationType aasId, OV_STRING pvslName){
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_openaas_PropertyValueStatementList ppvsl = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		ppvsl = Ov_StaticPtrCast(openaas_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), pvslName));
		if(ppvsl){
			result = Ov_DeleteObject(ppvsl);
			if(Ov_Fail(result)){
				ov_logfile_error("Fatal: could not delete PVSL object - reason: %s", ov_result_getresulttext(result));
				return openaas_modelmanager_ovresultToAASStatusCode(result);
			}
		}else{
			return AASSTATUSCODE_BADPVSLNAME;
		}
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}




