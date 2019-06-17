
/******************************************************************************
*
*   FILE
*   ----
*   interface.c
*
*   History
*   -------
*   2017-09-22   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_servicesOPCUAInterface
#define OV_COMPILE_LIBRARY_servicesOPCUAInterface
#endif


#include "servicesOPCUAInterface.h"
#include "libov/ov_macros.h"
#include "nodeset_services.h"

OV_DLLFNCEXPORT OV_RESULT servicesOPCUAInterface_interface_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_servicesOPCUAInterface_interface pinst = Ov_StaticPtrCast(servicesOPCUAInterface_interface, pobj);
    OV_RESULT    result;

    /* do what the base class does first */
    result = ov_object_constructor(pobj);
    if(Ov_Fail(result))
         return result;

    /* do what */
    pinst->v_index = 0;
	ov_string_setvalue(&pinst->v_uri, "acplt.org/services/");
	//TODO add dependent interface acplt.org/identification/

    return OV_ERR_OK;
}


OV_DLLFNCEXPORT void servicesOPCUAInterface_interface_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_servicesOPCUAInterface_interface pinst = Ov_StaticPtrCast(servicesOPCUAInterface_interface, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */
	UA_Nodestore_Default_Interface_new(&pinst->v_store);
	pinst->v_dataTypes = NULL;
	pinst->v_trafo = servicesOPCUAInterface_interface_ovNodeStoreInterfaceServicesNew(pinst);

    return;
}

OV_DLLFNCEXPORT void servicesOPCUAInterface_interface_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_servicesOPCUAInterface_interface pinst = Ov_StaticPtrCast(servicesOPCUAInterface_interface, pobj);

    /* do what */
	if (pinst->v_store){
		pinst->v_store->deleteNodestore(pinst->v_store->context);
		UA_free(pinst->v_store);
	}
	servicesOPCUAInterface_interface_ovNodeStoreInterfaceServicesDelete(pinst->v_trafo);

    /* set the object's state to "shut down" */
    ov_object_shutdown(pobj);

    return;
}


OV_DLLFNCEXPORT OV_BOOL servicesOPCUAInterface_interface_checkNode(OV_INSTPTR_opcua_interface pobj, OV_INSTPTR_ov_object pNode, OV_STRING virtualNodePath, void *context) {
    /*
    *   local variables
    */
	if(Ov_CanCastTo(services_Service, pNode)){
		return TRUE;
	}

	return FALSE;
}

OV_DLLFNCEXPORT OV_BOOL servicesOPCUAInterface_interface_checkReference(OV_INSTPTR_opcua_interface pobj, OV_UINT applicationIndex, OV_INSTPTR_ov_object pNode, UA_AddReferencesItem * parentRef) {
    /*
    *   local variables
    */
	if(Ov_CanCastTo(services_Service, pNode)){
		if(parentRef){
			parentRef->isForward = TRUE;
			parentRef->referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
			//parentRef->sourceNodeId
			parentRef->targetNodeClass = UA_NODECLASS_METHOD;
			ov_memstack_lock();
			parentRef->targetNodeId = UA_EXPANDEDNODEID_STRING_ALLOC(applicationIndex, ov_path_getcanonicalpath(pNode,2));
			ov_memstack_unlock();
			//parentRef->targetServerUri
		}
		return TRUE;
	}
	return FALSE;
}

OV_DLLFNCEXPORT UA_StatusCode servicesOPCUAInterface_interface_nodeset(UA_Server* server) {
    /*
    *   local variables
    */
	return nodeset_services(server);
}

