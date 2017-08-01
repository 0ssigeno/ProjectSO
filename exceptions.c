#include <libuarm.h>
#include <uARMconst.h>
#include <uARMtypes.h>
#include <arch.h>

#include "const.h"


#include "mikabooq.h"


#include "scheduler.h"
#include "nucleus.h"

state_t *tlb_old 	 = (state_t*) TLB_OLDAREA;
state_t *pgmtrap_old = (state_t*) PGMTRAP_OLDAREA;
state_t *sysbp_old 	 = (state_t*) SYSBK_OLDAREA;

void saveStateIn(state_t *from, state_t *to){
        to->a1                  = from->a1;
        to->a2                  = from->a2;
        to->a3                  = from->a3;
        to->a4                  = from->a4;
        to->v1                  = from->v1;
        to->v2                  = from->v2;
        to->v3                  = from->v3;
        to->v4                  = from->v4;
        to->v5                  = from->v5;
        to->v6                  = from->v6;
        to->sl                  = from->sl;
        to->fp                  = from->fp;
        to->ip                  = from->ip;
        to->sp                  = from->sp;
        to->lr                  = from->lr;
        to->pc                  = from->pc;
        to->cpsr                = from->cpsr;
        to->CP15_Control        = from->CP15_Control;
        to->CP15_EntryHi        = from->CP15_EntryHi;
        to->CP15_Cause          = from->CP15_Cause;
        to->TOD_Hi              = from->TOD_Hi;
        to->TOD_Low             = from->TOD_Low;
}

void useExStVec(int type){
	/*
    if(currentProcess!=NULL)  {
        // Se ho già fatto spectrapvec per il tipo di eccezione
        if (currentProcess->excStVec[type*2]!=NULL){
            // Salvo lo stato nella oldarea adeguata
            switch(type){
                case SPECTLB:
                	saveStateIn(tlb_old, currentProcess->excStVec[type*2]);
                    break;
                case SPECPGMT:
                	saveStateIn(pgmtrap_old, currentProcess->excStVec[type*2]);
                    break;
                case SPECSYSBP:
                	saveStateIn(sysbp_old, currentProcess->excStVec[type*2]);
                    break;
            }
            // Carico lo stato dalla newarea 
            LDST(currentProcess->excStVec[(type*2)+1]);
        }else{
           // Altrimenti tratto come una SYS2  
           terminateProcess(currentProcess); 
           scheduler();
        }
    } 
*/
}
void tlbHandler(){
    // Se un processo è eseguito dal processore salvo lo stato nella tlb_oldarea 
	/*
	if(currentProcess != NULL){
		saveStateIn(tlb_old, &currentProcess->p_s);
	}

	useExStVec(SPECTLB);
	*/
}

void pgmHandler(){
    //se un processo è eseguito dal processore salvo lo stato nella pgmtrap_oldarea*/
	/* if(currentProcess != NULL){
		saveStateIn(pgmtrap_old, &currentProcess->sp);
	}
	useExStVec(SPECPGMT);
	*/
}

void sysBpHandler(){
	saveStateIn(sysbp_old, &currentProcess->t_s);
	unsigned int cause = CAUSE_EXCCODE_GET(sysbp_old->CP15_Cause);
	unsigned int a0 = (*sysbp_old).a1;
	unsigned int a1 = (*sysbp_old).a2;
	unsigned int a2 = (*sysbp_old).a3;
	unsigned int a3 = (*sysbp_old).a4;

	// Se l'eccezione è di tipo System call 
	if(cause==EXC_SYSCALL){
    	// Se il processo è in kernel mode gestisce adeguatamente 
    	if( (currentProcess->t_s.cpsr & STATUS_SYS_MODE) == STATUS_SYS_MODE){
			// Se è fra SYS1 e SYS8 richiama le funzioni adeguate 
			switch(a0){
			    case SYS_SEND:
			        //do msg send
			        break;
			    case SYS_RECV:
			        //do message recv
			        break;
			    // Altrimenti la gestione viene passata in alto 
			    default:
			        //useExStVec(SPECSYSBP);
				break;            
			}
			
		    // Richiamo lo scheduler 
		    scheduler();
		// Se invece è in user mode 
		} else if((currentProcess->t_s.cpsr & STATUS_USER_MODE) == STATUS_USER_MODE){
			/*
			// Se è una system call 
			if(a0 >= CREATEPROCESS && a0 <= WAITIO){
			    // Gestisco come fosse una program trap 
			    saveStateIn(sysbp_old, pgmtrap_old);
			    // Setto il registro cause a Reserved Instruction 
			    pgmtrap_old->CP15_Cause = CAUSE_EXCCODE_SET(pgmtrap_old->CP15_Cause, EXC_RESERVEDINSTR);
			    // Richiamo l'handler per le pgmtrap 
			    pgmHandler();
			} else {
				useExStVec(SPECSYSBP);
			}
			*/
		}
	// Altrimenti se l'eccezione è di tipo BreakPoint 
	} else if(cause == EXC_BREAKPOINT){
		//useExStVec(SPECSYSBP);
	}

	PANIC();
}