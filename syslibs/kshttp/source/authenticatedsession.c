
/******************************************************************************
*
*   FILE
*   ----
*   authenticatedsession.c
*
*   History
*   -------
*   2012-04-02   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_kshttp
#define OV_COMPILE_LIBRARY_kshttp
#endif


#include "kshttp.h"
#include "ov_macros.h"


OV_DLLFNCEXPORT OV_TIME* kshttp_authenticatedsession_lastactivity_get(
    OV_INSTPTR_kshttp_authenticatedsession          pobj
) {
    return &pobj->v_lastactivity;
}

OV_DLLFNCEXPORT OV_RESULT kshttp_authenticatedsession_lastactivity_set(
    OV_INSTPTR_kshttp_authenticatedsession          pobj,
    const OV_TIME*  value
) {
    pobj->v_lastactivity = *value;
    return OV_ERR_OK;
}