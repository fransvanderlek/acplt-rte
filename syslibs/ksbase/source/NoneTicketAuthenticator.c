
/******************************************************************************
*
*   FILE
*   ----
*   NoneTicketAuthenticator.c
*
*   History
*   -------
*   2013-02-25   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ksbase
#define OV_COMPILE_LIBRARY_ksbase
#endif


#include "ksbase.h"
#include "ov_macros.h"

/*
 * global pointer to the NoneTicketAuthenticator (this implies no problem since the authenticator is a sigleton)
 * it is needed for the ticket functions to find necessary data
 */
OV_DLLVAREXPORT OV_INSTPTR_ksbase_NoneTicketAuthenticator ksbase_NoneAuth = NULL;

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_constructor(
		OV_INSTPTR_ov_object 	pobj
) {
	OV_RESULT result;

	result = ksbase_TicketAuthenticator_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	((OV_INSTPTR_ksbase_NoneTicketAuthenticator) pobj)->v_TicketAccess = (OV_AC_READWRITE
																			| OV_AC_DELETEABLE
																			| OV_AC_INSTANTIABLE
																			| OV_AC_LINKABLE
																			| OV_AC_RENAMEABLE
																			| OV_AC_UNLINKABLE);
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT void ksbase_NoneTicketAuthenticator_startup(
		OV_INSTPTR_ov_object 	pobj
) {

	OV_INSTPTR_ksbase_NoneTicketAuthenticator this = Ov_StaticPtrCast(ksbase_NoneTicketAuthenticator, pobj);

	/*	call baseclass' startup	*/
	ksbase_TicketAuthenticator_startup(pobj);

	this->v_TicketType = OV_TT_NONE;
	/*
	 * ksbase_TicketAuthenticator_constructor verifies that no other NoneTicketAuthenticator exists
	 * Hence, the global variable means no problems.
	 */
	ksbase_NoneAuth = this;


	return;
}

OV_DLLFNCEXPORT OV_TICKET* ksbase_NoneTicketAuthenticator_createticket(
	void *data,
	OV_TICKET_TYPE type
) {
    return &ksbase_NoneAuth->v_ticket;
}

OV_DLLFNCEXPORT void ksbase_NoneTicketAuthenticator_deleteticket(
    OV_TICKET* pticket
) {
    return;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_encodereply(
	void *data,
	OV_TICKET *pticket
) {
    return TRUE;
}

OV_DLLFNCEXPORT OV_RESULT ksbase_NoneTicketAuthenticator_TicketGetaccess(
	const OV_TICKET *pticket
) {
    return ksbase_NoneAuth->v_TicketAccess;
}
