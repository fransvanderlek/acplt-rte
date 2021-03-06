/*
*   The Framework was generated by the ACPLT/OV Builder.
*
*   Builder version: 1.0
*   Date of file generation:  16-Jul-2001 (17:44:49)
*/
#ifndef OV_COMPILE_LIBRARY_kshist
#define OV_COMPILE_LIBRARY_kshist
#endif

#include "kshist.h"
#include "ov_macros.h"


OV_DLLFNCEXPORT OV_UINT * kshist_KsSimpleStateTrack_StateVec_get(
             OV_INSTPTR_kshist_KsSimpleStateTrack          pobj,
             OV_UINT *pveclen
) {
             *pveclen = pobj->v_StateVec.veclen;
             return pobj->v_StateVec.value;
}

OV_DLLFNCEXPORT OV_RESULT kshist_KsSimpleStateTrack_StateVec_set(
             OV_INSTPTR_kshist_KsSimpleStateTrack          pobj,
             const OV_UINT *pvalue,
             const OV_UINT veclen
) {
             return Ov_SetDynamicVectorValue(&pobj->v_StateVec,pvalue,veclen,UINT);
}

OV_DLLFNCEXPORT OV_RESULT kshist_KsSimpleStateTrack_constructor(
	OV_INSTPTR_ov_object 	pobj
) {
	OV_INSTPTR_kshist_KsSimpleStateTrack 	ptrack;

	ptrack = Ov_StaticPtrCast(kshist_KsSimpleStateTrack, pobj);
	ptrack->v_Type = OV_VT_STATE;		
	ov_string_setvalue(&ptrack->v_TechUnit, "State");
	return OV_ERR_OK;
}

OV_DLLFNCEXPORT OV_RESULT kshist_KsSimpleStateTrack_gettrackhist(
  OV_INSTPTR_ov_object pobj,
  OV_HISTSELECTOR* phistsel,
  OV_VAR_VALUE* pvarvalue,
  OV_UINT max_answers	
) {
	OV_INSTPTR_kshist_KsSimpleTimeTrack 	pTimeTrack;
	OV_INSTPTR_kshist_KsSimpleStateTrack 	pStateTrack;
	OV_TIME	from,to;
	OV_STATE *pint;
	OV_UINT answers;
	OV_UINT i;

	pStateTrack = Ov_StaticPtrCast(kshist_KsSimpleStateTrack, pobj);
	pvarvalue->vartype = OV_VT_STATE_VEC;
	pvarvalue->valueunion.val_state_vec.value = (OV_STATE*)ov_memstack_alloc(max_answers*sizeof(OV_STATE));		
	pint = pvarvalue->valueunion.val_state_vec.value;
	switch(phistsel->hseltype) {
		case OV_HSELT_NONE:
			/*
			*	get all messages available
			*/
			from.secs = 0;
			from.usecs = 0;
			ov_time_gettime(&to);
			break;
		case OV_HSELT_TIME:
			/*
			*	only get selected messages
			*/
			if(phistsel->OV_HISTSELECTOR_u.ths.ip_mode != OV_IPM_NONE) {
				/* messages are change driven; no interpolation available */
				return OV_ERR_BADSELECTOR;
			}
			switch(phistsel->OV_HISTSELECTOR_u.ths.from.timetype) {
			case OV_TT_ABSOLUTE:
				from = phistsel->OV_HISTSELECTOR_u.ths.from.OV_ABSRELTIME_u.abstime;
				break;
			case OV_TT_RELATIVE:
				ov_time_gettime(&from);
				ov_time_add(&from, &from,
					&phistsel->OV_HISTSELECTOR_u.ths.from.OV_ABSRELTIME_u.reltime);
				break;
			default:
				return OV_ERR_BADSELECTOR;
			}
			switch(phistsel->OV_HISTSELECTOR_u.ths.to.timetype) {
			case OV_TT_ABSOLUTE:
				to = phistsel->OV_HISTSELECTOR_u.ths.to.OV_ABSRELTIME_u.abstime;
				break;
			case OV_TT_RELATIVE:
				ov_time_gettime(&to);
				ov_time_add(&to, &to,
					&phistsel->OV_HISTSELECTOR_u.ths.to.OV_ABSRELTIME_u.reltime);
				break;
			default:
				return OV_ERR_BADSELECTOR;
			}
			break;
		case OV_HSELT_STRING:
			/*
			*	not supported
			*/
			/* fall through... */
		default:
			return OV_ERR_BADSELECTOR;
	}	/* switch() */

	pTimeTrack = Ov_GetParent(kshist_isTimetrackOf, pStateTrack);
	answers = 0;
	if (pTimeTrack) {
		for (i=0; ((i< pStateTrack->v_StateVec.veclen) && (i< pTimeTrack->v_TimeVec.veclen) && (answers < max_answers)); i++) {
			if ((ov_time_compare((const OV_TIME*) &pTimeTrack->v_TimeVec.value[i], (const OV_TIME*) &from) >= 0 ) &&
			   (ov_time_compare((const OV_TIME*) &pTimeTrack->v_TimeVec.value[i], (const OV_TIME*) &to) <= 0 )) {
				*pint = pStateTrack->v_StateVec.value[i];
				answers++;
				pint++;
			}
		}
	}
	pvarvalue->valueunion.val_state_vec.veclen = answers;
	return OV_ERR_OK;
}

