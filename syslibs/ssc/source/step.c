
/******************************************************************************
*
*   FILE
*   ----
*   sscStep.c
*
*   History
*   -------
*   2011-05-27   File created
*
*******************************************************************************
*
*   This file is generated by the 'fb_builder' command
*
******************************************************************************/


#ifndef OV_COMPILE_LIBRARY_ssc
#define OV_COMPILE_LIBRARY_ssc
#endif

#ifdef ssc_ERROR
#define ssc_STEP_ERROR
#endif


#include "ssclib.h"
#include "fb_database.h"


OV_DLLFNCEXPORT OV_RESULT ssc_step_constructor(
		OV_INSTPTR_ov_object 	pobj
) {
	/*
	 *   local variables
	 */
	OV_INSTPTR_ssc_step pinst = Ov_StaticPtrCast(ssc_step, pobj);
	OV_INSTPTR_fb_task    pEntry = Ov_GetPartPtr(entry, pinst);
	OV_INSTPTR_fb_task    pDo 	 = Ov_GetPartPtr(do, pinst);
	OV_INSTPTR_fb_task    pExit  = Ov_GetPartPtr(exit, pinst);

	OV_INSTPTR_ssc_SequentialStateChart pSSC = Ov_DynamicPtrCast(ssc_SequentialStateChart, Ov_GetParent(ov_containment, pinst));
	OV_RESULT    result;

	/* do what the base class does first */
	result = fb_functionblock_constructor(pobj);
	if(Ov_Fail(result))
		return result;

	// check location
	if (pSSC==NULL){
		ov_logfile_error("ssc_step_constructor: step must be encapsulated in a SequentialStateChart.");
		return OV_ERR_BADPLACEMENT;
	}

	// local tasklist
	pEntry->v_actimode = FB_AM_ON;
	pDo->v_actimode = FB_AM_ON;
	pExit->v_actimode = FB_AM_OFF;

	// link entry, do and exit task
	result = Ov_Link(fb_tasklist, pinst, pEntry);
	if(Ov_Fail(result))
		return result;
	result = Ov_Link(fb_tasklist, pinst, pDo);
	if(Ov_Fail(result))
		return result;
	result = Ov_Link(fb_tasklist, pinst, pExit);
	if(Ov_Fail(result))
		return result;

	//activate
	pinst->v_iexreq=TRUE;

	return OV_ERR_OK;
}


OV_DLLFNCEXPORT void ssc_step_typemethod(
	OV_INSTPTR_fb_functionblock	pfb,
	OV_TIME						*pltc
) {
    /*    
    *   local variables
    */
    OV_INSTPTR_ssc_step pinst = Ov_StaticPtrCast(ssc_step, pfb);
    OV_INSTPTR_fb_task    pEntry = Ov_GetPartPtr(entry, pinst);
    OV_INSTPTR_fb_task    pDo 	 = Ov_GetPartPtr(do, pinst);
    OV_INSTPTR_fb_task    pExit  = Ov_GetPartPtr(exit, pinst);
    OV_INSTPTR_ssc_SequentialStateChart pSSC = Ov_DynamicPtrCast(ssc_SequentialStateChart, Ov_GetParent(ov_containment, pinst));
    OV_INSTPTR_ssc_step pNextStep = NULL;
    OV_INSTPTR_ssc_SequentialStateChart pSubSsc=NULL;
    OV_INSTPTR_ssc_execute pExecute = NULL;
    OV_INSTPTR_fb_functionblock pTargetObj = NULL;
	OV_INSTPTR_ssc_transition pTransition = NULL;

    OV_TIME elapsedTime;
    // helper variables
    OV_BOOL	  exitLoop=FALSE;
    OV_RESULT result = OV_ERR_OK;

    // check location
    if (pSSC==NULL)
    {
      	ov_logfile_error("ssc_step_constructor: step must be encapsulated in a SequentialStateChart.");
      	return;
    }

    // TODO:
    // check if SequentialStateChart is the taskparent.

    // init variables
    pinst->v_cyctime.secs = 0;
    pinst->v_cyctime.usecs = 0;
    pinst->v_iexreq = TRUE;
    pinst->v_evTransTrigger=FALSE;


    // execute subtasks
    do{
    	exitLoop=TRUE;

    	switch (pinst->v_phase)
    	{
    	/* phase 1: entry, do */
    	case SSC_PHASE_ENTRYDO:
    		pinst->v_X=TRUE;
    		if(pinst->v_qualifier == SSC_QUALIFIER_ENTRY){
    			/* entry */
    			ov_time_gettime(&pinst->v_startTime);
    			//initialized with zero
    			ov_time_diff(&pinst->v_T, &pinst->v_startTime, &pinst->v_startTime);
    			//printf("%s/%s/entry\n", pSSC->v_identifier, pinst->v_identifier);
    			pinst->v_qualifier = SSC_QUALIFIER_DO;
    			pEntry->v_actimode = FB_AM_ONCE;
    			pDo->v_actimode = FB_AM_ON;
    		}

    		/* event: SSC terminates */
    		if (pinst->v_internalRole == SSC_STEPROLE_END){
    			pSSC->v_workingState = SSC_WOST_TERMINATE;
    		}

    		pinst->v_phase = SSC_PHASE_EXITTRANS;
    		break;

    	/* phase 2: transitions, exit*/
    	case SSC_PHASE_EXITTRANS:
    		//check all transitions for the result
    		Ov_ForEachChildEx(ssc_nextTransitions, pinst, pTransition, ssc_transition){
        		if (pTransition->v_result){
            		// trigger
        			if (!pinst->v_evTransTrigger){
        				//this is the first transition which fires here, remember the next step
        				pinst->v_evTransTrigger=TRUE;

        				// link next-step to ssc-tasklist
        				pNextStep = Ov_GetParent(ssc_previousTransitions, pTransition);
        				if(pNextStep){
        					//transition is correct linked
        					if(Ov_GetParent(fb_tasklist, pNextStep)){
        						//ensure the next step is linked right
        						Ov_Unlink(fb_tasklist, Ov_GetParent(fb_tasklist, pNextStep), pNextStep);
        					}
            				result = Ov_Link(fb_tasklist, Ov_GetPartPtr(taskActiveStep, pSSC), pNextStep);
            				if(Ov_Fail(result)){
            					pinst->v_error = TRUE;
            					ov_string_setvalue(&pinst->v_errorDetail, "Internal linking failed. Giving up.");
            					pSSC->v_workingState = SSC_WOST_STOP;
            				}
            				pNextStep->v_actimode = FB_AM_ON;
            				pNextStep->v_phase = SSC_PHASE_ENTRYDO;
            				pNextStep->v_qualifier = SSC_QUALIFIER_ENTRY;
        				}else{
        					//we need to recover...
        					pinst->v_error = TRUE;
        					ov_string_setvalue(&pinst->v_errorDetail, "Internal associations not clean. Giving up.");
        					pSSC->v_workingState = SSC_WOST_STOP;
        				}
        			}
        		}
    		}

    		// if Trigger, or cmd "STOP" for final step
			if ( pinst->v_evTransTrigger || pSSC->v_workingState == SSC_WOST_STOP ){
				// stop/break subSSCs
				// find all action blocks calling subSSCs
				Ov_ForEachChildEx(ov_containment, pinst, pExecute, ssc_execute){
					ssc_getObjectFromExecute(pExecute, pExecute->v_targetObject, &pTargetObj);
					// find all subSSCs for do
					if (pTargetObj != NULL && Ov_CanCastTo(ssc_SequentialStateChart, pTargetObj) && pExecute->v_actionQualifier == SSC_QUALIFIER_DO){
						// stop subSSC
						pSubSsc = Ov_StaticPtrCast(ssc_SequentialStateChart, pTargetObj);
    					pSubSsc->v_EN = SSC_CMD_STOP;
    					if(fb_task_has_initial_taskparent(Ov_PtrUpCast(fb_task, pSubSsc)))
    						Ov_Call1(fb_task, Ov_PtrUpCast(fb_task, pSubSsc), execute, pltc);
    					else
    						ov_logfile_warning("%s: tried to execute ssc %s which is not connected to UrTask", pinst->v_identifier, pSubSsc->v_identifier);
					}
				}

				if (pinst->v_evTransTrigger){
					//we are leaving the step
					pExit->v_actimode = FB_AM_ONCE;
				}

    			/* exit */
				// check exit task is linked correctly
				if(Ov_GetParent(fb_tasklist, pExit)!=(Ov_PtrUpCast(fb_task,pinst))){
					Ov_Unlink(fb_tasklist, Ov_GetParent(fb_tasklist, pExit), pExit);
					Ov_Link(fb_tasklist, pinst, pExit);
				}
				Ov_Call1 (fb_task, pExit, execute, pltc);
    			pDo->v_actimode = FB_AM_OFF;
    			// unlink from SequentialStateChart.taskActiveStep
    			Ov_Unlink(fb_tasklist, Ov_GetParent(fb_tasklist, pinst), pinst);
    			pinst->v_X = FALSE;
    			pinst->v_qualifier = SSC_QUALIFIER_ENTRY;

    			// find next step and execute its entry & do (in the same execution cycle)
    			// Note: this job should be done by ov_ForEachChild(fb_tasklist, ...). But it is not possible to adapt the tasklist dynamically
//    			pNextStep=Ov_DynamicPtrCast(ssc_step, Ov_GetLastChild(fb_tasklist, Ov_GetPartPtr(taskActiveStep, pSSC)));
    			if (pNextStep != NULL){
    				// execute nextStep entry & do
    				Ov_Call1 (fb_task, Ov_DynamicPtrCast(fb_task, pNextStep), execute, pltc);
    			}
    		} else {
    			exitLoop=FALSE;
    		}

    		pinst->v_phase = SSC_PHASE_ENTRYDO;
    		break;
    	}
		ov_time_gettime(&elapsedTime);
		ov_time_diff(&pinst->v_T, &elapsedTime, &pinst->v_startTime);
    } while (!exitLoop);

    // step flag will be reseted by triggered transition
    return;
}

OV_DLLFNCEXPORT OV_RESULT ssc_step_resetStep(
		OV_INSTPTR_ssc_step	pinst
) {
	/*
	 *   local variables
	 */
    OV_INSTPTR_fb_task    pEntry = Ov_GetPartPtr(entry, pinst);
    OV_INSTPTR_fb_task    pDo 	 = Ov_GetPartPtr(do, pinst);
    OV_INSTPTR_fb_task    pExit  = Ov_GetPartPtr(exit, pinst);
	OV_INSTPTR_ssc_actionBlock pActionBlock  = NULL;

	OV_INSTPTR_fb_task pTaskParent = Ov_GetParent(fb_tasklist, pinst);

	//unlink from task parent
	if (pTaskParent != NULL){
		Ov_Unlink(fb_tasklist, pTaskParent, pinst);
	}

	//reset parameters
	pinst->v_actimode = FB_AM_OFF;
	pinst->v_cyctime.secs = 0;
	pinst->v_cyctime.usecs = 0;
	pinst->v_iexreq = TRUE;
	pinst->v_X = FALSE;
	//TODO: T, error, errorDetail
	pinst->v_phase = SSC_PHASE_ENTRYDO;
	pinst->v_qualifier = SSC_QUALIFIER_ENTRY;

	//reset subtasks
	pEntry->v_actimode = FB_AM_ON;
	pDo->v_actimode = FB_AM_ON;
	pExit->v_actimode = FB_AM_OFF;	//this is set to FB_AM_ONCE and executed on leaving the step

	// TODO: reset all action blocks

	//activate all action blocks
	Ov_ForEachChildEx(ov_containment, pinst, pActionBlock, ssc_actionBlock)
	{
		pActionBlock->v_actimode = FB_AM_ON;
		pActionBlock->v_cyctime.secs = 0;
		pActionBlock->v_cyctime.usecs = 0;
		pActionBlock->v_iexreq = TRUE;
	}


	return OV_ERR_OK;
}

/**
 * disallow deletion in run state
 */
OV_DLLFNCEXPORT OV_ACCESS ssc_step_getaccess(
	OV_INSTPTR_ov_object	pobj,
	const OV_ELEMENT		*pelem,
	const OV_TICKET			*pticket
) {
	/*
	*   local variables
	*/
	OV_INSTPTR_ssc_SequentialStateChart activeHeader = Ov_DynamicPtrCast(ssc_SequentialStateChart, Ov_GetParent(ov_containment, pobj));
	OV_ACCESS access_code = fb_functionblock_getaccess(pobj, pelem, pticket);

	/*
	*	switch based on the element's type
	*/
	switch(pelem->elemtype) {
		case OV_ET_OBJECT:
			if(!activeHeader){
				//skip handling
			}else if(	activeHeader->v_error == TRUE ||
						activeHeader->v_workingState == SSC_WOST_INIT ||
						activeHeader->v_workingState == SSC_WOST_STOP ||
						activeHeader->v_workingState == SSC_WOST_TERMINATE)
			{
				//allow deletion
				access_code = (access_code | OV_AC_DELETEABLE);
			}else{
				//disallow deletion
				access_code = (access_code &~ OV_AC_DELETEABLE);
			}
			break;
		default:
			break;
	}
	return access_code;
}
