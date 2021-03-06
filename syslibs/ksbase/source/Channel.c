
/******************************************************************************
*
*   FILE
*   ----
*   Channel.c
*
*   History
*   -------
*   2013-01-15   File created
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
#include "ov_malloc.h"
#include "ks_logfile.h"
#include "ksbase_helper.h"



OV_DLLFNCEXPORT void ksbase_Channel_startup(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
	OV_INSTPTR_ksbase_Channel this = Ov_StaticPtrCast(ksbase_Channel, pobj);
    /* do what the base class does first */
    ov_object_startup(pobj);

    /* do what */
    //run cyclic and fast
    this->v_actimode = 1;
    this->v_cycInterval = 0;

    //initialize datastructures
    this->v_inData.length = 0;
    this->v_inData.data = NULL;
    this->v_inData.readPT = NULL;
    this->v_inData.writePT = NULL;
    this->v_outData.length = 0;
    this->v_outData.data = NULL;
    this->v_outData.readPT = NULL;
    this->v_outData.writePT = NULL;

    return;
}

OV_DLLFNCEXPORT void ksbase_Channel_shutdown(
	OV_INSTPTR_ov_object 	pobj
) {
    /*
    *   local variables
    */
	OV_INSTPTR_ksbase_Channel this = Ov_StaticPtrCast(ksbase_Channel, pobj);
    /* do what */
	//free heap memory
	if(pobj->v_objectstate & OV_OS_STARTED)
	{
		if(this->v_inData.length)
		{
			KS_logfile_debug(("Channel shutdown %s: freeing inData", pobj->v_identifier));
			ksbase_free_KSDATAPACKET(&(this->v_inData));
		}
		if(this->v_outData.length)
		{
			KS_logfile_debug(("Channel shutdown %s: freeing outData", pobj->v_identifier));
			ksbase_free_KSDATAPACKET(&(this->v_outData));
		}
		/* set the object's state to "shut down" */
		ov_object_shutdown(pobj);
	}
    return;
}
