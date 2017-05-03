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



OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEAASIdString_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_LCEAASIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEAASIdType_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_LCEAASIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEId_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_LCEId = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCECreatingInstanceIdString_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_LCECreatingInstanceIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCECreatingInstanceIdType_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_LCECreatingInstanceIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEWritingInstanceIdString_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_LCEWritingInstanceIdString,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEWritingInstanceIdType_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_UINT  value
) {
    pobj->v_LCEWritingInstanceIdType = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEEventClass_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_LCEEventClass,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCESubject_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_STRING  value
) {
    return ov_string_setvalue(&pobj->v_LCESubject,value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEValue_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_ANY*  value
) {
    return ov_variable_setanyvalue(&pobj->v_LCEValue, value);
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCETimeStamp_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_TIME  *value
) {
    pobj->v_LCETimeStamp = *value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCECreate_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_BOOL  value
) {
	AASStatusCode result = AASSTATUSCODE_GOOD;
	pobj->v_LCECreate = value;
	if (pobj->v_LCECreate == true){
		IdentificationType tmpAASId;
		IdentificationType_init(&tmpAASId);
		tmpAASId.IdSpec = pobj->v_LCEAASIdString;
		tmpAASId.IdType = pobj->v_LCEAASIdType;

		LifeCycleEntry lce;
		LifeCycleEntry_init(&lce);
		lce.creatingInstance.IdSpec = pobj->v_LCECreatingInstanceIdString;
		lce.creatingInstance.IdType = pobj->v_LCECreatingInstanceIdType;
		lce.writingInstance.IdSpec = pobj->v_LCEWritingInstanceIdString;
		lce.writingInstance.IdType = pobj->v_LCEWritingInstanceIdType;
		lce.data = pobj->v_LCEValue;
		lce.data.time = pobj->v_LCETimeStamp;
		lce.eventClass = pobj->v_LCEEventClass;
		lce.lceId = pobj->v_LCEId;
		lce.subject = pobj->v_LCESubject;

		result = openaas_modelmanager_createLCE(tmpAASId, lce);
	}
	pobj->v_LCECreate = false;
	pobj->v_LCEStatus = result;
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT openaas_modelmanager_LCEDelete_set(
    OV_INSTPTR_openaas_modelmanager          pobj,
    const OV_BOOL  value
) {
	AASStatusCode result = AASSTATUSCODE_GOOD;
	pobj->v_LCEDelete = value;
	if (pobj->v_LCEDelete == true){
		IdentificationType tmpAASId;
		tmpAASId.IdSpec = pobj->v_LCEAASIdString;
		tmpAASId.IdType = pobj->v_LCEAASIdType;

		result = openaas_modelmanager_deleteLCE(tmpAASId, pobj->v_LCEId);
	}
	pobj->v_LCEDelete = false;
	pobj->v_LCEStatus = result;
	return OV_ERR_OK;
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_createLCE(IdentificationType aasId, LifeCycleEntry lce) {
	AASStatusCode result = AASSTATUSCODE_GOOD;
	OV_RESULT ovResult = OV_ERR_OK;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_lifeCycleEntry_LifeCycleEntry plce = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		if (paas->p_LifeCycleArchive.v_LifeCycleEntrySize > paas->p_LifeCycleArchive.v_LifeCycleEntrySizeMax-1){
			ptr = Ov_GetFirstChild(ov_containment, &paas->p_LifeCycleArchive);
			if (ptr){
				plce = Ov_StaticPtrCast(lifeCycleEntry_LifeCycleEntry,ptr);
				if(!plce){
					do{
						ptr = Ov_GetNextChild(ov_containment, &paas->p_LifeCycleArchive);
						if (ptr){
							plce = Ov_StaticPtrCast(lifeCycleEntry_LifeCycleEntry,ptr);
						}else{
							break;
						}
					}while(!plce);
				}
				if (plce){
					ovResult = Ov_DeleteObject(plce);
					if(Ov_Fail(ovResult)){
						ov_logfile_error("Fatal: could not delete LCE object - reason: %s", ov_result_getresulttext(ovResult));
						return openaas_modelmanager_ovresultToAASStatusCode(result);
					}
					paas->p_LifeCycleArchive.v_LifeCycleEntrySize -= 1;
				}
			}
		}
		OV_STRING tmpString = NULL;
		OV_UINT count = paas->p_LifeCycleArchive.v_LifeCycleEntryNameCount+1;
		ov_string_print(&tmpString, "%u", count);
		plce = NULL;
		ovResult = Ov_CreateObject(lifeCycleEntry_LifeCycleEntry, plce, &paas->p_LifeCycleArchive, tmpString);
		ov_database_free(tmpString);
		if(Ov_Fail(ovResult)){
			ov_logfile_error("Fatal: could not create LCE object - reason: %s", ov_result_getresulttext(ovResult));
			return openaas_modelmanager_ovresultToAASStatusCode(ovResult);
		}
		paas->p_LifeCycleArchive.v_LifeCycleEntrySize += 1;
		paas->p_LifeCycleArchive.v_LifeCycleEntryNameCount += 1;
		ov_string_setvalue(&(plce->v_CreatingInstanceIdString), lce.creatingInstance.IdSpec);
		plce->v_CreatingInstanceIdType = lce.creatingInstance.IdType;
		ov_string_setvalue(&(plce->v_WritingInstanceIdString), lce.writingInstance.IdSpec);
		plce->v_WritingInstanceIdType = lce.writingInstance.IdType;
		ov_string_setvalue(&(plce->v_EventClass), lce.eventClass);
		ov_string_setvalue(&(plce->v_Subject), lce.subject);
		ov_variable_setanyvalue(&(plce->v_Data), &lce.data);
		plce->v_TimeStamp = lce.data.time;
	}else{
		result = AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
    return result;
}

OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_deleteLCE(IdentificationType aasId, OV_UINT64 lceId) {
	AASStatusCode result = AASSTATUSCODE_GOOD;
	OV_RESULT ovResult = OV_ERR_OK;
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_lifeCycleEntry_LifeCycleEntry lce = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		OV_STRING tmpString = NULL;
		ov_string_print(&tmpString, "%lu", lceId);
		lce = Ov_StaticPtrCast(lifeCycleEntry_LifeCycleEntry, Ov_SearchChild(ov_containment, &paas->p_LifeCycleArchive, tmpString));
		ov_database_free(tmpString);
		if(lce){
			ovResult = Ov_DeleteObject(lce);
			if(Ov_Fail(ovResult)){
				ov_logfile_error("Fatal: could not delete LCE object - reason: %s", ov_result_getresulttext(ovResult));
				return openaas_modelmanager_ovresultToAASStatusCode(result);
			}
			paas->p_LifeCycleArchive.v_LifeCycleEntrySize -=1;
		}else{
			result = AASSTATUSCODE_BADLCEID;
		}
	}else{
		result = AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
    return result;
}



OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_getLCE(IdentificationType aasId, OV_UINT64 lceId, LifeCycleEntry *lce) {
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_lifeCycleEntry_LifeCycleEntry plce = NULL;
	OV_STRING lceName = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		lceName = NULL;
		ov_string_print(&lceName, "%lu", lceId);
		plce = Ov_StaticPtrCast(lifeCycleEntry_LifeCycleEntry, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_LifeCycleArchive), lceName));
		if (plce){
			ov_string_setvalue(&lce->creatingInstance.IdSpec, plce->v_CreatingInstanceIdString);
			lce->creatingInstance.IdType = plce->v_CreatingInstanceIdType;
			ov_string_setvalue(&lce->writingInstance.IdSpec, plce->v_WritingInstanceIdString);
			lce->writingInstance.IdType = plce->v_WritingInstanceIdType;
			ov_string_setvalue(&lce->eventClass, plce->v_EventClass);
			ov_string_setvalue(&lce->subject, plce->v_Subject);
			ov_variable_setanyvalue(&lce->data, &(plce->v_Data));
			lce->data.time = plce->v_TimeStamp;
			lce->lceId = atoi(plce->v_identifier);
		}else{
			return AASSTATUSCODE_BADLCEID;
		}
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_getLastLCEs(IdentificationType aasId, OV_UINT count, LifeCycleEntry **lce, OV_UINT *arrayDimension) {
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_lifeCycleEntry_LifeCycleEntry plce = NULL;
	OV_UINT lceSize = 0;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		Ov_ForEachChildEx(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_LifeCycleArchive), plce, lifeCycleEntry_LifeCycleEntry){
			lceSize++;
		}
		if (lceSize > count){
			*lce = ov_database_malloc(sizeof(LifeCycleEntry)*count);
			*arrayDimension = count;
		}
		else{
			*lce = ov_database_malloc(sizeof(LifeCycleEntry)*lceSize);
			*arrayDimension = lceSize;
		}

		OV_UINT i = 0;
		OV_UINT j = 0;
		do{
			j++;
			if (j == 1){
				ptr= Ov_GetLastChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_LifeCycleArchive));
			}else{
				ptr = Ov_GetPreviousChild(ov_containment, Ov_StaticPtrCast(ov_domain, ptr));
			}
			if (!Ov_CanCastTo(lifeCycleEntry_LifeCycleEntry, ptr))
				continue;

			plce = Ov_StaticPtrCast(lifeCycleEntry_LifeCycleEntry, ptr);

			LifeCycleEntry_init(&((*lce)[i]));
			ov_string_setvalue(&((*lce)[i].creatingInstance.IdSpec), plce->v_CreatingInstanceIdString);
			(*lce)[i].creatingInstance.IdType = plce->v_CreatingInstanceIdType;
			ov_string_setvalue(&((*lce)[i].writingInstance.IdSpec), plce->v_WritingInstanceIdString);
			(*lce)[i].writingInstance.IdType = plce->v_WritingInstanceIdType;
			ov_string_setvalue(&((*lce)[i].eventClass), plce->v_EventClass);
			ov_string_setvalue(&((*lce)[i].subject), plce->v_Subject);
			ov_variable_setanyvalue(&((*lce)[i].data), &(plce->v_Data));
			(*lce)[i].data.time = plce->v_TimeStamp;
			(*lce)[i].lceId = atoi(plce->v_identifier);
			i++;
		}while(i < *arrayDimension);
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}


OV_DLLFNCEXPORT AASStatusCode openaas_modelmanager_setLCE(IdentificationType aasId, LifeCycleEntry lce) {
	OV_INSTPTR_openaas_aas paas = NULL;
	OV_INSTPTR_ov_object ptr = NULL;
	OV_INSTPTR_lifeCycleEntry_LifeCycleEntry plce = NULL;
	OV_STRING lceName = NULL;
	ptr = ov_path_getobjectpointer(openaas_modelmanager_AASConvertListGet(aasId), 2);
	if (!ptr)
		return AASSTATUSCODE_BADAASID;
	paas = Ov_StaticPtrCast(openaas_aas, ptr);
	if (paas){
		lceName = NULL;
		ov_string_print(&lceName, "%lu", lce.lceId);
		plce = Ov_StaticPtrCast(lifeCycleEntry_LifeCycleEntry, Ov_SearchChild(ov_containment, Ov_StaticPtrCast(ov_domain, &paas->p_LifeCycleArchive), lceName));
		if (plce){
			ov_string_setvalue(&(plce->v_CreatingInstanceIdString), lce.creatingInstance.IdSpec);
			plce->v_CreatingInstanceIdType = lce.creatingInstance.IdType;
			ov_string_setvalue(&(plce->v_WritingInstanceIdString), lce.writingInstance.IdSpec);
			plce->v_WritingInstanceIdType = lce.writingInstance.IdType;
			ov_string_setvalue(&(plce->v_EventClass), lce.eventClass);
			ov_string_setvalue(&(plce->v_Subject), lce.subject);
			ov_variable_setanyvalue(&(plce->v_Data), &lce.data);
			plce->v_TimeStamp = lce.data.time;
		}else{
			return AASSTATUSCODE_BADLCEID;
		}
	}else{
		return AASSTATUSCODE_BADUNEXPECTEDERROR;
	}
	return AASSTATUSCODE_GOOD;
}
