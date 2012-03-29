#ifndef OV_COMPILE_LIBRARY_ksapi
#define OV_COMPILE_LIBRARY_ksapi
#endif


#include "ksapi.h"
#include "ksapi_h.h"
#include "libov/ov_scheduler.h"

#include "xdrhandling.h"


/*	ksapi_getSingle_receivesingle_get
 *	returns the current value of the receivesingle
 */
OV_DLLFNCEXPORT OV_SINGLE ksapi_getSingle_receivesingle_get(
	OV_INSTPTR_ksapi_getSingle	pobj
) {
	return pobj->v_receivesingle;
}

/*	ksapi_getSingle_receivesingle_set
 *	sets the current receivesingle to value
 */
OV_DLLFNCEXPORT OV_RESULT ksapi_getSingle_receivesingle_set(
	OV_INSTPTR_ksapi_getSingle	pobj,
	const OV_SINGLE				value
) {
	pobj->v_receivesingle=value;
	return OV_ERR_OK;
}

/*	ksapi_getSingle_startup
 *	On startup set all values to default and close all connections
 */
OV_DLLFNCEXPORT void ksapi_getSingle_startup(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_ksapi_getSingle sobj = Ov_StaticPtrCast(ksapi_getSingle, pobj);
	//To avoid a double Startup, which occures, because startup is executed while constructing 
	//the object and starting the FB, this check is implemented
	if (pobj->v_objectstate & OV_OS_STARTED) {
		return;
	}
	ksapi_KSCommon_startup(pobj);
	//initialize Values
	ksapi_getSingle_receivesingle_set(sobj,0);
	
	return;
}

/*	ksapi_getSingle_shutdown
 *	After closing the service and the connection, shutdown the object.
 */
OV_DLLFNCEXPORT void ksapi_getSingle_shutdown(
	OV_INSTPTR_ov_object	pobj
) {
	ksapi_KSCommon_shutdown(pobj);
	return;
}

OV_DLLFNCEXPORT void ksapi_getSingle_returnMethodxdr(
		OV_INSTPTR_ksapi_KSCommon			 	kscommon,
		OV_STRING									xdr,
		OV_INT										xdrlength
) {
	OV_INSTPTR_ksapi_getSingle pgi = Ov_StaticPtrCast(ksapi_getSingle, kscommon);
	OV_VTBLPTR_ksapi_getSingle  pvtableop; 
	OV_SINGLE result;
	
	Ov_GetVTablePtr(ksapi_getSingle, pvtableop, pgi);
	
	//Analyse xdr
	if((analysegetreply(OV_VT_SINGLE, xdr, xdrlength, &result)) == 0)
	{
		ksapi_getSingle_receivesingle_set(pgi, result);
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "Reading completed", 1);
	}
	else
	{
		pvtableop->m_returnMethod((OV_INSTPTR_ov_object)kscommon, "error", -1);
	}
	
	
	return;
}
