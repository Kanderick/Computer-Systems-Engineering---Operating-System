#ifndef EXC_WRAP
#define EXC_WRAP

#include "types.h"


extern void exception_DE_wrapper(void);
extern void exception_DB_wrapper(void);
extern void exception_NMI_wrapper(void);
extern void exception_BP_wrapper(void);
extern void exception_OF_wrapper(void);
extern void exception_BR_wrapper(void);
extern void exception_UD_wrapper(void);
extern void exception_NM_wrapper(void);
extern void exception_CSO_wrapper(void);
extern void exception_TS_wrapper(void);
extern void exception_NP_wrapper(void);
extern void exception_SS_wrapper(void);
extern void exception_GP_wrapper(void);
extern void exception_PF_wrapper(void);
extern void exception_MF_wrapper(void);
extern void exception_AC_wrapper(void);
extern void exception_MC_wrapper(void);
extern void exception_XF_wrapper(void);

#endif
