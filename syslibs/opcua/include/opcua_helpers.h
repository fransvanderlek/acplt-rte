/*
 * opcua_helpers.h
 *
 *  Created on: 21.10.2014
 *      Author: lars
 */

#ifndef opcua_HELPERS_H_
#define opcua_HELPERS_H_

#include "opcua.h"
#include "ov_path.h"
#include "ov_call_macros_10.h"

#define OV_OPCUA_DEFAULTNSINDEX 1
#define OV_OPCUA_VIRTUALNODESEPERATOR "||"
#define OV_OPCUA_NSINDEX_UNDEFINED UA_UINT16_MAX
#define OV_OPCUA_DEFAULT_APPLICATIONURI "acplt.org/"
#define OV_OPCUA_DEFAULT_APPLICATIONNAME "ACPLT/RTE" // the suffix /SERVERNAME is appended in uaServer startup via ov_vendortree_getServer, e.g. ACPLT/RTE/MANAGER

UA_Logger opcua_ovUAlogger_new(void);

OV_DLLFNCEXPORT void opcua_helpers_UA_String_append(UA_String * string, const char * append);

OV_DLLFNCEXPORT UA_StatusCode opcua_helpers_ovResultToUaStatusCode(OV_RESULT result);
OV_DLLFNCEXPORT UA_StatusCode opcua_helpers_ovAnyToUAVariant(const OV_ANY* pAny, UA_Variant* pVariant);
OV_DLLFNCEXPORT UA_StatusCode opcua_helpers_UAVariantToOVAny(const UA_Variant* pVariant, OV_ANY* pAny);
OV_DLLFNCEXPORT UA_NodeId opcua_helpers_ovVarTypeToNodeId(OV_VAR_TYPE type);
OV_DLLFNCEXPORT UA_StatusCode opcua_helpers_getVtblPointer(OV_ELEMENT *pelem, OV_INSTPTR_ov_object *pInstance, OV_VTBLPTR_ov_object *ppVtblObj);
OV_DLLFNCEXPORT OV_ACCESS opcua_helpers_getAccess(const OV_ELEMENT* pElem);
OV_DLLFNCEXPORT UA_NodeClass opcua_helpers_getNodeClass(const OV_ELEMENT* pElem);

OV_DLLFNCEXPORT OV_RESULT opcua_helpers_copyUAStringToOV(UA_String src, OV_STRING *dst);
/*
 * resolves a UA-nodeId to an OV_PATH
 * the nodeId has to be of type STRING or NUMERIC
 * in the latter case only objects can be addressed (no variables)
 * the STRING nodeIds are treated as a usual path, so the ov-id can be part of them (/.xxx)
 * Note: the memory for the path elements is allocated on the memory
 * stack, use ov_memstack_lock()/unlock() outside of this function
 */
OV_DLLFNCEXPORT UA_StatusCode opcua_helpers_resolveNodeIdToPath(const UA_NodeId nodeId, OV_PATH* pPath);
/*
 * resolves a UA-nodeId to an object
 * the nodeId has to be of type STRING or NUMERIC
 * in the latter case only objects can be addressed (no variables)
 * the STRING nodeIds are treated as a usual path, so the ov-id can be part of them (/.xxx)
 */
OV_DLLFNCEXPORT OV_INSTPTR_ov_object opcua_helpers_resolveNodeIdToOvObject(const UA_NodeId *nodeId);

OV_DLLFNCEXPORT OV_RESULT opcua_helpers_setRootEntryReference(const OV_STRING newPath, OV_INSTPTR_opcua_interface pobj, OV_STRING * poldPath);
OV_DLLFNCEXPORT OV_RESULT opcua_helpers_addReferencesToTrafo(UA_Server* pUaServer, OV_STRING entryPath);

OV_DLLFNCEXPORT UA_StatusCode
opcua_helpers_addReference(UA_Node* node, UA_Byte refTypeIndex,
		const UA_ExpandedNodeId targetNodeId, OV_STRING targetBrowseName, UA_UInt32 targetBrowseNameIndex, UA_Boolean isForward);

UA_StatusCode opcua_interface_setNamespace(UA_Server* server, const UA_String uriOld, const UA_String uriNew, size_t * indexOut);
OV_DLLFNCEXPORT UA_StatusCode opcua_helpers_getNumericalNodeIdForInputOutputArgs(UA_Server *server, UA_NodeId methodId, UA_NodeId* inArgsId, UA_NodeId* outArgsId);
#endif /* opcua_HELPERS_H_ */
