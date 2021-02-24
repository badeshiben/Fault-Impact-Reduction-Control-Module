/* Include files */

#include "CART3_Baseline_sfun.h"
#include "CART3_Baseline_sfun_debug_macros.h"
#include "c1_CART3_Baseline.h"
#include "c2_CART3_Baseline.h"
#include "c3_CART3_Baseline.h"
#include "c4_CART3_Baseline.h"
#include "c5_CART3_Baseline.h"

/* Type Definitions */

/* Named Constants */

/* Variable Declarations */

/* Variable Definitions */
uint32_T _CART3_BaselineMachineNumber_;

/* Function Declarations */

/* Function Definitions */
void CART3_Baseline_initializer(void)
{
}

void CART3_Baseline_terminator(void)
{
}

/* SFunction Glue Code */
unsigned int sf_CART3_Baseline_method_dispatcher(SimStruct *simstructPtr,
  unsigned int chartFileNumber, const char* specsCksum, int_T method, void *data)
{
  if (chartFileNumber==1) {
    c1_CART3_Baseline_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==2) {
    c2_CART3_Baseline_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==3) {
    c3_CART3_Baseline_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==4) {
    c4_CART3_Baseline_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  if (chartFileNumber==5) {
    c5_CART3_Baseline_method_dispatcher(simstructPtr, method, data);
    return 1;
  }

  return 0;
}

unsigned int sf_CART3_Baseline_process_check_sum_call( int nlhs, mxArray * plhs[],
  int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[20];
  if (nrhs<1 || !mxIsChar(prhs[0]) )
    return 0;

  /* Possible call to get the checksum */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"sf_get_check_sum"))
    return 0;
  plhs[0] = mxCreateDoubleMatrix( 1,4,mxREAL);
  if (nrhs>1 && mxIsChar(prhs[1])) {
    mxGetString(prhs[1], commandName,sizeof(commandName)/sizeof(char));
    commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
    if (!strcmp(commandName,"machine")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3909357710U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(877698426U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(3770184809U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1975561639U);
    } else if (!strcmp(commandName,"exportedFcn")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(0U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(0U);
    } else if (!strcmp(commandName,"makefile")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(1009251407U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(91971410U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(283239133U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(215020930U);
    } else if (nrhs==3 && !strcmp(commandName,"chart")) {
      unsigned int chartFileNumber;
      chartFileNumber = (unsigned int)mxGetScalar(prhs[2]);
      switch (chartFileNumber) {
       case 1:
        {
          extern void sf_c1_CART3_Baseline_get_check_sum(mxArray *plhs[]);
          sf_c1_CART3_Baseline_get_check_sum(plhs);
          break;
        }

       case 2:
        {
          extern void sf_c2_CART3_Baseline_get_check_sum(mxArray *plhs[]);
          sf_c2_CART3_Baseline_get_check_sum(plhs);
          break;
        }

       case 3:
        {
          extern void sf_c3_CART3_Baseline_get_check_sum(mxArray *plhs[]);
          sf_c3_CART3_Baseline_get_check_sum(plhs);
          break;
        }

       case 4:
        {
          extern void sf_c4_CART3_Baseline_get_check_sum(mxArray *plhs[]);
          sf_c4_CART3_Baseline_get_check_sum(plhs);
          break;
        }

       case 5:
        {
          extern void sf_c5_CART3_Baseline_get_check_sum(mxArray *plhs[]);
          sf_c5_CART3_Baseline_get_check_sum(plhs);
          break;
        }

       default:
        ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(0.0);
        ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(0.0);
        ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(0.0);
        ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(0.0);
      }
    } else if (!strcmp(commandName,"target")) {
      ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(814460797U);
      ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(400623215U);
      ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(1072597456U);
      ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1176453921U);
    } else {
      return 0;
    }
  } else {
    ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3133356259U);
    ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(4289856087U);
    ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(3579450760U);
    ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(4054936327U);
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_CART3_Baseline_autoinheritance_info( int nlhs, mxArray * plhs[],
  int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[32];
  char aiChksum[64];
  if (nrhs<3 || !mxIsChar(prhs[0]) )
    return 0;

  /* Possible call to get the autoinheritance_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_autoinheritance_info"))
    return 0;
  mxGetString(prhs[2], aiChksum,sizeof(aiChksum)/sizeof(char));
  aiChksum[(sizeof(aiChksum)/sizeof(char)-1)] = '\0';

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        if (strcmp(aiChksum, "YesZZdkQsY0Rcbtja3BLbD") == 0) {
          extern mxArray *sf_c1_CART3_Baseline_get_autoinheritance_info(void);
          plhs[0] = sf_c1_CART3_Baseline_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 2:
      {
        if (strcmp(aiChksum, "WMP4YCOVG8cwLhSDuFw29E") == 0) {
          extern mxArray *sf_c2_CART3_Baseline_get_autoinheritance_info(void);
          plhs[0] = sf_c2_CART3_Baseline_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 3:
      {
        if (strcmp(aiChksum, "ZdUZosQ34dVVTM8BdvwQmH") == 0) {
          extern mxArray *sf_c3_CART3_Baseline_get_autoinheritance_info(void);
          plhs[0] = sf_c3_CART3_Baseline_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 4:
      {
        if (strcmp(aiChksum, "4Uc0Ze5t2xRnqqtWgGLRgG") == 0) {
          extern mxArray *sf_c4_CART3_Baseline_get_autoinheritance_info(void);
          plhs[0] = sf_c4_CART3_Baseline_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     case 5:
      {
        if (strcmp(aiChksum, "jJJ9YYzlkkaPBU9QuZpTDF") == 0) {
          extern mxArray *sf_c5_CART3_Baseline_get_autoinheritance_info(void);
          plhs[0] = sf_c5_CART3_Baseline_get_autoinheritance_info();
          break;
        }

        plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
        break;
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_CART3_Baseline_get_eml_resolved_functions_info( int nlhs,
  mxArray * plhs[], int nrhs, const mxArray * prhs[] )
{

#ifdef MATLAB_MEX_FILE

  char commandName[64];
  if (nrhs<2 || !mxIsChar(prhs[0]))
    return 0;

  /* Possible call to get the get_eml_resolved_functions_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_eml_resolved_functions_info"))
    return 0;

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        extern const mxArray
          *sf_c1_CART3_Baseline_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c1_CART3_Baseline_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 2:
      {
        extern const mxArray
          *sf_c2_CART3_Baseline_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c2_CART3_Baseline_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 3:
      {
        extern const mxArray
          *sf_c3_CART3_Baseline_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c3_CART3_Baseline_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 4:
      {
        extern const mxArray
          *sf_c4_CART3_Baseline_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c4_CART3_Baseline_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     case 5:
      {
        extern const mxArray
          *sf_c5_CART3_Baseline_get_eml_resolved_functions_info(void);
        mxArray *persistentMxArray = (mxArray *)
          sf_c5_CART3_Baseline_get_eml_resolved_functions_info();
        plhs[0] = mxDuplicateArray(persistentMxArray);
        mxDestroyArray(persistentMxArray);
        break;
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;

#else

  return 0;

#endif

}

unsigned int sf_CART3_Baseline_third_party_uses_info( int nlhs, mxArray * plhs[],
  int nrhs, const mxArray * prhs[] )
{
  char commandName[64];
  char tpChksum[64];
  if (nrhs<3 || !mxIsChar(prhs[0]))
    return 0;

  /* Possible call to get the third_party_uses_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  mxGetString(prhs[2], tpChksum,sizeof(tpChksum)/sizeof(char));
  tpChksum[(sizeof(tpChksum)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_third_party_uses_info"))
    return 0;

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        if (strcmp(tpChksum, "Z7rAMZJEOcE6z6aOwfy5FH") == 0) {
          extern mxArray *sf_c1_CART3_Baseline_third_party_uses_info(void);
          plhs[0] = sf_c1_CART3_Baseline_third_party_uses_info();
          break;
        }
      }

     case 2:
      {
        if (strcmp(tpChksum, "u0rUT7AIAqQ8KtAk2XJTtB") == 0) {
          extern mxArray *sf_c2_CART3_Baseline_third_party_uses_info(void);
          plhs[0] = sf_c2_CART3_Baseline_third_party_uses_info();
          break;
        }
      }

     case 3:
      {
        if (strcmp(tpChksum, "d9QLWqChYWi3Bl4r5ENtAH") == 0) {
          extern mxArray *sf_c3_CART3_Baseline_third_party_uses_info(void);
          plhs[0] = sf_c3_CART3_Baseline_third_party_uses_info();
          break;
        }
      }

     case 4:
      {
        if (strcmp(tpChksum, "dL9E6zzHxRYY5fh0s58Y7G") == 0) {
          extern mxArray *sf_c4_CART3_Baseline_third_party_uses_info(void);
          plhs[0] = sf_c4_CART3_Baseline_third_party_uses_info();
          break;
        }
      }

     case 5:
      {
        if (strcmp(tpChksum, "13BbbCsLVwRHM4ozjv6KiE") == 0) {
          extern mxArray *sf_c5_CART3_Baseline_third_party_uses_info(void);
          plhs[0] = sf_c5_CART3_Baseline_third_party_uses_info();
          break;
        }
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;
}

unsigned int sf_CART3_Baseline_updateBuildInfo_args_info( int nlhs, mxArray *
  plhs[], int nrhs, const mxArray * prhs[] )
{
  char commandName[64];
  char tpChksum[64];
  if (nrhs<3 || !mxIsChar(prhs[0]))
    return 0;

  /* Possible call to get the updateBuildInfo_args_info */
  mxGetString(prhs[0], commandName,sizeof(commandName)/sizeof(char));
  commandName[(sizeof(commandName)/sizeof(char)-1)] = '\0';
  mxGetString(prhs[2], tpChksum,sizeof(tpChksum)/sizeof(char));
  tpChksum[(sizeof(tpChksum)/sizeof(char)-1)] = '\0';
  if (strcmp(commandName,"get_updateBuildInfo_args_info"))
    return 0;

  {
    unsigned int chartFileNumber;
    chartFileNumber = (unsigned int)mxGetScalar(prhs[1]);
    switch (chartFileNumber) {
     case 1:
      {
        if (strcmp(tpChksum, "Z7rAMZJEOcE6z6aOwfy5FH") == 0) {
          extern mxArray *sf_c1_CART3_Baseline_updateBuildInfo_args_info(void);
          plhs[0] = sf_c1_CART3_Baseline_updateBuildInfo_args_info();
          break;
        }
      }

     case 2:
      {
        if (strcmp(tpChksum, "u0rUT7AIAqQ8KtAk2XJTtB") == 0) {
          extern mxArray *sf_c2_CART3_Baseline_updateBuildInfo_args_info(void);
          plhs[0] = sf_c2_CART3_Baseline_updateBuildInfo_args_info();
          break;
        }
      }

     case 3:
      {
        if (strcmp(tpChksum, "d9QLWqChYWi3Bl4r5ENtAH") == 0) {
          extern mxArray *sf_c3_CART3_Baseline_updateBuildInfo_args_info(void);
          plhs[0] = sf_c3_CART3_Baseline_updateBuildInfo_args_info();
          break;
        }
      }

     case 4:
      {
        if (strcmp(tpChksum, "dL9E6zzHxRYY5fh0s58Y7G") == 0) {
          extern mxArray *sf_c4_CART3_Baseline_updateBuildInfo_args_info(void);
          plhs[0] = sf_c4_CART3_Baseline_updateBuildInfo_args_info();
          break;
        }
      }

     case 5:
      {
        if (strcmp(tpChksum, "13BbbCsLVwRHM4ozjv6KiE") == 0) {
          extern mxArray *sf_c5_CART3_Baseline_updateBuildInfo_args_info(void);
          plhs[0] = sf_c5_CART3_Baseline_updateBuildInfo_args_info();
          break;
        }
      }

     default:
      plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL);
    }
  }

  return 1;
}

void CART3_Baseline_debug_initialize(struct SfDebugInstanceStruct* debugInstance)
{
  _CART3_BaselineMachineNumber_ = sf_debug_initialize_machine(debugInstance,
    "CART3_Baseline","sfun",0,5,0,0,0);
  sf_debug_set_machine_event_thresholds(debugInstance,
    _CART3_BaselineMachineNumber_,0,0);
  sf_debug_set_machine_data_thresholds(debugInstance,
    _CART3_BaselineMachineNumber_,0);
}

void CART3_Baseline_register_exported_symbols(SimStruct* S)
{
}

static mxArray* sRtwOptimizationInfoStruct= NULL;
mxArray* load_CART3_Baseline_optimization_info(void)
{
  if (sRtwOptimizationInfoStruct==NULL) {
    sRtwOptimizationInfoStruct = sf_load_rtw_optimization_info("CART3_Baseline",
      "CART3_Baseline");
    mexMakeArrayPersistent(sRtwOptimizationInfoStruct);
  }

  return(sRtwOptimizationInfoStruct);
}

void unload_CART3_Baseline_optimization_info(void)
{
  if (sRtwOptimizationInfoStruct!=NULL) {
    mxDestroyArray(sRtwOptimizationInfoStruct);
    sRtwOptimizationInfoStruct = NULL;
  }
}
