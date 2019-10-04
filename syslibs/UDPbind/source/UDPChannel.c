
/******************************************************************************
*
*   FILE
*   ----
*   UDPChannel.c
*
*   History
*   -------
*   2015-01-20   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_UDPbind
#define OV_COMPILE_LIBRARY_UDPbind
#endif


#include "UDPbind.h"
#include "ov_macros.h"
#include "ov_result.h"
#include "UDPbind_helper.h"
#include "UDPbind_config.h"
#include "ks_logfile.h"

OV_DLLFNCEXPORT OV_INT UDPbind_UDPChannel_socket_get(
		OV_INSTPTR_UDPbind_UDPChannel          pobj
) {
	return pobj->v_socket;
}

OV_DLLFNCEXPORT OV_RESULT UDPbind_UDPChannel_socket_set(
    OV_INSTPTR_UDPbind_UDPChannel          pobj,
    const OV_INT  value
) {
	OV_INT activesocket;
	UDPBIND_SOCKET socket = UDPBIND_INVALID_SOCKET;

	activesocket = UDPbind_UDPChannel_socket_get(pobj);
	if(activesocket >= 0 && activesocket != value)
	{
		OV_UDPBIND_SETINT2SOCKET(activesocket, socket);
		UDPBIND_CLOSE_SOCKET(socket);
		KS_logfile_debug(("UDPChannel/socket %s closing socket %d", pobj->v_identifier, activesocket));
	}
	pobj->v_socket = value;
	if(pobj->v_socket < 0){
		pobj->v_ConnectionState = UDPbind_CONNSTATE_CLOSED;
	}

	//activate typemethod if a socket is there
	if(value >= 0){
		pobj->v_actimode = 1;
	}

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT UDPbind_UDPChannel_SendData(
	OV_INSTPTR_ksbase_Channel this
) {
	OV_INSTPTR_UDPbind_UDPChannel pChannel = Ov_StaticPtrCast(UDPbind_UDPChannel, this);
	OV_INT ret;
	if((pChannel->v_socket != -1) && pChannel->v_outData.length){
		do{
			ret = sendto(pChannel->v_socket, (char*) pChannel->v_outData.readPT,
					pChannel->v_outData.length - (pChannel->v_outData.readPT - pChannel->v_outData.data),
					0, (struct sockaddr*) &pChannel->v_remoteAddress, pChannel->v_remoteAddrLen);
			if (ret == UDPBIND_SOCKET_ERROR) {
				KS_logfile_error(("%s: error sending data", this->v_identifier));
				KS_logfile_print_sysMsg();
				ksbase_free_KSDATAPACKET(&pChannel->v_outData);
				pChannel->v_remoteAddrLen = 0;
				return OV_ERR_GENERIC;
			}
			KS_logfile_debug(("UDPCHannel: sent %d bytes.", ret));
			pChannel->v_outData.readPT += ret;
		} while(pChannel->v_outData.readPT < (pChannel->v_outData.data + pChannel->v_outData.length));
		if((pChannel->v_outData.readPT - pChannel->v_outData.data) >= (OV_INT) pChannel->v_outData.length)
		{
			ksbase_free_KSDATAPACKET(&(pChannel->v_outData));
			KS_logfile_debug(("%s: everything sent", pChannel->v_identifier));
		}
	}
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void UDPbind_UDPChannel_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_UDPbind_UDPChannel this = Ov_StaticPtrCast(UDPbind_UDPChannel, pobj);

    int socket;
    /* do what the base class does first */
    ksbase_Channel_startup(pobj);

    /* do what */
    Ov_StaticPtrCast(UDPbind_UDPChannel, pobj)->v_cycInterval = 1;
    Ov_StaticPtrCast(UDPbind_UDPChannel, pobj)->v_usesStreamProtocol = FALSE;

    socket = UDPbind_UDPChannel_socket_get(this);
    if(socket >= 0) {
    	KS_logfile_info(("UDPChannel/startup: %s starting with socket %d. resetting.", pobj->v_identifier, socket));
    	this->v_socket = -1;
    }
    if(this->v_ConnectionState != UDPbind_CONNSTATE_CLOSED)
    {
    	KS_logfile_info(("UDPChannel/startup: %s starting with connectionState not CLOSED. resetting.", pobj->v_identifier));
    	this->v_ConnectionState = UDPbind_CONNSTATE_CLOSED;
    }
    if(this->v_actimode == 1)
    	this->v_actimode = 0;
    //set time of creation of the connection
    ov_time_gettime(&(Ov_StaticPtrCast(UDPbind_UDPChannel, pobj)->v_LastReceiveTime));
    return;
}

OV_DLLFNCEXPORT void UDPbind_UDPChannel_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_UDPbind_UDPChannel this = Ov_StaticPtrCast(UDPbind_UDPChannel, pobj);
	OV_INSTPTR_ksbase_ClientHandler pClientHandler = NULL;
	int socket;
	/* do what */

	KS_logfile_debug(("UDPChannel/shutdown: %s", pobj->v_identifier));
	socket = UDPbind_UDPChannel_socket_get(this);
	if(socket >= 0)
	{
		UDPBIND_CLOSE_SOCKET(socket);
		KS_logfile_debug(("UDPChannel/shutdown %s closing socket %d", pobj->v_identifier, socket));
		UDPbind_UDPChannel_socket_set(this, -1);
		this->v_ConnectionState = UDPbind_CONNSTATE_CLOSED;
	}


	/*
	 * when acting on the server side, delete associated ClientHandlers (they won't be used anymore)
	 *  and then delete oneself (connections to a server are not static)
	 */

	if(this->v_ClientHandlerAssociated == KSBASE_CH_ASSOCIATED)
	{
		pClientHandler = Ov_GetChild(ksbase_AssocChannelClientHandler, this);
		if(pClientHandler)
		{
			// activate client handler so it can delete itself
			pClientHandler->v_actimode = 1;
		}
		this->v_ClientHandlerAssociated = KSBASE_CH_NOTASSOCATIED;
	}


	/* set the object's state to "shut down" */
	ksbase_Channel_shutdown(pobj);

	return;
}

OV_DLLFNCEXPORT void UDPbind_UDPChannel_typemethod (
	OV_INSTPTR_ksbase_ComTask	this
) {
    /*    
    *   local variables
    */
	OV_INSTPTR_UDPbind_UDPChannel thisCh = Ov_StaticPtrCast(UDPbind_UDPChannel, this);
	OV_INSTPTR_ksbase_ClientHandler pClientHandler = NULL;
	OV_VTBLPTR_ksbase_ClientHandler pVTBLClientHandler = NULL;
	OV_INSTPTR_ksbase_DataHandler pDataHandler = NULL;
	OV_VTBLPTR_ksbase_DataHandler pVTBLDataHandler = NULL;
	UDPBIND_SOCKET socket = UDPBIND_INVALID_SOCKET;
	int intsocket = -1;
	fd_set read_flags;
	struct timeval waitd;
	int err = 0;
	OV_TIME now;
	OV_TIME_SPAN tstemp;
	OV_TIME ttemp;
	OV_RESULT result;
	OV_BYTE* tempdata = NULL;
	OV_BOOL datareceived = FALSE;

	OV_INSTPTR_ksbase_RootComTask RCTask = NULL;

	RCTask = Ov_StaticPtrCast(ksbase_RootComTask, Ov_GetFirstChild(ov_instantiation, pclass_ksbase_RootComTask));
	if(RCTask)
		Ov_Link(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));

	/***********************************************************************************************************************************************************************************
	 *	Handle incoming data
	 **********************************************************************************************************************************************************************************/

	intsocket = UDPbind_UDPChannel_socket_get(thisCh);
	OV_UDPBIND_SETINT2SOCKET(intsocket, socket);

	if (socket != UDPBIND_INVALID_SOCKET && thisCh->v_ConnectionState == UDPbind_CONNSTATE_OPEN)		//if socket ok
	{
		//receive data in chunks (we dont know how much it will be)
		do
		{
			FD_ZERO(&read_flags);
			FD_SET(socket, &read_flags); // get read flags
			waitd.tv_sec = 0;     // Set Timeout
			waitd.tv_usec = 0;    //  do not wait
			err = select(socket + 1, &read_flags, (fd_set*) 0, (fd_set*)0, &waitd);
			if(err == UDPBIND_SOCKET_ERROR){
				KS_logfile_debug(("%s typemethod: select returned: %d; line %d", thisCh->v_identifier, err, __LINE__));
			}

			//check if data arrived
			if((err > 0) && FD_ISSET(socket, &read_flags)) {
				//Data arrived
				//reallocate memory for receiving data. Note the temp-pointer: if realloc fails, the original pointer is NOT freed
				tempdata = ov_realloc(thisCh->v_inData.data, thisCh->v_inData.length + UDPbind_CHUNKSIZE);
				if(!tempdata) {
					KS_logfile_error(("%s: failed to allocate memory for received data (length: %u)", this->v_identifier, thisCh->v_inData.length+UDPbind_CHUNKSIZE));
					ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
					Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
					return;
				}

				//if this is the first chunk of data in the packet, initialize the read pointer
				if(!thisCh->v_inData.readPT) {
					thisCh->v_inData.readPT = tempdata;
				} else {
					/*	get the readPT index and set readPT to this index in the newly allocated memory	*/
					thisCh->v_inData.readPT = tempdata + (thisCh->v_inData.readPT - thisCh->v_inData.data);
				}
				thisCh->v_inData.data = tempdata;
				/*	tempdata has done its job now. not used from here on	*/

				if(!thisCh->v_inData.writePT) {
					thisCh->v_inData.writePT = thisCh->v_inData.data;
				} else {
					thisCh->v_inData.writePT = thisCh->v_inData.data + thisCh->v_inData.length;
				}

				err = recvfrom(socket, (char*) thisCh->v_inData.writePT, UDPbind_CHUNKSIZE, 0, (struct sockaddr*) &thisCh->v_remoteAddress, &thisCh->v_remoteAddrLen);		//receive data
				if (err == UDPBIND_SOCKET_ERROR){
					KS_logfile_debug(("%s: error receiving. Closing socket.", this->v_identifier));
					UDPBIND_CLOSE_SOCKET(socket);
					UDPbind_UDPChannel_socket_set(thisCh, -1);
					thisCh->v_ConnectionState = UDPbind_CONNSTATE_CLOSED;
					if(!thisCh->v_inData.length)	/*	nothing was received --> free memory	*/
					{
						ksbase_free_KSDATAPACKET(&thisCh->v_inData);
					}
					/*	if we need a client handler and our inData buffer is empty --> delete channel (prevents lots of dead serverside channels in the database)	*/
					if(thisCh->v_ClientHandlerAssociated != KSBASE_CH_NOTNEEDED
							&& !thisCh->v_inData.length) {
						KS_logfile_debug(("%s: we are on the server side and have no data --> deleting channel", this->v_identifier));
						Ov_DeleteObject(thisCh);
					} else {
						KS_logfile_debug(("%s: Setting ConnectionTimeOut to %u.", this->v_identifier, thisCh->v_UnusedDataTimeOut));
						thisCh->v_ConnectionTimeOut = thisCh->v_UnusedDataTimeOut;
						Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
					}

					return;
				}

				//update data length
				thisCh->v_inData.length += err;
				//move writept to end of data
				thisCh->v_inData.writePT += err;

				datareceived = TRUE;
			}

		}while((err == UDPbind_CHUNKSIZE) && FD_ISSET(socket, &read_flags));

		if(datareceived) {

			/**********************************************************************************************************************************************************
			 *	Associate ClientHandler if needed
			 *********************************************************************************************************************************************************/
			if(thisCh->v_ClientHandlerAssociated == KSBASE_CH_NOTASSOCATIED) {
				UDPbind_UDPChannel_AssociateClientHandler(Ov_StaticPtrCast(ksbase_Channel, this));
			}

			/*****************************************************************************************************************************************************************************
			 *	Process received data
			 ****************************************************************************************************************************************************************************/

			//update receivetime
			ov_time_gettime(&(thisCh->v_LastReceiveTime));

			if(thisCh->v_ClientHandlerAssociated == KSBASE_CH_ASSOCIATED)
			{	//there is a ClientHandler associated. Call its HandleRequest function.
				pClientHandler = Ov_GetChild(ksbase_AssocChannelClientHandler, thisCh);
				if(pClientHandler) {
					Ov_GetVTablePtr(ksbase_ClientHandler, pVTBLClientHandler, pClientHandler);
					if(pVTBLClientHandler) {
						KS_logfile_debug(("%s: handing over data to %s to handle it.", thisCh->v_identifier, pClientHandler->v_identifier));
						result = pVTBLClientHandler->m_HandleRequest(pClientHandler, Ov_StaticPtrCast(ksbase_Channel, thisCh), &(thisCh->v_inData), &(thisCh->v_outData));
						if(Ov_Fail(result)) {
							KS_logfile_error(("%s: processing of received data by %s failed: %s", thisCh->v_identifier, pClientHandler->v_identifier, ov_result_getresulttext(result)));
							ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
							ksbase_free_KSDATAPACKET(&(thisCh->v_outData));
							Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
							return;
						} else {
							UDPbind_UDPChannel_SendData(Ov_StaticPtrCast(ksbase_Channel, thisCh));
							Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
							if((thisCh->v_CloseAfterSend == TRUE) && ((thisCh->v_outData.readPT - thisCh->v_outData.data) >= (OV_INT) thisCh->v_outData.length))
							{/*	channel should close after send and everything is sent	*/
								Ov_DeleteObject(thisCh);
							}
							return;
						}
					} else {
						KS_logfile_error(("%s: no Vtable found for ClientHandler %s. Deleting Channel.", thisCh->v_identifier, pClientHandler->v_identifier));
						Ov_DeleteObject(thisCh);
						return;
					}
				} else {
					KS_logfile_error(("%s: no ClientHandler associated. Deleting Channel.", thisCh->v_identifier));
					Ov_DeleteObject(thisCh);
					return;
				}
			} else if(thisCh->v_ClientHandlerAssociated == KSBASE_CH_NOTNEEDED)	//if acting on the client side, check if a "callback" has to be done
			{
				pDataHandler = Ov_GetChild(ksbase_AssocChannelDataHandler, thisCh);
				if(pDataHandler) {	//there is a DataHandler --> call its HandleData Function
					Ov_GetVTablePtr(ksbase_DataHandler, pVTBLDataHandler, pDataHandler);
					if(pVTBLDataHandler) {
						KS_logfile_debug(("%s: handing over data to %s to handle it.", thisCh->v_identifier, pDataHandler->v_identifier));
						result = pVTBLDataHandler->m_HandleData(pDataHandler, Ov_StaticPtrCast(ksbase_Channel, thisCh), &(thisCh->v_inData), &(thisCh->v_outData));
						if(Ov_Fail(result)) {
							KS_logfile_error(("%s: processing of received data by %s failed: %s", thisCh->v_identifier, pDataHandler->v_identifier, ov_result_getresulttext(result)));
							ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
							ksbase_free_KSDATAPACKET(&(thisCh->v_outData));
							Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
							return;
						} else {
							UDPbind_UDPChannel_SendData(Ov_StaticPtrCast(ksbase_Channel, thisCh));
							Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
							if((thisCh->v_CloseAfterSend == TRUE) && ((thisCh->v_outData.readPT - thisCh->v_outData.data) >= (OV_INT) thisCh->v_outData.length))
							{/*	channel should close after send and everything is sent	*/
								Ov_DeleteObject(thisCh);
							}
							return;
						}
					} else {
						KS_logfile_error(("%s: no Vtable found for DataHandler %s. Deleting Channel.", thisCh->v_identifier, pDataHandler->v_identifier));
						Ov_DeleteObject(thisCh);
						return;
					}
				}
			}
		}
		/*****************************************************************************************************************************************************************************
		 * Send data if there is still something in the buffer
		 ****************************************************************************************************************************************************************************/
		if(thisCh->v_outData.length) {
			UDPbind_UDPChannel_SendData(Ov_StaticPtrCast(ksbase_Channel, thisCh));
			if((thisCh->v_CloseAfterSend == TRUE) && ((thisCh->v_outData.readPT - thisCh->v_outData.data) >= (OV_INT) thisCh->v_outData.length))
			{/*	channel should close after send and everything is sent	*/
				Ov_DeleteObject(thisCh);
				return;
			}
		}
	} else {/*	no socket open	*/
		/*	if we need a client handler and our inData buffer is empty --> delete channel (prevents lots of dead serverside channels in the database)	*/
		if(thisCh->v_ClientHandlerAssociated != KSBASE_CH_NOTNEEDED
				&& !thisCh->v_inData.length) {
			KS_logfile_debug(("%s: we are on the server side and have no data --> deleting channel", this->v_identifier));
			Ov_DeleteObject(thisCh);
			return;
		} else {
#if LOG_KS || LOG_KS_DEBUG
			if(thisCh->v_ConnectionTimeOut != thisCh->v_UnusedDataTimeOut){
				//too much noise every millisecond if no one feels responsive for this data
				KS_logfile_debug(("%s: Setting ConnectionTimeOut to %u.", this->v_identifier, thisCh->v_UnusedDataTimeOut));
			}
#endif
			thisCh->v_ConnectionTimeOut = thisCh->v_UnusedDataTimeOut;
			Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
		}
	}
	/*******************************************************************************************************************************************************
	 *	check timeouts
	 ******************************************************************************************************************************************************/
	ov_time_gettime(&now);

	//TimeOut of connection
	tstemp.secs = thisCh->v_ConnectionTimeOut;
	tstemp.usecs = 0;
	ov_time_add(&ttemp, &(thisCh->v_LastReceiveTime), &tstemp);
	if(ov_time_compare(&now, &ttemp) == OV_TIMECMP_AFTER) {
		if(thisCh->v_ClientHandlerAssociated == KSBASE_CH_NOTNEEDED) {
			if(thisCh->v_ConnectionState == UDPbind_CONNSTATE_OPEN) {
				KS_logfile_info(("%s: received nothing for %u seconds. Closing connection.", this->v_identifier, thisCh->v_ConnectionTimeOut));
				UDPBIND_CLOSE_SOCKET(socket);
				UDPbind_UDPChannel_socket_set(thisCh, -1);
				thisCh->v_ConnectionState = UDPbind_CONNSTATE_CLOSED;
			}
		} else {
			KS_logfile_info(("%s: received nothing for %u seconds. Deleting UDPChannel", this->v_identifier, thisCh->v_ConnectionTimeOut));
			Ov_DeleteObject(thisCh);
			return;
		}
	}

	//Timeout of Data in inData (in Buffer)
	tstemp.secs = thisCh->v_UnusedDataTimeOut;
	tstemp.usecs = 0;
	ov_time_add(&ttemp, &(thisCh->v_LastReceiveTime), &tstemp);
	if((ov_time_compare(&now, &ttemp) == OV_TIMECMP_AFTER) && thisCh->v_inData.length) {
		KS_logfile_info(("%s: received nothing for %u seconds. Deleting inData.", this->v_identifier, thisCh->v_UnusedDataTimeOut));
		ksbase_free_KSDATAPACKET(&(thisCh->v_inData));
	}

	Ov_Unlink(ksbase_AssocCurrentChannel, RCTask, Ov_StaticPtrCast(ksbase_Channel, thisCh));
	return;
}

OV_DLLFNCEXPORT OV_RESULT UDPbind_UDPChannel_OpenLocalConn(
	OV_INSTPTR_ksbase_Channel this,
	OV_STRING port
) {
	return UDPbind_UDPChannel_OpenConnection(this, "localhost", port);
}

OV_DLLFNCEXPORT OV_RESULT UDPbind_UDPChannel_OpenConnection(
	OV_INSTPTR_ksbase_Channel this,
	OV_STRING host,
	OV_STRING port
) {
	struct addrinfo hints;
	struct addrinfo *res;
	int ret;
	UDPBIND_SOCKET sockfd = UDPBIND_INVALID_SOCKET;
	OV_INSTPTR_UDPbind_UDPChannel thisUDPCh = Ov_StaticPtrCast(UDPbind_UDPChannel, this);

	if(!host || !(*host) || !port || !(*port))
		return OV_ERR_BADPARAM;

	/*	close old connection	*/
	UDPbind_UDPChannel_CloseConnection(this);

	//set connection information
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	//resolve address
	if ((ret = getaddrinfo(host, port, &hints, &res)) != 0) {
		KS_logfile_error(("%s: getaddrinfo failed", this->v_identifier));
		thisUDPCh->v_ConnectionState = UDPbind_CONNSTATE_COULDNOTOPEN;
		return OV_ERR_GENERIC;
	}

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd == UDPBIND_INVALID_SOCKET){
		KS_logfile_debug(("%s:could not open socket", this->v_identifier));
		//free structures
		freeaddrinfo(res);
		thisUDPCh->v_ConnectionState = UDPbind_CONNSTATE_COULDNOTOPEN;
		return OV_ERR_GENERIC;
	}
	if(res->ai_addrlen <= sizeof(OV_SOCKADDR)){
		memcpy(&thisUDPCh->v_remoteAddress, res->ai_addr, res->ai_addrlen);
		thisUDPCh->v_remoteAddrLen = res->ai_addrlen;
	}
	//free structures
	freeaddrinfo(res);

	thisUDPCh->v_ConnectionState = UDPbind_CONNSTATE_OPEN;
	UDPbind_UDPChannel_socket_set(thisUDPCh, sockfd);

	KS_logfile_debug(("%s: connected.", thisUDPCh->v_identifier));
	//set time of Opening the Connection
	ov_time_gettime(&(this->v_LastReceiveTime));

	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void UDPbind_UDPChannel_CloseConnection(
	OV_INSTPTR_ksbase_Channel this
) {
	OV_INSTPTR_UDPbind_UDPChannel thisUDPCh = Ov_StaticPtrCast(UDPbind_UDPChannel, this);
	if(thisUDPCh->v_socket >= 0)
	{
		UDPbind_UDPChannel_socket_set(thisUDPCh, -1);
		KS_logfile_debug(("UDPChannel %s socket %d closed", thisUDPCh->v_identifier, thisUDPCh->v_socket));
	}
    return;
}

OV_DLLFNCEXPORT OV_RESULT UDPbind_UDPChannel_AssociateClientHandler(
	OV_INSTPTR_ksbase_Channel this
) {

    return UDPbind_UDPListener_AssociateClientHandler(this);
}
