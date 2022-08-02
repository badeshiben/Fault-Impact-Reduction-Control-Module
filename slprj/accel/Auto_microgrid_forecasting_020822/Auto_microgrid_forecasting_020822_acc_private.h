#ifndef RTW_HEADER_Auto_microgrid_forecasting_020822_acc_private_h_
#define RTW_HEADER_Auto_microgrid_forecasting_020822_acc_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include "Auto_microgrid_forecasting_020822_acc.h"
#if !defined(ss_VALIDATE_MEMORY)
#define ss_VALIDATE_MEMORY(S, ptr)   if(!(ptr)) {\
  ssSetErrorStatus(S, RT_MEMORY_ALLOCATION_ERROR);\
  }
#endif
#if !defined(rt_FREE)
#if !defined(_WIN32)
#define rt_FREE(ptr)   if((ptr) != (NULL)) {\
  free((ptr));\
  (ptr) = (NULL);\
  }
#else
#define rt_FREE(ptr)   if((ptr) != (NULL)) {\
  free((void *)(ptr));\
  (ptr) = (NULL);\
  }
#endif
#endif
#ifndef UCHAR_MAX
#include <limits.h>
#endif
#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error Code was generated for compiler with different sized uchar/char. \Consider adjusting Test hardware word size settings on the \Hardware Implementation pane to match your compiler word sizes as \defined in limits.h of the compiler. Alternatively, you can \select the Test hardware is the same as production hardware option and \select the Enable portable word sizes option on the Code Generation > \Verification pane for ERT based targets, which will disable the \preprocessor word size checks.
#endif
#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \Consider adjusting Test hardware word size settings on the \Hardware Implementation pane to match your compiler word sizes as \defined in limits.h of the compiler. Alternatively, you can \select the Test hardware is the same as production hardware option and \select the Enable portable word sizes option on the Code Generation > \Verification pane for ERT based targets, which will disable the \preprocessor word size checks.
#endif
#if ( UINT_MAX != (0xFFFFFFFFU) ) || ( INT_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized uint/int. \Consider adjusting Test hardware word size settings on the \Hardware Implementation pane to match your compiler word sizes as \defined in limits.h of the compiler. Alternatively, you can \select the Test hardware is the same as production hardware option and \select the Enable portable word sizes option on the Code Generation > \Verification pane for ERT based targets, which will disable the \preprocessor word size checks.
#endif
#if ( ULONG_MAX != (0xFFFFFFFFU) ) || ( LONG_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized ulong/long. \Consider adjusting Test hardware word size settings on the \Hardware Implementation pane to match your compiler word sizes as \defined in limits.h of the compiler. Alternatively, you can \select the Test hardware is the same as production hardware option and \select the Enable portable word sizes option on the Code Generation > \Verification pane for ERT based targets, which will disable the \preprocessor word size checks.
#endif
#ifndef __RTW_UTFREE__
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
extern void rt_invd5x5_snf ( const real_T u [ 25 ] , real_T y [ 25 ] ) ;
boolean_T Auto_microgrid_forecasting_020822_acc_rt_TDelayUpdateTailOrGrowBuf
( int_T * bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr ,
real_T tMinusDelay , real_T * * tBufPtr , real_T * * uBufPtr , real_T * *
xBufPtr , boolean_T isfixedbuf , boolean_T istransportdelay , int_T *
maxNewBufSzPtr ) ; real_T
Auto_microgrid_forecasting_020822_acc_rt_VTDelayfindtDInterpolate ( real_T x
, real_T * tBuf , real_T * uBuf , real_T * xBuf , int_T bufSz , int_T head ,
int_T tail , int_T * pLast , real_T t , real_T tStart , boolean_T discrete ,
boolean_T minorStepAndTAtLastMajorOutput , real_T initOutput , real_T *
appliedDelay ) ; real_T
Auto_microgrid_forecasting_020822_acc_rt_TDelayInterpolate ( real_T
tMinusDelay , real_T tStart , real_T * tBuf , real_T * uBuf , int_T bufSz ,
int_T * lastIdx , int_T oldestIdx , int_T newIdx , real_T initOutput ,
boolean_T discrete , boolean_T minorStepAndTAtLastMajorOutput ) ; extern
real_T look1_binlxpw ( real_T u0 , const real_T bp0 [ ] , const real_T table
[ ] , uint32_T maxIndex ) ; extern real_T look2_binlxpw ( real_T u0 , real_T
u1 , const real_T bp0 [ ] , const real_T bp1 [ ] , const real_T table [ ] ,
const uint32_T maxIndex [ ] , uint32_T stride ) ; extern real_T
look1_pbinlxpw ( real_T u0 , const real_T bp0 [ ] , const real_T table [ ] ,
uint32_T prevIndex [ ] , uint32_T maxIndex ) ; void
Auto_microgrid_forecasting_020822_Subsystempi2delay_Init ( SimStruct * S ,
B_Subsystempi2delay_Auto_microgrid_forecasting_020822_T * localB ,
P_Subsystempi2delay_Auto_microgrid_forecasting_020822_T * localP ) ; void
Auto_microgrid_forecasting_020822_Subsystempi2delay_Disable ( SimStruct * S ,
DW_Subsystempi2delay_Auto_microgrid_forecasting_020822_T * localDW ) ; void
Auto_microgrid_forecasting_020822_Subsystempi2delay ( SimStruct * S , uint8_T
rtu_Enable , const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_Auto_microgrid_forecasting_020822_T * localB ,
DW_Subsystempi2delay_Auto_microgrid_forecasting_020822_T * localDW ) ; void
Auto_microgrid_forecasting_020822_Subsystem1_Init ( SimStruct * S ,
B_Subsystem1_Auto_microgrid_forecasting_020822_T * localB ,
P_Subsystem1_Auto_microgrid_forecasting_020822_T * localP ) ; void
Auto_microgrid_forecasting_020822_Subsystem1_Disable ( SimStruct * S ,
DW_Subsystem1_Auto_microgrid_forecasting_020822_T * localDW ) ; void
Auto_microgrid_forecasting_020822_Subsystem1 ( SimStruct * S , uint8_T
rtu_Enable , const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystem1_Auto_microgrid_forecasting_020822_T * localB ,
DW_Subsystem1_Auto_microgrid_forecasting_020822_T * localDW ) ; void
Auto_microgrid_forecasting_020822_AutomaticGainControl_Init ( SimStruct * S ,
B_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localB ,
DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localDW ,
P_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localP ,
X_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localX ) ; void
Auto_microgrid_forecasting_020822_AutomaticGainControl_Deriv ( SimStruct * S
, B_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localB ,
DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localDW ,
P_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localP ,
XDot_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localXdot ) ;
void Auto_microgrid_forecasting_020822_AutomaticGainControl_ZC ( SimStruct *
S , B_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localB ,
DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localDW ,
P_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localP ,
ZCV_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localZCSV ) ;
void Auto_microgrid_forecasting_020822_AutomaticGainControl_Disable (
SimStruct * S , DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T *
localDW ) ; void
Auto_microgrid_forecasting_020822_AutomaticGainControl_Update ( SimStruct * S
, B_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localB ,
DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localDW ,
P_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localP ,
X_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localX ) ; void
Auto_microgrid_forecasting_020822_AutomaticGainControl ( SimStruct * S ,
real_T rtu_Enable , real_T rtu_Freq , real_T rtu_wt , const real_T rtu_Vabc [
3 ] , B_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localB ,
DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localDW ,
P_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localP ,
X_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localX ,
XDis_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localXdis ) ;
void Auto_microgrid_forecasting_020822_AutomaticGainControlTID6 ( SimStruct *
S , B_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localB ,
DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localDW ,
P_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localP ) ; void
Auto_microgrid_forecasting_020822_NegSeqComputation_Init ( SimStruct * S ,
B_NegSeqComputation_Auto_microgrid_forecasting_020822_T * localB ,
P_NegSeqComputation_Auto_microgrid_forecasting_020822_T * localP ) ; void
Auto_microgrid_forecasting_020822_NegSeqComputation ( SimStruct * S , real_T
rtu_Enable , creal_T rtu_In , creal_T rtu_In_i , creal_T rtu_In_m ,
B_NegSeqComputation_Auto_microgrid_forecasting_020822_T * localB ,
DW_NegSeqComputation_Auto_microgrid_forecasting_020822_T * localDW ,
P_NegSeqComputation_Auto_microgrid_forecasting_020822_T * localP ) ; void
Auto_microgrid_forecasting_020822_ZeroSeqComputation_Init ( SimStruct * S ,
B_ZeroSeqComputation_Auto_microgrid_forecasting_020822_T * localB ,
P_ZeroSeqComputation_Auto_microgrid_forecasting_020822_T * localP ) ; void
Auto_microgrid_forecasting_020822_ZeroSeqComputation ( SimStruct * S , real_T
rtu_Enable , creal_T rtu_In , creal_T rtu_In_a , creal_T rtu_In_j ,
B_ZeroSeqComputation_Auto_microgrid_forecasting_020822_T * localB ,
DW_ZeroSeqComputation_Auto_microgrid_forecasting_020822_T * localDW ,
P_ZeroSeqComputation_Auto_microgrid_forecasting_020822_T * localP ) ; void
Auto_microgrid_forecasting_020822_RMS_Init ( SimStruct * S ,
P_RMS_Auto_microgrid_forecasting_020822_T * localP ,
X_RMS_Auto_microgrid_forecasting_020822_T * localX ) ; void
Auto_microgrid_forecasting_020822_RMS_Deriv ( SimStruct * S ,
B_RMS_Auto_microgrid_forecasting_020822_T * localB ,
DW_RMS_Auto_microgrid_forecasting_020822_T * localDW ,
XDot_RMS_Auto_microgrid_forecasting_020822_T * localXdot ) ; void
Auto_microgrid_forecasting_020822_RMS_Disable ( SimStruct * S ,
DW_RMS_Auto_microgrid_forecasting_020822_T * localDW ) ; void
Auto_microgrid_forecasting_020822_RMS ( SimStruct * S , boolean_T rtu_Enable
, const real_T rtu_In [ 3 ] , B_RMS_Auto_microgrid_forecasting_020822_T *
localB , DW_RMS_Auto_microgrid_forecasting_020822_T * localDW ,
P_RMS_Auto_microgrid_forecasting_020822_T * localP ,
XDis_RMS_Auto_microgrid_forecasting_020822_T * localXdis ) ; void
Auto_microgrid_forecasting_020822_TrueRMS_Init ( SimStruct * S ,
DW_TrueRMS_Auto_microgrid_forecasting_020822_T * localDW ,
P_TrueRMS_Auto_microgrid_forecasting_020822_T * localP ,
X_TrueRMS_Auto_microgrid_forecasting_020822_T * localX ) ; void
Auto_microgrid_forecasting_020822_TrueRMS_Deriv ( SimStruct * S ,
B_TrueRMS_Auto_microgrid_forecasting_020822_T * localB ,
DW_TrueRMS_Auto_microgrid_forecasting_020822_T * localDW ,
XDot_TrueRMS_Auto_microgrid_forecasting_020822_T * localXdot ) ; void
Auto_microgrid_forecasting_020822_TrueRMS_ZC ( SimStruct * S ,
B_TrueRMS_Auto_microgrid_forecasting_020822_T * localB ,
DW_TrueRMS_Auto_microgrid_forecasting_020822_T * localDW ,
P_TrueRMS_Auto_microgrid_forecasting_020822_T * localP ,
ZCV_TrueRMS_Auto_microgrid_forecasting_020822_T * localZCSV ) ; void
Auto_microgrid_forecasting_020822_TrueRMS_Disable ( SimStruct * S ,
DW_TrueRMS_Auto_microgrid_forecasting_020822_T * localDW ) ; void
Auto_microgrid_forecasting_020822_TrueRMS_Update ( SimStruct * S ,
B_TrueRMS_Auto_microgrid_forecasting_020822_T * localB ,
DW_TrueRMS_Auto_microgrid_forecasting_020822_T * localDW ,
P_TrueRMS_Auto_microgrid_forecasting_020822_T * localP ) ; void
Auto_microgrid_forecasting_020822_TrueRMS ( SimStruct * S , boolean_T
rtu_Enable , const real_T rtu_In [ 3 ] ,
B_TrueRMS_Auto_microgrid_forecasting_020822_T * localB ,
DW_TrueRMS_Auto_microgrid_forecasting_020822_T * localDW ,
P_TrueRMS_Auto_microgrid_forecasting_020822_T * localP ,
X_TrueRMS_Auto_microgrid_forecasting_020822_T * localX ,
XDis_TrueRMS_Auto_microgrid_forecasting_020822_T * localXdis ) ; void
Auto_microgrid_forecasting_020822_NEGATIVEEdge_Init ( SimStruct * S ,
B_NEGATIVEEdge_Auto_microgrid_forecasting_020822_T * localB ,
P_NEGATIVEEdge_Auto_microgrid_forecasting_020822_T * localP ) ; void
Auto_microgrid_forecasting_020822_NEGATIVEEdge ( SimStruct * S , real_T
rtu_Enable , boolean_T rtu_IN , boolean_T rtu_INprevious ,
B_NEGATIVEEdge_Auto_microgrid_forecasting_020822_T * localB ,
DW_NEGATIVEEdge_Auto_microgrid_forecasting_020822_T * localDW ) ; void
Auto_microgrid_forecasting_020822_POSITIVEEdge_Init ( SimStruct * S ,
B_POSITIVEEdge_Auto_microgrid_forecasting_020822_T * localB ,
P_POSITIVEEdge_Auto_microgrid_forecasting_020822_T * localP ) ; void
Auto_microgrid_forecasting_020822_POSITIVEEdge ( SimStruct * S , real_T
rtu_Enable , boolean_T rtu_IN , boolean_T rtu_INprevious ,
B_POSITIVEEdge_Auto_microgrid_forecasting_020822_T * localB ,
DW_POSITIVEEdge_Auto_microgrid_forecasting_020822_T * localDW ) ; void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem_Enable (
SimStruct * S ) ; void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem_Disable (
SimStruct * S ) ; void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem ( SimStruct * S ,
real_T rtu_u_frt , boolean_T * rty_Out1 , boolean_T * rty_Output ,
B_SwitchCaseActionSubsystem_Auto_microgrid_forecasting_020822_T * localB ) ;
void Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystemTID6 (
SimStruct * S ,
B_SwitchCaseActionSubsystem_Auto_microgrid_forecasting_020822_T * localB ,
P_SwitchCaseActionSubsystem_Auto_microgrid_forecasting_020822_T * localP ) ;
void Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2_Enable (
SimStruct * S ) ; void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2_Disable (
SimStruct * S ) ; void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2 ( SimStruct * S
, real_T rtu_u_frt , boolean_T * rty_Out1 ,
B_SwitchCaseActionSubsystem2_Auto_microgrid_forecasting_020822_T * localB ) ;
void Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2TID6 (
SimStruct * S ,
B_SwitchCaseActionSubsystem2_Auto_microgrid_forecasting_020822_T * localB ,
P_SwitchCaseActionSubsystem2_Auto_microgrid_forecasting_020822_T * localP ) ;
void Auto_microgrid_forecasting_020822_Subsystempi2delay_i_Init ( SimStruct *
S , B_Subsystempi2delay_Auto_microgrid_forecasting_020822_h_T * localB ,
P_Subsystempi2delay_Auto_microgrid_forecasting_020822_f_T * localP ) ; void
Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( SimStruct * S ,
uint8_T rtu_Enable , const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_Auto_microgrid_forecasting_020822_h_T * localB ,
DW_Subsystempi2delay_Auto_microgrid_forecasting_020822_e_T * localDW ) ; void
Auto_microgrid_forecasting_020822_Subsystem1_k_Init ( SimStruct * S ,
B_Subsystem1_Auto_microgrid_forecasting_020822_c_T * localB ,
P_Subsystem1_Auto_microgrid_forecasting_020822_n_T * localP ) ; void
Auto_microgrid_forecasting_020822_Subsystem1_h ( SimStruct * S , uint8_T
rtu_Enable , const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystem1_Auto_microgrid_forecasting_020822_c_T * localB ,
DW_Subsystem1_Auto_microgrid_forecasting_020822_j_T * localDW ) ; void
Auto_microgrid_forecasting_020822_CounterASTfmax_Init ( SimStruct * S ,
B_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localB ,
DW_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localDW ,
P_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localP ) ; void
Auto_microgrid_forecasting_020822_CounterASTfmax ( SimStruct * S , boolean_T
rtu_Trigger , B_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localB ,
DW_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localDW ,
ZCE_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localZCE ) ; void
Auto_microgrid_forecasting_020822_CounterASTfmaxTID6 ( SimStruct * S ,
B_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localB ,
P_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localP ) ;
#endif
