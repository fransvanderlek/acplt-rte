/******************************************************************************
***                                                                         ***
***   iFBSpro   -   Funktionsbaustein-Model                                 ***
***   #####################################                                 ***
***                                                                         ***
***   L T S o f t                                                           ***
***   Agentur für Leittechnik Software GmbH                                 ***
***   Brabanterstr. 13                                                      ***
***   D-50171 Kerpen                                                        ***
***   Tel : 02237/92869-2                                                   ***
***   Fax : 02237/92869-9                                                   ***
***   e-Mail   : ltsoft@ltsoft.de                                           ***
***   Internet : http://www.ltsoft.de                                       ***
***                                                                         ***
***   -------------------------------------------------------------------   ***
***                                                                         ***
***   Implementierung des Funktionsbausteinsystems IFBSpro                  ***
***   RWTH,   Aachen                                                        ***
***   LTSoft, Kerpen                                                        ***
***                                                                         ***
*******************************************************************************
*                                                                             *
*   Datei                                                                     *
*   -----                                                                     *
*   fb_database.c                                                             *
*                                                                             *
*                                                                             *
*   Historie                                                                  *
*   --------                                                                  *
*   08-07-1999 Alexander Neugebauer: Erstellung, LTSoft GmbH, Kerpen          *
*                                    Lehrstuhl fuer Prozessleittechnik, Aachen*
*   2001-04-27 Anpassung an MC164    A.Neugebauer                             *
*   2001-06-12 Major revision.       A.Neugebauer                             *
*                                                                             *
*   Beschreibung                                                              *
*   ------------                                                              *
*   Funktionen der database utility                                           *
*                                                                             *
******************************************************************************/

#ifndef OV_COMPILE_LIBRARY_fb
#define OV_COMPILE_LIBRARY_fb
#endif


#include "fb.h"
#include "fb_namedef.h"
#include "fb_database.h"
#include "ov_database.h"
#include "ov_macros.h"
#include "ov_logfile.h"
#include "ov_association.h"
#include "ov_result.h"

#include <stdio.h>
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif
 OV_RESULT fb_database_wrapload(OV_STRING filename, OV_UINT size, OV_UINT flags);
 void      fb_database_wrapunload(void);
 OV_RESULT fb_database_wrapstartup(void);
#ifdef __cplusplus
}
#endif


#if OV_SYSTEM_MC164
void                                     /* Returns: nothing                  */
   forceReactionToDatabaseFlush( void     /* no parameter                     */
);
#endif

/*	----------------------------------------------------------------------	*/
/*
*   Global variables
*/
static OV_INSTPTR_fb_task           pUrTask = NULL;
static OV_INSTPTR_ov_domain         pLibCon = NULL;
static OV_INSTPTR_fb_dbinfoclass    pDbInfo = NULL;

/*	----------------------------------------------------------------------	*/
/*
*	Test, if a string is a valid identifier
*/
OV_DLLFNCEXPORT OV_BOOL fb_database_identifierok(
	char*		identifier
) {
	/*
	*	local variables
	*/
	char*   pident;
	
	/*
	*	check parameters
	*/
	if(!identifier) {
		return FALSE;
	}
	if(!*identifier) {
		return FALSE;
	}

    pident = identifier;
    while(pident && (*pident)) {
        if( ((*pident >= 'a') && (*pident <= 'z')) ||
            ((*pident >= 'A') && (*pident <= 'Z')) ||
            ((*pident >= '0') && (*pident <= '9')) ||
            (*pident == '_') ) {
            /* OK */
        } else {
            return FALSE;
        }
        pident++;
    }
    
    return TRUE;
}

/*
*	Create a new database
*/
OV_DLLFNCEXPORT OV_RESULT fb_database_create(
	OV_STRING	filename,
	OV_UINT		size,
	OV_UINT		flags
) {
	/*
	*	local variables
	*/
	OV_RESULT					result;

#if 0

#if !OV_SYSTEM_MC164
	char						help[512];
	char						hilf[512];
	char                       *pc;

	/*
	*	create FB database
	*/
	strcpy(help, "");   /* Hilfsstring initialisieren */
	
#if OV_SYSTEM_LINUX || OV_SYSTEM_NT || OV_SYSTEM_SOLARIS
	
	/* File-Name fuer weitere bearbeitung kopieren */
	strcpy(hilf, filename);

	pc = strstr(hilf, "database");
	
	if(pc) {
	    *pc = '\0'; /* String-Ende markieren */
		sprintf(help, "%sbin", hilf );
	}

	
	ov_library_setenv(help);

#endif  /* OV_SYSTEM_LINUX || OV_SYSTEM_NT */
#endif  /* !OV_SYSTEM_MC164 */

#endif  /* if 0 */
	
	result = ov_database_create(filename, size, flags);
	if(Ov_Fail(result)) {
		return result;
	}

	/*
	*	create fb structure
	*/
	result = fb_database_checkstruct();
	if(Ov_Fail(result)) {
	    ov_database_unload();
		return result;
	}
	/*
	*	we are done 
	*/
	return OV_ERR_OK;
}

/*	----------------------------------------------------------------------	*/

/*
*	Map an existing database
*/
OV_DLLFNCEXPORT OV_RESULT fb_database_load(OV_STRING	filename, OV_UINT size, OV_UINT flags) {
    return fb_database_wrapload(filename, size, flags);
}

/*	----------------------------------------------------------------------	*/

/*
*	Unmap the database
*/
OV_DLLFNCEXPORT void fb_database_unload(void) {
	fb_database_wrapunload();
}

/*	----------------------------------------------------------------------	*/

/*
*	Flush the contents of a database
*/
OV_DLLFNCEXPORT void fb_database_flush(void) {
#if OV_SYSTEM_MC164
        forceReactionToDatabaseFlush();
#else
	ov_database_flush();
#endif
}

/*	----------------------------------------------------------------------	*/

/*
*	Start up the database
*/
OV_DLLFNCEXPORT OV_RESULT fb_database_startup(void) {
    return fb_database_wrapstartup();
}

/*	----------------------------------------------------------------------	*/

/*
*	Shut down the database
*/
OV_DLLFNCEXPORT void fb_database_shutdown(void) {
	ov_database_shutdown();
}

/*	----------------------------------------------------------------------	*/

/*
*	Get UrTask object
*/
OV_DLLFNCEXPORT void* fb_database_geturtask(void) {
    
    /*
    *  Local variables
    */    
    OV_INSTPTR_ov_domain	    ptaskcontainer;
    
    /* UrTask found? */
    if(pUrTask) {
        return (void*)pUrTask;
    }
    
    /* Search it */
	ptaskcontainer = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, &pdb->root, FB_TASK_CONTAINER);
	if(!ptaskcontainer) {
        /* Tasks container not found */
        return NULL;
    }
    
	pUrTask = (OV_INSTPTR_fb_task)Ov_SearchChild(ov_containment, ptaskcontainer, FB_URTASK);
	return (void*)pUrTask;
}

/*
 * check if task is connected to UrTask or an initial task
 */
OV_DLLFNCEXPORT OV_BOOL fb_task_has_initial_taskparent(
	OV_INSTPTR_fb_task ptask){
	OV_INSTPTR_fb_task urtask = (OV_INSTPTR_fb_task)fb_database_geturtask();
	OV_INSTPTR_fb_task pcurTask = Ov_GetParent(fb_tasklist, ptask);

	OV_UINT count = 0;
	OV_UINT max_count = ov_vendortree_MaxHierarchyDepth() * 2;

	for(pcurTask=ptask; pcurTask; pcurTask=Ov_GetParent(fb_tasklist, pcurTask)){
		if(pcurTask==urtask)
			return TRUE;
		if(count > max_count){
			ov_logfile_warning("%s: max search depth reached; assuming not connected to urtask", ptask->v_identifier);
			return FALSE;
		}
		count++;
	}

	return TRUE;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get libraries container object
*/
OV_DLLFNCEXPORT void* fb_database_getlibcontainer(void) {

    /* Container found? */
    if(pLibCon) {
        return (void*)pLibCon;
    }
         
    /* Search it */
	pLibCon = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, &pdb->root, FB_LIBRARIES_CONTAINER);
	return (void*)pLibCon;
}

/*	----------------------------------------------------------------------	*/

/*
*	Get libraries container object
*/
OV_DLLFNCEXPORT void* fb_database_getdbinfo(void) {

    /* Object found? */
    if(pDbInfo) {
        return (void*)pDbInfo;
    }
         
    /* Search it */
	pDbInfo = (OV_INSTPTR_fb_dbinfoclass)Ov_SearchChild(ov_containment, &pdb->root, FB_DBINFO);
	return (void*)pDbInfo;
}
void fb_database_nildbinfo(void) {
    pDbInfo = NULL;
}

/*	----------------------------------------------------------------------	*/

/*
*	Check database structure
*/
OV_DLLFNCEXPORT OV_RESULT fb_database_checkstruct(void) {

    /*
    *  Local variables
    *  ---------------
    */        
#if OV_SYSTEM_LINUX
    OV_INSTPTR_ov_object        pobj;
#endif
    OV_INSTPTR_ov_domain        pdom;
    OV_INSTPTR_ov_library       plib;
    OV_RESULT                   result;

    /*
    *   Library fb
    */
    plib = ov_library_search("fb");
    if(!plib) {
#if OV_SYSTEM_LINUX
        result = Ov_CreateObject(ov_library, pobj, &pdb->root, "fb");
#else
        result = Ov_CreateObject(ov_library, plib, &pdb->root, "fb");
#endif
       	if(Ov_Fail(result)) {
	        ov_logfile_error("Can't load library 'fb'. Error: %s",
                    ov_result_getresulttext(result));
   	    	return result;
        }
#if OV_SYSTEM_LINUX
        plib = (OV_INSTPTR_ov_library)pobj;
#endif
    }

	/*
	*	DB-INFO object
	*/
	pDbInfo = (OV_INSTPTR_fb_dbinfoclass)Ov_SearchChild(ov_containment, &pdb->root, FB_DBINFO);
	if(!pDbInfo) {
#if OV_SYSTEM_LINUX
	    result = Ov_CreateObject(fb_dbinfoclass, pobj, &pdb->root, FB_DBINFO);
#else
	    result = Ov_CreateObject(fb_dbinfoclass, pDbInfo, &pdb->root, FB_DBINFO);
#endif
       	if(Ov_Fail(result)) {
	        ov_logfile_error("Can't create instance '%s'. Error: %s",
                        FB_DBINFO, ov_result_getresulttext(result) );
   	    	return result;
        }
#if OV_SYSTEM_LINUX
        pDbInfo = (OV_INSTPTR_fb_dbinfoclass)pobj;
#endif
	}
	/*
	*	Tasks container
	*/
	pdom = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, &pdb->root, FB_TASK_CONTAINER);
	if(!pdom) {
#if OV_SYSTEM_LINUX
    	result = Ov_CreateObject(ov_domain, pobj, &pdb->root, FB_TASK_CONTAINER);
#else
    	result = Ov_CreateObject(ov_domain, pdom, &pdb->root, FB_TASK_CONTAINER);
#endif
       	if(Ov_Fail(result)) {
	        ov_logfile_error("Can't create %s-Container. Error: %s",
                        FB_TASK_CONTAINER, ov_result_getresulttext(result) );
   	    	return result;
        }
#if OV_SYSTEM_LINUX
        pdom = (OV_INSTPTR_ov_domain)pobj;
#endif
	}
	/*
	*	Urtask-object
	*/
	pUrTask = (OV_INSTPTR_fb_task)Ov_SearchChild(ov_containment, pdom, FB_URTASK);
	if(!pUrTask) {
#if OV_SYSTEM_LINUX
    	result = Ov_CreateObject(fb_task, pobj, pdom, FB_URTASK);
#else
    	result = Ov_CreateObject(fb_task, pUrTask, pdom, FB_URTASK);
#endif
       	if(Ov_Fail(result)) {
	        ov_logfile_error("Can't create %s. Error: %s",
                        FB_URTASK, ov_result_getresulttext(result) );
   	    	return result;
        }
#if OV_SYSTEM_LINUX
        pUrTask = (OV_INSTPTR_fb_task)pobj;
#endif
        
#if OV_SYSTEM_MC164
        pUrTask->v_cyctime.secs = 0;
        pUrTask->v_cyctime.usecs = 50000ul;
#else
    	pUrTask->v_cyctime.secs = 1;
        pUrTask->v_cyctime.usecs = 0;
#endif
        pUrTask->v_actimode = FB_AM_ON;

	}
	/*
	*	TechUnits-container
	*/
	pdom = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, &pdb->root, FB_INSTANZ_CONTAINER);
	if(!pdom) {
#if OV_SYSTEM_LINUX
	    result = Ov_CreateObject(ov_domain, pobj, &pdb->root, FB_INSTANZ_CONTAINER);
#else
	    result = Ov_CreateObject(ov_domain, pdom, &pdb->root, FB_INSTANZ_CONTAINER);
#endif
       	if(Ov_Fail(result)) {
	        ov_logfile_error("Can't create %s-Container. Error: %s",
                        FB_INSTANZ_CONTAINER, ov_result_getresulttext(result) );
   	    	return result;
        }
	}
	/*
	*	ComCon container
	*/
	pdom = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, &pdb->root, FB_CONN_CONTAINER);
	if(!pdom) {
#if OV_SYSTEM_LINUX
	    result = Ov_CreateObject(ov_domain, pobj, &pdb->root, FB_CONN_CONTAINER);
#else
	    result = Ov_CreateObject(ov_domain, pdom, &pdb->root, FB_CONN_CONTAINER);
#endif
       	if(Ov_Fail(result)) {
	        ov_logfile_error("Can't create Connection-Container. Error: %s",
                        ov_result_getresulttext(result) );
   	    	return result;
        }
	}
	/*
	*	UserLib container
	*/
	pLibCon = (OV_INSTPTR_ov_domain)Ov_SearchChild(ov_containment, &pdb->root, FB_LIBRARIES_CONTAINER);
	if(!pLibCon) {
#if OV_SYSTEM_LINUX
	    result = Ov_CreateObject(ov_domain, pobj, &pdb->root, FB_LIBRARIES_CONTAINER);
#else
	    result = Ov_CreateObject(ov_domain, pLibCon, &pdb->root, FB_LIBRARIES_CONTAINER);
#endif
       	if(Ov_Fail(result)) {
	        ov_logfile_error("Can't create %s-Container. Error: %s",
                        FB_LIBRARIES_CONTAINER, ov_result_getresulttext(result) );
   	    	return result;
        }
	}
    return OV_ERR_OK;
}


/*	----------------------------------------------------------------------	*/

/*
*	End of file
*/
