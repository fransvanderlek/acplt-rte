
/******************************************************************************
*
*   FILE
*   ----
*   subscriptionSettings.c
*
*   History
*   -------
*   2018-02-07   File created
*
*******************************************************************************
*
*   This file is generated by the 'acplt_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_opcuaSubscription
#define OV_COMPILE_LIBRARY_opcuaSubscription
#endif


#include "opcuaSubscription.h"
#include "libov/ov_macros.h"


OV_DLLFNCEXPORT OV_ACCESS opcuaSubscription_subscriptionSettings_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
    /*    
    *   local variables
    */
    //OV_INSTPTR_opcuaSubscription_subscriptionSettings pinst = Ov_StaticPtrCast(opcuaSubscription_subscriptionSettings, pobj);

	return (OV_ACCESS)OV_AC_WRITE | OV_AC_READ | OV_AC_LINKABLE | OV_AC_UNLINKABLE | OV_AC_DELETEABLE | OV_AC_RENAMEABLE;
}
