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

#ifndef OV_COMPILE_LIBRARY_opcua
#define OV_COMPILE_LIBRARY_opcua
#endif

#include "libov/ov_macros.h"
#include "ksbase.h"
#include "opcua.h"
#include "opcua_helpers.h"
#include "NoneTicketAuthenticator.h"
#include "libov/ov_path.h"
#include "libov/ov_memstack.h"
#include "ks_logfile.h"

extern OV_INSTPTR_opcua_uaServer opcua_pUaServer;


OV_DLLFNCEXPORT UA_StatusCode opcua_nodeStoreFunctions_opcuaReferenceTypeNodeToOv(
		void *handle, UA_ReferenceTypeNode* opcuaNode, OV_INSTPTR_opcua_referenceTypeNode* ovNode, UA_NodeId *parrentNode) {

	OV_RESULT result = 0;
	OV_INSTPTR_ov_domain ptr = NULL;
//	if (parrentNode != NULL){
//		ptr = Ov_StaticPtrCast(ov_domain, opcua_nodeStoreFunctions_resolveNodeIdToOvObject(parrentNode));
//		if (ptr == NULL)
//			ptr = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &pdb->root, "TechUnits"));
//	}else
		ptr = Ov_StaticPtrCast(ov_domain, Ov_SearchChild(ov_containment, &pdb->root, "TechUnits"));
	OV_STRING tmpString = NULL;
	ov_string_print(&tmpString, "%u", opcuaNode->nodeId.identifier.numeric);
	*ovNode = NULL;
	result = Ov_CreateObject(opcua_referenceTypeNode, *ovNode, ptr, tmpString);
	if (*ovNode == NULL)
		return result;

	copyOPCUAQualifiedNameToOV	(&opcuaNode->browseName	, &(*ovNode)->p_browseName);
	copyOPCUALocalizedTextToOV	(&opcuaNode->description, &(*ovNode)->p_description);
	copyOPCUALocalizedTextToOV	(&opcuaNode->displayName, &(*ovNode)->p_displayName);
	copyOPCUANodeIdToOV			(&opcuaNode->nodeId		, &(*ovNode)->p_nodeId);


	(*ovNode)->v_nodeClass 		= opcuaNode->nodeClass;
	//(*ovNode)->v_userWriteMask 	= opcuaNode->userWriteMask;
	(*ovNode)->v_writeMask 		= opcuaNode->writeMask;

	//copy references to ov node structure
	OV_INSTPTR_opcua_reference tmpRefNode = NULL;
	for (size_t i = 0; i < opcuaNode->referencesSize; i++){
		ov_string_print(&tmpString, "Reference%u:%u", opcuaNode->references[i].referenceTypeId.identifier.numeric, opcuaNode->references[i].targetId.nodeId.identifier.numeric);
		result = createOpcuaReferenceNode(Ov_StaticPtrCast(ov_domain, &(*ovNode)->p_references), &tmpRefNode, tmpString);
		if (tmpRefNode == NULL)
			return result;
		copyOPCUAReferenceNodeToOV(&opcuaNode->references[i], tmpRefNode);
	}

	//referencetype related fields
	(*ovNode)->v_isAbstract = opcuaNode->isAbstract;
	(*ovNode)->v_symmetric = opcuaNode->symmetric;
	copyOPCUALocalizedTextToOV	(&opcuaNode->inverseName, &(*ovNode)->p_inverseName);
	return result;
}
