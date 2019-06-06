
/******************************************************************************
*
*   FILE
*   ----
*   interface.c
*
*   History
*   -------
*   2017-06-26   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_lifeCycleEntryOPCUAInterface
#define OV_COMPILE_LIBRARY_lifeCycleEntryOPCUAInterface
#endif


#include "lifeCycleEntryOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "libov/ov_result.h"
#include "nodeset_lifeCycleEntry.h"
#include "ua_lifeCycleEntry_generated.h"


OV_DLLFNCEXPORT OV_RESULT lifeCycleEntryOPCUAInterface_interface_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_lifeCycleEntryOPCUAInterface_interface pinst = Ov_StaticPtrCast(lifeCycleEntryOPCUAInterface_interface, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    pinst->v_index = 0;
	ov_string_setvalue(&pinst->v_uri, "acplt.org/lifeCycleEntry/"); //Will be overwritten by config->applicationDescription.applicationUri
	UA_Nodestore_Default_Interface_new(&pinst->v_store);
	UA_DataTypeArray *plifeCycleEntryTypes = (UA_DataTypeArray*)UA_malloc(sizeof(UA_DataTypeArray));
	UA_DataTypeArray lifeCycleEntryTypes = {NULL, UA_LIFECYCLEENTRY_COUNT, UA_LIFECYCLEENTRY};
	memcpy(plifeCycleEntryTypes, &lifeCycleEntryTypes, sizeof(lifeCycleEntryTypes));
	pinst->v_dataTypes = plifeCycleEntryTypes;
	pinst->v_trafo = lifeCycleEntryOPCUAInterface_interface_ovNodeStoreInterfaceLifeCycleEntryNew(pinst);


    return OV_ERR_OK;
}


OV_DLLFNCEXPORT void lifeCycleEntryOPCUAInterface_interface_destructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_lifeCycleEntryOPCUAInterface_interface pinst = Ov_StaticPtrCast(lifeCycleEntryOPCUAInterface_interface, pobj);

    /* do what */
	if (pinst->v_store){
		pinst->v_store->deleteNodestore(pinst->v_store->context);
		UA_free(pinst->v_store);
	}
	UA_free(pinst->v_dataTypes);
	lifeCycleEntryOPCUAInterface_interface_ovNodeStoreInterfaceLifeCycleEntryDelete(pinst->v_trafo);
    /* destroy object */
    ov_object_destructor(pobj);

    return;
}

OV_DLLFNCEXPORT OV_BOOL lifeCycleEntryOPCUAInterface_interface_checkNode(OV_INSTPTR_opcua_interface pobj, OV_INSTPTR_ov_object pNode, OV_STRING virtualNodePath, void *context) {
    /*
    *   local variables
    */

	if(Ov_CanCastTo(lifeCycleEntry_LifeCycleEntry, pNode) ||
	   Ov_CanCastTo(lifeCycleEntry_LifeCycleArchive, pNode)){
		return TRUE;
	}
	return FALSE;
}

OV_DLLFNCEXPORT OV_BOOL lifeCycleEntryOPCUAInterface_interface_checkReference(OV_INSTPTR_opcua_interface pobj, OV_UINT applicationIndex, OV_INSTPTR_ov_object pNode, UA_AddReferencesItem * parentRef) {
    /*
    *   local variables
    */

    return (OV_BOOL)0;
}

OV_DLLFNCEXPORT UA_StatusCode lifeCycleEntryOPCUAInterface_interface_nodeset(UA_Server* server) {
    /*
    *   local variables
    */

    return nodeset_lifeCycleEntry(server);
}
