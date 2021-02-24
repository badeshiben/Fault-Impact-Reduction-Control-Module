#ifndef __c2_CART3_BladeFlap_IPC_1204_h__
#define __c2_CART3_BladeFlap_IPC_1204_h__

/* Include files */
#include "sf_runtime/sfc_sf.h"
#include "sf_runtime/sfc_mex.h"
#include "rtwtypes.h"
#include "multiword_types.h"

/* Type Definitions */
#ifndef typedef_SFc2_CART3_BladeFlap_IPC_1204InstanceStruct
#define typedef_SFc2_CART3_BladeFlap_IPC_1204InstanceStruct

typedef struct {
  SimStruct *S;
  ChartInfoStruct chartInfo;
  uint32_T chartNumber;
  uint32_T instanceNumber;
  int32_T c2_sfEvent;
  boolean_T c2_isStable;
  boolean_T c2_doneDoubleBufferReInit;
  uint8_T c2_is_active_c2_CART3_BladeFlap_IPC_1204;
} SFc2_CART3_BladeFlap_IPC_1204InstanceStruct;

#endif                                 /*typedef_SFc2_CART3_BladeFlap_IPC_1204InstanceStruct*/

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */

/* Function Declarations */
extern const mxArray
  *sf_c2_CART3_BladeFlap_IPC_1204_get_eml_resolved_functions_info(void);

/* Function Definitions */
extern void sf_c2_CART3_BladeFlap_IPC_1204_get_check_sum(mxArray *plhs[]);
extern void c2_CART3_BladeFlap_IPC_1204_method_dispatcher(SimStruct *S, int_T
  method, void *data);

#endif
