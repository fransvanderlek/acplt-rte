
/******************************************************************************
 *
 *   FILE
 *   ----
 *   xdrClient_main.c
 *
 *   History
 *   -------
 *   2013-02-22   File created
 *
 *******************************************************************************
 *
 *   This file is generated by the 'acplt_builder' command
 *
 ******************************************************************************/

/************************************************************************************************************************************************************************
 * The xdrClient Handler is based on a state machine
 * the states are
 * INITIAL:
 * 		nothing is done. one can start from here. typemethod is deactivated.
 * AWAITINGCONNECTION:
 * 		a ks-service was requested. The associated channel is opening a connection.
 * 		the typemethod polls the connection status of the channel. if a connection is opened the request is sent and the state is changed to AWAITINGANSWER
 * 		if the connection does not open withing timeout seconds the typemethod sets an errorstate and deactivates itself
 * AWAITINGANSWER
 * 		the typemethod just calculates the timeout.
 * 		upon receipt of a reply the HandleData function is called by the Channel
 * 		HandleData tries to process the generic part of the message. if handle data fails because the message is incomplete the state is changed to BUSY.
 * 		if it fails for some other reason the state is changed to ERROR and the typemethod is deactivated
 * 		if HandleData succeeds the state is changed to COMPLETED
 * BUSY
 *		the typemethod calls HandleData again to try processing the answer. state changes are the same as in AWAITINGANSWER
 *		the timeout is calculated
 * ERROR
 *		the typemethod is deactivated
 *
 ************************************************************************************************************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksxdr
#define OV_COMPILE_LIBRARY_ksxdr
#endif


#include "ksxdr.h"
#include "ov_macros.h"
#include "ov_malloc.h"
#include "ov_result.h"
#include "ov_ksserver_backend.h"
#include "KSDATAPACKET_xdrhandling.h"
#include "ksxdr_config.h"
#include "ks_logfile.h"
#include "ksbase_helper.h"
#include "ksxdr_services.h"



/*******************************************************************************************************************************************************************************
 * 				typemethod
 *******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT void ksxdr_xdrClient_typemethod (
	OV_INSTPTR_ksbase_ComTask	this
) {
	OV_INSTPTR_ksxdr_xdrClient	thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_INSTPTR_ksbase_Channel	pChannel = NULL;
	OV_VTBLPTR_ksbase_Channel	pVtblChannel = NULL;
	OV_TIME						now;
	OV_TIME_SPAN				tstemp;
	OV_RESULT					result;

	/*
	 * state machine
	 */
	switch(thisCl->v_state)
	{
	case KSBASE_CLST_AWAITINGCONNECTION:	/*	check if connection is open now. if so, send. otherwise calculate timeout and return	*/
		/*	get pointer to channel and to its Vtable	*/
		result = ksxdr_getChannelPointer(Ov_PtrUpCast(ksbase_DataHandler,thisCl), &pChannel, &pVtblChannel);
		if(Ov_Fail(result))
		{
			KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
			return;
		}
		result = ksxdr_trySend(thisCl, pChannel, pVtblChannel);
		if(Ov_Fail(result))
			return;

		ov_time_gettime(&now);
		ov_time_diff(&tstemp, &now, &(thisCl->v_timeLastEvent));
		if((tstemp.secs > (OV_INT) thisCl->v_timeout) || ((tstemp.secs == (OV_INT) thisCl->v_timeout) && (tstemp.usecs > 0)))
		{
			thisCl->v_state = KSBASE_CLST_ERROR | XDRCL_TIMEOUT;
			thisCl->v_actimode = 0;
			ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
		}
		return;

	case KSBASE_CLST_AWAITINGANSWER:	/*	waiting for answer --> just calculate timeouts	*/
			ov_time_gettime(&now);
			ov_time_diff(&tstemp, &now, &(thisCl->v_timeLastEvent));
			if((tstemp.secs > (OV_INT) thisCl->v_timeout) || ((tstemp.secs == (OV_INT) thisCl->v_timeout) && (tstemp.usecs > 0)))
			{
				thisCl->v_state = KSBASE_CLST_ERROR | XDRCL_TIMEOUT;
				thisCl->v_actimode = 0;
				ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
			}
			return;

	case KSBASE_CLST_BUSY:	/*	call HandleData again to process data	*/
			/*	calculate timeout	*/
		ov_time_gettime(&now);
		ov_time_diff(&tstemp, &now, &(thisCl->v_timeLastEvent));
		if((tstemp.secs > (OV_INT) thisCl->v_timeout) || ((tstemp.secs == (OV_INT) thisCl->v_timeout) && (tstemp.usecs > 0)))
		{
			thisCl->v_state = KSBASE_CLST_ERROR | XDRCL_TIMEOUT;
			thisCl->v_actimode = 0;
			ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));
			return;
		}
		/*	get pointer to channel and to its Vtable	*/
		result = ksxdr_getChannelPointer(Ov_PtrUpCast(ksbase_DataHandler,thisCl), &pChannel, &pVtblChannel);
		if(Ov_Fail(result))
		{
			KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
			return;
		}
		/*	HadleData will do the state change to COMPLETED if the header could be processed and the message is completely received	*/
		result = ksxdr_xdrClient_HandleData(Ov_StaticPtrCast(ksbase_DataHandler, this), pChannel, &(pChannel->v_inData), NULL);
		if(Ov_Fail(result))
		{
			thisCl->v_state = KSBASE_CLST_ERROR;
			thisCl->v_actimode = 0;
		}
		break;

	case KSBASE_CLST_COMPLETED:	/*	issue the callback	*/
		thisCl->v_actimode = 0;
		KS_logfile_debug(("%s: completed. issuing callback", thisCl->v_identifier));
		if(thisCl->v_callback.callbackFunction)
			thisCl->v_callback.callbackFunction(thisCl->v_callback.instanceCalled, Ov_StaticPtrCast(ov_domain, this));

		if(!thisCl->v_holdConnection)	/*	close connection if not otherwise specified	*/
		{
			result = ksxdr_getChannelPointer(Ov_PtrUpCast(ksbase_DataHandler,thisCl), &pChannel, &pVtblChannel);
			if(Ov_Fail(result))
			{
				KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
				return;
			}

			pVtblChannel->m_CloseConnection(pChannel);
		}
		return;

	case (KSBASE_CLST_COMPLETED | XDRCL_WAITINGFORPORT):
	{/*	set the serverPort variable, open new connection and send previously generated message
	 	 this status is only reached, if we asked the manager for the port	*/
		OV_RESULT	getSrvRes;
		OV_STRING	servername = NULL;
		OV_UINT		serverVersion;
		OV_STRING	serverPort = NULL;
		OV_TIME		serverExpTime;
		OV_UINT		serverState;

		KS_logfile_debug(("%s: getserver for next ks-service completed. going on with service.", thisCl->v_identifier));
		if(!thisCl->v_serverPort || !(*thisCl->v_serverPort))
		{
			/*	set port	*/
			ov_memstack_lock();
			result = ksxdr_xdrClient_processGetServer(Ov_StaticPtrCast(ksbase_ClientBase, thisCl), NULL, &getSrvRes, &servername, &serverVersion, &serverPort, &serverExpTime, &serverState);
			if(Ov_Fail(result))
			{
				KS_logfile_error(("%s: Could not process getserver. reason: %lu", this->v_identifier, result));
				ov_memstack_unlock();
				thisCl->v_state = KSBASE_CLST_ERROR;
				thisCl->v_actimode = 0;
				return;
			}
			if(Ov_OK(getSrvRes))
			{
				result = ov_string_setvalue(&(thisCl->v_serverPort), serverPort);
				ov_memstack_unlock();
				if(Ov_Fail(result))
				{
					KS_logfile_error(("%s: Could not set serverport. reason: %lu", this->v_identifier, result));
					thisCl->v_state = KSBASE_CLST_ERROR;
					thisCl->v_actimode = 0;
					return;
				}
			}
			else
			{
				KS_logfile_error(("%s: getserver returned %lu. Aborting.", this->v_identifier, getSrvRes));
				thisCl->v_rpcErrCode = getSrvRes;
				ov_memstack_unlock();
				thisCl->v_state = KSBASE_CLST_ERROR;
				thisCl->v_actimode = 0;
				return;
			}
		}
		result = ksxdr_getChannelPointer(Ov_PtrUpCast(ksbase_DataHandler,thisCl), &pChannel, &pVtblChannel);
		if(Ov_Fail(result))
		{
			KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
			return;
		}
			/*	to use non blocking (and blocking) connections: the client will remain in this state until everything is done	*/
		if((pChannel->v_ConnectionState != KSBASE_CONNSTATE_CLOSED) && (pChannel->v_ConnectionState != KSBASE_CONNSTATE_CLOSING))
		{
			pVtblChannel->m_CloseConnection(pChannel);
			return;
		}
		if(pChannel->v_ConnectionState == KSBASE_CONNSTATE_CLOSED)
		{
			ksxdr_initiateConnection(thisCl, pChannel, pVtblChannel, FALSE, thisCl->v_serverHost, thisCl->v_serverPort);
			thisCl->v_state = (KSBASE_CLST_COMPLETED | KSBASE_CLST_AWAITINGCONNECTION);
			return;
		}
		return;
	}

	case (KSBASE_CLST_COMPLETED | KSBASE_CLST_AWAITINGCONNECTION):
		/*	reopening connection after getserver
		 	 this status is only reached, if we asked the manager for the port	*/
		result = ksxdr_getChannelPointer(Ov_PtrUpCast(ksbase_DataHandler,thisCl), &pChannel, &pVtblChannel);
		if(Ov_Fail(result))
		{
			KS_logfile_error(("%s: Could not get Channel pointers.", this->v_identifier));
			return;
		}
		if(pChannel->v_ConnectionState == KSBASE_CONNSTATE_OPEN)
		{
			/*	channel is fine --> hand over request xdr to channel and send it, set sentXID and callbackfunction	*/
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			pChannel->v_outData = thisCl->v_dataToSend;	/*	will be freed by channel after sending	*/
			thisCl->v_dataToSend.data = NULL;	/*	reset dataToSend	*/
			thisCl->v_dataToSend.length = 0;
			thisCl->v_dataToSend.readPT = NULL;
			thisCl->v_dataToSend.writePT = NULL;
			thisCl->v_sentXID = thisCl->v_tempXID;
			thisCl->v_callback = thisCl->v_tempCallback;
			pVtblChannel->m_SendData(pChannel);
			thisCl->v_state = KSBASE_CLST_AWAITINGANSWER;
		}
	break;
	case KSBASE_CLST_ERROR:
		thisCl->v_actimode = 0;
		return;

	default:
		KS_logfile_debug(("entering default in xdrClient typemethod - this should not happen"));
		thisCl->v_actimode = 0;
		break;
	}

	return;
}

/*******************************************************************************************************************************************************************************
 * 				HandleData
 *******************************************************************************************************************************************************************************/
OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_HandleData(
	OV_INSTPTR_ksbase_DataHandler this,
	OV_INSTPTR_ksbase_Channel pChannel,
	KS_DATAPACKET* dataReceived,
	KS_DATAPACKET* answer
) {
	OV_INSTPTR_ksxdr_xdrClient				thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_INSTPTR_ksbase_ClientTicketGenerator	pTicketGenerator = NULL;
	OV_UINT									xid;
	OV_RESULT								result;
	OV_UINT									procHeaderStatus;

	/*	process header and generic stuff	*/
	if(!pChannel) {
		KS_logfile_error(("%s: HandleData: no Channel object associated. cannot process data.", this->v_identifier));
		return OV_ERR_GENERIC;
	}

	pTicketGenerator = Ov_GetChild(ksbase_AssocClientToTicketGenerator, thisCl);

	KS_logfile_debug(("length of XDR: %lu\ndata: %p\nreadPT: %p\n", dataReceived->length, dataReceived->data, dataReceived->readPT));

	result = ksxdr_processServerReplyHeader(dataReceived, pTicketGenerator, thisCl->v_sentXID, pChannel->v_usesStreamProtocol, &xid, &(thisCl->v_msgAccepted),
			&(thisCl->v_msgStatus), &procHeaderStatus);
	KS_logfile_debug(("length of XDR: %lu\nreadPT - data: %lu\n", dataReceived->length, dataReceived->readPT-dataReceived->data));
	if(Ov_Fail(result))	/*	processing header failed	*/
	{
		if(procHeaderStatus == XDRCL_PROCANS_INCOMPLETE)
		{	/*	message incomplete --> keep waiting	*/
			thisCl->v_state = KSBASE_CLST_BUSY;
			thisCl->v_actimode = 1;
			return OV_ERR_OK;
		}
		if(procHeaderStatus == XDRCL_PROCANS_XIDMISSMATCH)
		{	/*	XID missmatch detected. free data, deactivate typemethod and set state to error	*/
			KS_logfile_error(("%s: HandleData: XID missmatch detected.", this->v_identifier));
			ksbase_free_KSDATAPACKET(dataReceived);
			thisCl->v_state = KSBASE_CLST_ERROR;
			thisCl->v_actimode = 0;
		}
		return result;
	}

	/*	handling message acceptance and status	*/
	if(thisCl->v_msgAccepted != XDR_MSG_ACCEPTED || thisCl->v_msgStatus != XDR_MSGST_SUCCESS)
	{
		KS_logfile_error(("%s: HandleData: server replied message denied or did not succeed in processing.", this->v_identifier));
		ksbase_free_KSDATAPACKET(dataReceived);
		thisCl->v_state = KSBASE_CLST_ERROR;
		thisCl->v_actimode = 0;
	}
	else
	{
		KS_logfile_debug(("decoding successful"));
		/*	global part finished. copy data from dataReceived to this->v_dataReceived	*/
		result = ksbase_KSDATAPACKET_append(&(thisCl->v_dataReceived), dataReceived->readPT, (dataReceived->length - (dataReceived->readPT - dataReceived->data)));
		if(Ov_Fail(result))
			return result;
		ksbase_free_KSDATAPACKET(dataReceived);
	/*
	 * we need to differentiate if we're doing a regular request here or if we're getting the port (GETSERVER) for another ks-service
	 */
		if(((!thisCl->v_serverPort) || !(*thisCl->v_serverPort)) && ((thisCl->v_sentProcID != KS_GETSERVER) && (thisCl->v_sentProcID != KS_REGISTER) && (thisCl->v_sentProcID != KS_UNREGISTER)))
		{	/*	port unknown and not working on getserver, register or unregister --> we're getting a port for another ks-service --> set state to COMPLETED | WAITINGFORPORT	*/
			thisCl->v_state = KSBASE_CLST_COMPLETED | XDRCL_WAITINGFORPORT;
		}
		else
			thisCl->v_state = KSBASE_CLST_COMPLETED;

		//make sure the typemethod calls the callback and do other stuff
		thisCl->v_actimode = 1;
	}
	return OV_ERR_OK;
}

/*******************************************************************************************************************************************************************************
 * 				Startup
 *******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT void ksxdr_xdrClient_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
    OV_INSTPTR_ksxdr_xdrClient this = Ov_StaticPtrCast(ksxdr_xdrClient, pobj);

    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */
    this->v_dataReceived.data = NULL;
    this->v_dataReceived.length = 0;
    this->v_dataReceived.readPT = NULL;
    this->v_dataReceived.writePT = NULL;
    this->v_dataToSend.data = NULL;
    this->v_dataToSend.length = 0;
    this->v_dataToSend.readPT = NULL;
    this->v_dataToSend.writePT = NULL;
    return;
}

/*******************************************************************************************************************************************************************************
 * 				Shutdown
 *******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT void ksxdr_xdrClient_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ksbase_ClientBase this = Ov_StaticPtrCast(ksbase_ClientBase, pobj);
	/* do what */

	ksxdr_xdrClient_reset(this);
	this->v_cycInterval = 100;
	/* set the object's state to "shut down" */
	ov_object_shutdown(pobj);


    return;
}

/*******************************************************************************************************************************************************************************
 * 				Reset
 *******************************************************************************************************************************************************************************/

OV_DLLFNCEXPORT OV_RESULT ksxdr_xdrClient_reset(OV_INSTPTR_ksbase_ClientBase this)
{
	OV_INSTPTR_ksxdr_xdrClient	thisCl = Ov_StaticPtrCast(ksxdr_xdrClient, this);
	OV_RESULT					result;

	result = ksbase_ClientBase_reset(this);
	if(Ov_Fail(result)){
		return result;
	}

	ksbase_free_KSDATAPACKET(&(thisCl->v_dataReceived));
	ksbase_free_KSDATAPACKET(&(thisCl->v_dataToSend));

	thisCl->v_msgAccepted = 0;
	thisCl->v_msgStatus = 0;
	thisCl->v_rpcErrCode = 0;
	thisCl->v_runningKSservice = 0;
	thisCl->v_sentProcID = 0;
	thisCl->v_sentXID = 0;

	return OV_ERR_OK;
}
