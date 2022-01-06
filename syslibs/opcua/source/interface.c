
/******************************************************************************
*
*   FILE
*   ----
*   uaInterface.c
*
*   History
*   -------
*   2019-01-24   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif


#include "opcua.h"
#include "ov_macros.h"
#include "ov_object.h"
#include "ovSwitch.h"
#include "helpers.h"

OV_DLLFNCEXPORT OV_ACCESS opcua_interface_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	return ov_object_getaccessEx(pobj, pelem, pticket);
}

OV_DLLFNCEXPORT OV_RESULT opcua_interface_entryPath_set(
    OV_INSTPTR_opcua_interface          pobj,
    const OV_STRING  value
) {
    // Check value for NULL or zero length
	OV_UINT length = ov_string_getlength(value);
	if(length == 0)
		return OV_ERR_BADPARAM;

	// Check value for valid characters
	OV_STRING *plist = NULL;
	OV_UINT    i,len;
	plist = ov_string_split(value,"/",&len);
	for(i = 0 ; i < len ; i++) {
		length = ov_string_getlength(plist[i]);
		for(OV_UINT j = 0 ; j < length ; j++){
			if(!ov_path_isvalidchar(plist[i][j]))
				return OV_ERR_BADNAME;
		}
	}
	ov_string_freelist(plist);

	OV_INSTPTR_opcua_server server = Ov_GetParent(opcua_serverToInterfaces, pobj);
	if(server != NULL && server->v_isRunning){
        return OV_ERR_GENERIC;
	}

	return ov_string_setvalue(&pobj->v_entryPath, value);
}

OV_DLLFNCEXPORT OV_RESULT opcua_interface_useOvTrafo_set(
    OV_INSTPTR_opcua_interface          pobj,
    const OV_BOOL  value
) {
	OV_INSTPTR_opcua_server server = Ov_GetParent(opcua_serverToInterfaces, pobj);
	if(server != NULL && server->v_isRunning){
        return OV_ERR_GENERIC;
	}
	pobj->v_useOvTrafo = value;
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT opcua_interface_load(OV_INSTPTR_opcua_interface pInterface, UA_Server* pServer) {
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_BOOL opcua_interface_checkNode(OV_INSTPTR_opcua_interface pInterface, OV_INSTPTR_ov_object pObj, OV_STRING virtualNodePath) {
    return FALSE;
}

OV_DLLFNCEXPORT OV_BOOL opcua_interface_checkReference(OV_INSTPTR_opcua_interface pInterface, OV_INSTPTR_ov_object pObj, UA_Node* pNode, UA_Byte refTypeIndex) {
    return FALSE;
}