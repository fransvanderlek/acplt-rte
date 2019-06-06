
/******************************************************************************
*
*   FILE
*   ----
*   ovInterface.c
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
#include "libov/ov_macros.h"
#include "opcua_ovStore.h"
#include "opcua_helpers.h"
#include "opcua_ovTrafo.h"

OV_DLLFNCEXPORT OV_RESULT opcua_ovInterface_entryPath_set(
    OV_INSTPTR_opcua_ovInterface          pobj,
    const OV_STRING  value
) {
    return opcua_helpers_setRootEntryReference(value, Ov_StaticPtrCast(opcua_interface, pobj), &pobj->v_entryPath);
}

OV_DLLFNCEXPORT OV_RESULT opcua_ovInterface_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_ovInterface pinst = Ov_StaticPtrCast(opcua_ovInterface, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    pinst->v_index = 0;
    ov_string_setvalue(&pinst->v_uri, OPCUA_DEFAULT_APPLICATIONURI); //Will be overwritten by config->applicationDescription.applicationUri
    pinst->v_store = opcua_ovStore_new(pinst);;
    pinst->v_dataTypes = NULL;
    pinst->v_trafo = opcua_ovTrafo_new(pinst);





	//Link generic ov interface interface to server as first association if it is a part
    if(pinst->v_pouterobject != NULL && Ov_CanCastTo(opcua_server, pinst->v_pouterobject)){
    	Ov_LinkPlaced(opcua_serverToInterfaces,
    			Ov_StaticPtrCast(opcua_server,pinst->v_pouterobject),
				Ov_PtrUpCast(opcua_interface, pinst),
				OV_PMH_BEGIN);
    }
    return OV_ERR_OK;
}

OV_DLLFNCEXPORT void opcua_ovInterface_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_opcua_ovInterface pinst = Ov_StaticPtrCast(opcua_ovInterface, pobj);

    /* do what */
    ov_string_setvalue(&pinst->v_uri, NULL);
    if (pinst->v_store)
    	opcua_ovStore_delete(pinst->v_store);
    if (pinst->v_trafo)
    	opcua_ovTrafo_delete(pinst->v_trafo);

    /* destroy object */
    ov_object_destructor(pobj);

    return;
}

OV_DLLFNCEXPORT OV_RESULT opcua_ovInterface_load(OV_INSTPTR_opcua_interface pobj, OV_BOOL forceLoad) {
    /*
    *   local variables
    */
    OV_INSTPTR_opcua_ovInterface pinst = Ov_StaticPtrCast(opcua_ovInterface, pobj);
	OV_INSTPTR_opcua_server uaServer = Ov_GetParent(opcua_serverToInterfaces, pobj);
	if(uaServer == NULL){
		return OV_ERR_GENERIC;
	}

    //Use generic load method of uaInterface to load the trafos
	opcua_interface_load(pobj, TRUE);

	//Add reference to OV root for generic interface
	UA_StatusCode retval = UA_STATUSCODE_GOOD;
	retval = UA_Server_addReference(uaServer->v_server, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
			UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), UA_EXPANDEDNODEID_STRING(pobj->v_index, pinst->v_entryPath), UA_TRUE);
	if(retval != UA_STATUSCODE_GOOD){
		Ov_Warning(UA_StatusCode_name(retval));
	}
	//Add reference to ov domain
	retval = UA_Server_addReference(uaServer->v_server, UA_NODEID_NUMERIC(0, UA_NS0ID_FOLDERTYPE),
			UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_EXPANDEDNODEID_STRING(pobj->v_index, "/acplt/ov/domain"), UA_TRUE);
	if(retval != UA_STATUSCODE_GOOD){
		Ov_Warning(UA_StatusCode_name(retval));
	}
	//Add reference to ov object
	retval = UA_Server_addReference(uaServer->v_server, UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
			UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), UA_EXPANDEDNODEID_STRING(pobj->v_index, "/acplt/ov/object"), UA_TRUE);
	if(retval != UA_STATUSCODE_GOOD){
		Ov_Warning(UA_StatusCode_name(retval));
	}

    return OV_ERR_OK;
}

//TODO add unload function to delete references