
/******************************************************************************
 *
 *   FILE
 *   ----
 *   PMAskList.c
 *
 *   History
 *   -------
 *   2015-01-23   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksxdr
#define OV_COMPILE_LIBRARY_ksxdr
#endif


#include "ksxdr.h"
#include "ov_macros.h"
#include "ov_result.h"
#include "ksxdr_config.h"
#include "ks_logfile.h"
#include "ksbase_helper.h"
#include "ksxdr_services.h"

OV_DLLFNCEXPORT OV_RESULT ksxdr_PMAskList_hosts_set(
		OV_INSTPTR_ksxdr_PMAskList          pobj,
		const OV_STRING*  value,
		const OV_UINT veclen
) {
	OV_RESULT result;
	OV_UINT i;
	Ov_SetDynamicVectorLength(&pobj->v_ports, 0, STRING);
	Ov_SetDynamicVectorLength(&pobj->v_results, 0, INT);
	result = Ov_SetDynamicVectorLength(&pobj->v_ports, veclen, STRING);
	if(Ov_Fail(result))
		return result;
	result = Ov_SetDynamicVectorLength(&pobj->v_results, veclen, INT);
	if(Ov_Fail(result))
		return result;
	for(i=0; i < veclen; i++){
		ov_string_setvalue(&(pobj->v_ports.value[i]), NULL);
		pobj->v_results.value[i] = 65536;
	}
	ov_time_gettime(&pobj->v_nextAskTime);
	return Ov_SetDynamicVectorValue(&pobj->v_hosts,value,veclen,STRING);
}

OV_DLLFNCEXPORT OV_RESULT ksxdr_PMAskList_HandleData(
		OV_INSTPTR_ksbase_DataHandler this,
		OV_INSTPTR_ksbase_Channel pChannel,
		KS_DATAPACKET* dataReceived,
		KS_DATAPACKET* answer
) {
	OV_INSTPTR_ksxdr_PMAskList pinst = Ov_StaticPtrCast(ksxdr_PMAskList, this);
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;
	if(Ov_Fail(process_getPort(&pChannel->v_inData, pChannel->v_usesStreamProtocol, &pinst->v_ports.value[pinst->v_index]))){
		pinst->v_results.value[pinst->v_index] = 4;
	} else {
		if(ov_string_compare(pinst->v_ports.value[pinst->v_index], "0") == OV_STRCMP_EQUAL){
			pinst->v_results.value[pinst->v_index] = 65535;
		} else {
			pinst->v_results.value[pinst->v_index] = 0;
		}
	}
	Ov_GetVTablePtr(ksbase_Channel, pVtblChannel, pChannel);
	if(pVtblChannel){
		pVtblChannel->m_CloseConnection(pChannel);
		pinst->v_state = STATE_CLOSING_CONNECTION;
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ksxdr_PMAskList_typemethod (
		OV_INSTPTR_ksbase_ComTask	this
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksxdr_PMAskList	thisAskList	=	Ov_StaticPtrCast(ksxdr_PMAskList, this);
	OV_INSTPTR_ksbase_Channel	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;
	OV_TIME						now;
	OV_RESULT					result;

	/*
	 * state machine
	 */
	switch(thisAskList->v_state)
	{
	case STATE_IDLE:
		ov_time_gettime(&now);
		if(ov_time_compare(&now, &thisAskList->v_nextAskTime) == OV_TIMECMP_BEFORE){
		//	nothing to do yet
			return;
		}
		thisAskList->v_index = 0;
		thisAskList->v_state = STATE_START;
		// fall through
		// no break
	case STATE_START:
		if(thisAskList->v_index < thisAskList->v_hosts.veclen){
			result = ksxdr_getChannelPointer(Ov_PtrUpCast(ksbase_DataHandler,
					thisAskList), &pChannel, &pVtblChannel);
			if(Ov_Fail(result)) {
				KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
				return;
			}
			result = pVtblChannel->m_OpenConnection(pChannel,
					thisAskList->v_hosts.value[thisAskList->v_index], PM_PORT);
			if(Ov_Fail(result)) {
				KS_logfile_error(("%s: Could not open connection to %s.", this->v_identifier, thisAskList->v_hosts.value[thisAskList->v_index]));
				thisAskList->v_results.value[thisAskList->v_index] = 2;
				thisAskList->v_index++;
				return;
			}
			ov_time_gettime(&now);
			thisAskList->v_state = STATE_AWAITING_CONNECTION;
			thisAskList->v_nextAskTime.secs = now.secs + 180;
			thisAskList->v_nextAskTime.usecs = 0;
		} else {
			ov_time_gettime(&now);
			thisAskList->v_state = STATE_IDLE;
			thisAskList->v_nextAskTime.secs = now.secs + 180;
			thisAskList->v_nextAskTime.usecs = 0;
			return;
		}

		// fall through
		// no break
	case STATE_AWAITING_CONNECTION:
		/*	get pointer to channel and to its Vtable	*/
		if(!pChannel || !pVtblChannel){
			result = ksxdr_getChannelPointer(Ov_PtrUpCast(ksbase_DataHandler, thisAskList), &pChannel, &pVtblChannel);
			if(Ov_Fail(result)) {
				KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
				return;
			}
		}
		if(pChannel->v_ConnectionState == KSBASE_CONNSTATE_OPEN){
			result = generate_PMAPPROC(&pChannel->v_outData,
					pChannel->v_usesStreamProtocol, PM_FUNC_GETPORT, PM_PROT_TCP, 0);
			if(Ov_Fail(result)){
				KS_logfile_error(("%s: typemethod: Generation of getport call failed.", this->v_identifier));
			} else if(Ov_Fail(pVtblChannel->m_SendData(pChannel))){
				KS_logfile_error(("%s: typemethod: Sending getport to portmapper failed. Reason: %s", this->v_identifier, ov_result_getresulttext(result)));
				thisAskList->v_results.value[thisAskList->v_index] = 2;
				pVtblChannel->m_CloseConnection(pChannel);
				thisAskList->v_state = STATE_CLOSING_CONNECTION;
				return;
			}

		}
		ov_time_gettime(&thisAskList->v_lastRequestTime);
		thisAskList->v_state = STATE_AWAITING_RESPONSE;
		return;

	case STATE_AWAITING_RESPONSE:
		ov_time_gettime(&now);
		if((now.secs > (thisAskList->v_lastRequestTime.secs + thisAskList->v_timeOut / 1000))
				|| ((now.secs == (thisAskList->v_lastRequestTime.secs + thisAskList->v_timeOut / 1000))
						&& (now.usecs > (thisAskList->v_lastRequestTime.usecs + (thisAskList->v_timeOut % 1000) * 1000)))){
			KS_logfile_info(("%s: typemethod: request for %uth element in list timed out", this->v_identifier, thisAskList->v_index));
			result = ksxdr_getChannelPointer(Ov_PtrUpCast(ksbase_DataHandler, thisAskList), &pChannel, &pVtblChannel);
			if(Ov_Fail(result)) {
				KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
				return;
			}
			pVtblChannel->m_CloseConnection(pChannel);
			thisAskList->v_state = STATE_CLOSING_CONNECTION;
			thisAskList->v_results.value[thisAskList->v_index] = 128;
		}
		return;

	case STATE_CLOSING_CONNECTION:
		result = ksxdr_getChannelPointer(Ov_PtrUpCast(ksbase_DataHandler,
				thisAskList), &pChannel, &pVtblChannel);
		if(Ov_Fail(result)) {
			KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
			return;
		}
		if(pChannel->v_ConnectionState == KSBASE_CONNSTATE_CLOSED){
			thisAskList->v_index++;
			if(thisAskList->v_index < thisAskList->v_hosts.veclen){
				result = ksxdr_getChannelPointer(Ov_PtrUpCast(ksbase_DataHandler,
						thisAskList), &pChannel, &pVtblChannel);
				if(Ov_Fail(result)) {
					KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
					return;
				}

				result = pVtblChannel->m_OpenConnection(pChannel,
						thisAskList->v_hosts.value[thisAskList->v_index], PM_PORT);
				if(Ov_Fail(result)) {
					KS_logfile_error(("%s: Could not open connection to %s.", this->v_identifier, thisAskList->v_hosts.value[thisAskList->v_index]));
					return;
				}
				thisAskList->v_state = STATE_AWAITING_CONNECTION;
			} else {
				thisAskList->v_state = STATE_IDLE;
			}
		}
		break;

	default:
		KS_logfile_debug(("entering default in PMAskList typemethod - this should not happen"));
		thisAskList->v_actimode = 0;
		break;
	}

	return;
}


OV_DLLFNCEXPORT void ksxdr_PMAskList_startup(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksxdr_PMAskList pinst = Ov_StaticPtrCast(ksxdr_PMAskList, pobj);
	OV_RESULT result = OV_ERR_OK;
	OV_UINT i;
	/* do what the base class does first */
	ov_object_startup(pobj);

	/* do what */
	result |= Ov_SetDynamicVectorLength(&pinst->v_ports, pinst->v_hosts.veclen, STRING);
	result |= Ov_SetDynamicVectorLength(&pinst->v_results, pinst->v_hosts.veclen, INT);
	if(Ov_OK(result)){
		for(i=0; i < pinst->v_hosts.veclen; i++){
			ov_string_setvalue(&(pinst->v_ports.value[i]), NULL);
			pinst->v_results.value[i] = 65536;
		}
	}
	ov_time_gettime(&pinst->v_nextAskTime);

	return;
}

OV_DLLFNCEXPORT void ksxdr_PMAskList_shutdown(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksxdr_PMAskList pinst = Ov_StaticPtrCast(ksxdr_PMAskList, pobj);

	/* do what */
	Ov_SetDynamicVectorLength(&pinst->v_ports, 0, STRING);
	Ov_SetDynamicVectorLength(&pinst->v_results, 0, INT);
	/* set the object's state to "shut down" */
	ov_object_shutdown(pobj);

	return;
}

