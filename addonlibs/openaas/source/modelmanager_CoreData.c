
/******************************************************************************
*
*   FILE
*   ----
*   modelmanager.c
*
*   History
*   -------
*   2016-12-22   File created
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

extern OV_INSTPTR_openaas_nodeStoreFunctions pNodeStoreFunctions;

OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_getCoreData(IdentificationType aasId, OV_UINT *number, PropertyValueStatementList **pvsl) {
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_openaas_PropertyValueStatementList ppvsl = NULL;
	OV_INSTPTR_openaas_PropertyValueStatement ppvs = NULL;
	OV_UINT pvslSize = 0;
	OV_UINT pvsSize = 0;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		Ov_ForEachChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), ppvsl, openaas_PropertyValueStatementList){
			pvslSize++;
		}
		*pvsl = ov_database_malloc(sizeof(PropertyValueStatementList)*pvslSize);
		*number = pvslSize;
		pvslSize = 0;
		Ov_ForEachChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_Body), ppvsl, openaas_PropertyValueStatementList){
			ov_string_setvalue(&(*pvsl)[pvslSize].Carrier.IdSpec, ppvsl->v_CarrierIdString);
			(*pvsl)[pvslSize].Carrier.IdType = ppvsl->v_CarrierIdType;
			ov_string_setvalue(&(*pvsl)[pvslSize].CreatingInstance.IdSpec, ppvsl->v_CreatingInstanceIdString);
			(*pvsl)[pvslSize].CreatingInstance.IdType = ppvsl->v_CreatingInstanceIdType;
			(*pvsl)[pvslSize].CreationTime = ppvsl->v_CreationTime;
			ov_string_setvalue(&(*pvsl)[pvslSize].pvslName, ppvsl->v_identifier);

			Ov_ForEachChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, ppvsl), ppvs, openaas_PropertyValueStatement){
				if (ppvs->v_Visibility == 2)
					pvsSize++;
			}
			(*pvsl)[pvslSize].pvs = ov_database_malloc(sizeof(PropertyValueStatement)*pvsSize);
			(*pvsl)[pvslSize].pvsNumber = pvsSize;
			pvsSize = 0;
			Ov_ForEachChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, ppvsl), ppvs, openaas_PropertyValueStatement){
				if (ppvs->v_Visibility == 2){
					(*pvsl)[pvslSize].pvs[pvsSize].ExpressionLogic = ppvs->v_ExpressionLogic;
					(*pvsl)[pvslSize].pvs[pvsSize].ExpressionSemantic = ppvs->v_ExpressionSemantic;
					ov_string_setvalue(&(*pvsl)[pvslSize].pvs[pvsSize].ID.IdSpec, ppvs->v_IDIdString);
					(*pvsl)[pvslSize].pvs[pvsSize].ID.IdType = ppvs->v_IDIdType;
					(*pvsl)[pvslSize].pvs[pvsSize].Visibility = ppvs->v_Visibility;
					ov_string_setvalue(&(*pvsl)[pvslSize].pvs[pvsSize].pvsName, ppvs->v_identifier);
					ov_string_setvalue(&(*pvsl)[pvslSize].pvs[pvsSize].unit, ppvs->v_Unit);
					(*pvsl)[pvslSize].pvs[pvsSize].value.Value = ppvs->v_Value;
					(*pvsl)[pvslSize].pvs[pvsSize].value.TimeStamp = ppvs->v_TimeStamp;
					(*pvsl)[pvslSize].pvs[pvsSize].view = ppvs->v_View;
					pvsSize++;
				}
			}
			pvslSize++;
		}
	}

    return (AASStatusCode)0;
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_triggerGetCoreData(IdentificationType sourceAASId, IdentificationType targetAASId) {
	OV_RESULT resultOV = 0;
	AASStatusCode result = 0;
	SRV_String *srvStringSend = SRV_String_new();
	SRV_msgHeader *headerSend = SRV_msgHeader_t_new();
	void *srvStructSend = NULL;
	SRV_service_t srvTypeSend;

	headerSend->sender.idSpec.data = sourceAASId.IdSpec;
	headerSend->sender.idSpec.length = ov_string_getlength(sourceAASId.IdSpec);
	headerSend->sender.idType = sourceAASId.IdType;
	headerSend->receiver.idSpec.data = targetAASId.IdSpec;
	headerSend->receiver.idSpec.length = ov_string_getlength(targetAASId.IdSpec);
	headerSend->receiver.idType = targetAASId.IdType;

	getCoreDataReq_t getCoreDataReq;
	getCoreDataReq_t_init(&getCoreDataReq);
	srvStructSend = &getCoreDataReq;
	srvTypeSend = SRV_getCoreDataReq;

	resultOV = encodeMSG(&srvStringSend, headerSend, srvStructSend, srvTypeSend, SRV_JSON);
	if (resultOV)
		return AASSTATUSCODE_BADUNEXPECTEDERROR;

	getCoreDataReq_t_deleteMembers(&getCoreDataReq);

	// Get the pointer to object for send the Message
	OV_INSTPTR_ksapi_setVar psendAASMessage = NULL;
	Ov_ForEachChildEx(ov_instantiation, pclass_ksapi_setVar, psendAASMessage, ksapi_setVar){
		if(ov_string_compare(psendAASMessage->v_identifier, "SendAASMessage") == OV_STRCMP_EQUAL){
			break;
		}
	}

	ov_string_setvalue(&psendAASMessage->v_serverHost, "localhost");
	ov_string_setvalue(&psendAASMessage->v_serverName, "MANAGER");
	ov_string_setvalue(&psendAASMessage->v_path, "/TechUnits/openAAS/AASFolder/ComCo.postoffice");

	// send message
	ov_string_setvalue(&psendAASMessage->v_varValue.value.valueunion.val_string, srvStringSend->data);
	psendAASMessage->v_varValue.value.vartype = OV_VT_STRING;

	psendAASMessage->v_Submit = TRUE;

	SRV_serviceGeneric_delete(srvStructSend, srvTypeSend);
	SRV_msgHeader_t_delete(headerSend);
	SRV_String_delete(srvStringSend);

	if (psendAASMessage->v_status != 2)
		result = AASSTATUSCODE_BADUNEXPECTEDERROR;
    return result;
}
