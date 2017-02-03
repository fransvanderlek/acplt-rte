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

OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_createPVS(IdentificationType aasId, OV_STRING pvslName, OV_STRING pvsName, RelationalExpressionEnum relationalExpression, ExpressionSemanticEnum ExpressionSemantic, OV_ANY value, OV_STRING unit, IdentificationType propertyReference, ViewEnum view, OV_BOOL IsPublic) {
	OV_RESULT result = OV_ERR_OK;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_openaas_PropertyValueStatementList ppvsl = NULL;
	OV_INSTPTR_openaas_PropertyValueStatement ppvs = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		ppvsl = Ov_StaticPtrCast(openaas_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain,&paas->p_Body), pvslName));
		if(ppvsl){
			result = Ov_CreateObject(openaas_PropertyValueStatement, ppvs, ppvsl, pvsName);
			if(Ov_Fail(result)){
				ov_logfile_error("Fatal: could not create PVS object - reason: %s", ov_result_getresulttext(result));
				return openaas_modelmanager_ovresultToAASStatusCode(result);
			}
			ppvs->v_RelationalExpression = relationalExpression;
			ppvs->v_ExpressionSemantic = ExpressionSemantic;
			ov_variable_setanyvalue(&(ppvs->v_Value), &value);
			ov_string_setvalue(&(ppvs->v_Unit), unit);
			ov_string_setvalue(&(ppvs->v_PropertyReferenceString), propertyReference.IdSpec);
			ppvs->v_PropertyReferenceType = propertyReference.IdType;
			ppvs->v_View = view;
			ppvs->v_IsPublic = IsPublic;
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
		OV_INSTPTR_openaas_PropertyValueStatementList ppvsl = NULL;
		OV_INSTPTR_openaas_PropertyValueStatement ppvs = NULL;
		ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
		if (!ptr)
			return AASSTATUSCODE_BADAASID;
		paas = Ov_StaticPtrCast(openaas_aas, ptr);
		if (paas){
			ppvsl = Ov_StaticPtrCast(openaas_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), pvslName));
			if(!ppvsl)
				ppvsl = Ov_StaticPtrCast(openaas_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Header), pvslName));
			if(ppvsl){
				ppvs = Ov_StaticPtrCast(openaas_PropertyValueStatement, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain,ppvsl), pvsName));
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


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_getPVS(IdentificationType aasId, OV_STRING pvslName, OV_STRING pvsName, RelationalExpressionEnum *relationalExpression, ExpressionSemanticEnum *ExpressionSemantic, OV_ANY *value, OV_STRING *unit, IdentificationType *propertyReference, ViewEnum *view, OV_BOOL *IsPublic) {
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_openaas_PropertyValueStatementList ppvsl = NULL;
	OV_INSTPTR_openaas_PropertyValueStatement ppvs = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		ppvsl = Ov_StaticPtrCast(openaas_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), pvslName));
		if(!ppvsl)
			ppvsl = Ov_StaticPtrCast(openaas_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Header), pvslName));
		if(ppvsl){
			ppvs = Ov_StaticPtrCast(openaas_PropertyValueStatement, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain,ppvsl), pvsName));
			if(ppvs){
				*relationalExpression = ppvs->v_RelationalExpression;
				*ExpressionSemantic = ppvs->v_ExpressionSemantic;
				ov_variable_setanyvalue(value, &(ppvs->v_Value));
				ov_string_setvalue(unit, ppvs->v_Unit);
				ov_string_setvalue(&propertyReference->IdSpec, ppvs->v_PropertyReferenceString);
				propertyReference->IdType = ppvs->v_PropertyReferenceType;
				*view = ppvs->v_View;
				*IsPublic = ppvs->v_IsPublic;
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

OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_setPVS(IdentificationType aasId, OV_STRING pvslName, OV_STRING pvsName, RelationalExpressionEnum relationalExpression, ExpressionSemanticEnum ExpressionSemantic, OV_ANY value, OV_STRING unit, IdentificationType propertyReference, ViewEnum view, OV_BOOL IsPublic) {
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_openaas_PropertyValueStatementList ppvsl = NULL;
	OV_INSTPTR_openaas_PropertyValueStatement ppvs = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		ppvsl = Ov_StaticPtrCast(openaas_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), pvslName));
		if(!ppvsl)
			ppvsl = Ov_StaticPtrCast(openaas_PropertyValueStatementList, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Header), pvslName));
		if(ppvsl){
			ppvs = Ov_StaticPtrCast(openaas_PropertyValueStatement, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain,ppvsl), pvsName));
			if (ppvs){
				ppvs->v_RelationalExpression = relationalExpression;
				ppvs->v_ExpressionSemantic = ExpressionSemantic;
				ov_variable_setanyvalue(&(ppvs->v_Value), &value);
				ov_string_setvalue(&(ppvs->v_Unit), unit);
				ov_string_setvalue(&(ppvs->v_PropertyReferenceString), propertyReference.IdSpec);
				ppvs->v_PropertyReferenceType = propertyReference.IdType;
				ppvs->v_View = view;
				ppvs->v_IsPublic = IsPublic;
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