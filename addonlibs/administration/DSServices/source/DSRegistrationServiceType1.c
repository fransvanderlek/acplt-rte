
/******************************************************************************
*
*   FILE
*   ----
*   DSRegistrationServiceType1.c
*
*   History
*   -------
*   2018-05-14   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_DSServices
#define OV_COMPILE_LIBRARY_DSServices
#endif


#include "DSServices.h"
#include "json_helper.h"
#include "service_helper.h"

struct endpoint{
	OV_STRING protocolType;
	OV_STRING endpointString;
};

struct tags{
	OV_STRING tag;
	OV_STRING value;
};

OV_DLLFNCEXPORT OV_RESULT DSServices_DSRegistrationServiceType1_executeService(OV_INSTPTR_openAASDiscoveryServer_DSService pinst, const json_data JsonInput, OV_STRING *JsonOutput, OV_STRING *errorMessage) {
    /*    
    *   local variables
    */
	// Parsing Body
	OV_UINT endpointArraySize = 0;
	struct endpoint *endpoints = NULL;
	OV_UINT tagsArraySize = 0;
	struct tags *tags = NULL;
	OV_STRING_VEC jsonTags;
	jsonTags.value = NULL;
	jsonTags.veclen = 0;
	Ov_SetDynamicVectorLength(&jsonTags, 4, STRING);
	ov_string_setvalue(&jsonTags.value[0], "componentID");
	ov_string_setvalue(&jsonTags.value[1], "securityKey");
	ov_string_setvalue(&jsonTags.value[2], "endpoints");
	ov_string_setvalue(&jsonTags.value[3], "tags");
	OV_UINT_VEC tokenIndex;
	tokenIndex.value = NULL;
	tokenIndex.veclen = 0;
	Ov_SetDynamicVectorLength(&tokenIndex, 4, UINT);

	jsonGetTokenIndexByTags(jsonTags, JsonInput, 1, &tokenIndex);

	OV_STRING componentID = NULL;
	jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[0]+1], &componentID);
	OV_STRING securityKey = NULL;
	jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[1]+1], &securityKey);

	// check securityKey
	OV_RESULT resultOV = checkSecurityKey(pinst->v_DBWrapperUsed, componentID, securityKey);
	if (resultOV){
		ov_string_setvalue(errorMessage, "SecurityKey is not correct");
		goto FINALIZE;
	}

	// get registrationData
	// get endpoints from JSON
	endpointArraySize = JsonInput.token[tokenIndex.value[2]+1].size;
	endpoints = malloc(sizeof(struct endpoint)*endpointArraySize);
	for (OV_UINT i = 0; i < endpointArraySize; i++){
		endpoints[i].protocolType = NULL;
		// value + 2 start of objects + i*5 next object + 2/4 values of protocolType and endpoint
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[2]+2+i*5+2], &endpoints[i].protocolType);
		endpoints[i].endpointString = NULL;
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[2]+2+i*5+4], &endpoints[i].endpointString);
	}
	// get tags from JSON
	tagsArraySize = JsonInput.token[tokenIndex.value[3]+1].size;
	tags = malloc(sizeof(struct endpoint)*tagsArraySize);
	for (OV_UINT i = 0; i < tagsArraySize; i++){
		tags[i].tag = NULL;
		// value + 2 start of objects + i*5 next object + 2/4 values of tag and value
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[3]+2+i*5+2], &tags[i].tag);
		tags[i].value = NULL;
		jsonGetValueByToken(JsonInput.js, &JsonInput.token[tokenIndex.value[3]+2+i*5+4], &tags[i].value);
	}

	// delete old registered Data
	// TODO: Extend to multiDBWrapper
	OV_INSTPTR_openAASDiscoveryServer_DBWrapper pDBWrapper = NULL;
	OV_VTBLPTR_openAASDiscoveryServer_DBWrapper pDBWrapperVTable = NULL;
	pDBWrapper = Ov_DynamicPtrCast(openAASDiscoveryServer_DBWrapper, ov_path_getobjectpointer(pinst->v_DBWrapperUsed.value[0], 2));
	if (!pDBWrapper){
		ov_string_setvalue(errorMessage, "Internel Error");
		ov_logfile_error("Could not find DBWrapper Object");
		goto FINALIZE;
	}
	Ov_GetVTablePtr(openAASDiscoveryServer_DBWrapper,pDBWrapperVTable, pDBWrapper);
	OV_STRING table  = "Endpoints";
	OV_STRING tmpField = "ComponentID";
	OV_STRING tmpValue = NULL;
	ov_string_print(&tmpValue, "'%s'", componentID);
	resultOV = pDBWrapperVTable->m_deleteData(table, &tmpField, 1, &tmpValue, 1);
	table  = "Tags";
	resultOV = pDBWrapperVTable->m_deleteData(table, &tmpField, 1, &tmpValue, 1);
	ov_string_setvalue(&tmpValue, NULL);

	// Insert endpoints in database
	// TODO: Extend to multiDBWrapper
	pDBWrapper = NULL;
	pDBWrapperVTable = NULL;
	pDBWrapper = Ov_DynamicPtrCast(openAASDiscoveryServer_DBWrapper, ov_path_getobjectpointer(pinst->v_DBWrapperUsed.value[0], 2));
	if (!pDBWrapper){
		ov_string_setvalue(errorMessage, "Internel Error");
		ov_logfile_error("Could not find DBWrapper Object");
		goto FINALIZE;
	}
	Ov_GetVTablePtr(openAASDiscoveryServer_DBWrapper,pDBWrapperVTable, pDBWrapper);
	table  = "Endpoints";
	OV_STRING tmpFields[3];
	OV_STRING tmpValues[3];
	tmpFields[0] = "ComponentID";
	tmpFields[1] = "ProtocolType";
	tmpFields[2] = "EndpointString";
	tmpValues[0] = NULL;
	ov_string_print(&tmpValues[0], "'%s'", componentID);
	for (OV_UINT i = 0; i < endpointArraySize; i++){
		tmpValues[1] = NULL;
		ov_string_print(&tmpValues[1], "'%s'", endpoints[i].protocolType);
		tmpValues[2] = NULL;
		ov_string_print(&tmpValues[2], "'%s'", endpoints[i].endpointString);
		resultOV = pDBWrapperVTable->m_insertData(table, tmpFields, 3, tmpValues, 3);
		ov_string_setvalue(&tmpValues[1], NULL);
		ov_string_setvalue(&tmpValues[2], NULL);
		if (resultOV != OV_ERR_OK){
			ov_string_setvalue(errorMessage, "Internal Error");
			ov_logfile_error("Could not insert endpoints in database");
			goto FINALIZE;
		}
	}

	// Insert tags in database
	table  = "Tags";
	tmpFields[0] = "ComponentID";
	tmpFields[1] = "Tag";
	tmpFields[2] = "Value";
	tmpValues[0] = NULL;
	ov_string_print(&tmpValues[0], "'%s'", componentID);
	for (OV_UINT i = 0; i < tagsArraySize; i++){
		tmpValues[1] = NULL;
		ov_string_print(&tmpValues[1], "'%s'", tags[i].tag);
		tmpValues[2] = NULL;
		ov_string_print(&tmpValues[2], "'%s'", tags[i].value);
		resultOV = pDBWrapperVTable->m_insertData(table, tmpFields, 3, tmpValues, 3);
		ov_string_setvalue(&tmpValues[1], NULL);
		ov_string_setvalue(&tmpValues[2], NULL);
		if (resultOV != OV_ERR_OK){
			ov_string_setvalue(errorMessage, "Internal Error");
			ov_logfile_error("Could not insert tags in database");
			goto FINALIZE;
		}
	}

	FINALIZE:
	ov_string_print(JsonOutput, "\"body\":{}");
	Ov_SetDynamicVectorLength(&jsonTags, 0, STRING);
	Ov_SetDynamicVectorLength(&tokenIndex, 0, UINT);
	ov_string_setvalue(&componentID, NULL);
	ov_string_setvalue(&securityKey, NULL);
	for (OV_UINT i = 0; i < endpointArraySize; i++){
		ov_string_setvalue(&endpoints[i].protocolType, NULL);
		ov_string_setvalue(&endpoints[i].endpointString, NULL);
	}
	if (endpoints)
		free(endpoints);
	for (OV_UINT i = 0; i < tagsArraySize; i++){
		ov_string_setvalue(&tags[i].tag, NULL);
		ov_string_setvalue(&tags[i].value, NULL);
	}
	if (tags)
		free(tags);
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_ACCESS DSServices_DSRegistrationServiceType1_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
    /*    
    *   local variables
    */

	switch(pelem->elemtype) {
		case OV_ET_VARIABLE:
			if(pelem->elemunion.pvar->v_offset >= offsetof(OV_INST_ov_object,__classinfo)) {
				if(pelem->elemunion.pvar->v_vartype == OV_VT_CTYPE)
					return OV_AC_NONE;
				else{
					if(pelem->elemunion.pvar->v_flags == 256) { // InputFlag is set
						return OV_AC_READWRITE;
					}
					/* Nicht FB? */
					if(pelem->elemunion.pvar->v_varprops & OV_VP_SETACCESSOR) {
						return OV_AC_READWRITE;
					}
					return OV_AC_READ;
				}
			}
		break;
		default:
		break;
	}

	return ov_object_getaccess(pobj, pelem, pticket);
}
