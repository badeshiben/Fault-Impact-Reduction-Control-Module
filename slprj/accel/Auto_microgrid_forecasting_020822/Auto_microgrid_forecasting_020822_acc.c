#if 0
This file is not available for use in any application other than as a MATLAB
( R ) MEX file for use with the Simulink ( R ) product . If you do not have
the Simulink Coder licensed , this file contains encrypted block names , etc
. If you purchase the Simulink Coder , this file will contain full block
descriptions and improved variable names .
#endif
#include <math.h>
#include "Auto_microgrid_forecasting_020822_acc.h"
#include "Auto_microgrid_forecasting_020822_acc_private.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simtarget/slSimTgtMdlrefSfcnBridge.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T Auto_microgrid_forecasting_020822_acc_rt_TDelayUpdateTailOrGrowBuf
( int_T * bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr ,
real_T tMinusDelay , real_T * * tBufPtr , real_T * * uBufPtr , real_T * *
xBufPtr , boolean_T isfixedbuf , boolean_T istransportdelay , int_T *
maxNewBufSzPtr ) { int_T testIdx ; int_T tail = * tailPtr ; int_T bufSz = *
bufSzPtr ; real_T * tBuf = * tBufPtr ; real_T * xBuf = ( NULL ) ; int_T
numBuffer = 2 ; if ( istransportdelay ) { numBuffer = 3 ; xBuf = * xBufPtr ;
} testIdx = ( tail < ( bufSz - 1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay
<= tBuf [ testIdx ] ) && ! isfixedbuf ) { int_T j ; real_T * tempT ; real_T *
tempU ; real_T * tempX = ( NULL ) ; real_T * uBuf = * uBufPtr ; int_T
newBufSz = bufSz + 1024 ; if ( newBufSz > * maxNewBufSzPtr ) { *
maxNewBufSzPtr = newBufSz ; } tempU = ( real_T * ) utMalloc ( numBuffer *
newBufSz * sizeof ( real_T ) ) ; if ( tempU == ( NULL ) ) { return ( false )
; } tempT = tempU + newBufSz ; if ( istransportdelay ) tempX = tempT +
newBufSz ; for ( j = tail ; j < bufSz ; j ++ ) { tempT [ j - tail ] = tBuf [
j ] ; tempU [ j - tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j -
tail ] = xBuf [ j ] ; } for ( j = 0 ; j < tail ; j ++ ) { tempT [ j + bufSz -
tail ] = tBuf [ j ] ; tempU [ j + bufSz - tail ] = uBuf [ j ] ; if (
istransportdelay ) tempX [ j + bufSz - tail ] = xBuf [ j ] ; } if ( * lastPtr
> tail ) { * lastPtr -= tail ; } else { * lastPtr += ( bufSz - tail ) ; } *
tailPtr = 0 ; * headPtr = bufSz ; utFree ( uBuf ) ; * bufSzPtr = newBufSz ; *
tBufPtr = tempT ; * uBufPtr = tempU ; if ( istransportdelay ) * xBufPtr =
tempX ; } else { * tailPtr = testIdx ; } return ( true ) ; } real_T
Auto_microgrid_forecasting_020822_acc_rt_VTDelayfindtDInterpolate ( real_T x
, real_T * tBuf , real_T * uBuf , real_T * xBuf , int_T bufSz , int_T head ,
int_T tail , int_T * pLast , real_T t , real_T tStart , boolean_T discrete ,
boolean_T minorStepAndTAtLastMajorOutput , real_T initOutput , real_T *
appliedDelay ) { int_T n , k ; real_T f ; int_T kp1 ; real_T tminustD , tL ,
tR , uD , uL , uR , fU ; if ( minorStepAndTAtLastMajorOutput ) { if ( * pLast
== head ) { * pLast = ( * pLast == 0 ) ? bufSz - 1 : * pLast - 1 ; } head = (
head == 0 ) ? bufSz - 1 : head - 1 ; } if ( x <= 1 ) { return initOutput ; }
k = * pLast ; n = 0 ; for ( ; ; ) { n ++ ; if ( n > bufSz ) break ; if ( x -
xBuf [ k ] > 1.0 ) { if ( k == head ) { int_T km1 ; f = ( x - 1.0 - xBuf [ k
] ) / ( x - xBuf [ k ] ) ; tminustD = ( 1.0 - f ) * tBuf [ k ] + f * t ; km1
= k - 1 ; if ( km1 < 0 ) km1 = bufSz - 1 ; tL = tBuf [ km1 ] ; tR = tBuf [ k
] ; uL = uBuf [ km1 ] ; uR = uBuf [ k ] ; break ; } kp1 = k + 1 ; if ( kp1 ==
bufSz ) kp1 = 0 ; if ( x - xBuf [ kp1 ] <= 1.0 ) { f = ( x - 1.0 - xBuf [ k ]
) / ( xBuf [ kp1 ] - xBuf [ k ] ) ; tL = tBuf [ k ] ; tR = tBuf [ kp1 ] ; uL
= uBuf [ k ] ; uR = uBuf [ kp1 ] ; tminustD = ( 1.0 - f ) * tL + f * tR ;
break ; } k = kp1 ; } else { if ( k == tail ) { f = ( x - 1.0 ) / xBuf [ k ]
; if ( discrete ) { return ( uBuf [ tail ] ) ; } kp1 = k + 1 ; if ( kp1 ==
bufSz ) kp1 = 0 ; tminustD = ( 1 - f ) * tStart + f * tBuf [ k ] ; tL = tBuf
[ k ] ; tR = tBuf [ kp1 ] ; uL = uBuf [ k ] ; uR = uBuf [ kp1 ] ; break ; } k
= k - 1 ; if ( k < 0 ) k = bufSz - 1 ; } } * pLast = k ; if ( tR == tL ) { fU
= 1.0 ; } else { fU = ( tminustD - tL ) / ( tR - tL ) ; } if ( discrete ) {
uD = ( fU > ( 1.0 - fU ) ) ? uR : uL ; } else { uD = ( 1.0 - fU ) * uL + fU *
uR ; } * appliedDelay = t - tminustD ; return uD ; } real_T
Auto_microgrid_forecasting_020822_acc_rt_TDelayInterpolate ( real_T
tMinusDelay , real_T tStart , real_T * tBuf , real_T * uBuf , int_T bufSz ,
int_T * lastIdx , int_T oldestIdx , int_T newIdx , real_T initOutput ,
boolean_T discrete , boolean_T minorStepAndTAtLastMajorOutput ) { int_T i ;
real_T yout , t1 , t2 , u1 , u2 ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 )
&& ( tMinusDelay > tStart ) ) return initOutput ; if ( tMinusDelay <= tStart
) return initOutput ; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if (
discrete ) { return ( uBuf [ oldestIdx ] ) ; } else { int_T tempIdx =
oldestIdx + 1 ; if ( oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [
oldestIdx ] ; t2 = tBuf [ tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [
tempIdx ] ; if ( t2 == t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else {
yout = u1 ; } } else { real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ;
real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2 ; } return yout ; } } if (
minorStepAndTAtLastMajorOutput ) { if ( newIdx != 0 ) { if ( * lastIdx ==
newIdx ) { ( * lastIdx ) -- ; } newIdx -- ; } else { if ( * lastIdx == newIdx
) { * lastIdx = bufSz - 1 ; } newIdx = bufSz - 1 ; } } i = * lastIdx ; if (
tBuf [ i ] < tMinusDelay ) { while ( tBuf [ i ] < tMinusDelay ) { if ( i ==
newIdx ) break ; i = ( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while
( tBuf [ i ] >= tMinusDelay ) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i =
( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) {
double tempEps = ( DBL_EPSILON ) * 128.0 ; double localEps = tempEps *
muDoubleScalarAbs ( tBuf [ i ] ) ; if ( tempEps > localEps ) { localEps =
tempEps ; } localEps = localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] -
localEps ) ) { yout = uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [
bufSz - 1 ] ; } else { yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) {
t1 = tBuf [ bufSz - 1 ] ; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i -
1 ] ; u1 = uBuf [ i - 1 ] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 ==
t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } } return ( yout ) ; } real_T look1_binlxpw ( real_T
u0 , const real_T bp0 [ ] , const real_T table [ ] , uint32_T maxIndex ) {
real_T frac ; uint32_T iRght ; uint32_T iLeft ; uint32_T bpIdx ; if ( u0 <=
bp0 [ 0U ] ) { iLeft = 0U ; frac = ( u0 - bp0 [ 0U ] ) / ( bp0 [ 1U ] - bp0 [
0U ] ) ; } else if ( u0 < bp0 [ maxIndex ] ) { bpIdx = maxIndex >> 1U ; iLeft
= 0U ; iRght = maxIndex ; while ( iRght - iLeft > 1U ) { if ( u0 < bp0 [
bpIdx ] ) { iRght = bpIdx ; } else { iLeft = bpIdx ; } bpIdx = ( iRght +
iLeft ) >> 1U ; } frac = ( u0 - bp0 [ iLeft ] ) / ( bp0 [ iLeft + 1U ] - bp0
[ iLeft ] ) ; } else { iLeft = maxIndex - 1U ; frac = ( u0 - bp0 [ maxIndex -
1U ] ) / ( bp0 [ maxIndex ] - bp0 [ maxIndex - 1U ] ) ; } return ( table [
iLeft + 1U ] - table [ iLeft ] ) * frac + table [ iLeft ] ; } real_T
look2_binlxpw ( real_T u0 , real_T u1 , const real_T bp0 [ ] , const real_T
bp1 [ ] , const real_T table [ ] , const uint32_T maxIndex [ ] , uint32_T
stride ) { real_T frac ; uint32_T bpIndices [ 2 ] ; real_T fractions [ 2 ] ;
real_T yL_1d ; uint32_T iRght ; uint32_T bpIdx ; uint32_T iLeft ; if ( u0 <=
bp0 [ 0U ] ) { iLeft = 0U ; frac = ( u0 - bp0 [ 0U ] ) / ( bp0 [ 1U ] - bp0 [
0U ] ) ; } else if ( u0 < bp0 [ maxIndex [ 0U ] ] ) { bpIdx = maxIndex [ 0U ]
>> 1U ; iLeft = 0U ; iRght = maxIndex [ 0U ] ; while ( iRght - iLeft > 1U ) {
if ( u0 < bp0 [ bpIdx ] ) { iRght = bpIdx ; } else { iLeft = bpIdx ; } bpIdx
= ( iRght + iLeft ) >> 1U ; } frac = ( u0 - bp0 [ iLeft ] ) / ( bp0 [ iLeft +
1U ] - bp0 [ iLeft ] ) ; } else { iLeft = maxIndex [ 0U ] - 1U ; frac = ( u0
- bp0 [ maxIndex [ 0U ] - 1U ] ) / ( bp0 [ maxIndex [ 0U ] ] - bp0 [ maxIndex
[ 0U ] - 1U ] ) ; } fractions [ 0U ] = frac ; bpIndices [ 0U ] = iLeft ; if (
u1 <= bp1 [ 0U ] ) { iLeft = 0U ; frac = ( u1 - bp1 [ 0U ] ) / ( bp1 [ 1U ] -
bp1 [ 0U ] ) ; } else if ( u1 < bp1 [ maxIndex [ 1U ] ] ) { bpIdx = maxIndex
[ 1U ] >> 1U ; iLeft = 0U ; iRght = maxIndex [ 1U ] ; while ( iRght - iLeft >
1U ) { if ( u1 < bp1 [ bpIdx ] ) { iRght = bpIdx ; } else { iLeft = bpIdx ; }
bpIdx = ( iRght + iLeft ) >> 1U ; } frac = ( u1 - bp1 [ iLeft ] ) / ( bp1 [
iLeft + 1U ] - bp1 [ iLeft ] ) ; } else { iLeft = maxIndex [ 1U ] - 1U ; frac
= ( u1 - bp1 [ maxIndex [ 1U ] - 1U ] ) / ( bp1 [ maxIndex [ 1U ] ] - bp1 [
maxIndex [ 1U ] - 1U ] ) ; } bpIdx = iLeft * stride + bpIndices [ 0U ] ;
yL_1d = ( table [ bpIdx + 1U ] - table [ bpIdx ] ) * fractions [ 0U ] + table
[ bpIdx ] ; bpIdx += stride ; return ( ( ( table [ bpIdx + 1U ] - table [
bpIdx ] ) * fractions [ 0U ] + table [ bpIdx ] ) - yL_1d ) * frac + yL_1d ; }
real_T look1_pbinlxpw ( real_T u0 , const real_T bp0 [ ] , const real_T table
[ ] , uint32_T prevIndex [ ] , uint32_T maxIndex ) { real_T frac ; uint32_T
iRght ; uint32_T iLeft ; uint32_T bpIdx ; uint32_T found ; if ( u0 <= bp0 [
0U ] ) { bpIdx = 0U ; frac = ( u0 - bp0 [ 0U ] ) / ( bp0 [ 1U ] - bp0 [ 0U ]
) ; } else if ( u0 < bp0 [ maxIndex ] ) { bpIdx = prevIndex [ 0U ] ; iLeft =
0U ; iRght = maxIndex ; found = 0U ; while ( found == 0U ) { if ( u0 < bp0 [
bpIdx ] ) { iRght = bpIdx - 1U ; bpIdx = ( iRght + iLeft ) >> 1U ; } else if
( u0 < bp0 [ bpIdx + 1U ] ) { found = 1U ; } else { iLeft = bpIdx + 1U ;
bpIdx = ( iRght + iLeft ) >> 1U ; } } frac = ( u0 - bp0 [ bpIdx ] ) / ( bp0 [
bpIdx + 1U ] - bp0 [ bpIdx ] ) ; } else { bpIdx = maxIndex - 1U ; frac = ( u0
- bp0 [ maxIndex - 1U ] ) / ( bp0 [ maxIndex ] - bp0 [ maxIndex - 1U ] ) ; }
prevIndex [ 0U ] = bpIdx ; return ( table [ bpIdx + 1U ] - table [ bpIdx ] )
* frac + table [ bpIdx ] ; } void rt_ssGetBlockPath ( SimStruct * S , int_T
sysIdx , int_T blkIdx , char_T * * path ) { _ssGetBlockPath ( S , sysIdx ,
blkIdx , path ) ; } void rt_ssSet_slErrMsg ( SimStruct * S , void * diag ) {
if ( ! _ssIsErrorStatusAslErrMsg ( S ) ) { _ssSet_slErrMsg ( S , diag ) ; }
else { _ssDiscardDiagnostic ( S , diag ) ; } } void
rt_ssReportDiagnosticAsWarning ( SimStruct * S , void * diag ) {
_ssReportDiagnosticAsWarning ( S , diag ) ; } void
Auto_microgrid_forecasting_020822_Subsystempi2delay_Init ( SimStruct * S ,
B_Subsystempi2delay_Auto_microgrid_forecasting_020822_T * localB ,
P_Subsystempi2delay_Auto_microgrid_forecasting_020822_T * localP ) { localB
-> B_65_0_0 = localP -> P_0 [ 0 ] ; localB -> B_65_1_0 = localP -> P_0 [ 1 ]
; } void Auto_microgrid_forecasting_020822_Subsystempi2delay_Disable (
SimStruct * S , DW_Subsystempi2delay_Auto_microgrid_forecasting_020822_T *
localDW ) { localDW -> Subsystempi2delay_MODE = false ; } void
Auto_microgrid_forecasting_020822_Subsystempi2delay ( SimStruct * S , uint8_T
rtu_Enable , const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystempi2delay_Auto_microgrid_forecasting_020822_T * localB ,
DW_Subsystempi2delay_Auto_microgrid_forecasting_020822_T * localDW ) {
int32_T isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) &&
( ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable > 0 ) { if ( ! localDW
-> Subsystempi2delay_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S
) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
Subsystempi2delay_MODE = true ; } } else { if ( localDW ->
Subsystempi2delay_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
Auto_microgrid_forecasting_020822_Subsystempi2delay_Disable ( S , localDW ) ;
} } } if ( localDW -> Subsystempi2delay_MODE ) { localB -> B_65_0_0 =
rtu_alpha_beta [ 0 ] * muDoubleScalarSin ( rtu_wt ) - rtu_alpha_beta [ 1 ] *
muDoubleScalarCos ( rtu_wt ) ; localB -> B_65_1_0 = rtu_alpha_beta [ 0 ] *
muDoubleScalarCos ( rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarSin (
rtu_wt ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
Subsystempi2delay_SubsysRanBC ) ; } } } void
Auto_microgrid_forecasting_020822_Subsystempi2delay_Term ( SimStruct * const
S ) { } void Auto_microgrid_forecasting_020822_Subsystem1_Init ( SimStruct *
S , B_Subsystem1_Auto_microgrid_forecasting_020822_T * localB ,
P_Subsystem1_Auto_microgrid_forecasting_020822_T * localP ) { localB ->
B_66_0_0 = localP -> P_0 [ 0 ] ; localB -> B_66_1_0 = localP -> P_0 [ 1 ] ; }
void Auto_microgrid_forecasting_020822_Subsystem1_Disable ( SimStruct * S ,
DW_Subsystem1_Auto_microgrid_forecasting_020822_T * localDW ) { localDW ->
Subsystem1_MODE = false ; } void Auto_microgrid_forecasting_020822_Subsystem1
( SimStruct * S , uint8_T rtu_Enable , const real_T rtu_alpha_beta [ 2 ] ,
real_T rtu_wt , B_Subsystem1_Auto_microgrid_forecasting_020822_T * localB ,
DW_Subsystem1_Auto_microgrid_forecasting_020822_T * localDW ) { int32_T isHit
; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable > 0 ) { if ( ! localDW ->
Subsystem1_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } localDW ->
Subsystem1_MODE = true ; } } else { if ( localDW -> Subsystem1_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
Auto_microgrid_forecasting_020822_Subsystem1_Disable ( S , localDW ) ; } } }
if ( localDW -> Subsystem1_MODE ) { localB -> B_66_0_0 = rtu_alpha_beta [ 0 ]
* muDoubleScalarCos ( rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarSin (
rtu_wt ) ; localB -> B_66_1_0 = - rtu_alpha_beta [ 0 ] * muDoubleScalarSin (
rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarCos ( rtu_wt ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
Subsystem1_SubsysRanBC ) ; } } } void
Auto_microgrid_forecasting_020822_Subsystem1_Term ( SimStruct * const S ) { }
void Auto_microgrid_forecasting_020822_AutomaticGainControl_Init ( SimStruct
* S , B_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localB ,
DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localDW ,
P_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localP ,
X_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localX ) {
localX -> VariableTransportDelay_CSTATE_l = 0.0 ; localX ->
integrator_CSTATE_k = localP -> P_3 ; localDW -> Memory_PreviousInput =
localP -> P_4 ; localX -> VariableTransportDelay_CSTATE_c = 0.0 ; localX ->
integrator_CSTATE_p = localP -> P_7 ; localDW -> Memory_PreviousInput_i =
localP -> P_8 ; Auto_microgrid_forecasting_020822_Subsystempi2delay_Init ( S
, & localB -> Subsystempi2delay , & localP -> Subsystempi2delay ) ;
Auto_microgrid_forecasting_020822_Subsystem1_Init ( S , & localB ->
Subsystem1 , & localP -> Subsystem1 ) ; localB -> B_67_17_0 = localP -> P_0 ;
} void Auto_microgrid_forecasting_020822_AutomaticGainControl_Disable (
SimStruct * S , DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T *
localDW ) { if ( localDW -> Subsystempi2delay . Subsystempi2delay_MODE ) {
Auto_microgrid_forecasting_020822_Subsystempi2delay_Disable ( S , & localDW
-> Subsystempi2delay ) ; } if ( localDW -> Subsystem1 . Subsystem1_MODE ) {
Auto_microgrid_forecasting_020822_Subsystem1_Disable ( S , & localDW ->
Subsystem1 ) ; } localDW -> AutomaticGainControl_MODE = false ; } void
Auto_microgrid_forecasting_020822_AutomaticGainControl ( SimStruct * S ,
real_T rtu_Enable , real_T rtu_Freq , real_T rtu_wt , const real_T rtu_Vabc [
3 ] , B_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localB ,
DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localDW ,
P_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localP ,
X_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localX ,
XDis_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localXdis ) {
int32_T isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) &&
( ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable > 0.0 ) { if ( ! localDW
-> AutomaticGainControl_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart
( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( void )
memset ( & ( localXdis -> VariableTransportDelay_CSTATE_l ) , 0 , 4 * sizeof
( boolean_T ) ) ; localDW -> AutomaticGainControl_MODE = true ; } } else { if
( localDW -> AutomaticGainControl_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> VariableTransportDelay_CSTATE_l ) , 1 , 4 * sizeof ( boolean_T )
) ; Auto_microgrid_forecasting_020822_AutomaticGainControl_Disable ( S ,
localDW ) ; } } } if ( localDW -> AutomaticGainControl_MODE ) { { real_T * *
uBuffer = ( real_T * * ) & localDW -> VariableTransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & localDW ->
VariableTransportDelay_PWORK . TUbufferPtrs [ 1 ] ; real_T * * xBuffer = (
real_T * * ) & localDW -> VariableTransportDelay_PWORK . TUbufferPtrs [ 2 ] ;
real_T simTime = ssGetT ( S ) ; real_T appliedDelay ; localB -> B_67_0_0 =
Auto_microgrid_forecasting_020822_acc_rt_VTDelayfindtDInterpolate ( localX ->
VariableTransportDelay_CSTATE_l , * tBuffer , * uBuffer , * xBuffer , localDW
-> VariableTransportDelay_IWORK . CircularBufSize , localDW ->
VariableTransportDelay_IWORK . Head , localDW -> VariableTransportDelay_IWORK
. Tail , & localDW -> VariableTransportDelay_IWORK . Last , simTime , 0.0 , 0
, ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) , localP -> P_2 , & appliedDelay ) ; } localB -> B_67_1_0 =
localX -> integrator_CSTATE_k ; localB -> B_67_2_0 = ssGetT ( S ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( ssIsMajorTimeStep ( S
) != 0 ) { localDW -> RelationalOperator_Mode = ( localB -> B_67_2_0 >=
localB -> B_67_0_0_m ) ; } localB -> B_67_3_0 = localDW ->
RelationalOperator_Mode ; localB -> B_67_4_0 = localDW ->
Memory_PreviousInput ; } if ( localB -> B_67_3_0 ) { localB -> B_63_0_0 =
localB -> B_67_1_0 - localB -> B_67_0_0 ; localB -> B_63_1_0 = localB ->
B_63_0_0 * rtu_Freq ; localB -> B_67_6_0 = localB -> B_63_1_0 ; } else {
localB -> B_67_6_0 = localB -> B_67_4_0 ; } { real_T * * uBuffer = ( real_T *
* ) & localDW -> VariableTransportDelay_PWORK_e . TUbufferPtrs [ 0 ] ; real_T
* * tBuffer = ( real_T * * ) & localDW -> VariableTransportDelay_PWORK_e .
TUbufferPtrs [ 1 ] ; real_T * * xBuffer = ( real_T * * ) & localDW ->
VariableTransportDelay_PWORK_e . TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT
( S ) ; real_T appliedDelay ; localB -> B_67_7_0 =
Auto_microgrid_forecasting_020822_acc_rt_VTDelayfindtDInterpolate ( localX ->
VariableTransportDelay_CSTATE_c , * tBuffer , * uBuffer , * xBuffer , localDW
-> VariableTransportDelay_IWORK_f . CircularBufSize , localDW ->
VariableTransportDelay_IWORK_f . Head , localDW ->
VariableTransportDelay_IWORK_f . Tail , & localDW ->
VariableTransportDelay_IWORK_f . Last , simTime , 0.0 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
, localP -> P_6 , & appliedDelay ) ; } localB -> B_67_8_0 = localX ->
integrator_CSTATE_p ; localB -> B_67_9_0 = ssGetT ( S ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { if ( ssIsMajorTimeStep ( S
) != 0 ) { localDW -> RelationalOperator_Mode_l = ( localB -> B_67_9_0 >=
localB -> B_67_1_0_c ) ; } localB -> B_67_10_0 = localDW ->
RelationalOperator_Mode_l ; localB -> B_67_11_0 = localDW ->
Memory_PreviousInput_i ; } if ( localB -> B_67_10_0 ) { localB -> B_64_0_0 =
localB -> B_67_8_0 - localB -> B_67_7_0 ; localB -> B_64_1_0 = localB ->
B_64_0_0 * rtu_Freq ; localB -> B_67_13_0 = localB -> B_64_1_0 ; } else {
localB -> B_67_13_0 = localB -> B_67_11_0 ; } localB -> B_67_14_0 . re =
localB -> B_67_6_0 ; localB -> B_67_14_0 . im = localB -> B_67_13_0 ; localB
-> B_67_15_0 = muDoubleScalarHypot ( localB -> B_67_14_0 . re , localB ->
B_67_14_0 . im ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { localDW ->
Saturation_MODE = localB -> B_67_15_0 >= localP -> P_9 ? 1 : localB ->
B_67_15_0 > localP -> P_10 ? 0 : - 1 ; } localB -> B_67_16_0 = localDW ->
Saturation_MODE == 1 ? localP -> P_9 : localDW -> Saturation_MODE == - 1 ?
localP -> P_10 : localB -> B_67_15_0 ; localB -> B_67_17_0 = 1.0 / localB ->
B_67_16_0 ; if ( rtu_Freq > localP -> P_11 ) { localB -> B_67_18_0 = localP
-> P_11 ; } else if ( rtu_Freq < localP -> P_12 ) { localB -> B_67_18_0 =
localP -> P_12 ; } else { localB -> B_67_18_0 = rtu_Freq ; } localB ->
B_67_19_0 = 1.0 / localB -> B_67_18_0 ; if ( rtu_Freq > localP -> P_13 ) {
localB -> B_67_20_0 = localP -> P_13 ; } else if ( rtu_Freq < localP -> P_14
) { localB -> B_67_20_0 = localP -> P_14 ; } else { localB -> B_67_20_0 =
rtu_Freq ; } localB -> B_67_21_0 = 1.0 / localB -> B_67_20_0 ; for ( isHit =
0 ; isHit < 3 ; isHit ++ ) { localB -> B_67_22_0 [ isHit ] = 0.0 ; localB ->
B_67_22_0 [ isHit ] += localP -> P_15 [ isHit ] * rtu_Vabc [ 0 ] ; localB ->
B_67_22_0 [ isHit ] += localP -> P_15 [ isHit + 3 ] * rtu_Vabc [ 1 ] ; localB
-> B_67_22_0 [ isHit ] += localP -> P_15 [ isHit + 6 ] * rtu_Vabc [ 2 ] ; }
localB -> B_67_23_0 [ 0 ] = localP -> P_16 * localB -> B_67_22_0 [ 0 ] ;
localB -> B_67_23_0 [ 1 ] = localP -> P_16 * localB -> B_67_22_0 [ 1 ] ;
localB -> B_67_23_0 [ 2 ] = localP -> P_16 * localB -> B_67_22_0 [ 2 ] ;
Auto_microgrid_forecasting_020822_Subsystempi2delay ( S , localB ->
B_67_6_0_c , & localB -> B_67_23_0 [ 0 ] , rtu_wt , & localB ->
Subsystempi2delay , & localDW -> Subsystempi2delay ) ;
Auto_microgrid_forecasting_020822_Subsystem1 ( S , localB -> B_67_4_0_k , &
localB -> B_67_23_0 [ 0 ] , rtu_wt , & localB -> Subsystem1 , & localDW ->
Subsystem1 ) ; if ( localB -> B_67_4_0_k != 0 ) { localB -> B_67_26_0 [ 0 ] =
localB -> Subsystem1 . B_66_0_0 ; localB -> B_67_26_0 [ 1 ] = localB ->
Subsystem1 . B_66_1_0 ; } else { localB -> B_67_26_0 [ 0 ] = localB ->
Subsystempi2delay . B_65_0_0 ; localB -> B_67_26_0 [ 1 ] = localB ->
Subsystempi2delay . B_65_1_0 ; } if ( ssIsMajorTimeStep ( S ) != 0 ) {
srUpdateBC ( localDW -> AutomaticGainControl_SubsysRanBC ) ; } } } void
Auto_microgrid_forecasting_020822_AutomaticGainControlTID6 ( SimStruct * S ,
B_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localB ,
DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localDW ,
P_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localP ) {
localB -> B_67_0_0_m = localP -> P_17 ; localB -> B_67_1_0_c = localP -> P_18
; localB -> B_67_4_0_k = ( uint8_T ) ( localP -> P_19 == localP -> P_20 ) ;
localB -> B_67_6_0_c = ( uint8_T ) ( localP -> P_19 == localP -> P_21 ) ; if
( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
AutomaticGainControl_SubsysRanBC ) ; } } void
Auto_microgrid_forecasting_020822_AutomaticGainControl_Update ( SimStruct * S
, B_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localB ,
DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localDW ,
P_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localP ,
X_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localX ) {
int32_T isHit ; if ( localDW -> AutomaticGainControl_MODE ) { { real_T * *
uBuffer = ( real_T * * ) & localDW -> VariableTransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T * * ) & localDW ->
VariableTransportDelay_PWORK . TUbufferPtrs [ 1 ] ; real_T * * xBuffer = (
real_T * * ) & localDW -> VariableTransportDelay_PWORK . TUbufferPtrs [ 2 ] ;
real_T simTime = ssGetT ( S ) ; localDW -> VariableTransportDelay_IWORK .
Head = ( ( localDW -> VariableTransportDelay_IWORK . Head < ( localDW ->
VariableTransportDelay_IWORK . CircularBufSize - 1 ) ) ? ( localDW ->
VariableTransportDelay_IWORK . Head + 1 ) : 0 ) ; if ( localDW ->
VariableTransportDelay_IWORK . Head == localDW ->
VariableTransportDelay_IWORK . Tail ) { if ( !
Auto_microgrid_forecasting_020822_acc_rt_TDelayUpdateTailOrGrowBuf ( &
localDW -> VariableTransportDelay_IWORK . CircularBufSize , & localDW ->
VariableTransportDelay_IWORK . Tail , & localDW ->
VariableTransportDelay_IWORK . Head , & localDW ->
VariableTransportDelay_IWORK . Last , simTime - localP -> P_1 , tBuffer ,
uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 1 , & localDW ->
VariableTransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ localDW ->
VariableTransportDelay_IWORK . Head ] = simTime ; ( * uBuffer ) [ localDW ->
VariableTransportDelay_IWORK . Head ] = localB -> B_67_1_0 ; ( * xBuffer ) [
localDW -> VariableTransportDelay_IWORK . Head ] = localX ->
VariableTransportDelay_CSTATE_l ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { localDW -> Memory_PreviousInput = localB -> B_67_6_0 ; } {
real_T * * uBuffer = ( real_T * * ) & localDW ->
VariableTransportDelay_PWORK_e . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & localDW -> VariableTransportDelay_PWORK_e . TUbufferPtrs [ 1 ]
; real_T * * xBuffer = ( real_T * * ) & localDW ->
VariableTransportDelay_PWORK_e . TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT
( S ) ; localDW -> VariableTransportDelay_IWORK_f . Head = ( ( localDW ->
VariableTransportDelay_IWORK_f . Head < ( localDW ->
VariableTransportDelay_IWORK_f . CircularBufSize - 1 ) ) ? ( localDW ->
VariableTransportDelay_IWORK_f . Head + 1 ) : 0 ) ; if ( localDW ->
VariableTransportDelay_IWORK_f . Head == localDW ->
VariableTransportDelay_IWORK_f . Tail ) { if ( !
Auto_microgrid_forecasting_020822_acc_rt_TDelayUpdateTailOrGrowBuf ( &
localDW -> VariableTransportDelay_IWORK_f . CircularBufSize , & localDW ->
VariableTransportDelay_IWORK_f . Tail , & localDW ->
VariableTransportDelay_IWORK_f . Head , & localDW ->
VariableTransportDelay_IWORK_f . Last , simTime - localP -> P_5 , tBuffer ,
uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 1 , & localDW ->
VariableTransportDelay_IWORK_f . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ localDW ->
VariableTransportDelay_IWORK_f . Head ] = simTime ; ( * uBuffer ) [ localDW
-> VariableTransportDelay_IWORK_f . Head ] = localB -> B_67_8_0 ; ( * xBuffer
) [ localDW -> VariableTransportDelay_IWORK_f . Head ] = localX ->
VariableTransportDelay_CSTATE_c ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { localDW -> Memory_PreviousInput_i = localB -> B_67_13_0 ; }
} } void Auto_microgrid_forecasting_020822_AutomaticGainControl_Deriv (
SimStruct * S , B_AutomaticGainControl_Auto_microgrid_forecasting_020822_T *
localB , DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T *
localDW , P_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localP
, XDot_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localXdot )
{ if ( localDW -> AutomaticGainControl_MODE ) { { real_T instantDelay ;
instantDelay = localB -> B_67_19_0 ; if ( instantDelay > localP -> P_1 ) {
instantDelay = localP -> P_1 ; } if ( instantDelay < 0.0 ) { localXdot ->
VariableTransportDelay_CSTATE_l = 0 ; } else { localXdot ->
VariableTransportDelay_CSTATE_l = 1.0 / instantDelay ; } } localXdot ->
integrator_CSTATE_k = localB -> B_67_26_0 [ 0 ] ; { real_T instantDelay ;
instantDelay = localB -> B_67_21_0 ; if ( instantDelay > localP -> P_5 ) {
instantDelay = localP -> P_5 ; } if ( instantDelay < 0.0 ) { localXdot ->
VariableTransportDelay_CSTATE_c = 0 ; } else { localXdot ->
VariableTransportDelay_CSTATE_c = 1.0 / instantDelay ; } } localXdot ->
integrator_CSTATE_p = localB -> B_67_26_0 [ 1 ] ; } else { { real_T * dx ;
int_T i ; dx = & ( localXdot -> VariableTransportDelay_CSTATE_l ) ; for ( i =
0 ; i < 4 ; i ++ ) { dx [ i ] = 0.0 ; } } } } void
Auto_microgrid_forecasting_020822_AutomaticGainControl_ZC ( SimStruct * S ,
B_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localB ,
DW_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localDW ,
P_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localP ,
ZCV_AutomaticGainControl_Auto_microgrid_forecasting_020822_T * localZCSV ) {
if ( localDW -> AutomaticGainControl_MODE ) { localZCSV ->
RelationalOperator_RelopInput_ZC_d = localB -> B_67_2_0 - localB ->
B_67_0_0_m ; localZCSV -> RelationalOperator_RelopInput_ZC_f = localB ->
B_67_9_0 - localB -> B_67_1_0_c ; localZCSV -> Saturation_UprLim_ZC_b =
localB -> B_67_15_0 - localP -> P_9 ; localZCSV -> Saturation_LwrLim_ZC_h =
localB -> B_67_15_0 - localP -> P_10 ; } else { { real_T * zcsv = & (
localZCSV -> RelationalOperator_RelopInput_ZC_d ) ; int_T i ; for ( i = 0 ; i
< 4 ; i ++ ) { zcsv [ i ] = 0.0 ; } } } } void
Auto_microgrid_forecasting_020822_AutomaticGainControl_Term ( SimStruct *
const S ) { } void Auto_microgrid_forecasting_020822_NegSeqComputation_Init (
SimStruct * S , B_NegSeqComputation_Auto_microgrid_forecasting_020822_T *
localB , P_NegSeqComputation_Auto_microgrid_forecasting_020822_T * localP ) {
localB -> B_122_2_0 . re = localP -> P_0 ; localB -> B_122_2_0 . im = 0.0 ; }
void Auto_microgrid_forecasting_020822_NegSeqComputation ( SimStruct * S ,
real_T rtu_Enable , creal_T rtu_In , creal_T rtu_In_i , creal_T rtu_In_m ,
B_NegSeqComputation_Auto_microgrid_forecasting_020822_T * localB ,
DW_NegSeqComputation_Auto_microgrid_forecasting_020822_T * localDW ,
P_NegSeqComputation_Auto_microgrid_forecasting_020822_T * localP ) { if (
rtu_Enable > 0.0 ) { localB -> B_122_2_0 . re = ( ( ( localP -> P_2 [ 0 ] .
re * rtu_In . re - localP -> P_2 [ 0 ] . im * rtu_In . im ) + ( localP -> P_2
[ 1 ] . re * rtu_In_i . re - localP -> P_2 [ 1 ] . im * rtu_In_i . im ) ) + (
localP -> P_2 [ 2 ] . re * rtu_In_m . re - localP -> P_2 [ 2 ] . im *
rtu_In_m . im ) ) * localP -> P_1 ; localB -> B_122_2_0 . im = ( ( ( localP
-> P_2 [ 0 ] . re * rtu_In . im + localP -> P_2 [ 0 ] . im * rtu_In . re ) +
( localP -> P_2 [ 1 ] . re * rtu_In_i . im + localP -> P_2 [ 1 ] . im *
rtu_In_i . re ) ) + ( localP -> P_2 [ 2 ] . re * rtu_In_m . im + localP ->
P_2 [ 2 ] . im * rtu_In_m . re ) ) * localP -> P_1 ; if ( ssIsMajorTimeStep (
S ) != 0 ) { srUpdateBC ( localDW -> NegSeqComputation_SubsysRanBC ) ; } } }
void Auto_microgrid_forecasting_020822_NegSeqComputation_Term ( SimStruct *
const S ) { } void Auto_microgrid_forecasting_020822_ZeroSeqComputation_Init
( SimStruct * S , B_ZeroSeqComputation_Auto_microgrid_forecasting_020822_T *
localB , P_ZeroSeqComputation_Auto_microgrid_forecasting_020822_T * localP )
{ localB -> B_124_1_0 . re = localP -> P_0 ; localB -> B_124_1_0 . im = 0.0 ;
} void Auto_microgrid_forecasting_020822_ZeroSeqComputation ( SimStruct * S ,
real_T rtu_Enable , creal_T rtu_In , creal_T rtu_In_a , creal_T rtu_In_j ,
B_ZeroSeqComputation_Auto_microgrid_forecasting_020822_T * localB ,
DW_ZeroSeqComputation_Auto_microgrid_forecasting_020822_T * localDW ,
P_ZeroSeqComputation_Auto_microgrid_forecasting_020822_T * localP ) { if (
rtu_Enable > 0.0 ) { localB -> B_124_1_0 . re = ( ( rtu_In . re + rtu_In_a .
re ) + rtu_In_j . re ) * localP -> P_1 ; localB -> B_124_1_0 . im = ( (
rtu_In . im + rtu_In_a . im ) + rtu_In_j . im ) * localP -> P_1 ; if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
ZeroSeqComputation_SubsysRanBC ) ; } } } void
Auto_microgrid_forecasting_020822_ZeroSeqComputation_Term ( SimStruct * const
S ) { } void Auto_microgrid_forecasting_020822_NegSeqComputation_l_Term (
SimStruct * const S ) { } void Auto_microgrid_forecasting_020822_RMS_Init (
SimStruct * S , P_RMS_Auto_microgrid_forecasting_020822_T * localP ,
X_RMS_Auto_microgrid_forecasting_020822_T * localX ) { localX ->
integrator_CSTATE_bw [ 0 ] = localP -> P_1 ; localX -> integrator_CSTATE_g [
0 ] = localP -> P_2 ; localX -> integrator_CSTATE_bw [ 1 ] = localP -> P_1 ;
localX -> integrator_CSTATE_g [ 1 ] = localP -> P_2 ; localX ->
integrator_CSTATE_bw [ 2 ] = localP -> P_1 ; localX -> integrator_CSTATE_g [
2 ] = localP -> P_2 ; } void Auto_microgrid_forecasting_020822_RMS_Disable (
SimStruct * S , DW_RMS_Auto_microgrid_forecasting_020822_T * localDW ) {
localDW -> RMS_MODE = false ; } void Auto_microgrid_forecasting_020822_RMS (
SimStruct * S , boolean_T rtu_Enable , const real_T rtu_In [ 3 ] ,
B_RMS_Auto_microgrid_forecasting_020822_T * localB ,
DW_RMS_Auto_microgrid_forecasting_020822_T * localDW ,
P_RMS_Auto_microgrid_forecasting_020822_T * localP ,
XDis_RMS_Auto_microgrid_forecasting_020822_T * localXdis ) { real_T
rtb_B_159_2_0 ; int32_T isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( (
isHit != 0 ) && ( ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable ) { if (
! localDW -> RMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) )
{ ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( void ) memset ( & (
localXdis -> integrator_CSTATE_bw ) , 0 , 6 * sizeof ( boolean_T ) ) ;
localDW -> RMS_MODE = true ; } } else { if ( localDW -> RMS_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> integrator_CSTATE_bw ) , 1 , 6 * sizeof ( boolean_T ) ) ;
Auto_microgrid_forecasting_020822_RMS_Disable ( S , localDW ) ; } } } if (
localDW -> RMS_MODE ) { rtb_B_159_2_0 = muDoubleScalarSin ( localP -> P_5 *
ssGetTaskTime ( S , 0 ) + localP -> P_6 ) * localP -> P_3 + localP -> P_4 ;
localB -> B_159_3_0 [ 0 ] = rtu_In [ 0 ] * rtb_B_159_2_0 ; localB ->
B_159_3_0 [ 1 ] = rtu_In [ 1 ] * rtb_B_159_2_0 ; localB -> B_159_3_0 [ 2 ] =
rtu_In [ 2 ] * rtb_B_159_2_0 ; rtb_B_159_2_0 = muDoubleScalarSin ( localP ->
P_9 * ssGetTaskTime ( S , 0 ) + localP -> P_10 ) * localP -> P_7 + localP ->
P_8 ; localB -> B_159_5_0 [ 0 ] = rtu_In [ 0 ] * rtb_B_159_2_0 ; localB ->
B_159_5_0 [ 1 ] = rtu_In [ 1 ] * rtb_B_159_2_0 ; localB -> B_159_5_0 [ 2 ] =
rtu_In [ 2 ] * rtb_B_159_2_0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
srUpdateBC ( localDW -> RMS_SubsysRanBC ) ; } } } void
Auto_microgrid_forecasting_020822_RMS_Deriv ( SimStruct * S ,
B_RMS_Auto_microgrid_forecasting_020822_T * localB ,
DW_RMS_Auto_microgrid_forecasting_020822_T * localDW ,
XDot_RMS_Auto_microgrid_forecasting_020822_T * localXdot ) { if ( localDW ->
RMS_MODE ) { localXdot -> integrator_CSTATE_bw [ 0 ] = localB -> B_159_5_0 [
0 ] ; localXdot -> integrator_CSTATE_g [ 0 ] = localB -> B_159_3_0 [ 0 ] ;
localXdot -> integrator_CSTATE_bw [ 1 ] = localB -> B_159_5_0 [ 1 ] ;
localXdot -> integrator_CSTATE_g [ 1 ] = localB -> B_159_3_0 [ 1 ] ;
localXdot -> integrator_CSTATE_bw [ 2 ] = localB -> B_159_5_0 [ 2 ] ;
localXdot -> integrator_CSTATE_g [ 2 ] = localB -> B_159_3_0 [ 2 ] ; } else {
{ real_T * dx ; int_T i ; dx = & ( localXdot -> integrator_CSTATE_bw [ 0 ] )
; for ( i = 0 ; i < 6 ; i ++ ) { dx [ i ] = 0.0 ; } } } } void
Auto_microgrid_forecasting_020822_RMS_Term ( SimStruct * const S ) { } void
Auto_microgrid_forecasting_020822_TrueRMS_Init ( SimStruct * S ,
DW_TrueRMS_Auto_microgrid_forecasting_020822_T * localDW ,
P_TrueRMS_Auto_microgrid_forecasting_020822_T * localP ,
X_TrueRMS_Auto_microgrid_forecasting_020822_T * localX ) { localX ->
integrator_CSTATE_b [ 0 ] = localP -> P_2 ; localDW -> Memory_PreviousInput [
0 ] = localP -> P_6 ; localX -> integrator_CSTATE_b [ 1 ] = localP -> P_2 ;
localDW -> Memory_PreviousInput [ 1 ] = localP -> P_6 ; localX ->
integrator_CSTATE_b [ 2 ] = localP -> P_2 ; localDW -> Memory_PreviousInput [
2 ] = localP -> P_6 ; } void
Auto_microgrid_forecasting_020822_TrueRMS_Disable ( SimStruct * S ,
DW_TrueRMS_Auto_microgrid_forecasting_020822_T * localDW ) { localDW ->
TrueRMS_MODE = false ; } void Auto_microgrid_forecasting_020822_TrueRMS (
SimStruct * S , boolean_T rtu_Enable , const real_T rtu_In [ 3 ] ,
B_TrueRMS_Auto_microgrid_forecasting_020822_T * localB ,
DW_TrueRMS_Auto_microgrid_forecasting_020822_T * localDW ,
P_TrueRMS_Auto_microgrid_forecasting_020822_T * localP ,
X_TrueRMS_Auto_microgrid_forecasting_020822_T * localX ,
XDis_TrueRMS_Auto_microgrid_forecasting_020822_T * localXdis ) { int32_T
isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( rtu_Enable ) { if ( ! localDW ->
TrueRMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( void ) memset ( & (
localXdis -> integrator_CSTATE_b ) , 0 , 3 * sizeof ( boolean_T ) ) ; localDW
-> TrueRMS_MODE = true ; } } else { if ( localDW -> TrueRMS_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> integrator_CSTATE_b ) , 1 , 3 * sizeof ( boolean_T ) ) ;
Auto_microgrid_forecasting_020822_TrueRMS_Disable ( S , localDW ) ; } } } if
( localDW -> TrueRMS_MODE ) { localB -> B_161_1_0 [ 0 ] = localX ->
integrator_CSTATE_b [ 0 ] ; localB -> B_161_1_0 [ 1 ] = localX ->
integrator_CSTATE_b [ 1 ] ; localB -> B_161_1_0 [ 2 ] = localX ->
integrator_CSTATE_b [ 2 ] ; { real_T * * uBuffer = ( real_T * * ) & localDW
-> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & localDW -> TransportDelay_PWORK . TUbufferPtrs [ 3 ] ; real_T simTime
= ssGetT ( S ) ; real_T tMinusDelay ; { int_T i1 ; real_T * y0 = & localB ->
B_161_2_0 [ 0 ] ; int_T * iw_Tail = & localDW -> TransportDelay_IWORK . Tail
[ 0 ] ; int_T * iw_Head = & localDW -> TransportDelay_IWORK . Head [ 0 ] ;
int_T * iw_Last = & localDW -> TransportDelay_IWORK . Last [ 0 ] ; int_T *
iw_CircularBufSize = & localDW -> TransportDelay_IWORK . CircularBufSize [ 0
] ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { tMinusDelay = ( ( localP -> P_3 > 0.0 )
? localP -> P_3 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; y0 [ i1 ] =
Auto_microgrid_forecasting_020822_acc_rt_TDelayInterpolate ( tMinusDelay ,
0.0 , * tBuffer , * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] ,
iw_Tail [ i1 ] , iw_Head [ i1 ] , localP -> P_4 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; tBuffer ++ ; uBuffer ++ ; } } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { localB -> B_161_3_0 = localP -> P_5 ; localB -> B_161_4_0 [
0 ] = localDW -> Memory_PreviousInput [ 0 ] ; localB -> B_161_4_0 [ 1 ] =
localDW -> Memory_PreviousInput [ 1 ] ; localB -> B_161_4_0 [ 2 ] = localDW
-> Memory_PreviousInput [ 2 ] ; } if ( ssGetT ( S ) >= localB -> B_161_3_0 )
{ localB -> B_160_0_0 [ 0 ] = localB -> B_161_1_0 [ 0 ] - localB -> B_161_2_0
[ 0 ] ; localB -> B_160_1_0 [ 0 ] = localP -> P_0 * localB -> B_160_0_0 [ 0 ]
; localB -> B_161_7_0 [ 0 ] = localB -> B_160_1_0 [ 0 ] ; localB -> B_160_0_0
[ 1 ] = localB -> B_161_1_0 [ 1 ] - localB -> B_161_2_0 [ 1 ] ; localB ->
B_160_1_0 [ 1 ] = localP -> P_0 * localB -> B_160_0_0 [ 1 ] ; localB ->
B_161_7_0 [ 1 ] = localB -> B_160_1_0 [ 1 ] ; localB -> B_160_0_0 [ 2 ] =
localB -> B_161_1_0 [ 2 ] - localB -> B_161_2_0 [ 2 ] ; localB -> B_160_1_0 [
2 ] = localP -> P_0 * localB -> B_160_0_0 [ 2 ] ; localB -> B_161_7_0 [ 2 ] =
localB -> B_160_1_0 [ 2 ] ; } else { localB -> B_161_7_0 [ 0 ] = localB ->
B_161_4_0 [ 0 ] ; localB -> B_161_7_0 [ 1 ] = localB -> B_161_4_0 [ 1 ] ;
localB -> B_161_7_0 [ 2 ] = localB -> B_161_4_0 [ 2 ] ; } localB -> B_161_8_0
[ 0 ] = rtu_In [ 0 ] * rtu_In [ 0 ] ; localB -> B_161_8_0 [ 1 ] = rtu_In [ 1
] * rtu_In [ 1 ] ; localB -> B_161_8_0 [ 2 ] = rtu_In [ 2 ] * rtu_In [ 2 ] ;
if ( ssIsMajorTimeStep ( S ) != 0 ) { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 0 ] = localB -> B_161_7_0 [ 0 ] >=
localP -> P_7 ? 1 : localB -> B_161_7_0 [ 0 ] > localP -> P_8 ? 0 : - 1 ;
localDW -> Saturationtoavoidnegativesqrt_MODE [ 1 ] = localB -> B_161_7_0 [ 1
] >= localP -> P_7 ? 1 : localB -> B_161_7_0 [ 1 ] > localP -> P_8 ? 0 : - 1
; localDW -> Saturationtoavoidnegativesqrt_MODE [ 2 ] = localB -> B_161_7_0 [
2 ] >= localP -> P_7 ? 1 : localB -> B_161_7_0 [ 2 ] > localP -> P_8 ? 0 : -
1 ; srUpdateBC ( localDW -> TrueRMS_SubsysRanBC ) ; } } } void
Auto_microgrid_forecasting_020822_TrueRMS_Update ( SimStruct * S ,
B_TrueRMS_Auto_microgrid_forecasting_020822_T * localB ,
DW_TrueRMS_Auto_microgrid_forecasting_020822_T * localDW ,
P_TrueRMS_Auto_microgrid_forecasting_020822_T * localP ) { int32_T isHit ; if
( localDW -> TrueRMS_MODE ) { { real_T * * uBuffer = ( real_T * * ) & localDW
-> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = ( real_T
* * ) & localDW -> TransportDelay_PWORK . TUbufferPtrs [ 3 ] ; real_T simTime
= ssGetT ( S ) ; localDW -> TransportDelay_IWORK . Head [ 0 ] = ( ( localDW
-> TransportDelay_IWORK . Head [ 0 ] < ( localDW -> TransportDelay_IWORK .
CircularBufSize [ 0 ] - 1 ) ) ? ( localDW -> TransportDelay_IWORK . Head [ 0
] + 1 ) : 0 ) ; if ( localDW -> TransportDelay_IWORK . Head [ 0 ] == localDW
-> TransportDelay_IWORK . Tail [ 0 ] ) { if ( !
Auto_microgrid_forecasting_020822_acc_rt_TDelayUpdateTailOrGrowBuf ( &
localDW -> TransportDelay_IWORK . CircularBufSize [ 0 ] , & localDW ->
TransportDelay_IWORK . Tail [ 0 ] , & localDW -> TransportDelay_IWORK . Head
[ 0 ] , & localDW -> TransportDelay_IWORK . Last [ 0 ] , simTime - localP ->
P_3 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ localDW
-> TransportDelay_IWORK . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ localDW
-> TransportDelay_IWORK . Head [ 0 ] ] = localB -> B_161_1_0 [ 0 ] ; localDW
-> TransportDelay_IWORK . Head [ 1 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 1 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 1 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 1 ] == localDW -> TransportDelay_IWORK .
Tail [ 1 ] ) { if ( !
Auto_microgrid_forecasting_020822_acc_rt_TDelayUpdateTailOrGrowBuf ( &
localDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] , & localDW ->
TransportDelay_IWORK . Tail [ 1 ] , & localDW -> TransportDelay_IWORK . Head
[ 1 ] , & localDW -> TransportDelay_IWORK . Last [ 1 ] , simTime - localP ->
P_3 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ++ ) [ localDW
-> TransportDelay_IWORK . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ localDW
-> TransportDelay_IWORK . Head [ 1 ] ] = localB -> B_161_1_0 [ 1 ] ; localDW
-> TransportDelay_IWORK . Head [ 2 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 2 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 2 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 2 ] == localDW -> TransportDelay_IWORK .
Tail [ 2 ] ) { if ( !
Auto_microgrid_forecasting_020822_acc_rt_TDelayUpdateTailOrGrowBuf ( &
localDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] , & localDW ->
TransportDelay_IWORK . Tail [ 2 ] , & localDW -> TransportDelay_IWORK . Head
[ 2 ] , & localDW -> TransportDelay_IWORK . Last [ 2 ] , simTime - localP ->
P_3 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = localB -> B_161_1_0 [ 2 ] ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput [ 0 ] = localB -> B_161_7_0 [ 0 ] ; localDW ->
Memory_PreviousInput [ 1 ] = localB -> B_161_7_0 [ 1 ] ; localDW ->
Memory_PreviousInput [ 2 ] = localB -> B_161_7_0 [ 2 ] ; } } } void
Auto_microgrid_forecasting_020822_TrueRMS_Deriv ( SimStruct * S ,
B_TrueRMS_Auto_microgrid_forecasting_020822_T * localB ,
DW_TrueRMS_Auto_microgrid_forecasting_020822_T * localDW ,
XDot_TrueRMS_Auto_microgrid_forecasting_020822_T * localXdot ) { if ( localDW
-> TrueRMS_MODE ) { localXdot -> integrator_CSTATE_b [ 0 ] = localB ->
B_161_8_0 [ 0 ] ; localXdot -> integrator_CSTATE_b [ 1 ] = localB ->
B_161_8_0 [ 1 ] ; localXdot -> integrator_CSTATE_b [ 2 ] = localB ->
B_161_8_0 [ 2 ] ; } else { { real_T * dx ; int_T i ; dx = & ( localXdot ->
integrator_CSTATE_b [ 0 ] ) ; for ( i = 0 ; i < 3 ; i ++ ) { dx [ i ] = 0.0 ;
} } } } void Auto_microgrid_forecasting_020822_TrueRMS_ZC ( SimStruct * S ,
B_TrueRMS_Auto_microgrid_forecasting_020822_T * localB ,
DW_TrueRMS_Auto_microgrid_forecasting_020822_T * localDW ,
P_TrueRMS_Auto_microgrid_forecasting_020822_T * localP ,
ZCV_TrueRMS_Auto_microgrid_forecasting_020822_T * localZCSV ) { if ( localDW
-> TrueRMS_MODE ) { localZCSV -> Saturationtoavoidnegativesqrt_UprLim_ZC [ 0
] = localB -> B_161_7_0 [ 0 ] - localP -> P_7 ; localZCSV ->
Saturationtoavoidnegativesqrt_LwrLim_ZC [ 0 ] = localB -> B_161_7_0 [ 0 ] -
localP -> P_8 ; localZCSV -> Saturationtoavoidnegativesqrt_UprLim_ZC [ 1 ] =
localB -> B_161_7_0 [ 1 ] - localP -> P_7 ; localZCSV ->
Saturationtoavoidnegativesqrt_LwrLim_ZC [ 1 ] = localB -> B_161_7_0 [ 1 ] -
localP -> P_8 ; localZCSV -> Saturationtoavoidnegativesqrt_UprLim_ZC [ 2 ] =
localB -> B_161_7_0 [ 2 ] - localP -> P_7 ; localZCSV ->
Saturationtoavoidnegativesqrt_LwrLim_ZC [ 2 ] = localB -> B_161_7_0 [ 2 ] -
localP -> P_8 ; } else { { real_T * zcsv = & ( localZCSV ->
Saturationtoavoidnegativesqrt_UprLim_ZC [ 0 ] ) ; int_T i ; for ( i = 0 ; i <
6 ; i ++ ) { zcsv [ i ] = 0.0 ; } } } } void
Auto_microgrid_forecasting_020822_TrueRMS_Term ( SimStruct * const S ) { }
void Auto_microgrid_forecasting_020822_NEGATIVEEdge_Init ( SimStruct * S ,
B_NEGATIVEEdge_Auto_microgrid_forecasting_020822_T * localB ,
P_NEGATIVEEdge_Auto_microgrid_forecasting_020822_T * localP ) { localB ->
B_220_0_0 = localP -> P_0 ; } void
Auto_microgrid_forecasting_020822_NEGATIVEEdge ( SimStruct * S , real_T
rtu_Enable , boolean_T rtu_IN , boolean_T rtu_INprevious ,
B_NEGATIVEEdge_Auto_microgrid_forecasting_020822_T * localB ,
DW_NEGATIVEEdge_Auto_microgrid_forecasting_020822_T * localDW ) { if (
rtu_Enable > 0.0 ) { localB -> B_220_0_0 = ( ( int32_T ) rtu_INprevious > (
int32_T ) rtu_IN ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC (
localDW -> NEGATIVEEdge_SubsysRanBC ) ; } } } void
Auto_microgrid_forecasting_020822_NEGATIVEEdge_Term ( SimStruct * const S ) {
} void Auto_microgrid_forecasting_020822_POSITIVEEdge_Init ( SimStruct * S ,
B_POSITIVEEdge_Auto_microgrid_forecasting_020822_T * localB ,
P_POSITIVEEdge_Auto_microgrid_forecasting_020822_T * localP ) { localB ->
B_221_0_0 = localP -> P_0 ; } void
Auto_microgrid_forecasting_020822_POSITIVEEdge ( SimStruct * S , real_T
rtu_Enable , boolean_T rtu_IN , boolean_T rtu_INprevious ,
B_POSITIVEEdge_Auto_microgrid_forecasting_020822_T * localB ,
DW_POSITIVEEdge_Auto_microgrid_forecasting_020822_T * localDW ) { if (
rtu_Enable > 0.0 ) { localB -> B_221_0_0 = ( ( int32_T ) rtu_IN > ( int32_T )
rtu_INprevious ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW
-> POSITIVEEdge_SubsysRanBC ) ; } } } void
Auto_microgrid_forecasting_020822_POSITIVEEdge_Term ( SimStruct * const S ) {
} void Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem_Enable (
SimStruct * S ) { if ( ssGetTaskTime ( S , 5 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem_Disable (
SimStruct * S ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem ( SimStruct * S ,
real_T rtu_u_frt , boolean_T * rty_Out1 , boolean_T * rty_Output ,
B_SwitchCaseActionSubsystem_Auto_microgrid_forecasting_020822_T * localB ) {
int32_T isHit ; isHit = ssIsSampleHit ( S , 5 , 0 ) ; if ( isHit != 0 ) { *
rty_Output = ( ( localB -> B_232_0_0 <= rtu_u_frt ) && ( rtu_u_frt <= localB
-> B_232_1_0 ) ) ; * rty_Out1 = ( ( localB -> B_232_2_0 <= rtu_u_frt ) && (
rtu_u_frt <= localB -> B_232_3_0 ) ) ; } } void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystemTID6 ( SimStruct *
S , B_SwitchCaseActionSubsystem_Auto_microgrid_forecasting_020822_T * localB
, P_SwitchCaseActionSubsystem_Auto_microgrid_forecasting_020822_T * localP )
{ localB -> B_232_0_0 = localP -> P_0 ; localB -> B_232_1_0 = localP -> P_1 ;
localB -> B_232_2_0 = localP -> P_2 ; localB -> B_232_3_0 = localP -> P_3 ; }
void Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem_Term (
SimStruct * const S ) { } void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2_Enable (
SimStruct * S ) { if ( ssGetTaskTime ( S , 5 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2_Disable (
SimStruct * S ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2 ( SimStruct * S
, real_T rtu_u_frt , boolean_T * rty_Out1 ,
B_SwitchCaseActionSubsystem2_Auto_microgrid_forecasting_020822_T * localB ) {
int32_T isHit ; isHit = ssIsSampleHit ( S , 5 , 0 ) ; if ( isHit != 0 ) { *
rty_Out1 = ( rtu_u_frt > localB -> B_234_0_0 ) ; } } void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2TID6 ( SimStruct
* S , B_SwitchCaseActionSubsystem2_Auto_microgrid_forecasting_020822_T *
localB , P_SwitchCaseActionSubsystem2_Auto_microgrid_forecasting_020822_T *
localP ) { localB -> B_234_0_0 = localP -> P_0 ; } void
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2_Term ( SimStruct
* const S ) { } void
Auto_microgrid_forecasting_020822_Subsystempi2delay_i_Init ( SimStruct * S ,
B_Subsystempi2delay_Auto_microgrid_forecasting_020822_h_T * localB ,
P_Subsystempi2delay_Auto_microgrid_forecasting_020822_f_T * localP ) { localB
-> B_240_0_0 = localP -> P_0 [ 0 ] ; localB -> B_240_1_0 = localP -> P_0 [ 1
] ; } void Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( SimStruct
* S , uint8_T rtu_Enable , const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt
, B_Subsystempi2delay_Auto_microgrid_forecasting_020822_h_T * localB ,
DW_Subsystempi2delay_Auto_microgrid_forecasting_020822_e_T * localDW ) { if (
rtu_Enable > 0 ) { localB -> B_240_0_0 = rtu_alpha_beta [ 0 ] *
muDoubleScalarSin ( rtu_wt ) - rtu_alpha_beta [ 1 ] * muDoubleScalarCos (
rtu_wt ) ; localB -> B_240_1_0 = rtu_alpha_beta [ 0 ] * muDoubleScalarCos (
rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarSin ( rtu_wt ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
Subsystempi2delay_SubsysRanBC ) ; } } } void
Auto_microgrid_forecasting_020822_Subsystempi2delay_i_Term ( SimStruct *
const S ) { } void Auto_microgrid_forecasting_020822_Subsystem1_k_Init (
SimStruct * S , B_Subsystem1_Auto_microgrid_forecasting_020822_c_T * localB ,
P_Subsystem1_Auto_microgrid_forecasting_020822_n_T * localP ) { localB ->
B_241_0_0 = localP -> P_0 [ 0 ] ; localB -> B_241_1_0 = localP -> P_0 [ 1 ] ;
} void Auto_microgrid_forecasting_020822_Subsystem1_h ( SimStruct * S ,
uint8_T rtu_Enable , const real_T rtu_alpha_beta [ 2 ] , real_T rtu_wt ,
B_Subsystem1_Auto_microgrid_forecasting_020822_c_T * localB ,
DW_Subsystem1_Auto_microgrid_forecasting_020822_j_T * localDW ) { if (
rtu_Enable > 0 ) { localB -> B_241_0_0 = rtu_alpha_beta [ 0 ] *
muDoubleScalarCos ( rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarSin (
rtu_wt ) ; localB -> B_241_1_0 = - rtu_alpha_beta [ 0 ] * muDoubleScalarSin (
rtu_wt ) + rtu_alpha_beta [ 1 ] * muDoubleScalarCos ( rtu_wt ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( localDW ->
Subsystem1_SubsysRanBC ) ; } } } void
Auto_microgrid_forecasting_020822_Subsystem1_k_Term ( SimStruct * const S ) {
} void Auto_microgrid_forecasting_020822_CounterASTfmax_Init ( SimStruct * S
, B_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localB ,
DW_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localDW ,
P_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localP ) { localDW ->
Memory_PreviousInput = localP -> P_1 ; localB -> B_263_1_0 = localP -> P_0 ;
} void Auto_microgrid_forecasting_020822_CounterASTfmax ( SimStruct * S ,
boolean_T rtu_Trigger , B_CounterASTfmax_Auto_microgrid_forecasting_020822_T
* localB , DW_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localDW ,
ZCE_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localZCE ) { if (
ssIsMajorTimeStep ( S ) != 0 ) { if ( rtu_Trigger && ( localZCE ->
CounterASTfmax_Trig_ZCE != POS_ZCSIG ) ) { localB -> B_263_1_0 = localDW ->
Memory_PreviousInput + localB -> B_263_0_0 ; localDW -> Memory_PreviousInput
= localB -> B_263_1_0 ; localDW -> CounterASTfmax_SubsysRanBC = 4 ; }
localZCE -> CounterASTfmax_Trig_ZCE = rtu_Trigger ; } } void
Auto_microgrid_forecasting_020822_CounterASTfmaxTID6 ( SimStruct * S ,
B_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localB ,
P_CounterASTfmax_Auto_microgrid_forecasting_020822_T * localP ) { localB ->
B_263_0_0 = localP -> P_2 ; } void
Auto_microgrid_forecasting_020822_CounterASTfmax_Term ( SimStruct * const S )
{ } void rt_invd5x5_snf ( const real_T u [ 25 ] , real_T y [ 25 ] ) { int8_T
p [ 5 ] ; real_T A [ 25 ] ; int8_T ipiv [ 5 ] ; int32_T pipk ; int32_T jj ;
int32_T jp1j ; int32_T jBcol ; int32_T kAcol ; int32_T c ; int32_T ix ;
real_T smax ; real_T s ; int32_T iy ; int32_T jA ; int32_T ijA ; for ( iy = 0
; iy < 25 ; iy ++ ) { y [ iy ] = 0.0 ; A [ iy ] = u [ iy ] ; } for ( iy = 0 ;
iy < 5 ; iy ++ ) { ipiv [ iy ] = ( int8_T ) ( iy + 1 ) ; } for ( pipk = 0 ;
pipk < 4 ; pipk ++ ) { kAcol = pipk + 1 ; jBcol = ( kAcol - 1 ) * 6 + 1 ; jj
= ( kAcol - 1 ) * 6 ; c = 6 - kAcol ; iy = 0 ; ix = jBcol ; smax =
muDoubleScalarAbs ( A [ jj ] ) ; for ( jA = 2 ; jA <= c ; jA ++ ) { ix ++ ; s
= muDoubleScalarAbs ( A [ ix - 1 ] ) ; if ( s > smax ) { iy = jA - 1 ; smax =
s ; } } if ( A [ ( jBcol + iy ) - 1 ] != 0.0 ) { if ( iy != 0 ) { ipiv [
kAcol - 1 ] = ( int8_T ) ( kAcol + iy ) ; ix = kAcol ; iy += kAcol ; for ( jA
= 0 ; jA < 5 ; jA ++ ) { smax = A [ ix - 1 ] ; A [ ix - 1 ] = A [ iy - 1 ] ;
A [ iy - 1 ] = smax ; ix += 5 ; iy += 5 ; } } iy = jBcol - kAcol ; for ( ix =
jBcol + 1 ; ix <= iy + 5 ; ix ++ ) { A [ ix - 1 ] /= A [ jj ] ; } } c = 4 -
kAcol ; jA = jj ; jj += 5 ; for ( jp1j = 0 ; jp1j <= c ; jp1j ++ ) { if ( A [
jj ] != 0.0 ) { smax = - A [ jj ] ; ix = jBcol ; iy = jA - kAcol ; for ( ijA
= jA + 7 ; ijA <= iy + 11 ; ijA ++ ) { A [ ijA - 1 ] += A [ ( ix + 1 ) - 1 ]
* smax ; ix ++ ; } } jj += 5 ; jA += 5 ; } } for ( iy = 0 ; iy < 5 ; iy ++ )
{ p [ iy ] = ( int8_T ) ( iy + 1 ) ; } if ( ipiv [ 0 ] > 1 ) { pipk = p [
ipiv [ 0 ] - 1 ] ; p [ ipiv [ 0 ] - 1 ] = p [ 0 ] ; p [ 0 ] = ( int8_T ) pipk
; } if ( ipiv [ 1 ] > 2 ) { pipk = p [ ipiv [ 1 ] - 1 ] ; p [ ipiv [ 1 ] - 1
] = p [ 1 ] ; p [ 1 ] = ( int8_T ) pipk ; } if ( ipiv [ 2 ] > 3 ) { pipk = p
[ ipiv [ 2 ] - 1 ] ; p [ ipiv [ 2 ] - 1 ] = p [ 2 ] ; p [ 2 ] = ( int8_T )
pipk ; } if ( ipiv [ 3 ] > 4 ) { pipk = p [ 4 ] ; p [ 4 ] = p [ 3 ] ; p [ 3 ]
= ( int8_T ) pipk ; } for ( jA = 0 ; jA < 5 ; jA ++ ) { kAcol = jA + 1 ;
jBcol = p [ kAcol - 1 ] ; y [ ( kAcol + 5 * ( p [ kAcol - 1 ] - 1 ) ) - 1 ] =
1.0 ; for ( pipk = kAcol ; pipk < 6 ; pipk ++ ) { if ( y [ ( ( jBcol - 1 ) *
5 + pipk ) - 1 ] != 0.0 ) { for ( ix = pipk + 1 ; ix < 6 ; ix ++ ) { y [ ( ix
+ 5 * ( jBcol - 1 ) ) - 1 ] -= y [ ( ( jBcol - 1 ) * 5 + pipk ) - 1 ] * A [ (
( pipk - 1 ) * 5 + ix ) - 1 ] ; } } } } for ( pipk = 0 ; pipk < 5 ; pipk ++ )
{ jBcol = 5 * pipk ; for ( jA = 4 ; jA >= 0 ; jA -- ) { kAcol = 5 * jA ; if (
y [ jA + jBcol ] != 0.0 ) { y [ jA + jBcol ] /= A [ jA + kAcol ] ; iy = jA -
1 ; for ( ix = 0 ; ix <= iy ; ix ++ ) { c = ix + 1 ; y [ ( c + jBcol ) - 1 ]
-= A [ ( c + kAcol ) - 1 ] * y [ jA + jBcol ] ; } } } } } static void
mdlOutputs ( SimStruct * S , int_T tid ) { creal_T B_311_186_0 ; creal_T
B_311_215_0 ; creal_T B_311_244_0 ; creal_T B_311_290_0 ; creal_T B_311_319_0
; creal_T B_311_348_0 ; creal_T B_311_2908_0 ; creal_T B_311_2937_0 ; creal_T
B_311_2966_0 ; creal_T B_311_4473_0 ; creal_T B_311_4502_0 ; creal_T
B_311_4531_0 ; creal_T B_311_4726_0 ; creal_T B_311_4755_0 ; creal_T
B_311_4784_0 ; real_T B_311_92_0 ; real_T B_311_1582_0 ; real_T B_311_1584_0
; real_T B_311_1608_0 ; real_T B_311_1610_0 ; real_T B_311_1615_0 ; real_T
B_311_1617_0 ; real_T B_311_1624_0 ; real_T B_311_1625_0 ; real_T
B_311_1631_0 ; real_T B_311_1632_0 ; real_T B_311_1995_0 ; real_T
B_311_1997_0 ; real_T B_311_2021_0 ; real_T B_311_2023_0 ; real_T
B_311_2028_0 ; real_T B_311_2030_0 ; real_T B_311_2038_0 ; real_T
B_311_2039_0 ; real_T B_311_2045_0 ; real_T B_311_2046_0 ; real_T
B_311_2995_0 [ 3 ] ; real_T B_311_2996_0 ; real_T B_311_3003_0 [ 3 ] ; real_T
B_311_3260_0 ; real_T B_311_3272_0 [ 3 ] ; real_T B_311_3310_0 [ 3 ] ; real_T
B_311_3315_0 ; real_T B_311_3417_0 [ 3 ] ; real_T B_311_3553_0 [ 3 ] ; real_T
B_311_3581_0 [ 3 ] ; real_T B_311_3733_0 [ 3 ] ; real_T B_311_3865_0 ; real_T
B_311_3869_0 [ 3 ] ; real_T B_311_4153_0 ; real_T B_311_4155_0 ; real_T
B_311_4180_0 ; real_T B_311_4182_0 ; real_T B_311_4187_0 ; real_T
B_311_4189_0 ; real_T B_311_4198_0 ; real_T B_311_4199_0 ; real_T
B_311_4205_0 ; real_T B_311_4206_0 ; real_T B_311_5541_0 ; real_T
B_311_5569_0 ; real_T B_292_1_0 [ 3 ] ; real_T B_271_1_0 [ 3 ] ; real_T
B_212_11_0 ; real_T B_156_39_0 ; real_T B_156_50_0 ; boolean_T B_311_3040_0 ;
boolean_T B_311_3046_0 ; boolean_T B_311_3394_0 ; boolean_T B_311_3472_0 ;
boolean_T B_311_3476_0 ; real_T rtb_B_311_124_0 [ 25 ] ; real_T
rtb_B_311_140_0 [ 15 ] ; real_T rtb_B_311_8_0 [ 5 ] ; real_T rtb_B_311_133_0
[ 15 ] ; real_T rtb_B_311_5514_0 [ 5 ] ; real_T rtb_B_311_100_0 ; real_T
rtb_B_311_3436_0 ; real_T rtb_B_311_3432_0 ; real_T rtb_B_311_3427_0 ; real_T
rtb_B_311_3426_0 ; real_T rtb_B_300_1_0 [ 25 ] ; real_T rtb_B_300_4_0 [ 25 ]
; real_T rtb_B_311_4_1 ; real_T rtb_B_311_4_0 ; real_T rtb_B_311_3279_0 ;
real_T rtb_B_311_3262_0 ; real_T rtb_B_311_3261_0 ; real_T rtb_B_311_4190_0 ;
real_T rtb_B_311_4188_0 ; real_T rtb_B_311_4186_0 ; real_T rtb_B_311_4181_0 ;
real_T rtb_B_311_4106_0 ; real_T rtb_B_311_3090_0 ; real_T rtb_B_311_5_0 [ 5
] ; real_T rtb_B_311_1335_0 [ 3 ] ; real_T rtb_B_311_1545_0 ; real_T
rtb_B_311_1544_0 ; real_T rtb_B_311_1543_0 ; real_T rtb_B_311_1540_0 ; real_T
rtb_B_311_1598_0 ; real_T rtb_B_311_1597_0 ; real_T rtb_B_311_1596_0 ; real_T
rtb_B_311_1580_0 ; real_T rtb_B_311_1579_0 ; real_T rtb_B_311_1578_0 ; real_T
rtb_B_311_2031_0 ; real_T rtb_B_311_2029_0 ; real_T rtb_B_311_2027_0 ; real_T
rtb_B_311_2022_0 ; real_T rtb_B_311_141_0 [ 6 ] ; real_T rtb_B_311_111_0 [ 10
] ; real_T rtb_B_299_15_0 [ 25 ] ; real_T rtb_B_311_35_0 ; real_T
rtb_B_299_3_0 ; real_T rtb_B_300_0_0 [ 5 ] ; real_T rtb_B_311_5582_0 ; real_T
rtb_B_311_3409_0 ; real_T rtb_B_311_3807_0 ; real_T rtb_B_311_3795_0 ; real_T
rtb_B_311_3349_0 ; real_T rtb_B_311_3271_0 [ 3 ] ; real_T rtb_B_311_3087_0 ;
real_T rtb_B_311_2877_0 [ 3 ] ; real_T rtb_B_311_68_0 ; real_T
rtb_B_311_3463_0 ; real_T rtb_B_5_7_0 ; real_T rtb_B_311_3465_0 ; real_T
rtb_B_311_3464_0 ; real_T rtb_B_311_3103_0 ; real_T rtb_B_311_3012_0 ; real_T
rtb_B_311_52_0 ; real_T rtb_B_5_3_0 ; real_T rtb_B_298_4_0 ; real_T
rtb_B_311_3886_0 ; real_T rtb_B_311_3750_0 ; boolean_T rtb_B_311_3375_0 ;
real_T rtb_B_311_3469_0 ; real_T rtb_B_311_3408_0 ; real_T rtb_B_311_3335_0 ;
real_T rtb_B_311_3334_0 ; real_T rtb_B_311_3255_0 ; real_T rtb_B_311_3253_0 ;
real_T rtb_B_311_3240_0 ; real_T rtb_B_311_3235_0 ; real_T rtb_B_311_3233_0 ;
real_T rtb_B_311_2980_0 ; real_T rtb_B_311_251_0 ; real_T rtb_B_311_1324_0 ;
real_T rtb_B_311_128_0 [ 5 ] ; real_T rtb_B_0_0_0 ; boolean_T rtb_B_311_69_0
; real_T rtb_B_311_47_0 ; real_T rtb_B_311_36_0 ; boolean_T rtb_B_311_44_0 ;
real_T rtb_B_299_1_0 ; real_T rtb_B_311_5546_0 ; boolean_T rtb_B_311_4555_0 ;
boolean_T rtb_B_311_4439_0 ; real_T rtb_B_311_3082_0 ; real_T
rtb_B_311_3086_0 ; real_T rtb_B_311_3815_0 ; real_T rtb_B_311_3805_0 ; real_T
rtb_B_311_3251_0 ; real_T rtb_B_311_3453_0 ; real_T rtb_B_311_3458_0 ; real_T
rtb_B_311_3348_0 ; boolean_T rtb_B_311_3377_0 ; boolean_T rtb_B_311_3366_0 ;
real_T rtb_B_311_3347_0 ; real_T rtb_B_311_3231_0 ; real_T rtb_B_311_3278_0 ;
real_T rtb_B_311_3001_0 ; int8_T rtAction ; int8_T rtPrevAction ; real_T
tmpForInput [ 9 ] ; real_T rtb_B_311_1587_0 ; real_T rtb_B_311_1550_0 ;
real_T rtb_B_311_1443_0 ; real_T rtb_B_311_62_0 ; real_T rtb_B_311_51_0 ;
real_T rtb_B_311_50_0 ; real_T rtb_B_311_37_0 ; int32_T i ; real_T B_299_15_0
[ 5 ] ; real_T B_311_104_0_0 [ 10 ] ; real_T B_311_126_0_0 [ 6 ] ; real_T
B_311_126_0_1 [ 2 ] ; real_T B_300_1_0 [ 25 ] ; real_T tmp [ 5 ] ; real_T
B_311_146_0_0 [ 18 ] ; int32_T i_0 ; int32_T i_1 ; real_T B_299_0_0_idx_2 ;
real_T B_299_0_0_idx_0 ; real_T B_299_0_0_idx_1 ; real_T B_311_1531_0_idx_0 ;
real_T B_311_1531_0_idx_1 ; real_T B_311_1537_0_idx_0 ; real_T
B_311_1537_0_idx_1 ; real_T B_311_3006_0_idx_1 ; real_T B_311_1965_0_idx_0 ;
real_T B_311_1965_0_idx_1 ; real_T B_311_4097_0_idx_0 ; real_T
B_311_4097_0_idx_1 ; real_T B_311_4535_0_idx_0 ; real_T B_311_1062_0_idx_0 ;
real_T B_311_1062_0_idx_1 ; real_T B_311_1062_0_idx_2 ; real_T
B_311_1536_0_idx_0 ; real_T B_311_1536_0_idx_1 ; real_T B_311_1536_0_idx_2 ;
real_T B_311_401_0_idx_1 ; real_T B_311_473_0_idx_0 ; real_T
B_311_1549_0_idx_0 ; real_T B_311_1556_0_idx_0 ; real_T B_311_1556_0_idx_1 ;
real_T B_311_1549_0_idx_1 ; real_T B_311_1940_0_idx_0 ; real_T
B_311_1940_0_idx_1 ; real_T B_311_1283_0_idx_0 ; real_T B_311_1283_0_idx_1 ;
real_T B_311_1283_0_idx_2 ; real_T B_311_1349_0_idx_0 ; real_T
B_311_1349_0_idx_1 ; real_T B_311_1349_0_idx_2 ; real_T B_311_1586_0_idx_0 ;
real_T B_311_1586_0_idx_1 ; real_T B_311_2829_0_idx_0 ; real_T
B_311_2829_0_idx_1 ; real_T B_311_2829_0_idx_2 ; real_T B_311_4115_0_idx_1 ;
real_T B_311_4123_0_idx_0 ; real_T B_311_4123_0_idx_1 ; real_T
B_311_104_0_idx_1 ; real_T B_311_2872_0_idx_0 ; real_T B_311_2874_0_idx_0 ;
real_T B_311_2872_0_idx_1 ; real_T B_311_2874_0_idx_1 ; real_T
B_311_2872_0_idx_2 ; real_T B_311_2874_0_idx_2 ; real_T B_311_3242_0_idx_0 ;
real_T B_311_3244_0_idx_0 ; real_T B_311_3265_0_idx_0 ; real_T
B_311_3267_0_idx_0 ; real_T B_311_3242_0_idx_1 ; real_T B_311_3244_0_idx_1 ;
real_T B_311_3265_0_idx_1 ; real_T B_311_3267_0_idx_1 ; real_T
B_311_3242_0_idx_2 ; real_T B_311_3244_0_idx_2 ; real_T B_311_3265_0_idx_2 ;
real_T B_311_3267_0_idx_2 ; real_T B_311_3303_0_idx_0 ; real_T
B_311_3305_0_idx_0 ; real_T B_311_3303_0_idx_1 ; real_T B_311_3305_0_idx_1 ;
real_T B_311_3303_0_idx_2 ; real_T B_311_3305_0_idx_2 ;
B_Auto_microgrid_forecasting_020822_T * _rtB ;
P_Auto_microgrid_forecasting_020822_T * _rtP ;
X_Auto_microgrid_forecasting_020822_T * _rtX ;
PrevZCX_Auto_microgrid_forecasting_020822_T * _rtZCE ;
DW_Auto_microgrid_forecasting_020822_T * _rtDW ; _rtDW = ( (
DW_Auto_microgrid_forecasting_020822_T * ) ssGetRootDWork ( S ) ) ; _rtZCE =
( ( PrevZCX_Auto_microgrid_forecasting_020822_T * ) _ssGetPrevZCSigState ( S
) ) ; _rtX = ( ( X_Auto_microgrid_forecasting_020822_T * ) ssGetContStates (
S ) ) ; _rtP = ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp (
S ) ) ; _rtB = ( ( B_Auto_microgrid_forecasting_020822_T * )
_ssGetModelBlockIO ( S ) ) ; i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 )
{ muDoubleScalarSinCos ( _rtDW -> Rotorangledthetae_DSTATE + _rtP -> P_593 *
ssGetTaskTime ( S , 2 ) , & rtb_B_311_4_0 , & rtb_B_311_4_1 ) ; for ( i = 0 ;
i < 5 ; i ++ ) { rtb_B_311_5_0 [ i ] = _rtDW -> fluxes_DSTATE [ i ] ; } if (
_rtB -> B_311_414_0 > 0.0 ) { i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 )
{ rtb_B_299_3_0 = muDoubleScalarAbs ( ( ( _rtP -> P_553 [ 0 ] * rtb_B_311_5_0
[ 1 ] + _rtP -> P_553 [ 1 ] * rtb_B_311_5_0 [ 2 ] ) + _rtP -> P_553 [ 2 ] *
rtb_B_311_5_0 [ 3 ] ) * ( 1.0 / ( ( _rtB -> B_299_3_0 [ 0 ] + _rtB ->
B_299_3_0 [ 1 ] ) + 1.0 / _rtDW -> Lmd_sat_DSTATE ) ) ) ; rtb_B_299_1_0 =
look1_pbinlxpw ( rtb_B_299_3_0 , _rtP -> P_556 , _rtP -> P_555 , & _rtDW ->
m_bpIndex , 1U ) ; if ( rtb_B_299_1_0 != 0.0 ) { rtb_B_299_3_0 /=
rtb_B_299_1_0 ; } else { rtb_B_299_3_0 = _rtB -> B_299_2_0 ; } _rtB ->
B_299_11_0 = _rtP -> P_557 * rtb_B_299_3_0 ; if ( _rtB -> B_299_0_0 ) { i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { rtb_B_299_1_0 =
muDoubleScalarAbs ( 1.0 / ( ( ( _rtB -> B_298_1_0 [ 0 ] + _rtB -> B_298_1_0 [
1 ] ) + _rtB -> B_298_1_0 [ 2 ] ) + 1.0 / _rtDW -> Lmq_sat_DSTATE ) * ( _rtP
-> P_542 [ 0 ] * rtb_B_311_5_0 [ 0 ] + _rtP -> P_542 [ 1 ] * rtb_B_311_5_0 [
4 ] ) ) ; rtb_B_298_4_0 = look1_pbinlxpw ( rtb_B_299_1_0 , _rtP -> P_545 ,
_rtP -> P_544 , & _rtDW -> m_bpIndex_g , 1U ) ; if ( rtb_B_298_4_0 != 0.0 ) {
rtb_B_299_1_0 /= rtb_B_298_4_0 ; } else { rtb_B_299_1_0 = _rtB -> B_298_0_0 ;
} _rtB -> B_298_11_0 = _rtP -> P_546 * rtb_B_299_1_0 ; } srUpdateBC ( _rtDW
-> Lmq_sat_SubsysRanBC ) ; } if ( _rtB -> B_299_1_0 ) { rtb_B_299_1_0 = _rtB
-> B_298_11_0 ; } else { rtb_B_299_1_0 = _rtB -> B_299_8_0 ; } memcpy ( &
rtb_B_299_15_0 [ 0 ] , & _rtB -> B_299_5_0 [ 0 ] , 25U * sizeof ( real_T ) )
; rtb_B_299_15_0 [ 0 ] = rtb_B_299_1_0 ; rtb_B_299_15_0 [ 4 ] = rtb_B_299_1_0
; rtb_B_299_15_0 [ 20 ] = rtb_B_299_1_0 ; rtb_B_299_15_0 [ 24 ] =
rtb_B_299_1_0 ; for ( i = 0 ; i < 3 ; i ++ ) { rtb_B_299_15_0 [ 5 * ( i + 1 )
+ 1 ] = _rtB -> B_299_11_0 ; rtb_B_299_15_0 [ 5 * ( i + 1 ) + 2 ] = _rtB ->
B_299_11_0 ; rtb_B_299_15_0 [ 5 * ( i + 1 ) + 3 ] = _rtB -> B_299_11_0 ; }
for ( i = 0 ; i < 25 ; i ++ ) { rtb_B_300_4_0 [ i ] = rtb_B_299_15_0 [ i ] +
_rtB -> B_299_6_0 [ i ] ; } rt_invd5x5_snf ( rtb_B_300_4_0 , _rtB ->
B_299_17_0 ) ; for ( i = 0 ; i < 5 ; i ++ ) { for ( i_0 = 0 ; i_0 < 5 ; i_0
++ ) { _rtB -> B_299_18_0 [ i_0 + 5 * i ] = 0.0 ; for ( i_1 = 0 ; i_1 < 5 ;
i_1 ++ ) { _rtB -> B_299_18_0 [ i_0 + 5 * i ] += _rtB -> B_299_7_0 [ 5 * i_1
+ i_0 ] * _rtB -> B_299_17_0 [ 5 * i + i_1 ] ; } } } } if ( ssIsMajorTimeStep
( S ) != 0 ) { srUpdateBC ( _rtDW -> Saturation_SubsysRanBC ) ; } } for ( i =
0 ; i < 25 ; i ++ ) { if ( _rtB -> B_311_416_0 >= _rtP -> P_595 ) {
rtb_B_299_15_0 [ i ] = _rtB -> B_299_17_0 [ i ] ; } else { rtb_B_299_15_0 [ i
] = _rtB -> B_311_417_0 [ i ] ; } } for ( i = 0 ; i < 5 ; i ++ ) { B_299_15_0
[ i ] = 0.0 ; for ( i_0 = 0 ; i_0 < 5 ; i_0 ++ ) { B_299_15_0 [ i ] +=
rtb_B_299_15_0 [ 5 * i_0 + i ] * rtb_B_311_5_0 [ i_0 ] ; } rtb_B_311_8_0 [ i
] = _rtP -> P_596 [ i ] * B_299_15_0 [ i ] ; } rtb_B_299_1_0 = rtb_B_311_8_0
[ 0 ] * rtb_B_311_4_1 + rtb_B_311_8_0 [ 1 ] * rtb_B_311_4_0 ; rtb_B_298_4_0 =
( ( - rtb_B_311_8_0 [ 0 ] - 1.7320508075688772 * rtb_B_311_8_0 [ 1 ] ) *
rtb_B_311_4_1 + ( 1.7320508075688772 * rtb_B_311_8_0 [ 0 ] - rtb_B_311_8_0 [
1 ] ) * rtb_B_311_4_0 ) * 0.5 ; _rtB -> B_311_13_0 [ 0 ] = _rtP -> P_597 *
rtb_B_299_1_0 ; _rtB -> B_311_13_0 [ 1 ] = _rtP -> P_597 * rtb_B_298_4_0 ;
_rtB -> B_311_13_0 [ 2 ] = ( ( 0.0 - rtb_B_299_1_0 ) - rtb_B_298_4_0 ) * _rtP
-> P_597 ; i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtB ->
B_311_14_0 [ 0 ] = _rtDW -> RateTransition3_Buffer0 [ 0 ] ; _rtB ->
B_311_14_0 [ 1 ] = _rtDW -> RateTransition3_Buffer0 [ 1 ] ; _rtB ->
B_311_14_0 [ 2 ] = _rtDW -> RateTransition3_Buffer0 [ 2 ] ; } i =
ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_15_0 [ 0 ] =
_rtDW -> RateTransition3_Buffer0_h [ 0 ] ; _rtB -> B_311_15_0 [ 1 ] = _rtDW
-> RateTransition3_Buffer0_h [ 1 ] ; _rtB -> B_311_15_0 [ 2 ] = _rtDW ->
RateTransition3_Buffer0_h [ 2 ] ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i
!= 0 ) { _rtB -> B_311_16_0 [ 0 ] = _rtDW -> RateTransition3_Buffer0_b [ 0 ]
; _rtB -> B_311_16_0 [ 1 ] = _rtDW -> RateTransition3_Buffer0_b [ 1 ] ; _rtB
-> B_311_16_0 [ 2 ] = _rtDW -> RateTransition3_Buffer0_b [ 2 ] ; }
rtb_B_299_1_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE ; B_299_0_0_idx_0 =
_rtDW -> UnitDelay_DSTATE [ 0 ] ; B_299_0_0_idx_1 = _rtDW -> UnitDelay_DSTATE
[ 1 ] ; B_299_0_0_idx_2 = _rtDW -> UnitDelay_DSTATE [ 2 ] ; rtb_B_299_3_0 = (
_rtDW -> UnitDelay_DSTATE [ 0 ] - _rtDW -> UnitDelay_DSTATE [ 1 ] ) * _rtDW
-> DiscreteTimeIntegrator_DSTATE * _rtP -> P_606 ; rtb_B_298_4_0 = _rtP ->
P_607 * _rtDW -> DiscreteTimeIntegrator_DSTATE ; if ( rtb_B_299_3_0 > _rtDW
-> DiscreteTimeIntegrator_DSTATE ) { _rtB -> B_311_25_0 = _rtDW ->
DiscreteTimeIntegrator_DSTATE ; } else if ( rtb_B_299_3_0 < rtb_B_298_4_0 ) {
_rtB -> B_311_25_0 = rtb_B_298_4_0 ; } else { _rtB -> B_311_25_0 =
rtb_B_299_3_0 ; } rtb_B_299_3_0 = ( _rtDW -> UnitDelay_DSTATE [ 1 ] - _rtDW
-> UnitDelay_DSTATE [ 2 ] ) * _rtDW -> DiscreteTimeIntegrator_DSTATE * _rtP
-> P_608 ; if ( rtb_B_299_3_0 > _rtDW -> DiscreteTimeIntegrator_DSTATE ) {
_rtB -> B_311_31_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE ; } else if (
rtb_B_299_3_0 < rtb_B_298_4_0 ) { _rtB -> B_311_31_0 = rtb_B_298_4_0 ; } else
{ _rtB -> B_311_31_0 = rtb_B_299_3_0 ; } rtb_B_298_4_0 = _rtB -> B_311_406_0
- _rtDW -> UnitDelay5_DSTATE ; _rtB -> B_311_34_0 = _rtDW ->
DiscreteTimeIntegrator_DSTATE * rtb_B_298_4_0 ; } i = ssIsSampleHit ( S , 1 ,
0 ) ; if ( i != 0 ) { rtb_B_311_35_0 = _rtDW -> itinit1_PreviousInput ;
rtb_B_311_36_0 = _rtP -> P_611 * _rtDW -> itinit1_PreviousInput ;
rtb_B_311_37_0 = 1.000001 * rtb_B_311_36_0 * 0.96711798839458663 / 0.9999 ; }
i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_38_0 = _rtP
-> P_612 * _rtDW -> Currentfilter_states ; _rtB -> B_311_40_0 = ( _rtB ->
B_311_38_0 > Auto_microgrid_forecasting_020822_rtC ( S ) -> B_311_0_0 ) ; } i
= ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_41_0 = _rtDW ->
itinit_PreviousInput ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { if
( _rtDW -> inti_IC_LOADING != 0 ) { _rtDW -> inti_DSTATE = _rtB -> B_311_41_0
; if ( _rtDW -> inti_DSTATE >= _rtP -> P_617 ) { _rtDW -> inti_DSTATE = _rtP
-> P_617 ; } else { if ( _rtDW -> inti_DSTATE <= _rtP -> P_618 ) { _rtDW ->
inti_DSTATE = _rtP -> P_618 ; } } } if ( ( _rtB -> B_311_40_0 > 0.0 ) && (
_rtDW -> inti_PrevResetState <= 0 ) ) { _rtDW -> inti_DSTATE = _rtB ->
B_311_41_0 ; if ( _rtDW -> inti_DSTATE >= _rtP -> P_617 ) { _rtDW ->
inti_DSTATE = _rtP -> P_617 ; } else { if ( _rtDW -> inti_DSTATE <= _rtP ->
P_618 ) { _rtDW -> inti_DSTATE = _rtP -> P_618 ; } } } if ( _rtDW ->
inti_DSTATE >= _rtP -> P_617 ) { _rtDW -> inti_DSTATE = _rtP -> P_617 ; }
else { if ( _rtDW -> inti_DSTATE <= _rtP -> P_618 ) { _rtDW -> inti_DSTATE =
_rtP -> P_618 ; } } _rtB -> B_311_43_0 = _rtP -> P_619 * _rtDW -> inti_DSTATE
; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { rtb_B_311_44_0 = ( _rtB
-> B_311_43_0 > rtb_B_311_36_0 ) ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i
!= 0 ) { if ( _rtB -> B_311_43_0 < _rtB -> B_311_23_0 ) { _rtB -> B_311_46_0
= _rtB -> B_311_23_0 ; } else { _rtB -> B_311_46_0 = _rtB -> B_311_43_0 ; } }
i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( rtb_B_311_44_0 ) {
rtb_B_311_47_0 = rtb_B_311_36_0 ; } else { rtb_B_311_47_0 = _rtB ->
B_311_46_0 ; } if ( rtb_B_311_37_0 <= rtb_B_311_47_0 ) { rtb_B_311_47_0 =
rtb_B_311_36_0 ; } rtb_B_311_50_0 = - 0.74918148048203947 * rtb_B_311_35_0 /
( rtb_B_311_35_0 - rtb_B_311_47_0 ) * rtb_B_311_47_0 ; rtb_B_311_51_0 = -
_rtB -> B_311_40_0 * 0.74918148048203947 * _rtB -> B_311_38_0 *
rtb_B_311_35_0 / ( rtb_B_311_35_0 - rtb_B_311_47_0 ) ; rtb_B_311_52_0 = _rtP
-> P_620 * rtb_B_311_35_0 ; if ( ! ( _rtB -> B_311_43_0 > rtb_B_311_52_0 ) )
{ rtb_B_311_36_0 = - rtb_B_311_52_0 * 0.999 * 0.1 * 0.9999 ; if ( _rtB ->
B_311_43_0 < rtb_B_311_36_0 ) { rtb_B_311_52_0 = rtb_B_311_36_0 ; } else {
rtb_B_311_52_0 = _rtB -> B_311_43_0 ; } } } i = ssIsSampleHit ( S , 2 , 0 ) ;
if ( i != 0 ) { _rtB -> B_311_57_0 = ( _rtB -> B_311_38_0 <
Auto_microgrid_forecasting_020822_rtC ( S ) -> B_311_1_0 ) ; } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { switch ( ( int32_T ) _rtB ->
B_311_24_0 ) { case 1 : rtb_B_311_62_0 = - ( _rtB -> B_311_28_0 * _rtB ->
B_311_57_0 ) * 0.74918148048203947 * ( _rtB -> B_311_28_0 * _rtB ->
B_311_38_0 ) * ( 10.339999999999973 / ( _rtB -> B_311_28_0 * rtb_B_311_52_0 +
1.0339999999999974 ) ) ; break ; case 2 : rtb_B_311_62_0 = _rtB ->
B_311_25_0_b * rtb_B_311_35_0 ; rtb_B_311_62_0 = - ( _rtB -> B_311_25_0_b *
_rtB -> B_311_57_0 ) * 0.74918148048203947 * ( _rtB -> B_311_25_0_b * _rtB ->
B_311_38_0 ) * rtb_B_311_62_0 / ( _rtB -> B_311_25_0_b * rtb_B_311_52_0 +
rtb_B_311_62_0 * 0.1 ) ; break ; case 3 : rtb_B_311_62_0 = - ( _rtB ->
B_311_27_0 * _rtB -> B_311_57_0 ) * 0.74918148048203947 * ( _rtB ->
B_311_27_0 * _rtB -> B_311_38_0 ) * ( 10.339999999999973 / (
muDoubleScalarAbs ( _rtB -> B_311_27_0 * rtb_B_311_52_0 ) +
1.0339999999999974 ) ) ; break ; default : rtb_B_311_62_0 = - ( _rtB ->
B_311_26_0 * _rtB -> B_311_57_0 ) * 0.74918148048203947 * ( _rtB ->
B_311_26_0 * _rtB -> B_311_38_0 ) * ( 10.339999999999973 / (
muDoubleScalarAbs ( _rtB -> B_311_26_0 * rtb_B_311_52_0 ) +
1.0339999999999974 ) ) ; break ; } } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i
!= 0 ) { _rtB -> B_311_63_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_a ; } i
= ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { switch ( ( int32_T ) _rtB ->
B_311_29_0 ) { case 1 : rtb_B_311_68_0 = _rtB -> B_311_63_0 ; break ; case 2
: if ( rtb_B_311_47_0 > _rtP -> P_67 ) { rtb_B_299_3_0 = _rtP -> P_67 ; }
else if ( rtb_B_311_47_0 < _rtP -> P_68 ) { rtb_B_299_3_0 = _rtP -> P_68 ; }
else { rtb_B_299_3_0 = rtb_B_311_47_0 ; } rtb_B_311_68_0 = muDoubleScalarExp
( - 6.106194690265486 * rtb_B_299_3_0 ) * 83.9759630617608 ; break ; case 3 :
rtb_B_311_68_0 = _rtB -> B_311_63_0 ; break ; default : rtb_B_311_68_0 = _rtB
-> B_311_63_0 ; break ; } rtb_B_311_68_0 = ( ( ( ( rtb_B_311_50_0 +
rtb_B_311_51_0 ) + rtb_B_311_62_0 ) + rtb_B_311_68_0 ) + - 0.0 *
rtb_B_311_47_0 ) + _rtB -> B_311_20_0 ; rtb_B_311_69_0 = ( rtb_B_311_68_0 >
_rtB -> B_311_21_0 ) ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
_rtB -> B_311_70_0 = _rtDW -> Memory2_PreviousInput ; } i = ssIsSampleHit ( S
, 1 , 0 ) ; if ( i != 0 ) { if ( rtb_B_311_69_0 ) { _rtB -> B_311_72_0 = _rtB
-> B_311_21_0 ; } else if ( rtb_B_311_68_0 < _rtB -> B_311_70_0 ) { _rtB ->
B_311_72_0 = _rtB -> B_311_70_0 ; } else { _rtB -> B_311_72_0 =
rtb_B_311_68_0 ; } } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { i =
ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_73_0 = _rtDW ->
UnitDelayToBreakTheLoop_DSTATE ; } _rtB -> B_311_74_0 = _rtDW ->
UnitDelayToBreakTheLoop_DSTATE_f ; } _rtB -> B_311_76_0 = look1_binlxpw (
ssGetT ( S ) , _rtP -> P_627 , _rtP -> P_626 , 11999U ) ; rtb_B_311_62_0 =
_rtP -> P_628 * _rtB -> B_311_76_0 ; _rtB -> B_311_78_0 = _rtB ->
B_311_169_0_j * rtb_B_311_62_0 ; i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i !=
0 ) { _rtB -> B_311_79_0 = _rtDW -> UnitDelay_DSTATE_p ; _rtB -> B_311_80_0 =
_rtP -> P_630 ; i = ssIsSampleHit ( S , 2 , 0 ) ; if ( ( i != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( _rtB -> B_311_80_0 > 0.0 ) { if ( !
_rtDW -> Signalgenerator_MODE ) { if ( ssGetTaskTime ( S , 2 ) != ssGetTStart
( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW ->
Signalgenerator_MODE = true ; } } else { if ( _rtDW -> Signalgenerator_MODE )
{ ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_m1 = _rtB -> B_5_2_0 ; _rtDW ->
DiscreteTimeIntegrator1_DSTATE_i = _rtB -> B_5_8_0 ; _rtB -> B_5_1_0 = _rtP
-> P_27 ; _rtB -> B_5_15_0 = _rtP -> P_4254 ; _rtB -> B_5_17_0 = _rtP -> P_28
; _rtB -> B_5_16_0 = _rtP -> P_29 ; _rtB -> B_5_19_0 = _rtP -> P_30 ; _rtDW
-> Signalgenerator_MODE = false ; } } } if ( _rtDW -> Signalgenerator_MODE )
{ i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_5_1_0 =
rt_Lookup ( _rtP -> P_31 , 6 , ssGetTaskTime ( S , 2 ) , _rtP -> P_32 ) ;
_rtB -> B_5_2_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_m1 ; if (
ssGetTaskTime ( S , 2 ) < _rtP -> P_35 ) { rtb_B_5_3_0 = _rtP -> P_36 ; }
else { rtb_B_5_3_0 = _rtP -> P_37 ; } if ( ssGetTaskTime ( S , 2 ) < _rtP ->
P_38 ) { rtb_B_5_7_0 = _rtP -> P_39 ; } else { rtb_B_5_7_0 = _rtP -> P_40 ; }
_rtB -> B_5_8_0 = _rtDW -> DiscreteTimeIntegrator1_DSTATE_i ; if (
rtb_B_5_3_0 >= _rtP -> P_44 ) { switch ( ( int32_T ) _rtB -> B_5_11_0_n ) {
case 1 : _rtB -> B_5_11_0 = _rtB -> B_5_25_0 * rtb_B_5_7_0 ; break ; case 2 :
_rtB -> B_5_11_0 = _rtB -> B_5_8_0 ; break ; case 3 : _rtB -> B_5_11_0 =
muDoubleScalarSin ( _rtB -> B_5_8_0 * _rtB -> B_5_20_0_p ) * _rtB -> B_5_26_0
; break ; default : _rtB -> B_5_11_0 = _rtB -> B_5_18_0 ; break ; } } else {
_rtB -> B_5_11_0 = _rtDW -> UnitDelay_DSTATE_h5 ; } if ( ( ! ( rtb_B_5_3_0 !=
0.0 ) ) && _rtB -> B_5_22_0 ) { rtb_B_5_3_0 = _rtB -> B_5_14_0_l ; } else {
rtb_B_5_3_0 = _rtB -> B_5_11_0 ; } if ( _rtB -> B_5_10_0 ) { rtb_B_299_3_0 =
rtb_B_5_3_0 ; } else { rtb_B_299_3_0 = _rtB -> B_5_5_0 ; } _rtB -> B_5_14_0 =
_rtP -> P_45 * rtb_B_299_3_0 ; _rtB -> B_5_15_0 = ( _rtB -> B_5_12_0 && _rtB
-> B_5_8_0_i ) ; _rtB -> B_5_16_0 = _rtB -> B_5_2_0 ; if ( _rtB -> B_5_8_0_i
) { _rtB -> B_5_17_0 = rtb_B_5_3_0 ; } else { _rtB -> B_5_17_0 = _rtB ->
B_5_1_0_o ; } if ( _rtB -> B_5_9_0 ) { _rtB -> B_5_19_0 = _rtP -> P_26 *
rtb_B_5_3_0 ; } else { _rtB -> B_5_19_0 = _rtB -> B_5_4_0 ; } if (
rtb_B_5_7_0 >= _rtP -> P_46 ) { _rtB -> B_5_20_0 = _rtB -> B_5_24_0 ; } else
{ _rtB -> B_5_20_0 = _rtB -> B_5_17_0_p ; } } if ( ssIsMajorTimeStep ( S ) !=
0 ) { srUpdateBC ( _rtDW -> Signalgenerator_SubsysRanBC ) ; } } rtb_B_5_7_0 =
_rtB -> B_5_17_0 + _rtB -> B_311_554_0 ; if ( _rtB -> B_5_15_0 ) {
rtb_B_5_3_0 = _rtB -> B_5_1_0 ; } else { rtb_B_5_3_0 = rtb_B_5_7_0 ; }
B_299_0_0_idx_0 = rtb_B_5_3_0 ; if ( _rtB -> B_311_553_0 >= _rtP -> P_631 ) {
B_299_0_0_idx_1 = rtb_B_5_7_0 ; B_299_0_0_idx_2 = rtb_B_5_7_0 ; } else {
B_299_0_0_idx_1 = rtb_B_5_3_0 ; B_299_0_0_idx_2 = rtb_B_5_3_0 ; } rtb_B_5_7_0
= ssGetTaskTime ( S , 2 ) * _rtB -> B_311_547_0 + _rtB -> B_5_16_0 ;
rtb_B_311_50_0 = ( _rtB -> B_311_549_0 + _rtB -> B_311_551_0_e [ 0 ] ) + _rtB
-> B_5_19_0 ; rtb_B_311_68_0 = ( _rtB -> B_311_549_0 + _rtB -> B_311_551_0_e
[ 1 ] ) + _rtB -> B_5_19_0 ; rtb_B_311_51_0 = ( _rtB -> B_311_549_0 + _rtB ->
B_311_551_0_e [ 2 ] ) + _rtB -> B_5_19_0 ; B_311_92_0 = _rtP -> P_632 ; i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( ( i != 0 ) && ( ssIsMajorTimeStep ( S ) !=
0 ) ) { if ( B_311_92_0 > 0.0 ) { if ( ! _rtDW -> HarmonicGenerator_MODE ) {
if ( ssGetTaskTime ( S , 2 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW ->
HarmonicGenerator_MODE = true ; } } else { if ( _rtDW ->
HarmonicGenerator_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
_rtB -> B_0_6_0 [ 0 ] = _rtP -> P_0 ; _rtB -> B_0_10_0 [ 0 ] = _rtP -> P_1 ;
_rtB -> B_0_6_0 [ 1 ] = _rtP -> P_0 ; _rtB -> B_0_10_0 [ 1 ] = _rtP -> P_1 ;
_rtB -> B_0_6_0 [ 2 ] = _rtP -> P_0 ; _rtB -> B_0_10_0 [ 2 ] = _rtP -> P_1 ;
_rtDW -> HarmonicGenerator_MODE = false ; } } } if ( _rtDW ->
HarmonicGenerator_MODE ) { i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
rtb_B_0_0_0 = _rtP -> P_2 * rtb_B_5_7_0 ; if ( ssGetTaskTime ( S , 2 ) < _rtP
-> P_3 ) { rtb_B_299_3_0 = _rtP -> P_4 ; } else { rtb_B_299_3_0 = _rtP -> P_5
; } if ( ssGetTaskTime ( S , 2 ) < _rtP -> P_6 ) { B_311_401_0_idx_1 = _rtP
-> P_7 ; } else { B_311_401_0_idx_1 = _rtP -> P_8 ; } rtb_B_5_3_0 =
rtb_B_299_3_0 + B_311_401_0_idx_1 ; _rtB -> B_0_6_0 [ 0 ] = muDoubleScalarSin
( ( rtb_B_0_0_0 + _rtB -> B_0_1_0 ) + _rtB -> B_0_8_0 [ 0 ] ) * ( rtb_B_5_3_0
* _rtB -> B_0_9_0 ) ; _rtB -> B_0_6_0 [ 1 ] = muDoubleScalarSin ( (
rtb_B_0_0_0 + _rtB -> B_0_1_0 ) + _rtB -> B_0_8_0 [ 1 ] ) * ( rtb_B_5_3_0 *
_rtB -> B_0_9_0 ) ; _rtB -> B_0_6_0 [ 2 ] = muDoubleScalarSin ( ( rtb_B_0_0_0
+ _rtB -> B_0_1_0 ) + _rtB -> B_0_8_0 [ 2 ] ) * ( rtb_B_5_3_0 * _rtB ->
B_0_9_0 ) ; rtb_B_0_0_0 = _rtP -> P_9 * rtb_B_5_7_0 ; _rtB -> B_0_10_0 [ 0 ]
= muDoubleScalarSin ( ( rtb_B_0_0_0 + _rtB -> B_0_11_0 ) + _rtB -> B_0_18_0 [
0 ] ) * ( rtb_B_5_3_0 * _rtB -> B_0_19_0 ) ; _rtB -> B_0_10_0 [ 1 ] =
muDoubleScalarSin ( ( rtb_B_0_0_0 + _rtB -> B_0_11_0 ) + _rtB -> B_0_18_0 [ 1
] ) * ( rtb_B_5_3_0 * _rtB -> B_0_19_0 ) ; _rtB -> B_0_10_0 [ 2 ] =
muDoubleScalarSin ( ( rtb_B_0_0_0 + _rtB -> B_0_11_0 ) + _rtB -> B_0_18_0 [ 2
] ) * ( rtb_B_5_3_0 * _rtB -> B_0_19_0 ) ; } if ( ssIsMajorTimeStep ( S ) !=
0 ) { srUpdateBC ( _rtDW -> HarmonicGenerator_SubsysRanBC ) ; } } _rtB ->
B_311_94_0 [ 0 ] = ( muDoubleScalarSin ( rtb_B_5_7_0 + rtb_B_311_50_0 ) *
B_299_0_0_idx_0 + _rtB -> B_0_6_0 [ 0 ] ) + _rtB -> B_0_10_0 [ 0 ] ; _rtB ->
B_311_94_0 [ 1 ] = ( muDoubleScalarSin ( rtb_B_5_7_0 + rtb_B_311_68_0 ) *
B_299_0_0_idx_1 + _rtB -> B_0_6_0 [ 1 ] ) + _rtB -> B_0_10_0 [ 1 ] ; _rtB ->
B_311_94_0 [ 2 ] = ( muDoubleScalarSin ( rtb_B_5_7_0 + rtb_B_311_51_0 ) *
B_299_0_0_idx_2 + _rtB -> B_0_6_0 [ 2 ] ) + _rtB -> B_0_10_0 [ 2 ] ;
rtb_B_5_7_0 = _rtP -> P_635 * ssGetTaskTime ( S , 2 ) + _rtDW ->
Rotorangledthetae_DSTATE_g ; rtb_B_0_0_0 = muDoubleScalarSin ( rtb_B_5_7_0 )
; rtb_B_311_100_0 = muDoubleScalarCos ( rtb_B_5_7_0 ) ; rtb_B_311_50_0 = _rtP
-> P_636 * rtb_B_0_0_0 * _rtP -> P_637 ; rtb_B_299_3_0 = _rtP -> P_636 *
rtb_B_311_100_0 * _rtP -> P_637 ; rtb_B_5_3_0 = 2.0 * rtb_B_299_3_0 ;
rtb_B_5_7_0 = 2.0 * rtb_B_311_50_0 ; B_311_104_0_idx_1 = 1.7320508075688772 *
rtb_B_311_50_0 - rtb_B_299_3_0 ; rtb_B_299_3_0 = - rtb_B_311_50_0 -
1.7320508075688772 * rtb_B_299_3_0 ; for ( i = 0 ; i < 5 ; i ++ ) {
B_311_104_0_0 [ i << 1 ] = 0.0 ; B_311_104_0_0 [ i << 1 ] += _rtB ->
B_311_579_0 [ i << 1 ] * rtb_B_5_3_0 ; B_311_104_0_0 [ i << 1 ] += _rtB ->
B_311_579_0 [ ( i << 1 ) + 1 ] * rtb_B_5_7_0 ; B_311_104_0_0 [ ( i << 1 ) + 1
] = 0.0 ; B_311_104_0_0 [ ( i << 1 ) + 1 ] += _rtB -> B_311_579_0 [ i << 1 ]
* B_311_104_0_idx_1 ; B_311_104_0_0 [ ( i << 1 ) + 1 ] += _rtB -> B_311_579_0
[ ( i << 1 ) + 1 ] * rtb_B_299_3_0 ; } for ( i = 0 ; i < 10 ; i ++ ) {
rtb_B_311_111_0 [ i ] = _rtP -> P_638 * B_311_104_0_0 [ i ] ; } _rtB ->
B_311_113_0 = _rtDW -> dw_delay_DSTATE ; rtb_B_5_7_0 = _rtP -> P_640 * _rtB
-> B_311_113_0 - _rtDW -> dw_predict_DSTATE ; rtb_B_5_3_0 = _rtB ->
B_311_585_0_a + rtb_B_5_7_0 ; memcpy ( & rtb_B_299_15_0 [ 0 ] , & _rtB ->
B_311_573_0 [ 0 ] , 25U * sizeof ( real_T ) ) ; rtb_B_299_15_0 [ 5 ] =
rtb_B_5_3_0 ; rtb_B_299_15_0 [ 1 ] = _rtP -> P_642 * rtb_B_5_3_0 ; for ( i =
0 ; i < 25 ; i ++ ) { B_311_104_0_idx_1 = ( ( 0.0 - rtb_B_299_15_0 [ i ] ) -
_rtB -> B_311_575_0_e [ i ] ) * _rtP -> P_643 * _rtP -> P_644 ; B_300_1_0 [ i
] = _rtB -> B_311_576_0 [ i ] - B_311_104_0_idx_1 ; rtb_B_299_15_0 [ i ] =
B_311_104_0_idx_1 ; } rt_invd5x5_snf ( B_300_1_0 , rtb_B_311_124_0 ) ; for (
i = 0 ; i < 5 ; i ++ ) { for ( i_0 = 0 ; i_0 < 2 ; i_0 ++ ) { B_311_104_0_0 [
i_0 + ( i << 1 ) ] = 0.0 ; for ( i_1 = 0 ; i_1 < 5 ; i_1 ++ ) { B_311_104_0_0
[ i_0 + ( i << 1 ) ] += rtb_B_311_111_0 [ ( i_1 << 1 ) + i_0 ] *
rtb_B_311_124_0 [ 5 * i + i_1 ] ; } } } for ( i = 0 ; i < 10 ; i ++ ) {
rtb_B_311_111_0 [ i ] = _rtP -> P_645 * B_311_104_0_0 [ i ] ; } for ( i = 0 ;
i < 5 ; i ++ ) { rtb_B_311_128_0 [ i ] = _rtDW -> UnitDelay_DSTATE_f [ i ] ;
} rtb_B_311_50_0 = _rtP -> P_647 * rtb_B_0_0_0 * _rtP -> P_648 ;
rtb_B_299_3_0 = _rtP -> P_647 * rtb_B_311_100_0 * _rtP -> P_648 ; memcpy ( &
rtb_B_311_133_0 [ 0 ] , & _rtB -> B_311_584_0_c [ 0 ] , 15U * sizeof ( real_T
) ) ; rtb_B_311_133_0 [ 0 ] = 2.0 * rtb_B_299_3_0 ; rtb_B_311_133_0 [ 5 ] =
1.7320508075688772 * rtb_B_311_50_0 + rtb_B_299_3_0 ; rtb_B_311_133_0 [ 1 ] =
2.0 * rtb_B_311_50_0 ; rtb_B_311_133_0 [ 6 ] = rtb_B_311_50_0 -
1.7320508075688772 * rtb_B_299_3_0 ; for ( i = 0 ; i < 15 ; i ++ ) {
rtb_B_311_140_0 [ i ] = _rtP -> P_649 * rtb_B_311_133_0 [ i ] ; } for ( i = 0
; i < 3 ; i ++ ) { for ( i_0 = 0 ; i_0 < 2 ; i_0 ++ ) { B_311_126_0_0 [ i_0 +
( i << 1 ) ] = 0.0 ; for ( i_1 = 0 ; i_1 < 5 ; i_1 ++ ) { B_311_126_0_0 [ i_0
+ ( i << 1 ) ] += rtb_B_311_111_0 [ ( i_1 << 1 ) + i_0 ] * rtb_B_311_140_0 [
5 * i + i_1 ] ; } } } for ( i = 0 ; i < 6 ; i ++ ) { rtb_B_311_141_0 [ i ] =
_rtP -> P_650 * B_311_126_0_0 [ i ] ; } _rtB -> B_311_144_0 = _rtDW ->
UnitDelay_DSTATE_k ; _rtB -> B_311_145_0 = _rtB -> B_311_144_0 ;
B_311_104_0_idx_1 = _rtP -> P_653 * _rtB -> B_311_145_0 ; for ( i = 0 ; i < 2
; i ++ ) { B_311_126_0_1 [ i ] = 0.0 ; for ( i_0 = 0 ; i_0 < 5 ; i_0 ++ ) {
B_311_126_0_1 [ i ] += rtb_B_311_111_0 [ ( i_0 << 1 ) + i ] * _rtDW ->
UnitDelay_DSTATE_f [ i_0 ] ; } _rtB -> B_311_148_0 [ i ] = rtb_B_311_141_0 [
i + 4 ] * B_311_104_0_idx_1 + B_311_126_0_1 [ i ] ; _rtB -> B_311_149_0 [ i
<< 1 ] = rtb_B_311_141_0 [ i << 1 ] ; _rtB -> B_311_149_0 [ ( i << 1 ) + 1 ]
= rtb_B_311_141_0 [ ( i << 1 ) + 1 ] ; } _rtB -> B_311_151_0 [ 0 ] = _rtB ->
B_311_149_0 [ 0 ] - _rtDW -> UnitDelay_DSTATE_pe [ 0 ] ; _rtB -> B_311_151_0
[ 1 ] = _rtB -> B_311_149_0 [ 1 ] - _rtDW -> UnitDelay_DSTATE_pe [ 1 ] ; _rtB
-> B_311_151_0 [ 2 ] = _rtB -> B_311_149_0 [ 2 ] - _rtDW ->
UnitDelay_DSTATE_pe [ 2 ] ; _rtB -> B_311_151_0 [ 3 ] = _rtB -> B_311_149_0 [
3 ] - _rtDW -> UnitDelay_DSTATE_pe [ 3 ] ; ssCallAccelRunBlock ( S , 311 ,
152 , SS_CALL_MDL_OUTPUTS ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0
) { _rtB -> B_311_153_0 = _rtDW -> Memory_PreviousInput ; } i = ssIsSampleHit
( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_157_0 [ 0 ] = _rtP -> P_704 *
_rtB -> B_311_152_0 [ 60 ] * _rtP -> P_707 ; _rtB -> B_311_157_0 [ 1 ] = _rtP
-> P_705 * _rtB -> B_311_152_0 [ 61 ] * _rtP -> P_707 ; _rtB -> B_311_157_0 [
2 ] = _rtP -> P_706 * _rtB -> B_311_152_0 [ 62 ] * _rtP -> P_707 ; if ( _rtDW
-> systemEnable != 0 ) { _rtDW -> lastSin = muDoubleScalarSin ( _rtP -> P_710
* ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos = muDoubleScalarCos ( _rtP ->
P_710 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable = 0 ; } _rtB ->
B_311_159_0 = ( ( ( _rtDW -> lastSin * _rtP -> P_714 + _rtDW -> lastCos *
_rtP -> P_713 ) * _rtP -> P_712 + ( _rtDW -> lastCos * _rtP -> P_714 - _rtDW
-> lastSin * _rtP -> P_713 ) * _rtP -> P_711 ) * _rtP -> P_708 + _rtP ->
P_709 ) * _rtB -> B_311_157_0 [ 0 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE != 0
) { _rtB -> B_311_160_0 = _rtDW -> Integ4_DSTATE ; } else { _rtB ->
B_311_160_0 = _rtP -> P_715 * _rtB -> B_311_159_0 + _rtDW -> Integ4_DSTATE ;
} _rtB -> B_311_161_0 = _rtP -> P_717 ; ssCallAccelRunBlock ( S , 311 , 162 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_161_0 )
{ _rtB -> B_311_169_0 = ( _rtB -> B_311_160_0 - _rtB -> B_311_162_0 ) * _rtP
-> P_726 + ( _rtP -> P_223 * _rtB -> B_311_159_0 - _rtP -> P_222 * _rtDW ->
UnitDelay_DSTATE_kk ) ; } else { _rtB -> B_311_169_0 = _rtDW ->
UnitDelay1_DSTATE ; } if ( _rtDW -> systemEnable_l != 0 ) { _rtDW ->
lastSin_b = muDoubleScalarSin ( _rtP -> P_731 * ssGetTaskTime ( S , 2 ) ) ;
_rtDW -> lastCos_k = muDoubleScalarCos ( _rtP -> P_731 * ssGetTaskTime ( S ,
2 ) ) ; _rtDW -> systemEnable_l = 0 ; } _rtB -> B_311_171_0 = ( ( ( _rtDW ->
lastSin_b * _rtP -> P_735 + _rtDW -> lastCos_k * _rtP -> P_734 ) * _rtP ->
P_733 + ( _rtDW -> lastCos_k * _rtP -> P_735 - _rtDW -> lastSin_b * _rtP ->
P_734 ) * _rtP -> P_732 ) * _rtP -> P_729 + _rtP -> P_730 ) * _rtB ->
B_311_157_0 [ 0 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_c != 0 ) { _rtB ->
B_311_172_0 = _rtDW -> Integ4_DSTATE_g ; } else { _rtB -> B_311_172_0 = _rtP
-> P_736 * _rtB -> B_311_171_0 + _rtDW -> Integ4_DSTATE_g ; } _rtB ->
B_311_173_0 = _rtP -> P_738 ; ssCallAccelRunBlock ( S , 311 , 174 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_173_0 )
{ _rtB -> B_311_181_0 = ( _rtB -> B_311_172_0 - _rtB -> B_311_174_0 ) * _rtP
-> P_747 + ( _rtP -> P_221 * _rtB -> B_311_171_0 - _rtP -> P_220 * _rtDW ->
UnitDelay_DSTATE_fl ) ; } else { _rtB -> B_311_181_0 = _rtDW ->
UnitDelay1_DSTATE_h ; } rtb_B_311_50_0 = _rtP -> P_750 * muDoubleScalarAtan2
( _rtB -> B_311_181_0 , _rtB -> B_311_169_0 ) * _rtP -> P_751 ; B_311_186_0 .
re = muDoubleScalarHypot ( _rtB -> B_311_169_0 , _rtB -> B_311_181_0 ) *
muDoubleScalarCos ( rtb_B_311_50_0 ) ; B_311_186_0 . im = muDoubleScalarHypot
( _rtB -> B_311_169_0 , _rtB -> B_311_181_0 ) * muDoubleScalarSin (
rtb_B_311_50_0 ) ; if ( _rtDW -> systemEnable_d != 0 ) { _rtDW -> lastSin_bv
= muDoubleScalarSin ( _rtP -> P_754 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
lastCos_c = muDoubleScalarCos ( _rtP -> P_754 * ssGetTaskTime ( S , 2 ) ) ;
_rtDW -> systemEnable_d = 0 ; } _rtB -> B_311_188_0 = ( ( ( _rtDW ->
lastSin_bv * _rtP -> P_758 + _rtDW -> lastCos_c * _rtP -> P_757 ) * _rtP ->
P_756 + ( _rtDW -> lastCos_c * _rtP -> P_758 - _rtDW -> lastSin_bv * _rtP ->
P_757 ) * _rtP -> P_755 ) * _rtP -> P_752 + _rtP -> P_753 ) * _rtB ->
B_311_157_0 [ 1 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_l != 0 ) { _rtB ->
B_311_189_0 = _rtDW -> Integ4_DSTATE_j ; } else { _rtB -> B_311_189_0 = _rtP
-> P_759 * _rtB -> B_311_188_0 + _rtDW -> Integ4_DSTATE_j ; } _rtB ->
B_311_190_0 = _rtP -> P_761 ; ssCallAccelRunBlock ( S , 311 , 191 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_190_0 )
{ _rtB -> B_311_198_0 = ( _rtB -> B_311_189_0 - _rtB -> B_311_191_0 ) * _rtP
-> P_770 + ( _rtP -> P_227 * _rtB -> B_311_188_0 - _rtP -> P_226 * _rtDW ->
UnitDelay_DSTATE_fli ) ; } else { _rtB -> B_311_198_0 = _rtDW ->
UnitDelay1_DSTATE_b ; } if ( _rtDW -> systemEnable_m != 0 ) { _rtDW ->
lastSin_p = muDoubleScalarSin ( _rtP -> P_775 * ssGetTaskTime ( S , 2 ) ) ;
_rtDW -> lastCos_l = muDoubleScalarCos ( _rtP -> P_775 * ssGetTaskTime ( S ,
2 ) ) ; _rtDW -> systemEnable_m = 0 ; } _rtB -> B_311_200_0 = ( ( ( _rtDW ->
lastSin_p * _rtP -> P_779 + _rtDW -> lastCos_l * _rtP -> P_778 ) * _rtP ->
P_777 + ( _rtDW -> lastCos_l * _rtP -> P_779 - _rtDW -> lastSin_p * _rtP ->
P_778 ) * _rtP -> P_776 ) * _rtP -> P_773 + _rtP -> P_774 ) * _rtB ->
B_311_157_0 [ 1 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_a != 0 ) { _rtB ->
B_311_201_0 = _rtDW -> Integ4_DSTATE_n ; } else { _rtB -> B_311_201_0 = _rtP
-> P_780 * _rtB -> B_311_200_0 + _rtDW -> Integ4_DSTATE_n ; } _rtB ->
B_311_202_0 = _rtP -> P_782 ; ssCallAccelRunBlock ( S , 311 , 203 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_202_0 )
{ _rtB -> B_311_210_0 = ( _rtB -> B_311_201_0 - _rtB -> B_311_203_0 ) * _rtP
-> P_791 + ( _rtP -> P_225 * _rtB -> B_311_200_0 - _rtP -> P_224 * _rtDW ->
UnitDelay_DSTATE_g ) ; } else { _rtB -> B_311_210_0 = _rtDW ->
UnitDelay1_DSTATE_d ; } rtb_B_311_50_0 = _rtP -> P_794 * muDoubleScalarAtan2
( _rtB -> B_311_210_0 , _rtB -> B_311_198_0 ) * _rtP -> P_795 ; B_311_215_0 .
re = muDoubleScalarHypot ( _rtB -> B_311_198_0 , _rtB -> B_311_210_0 ) *
muDoubleScalarCos ( rtb_B_311_50_0 ) ; B_311_215_0 . im = muDoubleScalarHypot
( _rtB -> B_311_198_0 , _rtB -> B_311_210_0 ) * muDoubleScalarSin (
rtb_B_311_50_0 ) ; if ( _rtDW -> systemEnable_h != 0 ) { _rtDW -> lastSin_a =
muDoubleScalarSin ( _rtP -> P_798 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
lastCos_m = muDoubleScalarCos ( _rtP -> P_798 * ssGetTaskTime ( S , 2 ) ) ;
_rtDW -> systemEnable_h = 0 ; } _rtB -> B_311_217_0 = ( ( ( _rtDW ->
lastSin_a * _rtP -> P_802 + _rtDW -> lastCos_m * _rtP -> P_801 ) * _rtP ->
P_800 + ( _rtDW -> lastCos_m * _rtP -> P_802 - _rtDW -> lastSin_a * _rtP ->
P_801 ) * _rtP -> P_799 ) * _rtP -> P_796 + _rtP -> P_797 ) * _rtB ->
B_311_157_0 [ 2 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_f != 0 ) { _rtB ->
B_311_218_0 = _rtDW -> Integ4_DSTATE_gk ; } else { _rtB -> B_311_218_0 = _rtP
-> P_803 * _rtB -> B_311_217_0 + _rtDW -> Integ4_DSTATE_gk ; } _rtB ->
B_311_219_0 = _rtP -> P_805 ; ssCallAccelRunBlock ( S , 311 , 220 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_219_0 )
{ _rtB -> B_311_227_0 = ( _rtB -> B_311_218_0 - _rtB -> B_311_220_0 ) * _rtP
-> P_814 + ( _rtP -> P_231 * _rtB -> B_311_217_0 - _rtP -> P_230 * _rtDW ->
UnitDelay_DSTATE_o ) ; } else { _rtB -> B_311_227_0 = _rtDW ->
UnitDelay1_DSTATE_j ; } if ( _rtDW -> systemEnable_c != 0 ) { _rtDW ->
lastSin_c = muDoubleScalarSin ( _rtP -> P_819 * ssGetTaskTime ( S , 2 ) ) ;
_rtDW -> lastCos_p = muDoubleScalarCos ( _rtP -> P_819 * ssGetTaskTime ( S ,
2 ) ) ; _rtDW -> systemEnable_c = 0 ; } _rtB -> B_311_229_0 = ( ( ( _rtDW ->
lastSin_c * _rtP -> P_823 + _rtDW -> lastCos_p * _rtP -> P_822 ) * _rtP ->
P_821 + ( _rtDW -> lastCos_p * _rtP -> P_823 - _rtDW -> lastSin_c * _rtP ->
P_822 ) * _rtP -> P_820 ) * _rtP -> P_817 + _rtP -> P_818 ) * _rtB ->
B_311_157_0 [ 2 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_d != 0 ) { _rtB ->
B_311_230_0 = _rtDW -> Integ4_DSTATE_h ; } else { _rtB -> B_311_230_0 = _rtP
-> P_824 * _rtB -> B_311_229_0 + _rtDW -> Integ4_DSTATE_h ; } _rtB ->
B_311_231_0 = _rtP -> P_826 ; ssCallAccelRunBlock ( S , 311 , 232 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_231_0 )
{ _rtB -> B_311_239_0 = ( _rtB -> B_311_230_0 - _rtB -> B_311_232_0 ) * _rtP
-> P_835 + ( _rtP -> P_229 * _rtB -> B_311_229_0 - _rtP -> P_228 * _rtDW ->
UnitDelay_DSTATE_b ) ; } else { _rtB -> B_311_239_0 = _rtDW ->
UnitDelay1_DSTATE_p ; } rtb_B_311_50_0 = _rtP -> P_838 * muDoubleScalarAtan2
( _rtB -> B_311_239_0 , _rtB -> B_311_227_0 ) * _rtP -> P_839 ; B_311_244_0 .
re = muDoubleScalarHypot ( _rtB -> B_311_227_0 , _rtB -> B_311_239_0 ) *
muDoubleScalarCos ( rtb_B_311_50_0 ) ; B_311_244_0 . im = muDoubleScalarHypot
( _rtB -> B_311_227_0 , _rtB -> B_311_239_0 ) * muDoubleScalarSin (
rtb_B_311_50_0 ) ; Auto_microgrid_forecasting_020822_NegSeqComputation ( S ,
_rtB -> B_311_274_0 , B_311_186_0 , B_311_215_0 , B_311_244_0 , & _rtB ->
PosSeqComputation_f , & _rtDW -> PosSeqComputation_f , & _rtP ->
PosSeqComputation_f ) ; Auto_microgrid_forecasting_020822_NegSeqComputation (
S , _rtB -> B_311_275_0_o , B_311_186_0 , B_311_215_0 , B_311_244_0 , & _rtB
-> NegSeqComputation_i , & _rtDW -> NegSeqComputation_i , & _rtP ->
NegSeqComputation_i ) ; Auto_microgrid_forecasting_020822_ZeroSeqComputation
( S , _rtB -> B_311_276_0_o , B_311_186_0 , B_311_215_0 , B_311_244_0 , &
_rtB -> ZeroSeqComputation_a , & _rtDW -> ZeroSeqComputation_a , & _rtP ->
ZeroSeqComputation_a ) ; B_299_0_0_idx_0 = muDoubleScalarHypot ( _rtB ->
PosSeqComputation_f . B_122_2_0 . re , _rtB -> PosSeqComputation_f .
B_122_2_0 . im ) ; B_299_0_0_idx_1 = muDoubleScalarHypot ( _rtB ->
NegSeqComputation_i . B_122_2_0 . re , _rtB -> NegSeqComputation_i .
B_122_2_0 . im ) ; B_299_0_0_idx_2 = muDoubleScalarHypot ( _rtB ->
ZeroSeqComputation_a . B_124_1_0 . re , _rtB -> ZeroSeqComputation_a .
B_124_1_0 . im ) ; _rtB -> B_311_249_0 = _rtP -> P_840 * B_299_0_0_idx_0 ; }
i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { rtb_B_311_251_0 = _rtP ->
P_841 ; _rtB -> B_311_253_0_f = ( _rtP -> P_841 - _rtB -> B_311_250_0 [ 0 ]
<= Auto_microgrid_forecasting_020822_rtC ( S ) -> B_311_3_0 ) ; _rtB ->
B_311_254_0 = _rtDW -> Memory_PreviousInput_j ; } _rtB -> B_311_255_0 = _rtP
-> P_843 * _rtB -> B_311_254_0 ; i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i !=
0 ) { if ( _rtDW -> Integrator_IC_LOADING != 0 ) { _rtDW -> Integrator_DSTATE
= _rtB -> B_311_255_0 ; if ( _rtDW -> Integrator_DSTATE >= _rtP -> P_845 ) {
_rtDW -> Integrator_DSTATE = _rtP -> P_845 ; } else { if ( _rtDW ->
Integrator_DSTATE <= _rtP -> P_846 ) { _rtDW -> Integrator_DSTATE = _rtP ->
P_846 ; } } } if ( _rtB -> B_311_253_0_f || ( _rtDW ->
Integrator_PrevResetState != 0 ) ) { _rtDW -> Integrator_DSTATE = _rtB ->
B_311_255_0 ; if ( _rtDW -> Integrator_DSTATE >= _rtP -> P_845 ) { _rtDW ->
Integrator_DSTATE = _rtP -> P_845 ; } else { if ( _rtDW -> Integrator_DSTATE
<= _rtP -> P_846 ) { _rtDW -> Integrator_DSTATE = _rtP -> P_846 ; } } } if (
_rtDW -> Integrator_DSTATE >= _rtP -> P_845 ) { _rtDW -> Integrator_DSTATE =
_rtP -> P_845 ; } else { if ( _rtDW -> Integrator_DSTATE <= _rtP -> P_846 ) {
_rtDW -> Integrator_DSTATE = _rtP -> P_846 ; } } if ( _rtDW ->
Integrator_DSTATE > _rtP -> P_847 ) { _rtB -> B_311_257_0 = _rtP -> P_847 ; }
else if ( _rtDW -> Integrator_DSTATE < _rtP -> P_848 ) { _rtB -> B_311_257_0
= _rtP -> P_848 ; } else { _rtB -> B_311_257_0 = _rtDW -> Integrator_DSTATE ;
} _rtB -> B_311_261_0 [ 0 ] = _rtP -> P_849 * _rtB -> B_311_152_0 [ 48 ] *
_rtP -> P_852 ; _rtB -> B_311_261_0 [ 1 ] = _rtP -> P_850 * _rtB ->
B_311_152_0 [ 49 ] * _rtP -> P_852 ; _rtB -> B_311_261_0 [ 2 ] = _rtP ->
P_851 * _rtB -> B_311_152_0 [ 50 ] * _rtP -> P_852 ; if ( _rtDW ->
systemEnable_lp != 0 ) { _rtDW -> lastSin_i = muDoubleScalarSin ( _rtP ->
P_855 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_lq = muDoubleScalarCos (
_rtP -> P_855 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_lp = 0 ; }
_rtB -> B_311_263_0 = ( ( ( _rtDW -> lastSin_i * _rtP -> P_859 + _rtDW ->
lastCos_lq * _rtP -> P_858 ) * _rtP -> P_857 + ( _rtDW -> lastCos_lq * _rtP
-> P_859 - _rtDW -> lastSin_i * _rtP -> P_858 ) * _rtP -> P_856 ) * _rtP ->
P_853 + _rtP -> P_854 ) * _rtB -> B_311_261_0 [ 0 ] ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_h != 0 ) { _rtB -> B_311_264_0 = _rtDW ->
Integ4_DSTATE_o ; } else { _rtB -> B_311_264_0 = _rtP -> P_860 * _rtB ->
B_311_263_0 + _rtDW -> Integ4_DSTATE_o ; } _rtB -> B_311_265_0 = _rtP ->
P_862 ; ssCallAccelRunBlock ( S , 311 , 266 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_265_0 ) { _rtB -> B_311_273_0 = (
_rtB -> B_311_264_0 - _rtB -> B_311_266_0 ) * _rtP -> P_871 + ( _rtP -> P_247
* _rtB -> B_311_263_0 - _rtP -> P_246 * _rtDW -> UnitDelay_DSTATE_c ) ; }
else { _rtB -> B_311_273_0 = _rtDW -> UnitDelay1_DSTATE_i ; } if ( _rtDW ->
systemEnable_p != 0 ) { _rtDW -> lastSin_av = muDoubleScalarSin ( _rtP ->
P_876 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_mv = muDoubleScalarCos (
_rtP -> P_876 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_p = 0 ; }
_rtB -> B_311_275_0 = ( ( ( _rtDW -> lastSin_av * _rtP -> P_880 + _rtDW ->
lastCos_mv * _rtP -> P_879 ) * _rtP -> P_878 + ( _rtDW -> lastCos_mv * _rtP
-> P_880 - _rtDW -> lastSin_av * _rtP -> P_879 ) * _rtP -> P_877 ) * _rtP ->
P_874 + _rtP -> P_875 ) * _rtB -> B_311_261_0 [ 0 ] ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_k != 0 ) { _rtB -> B_311_276_0 = _rtDW ->
Integ4_DSTATE_a ; } else { _rtB -> B_311_276_0 = _rtP -> P_881 * _rtB ->
B_311_275_0 + _rtDW -> Integ4_DSTATE_a ; } _rtB -> B_311_277_0 = _rtP ->
P_883 ; ssCallAccelRunBlock ( S , 311 , 278 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_277_0 ) { _rtB -> B_311_285_0 = (
_rtB -> B_311_276_0 - _rtB -> B_311_278_0 ) * _rtP -> P_892 + ( _rtP -> P_245
* _rtB -> B_311_275_0 - _rtP -> P_244 * _rtDW -> UnitDelay_DSTATE_a ) ; }
else { _rtB -> B_311_285_0 = _rtDW -> UnitDelay1_DSTATE_c ; } rtb_B_311_50_0
= _rtP -> P_895 * muDoubleScalarAtan2 ( _rtB -> B_311_285_0 , _rtB ->
B_311_273_0 ) * _rtP -> P_896 ; B_311_290_0 . re = muDoubleScalarHypot ( _rtB
-> B_311_273_0 , _rtB -> B_311_285_0 ) * muDoubleScalarCos ( rtb_B_311_50_0 )
; B_311_290_0 . im = muDoubleScalarHypot ( _rtB -> B_311_273_0 , _rtB ->
B_311_285_0 ) * muDoubleScalarSin ( rtb_B_311_50_0 ) ; if ( _rtDW ->
systemEnable_o != 0 ) { _rtDW -> lastSin_n = muDoubleScalarSin ( _rtP ->
P_899 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_d = muDoubleScalarCos (
_rtP -> P_899 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_o = 0 ; }
_rtB -> B_311_292_0 = ( ( ( _rtDW -> lastSin_n * _rtP -> P_903 + _rtDW ->
lastCos_d * _rtP -> P_902 ) * _rtP -> P_901 + ( _rtDW -> lastCos_d * _rtP ->
P_903 - _rtDW -> lastSin_n * _rtP -> P_902 ) * _rtP -> P_900 ) * _rtP ->
P_897 + _rtP -> P_898 ) * _rtB -> B_311_261_0 [ 1 ] ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_o != 0 ) { _rtB -> B_311_293_0 = _rtDW ->
Integ4_DSTATE_e ; } else { _rtB -> B_311_293_0 = _rtP -> P_904 * _rtB ->
B_311_292_0 + _rtDW -> Integ4_DSTATE_e ; } _rtB -> B_311_294_0 = _rtP ->
P_906 ; ssCallAccelRunBlock ( S , 311 , 295 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_294_0 ) { _rtB -> B_311_302_0 = (
_rtB -> B_311_293_0 - _rtB -> B_311_295_0 ) * _rtP -> P_915 + ( _rtP -> P_251
* _rtB -> B_311_292_0 - _rtP -> P_250 * _rtDW -> UnitDelay_DSTATE_on ) ; }
else { _rtB -> B_311_302_0 = _rtDW -> UnitDelay1_DSTATE_l ; } if ( _rtDW ->
systemEnable_a != 0 ) { _rtDW -> lastSin_f = muDoubleScalarSin ( _rtP ->
P_920 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_n = muDoubleScalarCos (
_rtP -> P_920 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_a = 0 ; }
_rtB -> B_311_304_0 = ( ( ( _rtDW -> lastSin_f * _rtP -> P_924 + _rtDW ->
lastCos_n * _rtP -> P_923 ) * _rtP -> P_922 + ( _rtDW -> lastCos_n * _rtP ->
P_924 - _rtDW -> lastSin_f * _rtP -> P_923 ) * _rtP -> P_921 ) * _rtP ->
P_918 + _rtP -> P_919 ) * _rtB -> B_311_261_0 [ 1 ] ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_j != 0 ) { _rtB -> B_311_305_0 = _rtDW ->
Integ4_DSTATE_f ; } else { _rtB -> B_311_305_0 = _rtP -> P_925 * _rtB ->
B_311_304_0 + _rtDW -> Integ4_DSTATE_f ; } _rtB -> B_311_306_0 = _rtP ->
P_927 ; ssCallAccelRunBlock ( S , 311 , 307 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_306_0 ) { _rtB -> B_311_314_0 = (
_rtB -> B_311_305_0 - _rtB -> B_311_307_0 ) * _rtP -> P_936 + ( _rtP -> P_249
* _rtB -> B_311_304_0 - _rtP -> P_248 * _rtDW -> UnitDelay_DSTATE_cq ) ; }
else { _rtB -> B_311_314_0 = _rtDW -> UnitDelay1_DSTATE_f ; } rtb_B_311_50_0
= _rtP -> P_939 * muDoubleScalarAtan2 ( _rtB -> B_311_314_0 , _rtB ->
B_311_302_0 ) * _rtP -> P_940 ; B_311_319_0 . re = muDoubleScalarHypot ( _rtB
-> B_311_302_0 , _rtB -> B_311_314_0 ) * muDoubleScalarCos ( rtb_B_311_50_0 )
; B_311_319_0 . im = muDoubleScalarHypot ( _rtB -> B_311_302_0 , _rtB ->
B_311_314_0 ) * muDoubleScalarSin ( rtb_B_311_50_0 ) ; if ( _rtDW ->
systemEnable_og != 0 ) { _rtDW -> lastSin_ak = muDoubleScalarSin ( _rtP ->
P_943 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_g = muDoubleScalarCos (
_rtP -> P_943 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_og = 0 ; }
_rtB -> B_311_321_0 = ( ( ( _rtDW -> lastSin_ak * _rtP -> P_947 + _rtDW ->
lastCos_g * _rtP -> P_946 ) * _rtP -> P_945 + ( _rtDW -> lastCos_g * _rtP ->
P_947 - _rtDW -> lastSin_ak * _rtP -> P_946 ) * _rtP -> P_944 ) * _rtP ->
P_941 + _rtP -> P_942 ) * _rtB -> B_311_261_0 [ 2 ] ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_p != 0 ) { _rtB -> B_311_322_0 = _rtDW ->
Integ4_DSTATE_k ; } else { _rtB -> B_311_322_0 = _rtP -> P_948 * _rtB ->
B_311_321_0 + _rtDW -> Integ4_DSTATE_k ; } _rtB -> B_311_323_0 = _rtP ->
P_950 ; ssCallAccelRunBlock ( S , 311 , 324 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_323_0 ) { _rtB -> B_311_331_0 = (
_rtB -> B_311_322_0 - _rtB -> B_311_324_0 ) * _rtP -> P_959 + ( _rtP -> P_255
* _rtB -> B_311_321_0 - _rtP -> P_254 * _rtDW -> UnitDelay_DSTATE_d ) ; }
else { _rtB -> B_311_331_0 = _rtDW -> UnitDelay1_DSTATE_k ; } if ( _rtDW ->
systemEnable_du != 0 ) { _rtDW -> lastSin_o = muDoubleScalarSin ( _rtP ->
P_964 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_ke = muDoubleScalarCos (
_rtP -> P_964 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_du = 0 ; }
_rtB -> B_311_333_0 = ( ( ( _rtDW -> lastSin_o * _rtP -> P_968 + _rtDW ->
lastCos_ke * _rtP -> P_967 ) * _rtP -> P_966 + ( _rtDW -> lastCos_ke * _rtP
-> P_968 - _rtDW -> lastSin_o * _rtP -> P_967 ) * _rtP -> P_965 ) * _rtP ->
P_962 + _rtP -> P_963 ) * _rtB -> B_311_261_0 [ 2 ] ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_e != 0 ) { _rtB -> B_311_334_0 = _rtDW ->
Integ4_DSTATE_jf ; } else { _rtB -> B_311_334_0 = _rtP -> P_969 * _rtB ->
B_311_333_0 + _rtDW -> Integ4_DSTATE_jf ; } _rtB -> B_311_335_0 = _rtP ->
P_971 ; ssCallAccelRunBlock ( S , 311 , 336 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_335_0 ) { _rtB -> B_311_343_0 = (
_rtB -> B_311_334_0 - _rtB -> B_311_336_0 ) * _rtP -> P_980 + ( _rtP -> P_253
* _rtB -> B_311_333_0 - _rtP -> P_252 * _rtDW -> UnitDelay_DSTATE_pw ) ; }
else { _rtB -> B_311_343_0 = _rtDW -> UnitDelay1_DSTATE_a ; } rtb_B_311_50_0
= _rtP -> P_983 * muDoubleScalarAtan2 ( _rtB -> B_311_343_0 , _rtB ->
B_311_331_0 ) * _rtP -> P_984 ; B_311_348_0 . re = muDoubleScalarHypot ( _rtB
-> B_311_331_0 , _rtB -> B_311_343_0 ) * muDoubleScalarCos ( rtb_B_311_50_0 )
; B_311_348_0 . im = muDoubleScalarHypot ( _rtB -> B_311_331_0 , _rtB ->
B_311_343_0 ) * muDoubleScalarSin ( rtb_B_311_50_0 ) ;
Auto_microgrid_forecasting_020822_NegSeqComputation ( S , _rtB ->
B_311_277_0_i , B_311_290_0 , B_311_319_0 , B_311_348_0 , & _rtB ->
PosSeqComputation_k , & _rtDW -> PosSeqComputation_k , & _rtP ->
PosSeqComputation_k ) ; Auto_microgrid_forecasting_020822_NegSeqComputation (
S , _rtB -> B_311_278_0_f , B_311_290_0 , B_311_319_0 , B_311_348_0 , & _rtB
-> NegSeqComputation_h , & _rtDW -> NegSeqComputation_h , & _rtP ->
NegSeqComputation_h ) ; Auto_microgrid_forecasting_020822_ZeroSeqComputation
( S , _rtB -> B_311_279_0 , B_311_290_0 , B_311_319_0 , B_311_348_0 , & _rtB
-> ZeroSeqComputation_j , & _rtDW -> ZeroSeqComputation_j , & _rtP ->
ZeroSeqComputation_j ) ; _rtB -> B_311_353_0 = _rtP -> P_985 *
muDoubleScalarHypot ( _rtB -> PosSeqComputation_k . B_122_2_0 . re , _rtB ->
PosSeqComputation_k . B_122_2_0 . im ) ; B_299_0_0_idx_0 = _rtP -> P_986 *
_rtB -> B_311_152_0 [ 54 ] * _rtP -> P_989 ; B_299_0_0_idx_1 = _rtP -> P_987
* _rtB -> B_311_152_0 [ 55 ] * _rtP -> P_989 ; B_299_0_0_idx_2 = _rtP ->
P_988 * _rtB -> B_311_152_0 [ 56 ] * _rtP -> P_989 ; _rtB -> B_311_360_0 = (
( _rtDW -> Integrator_DSTATE_o < _rtB -> B_311_240_0 ) || ( _rtDW ->
Integrator_DSTATE_o >= _rtB -> B_311_241_0 ) ) ; if ( _rtDW ->
Initial_FirstOutputTime_f ) { _rtDW -> Initial_FirstOutputTime_f = false ;
_rtB -> B_311_366_0 = _rtP -> P_992 ; } else { _rtB -> B_311_366_0 = _rtDW ->
Integrator_DSTATE_o - muDoubleScalarFloor ( ( _rtDW -> Integrator_DSTATE_o -
_rtB -> B_311_242_0 ) * _rtP -> P_990 ) * _rtP -> P_991 ; } if ( _rtB ->
B_311_360_0 && ( _rtDW -> Integrator_PrevResetState_f <= 0 ) ) { _rtDW ->
Integrator_DSTATE_o = _rtB -> B_311_366_0 ; } muDoubleScalarSinCos ( _rtDW ->
Integrator_DSTATE_o , & rtb_B_311_50_0 , & rtb_B_311_51_0 ) ; _rtB ->
B_311_369_0 [ 0 ] = B_299_0_0_idx_0 * rtb_B_311_50_0 ; _rtB -> B_311_369_0 [
1 ] = B_299_0_0_idx_0 * rtb_B_311_51_0 ; _rtB -> B_311_386_0 [ 0 ] =
B_299_0_0_idx_1 * rtb_B_311_50_0 ; _rtB -> B_311_386_0 [ 1 ] =
B_299_0_0_idx_1 * rtb_B_311_51_0 ; _rtB -> B_311_403_0 [ 0 ] =
B_299_0_0_idx_2 * rtb_B_311_50_0 ; _rtB -> B_311_403_0 [ 1 ] =
B_299_0_0_idx_2 * rtb_B_311_51_0 ; B_299_0_0_idx_0 = _rtP -> P_1021 * _rtB ->
B_311_152_0 [ 137 ] * _rtP -> P_1024 ; B_299_0_0_idx_1 = _rtP -> P_1022 *
_rtB -> B_311_152_0 [ 138 ] * _rtP -> P_1024 ; B_299_0_0_idx_2 = _rtP ->
P_1023 * _rtB -> B_311_152_0 [ 139 ] * _rtP -> P_1024 ; _rtB -> B_311_424_0 [
0 ] = B_299_0_0_idx_0 * rtb_B_311_50_0 ; _rtB -> B_311_424_0 [ 1 ] =
B_299_0_0_idx_0 * rtb_B_311_51_0 ; _rtB -> B_311_441_0 [ 0 ] =
B_299_0_0_idx_1 * rtb_B_311_50_0 ; _rtB -> B_311_441_0 [ 1 ] =
B_299_0_0_idx_1 * rtb_B_311_51_0 ; _rtB -> B_311_458_0 [ 0 ] =
B_299_0_0_idx_2 * rtb_B_311_50_0 ; _rtB -> B_311_458_0 [ 1 ] =
B_299_0_0_idx_2 * rtb_B_311_51_0 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE !=
0 ) { _rtB -> B_311_370_0 [ 0 ] = _rtDW -> Integrator_DSTATE_j [ 0 ] ; } else
{ _rtB -> B_311_370_0 [ 0 ] = _rtP -> P_994 * _rtB -> B_311_369_0 [ 0 ] +
_rtDW -> Integrator_DSTATE_j [ 0 ] ; } _rtB -> B_311_377_0 [ 0 ] = _rtDW ->
Delay_DSTATE [ 0 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_h != 0 ) { _rtB
-> B_311_387_0 [ 0 ] = _rtDW -> Integrator_DSTATE_f [ 0 ] ; } else { _rtB ->
B_311_387_0 [ 0 ] = _rtP -> P_1003 * _rtB -> B_311_386_0 [ 0 ] + _rtDW ->
Integrator_DSTATE_f [ 0 ] ; } _rtB -> B_311_394_0 [ 0 ] = _rtDW ->
Delay_DSTATE_o [ 0 ] ; rtb_B_311_68_0 = ( ( ( ( _rtB -> B_311_387_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_g [ 0 ] ) * _rtP -> P_1006 + _rtDW ->
UnitDelay1_DSTATE_g [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_m [ 0 ] ) + ( _rtB
-> B_311_387_0 [ 0 ] - ( ( _rtB -> B_311_394_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_m [ 0 ] ) * _rtP -> P_1009 + _rtDW -> UnitDelay2_DSTATE_m [
0 ] ) ) ) * _rtP -> P_1010 * _rtP -> P_1011 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_f != 0 ) { _rtB -> B_311_404_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_c [ 0 ] ; } else { _rtB -> B_311_404_0 [ 0 ] = _rtP ->
P_1012 * _rtB -> B_311_403_0 [ 0 ] + _rtDW -> Integrator_DSTATE_c [ 0 ] ; }
_rtB -> B_311_411_0 [ 0 ] = _rtDW -> Delay_DSTATE_p [ 0 ] ; rtb_B_311_51_0 =
( ( ( ( _rtB -> B_311_404_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_kl [ 0 ] ) *
_rtP -> P_1015 + _rtDW -> UnitDelay1_DSTATE_kl [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_i [ 0 ] ) + ( _rtB -> B_311_404_0 [ 0 ] - ( ( _rtB ->
B_311_411_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_i [ 0 ] ) * _rtP -> P_1018 +
_rtDW -> UnitDelay2_DSTATE_i [ 0 ] ) ) ) * _rtP -> P_1019 * _rtP -> P_1020 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_k != 0 ) { _rtB -> B_311_425_0 [ 0 ] =
_rtDW -> Integrator_DSTATE_d [ 0 ] ; } else { _rtB -> B_311_425_0 [ 0 ] =
_rtP -> P_1025 * _rtB -> B_311_424_0 [ 0 ] + _rtDW -> Integrator_DSTATE_d [ 0
] ; } _rtB -> B_311_432_0 [ 0 ] = _rtDW -> Delay_DSTATE_b [ 0 ] ; if ( _rtDW
-> Integrator_SYSTEM_ENABLE_j != 0 ) { _rtB -> B_311_442_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_jn [ 0 ] ; } else { _rtB -> B_311_442_0 [ 0 ] = _rtP ->
P_1034 * _rtB -> B_311_441_0 [ 0 ] + _rtDW -> Integrator_DSTATE_jn [ 0 ] ; }
_rtB -> B_311_449_0 [ 0 ] = _rtDW -> Delay_DSTATE_k [ 0 ] ; rtb_B_311_36_0 =
( ( ( ( _rtB -> B_311_442_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_l2 [ 0 ] ) *
_rtP -> P_1037 + _rtDW -> UnitDelay1_DSTATE_l2 [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_b [ 0 ] ) + ( _rtB -> B_311_442_0 [ 0 ] - ( ( _rtB ->
B_311_449_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_b [ 0 ] ) * _rtP -> P_1040 +
_rtDW -> UnitDelay2_DSTATE_b [ 0 ] ) ) ) * _rtP -> P_1041 * _rtP -> P_1042 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_l != 0 ) { _rtB -> B_311_459_0 [ 0 ] =
_rtDW -> Integrator_DSTATE_d5 [ 0 ] ; } else { _rtB -> B_311_459_0 [ 0 ] =
_rtP -> P_1043 * _rtB -> B_311_458_0 [ 0 ] + _rtDW -> Integrator_DSTATE_d5 [
0 ] ; } _rtB -> B_311_466_0 [ 0 ] = _rtDW -> Delay_DSTATE_m [ 0 ] ;
B_311_473_0_idx_0 = ( ( ( ( _rtB -> B_311_459_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_m [ 0 ] ) * _rtP -> P_1046 + _rtDW -> UnitDelay1_DSTATE_m [
0 ] ) - _rtDW -> UnitDelay2_DSTATE_k [ 0 ] ) + ( _rtB -> B_311_459_0 [ 0 ] -
( ( _rtB -> B_311_466_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_k [ 0 ] ) * _rtP
-> P_1049 + _rtDW -> UnitDelay2_DSTATE_k [ 0 ] ) ) ) * _rtP -> P_1050 * _rtP
-> P_1051 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE != 0 ) { _rtB ->
B_311_370_0 [ 1 ] = _rtDW -> Integrator_DSTATE_j [ 1 ] ; } else { _rtB ->
B_311_370_0 [ 1 ] = _rtP -> P_994 * _rtB -> B_311_369_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_j [ 1 ] ; } _rtB -> B_311_377_0 [ 1 ] = _rtDW ->
Delay_DSTATE [ 1 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_h != 0 ) { _rtB
-> B_311_387_0 [ 1 ] = _rtDW -> Integrator_DSTATE_f [ 1 ] ; } else { _rtB ->
B_311_387_0 [ 1 ] = _rtP -> P_1003 * _rtB -> B_311_386_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_f [ 1 ] ; } _rtB -> B_311_394_0 [ 1 ] = _rtDW ->
Delay_DSTATE_o [ 1 ] ; B_311_401_0_idx_1 = ( ( ( ( _rtB -> B_311_387_0 [ 1 ]
- _rtDW -> UnitDelay1_DSTATE_g [ 1 ] ) * _rtP -> P_1006 + _rtDW ->
UnitDelay1_DSTATE_g [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_m [ 1 ] ) + ( _rtB
-> B_311_387_0 [ 1 ] - ( ( _rtB -> B_311_394_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_m [ 1 ] ) * _rtP -> P_1009 + _rtDW -> UnitDelay2_DSTATE_m [
1 ] ) ) ) * _rtP -> P_1010 * _rtP -> P_1011 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_f != 0 ) { _rtB -> B_311_404_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_c [ 1 ] ; } else { _rtB -> B_311_404_0 [ 1 ] = _rtP ->
P_1012 * _rtB -> B_311_403_0 [ 1 ] + _rtDW -> Integrator_DSTATE_c [ 1 ] ; }
_rtB -> B_311_411_0 [ 1 ] = _rtDW -> Delay_DSTATE_p [ 1 ] ; rtb_B_311_52_0 =
( ( ( ( _rtB -> B_311_404_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_kl [ 1 ] ) *
_rtP -> P_1015 + _rtDW -> UnitDelay1_DSTATE_kl [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_i [ 1 ] ) + ( _rtB -> B_311_404_0 [ 1 ] - ( ( _rtB ->
B_311_411_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_i [ 1 ] ) * _rtP -> P_1018 +
_rtDW -> UnitDelay2_DSTATE_i [ 1 ] ) ) ) * _rtP -> P_1019 * _rtP -> P_1020 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_k != 0 ) { _rtB -> B_311_425_0 [ 1 ] =
_rtDW -> Integrator_DSTATE_d [ 1 ] ; } else { _rtB -> B_311_425_0 [ 1 ] =
_rtP -> P_1025 * _rtB -> B_311_424_0 [ 1 ] + _rtDW -> Integrator_DSTATE_d [ 1
] ; } _rtB -> B_311_432_0 [ 1 ] = _rtDW -> Delay_DSTATE_b [ 1 ] ; if ( _rtDW
-> Integrator_SYSTEM_ENABLE_j != 0 ) { _rtB -> B_311_442_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_jn [ 1 ] ; } else { _rtB -> B_311_442_0 [ 1 ] = _rtP ->
P_1034 * _rtB -> B_311_441_0 [ 1 ] + _rtDW -> Integrator_DSTATE_jn [ 1 ] ; }
_rtB -> B_311_449_0 [ 1 ] = _rtDW -> Delay_DSTATE_k [ 1 ] ; rtb_B_299_3_0 = (
( ( ( _rtB -> B_311_442_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_l2 [ 1 ] ) *
_rtP -> P_1037 + _rtDW -> UnitDelay1_DSTATE_l2 [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_b [ 1 ] ) + ( _rtB -> B_311_442_0 [ 1 ] - ( ( _rtB ->
B_311_449_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_b [ 1 ] ) * _rtP -> P_1040 +
_rtDW -> UnitDelay2_DSTATE_b [ 1 ] ) ) ) * _rtP -> P_1041 * _rtP -> P_1042 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_l != 0 ) { _rtB -> B_311_459_0 [ 1 ] =
_rtDW -> Integrator_DSTATE_d5 [ 1 ] ; } else { _rtB -> B_311_459_0 [ 1 ] =
_rtP -> P_1043 * _rtB -> B_311_458_0 [ 1 ] + _rtDW -> Integrator_DSTATE_d5 [
1 ] ; } _rtB -> B_311_466_0 [ 1 ] = _rtDW -> Delay_DSTATE_m [ 1 ] ;
rtb_B_311_50_0 = ( ( ( ( _rtB -> B_311_459_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_m [ 1 ] ) * _rtP -> P_1046 + _rtDW -> UnitDelay1_DSTATE_m [
1 ] ) - _rtDW -> UnitDelay2_DSTATE_k [ 1 ] ) + ( _rtB -> B_311_459_0 [ 1 ] -
( ( _rtB -> B_311_466_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_k [ 1 ] ) * _rtP
-> P_1049 + _rtDW -> UnitDelay2_DSTATE_k [ 1 ] ) ) ) * _rtP -> P_1050 * _rtP
-> P_1051 ; switch ( ( int32_T ) _rtB -> B_311_239_0_j ) { case 1 : _rtB ->
B_311_479_0 = ( ( ( ( ( ( ( _rtB -> B_311_370_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_bd [ 0 ] ) * _rtP -> P_997 + _rtDW -> UnitDelay1_DSTATE_bd
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE [ 0 ] ) + ( _rtB -> B_311_370_0 [ 0 ] -
( ( _rtB -> B_311_377_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE [ 0 ] ) * _rtP ->
P_1000 + _rtDW -> UnitDelay2_DSTATE [ 0 ] ) ) ) * _rtP -> P_1001 * _rtP ->
P_1002 + ( _rtP -> P_4200 . re * rtb_B_311_68_0 - _rtP -> P_4200 . im *
B_311_401_0_idx_1 ) ) + ( _rtP -> P_4199 . re * rtb_B_311_51_0 - _rtP ->
P_4199 . im * rtb_B_311_52_0 ) ) * _rtP -> P_152 * ( ( ( ( ( ( ( _rtB ->
B_311_425_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_n [ 0 ] ) * _rtP -> P_1028 +
_rtDW -> UnitDelay1_DSTATE_n [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_n [ 0 ] ) +
( _rtB -> B_311_425_0 [ 0 ] - ( ( _rtB -> B_311_432_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_n [ 0 ] ) * _rtP -> P_1031 + _rtDW -> UnitDelay2_DSTATE_n [
0 ] ) ) ) * _rtP -> P_1032 * _rtP -> P_1033 + ( _rtP -> P_4198 . re *
rtb_B_311_36_0 - _rtP -> P_4198 . im * rtb_B_299_3_0 ) ) + ( _rtP -> P_4197 .
re * B_311_473_0_idx_0 - _rtP -> P_4197 . im * rtb_B_311_50_0 ) ) * _rtP ->
P_151 ) - ( ( ( ( ( ( _rtB -> B_311_370_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_bd [ 1 ] ) * _rtP -> P_997 + _rtDW -> UnitDelay1_DSTATE_bd
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE [ 1 ] ) + ( _rtB -> B_311_370_0 [ 1 ] -
( ( _rtB -> B_311_377_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE [ 1 ] ) * _rtP ->
P_1000 + _rtDW -> UnitDelay2_DSTATE [ 1 ] ) ) ) * _rtP -> P_1001 * _rtP ->
P_1002 + ( _rtP -> P_4200 . re * B_311_401_0_idx_1 + _rtP -> P_4200 . im *
rtb_B_311_68_0 ) ) + ( _rtP -> P_4199 . re * rtb_B_311_52_0 + _rtP -> P_4199
. im * rtb_B_311_51_0 ) ) * _rtP -> P_152 * - ( ( ( ( ( ( ( _rtB ->
B_311_425_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_n [ 1 ] ) * _rtP -> P_1028 +
_rtDW -> UnitDelay1_DSTATE_n [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_n [ 1 ] ) +
( _rtB -> B_311_425_0 [ 1 ] - ( ( _rtB -> B_311_432_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_n [ 1 ] ) * _rtP -> P_1031 + _rtDW -> UnitDelay2_DSTATE_n [
1 ] ) ) ) * _rtP -> P_1032 * _rtP -> P_1033 + ( _rtP -> P_4198 . re *
rtb_B_299_3_0 + _rtP -> P_4198 . im * rtb_B_311_36_0 ) ) + ( _rtP -> P_4197 .
re * rtb_B_311_50_0 + _rtP -> P_4197 . im * B_311_473_0_idx_0 ) ) * _rtP ->
P_151 ) ) * _rtP -> P_153 ; break ; case 2 : _rtB -> B_311_479_0 = ( ( ( ( (
( ( _rtB -> B_311_370_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_bd [ 0 ] ) * _rtP
-> P_997 + _rtDW -> UnitDelay1_DSTATE_bd [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE
[ 0 ] ) + ( _rtB -> B_311_370_0 [ 0 ] - ( ( _rtB -> B_311_377_0 [ 0 ] - _rtDW
-> UnitDelay2_DSTATE [ 0 ] ) * _rtP -> P_1000 + _rtDW -> UnitDelay2_DSTATE [
0 ] ) ) ) * _rtP -> P_1001 * _rtP -> P_1002 + ( _rtP -> P_4196 . re *
rtb_B_311_68_0 - _rtP -> P_4196 . im * B_311_401_0_idx_1 ) ) + ( _rtP ->
P_4195 . re * rtb_B_311_51_0 - _rtP -> P_4195 . im * rtb_B_311_52_0 ) ) *
_rtP -> P_149 * ( ( ( ( ( ( ( _rtB -> B_311_425_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_n [ 0 ] ) * _rtP -> P_1028 + _rtDW -> UnitDelay1_DSTATE_n [
0 ] ) - _rtDW -> UnitDelay2_DSTATE_n [ 0 ] ) + ( _rtB -> B_311_425_0 [ 0 ] -
( ( _rtB -> B_311_432_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_n [ 0 ] ) * _rtP
-> P_1031 + _rtDW -> UnitDelay2_DSTATE_n [ 0 ] ) ) ) * _rtP -> P_1032 * _rtP
-> P_1033 + ( _rtP -> P_4194 . re * rtb_B_311_36_0 - _rtP -> P_4194 . im *
rtb_B_299_3_0 ) ) + ( _rtP -> P_4193 . re * B_311_473_0_idx_0 - _rtP ->
P_4193 . im * rtb_B_311_50_0 ) ) * _rtP -> P_148 ) - ( ( ( ( ( ( _rtB ->
B_311_370_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_bd [ 1 ] ) * _rtP -> P_997 +
_rtDW -> UnitDelay1_DSTATE_bd [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE [ 1 ] ) +
( _rtB -> B_311_370_0 [ 1 ] - ( ( _rtB -> B_311_377_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE [ 1 ] ) * _rtP -> P_1000 + _rtDW -> UnitDelay2_DSTATE [ 1 ]
) ) ) * _rtP -> P_1001 * _rtP -> P_1002 + ( _rtP -> P_4196 . re *
B_311_401_0_idx_1 + _rtP -> P_4196 . im * rtb_B_311_68_0 ) ) + ( _rtP ->
P_4195 . re * rtb_B_311_52_0 + _rtP -> P_4195 . im * rtb_B_311_51_0 ) ) *
_rtP -> P_149 * - ( ( ( ( ( ( ( _rtB -> B_311_425_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_n [ 1 ] ) * _rtP -> P_1028 + _rtDW -> UnitDelay1_DSTATE_n [
1 ] ) - _rtDW -> UnitDelay2_DSTATE_n [ 1 ] ) + ( _rtB -> B_311_425_0 [ 1 ] -
( ( _rtB -> B_311_432_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_n [ 1 ] ) * _rtP
-> P_1031 + _rtDW -> UnitDelay2_DSTATE_n [ 1 ] ) ) ) * _rtP -> P_1032 * _rtP
-> P_1033 + ( _rtP -> P_4194 . re * rtb_B_299_3_0 + _rtP -> P_4194 . im *
rtb_B_311_36_0 ) ) + ( _rtP -> P_4193 . re * rtb_B_311_50_0 + _rtP -> P_4193
. im * B_311_473_0_idx_0 ) ) * _rtP -> P_148 ) ) * _rtP -> P_150 ; break ;
default : _rtB -> B_311_479_0 = ( ( ( ( ( ( ( _rtB -> B_311_370_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_bd [ 0 ] ) * _rtP -> P_997 + _rtDW ->
UnitDelay1_DSTATE_bd [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE [ 0 ] ) + ( _rtB ->
B_311_370_0 [ 0 ] - ( ( _rtB -> B_311_377_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE [ 0 ] ) * _rtP -> P_1000 + _rtDW -> UnitDelay2_DSTATE [ 0 ]
) ) ) * _rtP -> P_1001 * _rtP -> P_1002 + rtb_B_311_68_0 ) + rtb_B_311_51_0 )
* _rtP -> P_146 * ( ( ( ( ( ( ( _rtB -> B_311_425_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_n [ 0 ] ) * _rtP -> P_1028 + _rtDW -> UnitDelay1_DSTATE_n [
0 ] ) - _rtDW -> UnitDelay2_DSTATE_n [ 0 ] ) + ( _rtB -> B_311_425_0 [ 0 ] -
( ( _rtB -> B_311_432_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_n [ 0 ] ) * _rtP
-> P_1031 + _rtDW -> UnitDelay2_DSTATE_n [ 0 ] ) ) ) * _rtP -> P_1032 * _rtP
-> P_1033 + rtb_B_311_36_0 ) + B_311_473_0_idx_0 ) * _rtP -> P_145 ) - ( ( (
( ( ( _rtB -> B_311_370_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_bd [ 1 ] ) *
_rtP -> P_997 + _rtDW -> UnitDelay1_DSTATE_bd [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE [ 1 ] ) + ( _rtB -> B_311_370_0 [ 1 ] - ( ( _rtB ->
B_311_377_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE [ 1 ] ) * _rtP -> P_1000 +
_rtDW -> UnitDelay2_DSTATE [ 1 ] ) ) ) * _rtP -> P_1001 * _rtP -> P_1002 +
B_311_401_0_idx_1 ) + rtb_B_311_52_0 ) * _rtP -> P_146 * - ( ( ( ( ( ( ( _rtB
-> B_311_425_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_n [ 1 ] ) * _rtP -> P_1028
+ _rtDW -> UnitDelay1_DSTATE_n [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_n [ 1 ] )
+ ( _rtB -> B_311_425_0 [ 1 ] - ( ( _rtB -> B_311_432_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_n [ 1 ] ) * _rtP -> P_1031 + _rtDW -> UnitDelay2_DSTATE_n [
1 ] ) ) ) * _rtP -> P_1032 * _rtP -> P_1033 + rtb_B_299_3_0 ) +
rtb_B_311_50_0 ) * _rtP -> P_145 ) ) * _rtP -> P_147 ; break ; } _rtB ->
B_311_483_0 [ 0 ] = _rtP -> P_1052 * _rtB -> B_311_152_0 [ 51 ] * _rtP ->
P_1055 ; _rtB -> B_311_483_0 [ 1 ] = _rtP -> P_1053 * _rtB -> B_311_152_0 [
52 ] * _rtP -> P_1055 ; _rtB -> B_311_483_0 [ 2 ] = _rtP -> P_1054 * _rtB ->
B_311_152_0 [ 53 ] * _rtP -> P_1055 ; _rtB -> B_311_486_0_h = ( ( _rtDW ->
Integrator_DSTATE_a < _rtB -> B_311_270_0 ) || ( _rtDW -> Integrator_DSTATE_a
>= _rtB -> B_311_271_0 ) ) ; if ( _rtDW -> Initial_FirstOutputTime_h ) {
_rtDW -> Initial_FirstOutputTime_h = false ; _rtB -> B_311_492_0 = _rtP ->
P_1058 ; } else { _rtB -> B_311_492_0 = _rtDW -> Integrator_DSTATE_a -
muDoubleScalarFloor ( ( _rtDW -> Integrator_DSTATE_a - _rtB -> B_311_272_0 )
* _rtP -> P_1056 ) * _rtP -> P_1057 ; } if ( _rtB -> B_311_486_0_h && ( _rtDW
-> Integrator_PrevResetState_e <= 0 ) ) { _rtDW -> Integrator_DSTATE_a = _rtB
-> B_311_492_0 ; } muDoubleScalarSinCos ( _rtDW -> Integrator_DSTATE_a , &
rtb_B_311_50_0 , & rtb_B_311_51_0 ) ; _rtB -> B_311_495_0 [ 0 ] = _rtB ->
B_311_483_0 [ 0 ] * rtb_B_311_50_0 ; _rtB -> B_311_495_0 [ 1 ] = _rtB ->
B_311_483_0 [ 0 ] * rtb_B_311_51_0 ; _rtB -> B_311_512_0 [ 0 ] = _rtB ->
B_311_483_0 [ 1 ] * rtb_B_311_50_0 ; _rtB -> B_311_512_0 [ 1 ] = _rtB ->
B_311_483_0 [ 1 ] * rtb_B_311_51_0 ; _rtB -> B_311_529_0 [ 0 ] = _rtB ->
B_311_483_0 [ 2 ] * rtb_B_311_50_0 ; _rtB -> B_311_529_0 [ 1 ] = _rtB ->
B_311_483_0 [ 2 ] * rtb_B_311_51_0 ; B_299_0_0_idx_0 = _rtP -> P_1087 * _rtB
-> B_311_152_0 [ 134 ] * _rtP -> P_1090 ; B_299_0_0_idx_1 = _rtP -> P_1088 *
_rtB -> B_311_152_0 [ 135 ] * _rtP -> P_1090 ; B_299_0_0_idx_2 = _rtP ->
P_1089 * _rtB -> B_311_152_0 [ 136 ] * _rtP -> P_1090 ; _rtB -> B_311_550_0 [
0 ] = B_299_0_0_idx_0 * rtb_B_311_50_0 ; _rtB -> B_311_550_0 [ 1 ] =
B_299_0_0_idx_0 * rtb_B_311_51_0 ; _rtB -> B_311_567_0 [ 0 ] =
B_299_0_0_idx_1 * rtb_B_311_50_0 ; _rtB -> B_311_567_0 [ 1 ] =
B_299_0_0_idx_1 * rtb_B_311_51_0 ; _rtB -> B_311_584_0 [ 0 ] =
B_299_0_0_idx_2 * rtb_B_311_50_0 ; _rtB -> B_311_584_0 [ 1 ] =
B_299_0_0_idx_2 * rtb_B_311_51_0 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_fz
!= 0 ) { _rtB -> B_311_496_0 [ 0 ] = _rtDW -> Integrator_DSTATE_az [ 0 ] ; }
else { _rtB -> B_311_496_0 [ 0 ] = _rtP -> P_1060 * _rtB -> B_311_495_0 [ 0 ]
+ _rtDW -> Integrator_DSTATE_az [ 0 ] ; } _rtB -> B_311_503_0 [ 0 ] = _rtDW
-> Delay_DSTATE_j [ 0 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_p != 0 ) {
_rtB -> B_311_513_0 [ 0 ] = _rtDW -> Integrator_DSTATE_b [ 0 ] ; } else {
_rtB -> B_311_513_0 [ 0 ] = _rtP -> P_1069 * _rtB -> B_311_512_0 [ 0 ] +
_rtDW -> Integrator_DSTATE_b [ 0 ] ; } _rtB -> B_311_520_0 [ 0 ] = _rtDW ->
Delay_DSTATE_d [ 0 ] ; rtb_B_311_68_0 = ( ( ( ( _rtB -> B_311_513_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_ht [ 0 ] ) * _rtP -> P_1072 + _rtDW ->
UnitDelay1_DSTATE_ht [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_il [ 0 ] ) + ( _rtB
-> B_311_513_0 [ 0 ] - ( ( _rtB -> B_311_520_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_il [ 0 ] ) * _rtP -> P_1075 + _rtDW -> UnitDelay2_DSTATE_il
[ 0 ] ) ) ) * _rtP -> P_1076 * _rtP -> P_1077 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_n != 0 ) { _rtB -> B_311_530_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_ar [ 0 ] ; } else { _rtB -> B_311_530_0 [ 0 ] = _rtP ->
P_1078 * _rtB -> B_311_529_0 [ 0 ] + _rtDW -> Integrator_DSTATE_ar [ 0 ] ; }
_rtB -> B_311_537_0 [ 0 ] = _rtDW -> Delay_DSTATE_c [ 0 ] ; rtb_B_311_51_0 =
( ( ( ( _rtB -> B_311_530_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_ds [ 0 ] ) *
_rtP -> P_1081 + _rtDW -> UnitDelay1_DSTATE_ds [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_ft [ 0 ] ) + ( _rtB -> B_311_530_0 [ 0 ] - ( ( _rtB ->
B_311_537_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_ft [ 0 ] ) * _rtP -> P_1084 +
_rtDW -> UnitDelay2_DSTATE_ft [ 0 ] ) ) ) * _rtP -> P_1085 * _rtP -> P_1086 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_lb != 0 ) { _rtB -> B_311_551_0 [ 0 ]
= _rtDW -> Integrator_DSTATE_n [ 0 ] ; } else { _rtB -> B_311_551_0 [ 0 ] =
_rtP -> P_1091 * _rtB -> B_311_550_0 [ 0 ] + _rtDW -> Integrator_DSTATE_n [ 0
] ; } _rtB -> B_311_558_0 [ 0 ] = _rtDW -> Delay_DSTATE_oo [ 0 ] ; if ( _rtDW
-> Integrator_SYSTEM_ENABLE_m != 0 ) { _rtB -> B_311_568_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_e [ 0 ] ; } else { _rtB -> B_311_568_0 [ 0 ] = _rtP ->
P_1100 * _rtB -> B_311_567_0 [ 0 ] + _rtDW -> Integrator_DSTATE_e [ 0 ] ; }
_rtB -> B_311_575_0 [ 0 ] = _rtDW -> Delay_DSTATE_f [ 0 ] ; rtb_B_311_36_0 =
( ( ( ( _rtB -> B_311_568_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_l1 [ 0 ] ) *
_rtP -> P_1103 + _rtDW -> UnitDelay1_DSTATE_l1 [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_nr [ 0 ] ) + ( _rtB -> B_311_568_0 [ 0 ] - ( ( _rtB ->
B_311_575_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_nr [ 0 ] ) * _rtP -> P_1106 +
_rtDW -> UnitDelay2_DSTATE_nr [ 0 ] ) ) ) * _rtP -> P_1107 * _rtP -> P_1108 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_fl != 0 ) { _rtB -> B_311_585_0 [ 0 ]
= _rtDW -> Integrator_DSTATE_cb [ 0 ] ; } else { _rtB -> B_311_585_0 [ 0 ] =
_rtP -> P_1109 * _rtB -> B_311_584_0 [ 0 ] + _rtDW -> Integrator_DSTATE_cb [
0 ] ; } _rtB -> B_311_592_0 [ 0 ] = _rtDW -> Delay_DSTATE_a [ 0 ] ;
B_311_473_0_idx_0 = ( ( ( ( _rtB -> B_311_585_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_hq [ 0 ] ) * _rtP -> P_1112 + _rtDW -> UnitDelay1_DSTATE_hq
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_h [ 0 ] ) + ( _rtB -> B_311_585_0 [ 0 ]
- ( ( _rtB -> B_311_592_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_h [ 0 ] ) * _rtP
-> P_1115 + _rtDW -> UnitDelay2_DSTATE_h [ 0 ] ) ) ) * _rtP -> P_1116 * _rtP
-> P_1117 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_fz != 0 ) { _rtB ->
B_311_496_0 [ 1 ] = _rtDW -> Integrator_DSTATE_az [ 1 ] ; } else { _rtB ->
B_311_496_0 [ 1 ] = _rtP -> P_1060 * _rtB -> B_311_495_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_az [ 1 ] ; } _rtB -> B_311_503_0 [ 1 ] = _rtDW ->
Delay_DSTATE_j [ 1 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_p != 0 ) { _rtB
-> B_311_513_0 [ 1 ] = _rtDW -> Integrator_DSTATE_b [ 1 ] ; } else { _rtB ->
B_311_513_0 [ 1 ] = _rtP -> P_1069 * _rtB -> B_311_512_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_b [ 1 ] ; } _rtB -> B_311_520_0 [ 1 ] = _rtDW ->
Delay_DSTATE_d [ 1 ] ; B_311_401_0_idx_1 = ( ( ( ( _rtB -> B_311_513_0 [ 1 ]
- _rtDW -> UnitDelay1_DSTATE_ht [ 1 ] ) * _rtP -> P_1072 + _rtDW ->
UnitDelay1_DSTATE_ht [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_il [ 1 ] ) + ( _rtB
-> B_311_513_0 [ 1 ] - ( ( _rtB -> B_311_520_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_il [ 1 ] ) * _rtP -> P_1075 + _rtDW -> UnitDelay2_DSTATE_il
[ 1 ] ) ) ) * _rtP -> P_1076 * _rtP -> P_1077 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_n != 0 ) { _rtB -> B_311_530_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_ar [ 1 ] ; } else { _rtB -> B_311_530_0 [ 1 ] = _rtP ->
P_1078 * _rtB -> B_311_529_0 [ 1 ] + _rtDW -> Integrator_DSTATE_ar [ 1 ] ; }
_rtB -> B_311_537_0 [ 1 ] = _rtDW -> Delay_DSTATE_c [ 1 ] ; rtb_B_311_52_0 =
( ( ( ( _rtB -> B_311_530_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_ds [ 1 ] ) *
_rtP -> P_1081 + _rtDW -> UnitDelay1_DSTATE_ds [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_ft [ 1 ] ) + ( _rtB -> B_311_530_0 [ 1 ] - ( ( _rtB ->
B_311_537_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_ft [ 1 ] ) * _rtP -> P_1084 +
_rtDW -> UnitDelay2_DSTATE_ft [ 1 ] ) ) ) * _rtP -> P_1085 * _rtP -> P_1086 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_lb != 0 ) { _rtB -> B_311_551_0 [ 1 ]
= _rtDW -> Integrator_DSTATE_n [ 1 ] ; } else { _rtB -> B_311_551_0 [ 1 ] =
_rtP -> P_1091 * _rtB -> B_311_550_0 [ 1 ] + _rtDW -> Integrator_DSTATE_n [ 1
] ; } _rtB -> B_311_558_0 [ 1 ] = _rtDW -> Delay_DSTATE_oo [ 1 ] ; if ( _rtDW
-> Integrator_SYSTEM_ENABLE_m != 0 ) { _rtB -> B_311_568_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_e [ 1 ] ; } else { _rtB -> B_311_568_0 [ 1 ] = _rtP ->
P_1100 * _rtB -> B_311_567_0 [ 1 ] + _rtDW -> Integrator_DSTATE_e [ 1 ] ; }
_rtB -> B_311_575_0 [ 1 ] = _rtDW -> Delay_DSTATE_f [ 1 ] ; rtb_B_299_3_0 = (
( ( ( _rtB -> B_311_568_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_l1 [ 1 ] ) *
_rtP -> P_1103 + _rtDW -> UnitDelay1_DSTATE_l1 [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_nr [ 1 ] ) + ( _rtB -> B_311_568_0 [ 1 ] - ( ( _rtB ->
B_311_575_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_nr [ 1 ] ) * _rtP -> P_1106 +
_rtDW -> UnitDelay2_DSTATE_nr [ 1 ] ) ) ) * _rtP -> P_1107 * _rtP -> P_1108 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_fl != 0 ) { _rtB -> B_311_585_0 [ 1 ]
= _rtDW -> Integrator_DSTATE_cb [ 1 ] ; } else { _rtB -> B_311_585_0 [ 1 ] =
_rtP -> P_1109 * _rtB -> B_311_584_0 [ 1 ] + _rtDW -> Integrator_DSTATE_cb [
1 ] ; } _rtB -> B_311_592_0 [ 1 ] = _rtDW -> Delay_DSTATE_a [ 1 ] ;
rtb_B_311_50_0 = ( ( ( ( _rtB -> B_311_585_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_hq [ 1 ] ) * _rtP -> P_1112 + _rtDW -> UnitDelay1_DSTATE_hq
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_h [ 1 ] ) + ( _rtB -> B_311_585_0 [ 1 ]
- ( ( _rtB -> B_311_592_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_h [ 1 ] ) * _rtP
-> P_1115 + _rtDW -> UnitDelay2_DSTATE_h [ 1 ] ) ) ) * _rtP -> P_1116 * _rtP
-> P_1117 ; switch ( ( int32_T ) _rtB -> B_311_269_0 ) { case 1 : _rtB ->
B_311_605_0 = ( ( ( ( ( ( ( _rtB -> B_311_496_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_fn [ 0 ] ) * _rtP -> P_1063 + _rtDW -> UnitDelay1_DSTATE_fn
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_f [ 0 ] ) + ( _rtB -> B_311_496_0 [ 0 ]
- ( ( _rtB -> B_311_503_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_f [ 0 ] ) * _rtP
-> P_1066 + _rtDW -> UnitDelay2_DSTATE_f [ 0 ] ) ) ) * _rtP -> P_1067 * _rtP
-> P_1068 + ( _rtP -> P_4248 . re * rtb_B_311_68_0 - _rtP -> P_4248 . im *
B_311_401_0_idx_1 ) ) + ( _rtP -> P_4247 . re * rtb_B_311_51_0 - _rtP ->
P_4247 . im * rtb_B_311_52_0 ) ) * _rtP -> P_206 * ( ( ( ( ( ( ( _rtB ->
B_311_551_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_jl [ 0 ] ) * _rtP -> P_1094 +
_rtDW -> UnitDelay1_DSTATE_jl [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_nq [ 0 ] )
+ ( _rtB -> B_311_551_0 [ 0 ] - ( ( _rtB -> B_311_558_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_nq [ 0 ] ) * _rtP -> P_1097 + _rtDW -> UnitDelay2_DSTATE_nq
[ 0 ] ) ) ) * _rtP -> P_1098 * _rtP -> P_1099 + ( _rtP -> P_4246 . re *
rtb_B_311_36_0 - _rtP -> P_4246 . im * rtb_B_299_3_0 ) ) + ( _rtP -> P_4245 .
re * B_311_473_0_idx_0 - _rtP -> P_4245 . im * rtb_B_311_50_0 ) ) * _rtP ->
P_205 ) - ( ( ( ( ( ( _rtB -> B_311_496_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_fn [ 1 ] ) * _rtP -> P_1063 + _rtDW -> UnitDelay1_DSTATE_fn
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_f [ 1 ] ) + ( _rtB -> B_311_496_0 [ 1 ]
- ( ( _rtB -> B_311_503_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_f [ 1 ] ) * _rtP
-> P_1066 + _rtDW -> UnitDelay2_DSTATE_f [ 1 ] ) ) ) * _rtP -> P_1067 * _rtP
-> P_1068 + ( _rtP -> P_4248 . re * B_311_401_0_idx_1 + _rtP -> P_4248 . im *
rtb_B_311_68_0 ) ) + ( _rtP -> P_4247 . re * rtb_B_311_52_0 + _rtP -> P_4247
. im * rtb_B_311_51_0 ) ) * _rtP -> P_206 * - ( ( ( ( ( ( ( _rtB ->
B_311_551_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_jl [ 1 ] ) * _rtP -> P_1094 +
_rtDW -> UnitDelay1_DSTATE_jl [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_nq [ 1 ] )
+ ( _rtB -> B_311_551_0 [ 1 ] - ( ( _rtB -> B_311_558_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_nq [ 1 ] ) * _rtP -> P_1097 + _rtDW -> UnitDelay2_DSTATE_nq
[ 1 ] ) ) ) * _rtP -> P_1098 * _rtP -> P_1099 + ( _rtP -> P_4246 . re *
rtb_B_299_3_0 + _rtP -> P_4246 . im * rtb_B_311_36_0 ) ) + ( _rtP -> P_4245 .
re * rtb_B_311_50_0 + _rtP -> P_4245 . im * B_311_473_0_idx_0 ) ) * _rtP ->
P_205 ) ) * _rtP -> P_207 ; break ; case 2 : _rtB -> B_311_605_0 = ( ( ( ( (
( ( _rtB -> B_311_496_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_fn [ 0 ] ) * _rtP
-> P_1063 + _rtDW -> UnitDelay1_DSTATE_fn [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_f [ 0 ] ) + ( _rtB -> B_311_496_0 [ 0 ] - ( ( _rtB ->
B_311_503_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_f [ 0 ] ) * _rtP -> P_1066 +
_rtDW -> UnitDelay2_DSTATE_f [ 0 ] ) ) ) * _rtP -> P_1067 * _rtP -> P_1068 +
( _rtP -> P_4244 . re * rtb_B_311_68_0 - _rtP -> P_4244 . im *
B_311_401_0_idx_1 ) ) + ( _rtP -> P_4243 . re * rtb_B_311_51_0 - _rtP ->
P_4243 . im * rtb_B_311_52_0 ) ) * _rtP -> P_203 * ( ( ( ( ( ( ( _rtB ->
B_311_551_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_jl [ 0 ] ) * _rtP -> P_1094 +
_rtDW -> UnitDelay1_DSTATE_jl [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_nq [ 0 ] )
+ ( _rtB -> B_311_551_0 [ 0 ] - ( ( _rtB -> B_311_558_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_nq [ 0 ] ) * _rtP -> P_1097 + _rtDW -> UnitDelay2_DSTATE_nq
[ 0 ] ) ) ) * _rtP -> P_1098 * _rtP -> P_1099 + ( _rtP -> P_4242 . re *
rtb_B_311_36_0 - _rtP -> P_4242 . im * rtb_B_299_3_0 ) ) + ( _rtP -> P_4241 .
re * B_311_473_0_idx_0 - _rtP -> P_4241 . im * rtb_B_311_50_0 ) ) * _rtP ->
P_202 ) - ( ( ( ( ( ( _rtB -> B_311_496_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_fn [ 1 ] ) * _rtP -> P_1063 + _rtDW -> UnitDelay1_DSTATE_fn
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_f [ 1 ] ) + ( _rtB -> B_311_496_0 [ 1 ]
- ( ( _rtB -> B_311_503_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_f [ 1 ] ) * _rtP
-> P_1066 + _rtDW -> UnitDelay2_DSTATE_f [ 1 ] ) ) ) * _rtP -> P_1067 * _rtP
-> P_1068 + ( _rtP -> P_4244 . re * B_311_401_0_idx_1 + _rtP -> P_4244 . im *
rtb_B_311_68_0 ) ) + ( _rtP -> P_4243 . re * rtb_B_311_52_0 + _rtP -> P_4243
. im * rtb_B_311_51_0 ) ) * _rtP -> P_203 * - ( ( ( ( ( ( ( _rtB ->
B_311_551_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_jl [ 1 ] ) * _rtP -> P_1094 +
_rtDW -> UnitDelay1_DSTATE_jl [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_nq [ 1 ] )
+ ( _rtB -> B_311_551_0 [ 1 ] - ( ( _rtB -> B_311_558_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_nq [ 1 ] ) * _rtP -> P_1097 + _rtDW -> UnitDelay2_DSTATE_nq
[ 1 ] ) ) ) * _rtP -> P_1098 * _rtP -> P_1099 + ( _rtP -> P_4242 . re *
rtb_B_299_3_0 + _rtP -> P_4242 . im * rtb_B_311_36_0 ) ) + ( _rtP -> P_4241 .
re * rtb_B_311_50_0 + _rtP -> P_4241 . im * B_311_473_0_idx_0 ) ) * _rtP ->
P_202 ) ) * _rtP -> P_204 ; break ; default : _rtB -> B_311_605_0 = ( ( ( ( (
( ( _rtB -> B_311_496_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_fn [ 0 ] ) * _rtP
-> P_1063 + _rtDW -> UnitDelay1_DSTATE_fn [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_f [ 0 ] ) + ( _rtB -> B_311_496_0 [ 0 ] - ( ( _rtB ->
B_311_503_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_f [ 0 ] ) * _rtP -> P_1066 +
_rtDW -> UnitDelay2_DSTATE_f [ 0 ] ) ) ) * _rtP -> P_1067 * _rtP -> P_1068 +
rtb_B_311_68_0 ) + rtb_B_311_51_0 ) * _rtP -> P_200 * ( ( ( ( ( ( ( _rtB ->
B_311_551_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_jl [ 0 ] ) * _rtP -> P_1094 +
_rtDW -> UnitDelay1_DSTATE_jl [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_nq [ 0 ] )
+ ( _rtB -> B_311_551_0 [ 0 ] - ( ( _rtB -> B_311_558_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_nq [ 0 ] ) * _rtP -> P_1097 + _rtDW -> UnitDelay2_DSTATE_nq
[ 0 ] ) ) ) * _rtP -> P_1098 * _rtP -> P_1099 + rtb_B_311_36_0 ) +
B_311_473_0_idx_0 ) * _rtP -> P_199 ) - ( ( ( ( ( ( _rtB -> B_311_496_0 [ 1 ]
- _rtDW -> UnitDelay1_DSTATE_fn [ 1 ] ) * _rtP -> P_1063 + _rtDW ->
UnitDelay1_DSTATE_fn [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_f [ 1 ] ) + ( _rtB
-> B_311_496_0 [ 1 ] - ( ( _rtB -> B_311_503_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_f [ 1 ] ) * _rtP -> P_1066 + _rtDW -> UnitDelay2_DSTATE_f [
1 ] ) ) ) * _rtP -> P_1067 * _rtP -> P_1068 + B_311_401_0_idx_1 ) +
rtb_B_311_52_0 ) * _rtP -> P_200 * - ( ( ( ( ( ( ( _rtB -> B_311_551_0 [ 1 ]
- _rtDW -> UnitDelay1_DSTATE_jl [ 1 ] ) * _rtP -> P_1094 + _rtDW ->
UnitDelay1_DSTATE_jl [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_nq [ 1 ] ) + ( _rtB
-> B_311_551_0 [ 1 ] - ( ( _rtB -> B_311_558_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_nq [ 1 ] ) * _rtP -> P_1097 + _rtDW -> UnitDelay2_DSTATE_nq
[ 1 ] ) ) ) * _rtP -> P_1098 * _rtP -> P_1099 + rtb_B_299_3_0 ) +
rtb_B_311_50_0 ) * _rtP -> P_199 ) ) * _rtP -> P_201 ; break ; } _rtB ->
B_311_609_0 [ 0 ] = _rtP -> P_1118 * _rtB -> B_311_152_0 [ 39 ] * _rtP ->
P_1121 ; _rtB -> B_311_609_0 [ 1 ] = _rtP -> P_1119 * _rtB -> B_311_152_0 [
40 ] * _rtP -> P_1121 ; _rtB -> B_311_609_0 [ 2 ] = _rtP -> P_1120 * _rtB ->
B_311_152_0 [ 41 ] * _rtP -> P_1121 ; _rtB -> B_311_612_0 = ( ( _rtDW ->
Integrator_DSTATE_au < _rtB -> B_311_245_0 ) || ( _rtDW ->
Integrator_DSTATE_au >= _rtB -> B_311_246_0 ) ) ; if ( _rtDW ->
Initial_FirstOutputTime_n ) { _rtDW -> Initial_FirstOutputTime_n = false ;
_rtB -> B_311_618_0 = _rtP -> P_1124 ; } else { _rtB -> B_311_618_0 = _rtDW
-> Integrator_DSTATE_au - muDoubleScalarFloor ( ( _rtDW ->
Integrator_DSTATE_au - _rtB -> B_311_247_0 ) * _rtP -> P_1122 ) * _rtP ->
P_1123 ; } if ( _rtB -> B_311_612_0 && ( _rtDW -> Integrator_PrevResetState_l
<= 0 ) ) { _rtDW -> Integrator_DSTATE_au = _rtB -> B_311_618_0 ; }
muDoubleScalarSinCos ( _rtDW -> Integrator_DSTATE_au , & rtb_B_311_50_0 , &
rtb_B_311_51_0 ) ; _rtB -> B_311_621_0 [ 0 ] = _rtB -> B_311_609_0 [ 0 ] *
rtb_B_311_50_0 ; _rtB -> B_311_621_0 [ 1 ] = _rtB -> B_311_609_0 [ 0 ] *
rtb_B_311_51_0 ; _rtB -> B_311_638_0 [ 0 ] = _rtB -> B_311_609_0 [ 1 ] *
rtb_B_311_50_0 ; _rtB -> B_311_638_0 [ 1 ] = _rtB -> B_311_609_0 [ 1 ] *
rtb_B_311_51_0 ; _rtB -> B_311_655_0 [ 0 ] = _rtB -> B_311_609_0 [ 2 ] *
rtb_B_311_50_0 ; _rtB -> B_311_655_0 [ 1 ] = _rtB -> B_311_609_0 [ 2 ] *
rtb_B_311_51_0 ; B_299_0_0_idx_0 = _rtP -> P_1153 * _rtB -> B_311_152_0 [ 122
] * _rtP -> P_1156 ; B_299_0_0_idx_1 = _rtP -> P_1154 * _rtB -> B_311_152_0 [
123 ] * _rtP -> P_1156 ; B_299_0_0_idx_2 = _rtP -> P_1155 * _rtB ->
B_311_152_0 [ 124 ] * _rtP -> P_1156 ; _rtB -> B_311_676_0 [ 0 ] =
B_299_0_0_idx_0 * rtb_B_311_50_0 ; _rtB -> B_311_676_0 [ 1 ] =
B_299_0_0_idx_0 * rtb_B_311_51_0 ; _rtB -> B_311_693_0 [ 0 ] =
B_299_0_0_idx_1 * rtb_B_311_50_0 ; _rtB -> B_311_693_0 [ 1 ] =
B_299_0_0_idx_1 * rtb_B_311_51_0 ; _rtB -> B_311_710_0 [ 0 ] =
B_299_0_0_idx_2 * rtb_B_311_50_0 ; _rtB -> B_311_710_0 [ 1 ] =
B_299_0_0_idx_2 * rtb_B_311_51_0 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_li
!= 0 ) { _rtB -> B_311_622_0 [ 0 ] = _rtDW -> Integrator_DSTATE_p [ 0 ] ; }
else { _rtB -> B_311_622_0 [ 0 ] = _rtP -> P_1126 * _rtB -> B_311_621_0 [ 0 ]
+ _rtDW -> Integrator_DSTATE_p [ 0 ] ; } _rtB -> B_311_629_0 [ 0 ] = _rtDW ->
Delay_DSTATE_l [ 0 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_d != 0 ) { _rtB
-> B_311_639_0 [ 0 ] = _rtDW -> Integrator_DSTATE_ec [ 0 ] ; } else { _rtB ->
B_311_639_0 [ 0 ] = _rtP -> P_1135 * _rtB -> B_311_638_0 [ 0 ] + _rtDW ->
Integrator_DSTATE_ec [ 0 ] ; } _rtB -> B_311_646_0 [ 0 ] = _rtDW ->
Delay_DSTATE_bg [ 0 ] ; rtb_B_311_68_0 = ( ( ( ( _rtB -> B_311_639_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_i3 [ 0 ] ) * _rtP -> P_1138 + _rtDW ->
UnitDelay1_DSTATE_i3 [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_ni [ 0 ] ) + ( _rtB
-> B_311_639_0 [ 0 ] - ( ( _rtB -> B_311_646_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_ni [ 0 ] ) * _rtP -> P_1141 + _rtDW -> UnitDelay2_DSTATE_ni
[ 0 ] ) ) ) * _rtP -> P_1142 * _rtP -> P_1143 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_b != 0 ) { _rtB -> B_311_656_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_h [ 0 ] ; } else { _rtB -> B_311_656_0 [ 0 ] = _rtP ->
P_1144 * _rtB -> B_311_655_0 [ 0 ] + _rtDW -> Integrator_DSTATE_h [ 0 ] ; }
_rtB -> B_311_663_0 [ 0 ] = _rtDW -> Delay_DSTATE_ky [ 0 ] ; rtb_B_311_51_0 =
( ( ( ( _rtB -> B_311_656_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_dq [ 0 ] ) *
_rtP -> P_1147 + _rtDW -> UnitDelay1_DSTATE_dq [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_l [ 0 ] ) + ( _rtB -> B_311_656_0 [ 0 ] - ( ( _rtB ->
B_311_663_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_l [ 0 ] ) * _rtP -> P_1150 +
_rtDW -> UnitDelay2_DSTATE_l [ 0 ] ) ) ) * _rtP -> P_1151 * _rtP -> P_1152 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_o != 0 ) { _rtB -> B_311_677_0 [ 0 ] =
_rtDW -> Integrator_DSTATE_ff [ 0 ] ; } else { _rtB -> B_311_677_0 [ 0 ] =
_rtP -> P_1157 * _rtB -> B_311_676_0 [ 0 ] + _rtDW -> Integrator_DSTATE_ff [
0 ] ; } _rtB -> B_311_684_0 [ 0 ] = _rtDW -> Delay_DSTATE_oe [ 0 ] ; if (
_rtDW -> Integrator_SYSTEM_ENABLE_om != 0 ) { _rtB -> B_311_694_0 [ 0 ] =
_rtDW -> Integrator_DSTATE_g [ 0 ] ; } else { _rtB -> B_311_694_0 [ 0 ] =
_rtP -> P_1166 * _rtB -> B_311_693_0 [ 0 ] + _rtDW -> Integrator_DSTATE_g [ 0
] ; } _rtB -> B_311_701_0 [ 0 ] = _rtDW -> Delay_DSTATE_dt [ 0 ] ;
rtb_B_311_36_0 = ( ( ( ( _rtB -> B_311_694_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_dh [ 0 ] ) * _rtP -> P_1169 + _rtDW -> UnitDelay1_DSTATE_dh
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_ko [ 0 ] ) + ( _rtB -> B_311_694_0 [ 0 ]
- ( ( _rtB -> B_311_701_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_ko [ 0 ] ) *
_rtP -> P_1172 + _rtDW -> UnitDelay2_DSTATE_ko [ 0 ] ) ) ) * _rtP -> P_1173 *
_rtP -> P_1174 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_k5 != 0 ) { _rtB ->
B_311_711_0 [ 0 ] = _rtDW -> Integrator_DSTATE_eq [ 0 ] ; } else { _rtB ->
B_311_711_0 [ 0 ] = _rtP -> P_1175 * _rtB -> B_311_710_0 [ 0 ] + _rtDW ->
Integrator_DSTATE_eq [ 0 ] ; } _rtB -> B_311_718_0 [ 0 ] = _rtDW ->
Delay_DSTATE_c5 [ 0 ] ; B_311_473_0_idx_0 = ( ( ( ( _rtB -> B_311_711_0 [ 0 ]
- _rtDW -> UnitDelay1_DSTATE_fo [ 0 ] ) * _rtP -> P_1178 + _rtDW ->
UnitDelay1_DSTATE_fo [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_e [ 0 ] ) + ( _rtB
-> B_311_711_0 [ 0 ] - ( ( _rtB -> B_311_718_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_e [ 0 ] ) * _rtP -> P_1181 + _rtDW -> UnitDelay2_DSTATE_e [
0 ] ) ) ) * _rtP -> P_1182 * _rtP -> P_1183 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_li != 0 ) { _rtB -> B_311_622_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_p [ 1 ] ; } else { _rtB -> B_311_622_0 [ 1 ] = _rtP ->
P_1126 * _rtB -> B_311_621_0 [ 1 ] + _rtDW -> Integrator_DSTATE_p [ 1 ] ; }
_rtB -> B_311_629_0 [ 1 ] = _rtDW -> Delay_DSTATE_l [ 1 ] ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_d != 0 ) { _rtB -> B_311_639_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_ec [ 1 ] ; } else { _rtB -> B_311_639_0 [ 1 ] = _rtP ->
P_1135 * _rtB -> B_311_638_0 [ 1 ] + _rtDW -> Integrator_DSTATE_ec [ 1 ] ; }
_rtB -> B_311_646_0 [ 1 ] = _rtDW -> Delay_DSTATE_bg [ 1 ] ;
B_311_401_0_idx_1 = ( ( ( ( _rtB -> B_311_639_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_i3 [ 1 ] ) * _rtP -> P_1138 + _rtDW -> UnitDelay1_DSTATE_i3
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_ni [ 1 ] ) + ( _rtB -> B_311_639_0 [ 1 ]
- ( ( _rtB -> B_311_646_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_ni [ 1 ] ) *
_rtP -> P_1141 + _rtDW -> UnitDelay2_DSTATE_ni [ 1 ] ) ) ) * _rtP -> P_1142 *
_rtP -> P_1143 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_b != 0 ) { _rtB ->
B_311_656_0 [ 1 ] = _rtDW -> Integrator_DSTATE_h [ 1 ] ; } else { _rtB ->
B_311_656_0 [ 1 ] = _rtP -> P_1144 * _rtB -> B_311_655_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_h [ 1 ] ; } _rtB -> B_311_663_0 [ 1 ] = _rtDW ->
Delay_DSTATE_ky [ 1 ] ; rtb_B_311_52_0 = ( ( ( ( _rtB -> B_311_656_0 [ 1 ] -
_rtDW -> UnitDelay1_DSTATE_dq [ 1 ] ) * _rtP -> P_1147 + _rtDW ->
UnitDelay1_DSTATE_dq [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_l [ 1 ] ) + ( _rtB
-> B_311_656_0 [ 1 ] - ( ( _rtB -> B_311_663_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_l [ 1 ] ) * _rtP -> P_1150 + _rtDW -> UnitDelay2_DSTATE_l [
1 ] ) ) ) * _rtP -> P_1151 * _rtP -> P_1152 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_o != 0 ) { _rtB -> B_311_677_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_ff [ 1 ] ; } else { _rtB -> B_311_677_0 [ 1 ] = _rtP ->
P_1157 * _rtB -> B_311_676_0 [ 1 ] + _rtDW -> Integrator_DSTATE_ff [ 1 ] ; }
_rtB -> B_311_684_0 [ 1 ] = _rtDW -> Delay_DSTATE_oe [ 1 ] ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_om != 0 ) { _rtB -> B_311_694_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_g [ 1 ] ; } else { _rtB -> B_311_694_0 [ 1 ] = _rtP ->
P_1166 * _rtB -> B_311_693_0 [ 1 ] + _rtDW -> Integrator_DSTATE_g [ 1 ] ; }
_rtB -> B_311_701_0 [ 1 ] = _rtDW -> Delay_DSTATE_dt [ 1 ] ; rtb_B_299_3_0 =
( ( ( ( _rtB -> B_311_694_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_dh [ 1 ] ) *
_rtP -> P_1169 + _rtDW -> UnitDelay1_DSTATE_dh [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_ko [ 1 ] ) + ( _rtB -> B_311_694_0 [ 1 ] - ( ( _rtB ->
B_311_701_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_ko [ 1 ] ) * _rtP -> P_1172 +
_rtDW -> UnitDelay2_DSTATE_ko [ 1 ] ) ) ) * _rtP -> P_1173 * _rtP -> P_1174 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_k5 != 0 ) { _rtB -> B_311_711_0 [ 1 ]
= _rtDW -> Integrator_DSTATE_eq [ 1 ] ; } else { _rtB -> B_311_711_0 [ 1 ] =
_rtP -> P_1175 * _rtB -> B_311_710_0 [ 1 ] + _rtDW -> Integrator_DSTATE_eq [
1 ] ; } _rtB -> B_311_718_0 [ 1 ] = _rtDW -> Delay_DSTATE_c5 [ 1 ] ;
rtb_B_311_50_0 = ( ( ( ( _rtB -> B_311_711_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_fo [ 1 ] ) * _rtP -> P_1178 + _rtDW -> UnitDelay1_DSTATE_fo
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_e [ 1 ] ) + ( _rtB -> B_311_711_0 [ 1 ]
- ( ( _rtB -> B_311_718_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_e [ 1 ] ) * _rtP
-> P_1181 + _rtDW -> UnitDelay2_DSTATE_e [ 1 ] ) ) ) * _rtP -> P_1182 * _rtP
-> P_1183 ; switch ( ( int32_T ) _rtB -> B_311_244_0 ) { case 1 :
rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_622_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_nr [ 0 ] ) * _rtP -> P_1129 + _rtDW -> UnitDelay1_DSTATE_nr
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_hg [ 0 ] ) + ( _rtB -> B_311_622_0 [ 0 ]
- ( ( _rtB -> B_311_629_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_hg [ 0 ] ) *
_rtP -> P_1132 + _rtDW -> UnitDelay2_DSTATE_hg [ 0 ] ) ) ) * _rtP -> P_1133 *
_rtP -> P_1134 + ( _rtP -> P_4208 . re * rtb_B_311_68_0 - _rtP -> P_4208 . im
* B_311_401_0_idx_1 ) ) + ( _rtP -> P_4207 . re * rtb_B_311_51_0 - _rtP ->
P_4207 . im * rtb_B_311_52_0 ) ) * _rtP -> P_161 ; rtb_B_311_52_0 = ( ( ( ( (
( _rtB -> B_311_622_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_nr [ 1 ] ) * _rtP ->
P_1129 + _rtDW -> UnitDelay1_DSTATE_nr [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_hg [ 1 ] ) + ( _rtB -> B_311_622_0 [ 1 ] - ( ( _rtB ->
B_311_629_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_hg [ 1 ] ) * _rtP -> P_1132 +
_rtDW -> UnitDelay2_DSTATE_hg [ 1 ] ) ) ) * _rtP -> P_1133 * _rtP -> P_1134 +
( _rtP -> P_4208 . re * B_311_401_0_idx_1 + _rtP -> P_4208 . im *
rtb_B_311_68_0 ) ) + ( _rtP -> P_4207 . re * rtb_B_311_52_0 + _rtP -> P_4207
. im * rtb_B_311_51_0 ) ) * _rtP -> P_161 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB
-> B_311_677_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_k0 [ 0 ] ) * _rtP -> P_1160
+ _rtDW -> UnitDelay1_DSTATE_k0 [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_hp [ 0 ]
) + ( _rtB -> B_311_677_0 [ 0 ] - ( ( _rtB -> B_311_684_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_hp [ 0 ] ) * _rtP -> P_1163 + _rtDW -> UnitDelay2_DSTATE_hp
[ 0 ] ) ) ) * _rtP -> P_1164 * _rtP -> P_1165 + ( _rtP -> P_4206 . re *
rtb_B_311_36_0 - _rtP -> P_4206 . im * rtb_B_299_3_0 ) ) + ( _rtP -> P_4205 .
re * B_311_473_0_idx_0 - _rtP -> P_4205 . im * rtb_B_311_50_0 ) ) * _rtP ->
P_160 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB -> B_311_677_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_k0 [ 1 ] ) * _rtP -> P_1160 + _rtDW -> UnitDelay1_DSTATE_k0
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_hp [ 1 ] ) + ( _rtB -> B_311_677_0 [ 1 ]
- ( ( _rtB -> B_311_684_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_hp [ 1 ] ) *
_rtP -> P_1163 + _rtDW -> UnitDelay2_DSTATE_hp [ 1 ] ) ) ) * _rtP -> P_1164 *
_rtP -> P_1165 + ( _rtP -> P_4206 . re * rtb_B_299_3_0 + _rtP -> P_4206 . im
* rtb_B_311_36_0 ) ) + ( _rtP -> P_4205 . re * rtb_B_311_50_0 + _rtP ->
P_4205 . im * B_311_473_0_idx_0 ) ) * _rtP -> P_160 ) ; rtb_B_311_36_0 = (
rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP ->
P_162 ; rtb_B_311_52_0 = ( rtb_B_311_37_0 * rtb_B_299_3_0 + rtb_B_311_52_0 *
rtb_B_311_68_0 ) * _rtP -> P_162 ; break ; case 2 : rtb_B_311_37_0 = ( ( ( (
( ( _rtB -> B_311_622_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_nr [ 0 ] ) * _rtP
-> P_1129 + _rtDW -> UnitDelay1_DSTATE_nr [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_hg [ 0 ] ) + ( _rtB -> B_311_622_0 [ 0 ] - ( ( _rtB ->
B_311_629_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_hg [ 0 ] ) * _rtP -> P_1132 +
_rtDW -> UnitDelay2_DSTATE_hg [ 0 ] ) ) ) * _rtP -> P_1133 * _rtP -> P_1134 +
( _rtP -> P_4204 . re * rtb_B_311_68_0 - _rtP -> P_4204 . im *
B_311_401_0_idx_1 ) ) + ( _rtP -> P_4203 . re * rtb_B_311_51_0 - _rtP ->
P_4203 . im * rtb_B_311_52_0 ) ) * _rtP -> P_158 ; rtb_B_311_52_0 = ( ( ( ( (
( _rtB -> B_311_622_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_nr [ 1 ] ) * _rtP ->
P_1129 + _rtDW -> UnitDelay1_DSTATE_nr [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_hg [ 1 ] ) + ( _rtB -> B_311_622_0 [ 1 ] - ( ( _rtB ->
B_311_629_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_hg [ 1 ] ) * _rtP -> P_1132 +
_rtDW -> UnitDelay2_DSTATE_hg [ 1 ] ) ) ) * _rtP -> P_1133 * _rtP -> P_1134 +
( _rtP -> P_4204 . re * B_311_401_0_idx_1 + _rtP -> P_4204 . im *
rtb_B_311_68_0 ) ) + ( _rtP -> P_4203 . re * rtb_B_311_52_0 + _rtP -> P_4203
. im * rtb_B_311_51_0 ) ) * _rtP -> P_158 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB
-> B_311_677_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_k0 [ 0 ] ) * _rtP -> P_1160
+ _rtDW -> UnitDelay1_DSTATE_k0 [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_hp [ 0 ]
) + ( _rtB -> B_311_677_0 [ 0 ] - ( ( _rtB -> B_311_684_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_hp [ 0 ] ) * _rtP -> P_1163 + _rtDW -> UnitDelay2_DSTATE_hp
[ 0 ] ) ) ) * _rtP -> P_1164 * _rtP -> P_1165 + ( _rtP -> P_4202 . re *
rtb_B_311_36_0 - _rtP -> P_4202 . im * rtb_B_299_3_0 ) ) + ( _rtP -> P_4201 .
re * B_311_473_0_idx_0 - _rtP -> P_4201 . im * rtb_B_311_50_0 ) ) * _rtP ->
P_157 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB -> B_311_677_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_k0 [ 1 ] ) * _rtP -> P_1160 + _rtDW -> UnitDelay1_DSTATE_k0
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_hp [ 1 ] ) + ( _rtB -> B_311_677_0 [ 1 ]
- ( ( _rtB -> B_311_684_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_hp [ 1 ] ) *
_rtP -> P_1163 + _rtDW -> UnitDelay2_DSTATE_hp [ 1 ] ) ) ) * _rtP -> P_1164 *
_rtP -> P_1165 + ( _rtP -> P_4202 . re * rtb_B_299_3_0 + _rtP -> P_4202 . im
* rtb_B_311_36_0 ) ) + ( _rtP -> P_4201 . re * rtb_B_311_50_0 + _rtP ->
P_4201 . im * B_311_473_0_idx_0 ) ) * _rtP -> P_157 ) ; rtb_B_311_36_0 = (
rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP ->
P_159 ; rtb_B_311_52_0 = ( rtb_B_311_37_0 * rtb_B_299_3_0 + rtb_B_311_52_0 *
rtb_B_311_68_0 ) * _rtP -> P_159 ; break ; default : rtb_B_311_37_0 = ( ( ( (
( ( _rtB -> B_311_622_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_nr [ 0 ] ) * _rtP
-> P_1129 + _rtDW -> UnitDelay1_DSTATE_nr [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_hg [ 0 ] ) + ( _rtB -> B_311_622_0 [ 0 ] - ( ( _rtB ->
B_311_629_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_hg [ 0 ] ) * _rtP -> P_1132 +
_rtDW -> UnitDelay2_DSTATE_hg [ 0 ] ) ) ) * _rtP -> P_1133 * _rtP -> P_1134 +
rtb_B_311_68_0 ) + rtb_B_311_51_0 ) * _rtP -> P_155 ; rtb_B_311_52_0 = ( ( (
( ( ( _rtB -> B_311_622_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_nr [ 1 ] ) *
_rtP -> P_1129 + _rtDW -> UnitDelay1_DSTATE_nr [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_hg [ 1 ] ) + ( _rtB -> B_311_622_0 [ 1 ] - ( ( _rtB ->
B_311_629_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_hg [ 1 ] ) * _rtP -> P_1132 +
_rtDW -> UnitDelay2_DSTATE_hg [ 1 ] ) ) ) * _rtP -> P_1133 * _rtP -> P_1134 +
B_311_401_0_idx_1 ) + rtb_B_311_52_0 ) * _rtP -> P_155 ; rtb_B_311_68_0 = ( (
( ( ( ( _rtB -> B_311_677_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_k0 [ 0 ] ) *
_rtP -> P_1160 + _rtDW -> UnitDelay1_DSTATE_k0 [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_hp [ 0 ] ) + ( _rtB -> B_311_677_0 [ 0 ] - ( ( _rtB ->
B_311_684_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_hp [ 0 ] ) * _rtP -> P_1163 +
_rtDW -> UnitDelay2_DSTATE_hp [ 0 ] ) ) ) * _rtP -> P_1164 * _rtP -> P_1165 +
rtb_B_311_36_0 ) + B_311_473_0_idx_0 ) * _rtP -> P_154 ; rtb_B_299_3_0 = - (
( ( ( ( ( ( _rtB -> B_311_677_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_k0 [ 1 ] )
* _rtP -> P_1160 + _rtDW -> UnitDelay1_DSTATE_k0 [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_hp [ 1 ] ) + ( _rtB -> B_311_677_0 [ 1 ] - ( ( _rtB ->
B_311_684_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_hp [ 1 ] ) * _rtP -> P_1163 +
_rtDW -> UnitDelay2_DSTATE_hp [ 1 ] ) ) ) * _rtP -> P_1164 * _rtP -> P_1165 +
rtb_B_299_3_0 ) + rtb_B_311_50_0 ) * _rtP -> P_154 ) ; rtb_B_311_36_0 = (
rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP ->
P_156 ; rtb_B_311_52_0 = ( rtb_B_311_37_0 * rtb_B_299_3_0 + rtb_B_311_52_0 *
rtb_B_311_68_0 ) * _rtP -> P_156 ; break ; } _rtB -> B_311_732_0 = _rtP ->
P_1184 * rtb_B_311_36_0 ; _rtB -> B_311_733_0 = _rtP -> P_1185 *
rtb_B_311_52_0 ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_311_734_0 = _rtDW -> Memory_PreviousInput_e ; } i = ssIsSampleHit ( S , 2 ,
0 ) ; if ( i != 0 ) { B_299_0_0_idx_0 = _rtP -> P_1186 * _rtB -> B_311_152_0
[ 84 ] * _rtP -> P_1189 ; B_299_0_0_idx_1 = _rtP -> P_1187 * _rtB ->
B_311_152_0 [ 85 ] * _rtP -> P_1189 ; B_299_0_0_idx_2 = _rtP -> P_1188 * _rtB
-> B_311_152_0 [ 86 ] * _rtP -> P_1189 ; _rtB -> B_311_741_0 = ( ( _rtDW ->
Integrator_DSTATE_dv < _rtB -> B_311_265_0_a ) || ( _rtDW ->
Integrator_DSTATE_dv >= _rtB -> B_311_266_0_i ) ) ; if ( _rtDW ->
Initial_FirstOutputTime_e ) { _rtDW -> Initial_FirstOutputTime_e = false ;
_rtB -> B_311_747_0 = _rtP -> P_1192 ; } else { _rtB -> B_311_747_0 = _rtDW
-> Integrator_DSTATE_dv - muDoubleScalarFloor ( ( _rtDW ->
Integrator_DSTATE_dv - _rtB -> B_311_267_0 ) * _rtP -> P_1190 ) * _rtP ->
P_1191 ; } if ( _rtB -> B_311_741_0 && ( _rtDW -> Integrator_PrevResetState_g
<= 0 ) ) { _rtDW -> Integrator_DSTATE_dv = _rtB -> B_311_747_0 ; }
muDoubleScalarSinCos ( _rtDW -> Integrator_DSTATE_dv , & rtb_B_311_50_0 , &
rtb_B_311_51_0 ) ; _rtB -> B_311_750_0 [ 0 ] = B_299_0_0_idx_0 *
rtb_B_311_50_0 ; _rtB -> B_311_750_0 [ 1 ] = B_299_0_0_idx_0 * rtb_B_311_51_0
; _rtB -> B_311_767_0 [ 0 ] = B_299_0_0_idx_1 * rtb_B_311_50_0 ; _rtB ->
B_311_767_0 [ 1 ] = B_299_0_0_idx_1 * rtb_B_311_51_0 ; _rtB -> B_311_784_0 [
0 ] = B_299_0_0_idx_2 * rtb_B_311_50_0 ; _rtB -> B_311_784_0 [ 1 ] =
B_299_0_0_idx_2 * rtb_B_311_51_0 ; B_299_0_0_idx_0 = _rtP -> P_1221 * _rtB ->
B_311_152_0 [ 167 ] * _rtP -> P_1224 ; B_299_0_0_idx_1 = _rtP -> P_1222 *
_rtB -> B_311_152_0 [ 168 ] * _rtP -> P_1224 ; B_299_0_0_idx_2 = _rtP ->
P_1223 * _rtB -> B_311_152_0 [ 169 ] * _rtP -> P_1224 ; _rtB -> B_311_805_0 [
0 ] = B_299_0_0_idx_0 * rtb_B_311_50_0 ; _rtB -> B_311_805_0 [ 1 ] =
B_299_0_0_idx_0 * rtb_B_311_51_0 ; _rtB -> B_311_822_0 [ 0 ] =
B_299_0_0_idx_1 * rtb_B_311_50_0 ; _rtB -> B_311_822_0 [ 1 ] =
B_299_0_0_idx_1 * rtb_B_311_51_0 ; _rtB -> B_311_839_0 [ 0 ] =
B_299_0_0_idx_2 * rtb_B_311_50_0 ; _rtB -> B_311_839_0 [ 1 ] =
B_299_0_0_idx_2 * rtb_B_311_51_0 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_pr
!= 0 ) { _rtB -> B_311_751_0 [ 0 ] = _rtDW -> Integrator_DSTATE_m [ 0 ] ; }
else { _rtB -> B_311_751_0 [ 0 ] = _rtP -> P_1194 * _rtB -> B_311_750_0 [ 0 ]
+ _rtDW -> Integrator_DSTATE_m [ 0 ] ; } _rtB -> B_311_758_0 [ 0 ] = _rtDW ->
Delay_DSTATE_f0 [ 0 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_mt != 0 ) {
_rtB -> B_311_768_0 [ 0 ] = _rtDW -> Integrator_DSTATE_aj [ 0 ] ; } else {
_rtB -> B_311_768_0 [ 0 ] = _rtP -> P_1203 * _rtB -> B_311_767_0 [ 0 ] +
_rtDW -> Integrator_DSTATE_aj [ 0 ] ; } _rtB -> B_311_775_0 [ 0 ] = _rtDW ->
Delay_DSTATE_ca [ 0 ] ; rtb_B_311_68_0 = ( ( ( ( _rtB -> B_311_768_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_ie [ 0 ] ) * _rtP -> P_1206 + _rtDW ->
UnitDelay1_DSTATE_ie [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_a [ 0 ] ) + ( _rtB
-> B_311_768_0 [ 0 ] - ( ( _rtB -> B_311_775_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_a [ 0 ] ) * _rtP -> P_1209 + _rtDW -> UnitDelay2_DSTATE_a [
0 ] ) ) ) * _rtP -> P_1210 * _rtP -> P_1211 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_e != 0 ) { _rtB -> B_311_785_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_pb [ 0 ] ; } else { _rtB -> B_311_785_0 [ 0 ] = _rtP ->
P_1212 * _rtB -> B_311_784_0 [ 0 ] + _rtDW -> Integrator_DSTATE_pb [ 0 ] ; }
_rtB -> B_311_792_0 [ 0 ] = _rtDW -> Delay_DSTATE_e [ 0 ] ; rtb_B_311_51_0 =
( ( ( ( _rtB -> B_311_785_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_i4 [ 0 ] ) *
_rtP -> P_1215 + _rtDW -> UnitDelay1_DSTATE_i4 [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_g [ 0 ] ) + ( _rtB -> B_311_785_0 [ 0 ] - ( ( _rtB ->
B_311_792_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_g [ 0 ] ) * _rtP -> P_1218 +
_rtDW -> UnitDelay2_DSTATE_g [ 0 ] ) ) ) * _rtP -> P_1219 * _rtP -> P_1220 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_bc != 0 ) { _rtB -> B_311_806_0 [ 0 ]
= _rtDW -> Integrator_DSTATE_jk [ 0 ] ; } else { _rtB -> B_311_806_0 [ 0 ] =
_rtP -> P_1225 * _rtB -> B_311_805_0 [ 0 ] + _rtDW -> Integrator_DSTATE_jk [
0 ] ; } _rtB -> B_311_813_0 [ 0 ] = _rtDW -> Delay_DSTATE_de [ 0 ] ; if (
_rtDW -> Integrator_SYSTEM_ENABLE_ks != 0 ) { _rtB -> B_311_823_0 [ 0 ] =
_rtDW -> Integrator_DSTATE_hx [ 0 ] ; } else { _rtB -> B_311_823_0 [ 0 ] =
_rtP -> P_1234 * _rtB -> B_311_822_0 [ 0 ] + _rtDW -> Integrator_DSTATE_hx [
0 ] ; } _rtB -> B_311_830_0 [ 0 ] = _rtDW -> Delay_DSTATE_dj [ 0 ] ;
rtb_B_311_36_0 = ( ( ( ( _rtB -> B_311_823_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_bm [ 0 ] ) * _rtP -> P_1237 + _rtDW -> UnitDelay1_DSTATE_bm
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_hgp [ 0 ] ) + ( _rtB -> B_311_823_0 [ 0
] - ( ( _rtB -> B_311_830_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_hgp [ 0 ] ) *
_rtP -> P_1240 + _rtDW -> UnitDelay2_DSTATE_hgp [ 0 ] ) ) ) * _rtP -> P_1241
* _rtP -> P_1242 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_kf != 0 ) { _rtB ->
B_311_840_0 [ 0 ] = _rtDW -> Integrator_DSTATE_i [ 0 ] ; } else { _rtB ->
B_311_840_0 [ 0 ] = _rtP -> P_1243 * _rtB -> B_311_839_0 [ 0 ] + _rtDW ->
Integrator_DSTATE_i [ 0 ] ; } _rtB -> B_311_847_0 [ 0 ] = _rtDW ->
Delay_DSTATE_i [ 0 ] ; B_311_473_0_idx_0 = ( ( ( ( _rtB -> B_311_840_0 [ 0 ]
- _rtDW -> UnitDelay1_DSTATE_id [ 0 ] ) * _rtP -> P_1246 + _rtDW ->
UnitDelay1_DSTATE_id [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_p [ 0 ] ) + ( _rtB
-> B_311_840_0 [ 0 ] - ( ( _rtB -> B_311_847_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_p [ 0 ] ) * _rtP -> P_1249 + _rtDW -> UnitDelay2_DSTATE_p [
0 ] ) ) ) * _rtP -> P_1250 * _rtP -> P_1251 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_pr != 0 ) { _rtB -> B_311_751_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_m [ 1 ] ; } else { _rtB -> B_311_751_0 [ 1 ] = _rtP ->
P_1194 * _rtB -> B_311_750_0 [ 1 ] + _rtDW -> Integrator_DSTATE_m [ 1 ] ; }
_rtB -> B_311_758_0 [ 1 ] = _rtDW -> Delay_DSTATE_f0 [ 1 ] ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_mt != 0 ) { _rtB -> B_311_768_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_aj [ 1 ] ; } else { _rtB -> B_311_768_0 [ 1 ] = _rtP ->
P_1203 * _rtB -> B_311_767_0 [ 1 ] + _rtDW -> Integrator_DSTATE_aj [ 1 ] ; }
_rtB -> B_311_775_0 [ 1 ] = _rtDW -> Delay_DSTATE_ca [ 1 ] ;
B_311_401_0_idx_1 = ( ( ( ( _rtB -> B_311_768_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_ie [ 1 ] ) * _rtP -> P_1206 + _rtDW -> UnitDelay1_DSTATE_ie
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_a [ 1 ] ) + ( _rtB -> B_311_768_0 [ 1 ]
- ( ( _rtB -> B_311_775_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_a [ 1 ] ) * _rtP
-> P_1209 + _rtDW -> UnitDelay2_DSTATE_a [ 1 ] ) ) ) * _rtP -> P_1210 * _rtP
-> P_1211 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_e != 0 ) { _rtB ->
B_311_785_0 [ 1 ] = _rtDW -> Integrator_DSTATE_pb [ 1 ] ; } else { _rtB ->
B_311_785_0 [ 1 ] = _rtP -> P_1212 * _rtB -> B_311_784_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_pb [ 1 ] ; } _rtB -> B_311_792_0 [ 1 ] = _rtDW ->
Delay_DSTATE_e [ 1 ] ; rtb_B_311_52_0 = ( ( ( ( _rtB -> B_311_785_0 [ 1 ] -
_rtDW -> UnitDelay1_DSTATE_i4 [ 1 ] ) * _rtP -> P_1215 + _rtDW ->
UnitDelay1_DSTATE_i4 [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_g [ 1 ] ) + ( _rtB
-> B_311_785_0 [ 1 ] - ( ( _rtB -> B_311_792_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_g [ 1 ] ) * _rtP -> P_1218 + _rtDW -> UnitDelay2_DSTATE_g [
1 ] ) ) ) * _rtP -> P_1219 * _rtP -> P_1220 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_bc != 0 ) { _rtB -> B_311_806_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_jk [ 1 ] ; } else { _rtB -> B_311_806_0 [ 1 ] = _rtP ->
P_1225 * _rtB -> B_311_805_0 [ 1 ] + _rtDW -> Integrator_DSTATE_jk [ 1 ] ; }
_rtB -> B_311_813_0 [ 1 ] = _rtDW -> Delay_DSTATE_de [ 1 ] ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_ks != 0 ) { _rtB -> B_311_823_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_hx [ 1 ] ; } else { _rtB -> B_311_823_0 [ 1 ] = _rtP ->
P_1234 * _rtB -> B_311_822_0 [ 1 ] + _rtDW -> Integrator_DSTATE_hx [ 1 ] ; }
_rtB -> B_311_830_0 [ 1 ] = _rtDW -> Delay_DSTATE_dj [ 1 ] ; rtb_B_299_3_0 =
( ( ( ( _rtB -> B_311_823_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_bm [ 1 ] ) *
_rtP -> P_1237 + _rtDW -> UnitDelay1_DSTATE_bm [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_hgp [ 1 ] ) + ( _rtB -> B_311_823_0 [ 1 ] - ( ( _rtB ->
B_311_830_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_hgp [ 1 ] ) * _rtP -> P_1240 +
_rtDW -> UnitDelay2_DSTATE_hgp [ 1 ] ) ) ) * _rtP -> P_1241 * _rtP -> P_1242
; if ( _rtDW -> Integrator_SYSTEM_ENABLE_kf != 0 ) { _rtB -> B_311_840_0 [ 1
] = _rtDW -> Integrator_DSTATE_i [ 1 ] ; } else { _rtB -> B_311_840_0 [ 1 ] =
_rtP -> P_1243 * _rtB -> B_311_839_0 [ 1 ] + _rtDW -> Integrator_DSTATE_i [ 1
] ; } _rtB -> B_311_847_0 [ 1 ] = _rtDW -> Delay_DSTATE_i [ 1 ] ;
rtb_B_311_50_0 = ( ( ( ( _rtB -> B_311_840_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_id [ 1 ] ) * _rtP -> P_1246 + _rtDW -> UnitDelay1_DSTATE_id
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_p [ 1 ] ) + ( _rtB -> B_311_840_0 [ 1 ]
- ( ( _rtB -> B_311_847_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_p [ 1 ] ) * _rtP
-> P_1249 + _rtDW -> UnitDelay2_DSTATE_p [ 1 ] ) ) ) * _rtP -> P_1250 * _rtP
-> P_1251 ; switch ( ( int32_T ) _rtB -> B_311_264_0_a ) { case 1 :
rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_751_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_e [ 0 ] ) * _rtP -> P_1197 + _rtDW -> UnitDelay1_DSTATE_e [
0 ] ) - _rtDW -> UnitDelay2_DSTATE_bw [ 0 ] ) + ( _rtB -> B_311_751_0 [ 0 ] -
( ( _rtB -> B_311_758_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_bw [ 0 ] ) * _rtP
-> P_1200 + _rtDW -> UnitDelay2_DSTATE_bw [ 0 ] ) ) ) * _rtP -> P_1201 * _rtP
-> P_1202 + ( _rtP -> P_4240 . re * rtb_B_311_68_0 - _rtP -> P_4240 . im *
B_311_401_0_idx_1 ) ) + ( _rtP -> P_4239 . re * rtb_B_311_51_0 - _rtP ->
P_4239 . im * rtb_B_311_52_0 ) ) * _rtP -> P_197 ; rtb_B_311_52_0 = ( ( ( ( (
( _rtB -> B_311_751_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_e [ 1 ] ) * _rtP ->
P_1197 + _rtDW -> UnitDelay1_DSTATE_e [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_bw
[ 1 ] ) + ( _rtB -> B_311_751_0 [ 1 ] - ( ( _rtB -> B_311_758_0 [ 1 ] - _rtDW
-> UnitDelay2_DSTATE_bw [ 1 ] ) * _rtP -> P_1200 + _rtDW ->
UnitDelay2_DSTATE_bw [ 1 ] ) ) ) * _rtP -> P_1201 * _rtP -> P_1202 + ( _rtP
-> P_4240 . re * B_311_401_0_idx_1 + _rtP -> P_4240 . im * rtb_B_311_68_0 ) )
+ ( _rtP -> P_4239 . re * rtb_B_311_52_0 + _rtP -> P_4239 . im *
rtb_B_311_51_0 ) ) * _rtP -> P_197 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB ->
B_311_806_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_ej [ 0 ] ) * _rtP -> P_1228 +
_rtDW -> UnitDelay1_DSTATE_ej [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_ml [ 0 ] )
+ ( _rtB -> B_311_806_0 [ 0 ] - ( ( _rtB -> B_311_813_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_ml [ 0 ] ) * _rtP -> P_1231 + _rtDW -> UnitDelay2_DSTATE_ml
[ 0 ] ) ) ) * _rtP -> P_1232 * _rtP -> P_1233 + ( _rtP -> P_4238 . re *
rtb_B_311_36_0 - _rtP -> P_4238 . im * rtb_B_299_3_0 ) ) + ( _rtP -> P_4237 .
re * B_311_473_0_idx_0 - _rtP -> P_4237 . im * rtb_B_311_50_0 ) ) * _rtP ->
P_196 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB -> B_311_806_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_ej [ 1 ] ) * _rtP -> P_1228 + _rtDW -> UnitDelay1_DSTATE_ej
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_ml [ 1 ] ) + ( _rtB -> B_311_806_0 [ 1 ]
- ( ( _rtB -> B_311_813_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_ml [ 1 ] ) *
_rtP -> P_1231 + _rtDW -> UnitDelay2_DSTATE_ml [ 1 ] ) ) ) * _rtP -> P_1232 *
_rtP -> P_1233 + ( _rtP -> P_4238 . re * rtb_B_299_3_0 + _rtP -> P_4238 . im
* rtb_B_311_36_0 ) ) + ( _rtP -> P_4237 . re * rtb_B_311_50_0 + _rtP ->
P_4237 . im * B_311_473_0_idx_0 ) ) * _rtP -> P_196 ) ; _rtB -> B_311_860_0 =
( rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP
-> P_198 ; _rtB -> B_311_860_1 = ( rtb_B_311_37_0 * rtb_B_299_3_0 +
rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP -> P_198 ; break ; case 2 :
rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_751_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_e [ 0 ] ) * _rtP -> P_1197 + _rtDW -> UnitDelay1_DSTATE_e [
0 ] ) - _rtDW -> UnitDelay2_DSTATE_bw [ 0 ] ) + ( _rtB -> B_311_751_0 [ 0 ] -
( ( _rtB -> B_311_758_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_bw [ 0 ] ) * _rtP
-> P_1200 + _rtDW -> UnitDelay2_DSTATE_bw [ 0 ] ) ) ) * _rtP -> P_1201 * _rtP
-> P_1202 + ( _rtP -> P_4236 . re * rtb_B_311_68_0 - _rtP -> P_4236 . im *
B_311_401_0_idx_1 ) ) + ( _rtP -> P_4235 . re * rtb_B_311_51_0 - _rtP ->
P_4235 . im * rtb_B_311_52_0 ) ) * _rtP -> P_194 ; rtb_B_311_52_0 = ( ( ( ( (
( _rtB -> B_311_751_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_e [ 1 ] ) * _rtP ->
P_1197 + _rtDW -> UnitDelay1_DSTATE_e [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_bw
[ 1 ] ) + ( _rtB -> B_311_751_0 [ 1 ] - ( ( _rtB -> B_311_758_0 [ 1 ] - _rtDW
-> UnitDelay2_DSTATE_bw [ 1 ] ) * _rtP -> P_1200 + _rtDW ->
UnitDelay2_DSTATE_bw [ 1 ] ) ) ) * _rtP -> P_1201 * _rtP -> P_1202 + ( _rtP
-> P_4236 . re * B_311_401_0_idx_1 + _rtP -> P_4236 . im * rtb_B_311_68_0 ) )
+ ( _rtP -> P_4235 . re * rtb_B_311_52_0 + _rtP -> P_4235 . im *
rtb_B_311_51_0 ) ) * _rtP -> P_194 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB ->
B_311_806_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_ej [ 0 ] ) * _rtP -> P_1228 +
_rtDW -> UnitDelay1_DSTATE_ej [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_ml [ 0 ] )
+ ( _rtB -> B_311_806_0 [ 0 ] - ( ( _rtB -> B_311_813_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_ml [ 0 ] ) * _rtP -> P_1231 + _rtDW -> UnitDelay2_DSTATE_ml
[ 0 ] ) ) ) * _rtP -> P_1232 * _rtP -> P_1233 + ( _rtP -> P_4234 . re *
rtb_B_311_36_0 - _rtP -> P_4234 . im * rtb_B_299_3_0 ) ) + ( _rtP -> P_4233 .
re * B_311_473_0_idx_0 - _rtP -> P_4233 . im * rtb_B_311_50_0 ) ) * _rtP ->
P_193 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB -> B_311_806_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_ej [ 1 ] ) * _rtP -> P_1228 + _rtDW -> UnitDelay1_DSTATE_ej
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_ml [ 1 ] ) + ( _rtB -> B_311_806_0 [ 1 ]
- ( ( _rtB -> B_311_813_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_ml [ 1 ] ) *
_rtP -> P_1231 + _rtDW -> UnitDelay2_DSTATE_ml [ 1 ] ) ) ) * _rtP -> P_1232 *
_rtP -> P_1233 + ( _rtP -> P_4234 . re * rtb_B_299_3_0 + _rtP -> P_4234 . im
* rtb_B_311_36_0 ) ) + ( _rtP -> P_4233 . re * rtb_B_311_50_0 + _rtP ->
P_4233 . im * B_311_473_0_idx_0 ) ) * _rtP -> P_193 ) ; _rtB -> B_311_860_0 =
( rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP
-> P_195 ; _rtB -> B_311_860_1 = ( rtb_B_311_37_0 * rtb_B_299_3_0 +
rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP -> P_195 ; break ; default :
rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_751_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_e [ 0 ] ) * _rtP -> P_1197 + _rtDW -> UnitDelay1_DSTATE_e [
0 ] ) - _rtDW -> UnitDelay2_DSTATE_bw [ 0 ] ) + ( _rtB -> B_311_751_0 [ 0 ] -
( ( _rtB -> B_311_758_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_bw [ 0 ] ) * _rtP
-> P_1200 + _rtDW -> UnitDelay2_DSTATE_bw [ 0 ] ) ) ) * _rtP -> P_1201 * _rtP
-> P_1202 + rtb_B_311_68_0 ) + rtb_B_311_51_0 ) * _rtP -> P_191 ;
rtb_B_311_52_0 = ( ( ( ( ( ( _rtB -> B_311_751_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_e [ 1 ] ) * _rtP -> P_1197 + _rtDW -> UnitDelay1_DSTATE_e [
1 ] ) - _rtDW -> UnitDelay2_DSTATE_bw [ 1 ] ) + ( _rtB -> B_311_751_0 [ 1 ] -
( ( _rtB -> B_311_758_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_bw [ 1 ] ) * _rtP
-> P_1200 + _rtDW -> UnitDelay2_DSTATE_bw [ 1 ] ) ) ) * _rtP -> P_1201 * _rtP
-> P_1202 + B_311_401_0_idx_1 ) + rtb_B_311_52_0 ) * _rtP -> P_191 ;
rtb_B_311_68_0 = ( ( ( ( ( ( _rtB -> B_311_806_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_ej [ 0 ] ) * _rtP -> P_1228 + _rtDW -> UnitDelay1_DSTATE_ej
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_ml [ 0 ] ) + ( _rtB -> B_311_806_0 [ 0 ]
- ( ( _rtB -> B_311_813_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_ml [ 0 ] ) *
_rtP -> P_1231 + _rtDW -> UnitDelay2_DSTATE_ml [ 0 ] ) ) ) * _rtP -> P_1232 *
_rtP -> P_1233 + rtb_B_311_36_0 ) + B_311_473_0_idx_0 ) * _rtP -> P_190 ;
rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB -> B_311_806_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_ej [ 1 ] ) * _rtP -> P_1228 + _rtDW -> UnitDelay1_DSTATE_ej
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_ml [ 1 ] ) + ( _rtB -> B_311_806_0 [ 1 ]
- ( ( _rtB -> B_311_813_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_ml [ 1 ] ) *
_rtP -> P_1231 + _rtDW -> UnitDelay2_DSTATE_ml [ 1 ] ) ) ) * _rtP -> P_1232 *
_rtP -> P_1233 + rtb_B_299_3_0 ) + rtb_B_311_50_0 ) * _rtP -> P_190 ) ; _rtB
-> B_311_860_0 = ( rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 *
rtb_B_299_3_0 ) * _rtP -> P_192 ; _rtB -> B_311_860_1 = ( rtb_B_311_37_0 *
rtb_B_299_3_0 + rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP -> P_192 ; break ; }
} i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { rtb_B_311_35_0 = ( 1.0 -
rtb_B_311_47_0 / ( _rtP -> P_1252 * rtb_B_311_35_0 ) ) * 100.0 ; if (
rtb_B_311_35_0 > _rtP -> P_1253 ) { rtb_B_311_35_0 = _rtP -> P_1253 ; } else
{ if ( rtb_B_311_35_0 < _rtP -> P_1254 ) { rtb_B_311_35_0 = _rtP -> P_1254 ;
} } _rtB -> B_311_864_0 = _rtP -> P_1255 * rtb_B_311_35_0 ; } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { B_299_0_0_idx_0 = _rtP ->
P_1256 * _rtB -> B_311_152_0 [ 63 ] * _rtP -> P_1259 ; B_299_0_0_idx_1 = _rtP
-> P_1257 * _rtB -> B_311_152_0 [ 64 ] * _rtP -> P_1259 ; B_299_0_0_idx_2 =
_rtP -> P_1258 * _rtB -> B_311_152_0 [ 65 ] * _rtP -> P_1259 ; _rtB ->
B_311_871_0 = ( ( _rtDW -> Integrator_DSTATE_ej < _rtB -> B_311_255_0_p ) ||
( _rtDW -> Integrator_DSTATE_ej >= _rtB -> B_311_256_0 ) ) ; if ( _rtDW ->
Initial_FirstOutputTime_k ) { _rtDW -> Initial_FirstOutputTime_k = false ;
_rtB -> B_311_877_0 = _rtP -> P_1262 ; } else { _rtB -> B_311_877_0 = _rtDW
-> Integrator_DSTATE_ej - muDoubleScalarFloor ( ( _rtDW ->
Integrator_DSTATE_ej - _rtB -> B_311_257_0_p ) * _rtP -> P_1260 ) * _rtP ->
P_1261 ; } if ( _rtB -> B_311_871_0 && ( _rtDW -> Integrator_PrevResetState_b
<= 0 ) ) { _rtDW -> Integrator_DSTATE_ej = _rtB -> B_311_877_0 ; }
muDoubleScalarSinCos ( _rtDW -> Integrator_DSTATE_ej , & rtb_B_311_35_0 , &
rtb_B_311_50_0 ) ; _rtB -> B_311_880_0 [ 0 ] = B_299_0_0_idx_0 *
rtb_B_311_35_0 ; _rtB -> B_311_880_0 [ 1 ] = B_299_0_0_idx_0 * rtb_B_311_50_0
; _rtB -> B_311_897_0 [ 0 ] = B_299_0_0_idx_1 * rtb_B_311_35_0 ; _rtB ->
B_311_897_0 [ 1 ] = B_299_0_0_idx_1 * rtb_B_311_50_0 ; _rtB -> B_311_914_0 [
0 ] = B_299_0_0_idx_2 * rtb_B_311_35_0 ; _rtB -> B_311_914_0 [ 1 ] =
B_299_0_0_idx_2 * rtb_B_311_50_0 ; B_299_0_0_idx_0 = _rtP -> P_1291 * _rtB ->
B_311_152_0 [ 146 ] * _rtP -> P_1294 ; B_299_0_0_idx_1 = _rtP -> P_1292 *
_rtB -> B_311_152_0 [ 147 ] * _rtP -> P_1294 ; B_299_0_0_idx_2 = _rtP ->
P_1293 * _rtB -> B_311_152_0 [ 148 ] * _rtP -> P_1294 ; _rtB -> B_311_935_0 [
0 ] = B_299_0_0_idx_0 * rtb_B_311_35_0 ; _rtB -> B_311_935_0 [ 1 ] =
B_299_0_0_idx_0 * rtb_B_311_50_0 ; _rtB -> B_311_952_0 [ 0 ] =
B_299_0_0_idx_1 * rtb_B_311_35_0 ; _rtB -> B_311_952_0 [ 1 ] =
B_299_0_0_idx_1 * rtb_B_311_50_0 ; _rtB -> B_311_969_0 [ 0 ] =
B_299_0_0_idx_2 * rtb_B_311_35_0 ; _rtB -> B_311_969_0 [ 1 ] =
B_299_0_0_idx_2 * rtb_B_311_50_0 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_jw
!= 0 ) { _rtB -> B_311_881_0 [ 0 ] = _rtDW -> Integrator_DSTATE_pk [ 0 ] ; }
else { _rtB -> B_311_881_0 [ 0 ] = _rtP -> P_1264 * _rtB -> B_311_880_0 [ 0 ]
+ _rtDW -> Integrator_DSTATE_pk [ 0 ] ; } _rtB -> B_311_888_0 [ 0 ] = _rtDW
-> Delay_DSTATE_bf [ 0 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_mh != 0 ) {
_rtB -> B_311_898_0 [ 0 ] = _rtDW -> Integrator_DSTATE_o4 [ 0 ] ; } else {
_rtB -> B_311_898_0 [ 0 ] = _rtP -> P_1273 * _rtB -> B_311_897_0 [ 0 ] +
_rtDW -> Integrator_DSTATE_o4 [ 0 ] ; } _rtB -> B_311_905_0 [ 0 ] = _rtDW ->
Delay_DSTATE_i1 [ 0 ] ; rtb_B_311_68_0 = ( ( ( ( _rtB -> B_311_898_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_o [ 0 ] ) * _rtP -> P_1276 + _rtDW ->
UnitDelay1_DSTATE_o [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_p0 [ 0 ] ) + ( _rtB
-> B_311_898_0 [ 0 ] - ( ( _rtB -> B_311_905_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_p0 [ 0 ] ) * _rtP -> P_1279 + _rtDW -> UnitDelay2_DSTATE_p0
[ 0 ] ) ) ) * _rtP -> P_1280 * _rtP -> P_1281 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_lih != 0 ) { _rtB -> B_311_915_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_hj [ 0 ] ; } else { _rtB -> B_311_915_0 [ 0 ] = _rtP ->
P_1282 * _rtB -> B_311_914_0 [ 0 ] + _rtDW -> Integrator_DSTATE_hj [ 0 ] ; }
_rtB -> B_311_922_0 [ 0 ] = _rtDW -> Delay_DSTATE_mn [ 0 ] ; rtb_B_311_51_0 =
( ( ( ( _rtB -> B_311_915_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_mc [ 0 ] ) *
_rtP -> P_1285 + _rtDW -> UnitDelay1_DSTATE_mc [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_h1 [ 0 ] ) + ( _rtB -> B_311_915_0 [ 0 ] - ( ( _rtB ->
B_311_922_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_h1 [ 0 ] ) * _rtP -> P_1288 +
_rtDW -> UnitDelay2_DSTATE_h1 [ 0 ] ) ) ) * _rtP -> P_1289 * _rtP -> P_1290 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_nx != 0 ) { _rtB -> B_311_936_0 [ 0 ]
= _rtDW -> Integrator_DSTATE_gj [ 0 ] ; } else { _rtB -> B_311_936_0 [ 0 ] =
_rtP -> P_1295 * _rtB -> B_311_935_0 [ 0 ] + _rtDW -> Integrator_DSTATE_gj [
0 ] ; } _rtB -> B_311_943_0 [ 0 ] = _rtDW -> Delay_DSTATE_f1 [ 0 ] ; if (
_rtDW -> Integrator_SYSTEM_ENABLE_l2 != 0 ) { _rtB -> B_311_953_0 [ 0 ] =
_rtDW -> Integrator_DSTATE_l [ 0 ] ; } else { _rtB -> B_311_953_0 [ 0 ] =
_rtP -> P_1304 * _rtB -> B_311_952_0 [ 0 ] + _rtDW -> Integrator_DSTATE_l [ 0
] ; } _rtB -> B_311_960_0 [ 0 ] = _rtDW -> Delay_DSTATE_p1 [ 0 ] ;
rtb_B_311_36_0 = ( ( ( ( _rtB -> B_311_953_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_n5 [ 0 ] ) * _rtP -> P_1307 + _rtDW -> UnitDelay1_DSTATE_n5
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_o [ 0 ] ) + ( _rtB -> B_311_953_0 [ 0 ]
- ( ( _rtB -> B_311_960_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_o [ 0 ] ) * _rtP
-> P_1310 + _rtDW -> UnitDelay2_DSTATE_o [ 0 ] ) ) ) * _rtP -> P_1311 * _rtP
-> P_1312 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_a != 0 ) { _rtB ->
B_311_970_0 [ 0 ] = _rtDW -> Integrator_DSTATE_j3 [ 0 ] ; } else { _rtB ->
B_311_970_0 [ 0 ] = _rtP -> P_1313 * _rtB -> B_311_969_0 [ 0 ] + _rtDW ->
Integrator_DSTATE_j3 [ 0 ] ; } _rtB -> B_311_977_0 [ 0 ] = _rtDW ->
Delay_DSTATE_cl [ 0 ] ; B_311_473_0_idx_0 = ( ( ( ( _rtB -> B_311_970_0 [ 0 ]
- _rtDW -> UnitDelay1_DSTATE_lqg [ 0 ] ) * _rtP -> P_1316 + _rtDW ->
UnitDelay1_DSTATE_lqg [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_oq [ 0 ] ) + (
_rtB -> B_311_970_0 [ 0 ] - ( ( _rtB -> B_311_977_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_oq [ 0 ] ) * _rtP -> P_1319 + _rtDW -> UnitDelay2_DSTATE_oq
[ 0 ] ) ) ) * _rtP -> P_1320 * _rtP -> P_1321 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_jw != 0 ) { _rtB -> B_311_881_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_pk [ 1 ] ; } else { _rtB -> B_311_881_0 [ 1 ] = _rtP ->
P_1264 * _rtB -> B_311_880_0 [ 1 ] + _rtDW -> Integrator_DSTATE_pk [ 1 ] ; }
_rtB -> B_311_888_0 [ 1 ] = _rtDW -> Delay_DSTATE_bf [ 1 ] ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_mh != 0 ) { _rtB -> B_311_898_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_o4 [ 1 ] ; } else { _rtB -> B_311_898_0 [ 1 ] = _rtP ->
P_1273 * _rtB -> B_311_897_0 [ 1 ] + _rtDW -> Integrator_DSTATE_o4 [ 1 ] ; }
_rtB -> B_311_905_0 [ 1 ] = _rtDW -> Delay_DSTATE_i1 [ 1 ] ;
B_311_401_0_idx_1 = ( ( ( ( _rtB -> B_311_898_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_o [ 1 ] ) * _rtP -> P_1276 + _rtDW -> UnitDelay1_DSTATE_o [
1 ] ) - _rtDW -> UnitDelay2_DSTATE_p0 [ 1 ] ) + ( _rtB -> B_311_898_0 [ 1 ] -
( ( _rtB -> B_311_905_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_p0 [ 1 ] ) * _rtP
-> P_1279 + _rtDW -> UnitDelay2_DSTATE_p0 [ 1 ] ) ) ) * _rtP -> P_1280 * _rtP
-> P_1281 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_lih != 0 ) { _rtB ->
B_311_915_0 [ 1 ] = _rtDW -> Integrator_DSTATE_hj [ 1 ] ; } else { _rtB ->
B_311_915_0 [ 1 ] = _rtP -> P_1282 * _rtB -> B_311_914_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_hj [ 1 ] ; } _rtB -> B_311_922_0 [ 1 ] = _rtDW ->
Delay_DSTATE_mn [ 1 ] ; rtb_B_311_52_0 = ( ( ( ( _rtB -> B_311_915_0 [ 1 ] -
_rtDW -> UnitDelay1_DSTATE_mc [ 1 ] ) * _rtP -> P_1285 + _rtDW ->
UnitDelay1_DSTATE_mc [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_h1 [ 1 ] ) + ( _rtB
-> B_311_915_0 [ 1 ] - ( ( _rtB -> B_311_922_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_h1 [ 1 ] ) * _rtP -> P_1288 + _rtDW -> UnitDelay2_DSTATE_h1
[ 1 ] ) ) ) * _rtP -> P_1289 * _rtP -> P_1290 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_nx != 0 ) { _rtB -> B_311_936_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_gj [ 1 ] ; } else { _rtB -> B_311_936_0 [ 1 ] = _rtP ->
P_1295 * _rtB -> B_311_935_0 [ 1 ] + _rtDW -> Integrator_DSTATE_gj [ 1 ] ; }
_rtB -> B_311_943_0 [ 1 ] = _rtDW -> Delay_DSTATE_f1 [ 1 ] ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_l2 != 0 ) { _rtB -> B_311_953_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_l [ 1 ] ; } else { _rtB -> B_311_953_0 [ 1 ] = _rtP ->
P_1304 * _rtB -> B_311_952_0 [ 1 ] + _rtDW -> Integrator_DSTATE_l [ 1 ] ; }
_rtB -> B_311_960_0 [ 1 ] = _rtDW -> Delay_DSTATE_p1 [ 1 ] ; rtb_B_299_3_0 =
( ( ( ( _rtB -> B_311_953_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_n5 [ 1 ] ) *
_rtP -> P_1307 + _rtDW -> UnitDelay1_DSTATE_n5 [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_o [ 1 ] ) + ( _rtB -> B_311_953_0 [ 1 ] - ( ( _rtB ->
B_311_960_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_o [ 1 ] ) * _rtP -> P_1310 +
_rtDW -> UnitDelay2_DSTATE_o [ 1 ] ) ) ) * _rtP -> P_1311 * _rtP -> P_1312 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_a != 0 ) { _rtB -> B_311_970_0 [ 1 ] =
_rtDW -> Integrator_DSTATE_j3 [ 1 ] ; } else { _rtB -> B_311_970_0 [ 1 ] =
_rtP -> P_1313 * _rtB -> B_311_969_0 [ 1 ] + _rtDW -> Integrator_DSTATE_j3 [
1 ] ; } _rtB -> B_311_977_0 [ 1 ] = _rtDW -> Delay_DSTATE_cl [ 1 ] ;
rtb_B_311_50_0 = ( ( ( ( _rtB -> B_311_970_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_lqg [ 1 ] ) * _rtP -> P_1316 + _rtDW ->
UnitDelay1_DSTATE_lqg [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_oq [ 1 ] ) + (
_rtB -> B_311_970_0 [ 1 ] - ( ( _rtB -> B_311_977_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_oq [ 1 ] ) * _rtP -> P_1319 + _rtDW -> UnitDelay2_DSTATE_oq
[ 1 ] ) ) ) * _rtP -> P_1320 * _rtP -> P_1321 ; switch ( ( int32_T ) _rtB ->
B_311_254_0_c ) { case 1 : rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_881_0 [
0 ] - _rtDW -> UnitDelay1_DSTATE_lq [ 0 ] ) * _rtP -> P_1267 + _rtDW ->
UnitDelay1_DSTATE_lq [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_fr [ 0 ] ) + ( _rtB
-> B_311_881_0 [ 0 ] - ( ( _rtB -> B_311_888_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_fr [ 0 ] ) * _rtP -> P_1270 + _rtDW -> UnitDelay2_DSTATE_fr
[ 0 ] ) ) ) * _rtP -> P_1271 * _rtP -> P_1272 + ( _rtP -> P_4224 . re *
rtb_B_311_68_0 - _rtP -> P_4224 . im * B_311_401_0_idx_1 ) ) + ( _rtP ->
P_4223 . re * rtb_B_311_51_0 - _rtP -> P_4223 . im * rtb_B_311_52_0 ) ) *
_rtP -> P_179 ; rtb_B_311_52_0 = ( ( ( ( ( ( _rtB -> B_311_881_0 [ 1 ] -
_rtDW -> UnitDelay1_DSTATE_lq [ 1 ] ) * _rtP -> P_1267 + _rtDW ->
UnitDelay1_DSTATE_lq [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_fr [ 1 ] ) + ( _rtB
-> B_311_881_0 [ 1 ] - ( ( _rtB -> B_311_888_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_fr [ 1 ] ) * _rtP -> P_1270 + _rtDW -> UnitDelay2_DSTATE_fr
[ 1 ] ) ) ) * _rtP -> P_1271 * _rtP -> P_1272 + ( _rtP -> P_4224 . re *
B_311_401_0_idx_1 + _rtP -> P_4224 . im * rtb_B_311_68_0 ) ) + ( _rtP ->
P_4223 . re * rtb_B_311_52_0 + _rtP -> P_4223 . im * rtb_B_311_51_0 ) ) *
_rtP -> P_179 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB -> B_311_936_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_g4 [ 0 ] ) * _rtP -> P_1298 + _rtDW ->
UnitDelay1_DSTATE_g4 [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_aj [ 0 ] ) + ( _rtB
-> B_311_936_0 [ 0 ] - ( ( _rtB -> B_311_943_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_aj [ 0 ] ) * _rtP -> P_1301 + _rtDW -> UnitDelay2_DSTATE_aj
[ 0 ] ) ) ) * _rtP -> P_1302 * _rtP -> P_1303 + ( _rtP -> P_4222 . re *
rtb_B_311_36_0 - _rtP -> P_4222 . im * rtb_B_299_3_0 ) ) + ( _rtP -> P_4221 .
re * B_311_473_0_idx_0 - _rtP -> P_4221 . im * rtb_B_311_50_0 ) ) * _rtP ->
P_178 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB -> B_311_936_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_g4 [ 1 ] ) * _rtP -> P_1298 + _rtDW -> UnitDelay1_DSTATE_g4
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_aj [ 1 ] ) + ( _rtB -> B_311_936_0 [ 1 ]
- ( ( _rtB -> B_311_943_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_aj [ 1 ] ) *
_rtP -> P_1301 + _rtDW -> UnitDelay2_DSTATE_aj [ 1 ] ) ) ) * _rtP -> P_1302 *
_rtP -> P_1303 + ( _rtP -> P_4222 . re * rtb_B_299_3_0 + _rtP -> P_4222 . im
* rtb_B_311_36_0 ) ) + ( _rtP -> P_4221 . re * rtb_B_311_50_0 + _rtP ->
P_4221 . im * B_311_473_0_idx_0 ) ) * _rtP -> P_178 ) ; rtb_B_311_36_0 = (
rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP ->
P_180 ; rtb_B_311_52_0 = ( rtb_B_311_37_0 * rtb_B_299_3_0 + rtb_B_311_52_0 *
rtb_B_311_68_0 ) * _rtP -> P_180 ; break ; case 2 : rtb_B_311_37_0 = ( ( ( (
( ( _rtB -> B_311_881_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_lq [ 0 ] ) * _rtP
-> P_1267 + _rtDW -> UnitDelay1_DSTATE_lq [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_fr [ 0 ] ) + ( _rtB -> B_311_881_0 [ 0 ] - ( ( _rtB ->
B_311_888_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_fr [ 0 ] ) * _rtP -> P_1270 +
_rtDW -> UnitDelay2_DSTATE_fr [ 0 ] ) ) ) * _rtP -> P_1271 * _rtP -> P_1272 +
( _rtP -> P_4220 . re * rtb_B_311_68_0 - _rtP -> P_4220 . im *
B_311_401_0_idx_1 ) ) + ( _rtP -> P_4219 . re * rtb_B_311_51_0 - _rtP ->
P_4219 . im * rtb_B_311_52_0 ) ) * _rtP -> P_176 ; rtb_B_311_52_0 = ( ( ( ( (
( _rtB -> B_311_881_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_lq [ 1 ] ) * _rtP ->
P_1267 + _rtDW -> UnitDelay1_DSTATE_lq [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_fr [ 1 ] ) + ( _rtB -> B_311_881_0 [ 1 ] - ( ( _rtB ->
B_311_888_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_fr [ 1 ] ) * _rtP -> P_1270 +
_rtDW -> UnitDelay2_DSTATE_fr [ 1 ] ) ) ) * _rtP -> P_1271 * _rtP -> P_1272 +
( _rtP -> P_4220 . re * B_311_401_0_idx_1 + _rtP -> P_4220 . im *
rtb_B_311_68_0 ) ) + ( _rtP -> P_4219 . re * rtb_B_311_52_0 + _rtP -> P_4219
. im * rtb_B_311_51_0 ) ) * _rtP -> P_176 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB
-> B_311_936_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_g4 [ 0 ] ) * _rtP -> P_1298
+ _rtDW -> UnitDelay1_DSTATE_g4 [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_aj [ 0 ]
) + ( _rtB -> B_311_936_0 [ 0 ] - ( ( _rtB -> B_311_943_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_aj [ 0 ] ) * _rtP -> P_1301 + _rtDW -> UnitDelay2_DSTATE_aj
[ 0 ] ) ) ) * _rtP -> P_1302 * _rtP -> P_1303 + ( _rtP -> P_4218 . re *
rtb_B_311_36_0 - _rtP -> P_4218 . im * rtb_B_299_3_0 ) ) + ( _rtP -> P_4217 .
re * B_311_473_0_idx_0 - _rtP -> P_4217 . im * rtb_B_311_50_0 ) ) * _rtP ->
P_175 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB -> B_311_936_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_g4 [ 1 ] ) * _rtP -> P_1298 + _rtDW -> UnitDelay1_DSTATE_g4
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_aj [ 1 ] ) + ( _rtB -> B_311_936_0 [ 1 ]
- ( ( _rtB -> B_311_943_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_aj [ 1 ] ) *
_rtP -> P_1301 + _rtDW -> UnitDelay2_DSTATE_aj [ 1 ] ) ) ) * _rtP -> P_1302 *
_rtP -> P_1303 + ( _rtP -> P_4218 . re * rtb_B_299_3_0 + _rtP -> P_4218 . im
* rtb_B_311_36_0 ) ) + ( _rtP -> P_4217 . re * rtb_B_311_50_0 + _rtP ->
P_4217 . im * B_311_473_0_idx_0 ) ) * _rtP -> P_175 ) ; rtb_B_311_36_0 = (
rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP ->
P_177 ; rtb_B_311_52_0 = ( rtb_B_311_37_0 * rtb_B_299_3_0 + rtb_B_311_52_0 *
rtb_B_311_68_0 ) * _rtP -> P_177 ; break ; default : rtb_B_311_37_0 = ( ( ( (
( ( _rtB -> B_311_881_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_lq [ 0 ] ) * _rtP
-> P_1267 + _rtDW -> UnitDelay1_DSTATE_lq [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_fr [ 0 ] ) + ( _rtB -> B_311_881_0 [ 0 ] - ( ( _rtB ->
B_311_888_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_fr [ 0 ] ) * _rtP -> P_1270 +
_rtDW -> UnitDelay2_DSTATE_fr [ 0 ] ) ) ) * _rtP -> P_1271 * _rtP -> P_1272 +
rtb_B_311_68_0 ) + rtb_B_311_51_0 ) * _rtP -> P_173 ; rtb_B_311_52_0 = ( ( (
( ( ( _rtB -> B_311_881_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_lq [ 1 ] ) *
_rtP -> P_1267 + _rtDW -> UnitDelay1_DSTATE_lq [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_fr [ 1 ] ) + ( _rtB -> B_311_881_0 [ 1 ] - ( ( _rtB ->
B_311_888_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_fr [ 1 ] ) * _rtP -> P_1270 +
_rtDW -> UnitDelay2_DSTATE_fr [ 1 ] ) ) ) * _rtP -> P_1271 * _rtP -> P_1272 +
B_311_401_0_idx_1 ) + rtb_B_311_52_0 ) * _rtP -> P_173 ; rtb_B_311_68_0 = ( (
( ( ( ( _rtB -> B_311_936_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_g4 [ 0 ] ) *
_rtP -> P_1298 + _rtDW -> UnitDelay1_DSTATE_g4 [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_aj [ 0 ] ) + ( _rtB -> B_311_936_0 [ 0 ] - ( ( _rtB ->
B_311_943_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_aj [ 0 ] ) * _rtP -> P_1301 +
_rtDW -> UnitDelay2_DSTATE_aj [ 0 ] ) ) ) * _rtP -> P_1302 * _rtP -> P_1303 +
rtb_B_311_36_0 ) + B_311_473_0_idx_0 ) * _rtP -> P_172 ; rtb_B_299_3_0 = - (
( ( ( ( ( ( _rtB -> B_311_936_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_g4 [ 1 ] )
* _rtP -> P_1298 + _rtDW -> UnitDelay1_DSTATE_g4 [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_aj [ 1 ] ) + ( _rtB -> B_311_936_0 [ 1 ] - ( ( _rtB ->
B_311_943_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_aj [ 1 ] ) * _rtP -> P_1301 +
_rtDW -> UnitDelay2_DSTATE_aj [ 1 ] ) ) ) * _rtP -> P_1302 * _rtP -> P_1303 +
rtb_B_299_3_0 ) + rtb_B_311_50_0 ) * _rtP -> P_172 ) ; rtb_B_311_36_0 = (
rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP ->
P_174 ; rtb_B_311_52_0 = ( rtb_B_311_37_0 * rtb_B_299_3_0 + rtb_B_311_52_0 *
rtb_B_311_68_0 ) * _rtP -> P_174 ; break ; } _rtB -> B_311_991_0 [ 0 ] = _rtP
-> P_1322 * rtb_B_311_36_0 ; _rtB -> B_311_991_0 [ 1 ] = _rtP -> P_1322 *
rtb_B_311_52_0 ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_311_992_0 = _rtDW -> Memory1_PreviousInput ; } i = ssIsSampleHit ( S , 2 ,
0 ) ; if ( i != 0 ) { B_299_0_0_idx_0 = _rtP -> P_1323 * _rtB -> B_311_152_0
[ 78 ] * _rtP -> P_1326 ; B_299_0_0_idx_1 = _rtP -> P_1324 * _rtB ->
B_311_152_0 [ 79 ] * _rtP -> P_1326 ; B_299_0_0_idx_2 = _rtP -> P_1325 * _rtB
-> B_311_152_0 [ 80 ] * _rtP -> P_1326 ; _rtB -> B_311_999_0 = ( ( _rtDW ->
Integrator_DSTATE_k < _rtB -> B_311_250_0_c ) || ( _rtDW ->
Integrator_DSTATE_k >= _rtB -> B_311_251_0 ) ) ; if ( _rtDW ->
Initial_FirstOutputTime_cl ) { _rtDW -> Initial_FirstOutputTime_cl = false ;
_rtB -> B_311_1005_0 = _rtP -> P_1329 ; } else { _rtB -> B_311_1005_0 = _rtDW
-> Integrator_DSTATE_k - muDoubleScalarFloor ( ( _rtDW -> Integrator_DSTATE_k
- _rtB -> B_311_252_0 ) * _rtP -> P_1327 ) * _rtP -> P_1328 ; } if ( _rtB ->
B_311_999_0 && ( _rtDW -> Integrator_PrevResetState_go <= 0 ) ) { _rtDW ->
Integrator_DSTATE_k = _rtB -> B_311_1005_0 ; } muDoubleScalarSinCos ( _rtDW
-> Integrator_DSTATE_k , & rtb_B_311_35_0 , & rtb_B_311_50_0 ) ; _rtB ->
B_311_1008_0 [ 0 ] = B_299_0_0_idx_0 * rtb_B_311_35_0 ; _rtB -> B_311_1008_0
[ 1 ] = B_299_0_0_idx_0 * rtb_B_311_50_0 ; _rtB -> B_311_1025_0 [ 0 ] =
B_299_0_0_idx_1 * rtb_B_311_35_0 ; _rtB -> B_311_1025_0 [ 1 ] =
B_299_0_0_idx_1 * rtb_B_311_50_0 ; _rtB -> B_311_1042_0 [ 0 ] =
B_299_0_0_idx_2 * rtb_B_311_35_0 ; _rtB -> B_311_1042_0 [ 1 ] =
B_299_0_0_idx_2 * rtb_B_311_50_0 ; B_311_1062_0_idx_0 = _rtP -> P_1358 * _rtB
-> B_311_152_0 [ 161 ] * _rtP -> P_1361 ; B_311_1062_0_idx_1 = _rtP -> P_1359
* _rtB -> B_311_152_0 [ 162 ] * _rtP -> P_1361 ; B_311_1062_0_idx_2 = _rtP ->
P_1360 * _rtB -> B_311_152_0 [ 163 ] * _rtP -> P_1361 ; _rtB -> B_311_1063_0
[ 0 ] = B_311_1062_0_idx_0 * rtb_B_311_35_0 ; _rtB -> B_311_1063_0 [ 1 ] =
B_311_1062_0_idx_0 * rtb_B_311_50_0 ; _rtB -> B_311_1080_0 [ 0 ] =
B_311_1062_0_idx_1 * rtb_B_311_35_0 ; _rtB -> B_311_1080_0 [ 1 ] =
B_311_1062_0_idx_1 * rtb_B_311_50_0 ; _rtB -> B_311_1097_0 [ 0 ] =
B_311_1062_0_idx_2 * rtb_B_311_35_0 ; _rtB -> B_311_1097_0 [ 1 ] =
B_311_1062_0_idx_2 * rtb_B_311_50_0 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_po != 0 ) { _rtB -> B_311_1009_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_mm [ 0 ] ; } else { _rtB -> B_311_1009_0 [ 0 ] = _rtP ->
P_1331 * _rtB -> B_311_1008_0 [ 0 ] + _rtDW -> Integrator_DSTATE_mm [ 0 ] ; }
_rtB -> B_311_1016_0 [ 0 ] = _rtDW -> Delay_DSTATE_bi [ 0 ] ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_i != 0 ) { _rtB -> B_311_1026_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_ng [ 0 ] ; } else { _rtB -> B_311_1026_0 [ 0 ] = _rtP ->
P_1340 * _rtB -> B_311_1025_0 [ 0 ] + _rtDW -> Integrator_DSTATE_ng [ 0 ] ; }
_rtB -> B_311_1033_0 [ 0 ] = _rtDW -> Delay_DSTATE_ok [ 0 ] ; rtb_B_311_68_0
= ( ( ( ( _rtB -> B_311_1026_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_p2 [ 0 ] )
* _rtP -> P_1343 + _rtDW -> UnitDelay1_DSTATE_p2 [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_i2 [ 0 ] ) + ( _rtB -> B_311_1026_0 [ 0 ] - ( ( _rtB ->
B_311_1033_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_i2 [ 0 ] ) * _rtP -> P_1346 +
_rtDW -> UnitDelay2_DSTATE_i2 [ 0 ] ) ) ) * _rtP -> P_1347 * _rtP -> P_1348 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_lt != 0 ) { _rtB -> B_311_1043_0 [ 0 ]
= _rtDW -> Integrator_DSTATE_bw [ 0 ] ; } else { _rtB -> B_311_1043_0 [ 0 ] =
_rtP -> P_1349 * _rtB -> B_311_1042_0 [ 0 ] + _rtDW -> Integrator_DSTATE_bw [
0 ] ; } _rtB -> B_311_1050_0 [ 0 ] = _rtDW -> Delay_DSTATE_h [ 0 ] ;
rtb_B_311_51_0 = ( ( ( ( _rtB -> B_311_1043_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_ez [ 0 ] ) * _rtP -> P_1352 + _rtDW -> UnitDelay1_DSTATE_ez
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_c [ 0 ] ) + ( _rtB -> B_311_1043_0 [ 0 ]
- ( ( _rtB -> B_311_1050_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_c [ 0 ] ) *
_rtP -> P_1355 + _rtDW -> UnitDelay2_DSTATE_c [ 0 ] ) ) ) * _rtP -> P_1356 *
_rtP -> P_1357 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_a5 != 0 ) { _rtB ->
B_311_1064_0 [ 0 ] = _rtDW -> Integrator_DSTATE_ka [ 0 ] ; } else { _rtB ->
B_311_1064_0 [ 0 ] = _rtP -> P_1362 * _rtB -> B_311_1063_0 [ 0 ] + _rtDW ->
Integrator_DSTATE_ka [ 0 ] ; } _rtB -> B_311_1071_0 [ 0 ] = _rtDW ->
Delay_DSTATE_cv [ 0 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_ix != 0 ) {
_rtB -> B_311_1081_0 [ 0 ] = _rtDW -> Integrator_DSTATE_ow [ 0 ] ; } else {
_rtB -> B_311_1081_0 [ 0 ] = _rtP -> P_1371 * _rtB -> B_311_1080_0 [ 0 ] +
_rtDW -> Integrator_DSTATE_ow [ 0 ] ; } _rtB -> B_311_1088_0 [ 0 ] = _rtDW ->
Delay_DSTATE_cu [ 0 ] ; rtb_B_311_36_0 = ( ( ( ( _rtB -> B_311_1081_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_o5 [ 0 ] ) * _rtP -> P_1374 + _rtDW ->
UnitDelay1_DSTATE_o5 [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_hc [ 0 ] ) + ( _rtB
-> B_311_1081_0 [ 0 ] - ( ( _rtB -> B_311_1088_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_hc [ 0 ] ) * _rtP -> P_1377 + _rtDW -> UnitDelay2_DSTATE_hc
[ 0 ] ) ) ) * _rtP -> P_1378 * _rtP -> P_1379 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_dz != 0 ) { _rtB -> B_311_1098_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_c1 [ 0 ] ; } else { _rtB -> B_311_1098_0 [ 0 ] = _rtP ->
P_1380 * _rtB -> B_311_1097_0 [ 0 ] + _rtDW -> Integrator_DSTATE_c1 [ 0 ] ; }
_rtB -> B_311_1105_0 [ 0 ] = _rtDW -> Delay_DSTATE_g [ 0 ] ;
B_311_473_0_idx_0 = ( ( ( ( _rtB -> B_311_1098_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_h2 [ 0 ] ) * _rtP -> P_1383 + _rtDW -> UnitDelay1_DSTATE_h2
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_pw [ 0 ] ) + ( _rtB -> B_311_1098_0 [ 0
] - ( ( _rtB -> B_311_1105_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_pw [ 0 ] ) *
_rtP -> P_1386 + _rtDW -> UnitDelay2_DSTATE_pw [ 0 ] ) ) ) * _rtP -> P_1387 *
_rtP -> P_1388 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_po != 0 ) { _rtB ->
B_311_1009_0 [ 1 ] = _rtDW -> Integrator_DSTATE_mm [ 1 ] ; } else { _rtB ->
B_311_1009_0 [ 1 ] = _rtP -> P_1331 * _rtB -> B_311_1008_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_mm [ 1 ] ; } _rtB -> B_311_1016_0 [ 1 ] = _rtDW ->
Delay_DSTATE_bi [ 1 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_i != 0 ) {
_rtB -> B_311_1026_0 [ 1 ] = _rtDW -> Integrator_DSTATE_ng [ 1 ] ; } else {
_rtB -> B_311_1026_0 [ 1 ] = _rtP -> P_1340 * _rtB -> B_311_1025_0 [ 1 ] +
_rtDW -> Integrator_DSTATE_ng [ 1 ] ; } _rtB -> B_311_1033_0 [ 1 ] = _rtDW ->
Delay_DSTATE_ok [ 1 ] ; B_311_401_0_idx_1 = ( ( ( ( _rtB -> B_311_1026_0 [ 1
] - _rtDW -> UnitDelay1_DSTATE_p2 [ 1 ] ) * _rtP -> P_1343 + _rtDW ->
UnitDelay1_DSTATE_p2 [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_i2 [ 1 ] ) + ( _rtB
-> B_311_1026_0 [ 1 ] - ( ( _rtB -> B_311_1033_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_i2 [ 1 ] ) * _rtP -> P_1346 + _rtDW -> UnitDelay2_DSTATE_i2
[ 1 ] ) ) ) * _rtP -> P_1347 * _rtP -> P_1348 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_lt != 0 ) { _rtB -> B_311_1043_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_bw [ 1 ] ; } else { _rtB -> B_311_1043_0 [ 1 ] = _rtP ->
P_1349 * _rtB -> B_311_1042_0 [ 1 ] + _rtDW -> Integrator_DSTATE_bw [ 1 ] ; }
_rtB -> B_311_1050_0 [ 1 ] = _rtDW -> Delay_DSTATE_h [ 1 ] ; rtb_B_311_52_0 =
( ( ( ( _rtB -> B_311_1043_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_ez [ 1 ] ) *
_rtP -> P_1352 + _rtDW -> UnitDelay1_DSTATE_ez [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_c [ 1 ] ) + ( _rtB -> B_311_1043_0 [ 1 ] - ( ( _rtB ->
B_311_1050_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_c [ 1 ] ) * _rtP -> P_1355 +
_rtDW -> UnitDelay2_DSTATE_c [ 1 ] ) ) ) * _rtP -> P_1356 * _rtP -> P_1357 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_a5 != 0 ) { _rtB -> B_311_1064_0 [ 1 ]
= _rtDW -> Integrator_DSTATE_ka [ 1 ] ; } else { _rtB -> B_311_1064_0 [ 1 ] =
_rtP -> P_1362 * _rtB -> B_311_1063_0 [ 1 ] + _rtDW -> Integrator_DSTATE_ka [
1 ] ; } _rtB -> B_311_1071_0 [ 1 ] = _rtDW -> Delay_DSTATE_cv [ 1 ] ; if (
_rtDW -> Integrator_SYSTEM_ENABLE_ix != 0 ) { _rtB -> B_311_1081_0 [ 1 ] =
_rtDW -> Integrator_DSTATE_ow [ 1 ] ; } else { _rtB -> B_311_1081_0 [ 1 ] =
_rtP -> P_1371 * _rtB -> B_311_1080_0 [ 1 ] + _rtDW -> Integrator_DSTATE_ow [
1 ] ; } _rtB -> B_311_1088_0 [ 1 ] = _rtDW -> Delay_DSTATE_cu [ 1 ] ;
rtb_B_299_3_0 = ( ( ( ( _rtB -> B_311_1081_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_o5 [ 1 ] ) * _rtP -> P_1374 + _rtDW -> UnitDelay1_DSTATE_o5
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_hc [ 1 ] ) + ( _rtB -> B_311_1081_0 [ 1
] - ( ( _rtB -> B_311_1088_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_hc [ 1 ] ) *
_rtP -> P_1377 + _rtDW -> UnitDelay2_DSTATE_hc [ 1 ] ) ) ) * _rtP -> P_1378 *
_rtP -> P_1379 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_dz != 0 ) { _rtB ->
B_311_1098_0 [ 1 ] = _rtDW -> Integrator_DSTATE_c1 [ 1 ] ; } else { _rtB ->
B_311_1098_0 [ 1 ] = _rtP -> P_1380 * _rtB -> B_311_1097_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_c1 [ 1 ] ; } _rtB -> B_311_1105_0 [ 1 ] = _rtDW ->
Delay_DSTATE_g [ 1 ] ; rtb_B_311_50_0 = ( ( ( ( _rtB -> B_311_1098_0 [ 1 ] -
_rtDW -> UnitDelay1_DSTATE_h2 [ 1 ] ) * _rtP -> P_1383 + _rtDW ->
UnitDelay1_DSTATE_h2 [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_pw [ 1 ] ) + ( _rtB
-> B_311_1098_0 [ 1 ] - ( ( _rtB -> B_311_1105_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_pw [ 1 ] ) * _rtP -> P_1386 + _rtDW -> UnitDelay2_DSTATE_pw
[ 1 ] ) ) ) * _rtP -> P_1387 * _rtP -> P_1388 ; switch ( ( int32_T ) _rtB ->
B_311_249_0_h ) { case 1 : rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_1009_0
[ 0 ] - _rtDW -> UnitDelay1_DSTATE_cs [ 0 ] ) * _rtP -> P_1334 + _rtDW ->
UnitDelay1_DSTATE_cs [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_at [ 0 ] ) + ( _rtB
-> B_311_1009_0 [ 0 ] - ( ( _rtB -> B_311_1016_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_at [ 0 ] ) * _rtP -> P_1337 + _rtDW -> UnitDelay2_DSTATE_at
[ 0 ] ) ) ) * _rtP -> P_1338 * _rtP -> P_1339 + ( _rtP -> P_4216 . re *
rtb_B_311_68_0 - _rtP -> P_4216 . im * B_311_401_0_idx_1 ) ) + ( _rtP ->
P_4215 . re * rtb_B_311_51_0 - _rtP -> P_4215 . im * rtb_B_311_52_0 ) ) *
_rtP -> P_170 ; rtb_B_311_52_0 = ( ( ( ( ( ( _rtB -> B_311_1009_0 [ 1 ] -
_rtDW -> UnitDelay1_DSTATE_cs [ 1 ] ) * _rtP -> P_1334 + _rtDW ->
UnitDelay1_DSTATE_cs [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_at [ 1 ] ) + ( _rtB
-> B_311_1009_0 [ 1 ] - ( ( _rtB -> B_311_1016_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_at [ 1 ] ) * _rtP -> P_1337 + _rtDW -> UnitDelay2_DSTATE_at
[ 1 ] ) ) ) * _rtP -> P_1338 * _rtP -> P_1339 + ( _rtP -> P_4216 . re *
B_311_401_0_idx_1 + _rtP -> P_4216 . im * rtb_B_311_68_0 ) ) + ( _rtP ->
P_4215 . re * rtb_B_311_52_0 + _rtP -> P_4215 . im * rtb_B_311_51_0 ) ) *
_rtP -> P_170 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB -> B_311_1064_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_ms [ 0 ] ) * _rtP -> P_1365 + _rtDW ->
UnitDelay1_DSTATE_ms [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_j [ 0 ] ) + ( _rtB
-> B_311_1064_0 [ 0 ] - ( ( _rtB -> B_311_1071_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_j [ 0 ] ) * _rtP -> P_1368 + _rtDW -> UnitDelay2_DSTATE_j [
0 ] ) ) ) * _rtP -> P_1369 * _rtP -> P_1370 + ( _rtP -> P_4214 . re *
rtb_B_311_36_0 - _rtP -> P_4214 . im * rtb_B_299_3_0 ) ) + ( _rtP -> P_4213 .
re * B_311_473_0_idx_0 - _rtP -> P_4213 . im * rtb_B_311_50_0 ) ) * _rtP ->
P_169 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB -> B_311_1064_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_ms [ 1 ] ) * _rtP -> P_1365 + _rtDW -> UnitDelay1_DSTATE_ms
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_j [ 1 ] ) + ( _rtB -> B_311_1064_0 [ 1 ]
- ( ( _rtB -> B_311_1071_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_j [ 1 ] ) *
_rtP -> P_1368 + _rtDW -> UnitDelay2_DSTATE_j [ 1 ] ) ) ) * _rtP -> P_1369 *
_rtP -> P_1370 + ( _rtP -> P_4214 . re * rtb_B_299_3_0 + _rtP -> P_4214 . im
* rtb_B_311_36_0 ) ) + ( _rtP -> P_4213 . re * rtb_B_311_50_0 + _rtP ->
P_4213 . im * B_311_473_0_idx_0 ) ) * _rtP -> P_169 ) ; _rtB -> B_311_1118_0
= ( rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP
-> P_171 ; _rtB -> B_311_1118_1 = ( rtb_B_311_37_0 * rtb_B_299_3_0 +
rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP -> P_171 ; break ; case 2 :
rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_1009_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_cs [ 0 ] ) * _rtP -> P_1334 + _rtDW -> UnitDelay1_DSTATE_cs
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_at [ 0 ] ) + ( _rtB -> B_311_1009_0 [ 0
] - ( ( _rtB -> B_311_1016_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_at [ 0 ] ) *
_rtP -> P_1337 + _rtDW -> UnitDelay2_DSTATE_at [ 0 ] ) ) ) * _rtP -> P_1338 *
_rtP -> P_1339 + ( _rtP -> P_4212 . re * rtb_B_311_68_0 - _rtP -> P_4212 . im
* B_311_401_0_idx_1 ) ) + ( _rtP -> P_4211 . re * rtb_B_311_51_0 - _rtP ->
P_4211 . im * rtb_B_311_52_0 ) ) * _rtP -> P_167 ; rtb_B_311_52_0 = ( ( ( ( (
( _rtB -> B_311_1009_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_cs [ 1 ] ) * _rtP
-> P_1334 + _rtDW -> UnitDelay1_DSTATE_cs [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_at [ 1 ] ) + ( _rtB -> B_311_1009_0 [ 1 ] - ( ( _rtB ->
B_311_1016_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_at [ 1 ] ) * _rtP -> P_1337 +
_rtDW -> UnitDelay2_DSTATE_at [ 1 ] ) ) ) * _rtP -> P_1338 * _rtP -> P_1339 +
( _rtP -> P_4212 . re * B_311_401_0_idx_1 + _rtP -> P_4212 . im *
rtb_B_311_68_0 ) ) + ( _rtP -> P_4211 . re * rtb_B_311_52_0 + _rtP -> P_4211
. im * rtb_B_311_51_0 ) ) * _rtP -> P_167 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB
-> B_311_1064_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_ms [ 0 ] ) * _rtP ->
P_1365 + _rtDW -> UnitDelay1_DSTATE_ms [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_j
[ 0 ] ) + ( _rtB -> B_311_1064_0 [ 0 ] - ( ( _rtB -> B_311_1071_0 [ 0 ] -
_rtDW -> UnitDelay2_DSTATE_j [ 0 ] ) * _rtP -> P_1368 + _rtDW ->
UnitDelay2_DSTATE_j [ 0 ] ) ) ) * _rtP -> P_1369 * _rtP -> P_1370 + ( _rtP ->
P_4210 . re * rtb_B_311_36_0 - _rtP -> P_4210 . im * rtb_B_299_3_0 ) ) + (
_rtP -> P_4209 . re * B_311_473_0_idx_0 - _rtP -> P_4209 . im *
rtb_B_311_50_0 ) ) * _rtP -> P_166 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB ->
B_311_1064_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_ms [ 1 ] ) * _rtP -> P_1365 +
_rtDW -> UnitDelay1_DSTATE_ms [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_j [ 1 ] )
+ ( _rtB -> B_311_1064_0 [ 1 ] - ( ( _rtB -> B_311_1071_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_j [ 1 ] ) * _rtP -> P_1368 + _rtDW -> UnitDelay2_DSTATE_j [
1 ] ) ) ) * _rtP -> P_1369 * _rtP -> P_1370 + ( _rtP -> P_4210 . re *
rtb_B_299_3_0 + _rtP -> P_4210 . im * rtb_B_311_36_0 ) ) + ( _rtP -> P_4209 .
re * rtb_B_311_50_0 + _rtP -> P_4209 . im * B_311_473_0_idx_0 ) ) * _rtP ->
P_166 ) ; _rtB -> B_311_1118_0 = ( rtb_B_311_37_0 * rtb_B_311_68_0 -
rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP -> P_168 ; _rtB -> B_311_1118_1 = (
rtb_B_311_37_0 * rtb_B_299_3_0 + rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP ->
P_168 ; break ; default : rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_1009_0 [
0 ] - _rtDW -> UnitDelay1_DSTATE_cs [ 0 ] ) * _rtP -> P_1334 + _rtDW ->
UnitDelay1_DSTATE_cs [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_at [ 0 ] ) + ( _rtB
-> B_311_1009_0 [ 0 ] - ( ( _rtB -> B_311_1016_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_at [ 0 ] ) * _rtP -> P_1337 + _rtDW -> UnitDelay2_DSTATE_at
[ 0 ] ) ) ) * _rtP -> P_1338 * _rtP -> P_1339 + rtb_B_311_68_0 ) +
rtb_B_311_51_0 ) * _rtP -> P_164 ; rtb_B_311_52_0 = ( ( ( ( ( ( _rtB ->
B_311_1009_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_cs [ 1 ] ) * _rtP -> P_1334 +
_rtDW -> UnitDelay1_DSTATE_cs [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_at [ 1 ] )
+ ( _rtB -> B_311_1009_0 [ 1 ] - ( ( _rtB -> B_311_1016_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_at [ 1 ] ) * _rtP -> P_1337 + _rtDW -> UnitDelay2_DSTATE_at
[ 1 ] ) ) ) * _rtP -> P_1338 * _rtP -> P_1339 + B_311_401_0_idx_1 ) +
rtb_B_311_52_0 ) * _rtP -> P_164 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB ->
B_311_1064_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_ms [ 0 ] ) * _rtP -> P_1365 +
_rtDW -> UnitDelay1_DSTATE_ms [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_j [ 0 ] )
+ ( _rtB -> B_311_1064_0 [ 0 ] - ( ( _rtB -> B_311_1071_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_j [ 0 ] ) * _rtP -> P_1368 + _rtDW -> UnitDelay2_DSTATE_j [
0 ] ) ) ) * _rtP -> P_1369 * _rtP -> P_1370 + rtb_B_311_36_0 ) +
B_311_473_0_idx_0 ) * _rtP -> P_163 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB ->
B_311_1064_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_ms [ 1 ] ) * _rtP -> P_1365 +
_rtDW -> UnitDelay1_DSTATE_ms [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_j [ 1 ] )
+ ( _rtB -> B_311_1064_0 [ 1 ] - ( ( _rtB -> B_311_1071_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_j [ 1 ] ) * _rtP -> P_1368 + _rtDW -> UnitDelay2_DSTATE_j [
1 ] ) ) ) * _rtP -> P_1369 * _rtP -> P_1370 + rtb_B_299_3_0 ) +
rtb_B_311_50_0 ) * _rtP -> P_163 ) ; _rtB -> B_311_1118_0 = ( rtb_B_311_37_0
* rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP -> P_165 ; _rtB ->
B_311_1118_1 = ( rtb_B_311_37_0 * rtb_B_299_3_0 + rtb_B_311_52_0 *
rtb_B_311_68_0 ) * _rtP -> P_165 ; break ; } } i = ssIsSampleHit ( S , 1 , 0
) ; if ( i != 0 ) { _rtB -> B_311_1119_0 = _rtDW -> Memory2_PreviousInput_m ;
} i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { B_311_1062_0_idx_0 = _rtP
-> P_1389 * _rtB -> B_311_152_0 [ 69 ] * _rtP -> P_1392 ; B_311_1062_0_idx_1
= _rtP -> P_1390 * _rtB -> B_311_152_0 [ 70 ] * _rtP -> P_1392 ;
B_311_1062_0_idx_2 = _rtP -> P_1391 * _rtB -> B_311_152_0 [ 71 ] * _rtP ->
P_1392 ; _rtB -> B_311_1126_0 = ( ( _rtDW -> Integrator_DSTATE_eh < _rtB ->
B_311_260_0 ) || ( _rtDW -> Integrator_DSTATE_eh >= _rtB -> B_311_261_0_a ) )
; if ( _rtDW -> Initial_FirstOutputTime_ki ) { _rtDW ->
Initial_FirstOutputTime_ki = false ; _rtB -> B_311_1132_0 = _rtP -> P_1395 ;
} else { _rtB -> B_311_1132_0 = _rtDW -> Integrator_DSTATE_eh -
muDoubleScalarFloor ( ( _rtDW -> Integrator_DSTATE_eh - _rtB -> B_311_262_0 )
* _rtP -> P_1393 ) * _rtP -> P_1394 ; } if ( _rtB -> B_311_1126_0 && ( _rtDW
-> Integrator_PrevResetState_h <= 0 ) ) { _rtDW -> Integrator_DSTATE_eh =
_rtB -> B_311_1132_0 ; } muDoubleScalarSinCos ( _rtDW -> Integrator_DSTATE_eh
, & rtb_B_311_35_0 , & rtb_B_311_50_0 ) ; _rtB -> B_311_1135_0 [ 0 ] =
B_311_1062_0_idx_0 * rtb_B_311_35_0 ; _rtB -> B_311_1135_0 [ 1 ] =
B_311_1062_0_idx_0 * rtb_B_311_50_0 ; _rtB -> B_311_1152_0 [ 0 ] =
B_311_1062_0_idx_1 * rtb_B_311_35_0 ; _rtB -> B_311_1152_0 [ 1 ] =
B_311_1062_0_idx_1 * rtb_B_311_50_0 ; _rtB -> B_311_1169_0 [ 0 ] =
B_311_1062_0_idx_2 * rtb_B_311_35_0 ; _rtB -> B_311_1169_0 [ 1 ] =
B_311_1062_0_idx_2 * rtb_B_311_50_0 ; B_311_1062_0_idx_0 = _rtP -> P_1424 *
_rtB -> B_311_152_0 [ 152 ] * _rtP -> P_1427 ; B_311_1062_0_idx_1 = _rtP ->
P_1425 * _rtB -> B_311_152_0 [ 153 ] * _rtP -> P_1427 ; B_311_1062_0_idx_2 =
_rtP -> P_1426 * _rtB -> B_311_152_0 [ 154 ] * _rtP -> P_1427 ; _rtB ->
B_311_1190_0 [ 0 ] = B_311_1062_0_idx_0 * rtb_B_311_35_0 ; _rtB ->
B_311_1190_0 [ 1 ] = B_311_1062_0_idx_0 * rtb_B_311_50_0 ; _rtB ->
B_311_1207_0 [ 0 ] = B_311_1062_0_idx_1 * rtb_B_311_35_0 ; _rtB ->
B_311_1207_0 [ 1 ] = B_311_1062_0_idx_1 * rtb_B_311_50_0 ; _rtB ->
B_311_1224_0 [ 0 ] = B_311_1062_0_idx_2 * rtb_B_311_35_0 ; _rtB ->
B_311_1224_0 [ 1 ] = B_311_1062_0_idx_2 * rtb_B_311_50_0 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_ln != 0 ) { _rtB -> B_311_1136_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_om [ 0 ] ; } else { _rtB -> B_311_1136_0 [ 0 ] = _rtP ->
P_1397 * _rtB -> B_311_1135_0 [ 0 ] + _rtDW -> Integrator_DSTATE_om [ 0 ] ; }
_rtB -> B_311_1143_0 [ 0 ] = _rtDW -> Delay_DSTATE_h0 [ 0 ] ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_omg != 0 ) { _rtB -> B_311_1153_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_bv [ 0 ] ; } else { _rtB -> B_311_1153_0 [ 0 ] = _rtP ->
P_1406 * _rtB -> B_311_1152_0 [ 0 ] + _rtDW -> Integrator_DSTATE_bv [ 0 ] ; }
_rtB -> B_311_1160_0 [ 0 ] = _rtDW -> Delay_DSTATE_mo [ 0 ] ; rtb_B_311_68_0
= ( ( ( ( _rtB -> B_311_1153_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_e5 [ 0 ] )
* _rtP -> P_1409 + _rtDW -> UnitDelay1_DSTATE_e5 [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_gy [ 0 ] ) + ( _rtB -> B_311_1153_0 [ 0 ] - ( ( _rtB ->
B_311_1160_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_gy [ 0 ] ) * _rtP -> P_1412 +
_rtDW -> UnitDelay2_DSTATE_gy [ 0 ] ) ) ) * _rtP -> P_1413 * _rtP -> P_1414 ;
if ( _rtDW -> Integrator_SYSTEM_ENABLE_nl != 0 ) { _rtB -> B_311_1170_0 [ 0 ]
= _rtDW -> Integrator_DSTATE_d5g [ 0 ] ; } else { _rtB -> B_311_1170_0 [ 0 ]
= _rtP -> P_1415 * _rtB -> B_311_1169_0 [ 0 ] + _rtDW ->
Integrator_DSTATE_d5g [ 0 ] ; } _rtB -> B_311_1177_0 [ 0 ] = _rtDW ->
Delay_DSTATE_ah [ 0 ] ; rtb_B_311_51_0 = ( ( ( ( _rtB -> B_311_1170_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_h2i [ 0 ] ) * _rtP -> P_1418 + _rtDW ->
UnitDelay1_DSTATE_h2i [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_k5t [ 0 ] ) + (
_rtB -> B_311_1170_0 [ 0 ] - ( ( _rtB -> B_311_1177_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_k5t [ 0 ] ) * _rtP -> P_1421 + _rtDW ->
UnitDelay2_DSTATE_k5t [ 0 ] ) ) ) * _rtP -> P_1422 * _rtP -> P_1423 ; if (
_rtDW -> Integrator_SYSTEM_ENABLE_jz != 0 ) { _rtB -> B_311_1191_0 [ 0 ] =
_rtDW -> Integrator_DSTATE_hw [ 0 ] ; } else { _rtB -> B_311_1191_0 [ 0 ] =
_rtP -> P_1428 * _rtB -> B_311_1190_0 [ 0 ] + _rtDW -> Integrator_DSTATE_hw [
0 ] ; } _rtB -> B_311_1198_0 [ 0 ] = _rtDW -> Delay_DSTATE_pi [ 0 ] ; if (
_rtDW -> Integrator_SYSTEM_ENABLE_hk != 0 ) { _rtB -> B_311_1208_0 [ 0 ] =
_rtDW -> Integrator_DSTATE_ax [ 0 ] ; } else { _rtB -> B_311_1208_0 [ 0 ] =
_rtP -> P_1437 * _rtB -> B_311_1207_0 [ 0 ] + _rtDW -> Integrator_DSTATE_ax [
0 ] ; } _rtB -> B_311_1215_0 [ 0 ] = _rtDW -> Delay_DSTATE_hg [ 0 ] ;
rtb_B_311_36_0 = ( ( ( ( _rtB -> B_311_1208_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_nf [ 0 ] ) * _rtP -> P_1440 + _rtDW -> UnitDelay1_DSTATE_nf
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_jd [ 0 ] ) + ( _rtB -> B_311_1208_0 [ 0
] - ( ( _rtB -> B_311_1215_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_jd [ 0 ] ) *
_rtP -> P_1443 + _rtDW -> UnitDelay2_DSTATE_jd [ 0 ] ) ) ) * _rtP -> P_1444 *
_rtP -> P_1445 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_kk != 0 ) { _rtB ->
B_311_1225_0 [ 0 ] = _rtDW -> Integrator_DSTATE_il [ 0 ] ; } else { _rtB ->
B_311_1225_0 [ 0 ] = _rtP -> P_1446 * _rtB -> B_311_1224_0 [ 0 ] + _rtDW ->
Integrator_DSTATE_il [ 0 ] ; } _rtB -> B_311_1232_0 [ 0 ] = _rtDW ->
Delay_DSTATE_f0p [ 0 ] ; B_311_473_0_idx_0 = ( ( ( ( _rtB -> B_311_1225_0 [ 0
] - _rtDW -> UnitDelay1_DSTATE_hx [ 0 ] ) * _rtP -> P_1449 + _rtDW ->
UnitDelay1_DSTATE_hx [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_hq [ 0 ] ) + ( _rtB
-> B_311_1225_0 [ 0 ] - ( ( _rtB -> B_311_1232_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_hq [ 0 ] ) * _rtP -> P_1452 + _rtDW -> UnitDelay2_DSTATE_hq
[ 0 ] ) ) ) * _rtP -> P_1453 * _rtP -> P_1454 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_ln != 0 ) { _rtB -> B_311_1136_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_om [ 1 ] ; } else { _rtB -> B_311_1136_0 [ 1 ] = _rtP ->
P_1397 * _rtB -> B_311_1135_0 [ 1 ] + _rtDW -> Integrator_DSTATE_om [ 1 ] ; }
_rtB -> B_311_1143_0 [ 1 ] = _rtDW -> Delay_DSTATE_h0 [ 1 ] ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_omg != 0 ) { _rtB -> B_311_1153_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_bv [ 1 ] ; } else { _rtB -> B_311_1153_0 [ 1 ] = _rtP ->
P_1406 * _rtB -> B_311_1152_0 [ 1 ] + _rtDW -> Integrator_DSTATE_bv [ 1 ] ; }
_rtB -> B_311_1160_0 [ 1 ] = _rtDW -> Delay_DSTATE_mo [ 1 ] ;
B_311_401_0_idx_1 = ( ( ( ( _rtB -> B_311_1153_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_e5 [ 1 ] ) * _rtP -> P_1409 + _rtDW -> UnitDelay1_DSTATE_e5
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_gy [ 1 ] ) + ( _rtB -> B_311_1153_0 [ 1
] - ( ( _rtB -> B_311_1160_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_gy [ 1 ] ) *
_rtP -> P_1412 + _rtDW -> UnitDelay2_DSTATE_gy [ 1 ] ) ) ) * _rtP -> P_1413 *
_rtP -> P_1414 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_nl != 0 ) { _rtB ->
B_311_1170_0 [ 1 ] = _rtDW -> Integrator_DSTATE_d5g [ 1 ] ; } else { _rtB ->
B_311_1170_0 [ 1 ] = _rtP -> P_1415 * _rtB -> B_311_1169_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_d5g [ 1 ] ; } _rtB -> B_311_1177_0 [ 1 ] = _rtDW ->
Delay_DSTATE_ah [ 1 ] ; rtb_B_311_52_0 = ( ( ( ( _rtB -> B_311_1170_0 [ 1 ] -
_rtDW -> UnitDelay1_DSTATE_h2i [ 1 ] ) * _rtP -> P_1418 + _rtDW ->
UnitDelay1_DSTATE_h2i [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_k5t [ 1 ] ) + (
_rtB -> B_311_1170_0 [ 1 ] - ( ( _rtB -> B_311_1177_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_k5t [ 1 ] ) * _rtP -> P_1421 + _rtDW ->
UnitDelay2_DSTATE_k5t [ 1 ] ) ) ) * _rtP -> P_1422 * _rtP -> P_1423 ; if (
_rtDW -> Integrator_SYSTEM_ENABLE_jz != 0 ) { _rtB -> B_311_1191_0 [ 1 ] =
_rtDW -> Integrator_DSTATE_hw [ 1 ] ; } else { _rtB -> B_311_1191_0 [ 1 ] =
_rtP -> P_1428 * _rtB -> B_311_1190_0 [ 1 ] + _rtDW -> Integrator_DSTATE_hw [
1 ] ; } _rtB -> B_311_1198_0 [ 1 ] = _rtDW -> Delay_DSTATE_pi [ 1 ] ; if (
_rtDW -> Integrator_SYSTEM_ENABLE_hk != 0 ) { _rtB -> B_311_1208_0 [ 1 ] =
_rtDW -> Integrator_DSTATE_ax [ 1 ] ; } else { _rtB -> B_311_1208_0 [ 1 ] =
_rtP -> P_1437 * _rtB -> B_311_1207_0 [ 1 ] + _rtDW -> Integrator_DSTATE_ax [
1 ] ; } _rtB -> B_311_1215_0 [ 1 ] = _rtDW -> Delay_DSTATE_hg [ 1 ] ;
rtb_B_299_3_0 = ( ( ( ( _rtB -> B_311_1208_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_nf [ 1 ] ) * _rtP -> P_1440 + _rtDW -> UnitDelay1_DSTATE_nf
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_jd [ 1 ] ) + ( _rtB -> B_311_1208_0 [ 1
] - ( ( _rtB -> B_311_1215_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_jd [ 1 ] ) *
_rtP -> P_1443 + _rtDW -> UnitDelay2_DSTATE_jd [ 1 ] ) ) ) * _rtP -> P_1444 *
_rtP -> P_1445 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_kk != 0 ) { _rtB ->
B_311_1225_0 [ 1 ] = _rtDW -> Integrator_DSTATE_il [ 1 ] ; } else { _rtB ->
B_311_1225_0 [ 1 ] = _rtP -> P_1446 * _rtB -> B_311_1224_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_il [ 1 ] ; } _rtB -> B_311_1232_0 [ 1 ] = _rtDW ->
Delay_DSTATE_f0p [ 1 ] ; rtb_B_311_50_0 = ( ( ( ( _rtB -> B_311_1225_0 [ 1 ]
- _rtDW -> UnitDelay1_DSTATE_hx [ 1 ] ) * _rtP -> P_1449 + _rtDW ->
UnitDelay1_DSTATE_hx [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_hq [ 1 ] ) + ( _rtB
-> B_311_1225_0 [ 1 ] - ( ( _rtB -> B_311_1232_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_hq [ 1 ] ) * _rtP -> P_1452 + _rtDW -> UnitDelay2_DSTATE_hq
[ 1 ] ) ) ) * _rtP -> P_1453 * _rtP -> P_1454 ; switch ( ( int32_T ) _rtB ->
B_311_259_0 ) { case 1 : rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_1136_0 [
0 ] - _rtDW -> UnitDelay1_DSTATE_go [ 0 ] ) * _rtP -> P_1400 + _rtDW ->
UnitDelay1_DSTATE_go [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_k5 [ 0 ] ) + ( _rtB
-> B_311_1136_0 [ 0 ] - ( ( _rtB -> B_311_1143_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_k5 [ 0 ] ) * _rtP -> P_1403 + _rtDW -> UnitDelay2_DSTATE_k5
[ 0 ] ) ) ) * _rtP -> P_1404 * _rtP -> P_1405 + ( _rtP -> P_4232 . re *
rtb_B_311_68_0 - _rtP -> P_4232 . im * B_311_401_0_idx_1 ) ) + ( _rtP ->
P_4231 . re * rtb_B_311_51_0 - _rtP -> P_4231 . im * rtb_B_311_52_0 ) ) *
_rtP -> P_188 ; rtb_B_311_52_0 = ( ( ( ( ( ( _rtB -> B_311_1136_0 [ 1 ] -
_rtDW -> UnitDelay1_DSTATE_go [ 1 ] ) * _rtP -> P_1400 + _rtDW ->
UnitDelay1_DSTATE_go [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_k5 [ 1 ] ) + ( _rtB
-> B_311_1136_0 [ 1 ] - ( ( _rtB -> B_311_1143_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_k5 [ 1 ] ) * _rtP -> P_1403 + _rtDW -> UnitDelay2_DSTATE_k5
[ 1 ] ) ) ) * _rtP -> P_1404 * _rtP -> P_1405 + ( _rtP -> P_4232 . re *
B_311_401_0_idx_1 + _rtP -> P_4232 . im * rtb_B_311_68_0 ) ) + ( _rtP ->
P_4231 . re * rtb_B_311_52_0 + _rtP -> P_4231 . im * rtb_B_311_51_0 ) ) *
_rtP -> P_188 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB -> B_311_1191_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_en [ 0 ] ) * _rtP -> P_1431 + _rtDW ->
UnitDelay1_DSTATE_en [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_nj [ 0 ] ) + ( _rtB
-> B_311_1191_0 [ 0 ] - ( ( _rtB -> B_311_1198_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_nj [ 0 ] ) * _rtP -> P_1434 + _rtDW -> UnitDelay2_DSTATE_nj
[ 0 ] ) ) ) * _rtP -> P_1435 * _rtP -> P_1436 + ( _rtP -> P_4230 . re *
rtb_B_311_36_0 - _rtP -> P_4230 . im * rtb_B_299_3_0 ) ) + ( _rtP -> P_4229 .
re * B_311_473_0_idx_0 - _rtP -> P_4229 . im * rtb_B_311_50_0 ) ) * _rtP ->
P_187 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB -> B_311_1191_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_en [ 1 ] ) * _rtP -> P_1431 + _rtDW -> UnitDelay1_DSTATE_en
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_nj [ 1 ] ) + ( _rtB -> B_311_1191_0 [ 1
] - ( ( _rtB -> B_311_1198_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_nj [ 1 ] ) *
_rtP -> P_1434 + _rtDW -> UnitDelay2_DSTATE_nj [ 1 ] ) ) ) * _rtP -> P_1435 *
_rtP -> P_1436 + ( _rtP -> P_4230 . re * rtb_B_299_3_0 + _rtP -> P_4230 . im
* rtb_B_311_36_0 ) ) + ( _rtP -> P_4229 . re * rtb_B_311_50_0 + _rtP ->
P_4229 . im * B_311_473_0_idx_0 ) ) * _rtP -> P_187 ) ; _rtB -> B_311_1245_0
= ( rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP
-> P_189 ; _rtB -> B_311_1245_1 = ( rtb_B_311_37_0 * rtb_B_299_3_0 +
rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP -> P_189 ; break ; case 2 :
rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_1136_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_go [ 0 ] ) * _rtP -> P_1400 + _rtDW -> UnitDelay1_DSTATE_go
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_k5 [ 0 ] ) + ( _rtB -> B_311_1136_0 [ 0
] - ( ( _rtB -> B_311_1143_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_k5 [ 0 ] ) *
_rtP -> P_1403 + _rtDW -> UnitDelay2_DSTATE_k5 [ 0 ] ) ) ) * _rtP -> P_1404 *
_rtP -> P_1405 + ( _rtP -> P_4228 . re * rtb_B_311_68_0 - _rtP -> P_4228 . im
* B_311_401_0_idx_1 ) ) + ( _rtP -> P_4227 . re * rtb_B_311_51_0 - _rtP ->
P_4227 . im * rtb_B_311_52_0 ) ) * _rtP -> P_185 ; rtb_B_311_52_0 = ( ( ( ( (
( _rtB -> B_311_1136_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_go [ 1 ] ) * _rtP
-> P_1400 + _rtDW -> UnitDelay1_DSTATE_go [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_k5 [ 1 ] ) + ( _rtB -> B_311_1136_0 [ 1 ] - ( ( _rtB ->
B_311_1143_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_k5 [ 1 ] ) * _rtP -> P_1403 +
_rtDW -> UnitDelay2_DSTATE_k5 [ 1 ] ) ) ) * _rtP -> P_1404 * _rtP -> P_1405 +
( _rtP -> P_4228 . re * B_311_401_0_idx_1 + _rtP -> P_4228 . im *
rtb_B_311_68_0 ) ) + ( _rtP -> P_4227 . re * rtb_B_311_52_0 + _rtP -> P_4227
. im * rtb_B_311_51_0 ) ) * _rtP -> P_185 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB
-> B_311_1191_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_en [ 0 ] ) * _rtP ->
P_1431 + _rtDW -> UnitDelay1_DSTATE_en [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_nj [ 0 ] ) + ( _rtB -> B_311_1191_0 [ 0 ] - ( ( _rtB ->
B_311_1198_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_nj [ 0 ] ) * _rtP -> P_1434 +
_rtDW -> UnitDelay2_DSTATE_nj [ 0 ] ) ) ) * _rtP -> P_1435 * _rtP -> P_1436 +
( _rtP -> P_4226 . re * rtb_B_311_36_0 - _rtP -> P_4226 . im * rtb_B_299_3_0
) ) + ( _rtP -> P_4225 . re * B_311_473_0_idx_0 - _rtP -> P_4225 . im *
rtb_B_311_50_0 ) ) * _rtP -> P_184 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB ->
B_311_1191_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_en [ 1 ] ) * _rtP -> P_1431 +
_rtDW -> UnitDelay1_DSTATE_en [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_nj [ 1 ] )
+ ( _rtB -> B_311_1191_0 [ 1 ] - ( ( _rtB -> B_311_1198_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_nj [ 1 ] ) * _rtP -> P_1434 + _rtDW -> UnitDelay2_DSTATE_nj
[ 1 ] ) ) ) * _rtP -> P_1435 * _rtP -> P_1436 + ( _rtP -> P_4226 . re *
rtb_B_299_3_0 + _rtP -> P_4226 . im * rtb_B_311_36_0 ) ) + ( _rtP -> P_4225 .
re * rtb_B_311_50_0 + _rtP -> P_4225 . im * B_311_473_0_idx_0 ) ) * _rtP ->
P_184 ) ; _rtB -> B_311_1245_0 = ( rtb_B_311_37_0 * rtb_B_311_68_0 -
rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP -> P_186 ; _rtB -> B_311_1245_1 = (
rtb_B_311_37_0 * rtb_B_299_3_0 + rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP ->
P_186 ; break ; default : rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_1136_0 [
0 ] - _rtDW -> UnitDelay1_DSTATE_go [ 0 ] ) * _rtP -> P_1400 + _rtDW ->
UnitDelay1_DSTATE_go [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_k5 [ 0 ] ) + ( _rtB
-> B_311_1136_0 [ 0 ] - ( ( _rtB -> B_311_1143_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_k5 [ 0 ] ) * _rtP -> P_1403 + _rtDW -> UnitDelay2_DSTATE_k5
[ 0 ] ) ) ) * _rtP -> P_1404 * _rtP -> P_1405 + rtb_B_311_68_0 ) +
rtb_B_311_51_0 ) * _rtP -> P_182 ; rtb_B_311_52_0 = ( ( ( ( ( ( _rtB ->
B_311_1136_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_go [ 1 ] ) * _rtP -> P_1400 +
_rtDW -> UnitDelay1_DSTATE_go [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_k5 [ 1 ] )
+ ( _rtB -> B_311_1136_0 [ 1 ] - ( ( _rtB -> B_311_1143_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_k5 [ 1 ] ) * _rtP -> P_1403 + _rtDW -> UnitDelay2_DSTATE_k5
[ 1 ] ) ) ) * _rtP -> P_1404 * _rtP -> P_1405 + B_311_401_0_idx_1 ) +
rtb_B_311_52_0 ) * _rtP -> P_182 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB ->
B_311_1191_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_en [ 0 ] ) * _rtP -> P_1431 +
_rtDW -> UnitDelay1_DSTATE_en [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_nj [ 0 ] )
+ ( _rtB -> B_311_1191_0 [ 0 ] - ( ( _rtB -> B_311_1198_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_nj [ 0 ] ) * _rtP -> P_1434 + _rtDW -> UnitDelay2_DSTATE_nj
[ 0 ] ) ) ) * _rtP -> P_1435 * _rtP -> P_1436 + rtb_B_311_36_0 ) +
B_311_473_0_idx_0 ) * _rtP -> P_181 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB ->
B_311_1191_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_en [ 1 ] ) * _rtP -> P_1431 +
_rtDW -> UnitDelay1_DSTATE_en [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_nj [ 1 ] )
+ ( _rtB -> B_311_1191_0 [ 1 ] - ( ( _rtB -> B_311_1198_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_nj [ 1 ] ) * _rtP -> P_1434 + _rtDW -> UnitDelay2_DSTATE_nj
[ 1 ] ) ) ) * _rtP -> P_1435 * _rtP -> P_1436 + rtb_B_299_3_0 ) +
rtb_B_311_50_0 ) * _rtP -> P_181 ) ; _rtB -> B_311_1245_0 = ( rtb_B_311_37_0
* rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP -> P_183 ; _rtB ->
B_311_1245_1 = ( rtb_B_311_37_0 * rtb_B_299_3_0 + rtb_B_311_52_0 *
rtb_B_311_68_0 ) * _rtP -> P_183 ; break ; } } { { const char * errMsg = (
NULL ) ; void * excelLoader = ( void * ) _rtDW -> FromSpreadsheet_PWORK .
ExcelLoader ; void * TempDoubleOutput = ( void * ) & _rtDW ->
FromSpreadsheet_RWORK . TempDoubleOutput ; if ( excelLoader != ( NULL ) && (
ssIsMajorTimeStep ( S ) || ! 0 ) ) { void * y = & _rtB -> B_311_1246_0 ;
real_T t = ssGetTaskTime ( S , 0 ) ; errMsg = rtwExcelLoaderGetOutput ( &
TempDoubleOutput , excelLoader , t , ssIsMajorTimeStep ( S ) ) ; if ( errMsg
!= ( NULL ) ) { ssSetErrorStatus ( S , errMsg ) ; return ; } _rtB ->
B_311_1246_0 = _rtDW -> FromSpreadsheet_RWORK . TempDoubleOutput ; } } } _rtB
-> B_311_1248_0 = muDoubleScalarPower ( _rtB -> B_311_1246_0 , _rtB ->
B_311_11_0 ) * 618460.0 / 1771.561 ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
_rtDW -> Saturation_MODE = _rtB -> B_311_1248_0 >= _rtP -> P_1455 ? 1 : _rtB
-> B_311_1248_0 > _rtP -> P_1456 ? 0 : - 1 ; } _rtB -> B_311_1249_0 = _rtDW
-> Saturation_MODE == 1 ? _rtP -> P_1455 : _rtDW -> Saturation_MODE == - 1 ?
_rtP -> P_1456 : _rtB -> B_311_1248_0 ; _rtB -> B_311_1250_0 = _rtB ->
B_311_76_0 / 1000.0 * 430000.0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW
-> Saturation1_MODE = _rtB -> B_311_1250_0 >= _rtP -> P_1457 ? 1 : _rtB ->
B_311_1250_0 > _rtP -> P_1458 ? 0 : - 1 ; } _rtB -> B_311_1251_0 = _rtDW ->
Saturation1_MODE == 1 ? _rtP -> P_1457 : _rtDW -> Saturation1_MODE == - 1 ?
_rtP -> P_1458 : _rtB -> B_311_1250_0 ; { { const char * errMsg = ( NULL ) ;
void * excelLoader = ( void * ) _rtDW -> FromSpreadsheet1_PWORK . ExcelLoader
; void * TempDoubleOutput = ( void * ) & _rtDW -> FromSpreadsheet1_RWORK .
TempDoubleOutput ; if ( excelLoader != ( NULL ) && ( ssIsMajorTimeStep ( S )
|| ! 0 ) ) { void * y = & _rtB -> B_311_1252_0 ; real_T t = ssGetTaskTime ( S
, 0 ) ; errMsg = rtwExcelLoaderGetOutput ( & TempDoubleOutput , excelLoader ,
t , ssIsMajorTimeStep ( S ) ) ; if ( errMsg != ( NULL ) ) { ssSetErrorStatus
( S , errMsg ) ; return ; } _rtB -> B_311_1252_0 = _rtDW ->
FromSpreadsheet1_RWORK . TempDoubleOutput ; } } } { { const char * errMsg = (
NULL ) ; void * excelLoader = ( void * ) _rtDW -> FromSpreadsheet2_PWORK .
ExcelLoader ; void * TempDoubleOutput = ( void * ) & _rtDW ->
FromSpreadsheet2_RWORK . TempDoubleOutput ; if ( excelLoader != ( NULL ) && (
ssIsMajorTimeStep ( S ) || ! 0 ) ) { void * y = & _rtB -> B_311_1253_0 ;
real_T t = ssGetTaskTime ( S , 0 ) ; errMsg = rtwExcelLoaderGetOutput ( &
TempDoubleOutput , excelLoader , t , ssIsMajorTimeStep ( S ) ) ; if ( errMsg
!= ( NULL ) ) { ssSetErrorStatus ( S , errMsg ) ; return ; } _rtB ->
B_311_1253_0 = _rtDW -> FromSpreadsheet2_RWORK . TempDoubleOutput ; } } }
ssCallAccelRunBlock ( S , 311 , 1254 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 1255 , SS_CALL_MDL_OUTPUTS ) ; i =
ssIsSampleHit ( S , 5 , 0 ) ; if ( i != 0 ) { i = ssIsSampleHit ( S , 5 , 0 )
; if ( i != 0 ) { _rtB -> B_156_1_0 = _rtP -> P_257 - _rtB -> B_311_353_0 ;
if ( _rtB -> B_156_1_0 > _rtP -> P_259 ) { _rtB -> B_156_2_0 = _rtB ->
B_156_1_0 - _rtP -> P_259 ; } else if ( _rtB -> B_156_1_0 >= _rtP -> P_258 )
{ _rtB -> B_156_2_0 = 0.0 ; } else { _rtB -> B_156_2_0 = _rtB -> B_156_1_0 -
_rtP -> P_258 ; } rtb_B_311_50_0 = _rtP -> P_260 ; _rtB -> B_156_6_0 = ( _rtP
-> P_260 - _rtB -> B_156_3_0 [ 0 ] <= _rtB -> B_156_0_0 ) ; _rtB ->
B_156_11_0 = ( ( _rtP -> P_261 * _rtB -> B_156_2_0 + _rtDW ->
Integrator_DSTATE_os ) + _rtP -> P_257 ) * _rtP -> P_264 ; if ( _rtDW ->
Integrator_IC_LOADING_l != 0 ) { _rtDW -> Integrator_DSTATE_ij = _rtB ->
B_156_11_0 ; if ( _rtDW -> Integrator_DSTATE_ij >= _rtP -> P_266 ) { _rtDW ->
Integrator_DSTATE_ij = _rtP -> P_266 ; } else { if ( _rtDW ->
Integrator_DSTATE_ij <= _rtP -> P_267 ) { _rtDW -> Integrator_DSTATE_ij =
_rtP -> P_267 ; } } } if ( _rtB -> B_156_6_0 || ( _rtDW ->
Integrator_PrevResetState_hs != 0 ) ) { _rtDW -> Integrator_DSTATE_ij = _rtB
-> B_156_11_0 ; if ( _rtDW -> Integrator_DSTATE_ij >= _rtP -> P_266 ) { _rtDW
-> Integrator_DSTATE_ij = _rtP -> P_266 ; } else { if ( _rtDW ->
Integrator_DSTATE_ij <= _rtP -> P_267 ) { _rtDW -> Integrator_DSTATE_ij =
_rtP -> P_267 ; } } } if ( _rtDW -> Integrator_DSTATE_ij >= _rtP -> P_266 ) {
_rtDW -> Integrator_DSTATE_ij = _rtP -> P_266 ; } else { if ( _rtDW ->
Integrator_DSTATE_ij <= _rtP -> P_267 ) { _rtDW -> Integrator_DSTATE_ij =
_rtP -> P_267 ; } } if ( _rtDW -> Integrator_DSTATE_ij > _rtP -> P_268 ) {
_rtB -> B_156_13_0 = _rtP -> P_268 ; } else if ( _rtDW ->
Integrator_DSTATE_ij < _rtP -> P_269 ) { _rtB -> B_156_13_0 = _rtP -> P_269 ;
} else { _rtB -> B_156_13_0 = _rtDW -> Integrator_DSTATE_ij ; }
ssCallAccelRunBlock ( S , 156 , 14 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_156_15_0 = _rtP -> P_270 ; _rtB -> B_156_17_0 = ( _rtB -> B_156_15_0 - _rtB
-> B_311_864_0 ) * _rtP -> P_271 ; rtb_B_311_35_0 = _rtP -> P_272 ; _rtB ->
B_156_19_0 = _rtB -> B_156_17_0 + _rtP -> P_272 ; ssCallAccelRunBlock ( S ,
156 , 20 , SS_CALL_MDL_OUTPUTS ) ; rtb_B_311_51_0 = rtb_B_311_35_0 - _rtB ->
B_311_257_0 ; if ( rtb_B_311_51_0 > _rtP -> P_274 ) { rtb_B_311_51_0 -= _rtP
-> P_274 ; } else if ( rtb_B_311_51_0 >= _rtP -> P_273 ) { rtb_B_311_51_0 =
0.0 ; } else { rtb_B_311_51_0 -= _rtP -> P_273 ; } rtb_B_311_50_0 =
muDoubleScalarMax ( _rtB -> B_156_3_0 [ 0 ] , rtb_B_311_50_0 ) ; _rtB ->
B_156_26_0 = 1.0 / ( ( real_T ) ( rtb_B_311_50_0 == 0.0 ) *
2.2204460492503131e-16 + rtb_B_311_50_0 ) * ( _rtB -> B_156_11_0 - _rtB ->
B_156_13_0 ) ; rtb_B_311_50_0 = muDoubleScalarMax ( _rtB -> B_156_27_0 [ 0 ]
, _rtP -> P_275 ) ; _rtB -> B_156_32_0 = ( _rtP -> P_275 - _rtB -> B_156_27_0
[ 0 ] <= _rtB -> B_156_1_0_p ) ; _rtB -> B_156_37_0 = ( ( _rtP -> P_276 *
rtb_B_311_51_0 + _rtDW -> Integrator_DSTATE_em ) + rtb_B_311_35_0 ) * _rtP ->
P_279 ; if ( _rtDW -> Integrator_IC_LOADING_n != 0 ) { _rtDW ->
Integrator_DSTATE_ho = _rtB -> B_156_37_0 ; if ( _rtDW ->
Integrator_DSTATE_ho >= _rtP -> P_281 ) { _rtDW -> Integrator_DSTATE_ho =
_rtP -> P_281 ; } else { if ( _rtDW -> Integrator_DSTATE_ho <= _rtP -> P_282
) { _rtDW -> Integrator_DSTATE_ho = _rtP -> P_282 ; } } } if ( _rtB ->
B_156_32_0 || ( _rtDW -> Integrator_PrevResetState_a != 0 ) ) { _rtDW ->
Integrator_DSTATE_ho = _rtB -> B_156_37_0 ; if ( _rtDW ->
Integrator_DSTATE_ho >= _rtP -> P_281 ) { _rtDW -> Integrator_DSTATE_ho =
_rtP -> P_281 ; } else { if ( _rtDW -> Integrator_DSTATE_ho <= _rtP -> P_282
) { _rtDW -> Integrator_DSTATE_ho = _rtP -> P_282 ; } } } if ( _rtDW ->
Integrator_DSTATE_ho >= _rtP -> P_281 ) { _rtDW -> Integrator_DSTATE_ho =
_rtP -> P_281 ; } else { if ( _rtDW -> Integrator_DSTATE_ho <= _rtP -> P_282
) { _rtDW -> Integrator_DSTATE_ho = _rtP -> P_282 ; } } if ( _rtDW ->
Integrator_DSTATE_ho > _rtP -> P_283 ) { B_156_39_0 = _rtP -> P_283 ; } else
if ( _rtDW -> Integrator_DSTATE_ho < _rtP -> P_284 ) { B_156_39_0 = _rtP ->
P_284 ; } else { B_156_39_0 = _rtDW -> Integrator_DSTATE_ho ; } _rtB ->
B_156_41_0 = 1.0 / ( ( real_T ) ( rtb_B_311_50_0 == 0.0 ) *
2.2204460492503131e-16 + rtb_B_311_50_0 ) * ( _rtB -> B_156_37_0 - B_156_39_0
) ; rtb_B_311_50_0 = muDoubleScalarMax ( _rtB -> B_156_42_0 [ 0 ] , _rtP ->
P_285 ) ; _rtB -> B_156_47_0 = ( _rtP -> P_285 - _rtB -> B_156_42_0 [ 0 ] <=
_rtB -> B_156_2_0_m ) ; _rtB -> B_156_48_0 = _rtP -> P_286 * _rtB ->
B_156_19_0 ; if ( _rtDW -> Integrator_IC_LOADING_h != 0 ) { _rtDW ->
Integrator_DSTATE_nj = _rtB -> B_156_48_0 ; if ( _rtDW ->
Integrator_DSTATE_nj >= _rtP -> P_288 ) { _rtDW -> Integrator_DSTATE_nj =
_rtP -> P_288 ; } else { if ( _rtDW -> Integrator_DSTATE_nj <= _rtP -> P_289
) { _rtDW -> Integrator_DSTATE_nj = _rtP -> P_289 ; } } } if ( _rtB ->
B_156_47_0 || ( _rtDW -> Integrator_PrevResetState_m != 0 ) ) { _rtDW ->
Integrator_DSTATE_nj = _rtB -> B_156_48_0 ; if ( _rtDW ->
Integrator_DSTATE_nj >= _rtP -> P_288 ) { _rtDW -> Integrator_DSTATE_nj =
_rtP -> P_288 ; } else { if ( _rtDW -> Integrator_DSTATE_nj <= _rtP -> P_289
) { _rtDW -> Integrator_DSTATE_nj = _rtP -> P_289 ; } } } if ( _rtDW ->
Integrator_DSTATE_nj >= _rtP -> P_288 ) { _rtDW -> Integrator_DSTATE_nj =
_rtP -> P_288 ; } else { if ( _rtDW -> Integrator_DSTATE_nj <= _rtP -> P_289
) { _rtDW -> Integrator_DSTATE_nj = _rtP -> P_289 ; } } if ( _rtDW ->
Integrator_DSTATE_nj > _rtP -> P_290 ) { B_156_50_0 = _rtP -> P_290 ; } else
if ( _rtDW -> Integrator_DSTATE_nj < _rtP -> P_291 ) { B_156_50_0 = _rtP ->
P_291 ; } else { B_156_50_0 = _rtDW -> Integrator_DSTATE_nj ; } _rtB ->
B_156_52_0 = 1.0 / ( ( real_T ) ( rtb_B_311_50_0 == 0.0 ) *
2.2204460492503131e-16 + rtb_B_311_50_0 ) * ( _rtB -> B_156_48_0 - B_156_50_0
) ; _rtB -> B_155_0_0 [ 0 ] = _rtB -> B_311_732_0 ; _rtB -> B_155_0_0 [ 1 ] =
_rtB -> B_311_733_0 ; _rtB -> B_155_1_0 [ 0 ] = _rtB -> B_311_860_0 ; _rtB ->
B_155_1_0 [ 1 ] = _rtB -> B_311_860_1 ; _rtB -> B_155_2_0 [ 0 ] = _rtB ->
B_311_1118_0 ; _rtB -> B_155_2_0 [ 1 ] = _rtB -> B_311_1118_1 ; _rtB ->
B_155_3_0 [ 0 ] = _rtB -> B_311_1245_0 ; _rtB -> B_155_3_0 [ 1 ] = _rtB ->
B_311_1245_1 ; _rtB -> B_155_4_0 [ 0 ] = _rtB -> B_311_1252_0 ; _rtB ->
B_155_4_0 [ 1 ] = _rtB -> B_311_1253_0 ; ssCallAccelRunBlock ( S , 155 , 5 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_156_54_0 = _rtP -> P_292 * rtb_B_311_51_0 ;
_rtB -> B_156_55_0 = _rtP -> P_293 * _rtB -> B_156_2_0 ; if ( _rtB ->
B_155_5_5 > _rtP -> P_294 ) { _rtB -> B_156_56_0 = B_156_50_0 ; } else { _rtB
-> B_156_56_0 = B_156_39_0 ; } } if ( _rtP -> P_4280 == 1 ) { _rtB ->
B_311_1257_0 = _rtB -> B_311_15_0_k ; } else { _rtB -> B_311_1257_0 = _rtB ->
B_155_5_2 ; } if ( _rtP -> P_4281 == 1 ) { _rtB -> B_311_1258_0 = _rtB ->
B_311_12_0 ; } else { _rtB -> B_311_1258_0 = _rtB -> B_155_5_3 ; } if ( _rtP
-> P_4282 == 1 ) { _rtB -> B_311_1259_0 = _rtB -> B_311_14_0_c ; } else {
_rtB -> B_311_1259_0 = _rtB -> B_155_5_4 ; } if ( _rtP -> P_4283 == 1 ) {
_rtB -> B_311_1260_0 = _rtB -> B_311_16_0_c ; } else { _rtB -> B_311_1260_0 =
_rtB -> B_155_5_5 ; } if ( _rtP -> P_4284 == 1 ) { _rtB -> B_311_1262_0 =
_rtB -> B_311_13_0_m ; } else { _rtB -> B_311_1262_0 = _rtB -> B_155_5_10 ; }
if ( _rtP -> P_4285 == 1 ) { _rtB -> B_311_1264_0 = _rtB -> B_311_19_0 ; }
else { _rtB -> B_311_1264_0 = _rtB -> B_155_5_12 ; } if ( _rtP -> P_4286 == 1
) { _rtB -> B_311_1265_0 = _rtB -> B_311_18_0 ; } else { _rtB -> B_311_1265_0
= _rtB -> B_155_5_13 ; } if ( _rtP -> P_4287 == 1 ) { _rtB -> B_311_1266_0 =
_rtB -> B_311_17_0 ; } else { _rtB -> B_311_1266_0 = _rtB -> B_155_5_14 ; }
if ( _rtB -> B_155_5_7 [ 0 ] > _rtP -> P_1459 [ 0 ] ) { _rtB -> B_311_1261_0
[ 0 ] = _rtP -> P_1459 [ 0 ] ; } else if ( _rtB -> B_155_5_7 [ 0 ] < _rtP ->
P_1460 [ 0 ] ) { _rtB -> B_311_1261_0 [ 0 ] = _rtP -> P_1460 [ 0 ] ; } else {
_rtB -> B_311_1261_0 [ 0 ] = _rtB -> B_155_5_7 [ 0 ] ; } if ( _rtB ->
B_155_5_11 [ 0 ] > _rtP -> P_1461 [ 0 ] ) { _rtB -> B_311_1263_0 [ 0 ] = _rtP
-> P_1461 [ 0 ] ; } else if ( _rtB -> B_155_5_11 [ 0 ] < _rtP -> P_1462 [ 0 ]
) { _rtB -> B_311_1263_0 [ 0 ] = _rtP -> P_1462 [ 0 ] ; } else { _rtB ->
B_311_1263_0 [ 0 ] = _rtB -> B_155_5_11 [ 0 ] ; } if ( _rtB -> B_155_5_15 [ 0
] > _rtP -> P_1463 [ 0 ] ) { _rtB -> B_311_1267_0 [ 0 ] = _rtP -> P_1463 [ 0
] ; } else if ( _rtB -> B_155_5_15 [ 0 ] < _rtP -> P_1464 [ 0 ] ) { _rtB ->
B_311_1267_0 [ 0 ] = _rtP -> P_1464 [ 0 ] ; } else { _rtB -> B_311_1267_0 [ 0
] = _rtB -> B_155_5_15 [ 0 ] ; } if ( _rtB -> B_155_5_7 [ 1 ] > _rtP ->
P_1459 [ 1 ] ) { _rtB -> B_311_1261_0 [ 1 ] = _rtP -> P_1459 [ 1 ] ; } else
if ( _rtB -> B_155_5_7 [ 1 ] < _rtP -> P_1460 [ 1 ] ) { _rtB -> B_311_1261_0
[ 1 ] = _rtP -> P_1460 [ 1 ] ; } else { _rtB -> B_311_1261_0 [ 1 ] = _rtB ->
B_155_5_7 [ 1 ] ; } if ( _rtB -> B_155_5_11 [ 1 ] > _rtP -> P_1461 [ 1 ] ) {
_rtB -> B_311_1263_0 [ 1 ] = _rtP -> P_1461 [ 1 ] ; } else if ( _rtB ->
B_155_5_11 [ 1 ] < _rtP -> P_1462 [ 1 ] ) { _rtB -> B_311_1263_0 [ 1 ] = _rtP
-> P_1462 [ 1 ] ; } else { _rtB -> B_311_1263_0 [ 1 ] = _rtB -> B_155_5_11 [
1 ] ; } if ( _rtB -> B_155_5_15 [ 1 ] > _rtP -> P_1463 [ 1 ] ) { _rtB ->
B_311_1267_0 [ 1 ] = _rtP -> P_1463 [ 1 ] ; } else if ( _rtB -> B_155_5_15 [
1 ] < _rtP -> P_1464 [ 1 ] ) { _rtB -> B_311_1267_0 [ 1 ] = _rtP -> P_1464 [
1 ] ; } else { _rtB -> B_311_1267_0 [ 1 ] = _rtB -> B_155_5_15 [ 1 ] ; }
ssCallAccelRunBlock ( S , 311 , 1268 , SS_CALL_MDL_OUTPUTS ) ; } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { B_311_1062_0_idx_0 = _rtP ->
P_1465 * _rtB -> B_311_152_0 [ 81 ] * _rtP -> P_1468 / _rtB -> B_311_217_0_n
; B_311_1062_0_idx_1 = _rtP -> P_1466 * _rtB -> B_311_152_0 [ 82 ] * _rtP ->
P_1468 / _rtB -> B_311_217_0_n ; B_311_1062_0_idx_2 = _rtP -> P_1467 * _rtB
-> B_311_152_0 [ 83 ] * _rtP -> P_1468 / _rtB -> B_311_217_0_n ; _rtB ->
B_311_1274_0 [ 0 ] = B_311_1062_0_idx_0 * B_311_1062_0_idx_0 ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_l1 != 0 ) { _rtB -> B_311_1275_0 [ 0 ] = _rtDW ->
Integ4_DSTATE_c [ 0 ] ; } else { _rtB -> B_311_1275_0 [ 0 ] = _rtP -> P_1469
* _rtB -> B_311_1274_0 [ 0 ] + _rtDW -> Integ4_DSTATE_c [ 0 ] ; } _rtB ->
B_311_1274_0 [ 1 ] = B_311_1062_0_idx_1 * B_311_1062_0_idx_1 ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_l1 != 0 ) { _rtB -> B_311_1275_0 [ 1 ] = _rtDW ->
Integ4_DSTATE_c [ 1 ] ; } else { _rtB -> B_311_1275_0 [ 1 ] = _rtP -> P_1469
* _rtB -> B_311_1274_0 [ 1 ] + _rtDW -> Integ4_DSTATE_c [ 1 ] ; } _rtB ->
B_311_1274_0 [ 2 ] = B_311_1062_0_idx_2 * B_311_1062_0_idx_2 ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_l1 != 0 ) { _rtB -> B_311_1275_0 [ 2 ] = _rtDW ->
Integ4_DSTATE_c [ 2 ] ; } else { _rtB -> B_311_1275_0 [ 2 ] = _rtP -> P_1469
* _rtB -> B_311_1274_0 [ 2 ] + _rtDW -> Integ4_DSTATE_c [ 2 ] ; } _rtB ->
B_311_1276_0 = _rtP -> P_1471 ; ssCallAccelRunBlock ( S , 311 , 1277 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) < _rtP -> P_1474 ) {
rtb_B_299_3_0 = _rtP -> P_1475 ; } else { rtb_B_299_3_0 = _rtP -> P_1476 ; }
if ( rtb_B_299_3_0 >= _rtP -> P_1478 ) { B_311_1283_0_idx_0 = ( _rtB ->
B_311_1275_0 [ 0 ] - _rtB -> B_311_1277_0 [ 0 ] ) * _rtP -> P_1472 + ( _rtP
-> P_130 * _rtB -> B_311_1274_0 [ 0 ] - _rtP -> P_129 * _rtDW ->
UnitDelay_DSTATE_e [ 0 ] ) ; B_311_1283_0_idx_1 = ( _rtB -> B_311_1275_0 [ 1
] - _rtB -> B_311_1277_0 [ 1 ] ) * _rtP -> P_1472 + ( _rtP -> P_130 * _rtB ->
B_311_1274_0 [ 1 ] - _rtP -> P_129 * _rtDW -> UnitDelay_DSTATE_e [ 1 ] ) ;
B_311_1283_0_idx_2 = ( _rtB -> B_311_1275_0 [ 2 ] - _rtB -> B_311_1277_0 [ 2
] ) * _rtP -> P_1472 + ( _rtP -> P_130 * _rtB -> B_311_1274_0 [ 2 ] - _rtP ->
P_129 * _rtDW -> UnitDelay_DSTATE_e [ 2 ] ) ; } else { B_311_1283_0_idx_0 =
_rtP -> P_1477 ; B_311_1283_0_idx_1 = _rtP -> P_1477 ; B_311_1283_0_idx_2 =
_rtP -> P_1477 ; } B_311_1283_0_idx_0 = muDoubleScalarAbs (
B_311_1283_0_idx_0 ) ; B_311_1283_0_idx_1 = muDoubleScalarAbs (
B_311_1283_0_idx_1 ) ; B_311_1283_0_idx_2 = muDoubleScalarAbs (
B_311_1283_0_idx_2 ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtDW ->
SignedSqrt_DWORK1 != 0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
_rtDW -> SignedSqrt_DWORK1 = 0 ; } B_311_1283_0_idx_0 = muDoubleScalarSqrt (
B_311_1283_0_idx_0 ) ; B_311_1283_0_idx_1 = muDoubleScalarSqrt (
B_311_1283_0_idx_1 ) ; B_311_1283_0_idx_2 = muDoubleScalarSqrt (
B_311_1283_0_idx_2 ) ; } else { B_311_1283_0_idx_0 = muDoubleScalarSqrt (
B_311_1283_0_idx_0 ) ; B_311_1283_0_idx_1 = muDoubleScalarSqrt (
B_311_1283_0_idx_1 ) ; B_311_1283_0_idx_2 = muDoubleScalarSqrt (
B_311_1283_0_idx_2 ) ; } _rtB -> B_311_1286_0 [ 0 ] = _rtP -> P_1479 *
B_311_1283_0_idx_0 ; B_311_1283_0_idx_0 = B_299_0_0_idx_0 / _rtB ->
B_311_217_0_n ; _rtB -> B_311_1288_0 [ 0 ] = B_311_1283_0_idx_0 *
B_311_1283_0_idx_0 ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_d4 != 0 ) { _rtB ->
B_311_1289_0 [ 0 ] = _rtDW -> Integ4_DSTATE_jv [ 0 ] ; } else { _rtB ->
B_311_1289_0 [ 0 ] = _rtP -> P_1480 * _rtB -> B_311_1288_0 [ 0 ] + _rtDW ->
Integ4_DSTATE_jv [ 0 ] ; } B_299_0_0_idx_0 = B_311_1283_0_idx_0 ; _rtB ->
B_311_1286_0 [ 1 ] = _rtP -> P_1479 * B_311_1283_0_idx_1 ; B_311_1283_0_idx_0
= B_299_0_0_idx_1 / _rtB -> B_311_217_0_n ; _rtB -> B_311_1288_0 [ 1 ] =
B_311_1283_0_idx_0 * B_311_1283_0_idx_0 ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_d4 != 0 ) { _rtB -> B_311_1289_0 [ 1 ] = _rtDW ->
Integ4_DSTATE_jv [ 1 ] ; } else { _rtB -> B_311_1289_0 [ 1 ] = _rtP -> P_1480
* _rtB -> B_311_1288_0 [ 1 ] + _rtDW -> Integ4_DSTATE_jv [ 1 ] ; }
B_299_0_0_idx_1 = B_311_1283_0_idx_0 ; _rtB -> B_311_1286_0 [ 2 ] = _rtP ->
P_1479 * B_311_1283_0_idx_2 ; B_311_1283_0_idx_0 = B_299_0_0_idx_2 / _rtB ->
B_311_217_0_n ; _rtB -> B_311_1288_0 [ 2 ] = B_311_1283_0_idx_0 *
B_311_1283_0_idx_0 ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_d4 != 0 ) { _rtB ->
B_311_1289_0 [ 2 ] = _rtDW -> Integ4_DSTATE_jv [ 2 ] ; } else { _rtB ->
B_311_1289_0 [ 2 ] = _rtP -> P_1480 * _rtB -> B_311_1288_0 [ 2 ] + _rtDW ->
Integ4_DSTATE_jv [ 2 ] ; } B_299_0_0_idx_2 = B_311_1283_0_idx_0 ; _rtB ->
B_311_1290_0 = _rtP -> P_1482 ; ssCallAccelRunBlock ( S , 311 , 1291 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) < _rtP -> P_1485 ) {
rtb_B_299_3_0 = _rtP -> P_1486 ; } else { rtb_B_299_3_0 = _rtP -> P_1487 ; }
if ( rtb_B_299_3_0 >= _rtP -> P_1489 ) { B_311_1283_0_idx_0 = ( _rtB ->
B_311_1289_0 [ 0 ] - _rtB -> B_311_1291_0 [ 0 ] ) * _rtP -> P_1483 + ( _rtP
-> P_128 * _rtB -> B_311_1288_0 [ 0 ] - _rtP -> P_127 * _rtDW ->
UnitDelay_DSTATE_ft [ 0 ] ) ; B_311_1283_0_idx_1 = ( _rtB -> B_311_1289_0 [ 1
] - _rtB -> B_311_1291_0 [ 1 ] ) * _rtP -> P_1483 + ( _rtP -> P_128 * _rtB ->
B_311_1288_0 [ 1 ] - _rtP -> P_127 * _rtDW -> UnitDelay_DSTATE_ft [ 1 ] ) ;
B_311_1283_0_idx_2 = ( _rtB -> B_311_1289_0 [ 2 ] - _rtB -> B_311_1291_0 [ 2
] ) * _rtP -> P_1483 + ( _rtP -> P_128 * _rtB -> B_311_1288_0 [ 2 ] - _rtP ->
P_127 * _rtDW -> UnitDelay_DSTATE_ft [ 2 ] ) ; } else { B_311_1283_0_idx_0 =
_rtP -> P_1488 ; B_311_1283_0_idx_1 = _rtP -> P_1488 ; B_311_1283_0_idx_2 =
_rtP -> P_1488 ; } B_311_1283_0_idx_0 = muDoubleScalarAbs (
B_311_1283_0_idx_0 ) ; B_311_1283_0_idx_1 = muDoubleScalarAbs (
B_311_1283_0_idx_1 ) ; B_311_1283_0_idx_2 = muDoubleScalarAbs (
B_311_1283_0_idx_2 ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtDW ->
SignedSqrt_DWORK1_k != 0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S )
; _rtDW -> SignedSqrt_DWORK1_k = 0 ; } B_311_1283_0_idx_0 =
muDoubleScalarSqrt ( B_311_1283_0_idx_0 ) ; B_311_1283_0_idx_1 =
muDoubleScalarSqrt ( B_311_1283_0_idx_1 ) ; B_311_1283_0_idx_2 =
muDoubleScalarSqrt ( B_311_1283_0_idx_2 ) ; } else { B_311_1283_0_idx_0 =
muDoubleScalarSqrt ( B_311_1283_0_idx_0 ) ; B_311_1283_0_idx_1 =
muDoubleScalarSqrt ( B_311_1283_0_idx_1 ) ; B_311_1283_0_idx_2 =
muDoubleScalarSqrt ( B_311_1283_0_idx_2 ) ; } _rtB -> B_311_1300_0 [ 0 ] =
_rtP -> P_1490 * B_311_1283_0_idx_0 ; _rtB -> B_311_1300_0 [ 1 ] = _rtP ->
P_1490 * B_311_1283_0_idx_1 ; _rtB -> B_311_1300_0 [ 2 ] = _rtP -> P_1490 *
B_311_1283_0_idx_2 ; ssCallAccelRunBlock ( S , 49 , 0 , SS_CALL_MDL_OUTPUTS )
; _rtB -> B_311_1302_0 = ( muDoubleScalarAbs ( _rtB -> B_311_1300_0 [ 0 ] -
_rtB -> B_311_1286_0 [ 0 ] ) <= _rtB -> B_311_214_0 ) ; _rtB -> B_311_1303_0
= _rtDW -> UnitDelay_DSTATE_ku ; _rtB -> B_311_1304_0 = _rtDW ->
UnitDelay_DSTATE_fa ; _rtB -> B_311_1305_0 = ( muDoubleScalarAbs ( _rtB ->
B_311_1304_0 - _rtB -> B_311_1303_0 ) <= _rtB -> B_311_216_0 ) ; _rtB ->
B_311_1306_0 = _rtDW -> UnitDelay1_DSTATE_jd ; _rtB -> B_311_1307_0 = _rtDW
-> UnitDelay1_DSTATE_nl ; rtb_B_311_35_0 = _rtB -> B_311_1307_0 - _rtB ->
B_311_1306_0 ; _rtB -> B_311_1315_0 = muDoubleScalarAbs ( ( ( real_T ) (
rtb_B_311_35_0 <= _rtB -> B_311_219_0_m ? _rtP -> P_4249 : 0U ) *
1.862645149230957E-9 + rtb_B_311_35_0 ) - ( real_T ) ( rtb_B_311_35_0 >= _rtB
-> B_311_220_0_c ? _rtP -> P_4250 : 0U ) * 1.862645149230957E-9 ) * _rtP ->
P_1495 ; _rtB -> B_311_1316_0 = ( _rtB -> B_311_1315_0 <= _rtB -> B_311_215_0
) ; _rtB -> B_311_1318_0 = ( _rtB -> B_49_0_1 || ( ( _rtB -> B_311_1302_0 !=
0.0 ) && ( _rtB -> B_311_1305_0 != 0.0 ) && ( _rtB -> B_311_1316_0 != 0.0 ) )
) ; _rtB -> B_311_1319_0 = ( _rtB -> B_311_218_0_k && ( _rtB -> B_311_1265_0
!= 0.0 ) && _rtB -> B_311_1318_0 ) ; } i = ssIsSampleHit ( S , 5 , 0 ) ; if (
i != 0 ) { _rtB -> B_311_1320_0 = ( ( _rtB -> B_311_212_0 != 0.0 ) || ( _rtB
-> B_311_1264_0 != 0.0 ) ) ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0
) { i = ( int32_T ) ( ( ( ( ( uint32_T ) _rtB -> B_311_1319_0 << 1 ) + _rtB
-> B_311_1320_0 ) << 1 ) + _rtDW -> Memory_PreviousInput_jf ) ; _rtB ->
B_311_1322_0 [ 0U ] = _rtP -> P_4263 [ ( uint32_T ) i ] ; _rtB ->
B_311_1322_0 [ 1U ] = _rtP -> P_4263 [ i + 8U ] ; ssCallAccelRunBlock ( S ,
311 , 1323 , SS_CALL_MDL_OUTPUTS ) ; rtb_B_311_1324_0 = _rtP -> P_1496 ;
B_311_1283_0_idx_2 = _rtB -> B_311_157_0 [ 0 ] / _rtB -> B_311_202_0_i ; _rtB
-> B_311_1326_0 [ 0 ] = B_311_1283_0_idx_2 * B_311_1283_0_idx_2 ; if ( _rtDW
-> Integ4_SYSTEM_ENABLE_ks != 0 ) { _rtB -> B_311_1327_0 [ 0 ] = _rtDW ->
Integ4_DSTATE_gd [ 0 ] ; } else { _rtB -> B_311_1327_0 [ 0 ] = _rtP -> P_1497
* _rtB -> B_311_1326_0 [ 0 ] + _rtDW -> Integ4_DSTATE_gd [ 0 ] ; }
B_311_1283_0_idx_0 = B_311_1283_0_idx_2 ; B_311_1283_0_idx_2 = _rtB ->
B_311_157_0 [ 1 ] / _rtB -> B_311_202_0_i ; _rtB -> B_311_1326_0 [ 1 ] =
B_311_1283_0_idx_2 * B_311_1283_0_idx_2 ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_ks != 0 ) { _rtB -> B_311_1327_0 [ 1 ] = _rtDW ->
Integ4_DSTATE_gd [ 1 ] ; } else { _rtB -> B_311_1327_0 [ 1 ] = _rtP -> P_1497
* _rtB -> B_311_1326_0 [ 1 ] + _rtDW -> Integ4_DSTATE_gd [ 1 ] ; }
B_311_1283_0_idx_1 = B_311_1283_0_idx_2 ; B_311_1283_0_idx_2 = _rtB ->
B_311_157_0 [ 2 ] / _rtB -> B_311_202_0_i ; _rtB -> B_311_1326_0 [ 2 ] =
B_311_1283_0_idx_2 * B_311_1283_0_idx_2 ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_ks != 0 ) { _rtB -> B_311_1327_0 [ 2 ] = _rtDW ->
Integ4_DSTATE_gd [ 2 ] ; } else { _rtB -> B_311_1327_0 [ 2 ] = _rtP -> P_1497
* _rtB -> B_311_1326_0 [ 2 ] + _rtDW -> Integ4_DSTATE_gd [ 2 ] ; } _rtB ->
B_311_1328_0 = _rtP -> P_1499 ; ssCallAccelRunBlock ( S , 311 , 1329 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) < _rtP -> P_1502 ) {
rtb_B_299_3_0 = _rtP -> P_1503 ; } else { rtb_B_299_3_0 = _rtP -> P_1504 ; }
if ( rtb_B_299_3_0 >= _rtP -> P_1506 ) { rtb_B_311_1335_0 [ 0 ] = ( _rtB ->
B_311_1327_0 [ 0 ] - _rtB -> B_311_1329_0 [ 0 ] ) * _rtP -> P_1500 + ( _rtP
-> P_122 * _rtB -> B_311_1326_0 [ 0 ] - _rtP -> P_121 * _rtDW ->
UnitDelay_DSTATE_ca [ 0 ] ) ; rtb_B_311_1335_0 [ 1 ] = ( _rtB -> B_311_1327_0
[ 1 ] - _rtB -> B_311_1329_0 [ 1 ] ) * _rtP -> P_1500 + ( _rtP -> P_122 *
_rtB -> B_311_1326_0 [ 1 ] - _rtP -> P_121 * _rtDW -> UnitDelay_DSTATE_ca [ 1
] ) ; rtb_B_311_1335_0 [ 2 ] = ( _rtB -> B_311_1327_0 [ 2 ] - _rtB ->
B_311_1329_0 [ 2 ] ) * _rtP -> P_1500 + ( _rtP -> P_122 * _rtB ->
B_311_1326_0 [ 2 ] - _rtP -> P_121 * _rtDW -> UnitDelay_DSTATE_ca [ 2 ] ) ; }
else { rtb_B_311_1335_0 [ 0 ] = _rtP -> P_1505 ; rtb_B_311_1335_0 [ 1 ] =
_rtP -> P_1505 ; rtb_B_311_1335_0 [ 2 ] = _rtP -> P_1505 ; } rtb_B_311_1335_0
[ 0 ] = muDoubleScalarAbs ( rtb_B_311_1335_0 [ 0 ] ) ; rtb_B_311_1335_0 [ 1 ]
= muDoubleScalarAbs ( rtb_B_311_1335_0 [ 1 ] ) ; rtb_B_311_1335_0 [ 2 ] =
muDoubleScalarAbs ( rtb_B_311_1335_0 [ 2 ] ) ; if ( ssIsMajorTimeStep ( S )
!= 0 ) { if ( _rtDW -> SignedSqrt_DWORK1_i != 0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW ->
SignedSqrt_DWORK1_i = 0 ; } rtb_B_311_1335_0 [ 0 ] = muDoubleScalarSqrt (
rtb_B_311_1335_0 [ 0 ] ) ; rtb_B_311_1335_0 [ 1 ] = muDoubleScalarSqrt (
rtb_B_311_1335_0 [ 1 ] ) ; rtb_B_311_1335_0 [ 2 ] = muDoubleScalarSqrt (
rtb_B_311_1335_0 [ 2 ] ) ; } else { rtb_B_311_1335_0 [ 0 ] =
muDoubleScalarSqrt ( rtb_B_311_1335_0 [ 0 ] ) ; rtb_B_311_1335_0 [ 1 ] =
muDoubleScalarSqrt ( rtb_B_311_1335_0 [ 1 ] ) ; rtb_B_311_1335_0 [ 2 ] =
muDoubleScalarSqrt ( rtb_B_311_1335_0 [ 2 ] ) ; } _rtB -> B_311_1338_0 [ 0 ]
= _rtP -> P_1507 * rtb_B_311_1335_0 [ 0 ] ; rtb_B_311_35_0 = _rtB ->
B_311_609_0 [ 0 ] / _rtB -> B_311_202_0_i ; _rtB -> B_311_1340_0 [ 0 ] =
rtb_B_311_35_0 * rtb_B_311_35_0 ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_b != 0 )
{ _rtB -> B_311_1341_0 [ 0 ] = _rtDW -> Integ4_DSTATE_ow [ 0 ] ; } else {
_rtB -> B_311_1341_0 [ 0 ] = _rtP -> P_1508 * _rtB -> B_311_1340_0 [ 0 ] +
_rtDW -> Integ4_DSTATE_ow [ 0 ] ; } rtb_B_311_1335_0 [ 0 ] = rtb_B_311_35_0 ;
_rtB -> B_311_1338_0 [ 1 ] = _rtP -> P_1507 * rtb_B_311_1335_0 [ 1 ] ;
rtb_B_311_35_0 = _rtB -> B_311_609_0 [ 1 ] / _rtB -> B_311_202_0_i ; _rtB ->
B_311_1340_0 [ 1 ] = rtb_B_311_35_0 * rtb_B_311_35_0 ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_b != 0 ) { _rtB -> B_311_1341_0 [ 1 ] = _rtDW ->
Integ4_DSTATE_ow [ 1 ] ; } else { _rtB -> B_311_1341_0 [ 1 ] = _rtP -> P_1508
* _rtB -> B_311_1340_0 [ 1 ] + _rtDW -> Integ4_DSTATE_ow [ 1 ] ; }
rtb_B_311_1335_0 [ 1 ] = rtb_B_311_35_0 ; _rtB -> B_311_1338_0 [ 2 ] = _rtP
-> P_1507 * rtb_B_311_1335_0 [ 2 ] ; rtb_B_311_35_0 = _rtB -> B_311_609_0 [ 2
] / _rtB -> B_311_202_0_i ; _rtB -> B_311_1340_0 [ 2 ] = rtb_B_311_35_0 *
rtb_B_311_35_0 ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_b != 0 ) { _rtB ->
B_311_1341_0 [ 2 ] = _rtDW -> Integ4_DSTATE_ow [ 2 ] ; } else { _rtB ->
B_311_1341_0 [ 2 ] = _rtP -> P_1508 * _rtB -> B_311_1340_0 [ 2 ] + _rtDW ->
Integ4_DSTATE_ow [ 2 ] ; } rtb_B_311_1335_0 [ 2 ] = rtb_B_311_35_0 ; _rtB ->
B_311_1342_0 = _rtP -> P_1510 ; ssCallAccelRunBlock ( S , 311 , 1343 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) < _rtP -> P_1513 ) {
rtb_B_299_3_0 = _rtP -> P_1514 ; } else { rtb_B_299_3_0 = _rtP -> P_1515 ; }
if ( rtb_B_299_3_0 >= _rtP -> P_1517 ) { B_311_1349_0_idx_0 = ( _rtB ->
B_311_1341_0 [ 0 ] - _rtB -> B_311_1343_0 [ 0 ] ) * _rtP -> P_1511 + ( _rtP
-> P_120 * _rtB -> B_311_1340_0 [ 0 ] - _rtP -> P_119 * _rtDW ->
UnitDelay_DSTATE_al [ 0 ] ) ; B_311_1349_0_idx_1 = ( _rtB -> B_311_1341_0 [ 1
] - _rtB -> B_311_1343_0 [ 1 ] ) * _rtP -> P_1511 + ( _rtP -> P_120 * _rtB ->
B_311_1340_0 [ 1 ] - _rtP -> P_119 * _rtDW -> UnitDelay_DSTATE_al [ 1 ] ) ;
B_311_1349_0_idx_2 = ( _rtB -> B_311_1341_0 [ 2 ] - _rtB -> B_311_1343_0 [ 2
] ) * _rtP -> P_1511 + ( _rtP -> P_120 * _rtB -> B_311_1340_0 [ 2 ] - _rtP ->
P_119 * _rtDW -> UnitDelay_DSTATE_al [ 2 ] ) ; } else { B_311_1349_0_idx_0 =
_rtP -> P_1516 ; B_311_1349_0_idx_1 = _rtP -> P_1516 ; B_311_1349_0_idx_2 =
_rtP -> P_1516 ; } B_311_1349_0_idx_0 = muDoubleScalarAbs (
B_311_1349_0_idx_0 ) ; B_311_1349_0_idx_1 = muDoubleScalarAbs (
B_311_1349_0_idx_1 ) ; B_311_1349_0_idx_2 = muDoubleScalarAbs (
B_311_1349_0_idx_2 ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtDW ->
SignedSqrt_DWORK1_d != 0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S )
; _rtDW -> SignedSqrt_DWORK1_d = 0 ; } B_311_1349_0_idx_0 =
muDoubleScalarSqrt ( B_311_1349_0_idx_0 ) ; B_311_1349_0_idx_1 =
muDoubleScalarSqrt ( B_311_1349_0_idx_1 ) ; B_311_1349_0_idx_2 =
muDoubleScalarSqrt ( B_311_1349_0_idx_2 ) ; } else { B_311_1349_0_idx_0 =
muDoubleScalarSqrt ( B_311_1349_0_idx_0 ) ; B_311_1349_0_idx_1 =
muDoubleScalarSqrt ( B_311_1349_0_idx_1 ) ; B_311_1349_0_idx_2 =
muDoubleScalarSqrt ( B_311_1349_0_idx_2 ) ; } _rtB -> B_311_1352_0 [ 0 ] =
_rtP -> P_1518 * B_311_1349_0_idx_0 ; _rtB -> B_311_1352_0 [ 1 ] = _rtP ->
P_1518 * B_311_1349_0_idx_1 ; _rtB -> B_311_1352_0 [ 2 ] = _rtP -> P_1518 *
B_311_1349_0_idx_2 ; ssCallAccelRunBlock ( S , 44 , 0 , SS_CALL_MDL_OUTPUTS )
; _rtB -> B_311_1354_0 = ( muDoubleScalarAbs ( _rtB -> B_311_1352_0 [ 0 ] -
_rtB -> B_311_1338_0 [ 0 ] ) <= _rtB -> B_311_199_0 ) ; _rtB -> B_311_1355_0
= _rtDW -> UnitDelay_DSTATE_n ; _rtB -> B_311_1356_0 = _rtDW ->
UnitDelay_DSTATE_j ; _rtB -> B_311_1357_0 = ( muDoubleScalarAbs ( _rtB ->
B_311_1356_0 - _rtB -> B_311_1355_0 ) <= _rtB -> B_311_201_0_n ) ; _rtB ->
B_311_1358_0 = _rtDW -> UnitDelay1_DSTATE_jdb ; _rtB -> B_311_1359_0 = _rtDW
-> UnitDelay1_DSTATE_kc ; rtb_B_311_35_0 = _rtB -> B_311_1359_0 - _rtB ->
B_311_1358_0 ; _rtB -> B_311_1365_0 = ( ( real_T ) ( rtb_B_311_35_0 <= _rtB
-> B_311_204_0 ? _rtP -> P_4251 : 0U ) * 1.862645149230957E-9 +
rtb_B_311_35_0 ) - ( real_T ) ( rtb_B_311_35_0 >= _rtB -> B_311_205_0 ? _rtP
-> P_4252 : 0U ) * 1.862645149230957E-9 ; _rtB -> B_311_1367_0 = _rtP ->
P_1523 * muDoubleScalarAbs ( _rtB -> B_311_1365_0 ) ; _rtB -> B_311_1368_0 =
( _rtB -> B_311_1367_0 <= _rtB -> B_311_200_0_o ) ; _rtB -> B_311_1370_0 = (
_rtB -> B_44_0_1 || ( ( _rtB -> B_311_1354_0 != 0.0 ) && ( _rtB ->
B_311_1357_0 != 0.0 ) && ( _rtB -> B_311_1368_0 != 0.0 ) ) ) ; _rtB ->
B_311_1371_0 = ( _rtB -> B_311_203_0_a && ( _rtB -> B_311_1257_0 != 0.0 ) &&
_rtB -> B_311_1370_0 ) ; } i = ssIsSampleHit ( S , 5 , 0 ) ; if ( i != 0 ) {
_rtB -> B_311_1372_0 = ( ( _rtB -> B_311_197_0 != 0.0 ) || ( _rtB ->
B_155_5_1 != 0.0 ) ) ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { i
= ( int32_T ) ( ( ( ( ( uint32_T ) _rtB -> B_311_1371_0 << 1 ) + _rtB ->
B_311_1372_0 ) << 1 ) + _rtDW -> Memory_PreviousInput_mv ) ; _rtB ->
B_311_1374_0 [ 0U ] = _rtP -> P_4265 [ ( uint32_T ) i ] ; _rtB ->
B_311_1374_0 [ 1U ] = _rtP -> P_4265 [ i + 8U ] ; if ( _rtB -> B_311_1_0 >=
_rtP -> P_1526 ) { rtb_B_311_35_0 = _rtB -> B_311_1374_0 [ 0 ] ; } else {
rtb_B_311_35_0 = rt_Lookup ( _rtP -> P_1524 , 4 , ssGetTaskTime ( S , 2 ) ,
_rtP -> P_1525 ) ; } if ( rtb_B_311_1324_0 >= _rtP -> P_1527 ) { if ( _rtB ->
B_311_2_0 >= _rtP -> P_302 ) { _rtB -> B_311_1381_0 = rtb_B_311_35_0 ; } else
{ _rtB -> B_311_1381_0 = _rtB -> B_311_5_0 ; } } else { _rtB -> B_311_1381_0
= rt_Lookup ( _rtP -> P_300 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_301 ) ;
} if ( _rtP -> P_1528 >= _rtP -> P_1529 ) { if ( _rtB -> B_311_3_0 >= _rtP ->
P_305 ) { _rtB -> B_311_1389_0 = rtb_B_311_35_0 ; } else { _rtB ->
B_311_1389_0 = _rtB -> B_311_5_0 ; } } else { _rtB -> B_311_1389_0 =
rt_Lookup ( _rtP -> P_303 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_304 ) ; }
if ( _rtP -> P_1530 >= _rtP -> P_1531 ) { if ( _rtB -> B_311_4_0 >= _rtP ->
P_308 ) { _rtB -> B_311_1397_0 = rtb_B_311_35_0 ; } else { _rtB ->
B_311_1397_0 = _rtB -> B_311_5_0 ; } } else { _rtB -> B_311_1397_0 =
rt_Lookup ( _rtP -> P_306 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_307 ) ; }
if ( _rtB -> B_311_6_0 >= _rtP -> P_1535 ) { rtb_B_311_1324_0 = _rtB ->
B_311_1322_0 [ 0 ] ; } else { rtb_B_311_1324_0 = rt_Lookup ( _rtP -> P_1533 ,
4 , ssGetTaskTime ( S , 2 ) , _rtP -> P_1534 ) ; } if ( _rtP -> P_1532 >=
_rtP -> P_1536 ) { if ( _rtB -> B_311_7_0 >= _rtP -> P_320 ) { _rtB ->
B_311_1415_0 = rtb_B_311_1324_0 ; } else { _rtB -> B_311_1415_0 = _rtB ->
B_311_10_0 ; } } else { _rtB -> B_311_1415_0 = rt_Lookup ( _rtP -> P_318 , 5
, ssGetTaskTime ( S , 2 ) , _rtP -> P_319 ) ; } if ( _rtP -> P_1537 >= _rtP
-> P_1538 ) { if ( _rtB -> B_311_8_0 >= _rtP -> P_323 ) { _rtB ->
B_311_1423_0 = rtb_B_311_1324_0 ; } else { _rtB -> B_311_1423_0 = _rtB ->
B_311_10_0 ; } } else { _rtB -> B_311_1423_0 = rt_Lookup ( _rtP -> P_321 , 5
, ssGetTaskTime ( S , 2 ) , _rtP -> P_322 ) ; } if ( _rtP -> P_1539 >= _rtP
-> P_1540 ) { if ( _rtB -> B_311_9_0 >= _rtP -> P_326 ) { _rtB ->
B_311_1431_0 = rtb_B_311_1324_0 ; } else { _rtB -> B_311_1431_0 = _rtB ->
B_311_10_0 ; } } else { _rtB -> B_311_1431_0 = rt_Lookup ( _rtP -> P_324 , 5
, ssGetTaskTime ( S , 2 ) , _rtP -> P_325 ) ; } } i = ssIsSampleHit ( S , 1 ,
0 ) ; if ( i != 0 ) { _rtDW -> Step2_MODE = ( ssGetTaskTime ( S , 1 ) >= _rtP
-> P_1541 ) ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
rtb_B_311_1443_0 = _rtP -> P_1544 * _rtB -> B_311_152_0 [ 102 ] ; _rtB ->
B_311_1450_0 = _rtP -> P_1545 * _rtB -> B_311_152_0 [ 205 ] ; _rtB ->
B_311_1460_0 = ( ( real_T ) ( _rtB -> B_311_1450_0 <
Auto_microgrid_forecasting_020822_rtC ( S ) -> B_311_2_0 ) * _rtP -> P_1546 -
_rtB -> B_311_63_0 ) * muDoubleScalarAbs ( _rtB -> B_311_1450_0 ) * _rtP ->
P_1547 ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_311_1461_0 = _rtP -> P_1548 * rtb_B_311_47_0 ; } i = ssIsSampleHit ( S , 2
, 0 ) ; if ( i != 0 ) { _rtB -> B_311_1462_0 = _rtP -> P_1549 * _rtB ->
B_311_1450_0 ; _rtB -> B_311_1472_0 [ 0 ] = _rtP -> P_1550 * _rtB ->
B_311_152_0 [ 90 ] * _rtP -> P_1553 ; _rtB -> B_311_1472_0 [ 1 ] = _rtP ->
P_1551 * _rtB -> B_311_152_0 [ 91 ] * _rtP -> P_1553 ; _rtB -> B_311_1472_0 [
2 ] = _rtP -> P_1552 * _rtB -> B_311_152_0 [ 92 ] * _rtP -> P_1553 ; _rtB ->
B_311_1476_0 [ 0 ] = _rtP -> P_1554 * _rtB -> B_311_152_0 [ 173 ] * _rtP ->
P_1557 ; _rtB -> B_311_1476_0 [ 1 ] = _rtP -> P_1555 * _rtB -> B_311_152_0 [
174 ] * _rtP -> P_1557 ; _rtB -> B_311_1476_0 [ 2 ] = _rtP -> P_1556 * _rtB
-> B_311_152_0 [ 175 ] * _rtP -> P_1557 ; if ( rtb_B_311_1443_0 > _rtP ->
P_1558 ) { rtb_B_311_1443_0 = _rtP -> P_1558 ; } else { if ( rtb_B_311_1443_0
< _rtP -> P_1559 ) { rtb_B_311_1443_0 = _rtP -> P_1559 ; } } _rtB ->
B_311_1481_0 = ( ( 0.0 - ( ( _rtB -> B_311_1472_0 [ 0 ] * _rtB ->
B_311_1476_0 [ 0 ] + _rtB -> B_311_1472_0 [ 1 ] * _rtB -> B_311_1476_0 [ 1 ]
) + _rtB -> B_311_1472_0 [ 2 ] * _rtB -> B_311_1476_0 [ 2 ] ) ) - _rtB ->
B_311_30_0 ) / rtb_B_311_1443_0 ; i = ssIsSampleHit ( S , 5 , 0 ) ; if ( i !=
0 ) { _rtB -> B_311_1496_0 = _rtDW -> RateTransition8_Buffer0 ; } if ( _rtB
-> B_311_31_0_p >= _rtP -> P_1564 ) { rtb_B_311_35_0 = _rtB -> B_311_1496_0 ;
} else { rtb_B_311_35_0 = rt_Lookup ( _rtP -> P_1562 , 4 , ssGetTaskTime ( S
, 2 ) , _rtP -> P_1563 ) ; } if ( _rtP -> P_1560 >= _rtP -> P_1565 ) { if (
_rtB -> B_311_32_0 >= _rtP -> P_76 ) { _rtB -> B_311_1503_0 = rtb_B_311_35_0
; } else { _rtB -> B_311_1503_0 = _rtB -> B_311_35_0 ; } } else { _rtB ->
B_311_1503_0 = rt_Lookup ( _rtP -> P_74 , 5 , ssGetTaskTime ( S , 2 ) , _rtP
-> P_75 ) ; } if ( _rtP -> P_1566 >= _rtP -> P_1567 ) { if ( _rtB ->
B_311_33_0 >= _rtP -> P_79 ) { _rtB -> B_311_1511_0 = rtb_B_311_35_0 ; } else
{ _rtB -> B_311_1511_0 = _rtB -> B_311_35_0 ; } } else { _rtB -> B_311_1511_0
= rt_Lookup ( _rtP -> P_77 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_78 ) ; }
if ( _rtP -> P_1568 >= _rtP -> P_1569 ) { if ( _rtB -> B_311_34_0_c >= _rtP
-> P_82 ) { _rtB -> B_311_1519_0 = rtb_B_311_35_0 ; } else { _rtB ->
B_311_1519_0 = _rtB -> B_311_35_0 ; } } else { _rtB -> B_311_1519_0 =
rt_Lookup ( _rtP -> P_80 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_81 ) ; } }
i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_1530_0 [ 0 ]
= _rtDW -> UnitDelay_DSTATE_be [ 0 ] ; B_311_1531_0_idx_0 = _rtDW ->
UnitDelay_DSTATE_p4 [ 0 ] ; _rtB -> B_311_1530_0 [ 1 ] = _rtDW ->
UnitDelay_DSTATE_be [ 1 ] ; B_311_1531_0_idx_1 = _rtDW -> UnitDelay_DSTATE_p4
[ 1 ] ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB ->
B_311_1535_0 [ 0 ] = _rtP -> P_1572 * _rtB -> B_311_152_0 [ 93 ] * _rtP ->
P_1575 ; _rtB -> B_311_1535_0 [ 1 ] = _rtP -> P_1573 * _rtB -> B_311_152_0 [
94 ] * _rtP -> P_1575 ; _rtB -> B_311_1535_0 [ 2 ] = _rtP -> P_1574 * _rtB ->
B_311_152_0 [ 95 ] * _rtP -> P_1575 ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if
( i != 0 ) { B_311_1536_0_idx_0 = _rtP -> P_1576 * _rtB -> B_311_1535_0 [ 0 ]
; B_311_1536_0_idx_1 = _rtP -> P_1576 * _rtB -> B_311_1535_0 [ 1 ] ;
B_311_1536_0_idx_2 = _rtP -> P_1576 * _rtB -> B_311_1535_0 [ 2 ] ;
rtb_B_311_1540_0 = ( 0.0 - _rtDW -> UnitDelay2_DSTATE_pk [ 0 ] * _rtB ->
B_311_62_0 ) - _rtDW -> UnitDelay2_DSTATE_pk [ 1 ] * _rtB -> B_311_63_0_p ;
rtb_B_311_1543_0 = _rtDW -> UnitDelay2_DSTATE_pk [ 0 ] * _rtB -> B_311_63_0_p
- _rtDW -> UnitDelay2_DSTATE_pk [ 1 ] * _rtB -> B_311_62_0 ; rtb_B_311_1544_0
= ( 0.0 - rtb_B_311_1540_0 ) - _rtDW -> UnitDelay2_DSTATE_pk [ 0 ] ;
rtb_B_311_1545_0 = ( 0.0 - rtb_B_311_1543_0 ) - _rtDW -> UnitDelay2_DSTATE_pk
[ 1 ] ; _rtB -> B_311_1546_0 = ( ( B_311_1536_0_idx_0 * _rtDW ->
UnitDelay2_DSTATE_pk [ 0 ] + B_311_1536_0_idx_1 * rtb_B_311_1540_0 ) +
B_311_1536_0_idx_2 * rtb_B_311_1544_0 ) * 0.66666666666666663 ; _rtB ->
B_311_1547_0 = ( ( B_311_1536_0_idx_0 * _rtDW -> UnitDelay2_DSTATE_pk [ 1 ] +
B_311_1536_0_idx_1 * rtb_B_311_1543_0 ) + B_311_1536_0_idx_2 *
rtb_B_311_1545_0 ) * 0.66666666666666663 ; if ( _rtP -> P_4288 == 1 ) { _rtB
-> B_311_1548_0 [ 0 ] = B_311_1531_0_idx_0 ; _rtB -> B_311_1548_0 [ 1 ] =
B_311_1531_0_idx_1 ; } else { _rtB -> B_311_1548_0 [ 0 ] = _rtB ->
B_311_1546_0 ; _rtB -> B_311_1548_0 [ 1 ] = _rtB -> B_311_1547_0 ; } _rtB ->
B_311_1553_0 = ( ( _rtB -> B_311_1496_0 != 0.0 ) && ( ! ( _rtB -> B_155_5_6
!= 0.0 ) ) ) ; rtb_B_311_44_0 = ! _rtB -> B_311_1553_0 ; _rtB -> B_311_1555_0
= ! rtb_B_311_44_0 ; B_311_1537_0_idx_0 = _rtDW -> UnitDelay2_DSTATE_pk [ 0 ]
; B_311_1549_0_idx_0 = _rtB -> B_311_1530_0 [ 0 ] - _rtB -> B_311_1548_0 [ 0
] ; B_311_1556_0_idx_0 = _rtDW -> UnitDelay1_DSTATE_hn [ 0 ] ;
B_311_1537_0_idx_1 = _rtDW -> UnitDelay2_DSTATE_pk [ 1 ] ; B_311_1549_0_idx_1
= _rtB -> B_311_1530_0 [ 1 ] - _rtB -> B_311_1548_0 [ 1 ] ;
B_311_1556_0_idx_1 = _rtDW -> UnitDelay1_DSTATE_hn [ 1 ] ; rtb_B_311_1550_0 =
_rtB -> B_311_41_0_g * B_311_1549_0_idx_0 ; _rtB -> B_311_1551_0 =
B_311_1549_0_idx_0 / _rtB -> B_311_43_0_g ; _rtB -> B_311_1557_0 = _rtDW ->
UnitDelay1_DSTATE_hn [ 0 ] - rtb_B_311_1550_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_IC_LOADING != 0 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_e = _rtB -> B_311_1557_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_e >= _rtP -> P_1580 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_e = _rtP -> P_1580 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_e <= _rtP -> P_1581 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_e = _rtP -> P_1581 ; } } _rtB -> B_311_1558_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_e ; } else if ( _rtB -> B_311_1555_0
|| ( _rtDW -> DiscreteTimeIntegrator_PrevResetState != 0 ) ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_e = _rtB -> B_311_1557_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_e >= _rtP -> P_1580 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_e = _rtP -> P_1580 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_e <= _rtP -> P_1581 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_e = _rtP -> P_1581 ; } } _rtB -> B_311_1558_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_e ; } else { _rtB -> B_311_1558_0 =
_rtP -> P_1579 * _rtB -> B_311_1551_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_e ; } if ( _rtB -> B_311_1558_0 >= _rtP ->
P_1580 ) { _rtB -> B_311_1558_0 = _rtP -> P_1580 ; } else { if ( _rtB ->
B_311_1558_0 <= _rtP -> P_1581 ) { _rtB -> B_311_1558_0 = _rtP -> P_1581 ; }
} rtb_B_311_35_0 = rtb_B_311_1550_0 + _rtB -> B_311_1558_0 ; if (
rtb_B_311_35_0 > _rtP -> P_1582 ) { _rtB -> B_311_1560_0 = _rtP -> P_1582 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_1583 ) { _rtB -> B_311_1560_0 = _rtP ->
P_1583 ; } else { _rtB -> B_311_1560_0 = rtb_B_311_35_0 ; } rtb_B_311_1550_0
= _rtB -> B_311_44_0 * B_311_1549_0_idx_1 ; _rtB -> B_311_1562_0 =
B_311_1549_0_idx_1 / _rtB -> B_311_46_0_m ; _rtB -> B_311_1563_0 = !
rtb_B_311_44_0 ; _rtB -> B_311_1564_0 = _rtDW -> UnitDelay1_DSTATE_hn [ 1 ] -
rtb_B_311_1550_0 ; if ( _rtDW -> DiscreteTimeIntegrator_IC_LOADING_f != 0 ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE_m = _rtB -> B_311_1564_0 ; if ( _rtDW
-> DiscreteTimeIntegrator_DSTATE_m >= _rtP -> P_1585 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_m = _rtP -> P_1585 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_m <= _rtP -> P_1586 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_m = _rtP -> P_1586 ; } } _rtB -> B_311_1565_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_m ; } else if ( _rtB -> B_311_1563_0
|| ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_n != 0 ) ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_m = _rtB -> B_311_1564_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_m >= _rtP -> P_1585 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_m = _rtP -> P_1585 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_m <= _rtP -> P_1586 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_m = _rtP -> P_1586 ; } } _rtB -> B_311_1565_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_m ; } else { _rtB -> B_311_1565_0 =
_rtP -> P_1584 * _rtB -> B_311_1562_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_m ; } if ( _rtB -> B_311_1565_0 >= _rtP ->
P_1585 ) { _rtB -> B_311_1565_0 = _rtP -> P_1585 ; } else { if ( _rtB ->
B_311_1565_0 <= _rtP -> P_1586 ) { _rtB -> B_311_1565_0 = _rtP -> P_1586 ; }
} rtb_B_311_35_0 = rtb_B_311_1550_0 + _rtB -> B_311_1565_0 ; if (
rtb_B_311_35_0 > _rtP -> P_1587 ) { _rtB -> B_311_1567_0 = _rtP -> P_1587 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_1588 ) { _rtB -> B_311_1567_0 = _rtP ->
P_1588 ; } else { _rtB -> B_311_1567_0 = rtb_B_311_35_0 ; }
ssCallAccelRunBlock ( S , 311 , 1568 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_311_1570_0 = _rtP -> P_1590 * _rtDW -> UnitDelay_DSTATE_m ; _rtB ->
B_311_1571_0 [ 0 ] = _rtP -> P_1591 * _rtB -> B_311_1472_0 [ 0 ] ; _rtB ->
B_311_1571_0 [ 1 ] = _rtP -> P_1591 * _rtB -> B_311_1472_0 [ 1 ] ; _rtB ->
B_311_1571_0 [ 2 ] = _rtP -> P_1591 * _rtB -> B_311_1472_0 [ 2 ] ;
B_311_1549_0_idx_0 = _rtDW -> UnitDelay_DSTATE_aw [ 0 ] ; B_311_1549_0_idx_1
= _rtDW -> UnitDelay_DSTATE_aw [ 1 ] ; rtb_B_311_1550_0 = ( 0.0 - _rtDW ->
UnitDelay_DSTATE_aw [ 0 ] * _rtB -> B_311_58_0 ) - _rtDW ->
UnitDelay_DSTATE_aw [ 1 ] * _rtB -> B_311_59_0 ; rtb_B_311_1578_0 = _rtDW ->
UnitDelay_DSTATE_aw [ 0 ] * _rtB -> B_311_59_0 - _rtDW -> UnitDelay_DSTATE_aw
[ 1 ] * _rtB -> B_311_58_0 ; rtb_B_311_1579_0 = ( 0.0 - rtb_B_311_1550_0 ) -
_rtDW -> UnitDelay_DSTATE_aw [ 0 ] ; rtb_B_311_1580_0 = ( 0.0 -
rtb_B_311_1578_0 ) - _rtDW -> UnitDelay_DSTATE_aw [ 1 ] ; _rtB ->
B_311_1581_0 = ( ( _rtB -> B_311_1571_0 [ 0 ] * _rtDW -> UnitDelay_DSTATE_aw
[ 0 ] + _rtB -> B_311_1571_0 [ 1 ] * rtb_B_311_1550_0 ) + _rtB ->
B_311_1571_0 [ 2 ] * rtb_B_311_1579_0 ) * 0.66666666666666663 ; {
B_311_1582_0 = ( _rtP -> P_1595 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE
[ 0 ] + ( _rtP -> P_1595 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE [ 1 ] ;
B_311_1582_0 += _rtP -> P_1596 * _rtB -> B_311_1581_0 ; } _rtB ->
B_311_1583_0 = ( ( _rtB -> B_311_1571_0 [ 0 ] * B_311_1549_0_idx_1 + _rtB ->
B_311_1571_0 [ 1 ] * rtb_B_311_1578_0 ) + _rtB -> B_311_1571_0 [ 2 ] *
rtb_B_311_1580_0 ) * 0.66666666666666663 ; { B_311_1584_0 = ( _rtP -> P_1600
[ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_c [ 0 ] + ( _rtP -> P_1600 [ 1 ]
) * _rtDW -> DiscreteStateSpace_DSTATE_c [ 1 ] ; B_311_1584_0 += _rtP ->
P_1601 * _rtB -> B_311_1583_0 ; } _rtB -> B_311_1585_0 [ 0 ] = B_311_1582_0 ;
_rtB -> B_311_1585_0 [ 1 ] = B_311_1584_0 ; rtb_B_311_1587_0 = _rtB ->
B_311_1585_0 [ 0 ] * _rtB -> B_311_1585_0 [ 0 ] + _rtB -> B_311_1585_0 [ 1 ]
* _rtB -> B_311_1585_0 [ 1 ] ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if (
_rtDW -> Sqrt_DWORK1 != 0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; _rtDW -> Sqrt_DWORK1 = 0 ; } _rtB -> B_311_1588_0 = muDoubleScalarSqrt (
rtb_B_311_1587_0 ) ; } else if ( rtb_B_311_1587_0 < 0.0 ) { _rtB ->
B_311_1588_0 = - muDoubleScalarSqrt ( muDoubleScalarAbs ( rtb_B_311_1587_0 )
) ; _rtDW -> Sqrt_DWORK1 = 1 ; } else { _rtB -> B_311_1588_0 =
muDoubleScalarSqrt ( rtb_B_311_1587_0 ) ; } _rtB -> B_311_1589_0 [ 0 ] = _rtP
-> P_1603 * _rtB -> B_311_1476_0 [ 0 ] ; _rtB -> B_311_1589_0 [ 1 ] = _rtP ->
P_1603 * _rtB -> B_311_1476_0 [ 1 ] ; _rtB -> B_311_1589_0 [ 2 ] = _rtP ->
P_1603 * _rtB -> B_311_1476_0 [ 2 ] ; B_311_1586_0_idx_0 = _rtDW ->
UnitDelay1_DSTATE_gw [ 0 ] ; B_311_1586_0_idx_1 = _rtDW ->
UnitDelay1_DSTATE_gw [ 1 ] ; rtb_B_311_1587_0 = ( 0.0 - _rtDW ->
UnitDelay1_DSTATE_gw [ 0 ] * _rtB -> B_311_60_0 ) - _rtDW ->
UnitDelay1_DSTATE_gw [ 1 ] * _rtB -> B_311_61_0 ; rtb_B_311_1596_0 = _rtDW ->
UnitDelay1_DSTATE_gw [ 0 ] * _rtB -> B_311_61_0 - _rtDW ->
UnitDelay1_DSTATE_gw [ 1 ] * _rtB -> B_311_60_0 ; rtb_B_311_1597_0 = ( 0.0 -
rtb_B_311_1587_0 ) - _rtDW -> UnitDelay1_DSTATE_gw [ 0 ] ; rtb_B_311_1598_0 =
( 0.0 - rtb_B_311_1596_0 ) - _rtDW -> UnitDelay1_DSTATE_gw [ 1 ] ; _rtB ->
B_311_1599_0 = ( ( _rtB -> B_311_1589_0 [ 0 ] * _rtDW -> UnitDelay1_DSTATE_gw
[ 0 ] + _rtB -> B_311_1589_0 [ 1 ] * rtb_B_311_1587_0 ) + _rtB ->
B_311_1589_0 [ 2 ] * rtb_B_311_1597_0 ) * 0.66666666666666663 ; _rtB ->
B_311_1600_0 = ( ( _rtB -> B_311_1589_0 [ 0 ] * _rtDW -> UnitDelay1_DSTATE_gw
[ 1 ] + _rtB -> B_311_1589_0 [ 1 ] * rtb_B_311_1596_0 ) + _rtB ->
B_311_1589_0 [ 2 ] * rtb_B_311_1598_0 ) * 0.66666666666666663 ; _rtB ->
B_311_1601_0 [ 0 ] = _rtP -> P_1605 [ 0 ] * _rtB -> B_311_1599_0 ; _rtB ->
B_311_1601_0 [ 1 ] = _rtP -> P_1605 [ 1 ] * _rtB -> B_311_1600_0 ; } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_1604_0 = ( _rtB
-> B_311_1374_0 [ 0 ] || ( ! ( _rtB -> B_155_5_8 != 0.0 ) ) ) ; if ( _rtB ->
B_311_1604_0 || ( _rtDW -> DiscreteTimeIntegrator1_PrevResetState != 0 ) ) {
_rtDW -> DiscreteTimeIntegrator1_DSTATE = _rtP -> P_1607 ; } if ( _rtB ->
B_155_5_8 > _rtP -> P_1608 ) { _rtB -> B_311_1606_0 = _rtDW ->
DiscreteTimeIntegrator1_DSTATE ; } else { _rtB -> B_311_1606_0 = _rtB ->
B_156_13_0 ; } } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtB ->
B_311_1607_0 [ 0 ] = _rtB -> B_311_36_0 - _rtB -> B_311_1570_0 ; _rtB ->
B_311_1607_0 [ 1 ] = _rtB -> B_311_1606_0 - _rtB -> B_311_1588_0 ; _rtB ->
B_311_1607_0 [ 2 ] = _rtB -> B_311_69_0 [ 0 ] - _rtB -> B_311_1601_0 [ 0 ] ;
_rtB -> B_311_1607_0 [ 3 ] = _rtB -> B_311_69_0 [ 1 ] - _rtB -> B_311_1601_0
[ 1 ] ; { B_311_1608_0 = ( _rtP -> P_1611 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_f [ 0 ] + ( _rtP -> P_1611 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_f [ 1 ] ; B_311_1608_0 += _rtP -> P_1612 * _rtB ->
B_311_1607_0 [ 2 ] ; } rtb_B_311_1443_0 = B_311_1608_0 * _rtB -> B_311_37_0 ;
{ B_311_1610_0 = ( _rtP -> P_1616 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_g [ 0 ] + ( _rtP -> P_1616 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_g [ 1 ] ; B_311_1610_0 += _rtP -> P_1617 * _rtB ->
B_311_1607_0 [ 3 ] ; } rtb_B_311_35_0 = B_311_1610_0 * _rtB -> B_311_38_0_f ;
if ( B_311_1610_0 > _rtP -> P_1620 ) { rtb_B_299_3_0 = B_311_1610_0 - _rtP ->
P_1620 ; } else if ( B_311_1610_0 >= _rtP -> P_1619 ) { rtb_B_299_3_0 = 0.0 ;
} else { rtb_B_299_3_0 = B_311_1610_0 - _rtP -> P_1619 ; } rtb_B_311_47_0 =
rtb_B_299_3_0 * _rtB -> B_311_70_0_l ; { B_311_1615_0 = _rtP -> P_1623 *
_rtDW -> DiscreteStateSpace_DSTATE_p ; B_311_1615_0 += _rtP -> P_1624 * _rtB
-> B_311_1607_0 [ 0 ] ; } rtb_B_311_1324_0 = B_311_1615_0 * _rtB ->
B_311_37_0 ; { B_311_1617_0 = _rtP -> P_1628 * _rtDW ->
DiscreteStateSpace_DSTATE_ce ; B_311_1617_0 += _rtP -> P_1629 * _rtB ->
B_311_1607_0 [ 1 ] ; } _rtB -> B_311_1619_0 [ 0 ] = rtb_B_311_1443_0 + _rtB
-> B_311_36_0 ; _rtB -> B_311_1619_0 [ 1 ] = ( rtb_B_311_35_0 +
rtb_B_311_47_0 ) + _rtB -> B_311_1606_0 ; _rtB -> B_311_1619_0 [ 2 ] =
rtb_B_311_1324_0 + _rtB -> B_311_1261_0 [ 0 ] ; _rtB -> B_311_1619_0 [ 3 ] =
B_311_1617_0 * _rtB -> B_311_38_0_f + _rtB -> B_311_1261_0 [ 1 ] ; if ( _rtB
-> B_311_1619_0 [ 1 ] > _rtP -> P_1631 ) { _rtB -> B_311_1620_0 = _rtP ->
P_1631 ; } else if ( _rtB -> B_311_1619_0 [ 1 ] < _rtP -> P_1632 ) { _rtB ->
B_311_1620_0 = _rtP -> P_1632 ; } else { _rtB -> B_311_1620_0 = _rtB ->
B_311_1619_0 [ 1 ] ; } if ( _rtB -> B_311_1553_0 ) { _rtB -> B_311_1622_0 [ 0
] = B_311_1556_0_idx_0 ; _rtB -> B_311_1622_0 [ 1 ] = B_311_1556_0_idx_1 ; }
else { B_311_1556_0_idx_0 = _rtB -> B_311_1620_0 - _rtB -> B_311_1530_0 [ 0 ]
; B_311_1556_0_idx_1 = _rtB -> B_311_39_0 - _rtB -> B_311_1530_0 [ 1 ] ; if (
B_311_1556_0_idx_0 > _rtB -> B_311_47_0 [ 0 ] ) { B_311_1556_0_idx_0 = _rtB
-> B_311_47_0 [ 0 ] ; } else { if ( B_311_1556_0_idx_0 < _rtB -> B_311_48_0 [
0 ] ) { B_311_1556_0_idx_0 = _rtB -> B_311_48_0 [ 0 ] ; } } _rtB ->
B_311_1622_0 [ 0 ] = B_311_1556_0_idx_0 + _rtB -> B_311_1530_0 [ 0 ] ; if (
B_311_1556_0_idx_1 > _rtB -> B_311_47_0 [ 1 ] ) { B_311_1556_0_idx_1 = _rtB
-> B_311_47_0 [ 1 ] ; } else { if ( B_311_1556_0_idx_1 < _rtB -> B_311_48_0 [
1 ] ) { B_311_1556_0_idx_1 = _rtB -> B_311_48_0 [ 1 ] ; } } _rtB ->
B_311_1622_0 [ 1 ] = B_311_1556_0_idx_1 + _rtB -> B_311_1530_0 [ 1 ] ; }
B_311_1556_0_idx_0 = _rtDW -> UnitDelay_DSTATE_nw [ 0 ] ; B_311_1556_0_idx_1
= _rtDW -> UnitDelay_DSTATE_nw [ 1 ] ; { B_311_1624_0 = ( _rtP -> P_1636 [ 0
] ) * _rtDW -> DiscreteStateSpace_DSTATE_k [ 0 ] + ( _rtP -> P_1636 [ 1 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_k [ 1 ] ; B_311_1624_0 += _rtP -> P_1637 *
_rtB -> B_311_1599_0 ; } { B_311_1625_0 = ( _rtP -> P_1641 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_j [ 0 ] + ( _rtP -> P_1641 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_j [ 1 ] ; B_311_1625_0 += _rtP -> P_1642 * _rtB ->
B_311_1600_0 ; } B_311_1531_0_idx_0 = B_311_1556_0_idx_0 - B_311_1624_0 ;
B_311_1531_0_idx_1 = B_311_1556_0_idx_1 - B_311_1625_0 ; rtb_B_311_1443_0 =
_rtB -> B_311_50_0 * B_311_1531_0_idx_0 ; _rtB -> B_311_1628_0 =
B_311_1531_0_idx_0 / _rtB -> B_311_52_0 ; _rtB -> B_311_1629_0 = ! _rtB ->
B_311_1553_0 ; rtb_B_311_50_0 = _rtDW -> UnitDelay_DSTATE_ay [ 0 ] ;
rtb_B_299_3_0 = _rtDW -> UnitDelay_DSTATE_ay [ 1 ] ; { B_311_1631_0 = ( _rtP
-> P_1647 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_ph [ 0 ] + ( _rtP ->
P_1647 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_ph [ 1 ] ; B_311_1631_0
+= _rtP -> P_1648 * _rtB -> B_311_1581_0 ; } { B_311_1632_0 = ( _rtP ->
P_1652 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_d [ 0 ] + ( _rtP ->
P_1652 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_d [ 1 ] ; B_311_1632_0 +=
_rtP -> P_1653 * _rtB -> B_311_1583_0 ; } _rtB -> B_311_1635_0 = (
rtb_B_311_50_0 - _rtP -> P_1655 * B_311_1631_0 ) - rtb_B_311_1443_0 ; if (
_rtDW -> DiscreteTimeIntegrator_IC_LOADING_n != 0 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_p = _rtB -> B_311_1635_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_p >= _rtP -> P_1657 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_p = _rtP -> P_1657 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_p <= _rtP -> P_1658 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_p = _rtP -> P_1658 ; } } _rtB -> B_311_1636_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_p ; } else if ( _rtB -> B_311_1629_0
|| ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_nq != 0 ) ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_p = _rtB -> B_311_1635_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_p >= _rtP -> P_1657 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_p = _rtP -> P_1657 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_p <= _rtP -> P_1658 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_p = _rtP -> P_1658 ; } } _rtB -> B_311_1636_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_p ; } else { _rtB -> B_311_1636_0 =
_rtP -> P_1656 * _rtB -> B_311_1628_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_p ; } if ( _rtB -> B_311_1636_0 >= _rtP ->
P_1657 ) { _rtB -> B_311_1636_0 = _rtP -> P_1657 ; } else { if ( _rtB ->
B_311_1636_0 <= _rtP -> P_1658 ) { _rtB -> B_311_1636_0 = _rtP -> P_1658 ; }
} rtb_B_311_35_0 = rtb_B_311_1443_0 + _rtB -> B_311_1636_0 ; if (
rtb_B_311_35_0 > _rtP -> P_1659 ) { _rtB -> B_311_1638_0 = _rtP -> P_1659 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_1660 ) { _rtB -> B_311_1638_0 = _rtP ->
P_1660 ; } else { _rtB -> B_311_1638_0 = rtb_B_311_35_0 ; } rtb_B_311_1443_0
= _rtB -> B_311_53_0 * B_311_1531_0_idx_1 ; _rtB -> B_311_1640_0 =
B_311_1531_0_idx_1 / _rtB -> B_311_55_0 ; _rtB -> B_311_1641_0 = ! _rtB ->
B_311_1553_0 ; _rtB -> B_311_1642_0 = ( rtb_B_299_3_0 - _rtP -> P_1655 *
B_311_1632_0 ) - rtb_B_311_1443_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_IC_LOADING_p != 0 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_n = _rtB -> B_311_1642_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_n >= _rtP -> P_1662 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_n = _rtP -> P_1662 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_n <= _rtP -> P_1663 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_n = _rtP -> P_1663 ; } } _rtB -> B_311_1643_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_n ; } else if ( _rtB -> B_311_1641_0
|| ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_i != 0 ) ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_n = _rtB -> B_311_1642_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_n >= _rtP -> P_1662 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_n = _rtP -> P_1662 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_n <= _rtP -> P_1663 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_n = _rtP -> P_1663 ; } } _rtB -> B_311_1643_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_n ; } else { _rtB -> B_311_1643_0 =
_rtP -> P_1661 * _rtB -> B_311_1640_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_n ; } if ( _rtB -> B_311_1643_0 >= _rtP ->
P_1662 ) { _rtB -> B_311_1643_0 = _rtP -> P_1662 ; } else { if ( _rtB ->
B_311_1643_0 <= _rtP -> P_1663 ) { _rtB -> B_311_1643_0 = _rtP -> P_1663 ; }
} rtb_B_311_35_0 = rtb_B_311_1443_0 + _rtB -> B_311_1643_0 ; if (
rtb_B_311_35_0 > _rtP -> P_1664 ) { _rtB -> B_311_1645_0 = _rtP -> P_1664 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_1665 ) { _rtB -> B_311_1645_0 = _rtP ->
P_1665 ; } else { _rtB -> B_311_1645_0 = rtb_B_311_35_0 ; } _rtB ->
B_311_1647_0 [ 0 ] = _rtP -> P_1666 * B_311_1631_0 + _rtB -> B_311_1638_0 ;
_rtB -> B_311_1647_0 [ 1 ] = _rtP -> P_1666 * B_311_1632_0 + _rtB ->
B_311_1645_0 ; ssCallAccelRunBlock ( S , 311 , 1648 , SS_CALL_MDL_OUTPUTS ) ;
if ( _rtB -> B_311_1619_0 [ 2 ] > _rtB -> B_311_85_0 ) { rtb_B_311_1443_0 =
_rtB -> B_311_85_0 ; } else if ( _rtB -> B_311_1619_0 [ 2 ] < _rtB ->
B_311_86_0 ) { rtb_B_311_1443_0 = _rtB -> B_311_86_0 ; } else {
rtb_B_311_1443_0 = _rtB -> B_311_1619_0 [ 2 ] ; } rtb_B_311_47_0 =
rtb_B_311_1443_0 ; rtb_B_311_1443_0 = muDoubleScalarAtan2 ( _rtB ->
B_311_1619_0 [ 3 ] , rtb_B_311_1443_0 ) ; rtb_B_311_35_0 =
muDoubleScalarHypot ( rtb_B_311_47_0 , _rtB -> B_311_1619_0 [ 3 ] ) ;
rtb_B_311_47_0 = muDoubleScalarHypot ( rtb_B_311_47_0 , _rtB -> B_311_1619_0
[ 3 ] ) ; if ( rtb_B_311_35_0 > _rtP -> P_1667 ) { rtb_B_311_35_0 = _rtP ->
P_1667 ; } else { if ( rtb_B_311_35_0 < _rtP -> P_1668 ) { rtb_B_311_35_0 =
_rtP -> P_1668 ; } } _rtB -> B_311_1657_0 = rtb_B_311_35_0 *
muDoubleScalarCos ( rtb_B_311_1443_0 ) ; if ( rtb_B_311_47_0 > _rtP -> P_1667
) { rtb_B_311_47_0 = _rtP -> P_1667 ; } else { if ( rtb_B_311_47_0 < _rtP ->
P_1668 ) { rtb_B_311_47_0 = _rtP -> P_1668 ; } } _rtB -> B_311_1657_1 =
rtb_B_311_47_0 * muDoubleScalarSin ( rtb_B_311_1443_0 ) ; if ( ! _rtB ->
B_311_1553_0 ) { _rtB -> B_311_1659_0 [ 0 ] = _rtB -> B_311_1599_0 ; _rtB ->
B_311_1659_0 [ 1 ] = _rtB -> B_311_1600_0 ; } else { B_311_1531_0_idx_0 =
_rtB -> B_311_1657_0 - B_311_1556_0_idx_0 ; B_311_1531_0_idx_1 = _rtB ->
B_311_1657_1 - B_311_1556_0_idx_1 ; if ( B_311_1531_0_idx_0 > _rtB ->
B_311_56_0 [ 0 ] ) { B_311_1531_0_idx_0 = _rtB -> B_311_56_0 [ 0 ] ; } else {
if ( B_311_1531_0_idx_0 < _rtB -> B_311_57_0_n [ 0 ] ) { B_311_1531_0_idx_0 =
_rtB -> B_311_57_0_n [ 0 ] ; } } _rtB -> B_311_1659_0 [ 0 ] =
B_311_1531_0_idx_0 + B_311_1556_0_idx_0 ; if ( B_311_1531_0_idx_1 > _rtB ->
B_311_56_0 [ 1 ] ) { B_311_1531_0_idx_1 = _rtB -> B_311_56_0 [ 1 ] ; } else {
if ( B_311_1531_0_idx_1 < _rtB -> B_311_57_0_n [ 1 ] ) { B_311_1531_0_idx_1 =
_rtB -> B_311_57_0_n [ 1 ] ; } } _rtB -> B_311_1659_0 [ 1 ] =
B_311_1531_0_idx_1 + B_311_1556_0_idx_1 ; } tmpForInput [ 0 ] = _rtB ->
B_311_1571_0 [ 0 ] ; tmpForInput [ 1 ] = _rtB -> B_311_1571_0 [ 1 ] ;
tmpForInput [ 2 ] = _rtB -> B_311_1571_0 [ 2 ] ; tmpForInput [ 3 ] =
B_311_1549_0_idx_0 ; tmpForInput [ 4 ] = B_311_1549_0_idx_1 ; tmpForInput [ 5
] = rtb_B_311_1550_0 ; tmpForInput [ 6 ] = rtb_B_311_1578_0 ; tmpForInput [ 7
] = rtb_B_311_1579_0 ; tmpForInput [ 8 ] = rtb_B_311_1580_0 ;
rtb_B_311_1443_0 = - 0.0 ; for ( i = 0 ; i < 9 ; i ++ ) { rtb_B_311_1443_0 +=
tmpForInput [ i ] ; } _rtB -> B_311_1661_0 = _rtP -> P_1669 *
rtb_B_311_1443_0 ; tmpForInput [ 0 ] = _rtB -> B_311_1589_0 [ 0 ] ;
tmpForInput [ 1 ] = _rtB -> B_311_1589_0 [ 1 ] ; tmpForInput [ 2 ] = _rtB ->
B_311_1589_0 [ 2 ] ; tmpForInput [ 3 ] = B_311_1586_0_idx_0 ; tmpForInput [ 4
] = B_311_1586_0_idx_1 ; tmpForInput [ 5 ] = rtb_B_311_1587_0 ; tmpForInput [
6 ] = rtb_B_311_1596_0 ; tmpForInput [ 7 ] = rtb_B_311_1597_0 ; tmpForInput [
8 ] = rtb_B_311_1598_0 ; rtb_B_311_1587_0 = - 0.0 ; for ( i = 0 ; i < 9 ; i
++ ) { rtb_B_311_1587_0 += tmpForInput [ i ] ; } _rtB -> B_311_1663_0 = _rtP
-> P_1670 * rtb_B_311_1587_0 ; { _rtB -> B_311_1664_0 = ( _rtP -> P_1673 [ 0
] ) * _rtDW -> DiscreteStateSpace_DSTATE_g1 [ 0 ] + ( _rtP -> P_1673 [ 1 ] )
* _rtDW -> DiscreteStateSpace_DSTATE_g1 [ 1 ] ; _rtB -> B_311_1664_0 += _rtP
-> P_1674 * _rtB -> B_311_1599_0 ; } { _rtB -> B_311_1665_0 = ( _rtP ->
P_1678 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_b [ 0 ] + ( _rtP ->
P_1678 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_b [ 1 ] ; _rtB ->
B_311_1665_0 += _rtP -> P_1679 * _rtB -> B_311_1600_0 ; } _rtB ->
B_311_1667_0 = ( _rtB -> B_311_1571_0 [ 0 ] * _rtB -> B_311_1589_0 [ 0 ] +
_rtB -> B_311_1571_0 [ 1 ] * _rtB -> B_311_1589_0 [ 1 ] ) + _rtB ->
B_311_1571_0 [ 2 ] * _rtB -> B_311_1589_0 [ 2 ] ; _rtB -> B_311_1673_0 = ( (
( _rtB -> B_311_1571_0 [ 1 ] - _rtB -> B_311_1571_0 [ 2 ] ) * _rtB ->
B_311_1589_0 [ 0 ] + ( _rtB -> B_311_1571_0 [ 2 ] - _rtB -> B_311_1571_0 [ 0
] ) * _rtB -> B_311_1589_0 [ 1 ] ) + ( _rtB -> B_311_1571_0 [ 0 ] - _rtB ->
B_311_1571_0 [ 1 ] ) * _rtB -> B_311_1589_0 [ 2 ] ) * _rtP -> P_1681 ; if (
_rtDW -> Integ4_SYSTEM_ENABLE_i != 0 ) { _rtB -> B_311_1674_0 [ 0 ] = _rtDW
-> Integ4_DSTATE_p [ 0 ] ; _rtB -> B_311_1674_0 [ 1 ] = _rtDW ->
Integ4_DSTATE_p [ 1 ] ; } else { _rtB -> B_311_1674_0 [ 0 ] = _rtP -> P_1682
* _rtB -> B_311_1667_0 + _rtDW -> Integ4_DSTATE_p [ 0 ] ; _rtB ->
B_311_1674_0 [ 1 ] = _rtP -> P_1682 * _rtB -> B_311_1673_0 + _rtDW ->
Integ4_DSTATE_p [ 1 ] ; } _rtB -> B_311_1675_0 = _rtP -> P_1684 ;
ssCallAccelRunBlock ( S , 311 , 1676 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 3 ) < _rtP -> P_1687 ) { rtb_B_299_3_0 = _rtP -> P_1688 ;
} else { rtb_B_299_3_0 = _rtP -> P_1689 ; } if ( rtb_B_299_3_0 >= _rtP ->
P_1691 ) { B_311_1549_0_idx_0 = ( _rtB -> B_311_1674_0 [ 0 ] - _rtB ->
B_311_1676_0 [ 0 ] ) * _rtP -> P_1685 + ( _rtP -> P_70 * _rtB -> B_311_1667_0
- _rtP -> P_69 * _rtDW -> UnitDelay_DSTATE_pv [ 0 ] ) ; B_311_1549_0_idx_1 =
( _rtB -> B_311_1674_0 [ 1 ] - _rtB -> B_311_1676_0 [ 1 ] ) * _rtP -> P_1685
+ ( _rtP -> P_70 * _rtB -> B_311_1673_0 - _rtP -> P_69 * _rtDW ->
UnitDelay_DSTATE_pv [ 1 ] ) ; } else { B_311_1549_0_idx_0 = _rtP -> P_1690 ;
B_311_1549_0_idx_1 = _rtP -> P_1690 ; } _rtB -> B_311_1683_0 [ 0 ] = _rtP ->
P_1692 * B_311_1549_0_idx_0 ; _rtB -> B_311_1683_0 [ 1 ] = _rtP -> P_1692 *
B_311_1549_0_idx_1 ; tmpForInput [ 0 ] = B_311_1536_0_idx_0 ; tmpForInput [ 1
] = B_311_1536_0_idx_1 ; tmpForInput [ 2 ] = B_311_1536_0_idx_2 ; tmpForInput
[ 3 ] = B_311_1537_0_idx_0 ; tmpForInput [ 4 ] = B_311_1537_0_idx_1 ;
tmpForInput [ 5 ] = rtb_B_311_1540_0 ; tmpForInput [ 6 ] = rtb_B_311_1543_0 ;
tmpForInput [ 7 ] = rtb_B_311_1544_0 ; tmpForInput [ 8 ] = rtb_B_311_1545_0 ;
B_311_1536_0_idx_0 = - 0.0 ; for ( i = 0 ; i < 9 ; i ++ ) {
B_311_1536_0_idx_0 += tmpForInput [ i ] ; } _rtB -> B_311_1685_0 = _rtP ->
P_1693 * B_311_1536_0_idx_0 ; ssCallAccelRunBlock ( S , 311 , 1686 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 311 , 1687 ,
SS_CALL_MDL_OUTPUTS ) ; if ( _rtB -> B_311_1553_0 ) { _rtB -> B_311_1688_0 [
0 ] = _rtB -> B_311_1647_0 [ 0 ] ; _rtB -> B_311_1688_0 [ 1 ] = _rtB ->
B_311_1647_0 [ 1 ] ; } else { _rtB -> B_311_1688_0 [ 0 ] = _rtB ->
B_311_1560_0 ; _rtB -> B_311_1688_0 [ 1 ] = _rtB -> B_311_1567_0 ; }
ssCallAccelRunBlock ( S , 311 , 1689 , SS_CALL_MDL_OUTPUTS ) ;
rtb_B_311_1540_0 = _rtDW -> UnitDelay1_DSTATE_pi ; _rtB -> B_311_1691_0 = ! (
_rtB -> B_311_1496_0 != 0.0 ) ; _rtB -> B_311_1692_0 = _rtDW ->
UnitDelay1_DSTATE_cg ; rtb_B_311_1543_0 = muDoubleScalarSin ( _rtB ->
B_311_1692_0 ) ; rtb_B_311_1544_0 = muDoubleScalarCos ( _rtB -> B_311_1692_0
) ; rtb_B_311_1545_0 = ( 0.0 - rtb_B_311_1543_0 * _rtB -> B_311_82_0 ) -
rtb_B_311_1544_0 * _rtB -> B_311_83_0 ; B_311_1536_0_idx_1 = rtb_B_311_1543_0
* _rtB -> B_311_83_0 - rtb_B_311_1544_0 * _rtB -> B_311_82_0 ; _rtB ->
B_311_1703_0 = ( ( ( 0.0 - B_311_1536_0_idx_1 ) - rtb_B_311_1544_0 ) * _rtB
-> B_311_1571_0 [ 2 ] + ( _rtB -> B_311_1571_0 [ 0 ] * rtb_B_311_1544_0 +
_rtB -> B_311_1571_0 [ 1 ] * B_311_1536_0_idx_1 ) ) * 0.66666666666666663 ;
rtb_B_311_1543_0 = ( ( ( 0.0 - rtb_B_311_1545_0 ) - rtb_B_311_1543_0 ) * _rtB
-> B_311_1571_0 [ 2 ] + ( _rtB -> B_311_1571_0 [ 0 ] * rtb_B_311_1543_0 +
_rtB -> B_311_1571_0 [ 1 ] * rtb_B_311_1545_0 ) ) * 0.66666666666666663 ;
rtb_B_311_1543_0 = ( ( _rtB -> B_311_71_0 <= _rtB -> B_311_1703_0 ) && ( _rtB
-> B_311_1703_0 <= _rtB -> B_311_72_0_j ) && ( ( _rtB -> B_311_73_0_d <=
rtb_B_311_1543_0 ) && ( rtb_B_311_1543_0 <= _rtB -> B_311_74_0_g ) ) ) ;
rtb_B_311_1544_0 = _rtDW -> Delay_DSTATE_en ; _rtB -> B_311_1715_0 = ( ( (
rtb_B_311_1543_0 != 0.0 ) == ( _rtDW -> Delay_DSTATE_en != 0.0 ) ) || ( _rtB
-> B_311_75_0 != 0.0 ) ) ; _rtB -> B_311_1717_0 = ( _rtB -> B_311_78_0_m && (
_rtDW -> Delay_DSTATE_en != 0.0 ) ) ; if ( _rtDW ->
DiscreteTimeIntegrator_IC_LOADING_d != 0 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_h = _rtB -> B_311_1717_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_h >= _rtP -> P_1698 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_h = _rtP -> P_1698 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_h <= _rtP -> P_1699 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_h = _rtP -> P_1699 ; } } } if ( _rtB ->
B_311_1715_0 || ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_o != 0 ) ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE_h = _rtB -> B_311_1717_0 ; if ( _rtDW
-> DiscreteTimeIntegrator_DSTATE_h >= _rtP -> P_1698 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_h = _rtP -> P_1698 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_h <= _rtP -> P_1699 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_h = _rtP -> P_1699 ; } } } if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_h >= _rtP -> P_1698 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_h = _rtP -> P_1698 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_h <= _rtP -> P_1699 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_h = _rtP -> P_1699 ; } } _rtDW -> Relay_Mode =
( ( _rtDW -> DiscreteTimeIntegrator_DSTATE_h >= _rtP -> P_1700 ) || ( ( ! (
_rtDW -> DiscreteTimeIntegrator_DSTATE_h <= _rtP -> P_1701 ) ) && _rtDW ->
Relay_Mode ) ) ; if ( _rtDW -> Relay_Mode ) { _rtB -> B_311_1719_0 = _rtP ->
P_1702 ; } else { _rtB -> B_311_1719_0 = _rtP -> P_1703 ; } _rtB ->
B_311_1724_0 = ( ( _rtB -> B_311_1691_0 && ( _rtB -> B_311_1719_0 != 0.0 ) )
|| ( ( ! _rtB -> B_311_1691_0 ) && ( ! ( _rtB -> B_155_5_6 != 0.0 ) ) ) ) ;
if ( ! _rtB -> B_311_1724_0 ) { _rtB -> B_311_1726_0 = _rtDW ->
UnitDelay1_DSTATE_pi ; } else { _rtB -> B_311_1726_0 = _rtB -> B_311_1692_0 ;
} B_311_1536_0_idx_1 = _rtB -> B_311_1726_0 + _rtP -> P_1704 ;
rtb_B_311_1545_0 = muDoubleScalarSin ( B_311_1536_0_idx_1 ) ;
rtb_B_311_1443_0 = muDoubleScalarCos ( B_311_1536_0_idx_1 ) ;
rtb_B_311_1587_0 = ( 0.0 - rtb_B_311_1545_0 * _rtB -> B_311_67_0 ) -
rtb_B_311_1443_0 * _rtB -> B_311_68_0 ; rtb_B_311_1550_0 = rtb_B_311_1545_0 *
_rtB -> B_311_68_0 - rtb_B_311_1443_0 * _rtB -> B_311_67_0 ;
B_311_1536_0_idx_1 = ( _rtB -> B_311_1688_0 [ 0 ] * rtb_B_311_1545_0 + _rtB
-> B_311_1688_0 [ 1 ] * rtb_B_311_1443_0 ) + _rtB -> B_311_64_0 ;
B_311_1536_0_idx_0 = ( _rtB -> B_311_1688_0 [ 0 ] * rtb_B_311_1587_0 + _rtB
-> B_311_1688_0 [ 1 ] * rtb_B_311_1550_0 ) + _rtB -> B_311_64_0 ;
rtb_B_311_1545_0 = ( ( ( 0.0 - rtb_B_311_1587_0 ) - rtb_B_311_1545_0 ) * _rtB
-> B_311_1688_0 [ 0 ] + ( ( 0.0 - rtb_B_311_1550_0 ) - rtb_B_311_1443_0 ) *
_rtB -> B_311_1688_0 [ 1 ] ) + _rtB -> B_311_64_0 ; if ( B_311_1536_0_idx_1 >
_rtB -> B_311_65_0 ) { B_311_1536_0_idx_1 = _rtB -> B_311_65_0 ; } else { if
( B_311_1536_0_idx_1 < _rtB -> B_311_66_0 ) { B_311_1536_0_idx_1 = _rtB ->
B_311_66_0 ; } } _rtB -> B_311_1745_0 [ 0 ] = _rtP -> P_1705 *
B_311_1536_0_idx_1 ; if ( B_311_1536_0_idx_0 > _rtB -> B_311_65_0 ) {
B_311_1536_0_idx_0 = _rtB -> B_311_65_0 ; } else { if ( B_311_1536_0_idx_0 <
_rtB -> B_311_66_0 ) { B_311_1536_0_idx_0 = _rtB -> B_311_66_0 ; } } _rtB ->
B_311_1745_0 [ 1 ] = _rtP -> P_1705 * B_311_1536_0_idx_0 ; if (
rtb_B_311_1545_0 > _rtB -> B_311_65_0 ) { rtb_B_311_1545_0 = _rtB ->
B_311_65_0 ; } else { if ( rtb_B_311_1545_0 < _rtB -> B_311_66_0 ) {
rtb_B_311_1545_0 = _rtB -> B_311_66_0 ; } } _rtB -> B_311_1745_0 [ 2 ] = _rtP
-> P_1705 * rtb_B_311_1545_0 ; ssCallAccelRunBlock ( S , 311 , 1746 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 311 , 1747 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 311 , 1748 ,
SS_CALL_MDL_OUTPUTS ) ; if ( _rtB -> B_311_1619_0 [ 0 ] > _rtP -> P_1706 ) {
_rtB -> B_311_1749_0 = _rtP -> P_1706 ; } else if ( _rtB -> B_311_1619_0 [ 0
] < _rtP -> P_1707 ) { _rtB -> B_311_1749_0 = _rtP -> P_1707 ; } else { _rtB
-> B_311_1749_0 = _rtB -> B_311_1619_0 [ 0 ] ; } ssCallAccelRunBlock ( S ,
311 , 1750 , SS_CALL_MDL_OUTPUTS ) ; if ( rtb_B_311_1544_0 >= _rtP -> P_1709
) { rtb_B_311_1544_0 = _rtB -> B_311_76_0_l ; } else { rtb_B_311_1544_0 =
_rtB -> B_311_77_0 ; } _rtB -> B_311_1753_0 = ( rtb_B_311_1543_0 + _rtP ->
P_1708 ) * rtb_B_311_1544_0 ; rtb_B_311_35_0 = _rtB -> B_311_79_0_d * _rtB ->
B_311_1703_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_l ; if ( rtb_B_311_35_0
> _rtP -> P_1714 ) { _rtB -> B_311_1757_0 = _rtP -> P_1714 ; } else if (
rtb_B_311_35_0 < _rtP -> P_1715 ) { _rtB -> B_311_1757_0 = _rtP -> P_1715 ; }
else { _rtB -> B_311_1757_0 = rtb_B_311_35_0 ; } ssCallAccelRunBlock ( S ,
311 , 1758 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_311_1759_0 = _rtB ->
B_311_1703_0 * _rtB -> B_311_80_0_d ; _rtB -> B_311_1760_0 = _rtP -> P_1716 *
_rtB -> B_311_1757_0 ; { _rtB -> B_311_1761_0 = ( _rtP -> P_1719 [ 0 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_be [ 0 ] + ( _rtP -> P_1719 [ 1 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_be [ 1 ] ; _rtB -> B_311_1761_0 += _rtP ->
P_1720 * _rtB -> B_311_1760_0 ; } _rtB -> B_311_1766_0 = ( _rtP -> P_1723 *
_rtB -> B_311_1757_0 - ( real_T ) ( _rtB -> B_311_1692_0 >= _rtB ->
B_311_81_0 ) * _rtP -> P_1722 ) + _rtB -> B_311_1692_0 ; _rtB -> B_311_1767_0
= muDoubleScalarSin ( _rtB -> B_311_1726_0 ) ; _rtB -> B_311_1768_0 =
muDoubleScalarCos ( _rtB -> B_311_1726_0 ) ; if ( _rtB -> B_311_1724_0 ) {
_rtB -> B_311_1770_0 = _rtB -> B_311_1692_0 ; } else { _rtB -> B_311_1770_0 =
( _rtP -> P_71 * _rtB -> B_311_1749_0 * _rtP -> P_72 - ( real_T ) (
rtb_B_311_1540_0 >= _rtB -> B_311_84_0 ) * _rtP -> P_73 ) + rtb_B_311_1540_0
; } ssCallAccelRunBlock ( S , 311 , 1771 , SS_CALL_MDL_OUTPUTS ) ; } _rtB ->
B_311_1773_0 = _rtX -> Integrator_CSTATE_g ; muDoubleScalarSinCos ( _rtB ->
B_311_1773_0 , & rtb_B_311_1540_0 , & rtb_B_311_1543_0 ) ; _rtB ->
B_311_1775_0 [ 0 ] = _rtB -> B_311_1535_0 [ 0 ] * rtb_B_311_1540_0 ; _rtB ->
B_311_1775_0 [ 1 ] = _rtB -> B_311_1535_0 [ 0 ] * rtb_B_311_1543_0 ; i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_1780_0 [ 0 ] =
_rtP -> P_1729 * _rtB -> B_311_152_0 [ 176 ] * _rtP -> P_1732 ; _rtB ->
B_311_1780_0 [ 1 ] = _rtP -> P_1730 * _rtB -> B_311_152_0 [ 177 ] * _rtP ->
P_1732 ; _rtB -> B_311_1780_0 [ 2 ] = _rtP -> P_1731 * _rtB -> B_311_152_0 [
178 ] * _rtP -> P_1732 ; } _rtB -> B_311_1781_0 [ 0 ] = _rtB -> B_311_1780_0
[ 0 ] * rtb_B_311_1540_0 ; _rtB -> B_311_1781_0 [ 1 ] = _rtB -> B_311_1780_0
[ 0 ] * rtb_B_311_1543_0 ; _rtB -> B_311_1783_0 [ 0 ] = _rtB -> B_311_1535_0
[ 1 ] * rtb_B_311_1540_0 ; _rtB -> B_311_1783_0 [ 1 ] = _rtB -> B_311_1535_0
[ 1 ] * rtb_B_311_1543_0 ; _rtB -> B_311_1785_0 [ 0 ] = _rtB -> B_311_1780_0
[ 1 ] * rtb_B_311_1540_0 ; _rtB -> B_311_1785_0 [ 1 ] = _rtB -> B_311_1780_0
[ 1 ] * rtb_B_311_1543_0 ; _rtB -> B_311_1787_0 [ 0 ] = _rtB -> B_311_1535_0
[ 2 ] * rtb_B_311_1540_0 ; _rtB -> B_311_1787_0 [ 1 ] = _rtB -> B_311_1535_0
[ 2 ] * rtb_B_311_1543_0 ; _rtB -> B_311_1789_0 [ 0 ] = _rtB -> B_311_1780_0
[ 2 ] * rtb_B_311_1540_0 ; _rtB -> B_311_1789_0 [ 1 ] = _rtB -> B_311_1780_0
[ 2 ] * rtb_B_311_1543_0 ; i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
_rtB -> B_311_1902_0 = _rtP -> P_1737 * _rtB -> B_311_152_0 [ 204 ] * _rtP ->
P_1738 * _rtB -> B_311_93_0 * _rtP -> P_1739 ; ssCallAccelRunBlock ( S , 311
, 1903 , SS_CALL_MDL_OUTPUTS ) ; i = ssIsSampleHit ( S , 5 , 0 ) ; if ( i !=
0 ) { _rtB -> B_311_1905_0 = _rtDW -> RateTransition8_Buffer0_m ; } if ( _rtB
-> B_311_88_0 >= _rtP -> P_1744 ) { rtb_B_311_1543_0 = _rtB -> B_311_1905_0 ;
} else { rtb_B_311_1543_0 = rt_Lookup ( _rtP -> P_1742 , 4 , ssGetTaskTime (
S , 2 ) , _rtP -> P_1743 ) ; } if ( _rtP -> P_1740 >= _rtP -> P_1745 ) { if (
_rtB -> B_311_89_0 >= _rtP -> P_108 ) { _rtB -> B_311_1912_0 =
rtb_B_311_1543_0 ; } else { _rtB -> B_311_1912_0 = _rtB -> B_311_92_0 ; } }
else { _rtB -> B_311_1912_0 = rt_Lookup ( _rtP -> P_106 , 5 , ssGetTaskTime (
S , 2 ) , _rtP -> P_107 ) ; } if ( _rtP -> P_1746 >= _rtP -> P_1747 ) { if (
_rtB -> B_311_90_0 >= _rtP -> P_111 ) { _rtB -> B_311_1920_0 =
rtb_B_311_1543_0 ; } else { _rtB -> B_311_1920_0 = _rtB -> B_311_92_0 ; } }
else { _rtB -> B_311_1920_0 = rt_Lookup ( _rtP -> P_109 , 5 , ssGetTaskTime (
S , 2 ) , _rtP -> P_110 ) ; } if ( _rtP -> P_1748 >= _rtP -> P_1749 ) { if (
_rtB -> B_311_91_0 >= _rtP -> P_114 ) { _rtB -> B_311_1928_0 =
rtb_B_311_1543_0 ; } else { _rtB -> B_311_1928_0 = _rtB -> B_311_92_0 ; } }
else { _rtB -> B_311_1928_0 = rt_Lookup ( _rtP -> P_112 , 5 , ssGetTaskTime (
S , 2 ) , _rtP -> P_113 ) ; } } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0
) { _rtB -> B_311_1939_0 [ 0 ] = _rtDW -> UnitDelay_DSTATE_g3 [ 0 ] ;
B_311_1940_0_idx_0 = _rtDW -> UnitDelay_DSTATE_h [ 0 ] ; _rtB -> B_311_1939_0
[ 1 ] = _rtDW -> UnitDelay_DSTATE_g3 [ 1 ] ; B_311_1940_0_idx_1 = _rtDW ->
UnitDelay_DSTATE_h [ 1 ] ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 )
{ _rtB -> B_311_1944_0 [ 0 ] = _rtP -> P_1752 * _rtB -> B_311_152_0 [ 96 ] *
_rtP -> P_1755 ; _rtB -> B_311_1944_0 [ 1 ] = _rtP -> P_1753 * _rtB ->
B_311_152_0 [ 97 ] * _rtP -> P_1755 ; _rtB -> B_311_1944_0 [ 2 ] = _rtP ->
P_1754 * _rtB -> B_311_152_0 [ 98 ] * _rtP -> P_1755 ; } i = ssIsSampleHit (
S , 3 , 0 ) ; if ( i != 0 ) { rtb_B_311_50_0 = _rtP -> P_1756 * _rtB ->
B_311_1944_0 [ 0 ] ; rtb_B_311_68_0 = _rtP -> P_1756 * _rtB -> B_311_1944_0 [
1 ] ; rtb_B_311_51_0 = _rtP -> P_1756 * _rtB -> B_311_1944_0 [ 2 ] ;
rtb_B_311_1540_0 = ( 0.0 - _rtDW -> UnitDelay2_DSTATE_bb [ 0 ] * _rtB ->
B_311_119_0 ) - _rtDW -> UnitDelay2_DSTATE_bb [ 1 ] * _rtB -> B_311_120_0 ;
rtb_B_311_1543_0 = _rtDW -> UnitDelay2_DSTATE_bb [ 0 ] * _rtB -> B_311_120_0
- _rtDW -> UnitDelay2_DSTATE_bb [ 1 ] * _rtB -> B_311_119_0 ; _rtB ->
B_311_1955_0 = ( ( ( 0.0 - rtb_B_311_1540_0 ) - _rtDW -> UnitDelay2_DSTATE_bb
[ 0 ] ) * rtb_B_311_51_0 + ( rtb_B_311_50_0 * _rtDW -> UnitDelay2_DSTATE_bb [
0 ] + rtb_B_311_68_0 * rtb_B_311_1540_0 ) ) * 0.66666666666666663 ; _rtB ->
B_311_1956_0 = ( ( ( 0.0 - rtb_B_311_1543_0 ) - _rtDW -> UnitDelay2_DSTATE_bb
[ 1 ] ) * rtb_B_311_51_0 + ( rtb_B_311_50_0 * _rtDW -> UnitDelay2_DSTATE_bb [
1 ] + rtb_B_311_68_0 * rtb_B_311_1543_0 ) ) * 0.66666666666666663 ; if ( _rtP
-> P_4289 == 1 ) { _rtB -> B_311_1957_0 [ 0 ] = B_311_1940_0_idx_0 ; _rtB ->
B_311_1957_0 [ 1 ] = B_311_1940_0_idx_1 ; } else { _rtB -> B_311_1957_0 [ 0 ]
= _rtB -> B_311_1955_0 ; _rtB -> B_311_1957_0 [ 1 ] = _rtB -> B_311_1956_0 ;
} _rtB -> B_311_1962_0 = ( ( _rtB -> B_311_1905_0 != 0.0 ) && ( ! ( _rtB ->
B_155_5_16 != 0.0 ) ) ) ; rtb_B_311_44_0 = ! _rtB -> B_311_1962_0 ; _rtB ->
B_311_1964_0 = ! rtb_B_311_44_0 ; B_311_1537_0_idx_0 = _rtB -> B_311_1939_0 [
0 ] - _rtB -> B_311_1957_0 [ 0 ] ; B_311_1965_0_idx_0 = _rtDW ->
UnitDelay1_DSTATE_d5 [ 0 ] ; B_311_1537_0_idx_1 = _rtB -> B_311_1939_0 [ 1 ]
- _rtB -> B_311_1957_0 [ 1 ] ; B_311_1965_0_idx_1 = _rtDW ->
UnitDelay1_DSTATE_d5 [ 1 ] ; rtb_B_311_1540_0 = _rtB -> B_311_98_0 *
B_311_1537_0_idx_0 ; _rtB -> B_311_1960_0 = B_311_1537_0_idx_0 / _rtB ->
B_311_100_0 ; _rtB -> B_311_1966_0 = _rtDW -> UnitDelay1_DSTATE_d5 [ 0 ] -
rtb_B_311_1540_0 ; if ( _rtDW -> DiscreteTimeIntegrator_IC_LOADING_do != 0 )
{ _rtDW -> DiscreteTimeIntegrator_DSTATE_pi = _rtB -> B_311_1966_0 ; if (
_rtDW -> DiscreteTimeIntegrator_DSTATE_pi >= _rtP -> P_1760 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_pi = _rtP -> P_1760 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_pi <= _rtP -> P_1761 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_pi = _rtP -> P_1761 ; } } _rtB -> B_311_1967_0
= _rtDW -> DiscreteTimeIntegrator_DSTATE_pi ; } else if ( _rtB ->
B_311_1964_0 || ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_g != 0 ) ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE_pi = _rtB -> B_311_1966_0 ; if ( _rtDW
-> DiscreteTimeIntegrator_DSTATE_pi >= _rtP -> P_1760 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_pi = _rtP -> P_1760 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_pi <= _rtP -> P_1761 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_pi = _rtP -> P_1761 ; } } _rtB -> B_311_1967_0
= _rtDW -> DiscreteTimeIntegrator_DSTATE_pi ; } else { _rtB -> B_311_1967_0 =
_rtP -> P_1759 * _rtB -> B_311_1960_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_pi ; } if ( _rtB -> B_311_1967_0 >= _rtP ->
P_1760 ) { _rtB -> B_311_1967_0 = _rtP -> P_1760 ; } else { if ( _rtB ->
B_311_1967_0 <= _rtP -> P_1761 ) { _rtB -> B_311_1967_0 = _rtP -> P_1761 ; }
} rtb_B_311_35_0 = rtb_B_311_1540_0 + _rtB -> B_311_1967_0 ; if (
rtb_B_311_35_0 > _rtP -> P_1762 ) { _rtB -> B_311_1969_0 = _rtP -> P_1762 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_1763 ) { _rtB -> B_311_1969_0 = _rtP ->
P_1763 ; } else { _rtB -> B_311_1969_0 = rtb_B_311_35_0 ; } rtb_B_311_1540_0
= _rtB -> B_311_101_0 * B_311_1537_0_idx_1 ; _rtB -> B_311_1971_0 =
B_311_1537_0_idx_1 / _rtB -> B_311_103_0 ; _rtB -> B_311_1972_0 = !
rtb_B_311_44_0 ; _rtB -> B_311_1973_0 = _rtDW -> UnitDelay1_DSTATE_d5 [ 1 ] -
rtb_B_311_1540_0 ; if ( _rtDW -> DiscreteTimeIntegrator_IC_LOADING_k != 0 ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE_o = _rtB -> B_311_1973_0 ; if ( _rtDW
-> DiscreteTimeIntegrator_DSTATE_o >= _rtP -> P_1765 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_o = _rtP -> P_1765 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_o <= _rtP -> P_1766 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_o = _rtP -> P_1766 ; } } _rtB -> B_311_1974_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_o ; } else if ( _rtB -> B_311_1972_0
|| ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_k != 0 ) ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_o = _rtB -> B_311_1973_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_o >= _rtP -> P_1765 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_o = _rtP -> P_1765 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_o <= _rtP -> P_1766 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_o = _rtP -> P_1766 ; } } _rtB -> B_311_1974_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_o ; } else { _rtB -> B_311_1974_0 =
_rtP -> P_1764 * _rtB -> B_311_1971_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_o ; } if ( _rtB -> B_311_1974_0 >= _rtP ->
P_1765 ) { _rtB -> B_311_1974_0 = _rtP -> P_1765 ; } else { if ( _rtB ->
B_311_1974_0 <= _rtP -> P_1766 ) { _rtB -> B_311_1974_0 = _rtP -> P_1766 ; }
} rtb_B_311_35_0 = rtb_B_311_1540_0 + _rtB -> B_311_1974_0 ; if (
rtb_B_311_35_0 > _rtP -> P_1767 ) { _rtB -> B_311_1976_0 = _rtP -> P_1767 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_1768 ) { _rtB -> B_311_1976_0 = _rtP ->
P_1768 ; } else { _rtB -> B_311_1976_0 = rtb_B_311_35_0 ; }
ssCallAccelRunBlock ( S , 311 , 1977 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_311_1979_0 = _rtP -> P_1770 * _rtDW -> UnitDelay_DSTATE_cd ; } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_1983_0 [ 0 ] =
_rtP -> P_1771 * _rtB -> B_311_152_0 [ 99 ] * _rtP -> P_1774 ; _rtB ->
B_311_1983_0 [ 1 ] = _rtP -> P_1772 * _rtB -> B_311_152_0 [ 100 ] * _rtP ->
P_1774 ; _rtB -> B_311_1983_0 [ 2 ] = _rtP -> P_1773 * _rtB -> B_311_152_0 [
101 ] * _rtP -> P_1774 ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) {
_rtB -> B_311_1589_0 [ 0 ] = _rtP -> P_1775 * _rtB -> B_311_1983_0 [ 0 ] ;
_rtB -> B_311_1589_0 [ 1 ] = _rtP -> P_1775 * _rtB -> B_311_1983_0 [ 1 ] ;
_rtB -> B_311_1589_0 [ 2 ] = _rtP -> P_1775 * _rtB -> B_311_1983_0 [ 2 ] ;
B_311_1537_0_idx_1 = _rtDW -> UnitDelay_DSTATE_hf [ 1 ] ; rtb_B_311_1540_0 =
( 0.0 - _rtDW -> UnitDelay_DSTATE_hf [ 0 ] * _rtB -> B_311_115_0 ) - _rtDW ->
UnitDelay_DSTATE_hf [ 1 ] * _rtB -> B_311_116_0 ; rtb_B_311_1543_0 = _rtDW ->
UnitDelay_DSTATE_hf [ 0 ] * _rtB -> B_311_116_0 - _rtDW ->
UnitDelay_DSTATE_hf [ 1 ] * _rtB -> B_311_115_0 ; rtb_B_311_1545_0 = ( 0.0 -
rtb_B_311_1543_0 ) - _rtDW -> UnitDelay_DSTATE_hf [ 1 ] ; _rtB ->
B_311_1994_0 = ( ( ( 0.0 - rtb_B_311_1540_0 ) - _rtDW -> UnitDelay_DSTATE_hf
[ 0 ] ) * _rtB -> B_311_1589_0 [ 2 ] + ( _rtB -> B_311_1589_0 [ 0 ] * _rtDW
-> UnitDelay_DSTATE_hf [ 0 ] + _rtB -> B_311_1589_0 [ 1 ] * rtb_B_311_1540_0
) ) * 0.66666666666666663 ; { B_311_1995_0 = ( _rtP -> P_1779 [ 0 ] ) * _rtDW
-> DiscreteStateSpace_DSTATE_f5 [ 0 ] + ( _rtP -> P_1779 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_f5 [ 1 ] ; B_311_1995_0 += _rtP -> P_1780 * _rtB ->
B_311_1994_0 ; } _rtB -> B_311_1996_0 = ( ( _rtB -> B_311_1589_0 [ 0 ] *
B_311_1537_0_idx_1 + _rtB -> B_311_1589_0 [ 1 ] * rtb_B_311_1543_0 ) + _rtB
-> B_311_1589_0 [ 2 ] * rtb_B_311_1545_0 ) * 0.66666666666666663 ; {
B_311_1997_0 = ( _rtP -> P_1784 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_gl [ 0 ] + ( _rtP -> P_1784 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_gl [ 1 ] ; B_311_1997_0 += _rtP -> P_1785 * _rtB ->
B_311_1996_0 ; } rtb_B_311_1545_0 = B_311_1995_0 * B_311_1995_0 +
B_311_1997_0 * B_311_1997_0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if (
_rtDW -> Sqrt_DWORK1_p != 0 ) { ssSetBlockStateForSolverChangedAtMajorStep (
S ) ; _rtDW -> Sqrt_DWORK1_p = 0 ; } _rtB -> B_311_2001_0 =
muDoubleScalarSqrt ( rtb_B_311_1545_0 ) ; } else if ( rtb_B_311_1545_0 < 0.0
) { _rtB -> B_311_2001_0 = - muDoubleScalarSqrt ( muDoubleScalarAbs (
rtb_B_311_1545_0 ) ) ; _rtDW -> Sqrt_DWORK1_p = 1 ; } else { _rtB ->
B_311_2001_0 = muDoubleScalarSqrt ( rtb_B_311_1545_0 ) ; } } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_2005_0 [ 0 ] =
_rtP -> P_1787 * _rtB -> B_311_152_0 [ 182 ] * _rtP -> P_1790 ; _rtB ->
B_311_2005_0 [ 1 ] = _rtP -> P_1788 * _rtB -> B_311_152_0 [ 183 ] * _rtP ->
P_1790 ; _rtB -> B_311_2005_0 [ 2 ] = _rtP -> P_1789 * _rtB -> B_311_152_0 [
184 ] * _rtP -> P_1790 ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) {
_rtB -> B_311_1571_0 [ 0 ] = _rtP -> P_1791 * _rtB -> B_311_2005_0 [ 0 ] ;
_rtB -> B_311_1571_0 [ 1 ] = _rtP -> P_1791 * _rtB -> B_311_2005_0 [ 1 ] ;
_rtB -> B_311_1571_0 [ 2 ] = _rtP -> P_1791 * _rtB -> B_311_2005_0 [ 2 ] ;
rtb_B_311_1545_0 = ( 0.0 - _rtDW -> UnitDelay1_DSTATE_n0 [ 0 ] * _rtB ->
B_311_117_0 ) - _rtDW -> UnitDelay1_DSTATE_n0 [ 1 ] * _rtB -> B_311_118_0 ;
rtb_B_311_1540_0 = _rtDW -> UnitDelay1_DSTATE_n0 [ 0 ] * _rtB -> B_311_118_0
- _rtDW -> UnitDelay1_DSTATE_n0 [ 1 ] * _rtB -> B_311_117_0 ; _rtB ->
B_311_2016_0 = ( ( ( 0.0 - rtb_B_311_1545_0 ) - _rtDW -> UnitDelay1_DSTATE_n0
[ 0 ] ) * _rtB -> B_311_1571_0 [ 2 ] + ( _rtB -> B_311_1571_0 [ 0 ] * _rtDW
-> UnitDelay1_DSTATE_n0 [ 0 ] + _rtB -> B_311_1571_0 [ 1 ] * rtb_B_311_1545_0
) ) * 0.66666666666666663 ; _rtB -> B_311_2017_0 = ( ( ( 0.0 -
rtb_B_311_1540_0 ) - _rtDW -> UnitDelay1_DSTATE_n0 [ 1 ] ) * _rtB ->
B_311_1571_0 [ 2 ] + ( _rtB -> B_311_1571_0 [ 0 ] * _rtDW ->
UnitDelay1_DSTATE_n0 [ 1 ] + _rtB -> B_311_1571_0 [ 1 ] * rtb_B_311_1540_0 )
) * 0.66666666666666663 ; _rtB -> B_311_2018_0 [ 0 ] = _rtP -> P_1793 [ 0 ] *
_rtB -> B_311_2016_0 ; _rtB -> B_311_2018_0 [ 1 ] = _rtP -> P_1793 [ 1 ] *
_rtB -> B_311_2017_0 ; } i = ssIsSampleHit ( S , 5 , 0 ) ; if ( i != 0 ) {
_rtB -> B_311_2019_0 = _rtP -> P_1794 * _rtB -> B_156_56_0 ; } i =
ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_2020_0 [ 0 ] =
_rtB -> B_311_2019_0 - _rtB -> B_311_1979_0 ; _rtB -> B_311_2020_0 [ 1 ] =
_rtB -> B_156_13_0 - _rtB -> B_311_2001_0 ; _rtB -> B_311_2020_0 [ 2 ] = _rtB
-> B_311_126_0 [ 0 ] - _rtB -> B_311_2018_0 [ 0 ] ; _rtB -> B_311_2020_0 [ 3
] = _rtB -> B_311_126_0 [ 1 ] - _rtB -> B_311_2018_0 [ 1 ] ; { B_311_2021_0 =
( _rtP -> P_1797 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_cy [ 0 ] + (
_rtP -> P_1797 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_cy [ 1 ] ;
B_311_2021_0 += _rtP -> P_1798 * _rtB -> B_311_2020_0 [ 2 ] ; }
rtb_B_311_2022_0 = B_311_2021_0 * _rtB -> B_311_94_0_l ; { B_311_2023_0 = (
_rtP -> P_1802 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_m [ 0 ] + ( _rtP
-> P_1802 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_m [ 1 ] ; B_311_2023_0
+= _rtP -> P_1803 * _rtB -> B_311_2020_0 [ 3 ] ; } if ( B_311_2023_0 > _rtP
-> P_1806 ) { _rtB -> B_311_2025_0 = B_311_2023_0 - _rtP -> P_1806 ; } else
if ( B_311_2023_0 >= _rtP -> P_1805 ) { _rtB -> B_311_2025_0 = 0.0 ; } else {
_rtB -> B_311_2025_0 = B_311_2023_0 - _rtP -> P_1805 ; } rtb_B_311_2027_0 =
B_311_2023_0 * _rtB -> B_311_95_0 + _rtB -> B_311_2025_0 * _rtB ->
B_311_127_0 ; { B_311_2028_0 = _rtP -> P_1809 * _rtDW ->
DiscreteStateSpace_DSTATE_pf ; B_311_2028_0 += _rtP -> P_1810 * _rtB ->
B_311_2020_0 [ 0 ] ; } rtb_B_311_2029_0 = B_311_2028_0 * _rtB -> B_311_94_0_l
; { B_311_2030_0 = _rtP -> P_1814 * _rtDW -> DiscreteStateSpace_DSTATE_l ;
B_311_2030_0 += _rtP -> P_1815 * _rtB -> B_311_2020_0 [ 1 ] ; }
rtb_B_311_2031_0 = B_311_2030_0 * _rtB -> B_311_95_0 ; } i = ssIsSampleHit (
S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_2032_0 = muDoubleScalarMin ( _rtB
-> B_311_1267_0 [ 0 ] , _rtB -> B_311_1902_0 ) ; } i = ssIsSampleHit ( S , 3
, 0 ) ; if ( i != 0 ) { _rtB -> B_311_2033_0 [ 0 ] = rtb_B_311_2022_0 + _rtB
-> B_311_2019_0 ; _rtB -> B_311_2033_0 [ 1 ] = rtb_B_311_2027_0 + _rtB ->
B_156_13_0 ; _rtB -> B_311_2033_0 [ 2 ] = rtb_B_311_2029_0 + _rtB ->
B_311_2032_0 ; _rtB -> B_311_2033_0 [ 3 ] = rtb_B_311_2031_0 + _rtB ->
B_311_1267_0 [ 1 ] ; if ( _rtB -> B_311_2033_0 [ 1 ] > _rtP -> P_1817 ) {
_rtB -> B_311_2034_0 = _rtP -> P_1817 ; } else if ( _rtB -> B_311_2033_0 [ 1
] < _rtP -> P_1818 ) { _rtB -> B_311_2034_0 = _rtP -> P_1818 ; } else { _rtB
-> B_311_2034_0 = _rtB -> B_311_2033_0 [ 1 ] ; } if ( _rtB -> B_311_1962_0 )
{ _rtB -> B_311_2036_0 [ 0 ] = B_311_1965_0_idx_0 ; _rtB -> B_311_2036_0 [ 1
] = B_311_1965_0_idx_1 ; } else { B_311_1965_0_idx_0 = _rtB -> B_311_2034_0 -
_rtB -> B_311_1939_0 [ 0 ] ; B_311_1965_0_idx_1 = _rtB -> B_311_96_0 - _rtB
-> B_311_1939_0 [ 1 ] ; if ( B_311_1965_0_idx_0 > _rtB -> B_311_104_0 [ 0 ] )
{ B_311_1965_0_idx_0 = _rtB -> B_311_104_0 [ 0 ] ; } else { if (
B_311_1965_0_idx_0 < _rtB -> B_311_105_0 [ 0 ] ) { B_311_1965_0_idx_0 = _rtB
-> B_311_105_0 [ 0 ] ; } } _rtB -> B_311_2036_0 [ 0 ] = B_311_1965_0_idx_0 +
_rtB -> B_311_1939_0 [ 0 ] ; if ( B_311_1965_0_idx_1 > _rtB -> B_311_104_0 [
1 ] ) { B_311_1965_0_idx_1 = _rtB -> B_311_104_0 [ 1 ] ; } else { if (
B_311_1965_0_idx_1 < _rtB -> B_311_105_0 [ 1 ] ) { B_311_1965_0_idx_1 = _rtB
-> B_311_105_0 [ 1 ] ; } } _rtB -> B_311_2036_0 [ 1 ] = B_311_1965_0_idx_1 +
_rtB -> B_311_1939_0 [ 1 ] ; } B_311_1965_0_idx_0 = _rtDW ->
UnitDelay_DSTATE_om [ 0 ] ; B_311_1965_0_idx_1 = _rtDW -> UnitDelay_DSTATE_om
[ 1 ] ; { B_311_2038_0 = ( _rtP -> P_1822 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_bi [ 0 ] + ( _rtP -> P_1822 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_bi [ 1 ] ; B_311_2038_0 += _rtP -> P_1823 * _rtB ->
B_311_2016_0 ; } { B_311_2039_0 = ( _rtP -> P_1827 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_i [ 0 ] + ( _rtP -> P_1827 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_i [ 1 ] ; B_311_2039_0 += _rtP -> P_1828 * _rtB ->
B_311_2017_0 ; } B_311_1537_0_idx_0 = B_311_1965_0_idx_0 - B_311_2038_0 ;
B_311_1537_0_idx_1 = B_311_1965_0_idx_1 - B_311_2039_0 ; rtb_B_311_2022_0 =
_rtB -> B_311_107_0 * B_311_1537_0_idx_0 ; _rtB -> B_311_2042_0 =
B_311_1537_0_idx_0 / _rtB -> B_311_109_0 ; _rtB -> B_311_2043_0 = ! _rtB ->
B_311_1962_0 ; B_311_1940_0_idx_0 = _rtDW -> UnitDelay_DSTATE_dz [ 0 ] ;
B_311_1940_0_idx_1 = _rtDW -> UnitDelay_DSTATE_dz [ 1 ] ; { B_311_2045_0 = (
_rtP -> P_1833 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_fl [ 0 ] + ( _rtP
-> P_1833 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_fl [ 1 ] ;
B_311_2045_0 += _rtP -> P_1834 * _rtB -> B_311_1955_0 ; } { B_311_2046_0 = (
_rtP -> P_1838 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_o [ 0 ] + ( _rtP
-> P_1838 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_o [ 1 ] ; B_311_2046_0
+= _rtP -> P_1839 * _rtB -> B_311_1956_0 ; } _rtB -> B_311_2049_0 = (
B_311_1940_0_idx_0 - _rtP -> P_1841 * B_311_2045_0 ) - rtb_B_311_2022_0 ; if
( _rtDW -> DiscreteTimeIntegrator_IC_LOADING_g != 0 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_k = _rtB -> B_311_2049_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_k >= _rtP -> P_1843 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_k = _rtP -> P_1843 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_k <= _rtP -> P_1844 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_k = _rtP -> P_1844 ; } } _rtB -> B_311_2050_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_k ; } else if ( _rtB -> B_311_2043_0
|| ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_g1 != 0 ) ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_k = _rtB -> B_311_2049_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_k >= _rtP -> P_1843 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_k = _rtP -> P_1843 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_k <= _rtP -> P_1844 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_k = _rtP -> P_1844 ; } } _rtB -> B_311_2050_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_k ; } else { _rtB -> B_311_2050_0 =
_rtP -> P_1842 * _rtB -> B_311_2042_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_k ; } if ( _rtB -> B_311_2050_0 >= _rtP ->
P_1843 ) { _rtB -> B_311_2050_0 = _rtP -> P_1843 ; } else { if ( _rtB ->
B_311_2050_0 <= _rtP -> P_1844 ) { _rtB -> B_311_2050_0 = _rtP -> P_1844 ; }
} rtb_B_311_35_0 = rtb_B_311_2022_0 + _rtB -> B_311_2050_0 ; if (
rtb_B_311_35_0 > _rtP -> P_1845 ) { _rtB -> B_311_2052_0 = _rtP -> P_1845 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_1846 ) { _rtB -> B_311_2052_0 = _rtP ->
P_1846 ; } else { _rtB -> B_311_2052_0 = rtb_B_311_35_0 ; } rtb_B_311_2022_0
= _rtB -> B_311_110_0 * B_311_1537_0_idx_1 ; _rtB -> B_311_2054_0 =
B_311_1537_0_idx_1 / _rtB -> B_311_112_0 ; _rtB -> B_311_2055_0 = ! _rtB ->
B_311_1962_0 ; _rtB -> B_311_2056_0 = ( B_311_1940_0_idx_1 - _rtP -> P_1841 *
B_311_2046_0 ) - rtb_B_311_2022_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_IC_LOADING_o != 0 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_j = _rtB -> B_311_2056_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_j >= _rtP -> P_1848 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_j = _rtP -> P_1848 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_j <= _rtP -> P_1849 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_j = _rtP -> P_1849 ; } } _rtB -> B_311_2057_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_j ; } else if ( _rtB -> B_311_2055_0
|| ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_k3 != 0 ) ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_j = _rtB -> B_311_2056_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_j >= _rtP -> P_1848 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_j = _rtP -> P_1848 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_j <= _rtP -> P_1849 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_j = _rtP -> P_1849 ; } } _rtB -> B_311_2057_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_j ; } else { _rtB -> B_311_2057_0 =
_rtP -> P_1847 * _rtB -> B_311_2054_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_j ; } if ( _rtB -> B_311_2057_0 >= _rtP ->
P_1848 ) { _rtB -> B_311_2057_0 = _rtP -> P_1848 ; } else { if ( _rtB ->
B_311_2057_0 <= _rtP -> P_1849 ) { _rtB -> B_311_2057_0 = _rtP -> P_1849 ; }
} rtb_B_311_35_0 = rtb_B_311_2022_0 + _rtB -> B_311_2057_0 ; if (
rtb_B_311_35_0 > _rtP -> P_1850 ) { _rtB -> B_311_2059_0 = _rtP -> P_1850 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_1851 ) { _rtB -> B_311_2059_0 = _rtP ->
P_1851 ; } else { _rtB -> B_311_2059_0 = rtb_B_311_35_0 ; } _rtB ->
B_311_2061_0 [ 0 ] = _rtP -> P_1852 * B_311_2045_0 + _rtB -> B_311_2052_0 ;
_rtB -> B_311_2061_0 [ 1 ] = _rtP -> P_1852 * B_311_2046_0 + _rtB ->
B_311_2059_0 ; ssCallAccelRunBlock ( S , 311 , 2062 , SS_CALL_MDL_OUTPUTS ) ;
if ( _rtB -> B_311_2033_0 [ 2 ] > _rtB -> B_311_142_0 ) { rtb_B_311_2022_0 =
_rtB -> B_311_142_0 ; } else if ( _rtB -> B_311_2033_0 [ 2 ] < _rtB ->
B_311_143_0 ) { rtb_B_311_2022_0 = _rtB -> B_311_143_0 ; } else {
rtb_B_311_2022_0 = _rtB -> B_311_2033_0 [ 2 ] ; } rtb_B_311_47_0 =
rtb_B_311_2022_0 ; rtb_B_311_2022_0 = muDoubleScalarAtan2 ( _rtB ->
B_311_2033_0 [ 3 ] , rtb_B_311_2022_0 ) ; rtb_B_311_35_0 =
muDoubleScalarHypot ( rtb_B_311_47_0 , _rtB -> B_311_2033_0 [ 3 ] ) ;
rtb_B_311_47_0 = muDoubleScalarHypot ( rtb_B_311_47_0 , _rtB -> B_311_2033_0
[ 3 ] ) ; if ( rtb_B_311_35_0 > _rtP -> P_1853 ) { rtb_B_311_35_0 = _rtP ->
P_1853 ; } else { if ( rtb_B_311_35_0 < _rtP -> P_1854 ) { rtb_B_311_35_0 =
_rtP -> P_1854 ; } } _rtB -> B_311_2071_0 = rtb_B_311_35_0 *
muDoubleScalarCos ( rtb_B_311_2022_0 ) ; if ( rtb_B_311_47_0 > _rtP -> P_1853
) { rtb_B_311_47_0 = _rtP -> P_1853 ; } else { if ( rtb_B_311_47_0 < _rtP ->
P_1854 ) { rtb_B_311_47_0 = _rtP -> P_1854 ; } } _rtB -> B_311_2071_1 =
rtb_B_311_47_0 * muDoubleScalarSin ( rtb_B_311_2022_0 ) ; if ( ! _rtB ->
B_311_1962_0 ) { _rtB -> B_311_2073_0 [ 0 ] = _rtB -> B_311_2016_0 ; _rtB ->
B_311_2073_0 [ 1 ] = _rtB -> B_311_2017_0 ; } else { B_311_1537_0_idx_0 =
_rtB -> B_311_2071_0 - B_311_1965_0_idx_0 ; B_311_1537_0_idx_1 = _rtB ->
B_311_2071_1 - B_311_1965_0_idx_1 ; if ( B_311_1537_0_idx_0 > _rtB ->
B_311_113_0_o [ 0 ] ) { B_311_1537_0_idx_0 = _rtB -> B_311_113_0_o [ 0 ] ; }
else { if ( B_311_1537_0_idx_0 < _rtB -> B_311_114_0 [ 0 ] ) {
B_311_1537_0_idx_0 = _rtB -> B_311_114_0 [ 0 ] ; } } _rtB -> B_311_2073_0 [ 0
] = B_311_1537_0_idx_0 + B_311_1965_0_idx_0 ; if ( B_311_1537_0_idx_1 > _rtB
-> B_311_113_0_o [ 1 ] ) { B_311_1537_0_idx_1 = _rtB -> B_311_113_0_o [ 1 ] ;
} else { if ( B_311_1537_0_idx_1 < _rtB -> B_311_114_0 [ 1 ] ) {
B_311_1537_0_idx_1 = _rtB -> B_311_114_0 [ 1 ] ; } } _rtB -> B_311_2073_0 [ 1
] = B_311_1537_0_idx_1 + B_311_1965_0_idx_1 ; } _rtB -> B_311_2075_0 = ( _rtB
-> B_311_1589_0 [ 0 ] * _rtB -> B_311_1571_0 [ 0 ] + _rtB -> B_311_1589_0 [ 1
] * _rtB -> B_311_1571_0 [ 1 ] ) + _rtB -> B_311_1589_0 [ 2 ] * _rtB ->
B_311_1571_0 [ 2 ] ; _rtB -> B_311_2081_0 = ( ( ( _rtB -> B_311_1589_0 [ 1 ]
- _rtB -> B_311_1589_0 [ 2 ] ) * _rtB -> B_311_1571_0 [ 0 ] + ( _rtB ->
B_311_1589_0 [ 2 ] - _rtB -> B_311_1589_0 [ 0 ] ) * _rtB -> B_311_1571_0 [ 1
] ) + ( _rtB -> B_311_1589_0 [ 0 ] - _rtB -> B_311_1589_0 [ 1 ] ) * _rtB ->
B_311_1571_0 [ 2 ] ) * _rtP -> P_1855 ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_kx
!= 0 ) { _rtB -> B_311_2082_0 [ 0 ] = _rtDW -> Integ4_DSTATE_ey [ 0 ] ; _rtB
-> B_311_2082_0 [ 1 ] = _rtDW -> Integ4_DSTATE_ey [ 1 ] ; } else { _rtB ->
B_311_2082_0 [ 0 ] = _rtP -> P_1856 * _rtB -> B_311_2075_0 + _rtDW ->
Integ4_DSTATE_ey [ 0 ] ; _rtB -> B_311_2082_0 [ 1 ] = _rtP -> P_1856 * _rtB
-> B_311_2081_0 + _rtDW -> Integ4_DSTATE_ey [ 1 ] ; } _rtB -> B_311_2083_0 =
_rtP -> P_1858 ; ssCallAccelRunBlock ( S , 311 , 2084 , SS_CALL_MDL_OUTPUTS )
; if ( ssGetTaskTime ( S , 3 ) < _rtP -> P_1861 ) { rtb_B_299_3_0 = _rtP ->
P_1862 ; } else { rtb_B_299_3_0 = _rtP -> P_1863 ; } if ( rtb_B_299_3_0 >=
_rtP -> P_1865 ) { B_311_1965_0_idx_0 = ( _rtB -> B_311_2082_0 [ 0 ] - _rtB
-> B_311_2084_0 [ 0 ] ) * _rtP -> P_1859 + ( _rtP -> P_84 * _rtB ->
B_311_2075_0 - _rtP -> P_83 * _rtDW -> UnitDelay_DSTATE_gj [ 0 ] ) ;
B_311_1965_0_idx_1 = ( _rtB -> B_311_2082_0 [ 1 ] - _rtB -> B_311_2084_0 [ 1
] ) * _rtP -> P_1859 + ( _rtP -> P_84 * _rtB -> B_311_2081_0 - _rtP -> P_83 *
_rtDW -> UnitDelay_DSTATE_gj [ 1 ] ) ; } else { B_311_1965_0_idx_0 = _rtP ->
P_1864 ; B_311_1965_0_idx_1 = _rtP -> P_1864 ; } _rtB -> B_311_2091_0 [ 0 ] =
_rtP -> P_1866 * B_311_1965_0_idx_0 ; _rtB -> B_311_2091_0 [ 1 ] = _rtP ->
P_1866 * B_311_1965_0_idx_1 ; ssCallAccelRunBlock ( S , 311 , 2092 ,
SS_CALL_MDL_OUTPUTS ) ; if ( _rtB -> B_311_1962_0 ) { _rtB -> B_311_2093_0 [
0 ] = _rtB -> B_311_2061_0 [ 0 ] ; _rtB -> B_311_2093_0 [ 1 ] = _rtB ->
B_311_2061_0 [ 1 ] ; } else { _rtB -> B_311_2093_0 [ 0 ] = _rtB ->
B_311_1969_0 ; _rtB -> B_311_2093_0 [ 1 ] = _rtB -> B_311_1976_0 ; }
ssCallAccelRunBlock ( S , 311 , 2094 , SS_CALL_MDL_OUTPUTS ) ;
B_311_1965_0_idx_0 = _rtDW -> UnitDelay1_DSTATE_o0 ; _rtB -> B_311_2096_0 = !
( _rtB -> B_311_1905_0 != 0.0 ) ; _rtB -> B_311_2097_0 = _rtDW ->
UnitDelay1_DSTATE_ft ; rtb_B_311_2022_0 = muDoubleScalarSin ( _rtB ->
B_311_2097_0 ) ; rtb_B_311_2027_0 = muDoubleScalarCos ( _rtB -> B_311_2097_0
) ; rtb_B_311_2029_0 = ( 0.0 - rtb_B_311_2022_0 * _rtB -> B_311_139_0 ) -
rtb_B_311_2027_0 * _rtB -> B_311_140_0 ; rtb_B_311_2031_0 = rtb_B_311_2022_0
* _rtB -> B_311_140_0 - rtb_B_311_2027_0 * _rtB -> B_311_139_0 ; _rtB ->
B_311_2108_0 = ( ( ( 0.0 - rtb_B_311_2031_0 ) - rtb_B_311_2027_0 ) * _rtB ->
B_311_1589_0 [ 2 ] + ( _rtB -> B_311_1589_0 [ 0 ] * rtb_B_311_2027_0 + _rtB
-> B_311_1589_0 [ 1 ] * rtb_B_311_2031_0 ) ) * 0.66666666666666663 ;
rtb_B_311_2022_0 = ( ( ( 0.0 - rtb_B_311_2029_0 ) - rtb_B_311_2022_0 ) * _rtB
-> B_311_1589_0 [ 2 ] + ( _rtB -> B_311_1589_0 [ 0 ] * rtb_B_311_2022_0 +
_rtB -> B_311_1589_0 [ 1 ] * rtb_B_311_2029_0 ) ) * 0.66666666666666663 ;
rtb_B_311_2022_0 = ( ( _rtB -> B_311_128_0 <= _rtB -> B_311_2108_0 ) && (
_rtB -> B_311_2108_0 <= _rtB -> B_311_129_0 ) && ( ( _rtB -> B_311_130_0 <=
rtb_B_311_2022_0 ) && ( rtb_B_311_2022_0 <= _rtB -> B_311_131_0 ) ) ) ;
rtb_B_311_2027_0 = _rtDW -> Delay_DSTATE_cw ; _rtB -> B_311_2120_0 = ( ( (
rtb_B_311_2022_0 != 0.0 ) == ( _rtDW -> Delay_DSTATE_cw != 0.0 ) ) || ( _rtB
-> B_311_132_0 != 0.0 ) ) ; _rtB -> B_311_2122_0 = ( _rtB -> B_311_135_0 && (
_rtDW -> Delay_DSTATE_cw != 0.0 ) ) ; if ( _rtDW ->
DiscreteTimeIntegrator_IC_LOADING_dn != 0 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 = _rtB -> B_311_2122_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 >= _rtP -> P_1871 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 = _rtP -> P_1871 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 <= _rtP -> P_1872 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 = _rtP -> P_1872 ; } } } if ( _rtB ->
B_311_2120_0 || ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_gz != 0 ) )
{ _rtDW -> DiscreteTimeIntegrator_DSTATE_a5 = _rtB -> B_311_2122_0 ; if (
_rtDW -> DiscreteTimeIntegrator_DSTATE_a5 >= _rtP -> P_1871 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 = _rtP -> P_1871 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 <= _rtP -> P_1872 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 = _rtP -> P_1872 ; } } } if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 >= _rtP -> P_1871 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 = _rtP -> P_1871 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 <= _rtP -> P_1872 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 = _rtP -> P_1872 ; } } _rtDW -> Relay_Mode_m
= ( ( _rtDW -> DiscreteTimeIntegrator_DSTATE_a5 >= _rtP -> P_1873 ) || ( ( !
( _rtDW -> DiscreteTimeIntegrator_DSTATE_a5 <= _rtP -> P_1874 ) ) && _rtDW ->
Relay_Mode_m ) ) ; if ( _rtDW -> Relay_Mode_m ) { _rtB -> B_311_2124_0 = _rtP
-> P_1875 ; } else { _rtB -> B_311_2124_0 = _rtP -> P_1876 ; } _rtB ->
B_311_2129_0 = ( ( _rtB -> B_311_2096_0 && ( _rtB -> B_311_2124_0 != 0.0 ) )
|| ( ( ! _rtB -> B_311_2096_0 ) && ( ! ( _rtB -> B_155_5_16 != 0.0 ) ) ) ) ;
if ( ! _rtB -> B_311_2129_0 ) { _rtB -> B_311_2131_0 = _rtDW ->
UnitDelay1_DSTATE_o0 ; } else { _rtB -> B_311_2131_0 = _rtB -> B_311_2097_0 ;
} rtb_B_311_2031_0 = _rtB -> B_311_2131_0 + _rtP -> P_1877 ; rtb_B_311_2029_0
= muDoubleScalarSin ( rtb_B_311_2031_0 ) ; rtb_B_311_1540_0 =
muDoubleScalarCos ( rtb_B_311_2031_0 ) ; rtb_B_311_1543_0 = ( 0.0 -
rtb_B_311_2029_0 * _rtB -> B_311_124_0 ) - rtb_B_311_1540_0 * _rtB ->
B_311_125_0 ; rtb_B_311_1544_0 = rtb_B_311_2029_0 * _rtB -> B_311_125_0 -
rtb_B_311_1540_0 * _rtB -> B_311_124_0 ; rtb_B_311_2031_0 = ( _rtB ->
B_311_2093_0 [ 0 ] * rtb_B_311_2029_0 + _rtB -> B_311_2093_0 [ 1 ] *
rtb_B_311_1540_0 ) + _rtB -> B_311_121_0 ; rtb_B_311_1545_0 = ( _rtB ->
B_311_2093_0 [ 0 ] * rtb_B_311_1543_0 + _rtB -> B_311_2093_0 [ 1 ] *
rtb_B_311_1544_0 ) + _rtB -> B_311_121_0 ; rtb_B_311_2029_0 = ( ( ( 0.0 -
rtb_B_311_1543_0 ) - rtb_B_311_2029_0 ) * _rtB -> B_311_2093_0 [ 0 ] + ( (
0.0 - rtb_B_311_1544_0 ) - rtb_B_311_1540_0 ) * _rtB -> B_311_2093_0 [ 1 ] )
+ _rtB -> B_311_121_0 ; if ( rtb_B_311_2031_0 > _rtB -> B_311_122_0 ) {
rtb_B_311_2031_0 = _rtB -> B_311_122_0 ; } else { if ( rtb_B_311_2031_0 <
_rtB -> B_311_123_0 ) { rtb_B_311_2031_0 = _rtB -> B_311_123_0 ; } } _rtB ->
B_311_2150_0 [ 0 ] = _rtP -> P_1878 * rtb_B_311_2031_0 ; if (
rtb_B_311_1545_0 > _rtB -> B_311_122_0 ) { rtb_B_311_1545_0 = _rtB ->
B_311_122_0 ; } else { if ( rtb_B_311_1545_0 < _rtB -> B_311_123_0 ) {
rtb_B_311_1545_0 = _rtB -> B_311_123_0 ; } } _rtB -> B_311_2150_0 [ 1 ] =
_rtP -> P_1878 * rtb_B_311_1545_0 ; if ( rtb_B_311_2029_0 > _rtB ->
B_311_122_0 ) { rtb_B_311_2029_0 = _rtB -> B_311_122_0 ; } else { if (
rtb_B_311_2029_0 < _rtB -> B_311_123_0 ) { rtb_B_311_2029_0 = _rtB ->
B_311_123_0 ; } } _rtB -> B_311_2150_0 [ 2 ] = _rtP -> P_1878 *
rtb_B_311_2029_0 ; ssCallAccelRunBlock ( S , 311 , 2151 , SS_CALL_MDL_OUTPUTS
) ; ssCallAccelRunBlock ( S , 311 , 2152 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2153 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2154 , SS_CALL_MDL_OUTPUTS ) ; if ( _rtB ->
B_311_2033_0 [ 0 ] > _rtP -> P_1879 ) { _rtB -> B_311_2155_0 = _rtP -> P_1879
; } else if ( _rtB -> B_311_2033_0 [ 0 ] < _rtP -> P_1880 ) { _rtB ->
B_311_2155_0 = _rtP -> P_1880 ; } else { _rtB -> B_311_2155_0 = _rtB ->
B_311_2033_0 [ 0 ] ; } ssCallAccelRunBlock ( S , 311 , 2156 ,
SS_CALL_MDL_OUTPUTS ) ; if ( rtb_B_311_2027_0 >= _rtP -> P_1882 ) {
rtb_B_311_2027_0 = _rtB -> B_311_133_0 ; } else { rtb_B_311_2027_0 = _rtB ->
B_311_134_0 ; } _rtB -> B_311_2159_0 = ( rtb_B_311_2022_0 + _rtP -> P_1881 )
* rtb_B_311_2027_0 ; rtb_B_311_35_0 = _rtB -> B_311_136_0 * _rtB ->
B_311_2108_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_g ; if ( rtb_B_311_35_0
> _rtP -> P_1887 ) { _rtB -> B_311_2163_0 = _rtP -> P_1887 ; } else if (
rtb_B_311_35_0 < _rtP -> P_1888 ) { _rtB -> B_311_2163_0 = _rtP -> P_1888 ; }
else { _rtB -> B_311_2163_0 = rtb_B_311_35_0 ; } ssCallAccelRunBlock ( S ,
311 , 2164 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_311_2165_0 = _rtB ->
B_311_2108_0 * _rtB -> B_311_137_0 ; _rtB -> B_311_2166_0 = _rtP -> P_1889 *
_rtB -> B_311_2163_0 ; { _rtB -> B_311_2167_0 = ( _rtP -> P_1892 [ 0 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_pq [ 0 ] + ( _rtP -> P_1892 [ 1 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_pq [ 1 ] ; _rtB -> B_311_2167_0 += _rtP ->
P_1893 * _rtB -> B_311_2166_0 ; } _rtB -> B_311_2172_0 = ( _rtP -> P_1896 *
_rtB -> B_311_2163_0 - ( real_T ) ( _rtB -> B_311_2097_0 >= _rtB ->
B_311_138_0 ) * _rtP -> P_1895 ) + _rtB -> B_311_2097_0 ; _rtB ->
B_311_2173_0 = muDoubleScalarSin ( _rtB -> B_311_2131_0 ) ; _rtB ->
B_311_2174_0 = muDoubleScalarCos ( _rtB -> B_311_2131_0 ) ; if ( _rtB ->
B_311_2129_0 ) { _rtB -> B_311_2176_0 = _rtB -> B_311_2097_0 ; } else { _rtB
-> B_311_2176_0 = ( _rtP -> P_85 * _rtB -> B_311_2155_0 * _rtP -> P_86 - (
real_T ) ( B_311_1965_0_idx_0 >= _rtB -> B_311_141_0 ) * _rtP -> P_87 ) +
B_311_1965_0_idx_0 ; } ssCallAccelRunBlock ( S , 311 , 2177 ,
SS_CALL_MDL_OUTPUTS ) ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
_rtB -> B_311_2180_0 = ( ( _rtDW -> Integrator_DSTATE_lv < _rtB ->
B_311_145_0_n ) || ( _rtDW -> Integrator_DSTATE_lv >= _rtB -> B_311_146_0 ) )
; if ( _rtDW -> Initial_FirstOutputTime_d ) { _rtDW ->
Initial_FirstOutputTime_d = false ; _rtB -> B_311_2186_0 = _rtP -> P_1899 ; }
else { _rtB -> B_311_2186_0 = _rtDW -> Integrator_DSTATE_lv -
muDoubleScalarFloor ( ( _rtDW -> Integrator_DSTATE_lv - _rtB -> B_311_147_0 )
* _rtP -> P_1897 ) * _rtP -> P_1898 ; } if ( _rtB -> B_311_2180_0 && ( _rtDW
-> Integrator_PrevResetState_f3 <= 0 ) ) { _rtDW -> Integrator_DSTATE_lv =
_rtB -> B_311_2186_0 ; } muDoubleScalarSinCos ( _rtDW -> Integrator_DSTATE_lv
, & B_311_1965_0_idx_0 , & rtb_B_311_2022_0 ) ; _rtB -> B_311_2189_0 [ 0 ] =
_rtB -> B_311_1944_0 [ 0 ] * B_311_1965_0_idx_0 ; _rtB -> B_311_2189_0 [ 1 ]
= _rtB -> B_311_1944_0 [ 0 ] * rtb_B_311_2022_0 ; _rtB -> B_311_2206_0 [ 0 ]
= _rtB -> B_311_1944_0 [ 1 ] * B_311_1965_0_idx_0 ; _rtB -> B_311_2206_0 [ 1
] = _rtB -> B_311_1944_0 [ 1 ] * rtb_B_311_2022_0 ; _rtB -> B_311_2223_0 [ 0
] = _rtB -> B_311_1944_0 [ 2 ] * B_311_1965_0_idx_0 ; _rtB -> B_311_2223_0 [
1 ] = _rtB -> B_311_1944_0 [ 2 ] * rtb_B_311_2022_0 ; B_311_1349_0_idx_0 =
_rtP -> P_1928 * _rtB -> B_311_152_0 [ 179 ] * _rtP -> P_1931 ;
B_311_1349_0_idx_1 = _rtP -> P_1929 * _rtB -> B_311_152_0 [ 180 ] * _rtP ->
P_1931 ; B_311_1349_0_idx_2 = _rtP -> P_1930 * _rtB -> B_311_152_0 [ 181 ] *
_rtP -> P_1931 ; _rtB -> B_311_2244_0 [ 0 ] = B_311_1349_0_idx_0 *
B_311_1965_0_idx_0 ; _rtB -> B_311_2244_0 [ 1 ] = B_311_1349_0_idx_0 *
rtb_B_311_2022_0 ; _rtB -> B_311_2261_0 [ 0 ] = B_311_1349_0_idx_1 *
B_311_1965_0_idx_0 ; _rtB -> B_311_2261_0 [ 1 ] = B_311_1349_0_idx_1 *
rtb_B_311_2022_0 ; _rtB -> B_311_2278_0 [ 0 ] = B_311_1349_0_idx_2 *
B_311_1965_0_idx_0 ; _rtB -> B_311_2278_0 [ 1 ] = B_311_1349_0_idx_2 *
rtb_B_311_2022_0 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_c != 0 ) { _rtB ->
B_311_2190_0 [ 0 ] = _rtDW -> Integrator_DSTATE_j5 [ 0 ] ; } else { _rtB ->
B_311_2190_0 [ 0 ] = _rtP -> P_1901 * _rtB -> B_311_2189_0 [ 0 ] + _rtDW ->
Integrator_DSTATE_j5 [ 0 ] ; } _rtB -> B_311_2197_0 [ 0 ] = _rtDW ->
Delay_DSTATE_jl [ 0 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_j5 != 0 ) {
_rtB -> B_311_2207_0 [ 0 ] = _rtDW -> Integrator_DSTATE_og [ 0 ] ; } else {
_rtB -> B_311_2207_0 [ 0 ] = _rtP -> P_1910 * _rtB -> B_311_2206_0 [ 0 ] +
_rtDW -> Integrator_DSTATE_og [ 0 ] ; } _rtB -> B_311_2214_0 [ 0 ] = _rtDW ->
Delay_DSTATE_dk [ 0 ] ; B_311_1537_0_idx_0 = ( ( ( ( _rtB -> B_311_2207_0 [ 0
] - _rtDW -> UnitDelay1_DSTATE_p2c [ 0 ] ) * _rtP -> P_1913 + _rtDW ->
UnitDelay1_DSTATE_p2c [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_ew [ 0 ] ) + (
_rtB -> B_311_2207_0 [ 0 ] - ( ( _rtB -> B_311_2214_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_ew [ 0 ] ) * _rtP -> P_1916 + _rtDW -> UnitDelay2_DSTATE_ew
[ 0 ] ) ) ) * _rtP -> P_1917 * _rtP -> P_1918 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_lk != 0 ) { _rtB -> B_311_2224_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_gv [ 0 ] ; } else { _rtB -> B_311_2224_0 [ 0 ] = _rtP ->
P_1919 * _rtB -> B_311_2223_0 [ 0 ] + _rtDW -> Integrator_DSTATE_gv [ 0 ] ; }
_rtB -> B_311_2231_0 [ 0 ] = _rtDW -> Delay_DSTATE_fs [ 0 ] ;
B_311_1940_0_idx_0 = ( ( ( ( _rtB -> B_311_2224_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_ow [ 0 ] ) * _rtP -> P_1922 + _rtDW -> UnitDelay1_DSTATE_ow
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_d [ 0 ] ) + ( _rtB -> B_311_2224_0 [ 0 ]
- ( ( _rtB -> B_311_2231_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_d [ 0 ] ) *
_rtP -> P_1925 + _rtDW -> UnitDelay2_DSTATE_d [ 0 ] ) ) ) * _rtP -> P_1926 *
_rtP -> P_1927 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_o4 != 0 ) { _rtB ->
B_311_2245_0 [ 0 ] = _rtDW -> Integrator_DSTATE_ko [ 0 ] ; } else { _rtB ->
B_311_2245_0 [ 0 ] = _rtP -> P_1932 * _rtB -> B_311_2244_0 [ 0 ] + _rtDW ->
Integrator_DSTATE_ko [ 0 ] ; } _rtB -> B_311_2252_0 [ 0 ] = _rtDW ->
Delay_DSTATE_mr [ 0 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_ef != 0 ) {
_rtB -> B_311_2262_0 [ 0 ] = _rtDW -> Integrator_DSTATE_pki [ 0 ] ; } else {
_rtB -> B_311_2262_0 [ 0 ] = _rtP -> P_1941 * _rtB -> B_311_2261_0 [ 0 ] +
_rtDW -> Integrator_DSTATE_pki [ 0 ] ; } _rtB -> B_311_2269_0 [ 0 ] = _rtDW
-> Delay_DSTATE_pr [ 0 ] ; B_311_1586_0_idx_0 = ( ( ( ( _rtB -> B_311_2262_0
[ 0 ] - _rtDW -> UnitDelay1_DSTATE_j1 [ 0 ] ) * _rtP -> P_1944 + _rtDW ->
UnitDelay1_DSTATE_j1 [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_e2 [ 0 ] ) + ( _rtB
-> B_311_2262_0 [ 0 ] - ( ( _rtB -> B_311_2269_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_e2 [ 0 ] ) * _rtP -> P_1947 + _rtDW -> UnitDelay2_DSTATE_e2
[ 0 ] ) ) ) * _rtP -> P_1948 * _rtP -> P_1949 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_b3 != 0 ) { _rtB -> B_311_2279_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_bz [ 0 ] ; } else { _rtB -> B_311_2279_0 [ 0 ] = _rtP ->
P_1950 * _rtB -> B_311_2278_0 [ 0 ] + _rtDW -> Integrator_DSTATE_bz [ 0 ] ; }
_rtB -> B_311_2286_0 [ 0 ] = _rtDW -> Delay_DSTATE_cvb [ 0 ] ;
B_311_1556_0_idx_0 = ( ( ( ( _rtB -> B_311_2279_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_gu [ 0 ] ) * _rtP -> P_1953 + _rtDW -> UnitDelay1_DSTATE_gu
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_cd [ 0 ] ) + ( _rtB -> B_311_2279_0 [ 0
] - ( ( _rtB -> B_311_2286_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_cd [ 0 ] ) *
_rtP -> P_1956 + _rtDW -> UnitDelay2_DSTATE_cd [ 0 ] ) ) ) * _rtP -> P_1957 *
_rtP -> P_1958 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_c != 0 ) { _rtB ->
B_311_2190_0 [ 1 ] = _rtDW -> Integrator_DSTATE_j5 [ 1 ] ; } else { _rtB ->
B_311_2190_0 [ 1 ] = _rtP -> P_1901 * _rtB -> B_311_2189_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_j5 [ 1 ] ; } _rtB -> B_311_2197_0 [ 1 ] = _rtDW ->
Delay_DSTATE_jl [ 1 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_j5 != 0 ) {
_rtB -> B_311_2207_0 [ 1 ] = _rtDW -> Integrator_DSTATE_og [ 1 ] ; } else {
_rtB -> B_311_2207_0 [ 1 ] = _rtP -> P_1910 * _rtB -> B_311_2206_0 [ 1 ] +
_rtDW -> Integrator_DSTATE_og [ 1 ] ; } _rtB -> B_311_2214_0 [ 1 ] = _rtDW ->
Delay_DSTATE_dk [ 1 ] ; B_311_1537_0_idx_1 = ( ( ( ( _rtB -> B_311_2207_0 [ 1
] - _rtDW -> UnitDelay1_DSTATE_p2c [ 1 ] ) * _rtP -> P_1913 + _rtDW ->
UnitDelay1_DSTATE_p2c [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_ew [ 1 ] ) + (
_rtB -> B_311_2207_0 [ 1 ] - ( ( _rtB -> B_311_2214_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_ew [ 1 ] ) * _rtP -> P_1916 + _rtDW -> UnitDelay2_DSTATE_ew
[ 1 ] ) ) ) * _rtP -> P_1917 * _rtP -> P_1918 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_lk != 0 ) { _rtB -> B_311_2224_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_gv [ 1 ] ; } else { _rtB -> B_311_2224_0 [ 1 ] = _rtP ->
P_1919 * _rtB -> B_311_2223_0 [ 1 ] + _rtDW -> Integrator_DSTATE_gv [ 1 ] ; }
_rtB -> B_311_2231_0 [ 1 ] = _rtDW -> Delay_DSTATE_fs [ 1 ] ;
B_311_1940_0_idx_1 = ( ( ( ( _rtB -> B_311_2224_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_ow [ 1 ] ) * _rtP -> P_1922 + _rtDW -> UnitDelay1_DSTATE_ow
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_d [ 1 ] ) + ( _rtB -> B_311_2224_0 [ 1 ]
- ( ( _rtB -> B_311_2231_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_d [ 1 ] ) *
_rtP -> P_1925 + _rtDW -> UnitDelay2_DSTATE_d [ 1 ] ) ) ) * _rtP -> P_1926 *
_rtP -> P_1927 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_o4 != 0 ) { _rtB ->
B_311_2245_0 [ 1 ] = _rtDW -> Integrator_DSTATE_ko [ 1 ] ; } else { _rtB ->
B_311_2245_0 [ 1 ] = _rtP -> P_1932 * _rtB -> B_311_2244_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_ko [ 1 ] ; } _rtB -> B_311_2252_0 [ 1 ] = _rtDW ->
Delay_DSTATE_mr [ 1 ] ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_ef != 0 ) {
_rtB -> B_311_2262_0 [ 1 ] = _rtDW -> Integrator_DSTATE_pki [ 1 ] ; } else {
_rtB -> B_311_2262_0 [ 1 ] = _rtP -> P_1941 * _rtB -> B_311_2261_0 [ 1 ] +
_rtDW -> Integrator_DSTATE_pki [ 1 ] ; } _rtB -> B_311_2269_0 [ 1 ] = _rtDW
-> Delay_DSTATE_pr [ 1 ] ; B_311_1586_0_idx_1 = ( ( ( ( _rtB -> B_311_2262_0
[ 1 ] - _rtDW -> UnitDelay1_DSTATE_j1 [ 1 ] ) * _rtP -> P_1944 + _rtDW ->
UnitDelay1_DSTATE_j1 [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_e2 [ 1 ] ) + ( _rtB
-> B_311_2262_0 [ 1 ] - ( ( _rtB -> B_311_2269_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_e2 [ 1 ] ) * _rtP -> P_1947 + _rtDW -> UnitDelay2_DSTATE_e2
[ 1 ] ) ) ) * _rtP -> P_1948 * _rtP -> P_1949 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_b3 != 0 ) { _rtB -> B_311_2279_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_bz [ 1 ] ; } else { _rtB -> B_311_2279_0 [ 1 ] = _rtP ->
P_1950 * _rtB -> B_311_2278_0 [ 1 ] + _rtDW -> Integrator_DSTATE_bz [ 1 ] ; }
_rtB -> B_311_2286_0 [ 1 ] = _rtDW -> Delay_DSTATE_cvb [ 1 ] ;
B_311_1556_0_idx_1 = ( ( ( ( _rtB -> B_311_2279_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_gu [ 1 ] ) * _rtP -> P_1953 + _rtDW -> UnitDelay1_DSTATE_gu
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_cd [ 1 ] ) + ( _rtB -> B_311_2279_0 [ 1
] - ( ( _rtB -> B_311_2286_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_cd [ 1 ] ) *
_rtP -> P_1956 + _rtDW -> UnitDelay2_DSTATE_cd [ 1 ] ) ) ) * _rtP -> P_1957 *
_rtP -> P_1958 ; switch ( ( int32_T ) _rtB -> B_311_144_0_b ) { case 1 :
rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_2190_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_et [ 0 ] ) * _rtP -> P_1904 + _rtDW -> UnitDelay1_DSTATE_et
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_ne [ 0 ] ) + ( _rtB -> B_311_2190_0 [ 0
] - ( ( _rtB -> B_311_2197_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_ne [ 0 ] ) *
_rtP -> P_1907 + _rtDW -> UnitDelay2_DSTATE_ne [ 0 ] ) ) ) * _rtP -> P_1908 *
_rtP -> P_1909 + ( _rtP -> P_4192 . re * B_311_1537_0_idx_0 - _rtP -> P_4192
. im * B_311_1537_0_idx_1 ) ) + ( _rtP -> P_4191 . re * B_311_1940_0_idx_0 -
_rtP -> P_4191 . im * B_311_1940_0_idx_1 ) ) * _rtP -> P_104 ; rtb_B_311_52_0
= ( ( ( ( ( ( _rtB -> B_311_2190_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_et [ 1
] ) * _rtP -> P_1904 + _rtDW -> UnitDelay1_DSTATE_et [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_ne [ 1 ] ) + ( _rtB -> B_311_2190_0 [ 1 ] - ( ( _rtB ->
B_311_2197_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_ne [ 1 ] ) * _rtP -> P_1907 +
_rtDW -> UnitDelay2_DSTATE_ne [ 1 ] ) ) ) * _rtP -> P_1908 * _rtP -> P_1909 +
( _rtP -> P_4192 . re * B_311_1537_0_idx_1 + _rtP -> P_4192 . im *
B_311_1537_0_idx_0 ) ) + ( _rtP -> P_4191 . re * B_311_1940_0_idx_1 + _rtP ->
P_4191 . im * B_311_1940_0_idx_0 ) ) * _rtP -> P_104 ; rtb_B_311_68_0 = ( ( (
( ( ( _rtB -> B_311_2245_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_kp [ 0 ] ) *
_rtP -> P_1935 + _rtDW -> UnitDelay1_DSTATE_kp [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_nw [ 0 ] ) + ( _rtB -> B_311_2245_0 [ 0 ] - ( ( _rtB ->
B_311_2252_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_nw [ 0 ] ) * _rtP -> P_1938 +
_rtDW -> UnitDelay2_DSTATE_nw [ 0 ] ) ) ) * _rtP -> P_1939 * _rtP -> P_1940 +
( _rtP -> P_4190 . re * B_311_1586_0_idx_0 - _rtP -> P_4190 . im *
B_311_1586_0_idx_1 ) ) + ( _rtP -> P_4189 . re * B_311_1556_0_idx_0 - _rtP ->
P_4189 . im * B_311_1556_0_idx_1 ) ) * _rtP -> P_103 ; rtb_B_299_3_0 = - ( (
( ( ( ( ( _rtB -> B_311_2245_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_kp [ 1 ] )
* _rtP -> P_1935 + _rtDW -> UnitDelay1_DSTATE_kp [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_nw [ 1 ] ) + ( _rtB -> B_311_2245_0 [ 1 ] - ( ( _rtB ->
B_311_2252_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_nw [ 1 ] ) * _rtP -> P_1938 +
_rtDW -> UnitDelay2_DSTATE_nw [ 1 ] ) ) ) * _rtP -> P_1939 * _rtP -> P_1940 +
( _rtP -> P_4190 . re * B_311_1586_0_idx_1 + _rtP -> P_4190 . im *
B_311_1586_0_idx_0 ) ) + ( _rtP -> P_4189 . re * B_311_1556_0_idx_1 + _rtP ->
P_4189 . im * B_311_1556_0_idx_0 ) ) * _rtP -> P_103 ) ; _rtB -> B_311_2299_0
= ( rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP
-> P_105 ; _rtB -> B_311_2299_1 = ( rtb_B_311_37_0 * rtb_B_299_3_0 +
rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP -> P_105 ; break ; case 2 :
rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_2190_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_et [ 0 ] ) * _rtP -> P_1904 + _rtDW -> UnitDelay1_DSTATE_et
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_ne [ 0 ] ) + ( _rtB -> B_311_2190_0 [ 0
] - ( ( _rtB -> B_311_2197_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_ne [ 0 ] ) *
_rtP -> P_1907 + _rtDW -> UnitDelay2_DSTATE_ne [ 0 ] ) ) ) * _rtP -> P_1908 *
_rtP -> P_1909 + ( _rtP -> P_4188 . re * B_311_1537_0_idx_0 - _rtP -> P_4188
. im * B_311_1537_0_idx_1 ) ) + ( _rtP -> P_4187 . re * B_311_1940_0_idx_0 -
_rtP -> P_4187 . im * B_311_1940_0_idx_1 ) ) * _rtP -> P_101 ; rtb_B_311_52_0
= ( ( ( ( ( ( _rtB -> B_311_2190_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_et [ 1
] ) * _rtP -> P_1904 + _rtDW -> UnitDelay1_DSTATE_et [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_ne [ 1 ] ) + ( _rtB -> B_311_2190_0 [ 1 ] - ( ( _rtB ->
B_311_2197_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_ne [ 1 ] ) * _rtP -> P_1907 +
_rtDW -> UnitDelay2_DSTATE_ne [ 1 ] ) ) ) * _rtP -> P_1908 * _rtP -> P_1909 +
( _rtP -> P_4188 . re * B_311_1537_0_idx_1 + _rtP -> P_4188 . im *
B_311_1537_0_idx_0 ) ) + ( _rtP -> P_4187 . re * B_311_1940_0_idx_1 + _rtP ->
P_4187 . im * B_311_1940_0_idx_0 ) ) * _rtP -> P_101 ; rtb_B_311_68_0 = ( ( (
( ( ( _rtB -> B_311_2245_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_kp [ 0 ] ) *
_rtP -> P_1935 + _rtDW -> UnitDelay1_DSTATE_kp [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_nw [ 0 ] ) + ( _rtB -> B_311_2245_0 [ 0 ] - ( ( _rtB ->
B_311_2252_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_nw [ 0 ] ) * _rtP -> P_1938 +
_rtDW -> UnitDelay2_DSTATE_nw [ 0 ] ) ) ) * _rtP -> P_1939 * _rtP -> P_1940 +
( _rtP -> P_4186 . re * B_311_1586_0_idx_0 - _rtP -> P_4186 . im *
B_311_1586_0_idx_1 ) ) + ( _rtP -> P_4185 . re * B_311_1556_0_idx_0 - _rtP ->
P_4185 . im * B_311_1556_0_idx_1 ) ) * _rtP -> P_100 ; rtb_B_299_3_0 = - ( (
( ( ( ( ( _rtB -> B_311_2245_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_kp [ 1 ] )
* _rtP -> P_1935 + _rtDW -> UnitDelay1_DSTATE_kp [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_nw [ 1 ] ) + ( _rtB -> B_311_2245_0 [ 1 ] - ( ( _rtB ->
B_311_2252_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_nw [ 1 ] ) * _rtP -> P_1938 +
_rtDW -> UnitDelay2_DSTATE_nw [ 1 ] ) ) ) * _rtP -> P_1939 * _rtP -> P_1940 +
( _rtP -> P_4186 . re * B_311_1586_0_idx_1 + _rtP -> P_4186 . im *
B_311_1586_0_idx_0 ) ) + ( _rtP -> P_4185 . re * B_311_1556_0_idx_1 + _rtP ->
P_4185 . im * B_311_1556_0_idx_0 ) ) * _rtP -> P_100 ) ; _rtB -> B_311_2299_0
= ( rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP
-> P_102 ; _rtB -> B_311_2299_1 = ( rtb_B_311_37_0 * rtb_B_299_3_0 +
rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP -> P_102 ; break ; default :
rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_2190_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_et [ 0 ] ) * _rtP -> P_1904 + _rtDW -> UnitDelay1_DSTATE_et
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_ne [ 0 ] ) + ( _rtB -> B_311_2190_0 [ 0
] - ( ( _rtB -> B_311_2197_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_ne [ 0 ] ) *
_rtP -> P_1907 + _rtDW -> UnitDelay2_DSTATE_ne [ 0 ] ) ) ) * _rtP -> P_1908 *
_rtP -> P_1909 + B_311_1537_0_idx_0 ) + B_311_1940_0_idx_0 ) * _rtP -> P_98 ;
rtb_B_311_52_0 = ( ( ( ( ( ( _rtB -> B_311_2190_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_et [ 1 ] ) * _rtP -> P_1904 + _rtDW -> UnitDelay1_DSTATE_et
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_ne [ 1 ] ) + ( _rtB -> B_311_2190_0 [ 1
] - ( ( _rtB -> B_311_2197_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_ne [ 1 ] ) *
_rtP -> P_1907 + _rtDW -> UnitDelay2_DSTATE_ne [ 1 ] ) ) ) * _rtP -> P_1908 *
_rtP -> P_1909 + B_311_1537_0_idx_1 ) + B_311_1940_0_idx_1 ) * _rtP -> P_98 ;
rtb_B_311_68_0 = ( ( ( ( ( ( _rtB -> B_311_2245_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_kp [ 0 ] ) * _rtP -> P_1935 + _rtDW -> UnitDelay1_DSTATE_kp
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_nw [ 0 ] ) + ( _rtB -> B_311_2245_0 [ 0
] - ( ( _rtB -> B_311_2252_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_nw [ 0 ] ) *
_rtP -> P_1938 + _rtDW -> UnitDelay2_DSTATE_nw [ 0 ] ) ) ) * _rtP -> P_1939 *
_rtP -> P_1940 + B_311_1586_0_idx_0 ) + B_311_1556_0_idx_0 ) * _rtP -> P_97 ;
rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB -> B_311_2245_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_kp [ 1 ] ) * _rtP -> P_1935 + _rtDW -> UnitDelay1_DSTATE_kp
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_nw [ 1 ] ) + ( _rtB -> B_311_2245_0 [ 1
] - ( ( _rtB -> B_311_2252_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_nw [ 1 ] ) *
_rtP -> P_1938 + _rtDW -> UnitDelay2_DSTATE_nw [ 1 ] ) ) ) * _rtP -> P_1939 *
_rtP -> P_1940 + B_311_1586_0_idx_1 ) + B_311_1556_0_idx_1 ) * _rtP -> P_97 )
; _rtB -> B_311_2299_0 = ( rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 *
rtb_B_299_3_0 ) * _rtP -> P_99 ; _rtB -> B_311_2299_1 = ( rtb_B_311_37_0 *
rtb_B_299_3_0 + rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP -> P_99 ; break ; }
ssCallAccelRunBlock ( S , 311 , 2300 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2301 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_311_2304_0 = ( ( _rtDW -> Integrator_DSTATE_d4 < _rtB -> B_311_150_0 ) || (
_rtDW -> Integrator_DSTATE_d4 >= _rtB -> B_311_151_0_h ) ) ; if ( _rtDW ->
Initial_FirstOutputTime_i ) { _rtDW -> Initial_FirstOutputTime_i = false ;
_rtB -> B_311_2310_0 = _rtP -> P_1961 ; } else { _rtB -> B_311_2310_0 = _rtDW
-> Integrator_DSTATE_d4 - muDoubleScalarFloor ( ( _rtDW ->
Integrator_DSTATE_d4 - _rtB -> B_311_152_0_b ) * _rtP -> P_1959 ) * _rtP ->
P_1960 ; } if ( _rtB -> B_311_2304_0 && ( _rtDW ->
Integrator_PrevResetState_p <= 0 ) ) { _rtDW -> Integrator_DSTATE_d4 = _rtB
-> B_311_2310_0 ; } muDoubleScalarSinCos ( _rtDW -> Integrator_DSTATE_d4 , &
B_311_1965_0_idx_0 , & rtb_B_311_2022_0 ) ; _rtB -> B_311_2313_0 [ 0 ] = _rtB
-> B_311_1983_0 [ 0 ] * B_311_1965_0_idx_0 ; _rtB -> B_311_2313_0 [ 1 ] =
_rtB -> B_311_1983_0 [ 0 ] * rtb_B_311_2022_0 ; _rtB -> B_311_2330_0 [ 0 ] =
_rtB -> B_311_1983_0 [ 1 ] * B_311_1965_0_idx_0 ; _rtB -> B_311_2330_0 [ 1 ]
= _rtB -> B_311_1983_0 [ 1 ] * rtb_B_311_2022_0 ; _rtB -> B_311_2347_0 [ 0 ]
= _rtB -> B_311_1983_0 [ 2 ] * B_311_1965_0_idx_0 ; _rtB -> B_311_2347_0 [ 1
] = _rtB -> B_311_1983_0 [ 2 ] * rtb_B_311_2022_0 ; _rtB -> B_311_2364_0 [ 0
] = _rtB -> B_311_2005_0 [ 0 ] * B_311_1965_0_idx_0 ; _rtB -> B_311_2364_0 [
1 ] = _rtB -> B_311_2005_0 [ 0 ] * rtb_B_311_2022_0 ; _rtB -> B_311_2381_0 [
0 ] = _rtB -> B_311_2005_0 [ 1 ] * B_311_1965_0_idx_0 ; _rtB -> B_311_2381_0
[ 1 ] = _rtB -> B_311_2005_0 [ 1 ] * rtb_B_311_2022_0 ; _rtB -> B_311_2398_0
[ 0 ] = _rtB -> B_311_2005_0 [ 2 ] * B_311_1965_0_idx_0 ; _rtB ->
B_311_2398_0 [ 1 ] = _rtB -> B_311_2005_0 [ 2 ] * rtb_B_311_2022_0 ; if (
_rtDW -> Integrator_SYSTEM_ENABLE_mu != 0 ) { _rtB -> B_311_2314_0 [ 0 ] =
_rtDW -> Integrator_DSTATE_px [ 0 ] ; } else { _rtB -> B_311_2314_0 [ 0 ] =
_rtP -> P_1963 * _rtB -> B_311_2313_0 [ 0 ] + _rtDW -> Integrator_DSTATE_px [
0 ] ; } _rtB -> B_311_2321_0 [ 0 ] = _rtDW -> Delay_DSTATE_ob [ 0 ] ; if (
_rtDW -> Integrator_SYSTEM_ENABLE_n2 != 0 ) { _rtB -> B_311_2331_0 [ 0 ] =
_rtDW -> Integrator_DSTATE_ms [ 0 ] ; } else { _rtB -> B_311_2331_0 [ 0 ] =
_rtP -> P_1972 * _rtB -> B_311_2330_0 [ 0 ] + _rtDW -> Integrator_DSTATE_ms [
0 ] ; } _rtB -> B_311_2338_0 [ 0 ] = _rtDW -> Delay_DSTATE_h4 [ 0 ] ;
B_311_1537_0_idx_0 = ( ( ( ( _rtB -> B_311_2331_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_im [ 0 ] ) * _rtP -> P_1975 + _rtDW -> UnitDelay1_DSTATE_im
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_j2 [ 0 ] ) + ( _rtB -> B_311_2331_0 [ 0
] - ( ( _rtB -> B_311_2338_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_j2 [ 0 ] ) *
_rtP -> P_1978 + _rtDW -> UnitDelay2_DSTATE_j2 [ 0 ] ) ) ) * _rtP -> P_1979 *
_rtP -> P_1980 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_iz != 0 ) { _rtB ->
B_311_2348_0 [ 0 ] = _rtDW -> Integrator_DSTATE_gp [ 0 ] ; } else { _rtB ->
B_311_2348_0 [ 0 ] = _rtP -> P_1981 * _rtB -> B_311_2347_0 [ 0 ] + _rtDW ->
Integrator_DSTATE_gp [ 0 ] ; } _rtB -> B_311_2355_0 [ 0 ] = _rtDW ->
Delay_DSTATE_oox [ 0 ] ; B_311_1940_0_idx_0 = ( ( ( ( _rtB -> B_311_2348_0 [
0 ] - _rtDW -> UnitDelay1_DSTATE_fg [ 0 ] ) * _rtP -> P_1984 + _rtDW ->
UnitDelay1_DSTATE_fg [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_fn [ 0 ] ) + ( _rtB
-> B_311_2348_0 [ 0 ] - ( ( _rtB -> B_311_2355_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_fn [ 0 ] ) * _rtP -> P_1987 + _rtDW -> UnitDelay2_DSTATE_fn
[ 0 ] ) ) ) * _rtP -> P_1988 * _rtP -> P_1989 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_g != 0 ) { _rtB -> B_311_2365_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_lq [ 0 ] ; } else { _rtB -> B_311_2365_0 [ 0 ] = _rtP ->
P_1990 * _rtB -> B_311_2364_0 [ 0 ] + _rtDW -> Integrator_DSTATE_lq [ 0 ] ; }
_rtB -> B_311_2372_0 [ 0 ] = _rtDW -> Delay_DSTATE_lw [ 0 ] ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_ju != 0 ) { _rtB -> B_311_2382_0 [ 0 ] = _rtDW ->
Integrator_DSTATE_ik [ 0 ] ; } else { _rtB -> B_311_2382_0 [ 0 ] = _rtP ->
P_1999 * _rtB -> B_311_2381_0 [ 0 ] + _rtDW -> Integrator_DSTATE_ik [ 0 ] ; }
_rtB -> B_311_2389_0 [ 0 ] = _rtDW -> Delay_DSTATE_ft [ 0 ] ;
B_311_1586_0_idx_0 = ( ( ( ( _rtB -> B_311_2382_0 [ 0 ] - _rtDW ->
UnitDelay1_DSTATE_fw [ 0 ] ) * _rtP -> P_2002 + _rtDW -> UnitDelay1_DSTATE_fw
[ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_mi [ 0 ] ) + ( _rtB -> B_311_2382_0 [ 0
] - ( ( _rtB -> B_311_2389_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_mi [ 0 ] ) *
_rtP -> P_2005 + _rtDW -> UnitDelay2_DSTATE_mi [ 0 ] ) ) ) * _rtP -> P_2006 *
_rtP -> P_2007 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_ek != 0 ) { _rtB ->
B_311_2399_0 [ 0 ] = _rtDW -> Integrator_DSTATE_ba [ 0 ] ; } else { _rtB ->
B_311_2399_0 [ 0 ] = _rtP -> P_2008 * _rtB -> B_311_2398_0 [ 0 ] + _rtDW ->
Integrator_DSTATE_ba [ 0 ] ; } _rtB -> B_311_2406_0 [ 0 ] = _rtDW ->
Delay_DSTATE_cr [ 0 ] ; B_311_1556_0_idx_0 = ( ( ( ( _rtB -> B_311_2399_0 [ 0
] - _rtDW -> UnitDelay1_DSTATE_ly [ 0 ] ) * _rtP -> P_2011 + _rtDW ->
UnitDelay1_DSTATE_ly [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_mg [ 0 ] ) + ( _rtB
-> B_311_2399_0 [ 0 ] - ( ( _rtB -> B_311_2406_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_mg [ 0 ] ) * _rtP -> P_2014 + _rtDW -> UnitDelay2_DSTATE_mg
[ 0 ] ) ) ) * _rtP -> P_2015 * _rtP -> P_2016 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_mu != 0 ) { _rtB -> B_311_2314_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_px [ 1 ] ; } else { _rtB -> B_311_2314_0 [ 1 ] = _rtP ->
P_1963 * _rtB -> B_311_2313_0 [ 1 ] + _rtDW -> Integrator_DSTATE_px [ 1 ] ; }
_rtB -> B_311_2321_0 [ 1 ] = _rtDW -> Delay_DSTATE_ob [ 1 ] ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_n2 != 0 ) { _rtB -> B_311_2331_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_ms [ 1 ] ; } else { _rtB -> B_311_2331_0 [ 1 ] = _rtP ->
P_1972 * _rtB -> B_311_2330_0 [ 1 ] + _rtDW -> Integrator_DSTATE_ms [ 1 ] ; }
_rtB -> B_311_2338_0 [ 1 ] = _rtDW -> Delay_DSTATE_h4 [ 1 ] ;
B_311_1537_0_idx_1 = ( ( ( ( _rtB -> B_311_2331_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_im [ 1 ] ) * _rtP -> P_1975 + _rtDW -> UnitDelay1_DSTATE_im
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_j2 [ 1 ] ) + ( _rtB -> B_311_2331_0 [ 1
] - ( ( _rtB -> B_311_2338_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_j2 [ 1 ] ) *
_rtP -> P_1978 + _rtDW -> UnitDelay2_DSTATE_j2 [ 1 ] ) ) ) * _rtP -> P_1979 *
_rtP -> P_1980 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_iz != 0 ) { _rtB ->
B_311_2348_0 [ 1 ] = _rtDW -> Integrator_DSTATE_gp [ 1 ] ; } else { _rtB ->
B_311_2348_0 [ 1 ] = _rtP -> P_1981 * _rtB -> B_311_2347_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_gp [ 1 ] ; } _rtB -> B_311_2355_0 [ 1 ] = _rtDW ->
Delay_DSTATE_oox [ 1 ] ; B_311_1940_0_idx_1 = ( ( ( ( _rtB -> B_311_2348_0 [
1 ] - _rtDW -> UnitDelay1_DSTATE_fg [ 1 ] ) * _rtP -> P_1984 + _rtDW ->
UnitDelay1_DSTATE_fg [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_fn [ 1 ] ) + ( _rtB
-> B_311_2348_0 [ 1 ] - ( ( _rtB -> B_311_2355_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_fn [ 1 ] ) * _rtP -> P_1987 + _rtDW -> UnitDelay2_DSTATE_fn
[ 1 ] ) ) ) * _rtP -> P_1988 * _rtP -> P_1989 ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_g != 0 ) { _rtB -> B_311_2365_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_lq [ 1 ] ; } else { _rtB -> B_311_2365_0 [ 1 ] = _rtP ->
P_1990 * _rtB -> B_311_2364_0 [ 1 ] + _rtDW -> Integrator_DSTATE_lq [ 1 ] ; }
_rtB -> B_311_2372_0 [ 1 ] = _rtDW -> Delay_DSTATE_lw [ 1 ] ; if ( _rtDW ->
Integrator_SYSTEM_ENABLE_ju != 0 ) { _rtB -> B_311_2382_0 [ 1 ] = _rtDW ->
Integrator_DSTATE_ik [ 1 ] ; } else { _rtB -> B_311_2382_0 [ 1 ] = _rtP ->
P_1999 * _rtB -> B_311_2381_0 [ 1 ] + _rtDW -> Integrator_DSTATE_ik [ 1 ] ; }
_rtB -> B_311_2389_0 [ 1 ] = _rtDW -> Delay_DSTATE_ft [ 1 ] ;
B_311_1586_0_idx_1 = ( ( ( ( _rtB -> B_311_2382_0 [ 1 ] - _rtDW ->
UnitDelay1_DSTATE_fw [ 1 ] ) * _rtP -> P_2002 + _rtDW -> UnitDelay1_DSTATE_fw
[ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_mi [ 1 ] ) + ( _rtB -> B_311_2382_0 [ 1
] - ( ( _rtB -> B_311_2389_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_mi [ 1 ] ) *
_rtP -> P_2005 + _rtDW -> UnitDelay2_DSTATE_mi [ 1 ] ) ) ) * _rtP -> P_2006 *
_rtP -> P_2007 ; if ( _rtDW -> Integrator_SYSTEM_ENABLE_ek != 0 ) { _rtB ->
B_311_2399_0 [ 1 ] = _rtDW -> Integrator_DSTATE_ba [ 1 ] ; } else { _rtB ->
B_311_2399_0 [ 1 ] = _rtP -> P_2008 * _rtB -> B_311_2398_0 [ 1 ] + _rtDW ->
Integrator_DSTATE_ba [ 1 ] ; } _rtB -> B_311_2406_0 [ 1 ] = _rtDW ->
Delay_DSTATE_cr [ 1 ] ; B_311_1556_0_idx_1 = ( ( ( ( _rtB -> B_311_2399_0 [ 1
] - _rtDW -> UnitDelay1_DSTATE_ly [ 1 ] ) * _rtP -> P_2011 + _rtDW ->
UnitDelay1_DSTATE_ly [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_mg [ 1 ] ) + ( _rtB
-> B_311_2399_0 [ 1 ] - ( ( _rtB -> B_311_2406_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_mg [ 1 ] ) * _rtP -> P_2014 + _rtDW -> UnitDelay2_DSTATE_mg
[ 1 ] ) ) ) * _rtP -> P_2015 * _rtP -> P_2016 ; switch ( ( int32_T ) _rtB ->
B_311_149_0_l ) { case 1 : rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_2314_0
[ 0 ] - _rtDW -> UnitDelay1_DSTATE_dqh [ 0 ] ) * _rtP -> P_1966 + _rtDW ->
UnitDelay1_DSTATE_dqh [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_b5 [ 0 ] ) + (
_rtB -> B_311_2314_0 [ 0 ] - ( ( _rtB -> B_311_2321_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_b5 [ 0 ] ) * _rtP -> P_1969 + _rtDW -> UnitDelay2_DSTATE_b5
[ 0 ] ) ) ) * _rtP -> P_1970 * _rtP -> P_1971 + ( _rtP -> P_4184 . re *
B_311_1537_0_idx_0 - _rtP -> P_4184 . im * B_311_1537_0_idx_1 ) ) + ( _rtP ->
P_4183 . re * B_311_1940_0_idx_0 - _rtP -> P_4183 . im * B_311_1940_0_idx_1 )
) * _rtP -> P_95 ; rtb_B_311_52_0 = ( ( ( ( ( ( _rtB -> B_311_2314_0 [ 1 ] -
_rtDW -> UnitDelay1_DSTATE_dqh [ 1 ] ) * _rtP -> P_1966 + _rtDW ->
UnitDelay1_DSTATE_dqh [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_b5 [ 1 ] ) + (
_rtB -> B_311_2314_0 [ 1 ] - ( ( _rtB -> B_311_2321_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_b5 [ 1 ] ) * _rtP -> P_1969 + _rtDW -> UnitDelay2_DSTATE_b5
[ 1 ] ) ) ) * _rtP -> P_1970 * _rtP -> P_1971 + ( _rtP -> P_4184 . re *
B_311_1537_0_idx_1 + _rtP -> P_4184 . im * B_311_1537_0_idx_0 ) ) + ( _rtP ->
P_4183 . re * B_311_1940_0_idx_1 + _rtP -> P_4183 . im * B_311_1940_0_idx_0 )
) * _rtP -> P_95 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB -> B_311_2365_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_ew [ 0 ] ) * _rtP -> P_1993 + _rtDW ->
UnitDelay1_DSTATE_ew [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_lw [ 0 ] ) + ( _rtB
-> B_311_2365_0 [ 0 ] - ( ( _rtB -> B_311_2372_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_lw [ 0 ] ) * _rtP -> P_1996 + _rtDW -> UnitDelay2_DSTATE_lw
[ 0 ] ) ) ) * _rtP -> P_1997 * _rtP -> P_1998 + ( _rtP -> P_4182 . re *
B_311_1586_0_idx_0 - _rtP -> P_4182 . im * B_311_1586_0_idx_1 ) ) + ( _rtP ->
P_4181 . re * B_311_1556_0_idx_0 - _rtP -> P_4181 . im * B_311_1556_0_idx_1 )
) * _rtP -> P_94 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB -> B_311_2365_0 [ 1 ]
- _rtDW -> UnitDelay1_DSTATE_ew [ 1 ] ) * _rtP -> P_1993 + _rtDW ->
UnitDelay1_DSTATE_ew [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_lw [ 1 ] ) + ( _rtB
-> B_311_2365_0 [ 1 ] - ( ( _rtB -> B_311_2372_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_lw [ 1 ] ) * _rtP -> P_1996 + _rtDW -> UnitDelay2_DSTATE_lw
[ 1 ] ) ) ) * _rtP -> P_1997 * _rtP -> P_1998 + ( _rtP -> P_4182 . re *
B_311_1586_0_idx_1 + _rtP -> P_4182 . im * B_311_1586_0_idx_0 ) ) + ( _rtP ->
P_4181 . re * B_311_1556_0_idx_1 + _rtP -> P_4181 . im * B_311_1556_0_idx_0 )
) * _rtP -> P_94 ) ; _rtB -> B_311_2419_0 = ( rtb_B_311_37_0 * rtb_B_311_68_0
- rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP -> P_96 ; _rtB -> B_311_2419_1 = (
rtb_B_311_37_0 * rtb_B_299_3_0 + rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP ->
P_96 ; break ; case 2 : rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_2314_0 [ 0
] - _rtDW -> UnitDelay1_DSTATE_dqh [ 0 ] ) * _rtP -> P_1966 + _rtDW ->
UnitDelay1_DSTATE_dqh [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_b5 [ 0 ] ) + (
_rtB -> B_311_2314_0 [ 0 ] - ( ( _rtB -> B_311_2321_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_b5 [ 0 ] ) * _rtP -> P_1969 + _rtDW -> UnitDelay2_DSTATE_b5
[ 0 ] ) ) ) * _rtP -> P_1970 * _rtP -> P_1971 + ( _rtP -> P_4180 . re *
B_311_1537_0_idx_0 - _rtP -> P_4180 . im * B_311_1537_0_idx_1 ) ) + ( _rtP ->
P_4179 . re * B_311_1940_0_idx_0 - _rtP -> P_4179 . im * B_311_1940_0_idx_1 )
) * _rtP -> P_92 ; rtb_B_311_52_0 = ( ( ( ( ( ( _rtB -> B_311_2314_0 [ 1 ] -
_rtDW -> UnitDelay1_DSTATE_dqh [ 1 ] ) * _rtP -> P_1966 + _rtDW ->
UnitDelay1_DSTATE_dqh [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_b5 [ 1 ] ) + (
_rtB -> B_311_2314_0 [ 1 ] - ( ( _rtB -> B_311_2321_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_b5 [ 1 ] ) * _rtP -> P_1969 + _rtDW -> UnitDelay2_DSTATE_b5
[ 1 ] ) ) ) * _rtP -> P_1970 * _rtP -> P_1971 + ( _rtP -> P_4180 . re *
B_311_1537_0_idx_1 + _rtP -> P_4180 . im * B_311_1537_0_idx_0 ) ) + ( _rtP ->
P_4179 . re * B_311_1940_0_idx_1 + _rtP -> P_4179 . im * B_311_1940_0_idx_0 )
) * _rtP -> P_92 ; rtb_B_311_68_0 = ( ( ( ( ( ( _rtB -> B_311_2365_0 [ 0 ] -
_rtDW -> UnitDelay1_DSTATE_ew [ 0 ] ) * _rtP -> P_1993 + _rtDW ->
UnitDelay1_DSTATE_ew [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_lw [ 0 ] ) + ( _rtB
-> B_311_2365_0 [ 0 ] - ( ( _rtB -> B_311_2372_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_lw [ 0 ] ) * _rtP -> P_1996 + _rtDW -> UnitDelay2_DSTATE_lw
[ 0 ] ) ) ) * _rtP -> P_1997 * _rtP -> P_1998 + ( _rtP -> P_4178 . re *
B_311_1586_0_idx_0 - _rtP -> P_4178 . im * B_311_1586_0_idx_1 ) ) + ( _rtP ->
P_4177 . re * B_311_1556_0_idx_0 - _rtP -> P_4177 . im * B_311_1556_0_idx_1 )
) * _rtP -> P_91 ; rtb_B_299_3_0 = - ( ( ( ( ( ( ( _rtB -> B_311_2365_0 [ 1 ]
- _rtDW -> UnitDelay1_DSTATE_ew [ 1 ] ) * _rtP -> P_1993 + _rtDW ->
UnitDelay1_DSTATE_ew [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_lw [ 1 ] ) + ( _rtB
-> B_311_2365_0 [ 1 ] - ( ( _rtB -> B_311_2372_0 [ 1 ] - _rtDW ->
UnitDelay2_DSTATE_lw [ 1 ] ) * _rtP -> P_1996 + _rtDW -> UnitDelay2_DSTATE_lw
[ 1 ] ) ) ) * _rtP -> P_1997 * _rtP -> P_1998 + ( _rtP -> P_4178 . re *
B_311_1586_0_idx_1 + _rtP -> P_4178 . im * B_311_1586_0_idx_0 ) ) + ( _rtP ->
P_4177 . re * B_311_1556_0_idx_1 + _rtP -> P_4177 . im * B_311_1556_0_idx_0 )
) * _rtP -> P_91 ) ; _rtB -> B_311_2419_0 = ( rtb_B_311_37_0 * rtb_B_311_68_0
- rtb_B_311_52_0 * rtb_B_299_3_0 ) * _rtP -> P_93 ; _rtB -> B_311_2419_1 = (
rtb_B_311_37_0 * rtb_B_299_3_0 + rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP ->
P_93 ; break ; default : rtb_B_311_37_0 = ( ( ( ( ( ( _rtB -> B_311_2314_0 [
0 ] - _rtDW -> UnitDelay1_DSTATE_dqh [ 0 ] ) * _rtP -> P_1966 + _rtDW ->
UnitDelay1_DSTATE_dqh [ 0 ] ) - _rtDW -> UnitDelay2_DSTATE_b5 [ 0 ] ) + (
_rtB -> B_311_2314_0 [ 0 ] - ( ( _rtB -> B_311_2321_0 [ 0 ] - _rtDW ->
UnitDelay2_DSTATE_b5 [ 0 ] ) * _rtP -> P_1969 + _rtDW -> UnitDelay2_DSTATE_b5
[ 0 ] ) ) ) * _rtP -> P_1970 * _rtP -> P_1971 + B_311_1537_0_idx_0 ) +
B_311_1940_0_idx_0 ) * _rtP -> P_89 ; rtb_B_311_52_0 = ( ( ( ( ( ( _rtB ->
B_311_2314_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_dqh [ 1 ] ) * _rtP -> P_1966
+ _rtDW -> UnitDelay1_DSTATE_dqh [ 1 ] ) - _rtDW -> UnitDelay2_DSTATE_b5 [ 1
] ) + ( _rtB -> B_311_2314_0 [ 1 ] - ( ( _rtB -> B_311_2321_0 [ 1 ] - _rtDW
-> UnitDelay2_DSTATE_b5 [ 1 ] ) * _rtP -> P_1969 + _rtDW ->
UnitDelay2_DSTATE_b5 [ 1 ] ) ) ) * _rtP -> P_1970 * _rtP -> P_1971 +
B_311_1537_0_idx_1 ) + B_311_1940_0_idx_1 ) * _rtP -> P_89 ; rtb_B_311_68_0 =
( ( ( ( ( ( _rtB -> B_311_2365_0 [ 0 ] - _rtDW -> UnitDelay1_DSTATE_ew [ 0 ]
) * _rtP -> P_1993 + _rtDW -> UnitDelay1_DSTATE_ew [ 0 ] ) - _rtDW ->
UnitDelay2_DSTATE_lw [ 0 ] ) + ( _rtB -> B_311_2365_0 [ 0 ] - ( ( _rtB ->
B_311_2372_0 [ 0 ] - _rtDW -> UnitDelay2_DSTATE_lw [ 0 ] ) * _rtP -> P_1996 +
_rtDW -> UnitDelay2_DSTATE_lw [ 0 ] ) ) ) * _rtP -> P_1997 * _rtP -> P_1998 +
B_311_1586_0_idx_0 ) + B_311_1556_0_idx_0 ) * _rtP -> P_88 ; rtb_B_299_3_0 =
- ( ( ( ( ( ( ( _rtB -> B_311_2365_0 [ 1 ] - _rtDW -> UnitDelay1_DSTATE_ew [
1 ] ) * _rtP -> P_1993 + _rtDW -> UnitDelay1_DSTATE_ew [ 1 ] ) - _rtDW ->
UnitDelay2_DSTATE_lw [ 1 ] ) + ( _rtB -> B_311_2365_0 [ 1 ] - ( ( _rtB ->
B_311_2372_0 [ 1 ] - _rtDW -> UnitDelay2_DSTATE_lw [ 1 ] ) * _rtP -> P_1996 +
_rtDW -> UnitDelay2_DSTATE_lw [ 1 ] ) ) ) * _rtP -> P_1997 * _rtP -> P_1998 +
B_311_1586_0_idx_1 ) + B_311_1556_0_idx_1 ) * _rtP -> P_88 ) ; _rtB ->
B_311_2419_0 = ( rtb_B_311_37_0 * rtb_B_311_68_0 - rtb_B_311_52_0 *
rtb_B_299_3_0 ) * _rtP -> P_90 ; _rtB -> B_311_2419_1 = ( rtb_B_311_37_0 *
rtb_B_299_3_0 + rtb_B_311_52_0 * rtb_B_311_68_0 ) * _rtP -> P_90 ; break ; }
ssCallAccelRunBlock ( S , 311 , 2420 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_311_2507_0 = _rtP -> P_2017 * _rtB -> B_311_152_0 [ 121 ] ; _rtB ->
B_311_2508_0 = _rtP -> P_2018 * _rtB -> B_311_152_0 [ 206 ] ; } i =
ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtB -> B_41_0_0 [ 0 ] = _rtB
-> B_311_155_0 ; _rtB -> B_41_0_0 [ 1 ] = _rtB -> B_311_156_0 ; _rtB ->
B_41_0_0 [ 2 ] = _rtB -> B_311_157_0_e ; _rtB -> B_41_0_0 [ 3 ] = _rtB ->
B_311_158_0 ; ssCallAccelRunBlock ( S , 41 , 1 , SS_CALL_MDL_OUTPUTS ) ; } i
= ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_2523_0 = _rtB
-> B_311_2508_0 * _rtB -> B_311_170_0 + _rtB -> B_311_2507_0 ; _rtB ->
B_311_2530_0 = ( muDoubleScalarExp ( 1.0 / _rtB -> B_311_189_0_a * _rtB ->
B_311_2523_0 ) - _rtB -> B_311_190_0_j ) * _rtB -> B_311_186_0 ; } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_2532_0 = _rtB ->
B_311_2523_0 / _rtB -> B_311_188_0_f ; } _rtB -> B_311_2533_0 = ( _rtB ->
B_311_2523_0 / _rtB -> B_311_187_0 * rtb_B_311_62_0 + _rtB -> B_311_2530_0 )
- _rtB -> B_311_2532_0 ; i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { if
( _rtB -> B_311_159_0_b != 0.0 ) { _rtB -> B_311_2543_0 = _rtB -> B_311_154_0
; } else { _rtB -> B_311_2543_0 = _rtB -> B_41_1_1 ; } } i = ssIsSampleHit (
S , 2 , 0 ) ; if ( i != 0 ) { ssCallAccelRunBlock ( S , 311 , 2554 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_311_2555_0 = _rtB -> B_311_1356_0 - _rtB ->
B_311_1355_0 ; _rtB -> B_311_2556_0 = _rtB -> B_311_1352_0 [ 0 ] - _rtB ->
B_311_1338_0 [ 0 ] ; ssCallAccelRunBlock ( S , 311 , 2557 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 311 , 2558 ,
SS_CALL_MDL_OUTPUTS ) ; rtb_B_311_62_0 = muDoubleScalarCos ( _rtB ->
B_311_1358_0 ) ; rtb_B_311_2027_0 = muDoubleScalarSin ( _rtB -> B_311_1358_0
) * _rtB -> B_311_193_0 - rtb_B_311_62_0 * _rtB -> B_311_192_0 ; _rtB ->
B_311_2569_0 = ( ( ( 0.0 - rtb_B_311_2027_0 ) - rtb_B_311_62_0 ) *
B_311_1283_0_idx_2 + ( B_311_1283_0_idx_0 * rtb_B_311_62_0 +
B_311_1283_0_idx_1 * rtb_B_311_2027_0 ) ) * 0.66666666666666663 ; if ( _rtDW
-> Integ4_SYSTEM_ENABLE_jr != 0 ) { _rtB -> B_311_2570_0 = _rtDW ->
Integ4_DSTATE_b ; } else { _rtB -> B_311_2570_0 = _rtP -> P_2019 * _rtB ->
B_311_2569_0 + _rtDW -> Integ4_DSTATE_b ; } _rtB -> B_311_2571_0 = _rtP ->
P_2021 ; ssCallAccelRunBlock ( S , 311 , 2572 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) < _rtP -> P_2024 ) { rtb_B_299_3_0 = _rtP -> P_2025 ;
} else { rtb_B_299_3_0 = _rtP -> P_2026 ; } if ( rtb_B_299_3_0 >= _rtP ->
P_2028 ) { B_311_1965_0_idx_0 = ( _rtB -> B_311_2570_0 - _rtB -> B_311_2572_0
) * _rtP -> P_2022 + ( _rtP -> P_116 * _rtB -> B_311_2569_0 - _rtP -> P_115 *
_rtDW -> UnitDelay_DSTATE_f4 ) ; } else { B_311_1965_0_idx_0 = _rtP -> P_2027
; } _rtB -> B_311_2582_0 = _rtP -> P_2034 * B_311_1965_0_idx_0 * _rtP ->
P_2035 ; rtb_B_311_62_0 = ( _rtP -> P_2029 * B_311_1965_0_idx_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_pk ) + ( _rtB -> B_311_2582_0 - _rtDW ->
UD_DSTATE ) ; if ( rtb_B_311_62_0 > _rtP -> P_2037 ) { rtb_B_311_62_0 = _rtP
-> P_2037 ; } else { if ( rtb_B_311_62_0 < _rtP -> P_2038 ) { rtb_B_311_62_0
= _rtP -> P_2038 ; } } rtb_B_311_35_0 = _rtP -> P_2039 * rtb_B_311_62_0 -
_rtDW -> UnitDelay_DSTATE_l ; if ( rtb_B_311_35_0 > _rtP -> P_2041 ) {
rtb_B_311_35_0 = _rtP -> P_2041 ; } else { if ( rtb_B_311_35_0 < _rtP ->
P_2042 ) { rtb_B_311_35_0 = _rtP -> P_2042 ; } } _rtB -> B_311_2591_0 =
rtb_B_311_35_0 + _rtDW -> UnitDelay_DSTATE_l ; { _rtB -> B_311_2592_0 = (
_rtP -> P_2045 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_fa [ 0 ] + ( _rtP
-> P_2045 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_fa [ 1 ] ; _rtB ->
B_311_2592_0 += _rtP -> P_2046 * _rtB -> B_311_2591_0 ; } _rtB ->
B_311_2593_0 = _rtP -> P_2048 * B_311_1965_0_idx_0 ; _rtB -> B_311_2597_0 = (
_rtP -> P_2049 * rtb_B_311_62_0 - ( real_T ) _rtP -> P_4275 * 0.03125 * (
real_T ) ( _rtB -> B_311_1358_0 >= _rtB -> B_311_191_0_j ) ) + _rtB ->
B_311_1358_0 ; rtb_B_311_62_0 = muDoubleScalarCos ( _rtB -> B_311_1359_0 ) ;
rtb_B_311_2027_0 = muDoubleScalarSin ( _rtB -> B_311_1359_0 ) * _rtB ->
B_311_196_0 - rtb_B_311_62_0 * _rtB -> B_311_195_0 ; _rtB -> B_311_2608_0 = (
( ( 0.0 - rtb_B_311_2027_0 ) - rtb_B_311_62_0 ) * rtb_B_311_1335_0 [ 2 ] + (
rtb_B_311_1335_0 [ 0 ] * rtb_B_311_62_0 + rtb_B_311_1335_0 [ 1 ] *
rtb_B_311_2027_0 ) ) * 0.66666666666666663 ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_ev != 0 ) { _rtB -> B_311_2609_0 = _rtDW ->
Integ4_DSTATE_at ; } else { _rtB -> B_311_2609_0 = _rtP -> P_2050 * _rtB ->
B_311_2608_0 + _rtDW -> Integ4_DSTATE_at ; } _rtB -> B_311_2610_0 = _rtP ->
P_2052 ; ssCallAccelRunBlock ( S , 311 , 2611 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) < _rtP -> P_2055 ) { rtb_B_299_3_0 = _rtP -> P_2056 ;
} else { rtb_B_299_3_0 = _rtP -> P_2057 ; } if ( rtb_B_299_3_0 >= _rtP ->
P_2059 ) { B_311_1965_0_idx_0 = ( _rtB -> B_311_2609_0 - _rtB -> B_311_2611_0
) * _rtP -> P_2053 + ( _rtP -> P_118 * _rtB -> B_311_2608_0 - _rtP -> P_117 *
_rtDW -> UnitDelay_DSTATE_ji ) ; } else { B_311_1965_0_idx_0 = _rtP -> P_2058
; } _rtB -> B_311_2621_0 = _rtP -> P_2065 * B_311_1965_0_idx_0 * _rtP ->
P_2066 ; rtb_B_311_62_0 = ( _rtP -> P_2060 * B_311_1965_0_idx_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_m3 ) + ( _rtB -> B_311_2621_0 - _rtDW ->
UD_DSTATE_i ) ; if ( rtb_B_311_62_0 > _rtP -> P_2068 ) { rtb_B_311_62_0 =
_rtP -> P_2068 ; } else { if ( rtb_B_311_62_0 < _rtP -> P_2069 ) {
rtb_B_311_62_0 = _rtP -> P_2069 ; } } rtb_B_311_35_0 = _rtP -> P_2070 *
rtb_B_311_62_0 - _rtDW -> UnitDelay_DSTATE_jq ; if ( rtb_B_311_35_0 > _rtP ->
P_2072 ) { rtb_B_311_35_0 = _rtP -> P_2072 ; } else { if ( rtb_B_311_35_0 <
_rtP -> P_2073 ) { rtb_B_311_35_0 = _rtP -> P_2073 ; } } _rtB -> B_311_2630_0
= rtb_B_311_35_0 + _rtDW -> UnitDelay_DSTATE_jq ; { _rtB -> B_311_2631_0 = (
_rtP -> P_2076 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_in [ 0 ] + ( _rtP
-> P_2076 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_in [ 1 ] ; _rtB ->
B_311_2631_0 += _rtP -> P_2077 * _rtB -> B_311_2630_0 ; } _rtB ->
B_311_2632_0 = _rtP -> P_2079 * B_311_1965_0_idx_0 ; _rtB -> B_311_2636_0 = (
_rtP -> P_2080 * rtb_B_311_62_0 - ( real_T ) _rtP -> P_4276 * 0.03125 * (
real_T ) ( _rtB -> B_311_1359_0 >= _rtB -> B_311_194_0 ) ) + _rtB ->
B_311_1359_0 ; ssCallAccelRunBlock ( S , 311 , 2637 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2638 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2639 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2640 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2641 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2642 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2643 , SS_CALL_MDL_OUTPUTS ) ; rtb_B_311_62_0
= muDoubleScalarCos ( _rtB -> B_311_1306_0 ) ; rtb_B_311_2027_0 =
muDoubleScalarSin ( _rtB -> B_311_1306_0 ) * _rtB -> B_311_208_0 -
rtb_B_311_62_0 * _rtB -> B_311_207_0 ; _rtB -> B_311_2654_0 = ( ( ( 0.0 -
rtb_B_311_2027_0 ) - rtb_B_311_62_0 ) * B_311_1062_0_idx_2 + (
B_311_1062_0_idx_0 * rtb_B_311_62_0 + B_311_1062_0_idx_1 * rtb_B_311_2027_0 )
) * 0.66666666666666663 ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_fy != 0 ) { _rtB
-> B_311_2655_0 = _rtDW -> Integ4_DSTATE_ko ; } else { _rtB -> B_311_2655_0 =
_rtP -> P_2081 * _rtB -> B_311_2654_0 + _rtDW -> Integ4_DSTATE_ko ; } _rtB ->
B_311_2656_0 = _rtP -> P_2083 ; ssCallAccelRunBlock ( S , 311 , 2657 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) < _rtP -> P_2086 ) {
rtb_B_299_3_0 = _rtP -> P_2087 ; } else { rtb_B_299_3_0 = _rtP -> P_2088 ; }
if ( rtb_B_299_3_0 >= _rtP -> P_2090 ) { B_311_1965_0_idx_0 = ( _rtB ->
B_311_2655_0 - _rtB -> B_311_2657_0 ) * _rtP -> P_2084 + ( _rtP -> P_124 *
_rtB -> B_311_2654_0 - _rtP -> P_123 * _rtDW -> UnitDelay_DSTATE_gg ) ; }
else { B_311_1965_0_idx_0 = _rtP -> P_2089 ; } _rtB -> B_311_2667_0 = _rtP ->
P_2096 * B_311_1965_0_idx_0 * _rtP -> P_2097 ; rtb_B_311_62_0 = ( _rtP ->
P_2091 * B_311_1965_0_idx_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_d ) + (
_rtB -> B_311_2667_0 - _rtDW -> UD_DSTATE_m ) ; if ( rtb_B_311_62_0 > _rtP ->
P_2099 ) { rtb_B_311_62_0 = _rtP -> P_2099 ; } else { if ( rtb_B_311_62_0 <
_rtP -> P_2100 ) { rtb_B_311_62_0 = _rtP -> P_2100 ; } } rtb_B_311_35_0 =
_rtP -> P_2101 * rtb_B_311_62_0 - _rtDW -> UnitDelay_DSTATE_ob ; if (
rtb_B_311_35_0 > _rtP -> P_2103 ) { rtb_B_311_35_0 = _rtP -> P_2103 ; } else
{ if ( rtb_B_311_35_0 < _rtP -> P_2104 ) { rtb_B_311_35_0 = _rtP -> P_2104 ;
} } _rtB -> B_311_2676_0 = rtb_B_311_35_0 + _rtDW -> UnitDelay_DSTATE_ob ; {
_rtB -> B_311_2677_0 = ( _rtP -> P_2107 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_gw [ 0 ] + ( _rtP -> P_2107 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_gw [ 1 ] ; _rtB -> B_311_2677_0 += _rtP -> P_2108 *
_rtB -> B_311_2676_0 ; } _rtB -> B_311_2678_0 = _rtP -> P_2110 *
B_311_1965_0_idx_0 ; _rtB -> B_311_2682_0 = ( _rtP -> P_2111 * rtb_B_311_62_0
- ( real_T ) _rtP -> P_4277 * 0.03125 * ( real_T ) ( _rtB -> B_311_1306_0 >=
_rtB -> B_311_206_0 ) ) + _rtB -> B_311_1306_0 ; rtb_B_311_62_0 =
muDoubleScalarCos ( _rtB -> B_311_1307_0 ) ; rtb_B_311_2027_0 =
muDoubleScalarSin ( _rtB -> B_311_1307_0 ) * _rtB -> B_311_211_0 -
rtb_B_311_62_0 * _rtB -> B_311_210_0_o ; _rtB -> B_311_2693_0 = ( ( ( 0.0 -
rtb_B_311_2027_0 ) - rtb_B_311_62_0 ) * B_299_0_0_idx_2 + ( B_299_0_0_idx_0 *
rtb_B_311_62_0 + B_299_0_0_idx_1 * rtb_B_311_2027_0 ) ) * 0.66666666666666663
; if ( _rtDW -> Integ4_SYSTEM_ENABLE_n != 0 ) { _rtB -> B_311_2694_0 = _rtDW
-> Integ4_DSTATE_cj ; } else { _rtB -> B_311_2694_0 = _rtP -> P_2112 * _rtB
-> B_311_2693_0 + _rtDW -> Integ4_DSTATE_cj ; } _rtB -> B_311_2695_0 = _rtP
-> P_2114 ; ssCallAccelRunBlock ( S , 311 , 2696 , SS_CALL_MDL_OUTPUTS ) ; if
( ssGetTaskTime ( S , 2 ) < _rtP -> P_2117 ) { rtb_B_299_3_0 = _rtP -> P_2118
; } else { rtb_B_299_3_0 = _rtP -> P_2119 ; } if ( rtb_B_299_3_0 >= _rtP ->
P_2121 ) { B_311_1965_0_idx_0 = ( _rtB -> B_311_2694_0 - _rtB -> B_311_2696_0
) * _rtP -> P_2115 + ( _rtP -> P_126 * _rtB -> B_311_2693_0 - _rtP -> P_125 *
_rtDW -> UnitDelay_DSTATE_aq ) ; } else { B_311_1965_0_idx_0 = _rtP -> P_2120
; } _rtB -> B_311_2706_0 = _rtP -> P_2127 * B_311_1965_0_idx_0 * _rtP ->
P_2128 ; rtb_B_311_62_0 = ( _rtP -> P_2122 * B_311_1965_0_idx_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_f ) + ( _rtB -> B_311_2706_0 - _rtDW ->
UD_DSTATE_p ) ; if ( rtb_B_311_62_0 > _rtP -> P_2130 ) { rtb_B_311_62_0 =
_rtP -> P_2130 ; } else { if ( rtb_B_311_62_0 < _rtP -> P_2131 ) {
rtb_B_311_62_0 = _rtP -> P_2131 ; } } rtb_B_311_35_0 = _rtP -> P_2132 *
rtb_B_311_62_0 - _rtDW -> UnitDelay_DSTATE_ar ; if ( rtb_B_311_35_0 > _rtP ->
P_2134 ) { rtb_B_311_35_0 = _rtP -> P_2134 ; } else { if ( rtb_B_311_35_0 <
_rtP -> P_2135 ) { rtb_B_311_35_0 = _rtP -> P_2135 ; } } _rtB -> B_311_2715_0
= rtb_B_311_35_0 + _rtDW -> UnitDelay_DSTATE_ar ; { _rtB -> B_311_2716_0 = (
_rtP -> P_2138 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_ie [ 0 ] + ( _rtP
-> P_2138 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_ie [ 1 ] ; _rtB ->
B_311_2716_0 += _rtP -> P_2139 * _rtB -> B_311_2715_0 ; } _rtB ->
B_311_2717_0 = _rtP -> P_2141 * B_311_1965_0_idx_0 ; _rtB -> B_311_2721_0 = (
_rtP -> P_2142 * rtb_B_311_62_0 - ( real_T ) _rtP -> P_4278 * 0.03125 * (
real_T ) ( _rtB -> B_311_1307_0 >= _rtB -> B_311_209_0 ) ) + _rtB ->
B_311_1307_0 ; ssCallAccelRunBlock ( S , 311 , 2722 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2723 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2724 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2725 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 2726 , SS_CALL_MDL_OUTPUTS ) ; } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { rtb_B_311_50_0 =
muDoubleScalarMax ( _rtB -> B_311_250_0 [ 0 ] , rtb_B_311_251_0 ) ; _rtB ->
B_311_2728_0 = ( real_T ) ( rtb_B_311_50_0 == 0.0 ) * 2.2204460492503131e-16
+ rtb_B_311_50_0 ; } _rtB -> B_311_2730_0 = 1.0 / _rtB -> B_311_2728_0 * (
_rtB -> B_311_255_0 - _rtB -> B_311_257_0 ) ; i = ssIsSampleHit ( S , 2 , 0 )
; if ( i != 0 ) { _rtB -> B_311_2731_0 = _rtP -> P_2143 * _rtB ->
B_311_2556_0 ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if (
ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> RelationalOperator_Mode = ( _rtX ->
Integrator_CSTATE_d > _rtB -> B_311_223_0 ) ; } _rtB -> B_311_2732_0 = _rtDW
-> RelationalOperator_Mode ; } _rtB -> B_311_2733_0 = _rtX ->
Integrator_CSTATE_d - _rtB -> B_311_223_0 ; if ( ( _rtDW ->
Initial_FirstOutputTime == ( rtMinusInf ) ) || ( _rtDW ->
Initial_FirstOutputTime == ssGetTaskTime ( S , 0 ) ) ) { _rtDW ->
Initial_FirstOutputTime = ssGetTaskTime ( S , 0 ) ; _rtB -> B_311_2734_0 =
_rtP -> P_2144 ; } else { _rtB -> B_311_2734_0 = _rtB -> B_311_2733_0 ; } if
( ssIsMajorTimeStep ( S ) != 0 ) { rtb_B_311_3366_0 = ( _rtB -> B_311_2732_0
&& ( _rtZCE -> Integrator_Reset_ZCE != 1 ) ) ; _rtZCE -> Integrator_Reset_ZCE
= _rtB -> B_311_2732_0 ; if ( rtb_B_311_3366_0 || ( _rtDW -> Integrator_IWORK
!= 0 ) ) { _rtX -> Integrator_CSTATE_d = _rtB -> B_311_2734_0 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } if ( _rtX ->
Integrator_CSTATE_d >= _rtP -> P_2145 ) { if ( _rtX -> Integrator_CSTATE_d >
_rtP -> P_2145 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtX
-> Integrator_CSTATE_d = _rtP -> P_2145 ; } else { if ( _rtX ->
Integrator_CSTATE_d <= _rtP -> P_2146 ) { if ( _rtX -> Integrator_CSTATE_d <
_rtP -> P_2146 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtX
-> Integrator_CSTATE_d = _rtP -> P_2146 ; } } } _rtB -> B_311_2735_0 = _rtX
-> Integrator_CSTATE_d ;
Auto_microgrid_forecasting_020822_AutomaticGainControl ( S , _rtB ->
B_311_222_0 , _rtB -> B_311_254_0 , _rtB -> B_311_2735_0 , _rtB ->
B_311_261_0 , & _rtB -> AutomaticGainControl_c , & _rtDW ->
AutomaticGainControl_c , & _rtP -> AutomaticGainControl_c , & _rtX ->
AutomaticGainControl_c , & ( ( XDis_Auto_microgrid_forecasting_020822_T * )
ssGetContStateDisabled ( S ) ) -> AutomaticGainControl_c ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtX -> Integrator_CSTATE_c >= _rtP ->
P_2149 ) { if ( _rtX -> Integrator_CSTATE_c > _rtP -> P_2149 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtX ->
Integrator_CSTATE_c = _rtP -> P_2149 ; } else { if ( _rtX ->
Integrator_CSTATE_c <= _rtP -> P_2150 ) { if ( _rtX -> Integrator_CSTATE_c <
_rtP -> P_2150 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtX
-> Integrator_CSTATE_c = _rtP -> P_2150 ; } } } _rtB -> B_311_2738_0 = _rtX
-> Integrator_CSTATE_c ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> VariableTransportDelay_PWORK . TUbufferPtrs [ 1 ] ;
real_T * * xBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK .
TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT ( S ) ; real_T appliedDelay ;
_rtB -> B_311_2739_0 =
Auto_microgrid_forecasting_020822_acc_rt_VTDelayfindtDInterpolate ( ( (
X_Auto_microgrid_forecasting_020822_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE , * tBuffer , * uBuffer , * xBuffer , _rtDW ->
VariableTransportDelay_IWORK . CircularBufSize , _rtDW ->
VariableTransportDelay_IWORK . Head , _rtDW -> VariableTransportDelay_IWORK .
Tail , & _rtDW -> VariableTransportDelay_IWORK . Last , simTime , 0.0 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) ==
ssGetT ( S ) ) ) , _rtP -> P_2152 , & appliedDelay ) ; } _rtB -> B_311_2740_0
= _rtX -> integrator_CSTATE ; _rtB -> B_311_2741_0 = ssGetT ( S ) ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( ssIsMajorTimeStep ( S ) !=
0 ) { _rtDW -> RelationalOperator_Mode_p = ( _rtB -> B_311_2741_0 >= _rtB ->
B_311_224_0 ) ; } _rtB -> B_311_2742_0 = _rtDW -> RelationalOperator_Mode_p ;
_rtB -> B_311_2743_0 = _rtDW -> Memory_PreviousInput_n ; } if ( _rtB ->
B_311_2742_0 ) { _rtB -> B_76_0_0 = _rtB -> B_311_2740_0 - _rtB ->
B_311_2739_0 ; _rtB -> B_76_1_0 = _rtB -> B_76_0_0 * _rtB -> B_311_254_0 ;
_rtB -> B_311_2745_0 = _rtB -> B_76_1_0 ; } else { _rtB -> B_311_2745_0 =
_rtB -> B_311_2743_0 ; } _rtB -> B_311_2746_0 = _rtB -> B_311_2745_0 * _rtB
-> AutomaticGainControl_c . B_67_17_0 ; _rtB -> B_311_2747_0 = _rtP -> P_2155
* _rtB -> B_311_2746_0 ; _rtB -> B_311_2748_0 = _rtP -> P_2156 * _rtB ->
B_311_2746_0 ; _rtB -> B_311_2749_0 = _rtP -> P_2157 * _rtB -> B_311_2746_0 ;
_rtB -> B_311_2750_0 = 0.0 ; _rtB -> B_311_2750_0 += _rtP -> P_2159 * _rtX ->
TransferFcn_CSTATE ; _rtB -> B_311_2750_0 += _rtP -> P_2160 * _rtB ->
B_311_2749_0 ; _rtB -> B_311_2751_0 = ( _rtB -> B_311_2747_0 + _rtB ->
B_311_2738_0 ) + _rtB -> B_311_2750_0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
_rtDW -> Saturation2_MODE = _rtB -> B_311_2751_0 >= _rtP -> P_2161 ? 1 : _rtB
-> B_311_2751_0 > _rtP -> P_2162 ? 0 : - 1 ; } _rtB -> B_311_2752_0 = _rtDW
-> Saturation2_MODE == 1 ? _rtP -> P_2161 : _rtDW -> Saturation2_MODE == - 1
? _rtP -> P_2162 : _rtB -> B_311_2751_0 ; _rtB -> B_311_2753_0 = _rtP ->
P_2163 * _rtB -> B_311_2752_0 ; if ( ( _rtDW -> LastMajorTimeA >=
ssGetTaskTime ( S , 0 ) ) && ( _rtDW -> LastMajorTimeB >= ssGetTaskTime ( S ,
0 ) ) ) { _rtB -> B_311_2754_0 = _rtB -> B_311_2753_0 ; } else { if ( ( (
_rtDW -> LastMajorTimeA < _rtDW -> LastMajorTimeB ) && ( _rtDW ->
LastMajorTimeB < ssGetTaskTime ( S , 0 ) ) ) || ( ( _rtDW -> LastMajorTimeA
>= _rtDW -> LastMajorTimeB ) && ( _rtDW -> LastMajorTimeA >= ssGetTaskTime (
S , 0 ) ) ) ) { rtb_B_311_62_0 = ssGetTaskTime ( S , 0 ) - _rtDW ->
LastMajorTimeB ; B_311_1965_0_idx_0 = _rtDW -> PrevYB ; } else {
rtb_B_311_62_0 = ssGetTaskTime ( S , 0 ) - _rtDW -> LastMajorTimeA ;
B_311_1965_0_idx_0 = _rtDW -> PrevYA ; } rtb_B_311_2022_0 = rtb_B_311_62_0 *
_rtP -> P_2164 ; rtb_B_311_251_0 = _rtB -> B_311_2753_0 - B_311_1965_0_idx_0
; if ( rtb_B_311_251_0 > rtb_B_311_2022_0 ) { _rtB -> B_311_2754_0 =
B_311_1965_0_idx_0 + rtb_B_311_2022_0 ; } else { rtb_B_311_62_0 *= _rtP ->
P_2165 ; if ( rtb_B_311_251_0 < rtb_B_311_62_0 ) { _rtB -> B_311_2754_0 =
B_311_1965_0_idx_0 + rtb_B_311_62_0 ; } else { _rtB -> B_311_2754_0 = _rtB ->
B_311_2753_0 ; } } } _rtB -> B_311_2755_0 = _rtX -> Integrator_x1_CSTATE ;
_rtB -> B_311_2756_0 = _rtP -> P_2167 * _rtB -> B_311_2755_0 ; _rtB ->
B_311_2757_0 = _rtX -> Integrator_x2_CSTATE ; _rtB -> B_311_2758_0 = _rtP ->
P_2169 * _rtB -> B_311_2757_0 ; _rtB -> B_311_2759_0 = _rtP -> P_2170 * _rtB
-> B_311_2755_0 ; _rtB -> B_311_2760_0 = _rtP -> P_2171 * _rtB ->
B_311_2757_0 ; _rtB -> B_311_2761_0 = _rtB -> B_311_2756_0 + _rtB ->
B_311_2758_0 ; _rtB -> B_311_2762_0 = _rtB -> B_311_2759_0 + _rtB ->
B_311_2760_0 ; _rtB -> B_311_2763_0 = _rtP -> P_2172 * _rtB -> B_311_2754_0 ;
_rtB -> B_311_2764_0 = _rtB -> B_311_2761_0 + _rtB -> B_311_2763_0 ; _rtB ->
B_311_2765_0 = _rtP -> P_2173 * _rtB -> B_311_2754_0 ; _rtB -> B_311_2766_0 =
_rtB -> B_311_2762_0 + _rtB -> B_311_2765_0 ; _rtB -> B_311_2771_0 = ( _rtP
-> P_2174 * _rtB -> B_311_2755_0 + _rtP -> P_2175 * _rtB -> B_311_2757_0 ) +
_rtP -> P_2176 * _rtB -> B_311_2754_0 ; if ( _rtB -> B_311_254_0 > _rtP ->
P_2177 ) { _rtB -> B_311_2772_0 = _rtP -> P_2177 ; } else if ( _rtB ->
B_311_254_0 < _rtP -> P_2178 ) { _rtB -> B_311_2772_0 = _rtP -> P_2178 ; }
else { _rtB -> B_311_2772_0 = _rtB -> B_311_254_0 ; } _rtB -> B_311_2773_0 =
1.0 / _rtB -> B_311_2772_0 ; for ( i = 0 ; i < 3 ; i ++ ) { _rtB ->
B_311_2774_0 [ i ] = 0.0 ; _rtB -> B_311_2774_0 [ i ] += _rtP -> P_2179 [ i ]
* _rtB -> B_311_261_0 [ 0 ] ; _rtB -> B_311_2774_0 [ i ] += _rtP -> P_2179 [
i + 3 ] * _rtB -> B_311_261_0 [ 1 ] ; _rtB -> B_311_2774_0 [ i ] += _rtP ->
P_2179 [ i + 6 ] * _rtB -> B_311_261_0 [ 2 ] ; _rtB -> B_311_2775_0 [ i ] =
_rtP -> P_2180 * _rtB -> B_311_2774_0 [ i ] ; }
Auto_microgrid_forecasting_020822_Subsystempi2delay ( S , _rtB ->
B_311_229_0_i , & _rtB -> B_311_2775_0 [ 0 ] , _rtB -> B_311_2735_0 , & _rtB
-> Subsystempi2delay_n , & _rtDW -> Subsystempi2delay_n ) ;
Auto_microgrid_forecasting_020822_Subsystem1 ( S , _rtB -> B_311_227_0_f , &
_rtB -> B_311_2775_0 [ 0 ] , _rtB -> B_311_2735_0 , & _rtB -> Subsystem1_m ,
& _rtDW -> Subsystem1_m ) ; if ( _rtB -> B_311_227_0_f != 0 ) { _rtB ->
B_311_2778_0 [ 0 ] = _rtB -> Subsystem1_m . B_66_0_0 ; _rtB -> B_311_2778_0 [
1 ] = _rtB -> Subsystem1_m . B_66_1_0 ; } else { _rtB -> B_311_2778_0 [ 0 ] =
_rtB -> Subsystempi2delay_n . B_65_0_0 ; _rtB -> B_311_2778_0 [ 1 ] = _rtB ->
Subsystempi2delay_n . B_65_1_0 ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i
!= 0 ) { if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW ->
RelationalOperator_Mode_g = ( _rtX -> Integrator_CSTATE_mn > _rtB ->
B_311_232_0_m ) ; } _rtB -> B_311_2779_0 = _rtDW -> RelationalOperator_Mode_g
; } _rtB -> B_311_2780_0 = _rtX -> Integrator_CSTATE_mn - _rtB ->
B_311_232_0_m ; if ( ( _rtDW -> Initial_FirstOutputTime_c == ( rtMinusInf ) )
|| ( _rtDW -> Initial_FirstOutputTime_c == ssGetTaskTime ( S , 0 ) ) ) {
_rtDW -> Initial_FirstOutputTime_c = ssGetTaskTime ( S , 0 ) ; _rtB ->
B_311_2781_0 = _rtP -> P_2181 ; } else { _rtB -> B_311_2781_0 = _rtB ->
B_311_2780_0 ; } if ( ssIsMajorTimeStep ( S ) != 0 ) { rtb_B_311_3366_0 = (
_rtB -> B_311_2779_0 && ( _rtZCE -> Integrator_Reset_ZCE_j != 1 ) ) ; _rtZCE
-> Integrator_Reset_ZCE_j = _rtB -> B_311_2779_0 ; if ( rtb_B_311_3366_0 || (
_rtDW -> Integrator_IWORK_o != 0 ) ) { _rtX -> Integrator_CSTATE_mn = _rtB ->
B_311_2781_0 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } if ( _rtX ->
Integrator_CSTATE_mn >= _rtP -> P_2182 ) { if ( _rtX -> Integrator_CSTATE_mn
> _rtP -> P_2182 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
_rtX -> Integrator_CSTATE_mn = _rtP -> P_2182 ; } else { if ( _rtX ->
Integrator_CSTATE_mn <= _rtP -> P_2183 ) { if ( _rtX -> Integrator_CSTATE_mn
< _rtP -> P_2183 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
_rtX -> Integrator_CSTATE_mn = _rtP -> P_2183 ; } } } _rtB -> B_311_2782_0 =
_rtX -> Integrator_CSTATE_mn ;
Auto_microgrid_forecasting_020822_AutomaticGainControl ( S , _rtB ->
B_311_231_0_m , _rtB -> B_311_153_0 , _rtB -> B_311_2782_0 , _rtB ->
B_311_157_0 , & _rtB -> AutomaticGainControl_e , & _rtDW ->
AutomaticGainControl_e , & _rtP -> AutomaticGainControl_e , & _rtX ->
AutomaticGainControl_e , & ( ( XDis_Auto_microgrid_forecasting_020822_T * )
ssGetContStateDisabled ( S ) ) -> AutomaticGainControl_e ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtX -> Integrator_CSTATE_dx >= _rtP ->
P_2186 ) { if ( _rtX -> Integrator_CSTATE_dx > _rtP -> P_2186 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtX ->
Integrator_CSTATE_dx = _rtP -> P_2186 ; } else { if ( _rtX ->
Integrator_CSTATE_dx <= _rtP -> P_2187 ) { if ( _rtX -> Integrator_CSTATE_dx
< _rtP -> P_2187 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
_rtX -> Integrator_CSTATE_dx = _rtP -> P_2187 ; } } } _rtB -> B_311_2785_0 =
_rtX -> Integrator_CSTATE_dx ; { real_T * * uBuffer = ( real_T * * ) & _rtDW
-> VariableTransportDelay_PWORK_m . TUbufferPtrs [ 0 ] ; real_T * * tBuffer =
( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_m . TUbufferPtrs [ 1 ]
; real_T * * xBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK_m . TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT
( S ) ; real_T appliedDelay ; _rtB -> B_311_2786_0 =
Auto_microgrid_forecasting_020822_acc_rt_VTDelayfindtDInterpolate ( ( (
X_Auto_microgrid_forecasting_020822_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE_h , * tBuffer , * uBuffer , * xBuffer , _rtDW
-> VariableTransportDelay_IWORK_e . CircularBufSize , _rtDW ->
VariableTransportDelay_IWORK_e . Head , _rtDW ->
VariableTransportDelay_IWORK_e . Tail , & _rtDW ->
VariableTransportDelay_IWORK_e . Last , simTime , 0.0 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
, _rtP -> P_2189 , & appliedDelay ) ; } _rtB -> B_311_2787_0 = _rtX ->
integrator_CSTATE_j ; _rtB -> B_311_2788_0 = ssGetT ( S ) ; i = ssIsSampleHit
( S , 1 , 0 ) ; if ( i != 0 ) { if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW
-> RelationalOperator_Mode_j = ( _rtB -> B_311_2788_0 >= _rtB -> B_311_233_0
) ; } _rtB -> B_311_2789_0 = _rtDW -> RelationalOperator_Mode_j ; _rtB ->
B_311_2790_0 = _rtDW -> Memory_PreviousInput_m ; } if ( _rtB -> B_311_2789_0
) { _rtB -> B_84_0_0 = _rtB -> B_311_2787_0 - _rtB -> B_311_2786_0 ; _rtB ->
B_84_1_0 = _rtB -> B_84_0_0 * _rtB -> B_311_153_0 ; _rtB -> B_311_2792_0 =
_rtB -> B_84_1_0 ; } else { _rtB -> B_311_2792_0 = _rtB -> B_311_2790_0 ; }
_rtB -> B_311_2793_0 = _rtB -> B_311_2792_0 * _rtB -> AutomaticGainControl_e
. B_67_17_0 ; _rtB -> B_311_2794_0 = _rtP -> P_2192 * _rtB -> B_311_2793_0 ;
_rtB -> B_311_2795_0 = _rtP -> P_2193 * _rtB -> B_311_2793_0 ; _rtB ->
B_311_2796_0 = _rtP -> P_2194 * _rtB -> B_311_2793_0 ; _rtB -> B_311_2797_0 =
0.0 ; _rtB -> B_311_2797_0 += _rtP -> P_2196 * _rtX -> TransferFcn_CSTATE_l ;
_rtB -> B_311_2797_0 += _rtP -> P_2197 * _rtB -> B_311_2796_0 ; _rtB ->
B_311_2798_0 = ( _rtB -> B_311_2794_0 + _rtB -> B_311_2785_0 ) + _rtB ->
B_311_2797_0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW ->
Saturation2_MODE_n = _rtB -> B_311_2798_0 >= _rtP -> P_2198 ? 1 : _rtB ->
B_311_2798_0 > _rtP -> P_2199 ? 0 : - 1 ; } _rtB -> B_311_2799_0 = _rtDW ->
Saturation2_MODE_n == 1 ? _rtP -> P_2198 : _rtDW -> Saturation2_MODE_n == - 1
? _rtP -> P_2199 : _rtB -> B_311_2798_0 ; _rtB -> B_311_2800_0 = _rtP ->
P_2200 * _rtB -> B_311_2799_0 ; if ( ( _rtDW -> LastMajorTimeA_o >=
ssGetTaskTime ( S , 0 ) ) && ( _rtDW -> LastMajorTimeB_c >= ssGetTaskTime ( S
, 0 ) ) ) { _rtB -> B_311_2801_0 = _rtB -> B_311_2800_0 ; } else { if ( ( (
_rtDW -> LastMajorTimeA_o < _rtDW -> LastMajorTimeB_c ) && ( _rtDW ->
LastMajorTimeB_c < ssGetTaskTime ( S , 0 ) ) ) || ( ( _rtDW ->
LastMajorTimeA_o >= _rtDW -> LastMajorTimeB_c ) && ( _rtDW ->
LastMajorTimeA_o >= ssGetTaskTime ( S , 0 ) ) ) ) { rtb_B_311_62_0 =
ssGetTaskTime ( S , 0 ) - _rtDW -> LastMajorTimeB_c ; B_311_1965_0_idx_0 =
_rtDW -> PrevYB_m ; } else { rtb_B_311_62_0 = ssGetTaskTime ( S , 0 ) - _rtDW
-> LastMajorTimeA_o ; B_311_1965_0_idx_0 = _rtDW -> PrevYA_d ; }
rtb_B_311_2022_0 = rtb_B_311_62_0 * _rtP -> P_2201 ; rtb_B_311_251_0 = _rtB
-> B_311_2800_0 - B_311_1965_0_idx_0 ; if ( rtb_B_311_251_0 >
rtb_B_311_2022_0 ) { _rtB -> B_311_2801_0 = B_311_1965_0_idx_0 +
rtb_B_311_2022_0 ; } else { rtb_B_311_62_0 *= _rtP -> P_2202 ; if (
rtb_B_311_251_0 < rtb_B_311_62_0 ) { _rtB -> B_311_2801_0 =
B_311_1965_0_idx_0 + rtb_B_311_62_0 ; } else { _rtB -> B_311_2801_0 = _rtB ->
B_311_2800_0 ; } } } _rtB -> B_311_2802_0 = _rtX -> Integrator_x1_CSTATE_n ;
_rtB -> B_311_2803_0 = _rtP -> P_2204 * _rtB -> B_311_2802_0 ; _rtB ->
B_311_2804_0 = _rtX -> Integrator_x2_CSTATE_p ; _rtB -> B_311_2805_0 = _rtP
-> P_2206 * _rtB -> B_311_2804_0 ; _rtB -> B_311_2806_0 = _rtP -> P_2207 *
_rtB -> B_311_2802_0 ; _rtB -> B_311_2807_0 = _rtP -> P_2208 * _rtB ->
B_311_2804_0 ; _rtB -> B_311_2808_0 = _rtB -> B_311_2803_0 + _rtB ->
B_311_2805_0 ; _rtB -> B_311_2809_0 = _rtB -> B_311_2806_0 + _rtB ->
B_311_2807_0 ; _rtB -> B_311_2810_0 = _rtP -> P_2209 * _rtB -> B_311_2801_0 ;
_rtB -> B_311_2811_0 = _rtB -> B_311_2808_0 + _rtB -> B_311_2810_0 ; _rtB ->
B_311_2812_0 = _rtP -> P_2210 * _rtB -> B_311_2801_0 ; _rtB -> B_311_2813_0 =
_rtB -> B_311_2809_0 + _rtB -> B_311_2812_0 ; _rtB -> B_311_2818_0 = ( _rtP
-> P_2211 * _rtB -> B_311_2802_0 + _rtP -> P_2212 * _rtB -> B_311_2804_0 ) +
_rtP -> P_2213 * _rtB -> B_311_2801_0 ; if ( _rtB -> B_311_153_0 > _rtP ->
P_2214 ) { _rtB -> B_311_2819_0 = _rtP -> P_2214 ; } else if ( _rtB ->
B_311_153_0 < _rtP -> P_2215 ) { _rtB -> B_311_2819_0 = _rtP -> P_2215 ; }
else { _rtB -> B_311_2819_0 = _rtB -> B_311_153_0 ; } _rtB -> B_311_2820_0 =
1.0 / _rtB -> B_311_2819_0 ; for ( i = 0 ; i < 3 ; i ++ ) { _rtB ->
B_311_2821_0 [ i ] = 0.0 ; _rtB -> B_311_2821_0 [ i ] += _rtP -> P_2216 [ i ]
* _rtB -> B_311_157_0 [ 0 ] ; _rtB -> B_311_2821_0 [ i ] += _rtP -> P_2216 [
i + 3 ] * _rtB -> B_311_157_0 [ 1 ] ; _rtB -> B_311_2821_0 [ i ] += _rtP ->
P_2216 [ i + 6 ] * _rtB -> B_311_157_0 [ 2 ] ; _rtB -> B_311_2822_0 [ i ] =
_rtP -> P_2217 * _rtB -> B_311_2821_0 [ i ] ; }
Auto_microgrid_forecasting_020822_Subsystempi2delay ( S , _rtB -> B_311_238_0
, & _rtB -> B_311_2822_0 [ 0 ] , _rtB -> B_311_2782_0 , & _rtB ->
Subsystempi2delay_h , & _rtDW -> Subsystempi2delay_h ) ;
Auto_microgrid_forecasting_020822_Subsystem1 ( S , _rtB -> B_311_236_0 , &
_rtB -> B_311_2822_0 [ 0 ] , _rtB -> B_311_2782_0 , & _rtB -> Subsystem1_j ,
& _rtDW -> Subsystem1_j ) ; if ( _rtB -> B_311_236_0 != 0 ) { _rtB ->
B_311_2825_0 [ 0 ] = _rtB -> Subsystem1_j . B_66_0_0 ; _rtB -> B_311_2825_0 [
1 ] = _rtB -> Subsystem1_j . B_66_1_0 ; } else { _rtB -> B_311_2825_0 [ 0 ] =
_rtB -> Subsystempi2delay_h . B_65_0_0 ; _rtB -> B_311_2825_0 [ 1 ] = _rtB ->
Subsystempi2delay_h . B_65_1_0 ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i
!= 0 ) { B_311_2829_0_idx_0 = _rtP -> P_2218 * _rtB -> B_311_152_0 [ 112 ] *
_rtP -> P_2221 ; B_311_2829_0_idx_1 = _rtP -> P_2219 * _rtB -> B_311_152_0 [
113 ] * _rtP -> P_2221 ; B_311_2829_0_idx_2 = _rtP -> P_2220 * _rtB ->
B_311_152_0 [ 114 ] * _rtP -> P_2221 ; if ( _rtDW -> systemEnable_me != 0 ) {
_rtDW -> lastSin_nh = muDoubleScalarSin ( _rtP -> P_2224 * ssGetTaskTime ( S
, 2 ) ) ; _rtDW -> lastCos_e = muDoubleScalarCos ( _rtP -> P_2224 *
ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_me = 0 ; } _rtB ->
B_311_2831_0 = ( ( ( _rtDW -> lastSin_nh * _rtP -> P_2228 + _rtDW ->
lastCos_e * _rtP -> P_2227 ) * _rtP -> P_2226 + ( _rtDW -> lastCos_e * _rtP
-> P_2228 - _rtDW -> lastSin_nh * _rtP -> P_2227 ) * _rtP -> P_2225 ) * _rtP
-> P_2222 + _rtP -> P_2223 ) * B_311_2829_0_idx_0 ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_di != 0 ) { _rtB -> B_311_2832_0 = _rtDW ->
Integ4_DSTATE_ku ; } else { _rtB -> B_311_2832_0 = _rtP -> P_2229 * _rtB ->
B_311_2831_0 + _rtDW -> Integ4_DSTATE_ku ; } _rtB -> B_311_2833_0 = _rtP ->
P_2231 ; ssCallAccelRunBlock ( S , 311 , 2834 , SS_CALL_MDL_OUTPUTS ) ; if (
_rtDW -> systemEnable_i != 0 ) { _rtDW -> lastSin_pc = muDoubleScalarSin (
_rtP -> P_2242 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_l5 =
muDoubleScalarCos ( _rtP -> P_2242 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_i = 0 ; } _rtB -> B_311_2836_0 = ( ( ( _rtDW -> lastSin_pc *
_rtP -> P_2246 + _rtDW -> lastCos_l5 * _rtP -> P_2245 ) * _rtP -> P_2244 + (
_rtDW -> lastCos_l5 * _rtP -> P_2246 - _rtDW -> lastSin_pc * _rtP -> P_2245 )
* _rtP -> P_2243 ) * _rtP -> P_2240 + _rtP -> P_2241 ) * B_311_2829_0_idx_0 ;
if ( _rtDW -> Integ4_SYSTEM_ENABLE_er != 0 ) { _rtB -> B_311_2837_0 = _rtDW
-> Integ4_DSTATE_a4 ; } else { _rtB -> B_311_2837_0 = _rtP -> P_2247 * _rtB
-> B_311_2836_0 + _rtDW -> Integ4_DSTATE_a4 ; } _rtB -> B_311_2838_0 = _rtP
-> P_2249 ; ssCallAccelRunBlock ( S , 311 , 2839 , SS_CALL_MDL_OUTPUTS ) ; if
( _rtDW -> systemEnable_dp != 0 ) { _rtDW -> lastSin_px = muDoubleScalarSin (
_rtP -> P_2260 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_g5 =
muDoubleScalarCos ( _rtP -> P_2260 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_dp = 0 ; } _rtB -> B_311_2841_0 = ( ( ( _rtDW -> lastSin_px *
_rtP -> P_2264 + _rtDW -> lastCos_g5 * _rtP -> P_2263 ) * _rtP -> P_2262 + (
_rtDW -> lastCos_g5 * _rtP -> P_2264 - _rtDW -> lastSin_px * _rtP -> P_2263 )
* _rtP -> P_2261 ) * _rtP -> P_2258 + _rtP -> P_2259 ) * B_311_2829_0_idx_1 ;
if ( _rtDW -> Integ4_SYSTEM_ENABLE_lc != 0 ) { _rtB -> B_311_2842_0 = _rtDW
-> Integ4_DSTATE_jk ; } else { _rtB -> B_311_2842_0 = _rtP -> P_2265 * _rtB
-> B_311_2841_0 + _rtDW -> Integ4_DSTATE_jk ; } _rtB -> B_311_2843_0 = _rtP
-> P_2267 ; ssCallAccelRunBlock ( S , 311 , 2844 , SS_CALL_MDL_OUTPUTS ) ; if
( _rtDW -> systemEnable_hg != 0 ) { _rtDW -> lastSin_cf = muDoubleScalarSin (
_rtP -> P_2278 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_pu =
muDoubleScalarCos ( _rtP -> P_2278 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_hg = 0 ; } _rtB -> B_311_2846_0 = ( ( ( _rtDW -> lastSin_cf *
_rtP -> P_2282 + _rtDW -> lastCos_pu * _rtP -> P_2281 ) * _rtP -> P_2280 + (
_rtDW -> lastCos_pu * _rtP -> P_2282 - _rtDW -> lastSin_cf * _rtP -> P_2281 )
* _rtP -> P_2279 ) * _rtP -> P_2276 + _rtP -> P_2277 ) * B_311_2829_0_idx_1 ;
if ( _rtDW -> Integ4_SYSTEM_ENABLE_a4 != 0 ) { _rtB -> B_311_2847_0 = _rtDW
-> Integ4_DSTATE_ek ; } else { _rtB -> B_311_2847_0 = _rtP -> P_2283 * _rtB
-> B_311_2846_0 + _rtDW -> Integ4_DSTATE_ek ; } _rtB -> B_311_2848_0 = _rtP
-> P_2285 ; ssCallAccelRunBlock ( S , 311 , 2849 , SS_CALL_MDL_OUTPUTS ) ; if
( _rtDW -> systemEnable_he != 0 ) { _rtDW -> lastSin_af = muDoubleScalarSin (
_rtP -> P_2296 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_gl =
muDoubleScalarCos ( _rtP -> P_2296 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_he = 0 ; } _rtB -> B_311_2851_0 = ( ( ( _rtDW -> lastSin_af *
_rtP -> P_2300 + _rtDW -> lastCos_gl * _rtP -> P_2299 ) * _rtP -> P_2298 + (
_rtDW -> lastCos_gl * _rtP -> P_2300 - _rtDW -> lastSin_af * _rtP -> P_2299 )
* _rtP -> P_2297 ) * _rtP -> P_2294 + _rtP -> P_2295 ) * B_311_2829_0_idx_2 ;
if ( _rtDW -> Integ4_SYSTEM_ENABLE_nk != 0 ) { _rtB -> B_311_2852_0 = _rtDW
-> Integ4_DSTATE_cjp ; } else { _rtB -> B_311_2852_0 = _rtP -> P_2301 * _rtB
-> B_311_2851_0 + _rtDW -> Integ4_DSTATE_cjp ; } _rtB -> B_311_2853_0 = _rtP
-> P_2303 ; ssCallAccelRunBlock ( S , 311 , 2854 , SS_CALL_MDL_OUTPUTS ) ; if
( _rtDW -> systemEnable_cg != 0 ) { _rtDW -> lastSin_ac = muDoubleScalarSin (
_rtP -> P_2314 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_f =
muDoubleScalarCos ( _rtP -> P_2314 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_cg = 0 ; } _rtB -> B_311_2856_0 = ( ( ( _rtDW -> lastSin_ac *
_rtP -> P_2318 + _rtDW -> lastCos_f * _rtP -> P_2317 ) * _rtP -> P_2316 + (
_rtDW -> lastCos_f * _rtP -> P_2318 - _rtDW -> lastSin_ac * _rtP -> P_2317 )
* _rtP -> P_2315 ) * _rtP -> P_2312 + _rtP -> P_2313 ) * B_311_2829_0_idx_2 ;
if ( _rtDW -> Integ4_SYSTEM_ENABLE_pz != 0 ) { _rtB -> B_311_2857_0 = _rtDW
-> Integ4_DSTATE_bd ; } else { _rtB -> B_311_2857_0 = _rtP -> P_2319 * _rtB
-> B_311_2856_0 + _rtDW -> Integ4_DSTATE_bd ; } _rtB -> B_311_2858_0 = _rtP
-> P_2321 ; ssCallAccelRunBlock ( S , 311 , 2859 , SS_CALL_MDL_OUTPUTS ) ;
B_311_2872_0_idx_0 = _rtDW -> Delay_x1_DSTATE [ 0 ] ; B_311_2874_0_idx_0 =
_rtDW -> Delay_x2_DSTATE [ 0 ] ; B_311_1062_0_idx_0 = ( _rtP -> P_2332 *
_rtDW -> Delay_x1_DSTATE [ 0 ] + _rtP -> P_2334 * _rtDW -> Delay_x2_DSTATE [
0 ] ) + _rtP -> P_2330 * B_311_2829_0_idx_0 ; B_311_1349_0_idx_2 = _rtP ->
P_2335 * B_311_1062_0_idx_0 ; rtb_B_311_1335_0 [ 0 ] = _rtP -> P_2336 *
B_311_1349_0_idx_2 ; rtb_B_311_2877_0 [ 0 ] = B_311_1062_0_idx_0 ;
B_311_1349_0_idx_0 = B_311_1349_0_idx_2 ; B_311_2872_0_idx_1 = _rtDW ->
Delay_x1_DSTATE [ 1 ] ; B_311_2874_0_idx_1 = _rtDW -> Delay_x2_DSTATE [ 1 ] ;
B_311_1062_0_idx_0 = ( _rtP -> P_2332 * _rtDW -> Delay_x1_DSTATE [ 1 ] + _rtP
-> P_2334 * _rtDW -> Delay_x2_DSTATE [ 1 ] ) + _rtP -> P_2330 *
B_311_2829_0_idx_1 ; B_311_1349_0_idx_2 = _rtP -> P_2335 * B_311_1062_0_idx_0
; rtb_B_311_1335_0 [ 1 ] = _rtP -> P_2336 * B_311_1349_0_idx_2 ;
rtb_B_311_2877_0 [ 1 ] = B_311_1062_0_idx_0 ; B_311_1349_0_idx_1 =
B_311_1349_0_idx_2 ; B_311_2872_0_idx_2 = _rtDW -> Delay_x1_DSTATE [ 2 ] ;
B_311_2874_0_idx_2 = _rtDW -> Delay_x2_DSTATE [ 2 ] ; B_311_1062_0_idx_0 = (
_rtP -> P_2332 * _rtDW -> Delay_x1_DSTATE [ 2 ] + _rtP -> P_2334 * _rtDW ->
Delay_x2_DSTATE [ 2 ] ) + _rtP -> P_2330 * B_311_2829_0_idx_2 ;
B_311_1349_0_idx_2 = _rtP -> P_2335 * B_311_1062_0_idx_0 ; rtb_B_311_1335_0 [
2 ] = _rtP -> P_2336 * B_311_1349_0_idx_2 ; rtb_B_311_2877_0 [ 2 ] =
B_311_1062_0_idx_0 ; if ( _rtDW -> systemEnable_g != 0 ) { _rtDW -> lastSin_j
= muDoubleScalarSin ( _rtP -> P_2339 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
lastCos_lu = muDoubleScalarCos ( _rtP -> P_2339 * ssGetTaskTime ( S , 2 ) ) ;
_rtDW -> systemEnable_g = 0 ; } _rtB -> B_311_2881_0 = ( ( ( _rtDW ->
lastSin_j * _rtP -> P_2343 + _rtDW -> lastCos_lu * _rtP -> P_2342 ) * _rtP ->
P_2341 + ( _rtDW -> lastCos_lu * _rtP -> P_2343 - _rtDW -> lastSin_j * _rtP
-> P_2342 ) * _rtP -> P_2340 ) * _rtP -> P_2337 + _rtP -> P_2338 ) *
rtb_B_311_1335_0 [ 0 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_d5 != 0 ) { _rtB
-> B_311_2882_0 = _rtDW -> Integ4_DSTATE_en ; } else { _rtB -> B_311_2882_0 =
_rtP -> P_2344 * _rtB -> B_311_2881_0 + _rtDW -> Integ4_DSTATE_en ; } _rtB ->
B_311_2883_0 = _rtP -> P_2346 ; ssCallAccelRunBlock ( S , 311 , 2884 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_2883_0
) { _rtB -> B_311_2891_0 = ( _rtB -> B_311_2882_0 - _rtB -> B_311_2884_0 ) *
_rtP -> P_2355 + ( _rtP -> P_437 * _rtB -> B_311_2881_0 - _rtP -> P_436 *
_rtDW -> UnitDelay_DSTATE_fw ) ; } else { _rtB -> B_311_2891_0 = _rtDW ->
UnitDelay1_DSTATE_ou ; } if ( _rtDW -> systemEnable_i2 != 0 ) { _rtDW ->
lastSin_c4 = muDoubleScalarSin ( _rtP -> P_2360 * ssGetTaskTime ( S , 2 ) ) ;
_rtDW -> lastCos_b = muDoubleScalarCos ( _rtP -> P_2360 * ssGetTaskTime ( S ,
2 ) ) ; _rtDW -> systemEnable_i2 = 0 ; } _rtB -> B_311_2893_0 = ( ( ( _rtDW
-> lastSin_c4 * _rtP -> P_2364 + _rtDW -> lastCos_b * _rtP -> P_2363 ) * _rtP
-> P_2362 + ( _rtDW -> lastCos_b * _rtP -> P_2364 - _rtDW -> lastSin_c4 *
_rtP -> P_2363 ) * _rtP -> P_2361 ) * _rtP -> P_2358 + _rtP -> P_2359 ) *
rtb_B_311_1335_0 [ 0 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_jq != 0 ) { _rtB
-> B_311_2894_0 = _rtDW -> Integ4_DSTATE_og ; } else { _rtB -> B_311_2894_0 =
_rtP -> P_2365 * _rtB -> B_311_2893_0 + _rtDW -> Integ4_DSTATE_og ; } _rtB ->
B_311_2895_0 = _rtP -> P_2367 ; ssCallAccelRunBlock ( S , 311 , 2896 ,
SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_2895_0
) { _rtB -> B_311_2903_0 = ( _rtB -> B_311_2894_0 - _rtB -> B_311_2896_0 ) *
_rtP -> P_2376 + ( _rtP -> P_435 * _rtB -> B_311_2893_0 - _rtP -> P_434 *
_rtDW -> UnitDelay_DSTATE_cl ) ; } else { _rtB -> B_311_2903_0 = _rtDW ->
UnitDelay1_DSTATE_pb ; } rtb_B_311_251_0 = _rtP -> P_2379 *
muDoubleScalarAtan2 ( _rtB -> B_311_2903_0 , _rtB -> B_311_2891_0 ) * _rtP ->
P_2380 ; B_311_2908_0 . re = muDoubleScalarHypot ( _rtB -> B_311_2891_0 ,
_rtB -> B_311_2903_0 ) * muDoubleScalarCos ( rtb_B_311_251_0 ) ; B_311_2908_0
. im = muDoubleScalarHypot ( _rtB -> B_311_2891_0 , _rtB -> B_311_2903_0 ) *
muDoubleScalarSin ( rtb_B_311_251_0 ) ; if ( _rtDW -> systemEnable_o3 != 0 )
{ _rtDW -> lastSin_e = muDoubleScalarSin ( _rtP -> P_2383 * ssGetTaskTime ( S
, 2 ) ) ; _rtDW -> lastCos_a = muDoubleScalarCos ( _rtP -> P_2383 *
ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_o3 = 0 ; } _rtB ->
B_311_2910_0 = ( ( ( _rtDW -> lastSin_e * _rtP -> P_2387 + _rtDW -> lastCos_a
* _rtP -> P_2386 ) * _rtP -> P_2385 + ( _rtDW -> lastCos_a * _rtP -> P_2387 -
_rtDW -> lastSin_e * _rtP -> P_2386 ) * _rtP -> P_2384 ) * _rtP -> P_2381 +
_rtP -> P_2382 ) * rtb_B_311_1335_0 [ 1 ] ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_l2 != 0 ) { _rtB -> B_311_2911_0 = _rtDW ->
Integ4_DSTATE_bl ; } else { _rtB -> B_311_2911_0 = _rtP -> P_2388 * _rtB ->
B_311_2910_0 + _rtDW -> Integ4_DSTATE_bl ; } _rtB -> B_311_2912_0 = _rtP ->
P_2390 ; ssCallAccelRunBlock ( S , 311 , 2913 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_2912_0 ) { _rtB -> B_311_2920_0 = (
_rtB -> B_311_2911_0 - _rtB -> B_311_2913_0 ) * _rtP -> P_2399 + ( _rtP ->
P_441 * _rtB -> B_311_2910_0 - _rtP -> P_440 * _rtDW -> UnitDelay_DSTATE_a4 )
; } else { _rtB -> B_311_2920_0 = _rtDW -> UnitDelay1_DSTATE_f5 ; } if (
_rtDW -> systemEnable_f != 0 ) { _rtDW -> lastSin_ck = muDoubleScalarSin (
_rtP -> P_2404 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_et =
muDoubleScalarCos ( _rtP -> P_2404 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_f = 0 ; } _rtB -> B_311_2922_0 = ( ( ( _rtDW -> lastSin_ck *
_rtP -> P_2408 + _rtDW -> lastCos_et * _rtP -> P_2407 ) * _rtP -> P_2406 + (
_rtDW -> lastCos_et * _rtP -> P_2408 - _rtDW -> lastSin_ck * _rtP -> P_2407 )
* _rtP -> P_2405 ) * _rtP -> P_2402 + _rtP -> P_2403 ) * rtb_B_311_1335_0 [ 1
] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_b4 != 0 ) { _rtB -> B_311_2923_0 =
_rtDW -> Integ4_DSTATE_a2 ; } else { _rtB -> B_311_2923_0 = _rtP -> P_2409 *
_rtB -> B_311_2922_0 + _rtDW -> Integ4_DSTATE_a2 ; } _rtB -> B_311_2924_0 =
_rtP -> P_2411 ; ssCallAccelRunBlock ( S , 311 , 2925 , SS_CALL_MDL_OUTPUTS )
; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_2924_0 ) { _rtB ->
B_311_2932_0 = ( _rtB -> B_311_2923_0 - _rtB -> B_311_2925_0 ) * _rtP ->
P_2420 + ( _rtP -> P_439 * _rtB -> B_311_2922_0 - _rtP -> P_438 * _rtDW ->
UnitDelay_DSTATE_ai ) ; } else { _rtB -> B_311_2932_0 = _rtDW ->
UnitDelay1_DSTATE_p1 ; } rtb_B_311_251_0 = _rtP -> P_2423 *
muDoubleScalarAtan2 ( _rtB -> B_311_2932_0 , _rtB -> B_311_2920_0 ) * _rtP ->
P_2424 ; B_311_2937_0 . re = muDoubleScalarHypot ( _rtB -> B_311_2920_0 ,
_rtB -> B_311_2932_0 ) * muDoubleScalarCos ( rtb_B_311_251_0 ) ; B_311_2937_0
. im = muDoubleScalarHypot ( _rtB -> B_311_2920_0 , _rtB -> B_311_2932_0 ) *
muDoubleScalarSin ( rtb_B_311_251_0 ) ; if ( _rtDW -> systemEnable_dc != 0 )
{ _rtDW -> lastSin_aca = muDoubleScalarSin ( _rtP -> P_2427 * ssGetTaskTime (
S , 2 ) ) ; _rtDW -> lastCos_fr = muDoubleScalarCos ( _rtP -> P_2427 *
ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_dc = 0 ; } _rtB ->
B_311_2939_0 = ( ( ( _rtDW -> lastSin_aca * _rtP -> P_2431 + _rtDW ->
lastCos_fr * _rtP -> P_2430 ) * _rtP -> P_2429 + ( _rtDW -> lastCos_fr * _rtP
-> P_2431 - _rtDW -> lastSin_aca * _rtP -> P_2430 ) * _rtP -> P_2428 ) * _rtP
-> P_2425 + _rtP -> P_2426 ) * rtb_B_311_1335_0 [ 2 ] ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_ke != 0 ) { _rtB -> B_311_2940_0 = _rtDW ->
Integ4_DSTATE_l ; } else { _rtB -> B_311_2940_0 = _rtP -> P_2432 * _rtB ->
B_311_2939_0 + _rtDW -> Integ4_DSTATE_l ; } _rtB -> B_311_2941_0 = _rtP ->
P_2434 ; ssCallAccelRunBlock ( S , 311 , 2942 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_2941_0 ) { _rtB -> B_311_2949_0 = (
_rtB -> B_311_2940_0 - _rtB -> B_311_2942_0 ) * _rtP -> P_2443 + ( _rtP ->
P_445 * _rtB -> B_311_2939_0 - _rtP -> P_444 * _rtDW -> UnitDelay_DSTATE_nwk
) ; } else { _rtB -> B_311_2949_0 = _rtDW -> UnitDelay1_DSTATE_hv ; } if (
_rtDW -> systemEnable_mes != 0 ) { _rtDW -> lastSin_af0 = muDoubleScalarSin (
_rtP -> P_2448 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_o =
muDoubleScalarCos ( _rtP -> P_2448 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_mes = 0 ; } _rtB -> B_311_2951_0 = ( ( ( _rtDW -> lastSin_af0 *
_rtP -> P_2452 + _rtDW -> lastCos_o * _rtP -> P_2451 ) * _rtP -> P_2450 + (
_rtDW -> lastCos_o * _rtP -> P_2452 - _rtDW -> lastSin_af0 * _rtP -> P_2451 )
* _rtP -> P_2449 ) * _rtP -> P_2446 + _rtP -> P_2447 ) * rtb_B_311_1335_0 [ 2
] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_nb != 0 ) { _rtB -> B_311_2952_0 =
_rtDW -> Integ4_DSTATE_kv ; } else { _rtB -> B_311_2952_0 = _rtP -> P_2453 *
_rtB -> B_311_2951_0 + _rtDW -> Integ4_DSTATE_kv ; } _rtB -> B_311_2953_0 =
_rtP -> P_2455 ; ssCallAccelRunBlock ( S , 311 , 2954 , SS_CALL_MDL_OUTPUTS )
; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_2953_0 ) { _rtB ->
B_311_2961_0 = ( _rtB -> B_311_2952_0 - _rtB -> B_311_2954_0 ) * _rtP ->
P_2464 + ( _rtP -> P_443 * _rtB -> B_311_2951_0 - _rtP -> P_442 * _rtDW ->
UnitDelay_DSTATE_ck ) ; } else { _rtB -> B_311_2961_0 = _rtDW ->
UnitDelay1_DSTATE_b3 ; } rtb_B_311_251_0 = _rtP -> P_2467 *
muDoubleScalarAtan2 ( _rtB -> B_311_2961_0 , _rtB -> B_311_2949_0 ) * _rtP ->
P_2468 ; B_311_2966_0 . re = muDoubleScalarHypot ( _rtB -> B_311_2949_0 ,
_rtB -> B_311_2961_0 ) * muDoubleScalarCos ( rtb_B_311_251_0 ) ; B_311_2966_0
. im = muDoubleScalarHypot ( _rtB -> B_311_2949_0 , _rtB -> B_311_2961_0 ) *
muDoubleScalarSin ( rtb_B_311_251_0 ) ;
Auto_microgrid_forecasting_020822_NegSeqComputation ( S , _rtB -> B_311_356_0
, B_311_2908_0 , B_311_2937_0 , B_311_2966_0 , & _rtB -> PosSeqComputation_kv
, & _rtDW -> PosSeqComputation_kv , & _rtP -> PosSeqComputation_kv ) ;
Auto_microgrid_forecasting_020822_NegSeqComputation ( S , _rtB -> B_311_357_0
, B_311_2908_0 , B_311_2937_0 , B_311_2966_0 , & _rtB -> NegSeqComputation_o
, & _rtDW -> NegSeqComputation_o , & _rtP -> NegSeqComputation_o ) ;
Auto_microgrid_forecasting_020822_ZeroSeqComputation ( S , _rtB ->
B_311_358_0 , B_311_2908_0 , B_311_2937_0 , B_311_2966_0 , & _rtB ->
ZeroSeqComputation_al , & _rtDW -> ZeroSeqComputation_al , & _rtP ->
ZeroSeqComputation_al ) ; rtb_B_311_1335_0 [ 0 ] = muDoubleScalarHypot ( _rtB
-> PosSeqComputation_kv . B_122_2_0 . re , _rtB -> PosSeqComputation_kv .
B_122_2_0 . im ) ; rtb_B_311_1335_0 [ 1 ] = muDoubleScalarHypot ( _rtB ->
NegSeqComputation_o . B_122_2_0 . re , _rtB -> NegSeqComputation_o .
B_122_2_0 . im ) ; rtb_B_311_1335_0 [ 2 ] = muDoubleScalarHypot ( _rtB ->
ZeroSeqComputation_al . B_124_1_0 . re , _rtB -> ZeroSeqComputation_al .
B_124_1_0 . im ) ; } i = ssIsSampleHit ( S , 5 , 0 ) ; if ( i != 0 ) { _rtB
-> B_311_2971_0 = _rtB -> B_156_13_0 - _rtB -> B_311_300_0 ; } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_2972_0 =
rtb_B_311_1335_0 [ 0 ] - _rtB -> B_311_2971_0 ; } i = ssIsSampleHit ( S , 5 ,
0 ) ; if ( i != 0 ) { rtPrevAction = _rtDW -> SwitchCase_ActiveSubsystem ; if
( ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtB -> B_311_1262_0 < 0.0 ) {
rtb_B_299_3_0 = muDoubleScalarCeil ( _rtB -> B_311_1262_0 ) ; } else {
rtb_B_299_3_0 = muDoubleScalarFloor ( _rtB -> B_311_1262_0 ) ; } if (
muDoubleScalarIsNaN ( rtb_B_299_3_0 ) || muDoubleScalarIsInf ( rtb_B_299_3_0
) ) { rtb_B_299_3_0 = 0.0 ; } else { rtb_B_299_3_0 = muDoubleScalarRem (
rtb_B_299_3_0 , 4.294967296E+9 ) ; } switch ( rtb_B_299_3_0 < 0.0 ? - (
int32_T ) ( uint32_T ) - rtb_B_299_3_0 : ( int32_T ) ( uint32_T )
rtb_B_299_3_0 ) { case 2 : rtAction = 0 ; break ; case 3 : rtAction = 1 ;
break ; default : rtAction = 2 ; break ; } _rtDW ->
SwitchCase_ActiveSubsystem = rtAction ; } else { rtAction = _rtDW ->
SwitchCase_ActiveSubsystem ; } if ( rtPrevAction != rtAction ) { switch (
rtPrevAction ) { case 0 :
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem_Disable ( S ) ;
break ; case 1 :
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem_Disable ( S ) ;
break ; case 2 : break ; } } switch ( rtAction ) { case 0 : if ( rtAction !=
rtPrevAction ) {
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem_Enable ( S ) ; }
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem ( S , _rtB ->
B_311_2972_0 , & _rtB -> B_311_2976_0 , & _rtB -> B_311_2977_0 , & _rtB ->
SwitchCaseActionSubsystem ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
srUpdateBC ( _rtDW -> SwitchCaseActionSubsystem .
SwitchCaseActionSubsystem_SubsysRanBC ) ; } break ; case 1 : if ( rtAction !=
rtPrevAction ) {
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem_Enable ( S ) ; }
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem ( S , _rtB ->
B_311_2972_0 , & _rtB -> B_311_2976_0 , & _rtB -> B_311_2977_0 , & _rtB ->
SwitchCaseActionSubsystem1 ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
srUpdateBC ( _rtDW -> SwitchCaseActionSubsystem1 .
SwitchCaseActionSubsystem_SubsysRanBC ) ; } break ; case 2 : break ; } _rtB
-> B_311_2978_0 = ( _rtB -> B_311_2976_0 || _rtB -> B_311_2977_0 ) ; } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { if ( ssGetTaskTime ( S , 2 ) >
_rtP -> P_2469 ) { rtb_B_311_2980_0 = _rtB -> B_311_2978_0 ; } else {
rtb_B_311_2980_0 = _rtB -> B_311_304_0_l ; } } i = ssIsSampleHit ( S , 5 , 0
) ; if ( i != 0 ) { _rtB -> B_311_2981_0 = ( _rtB -> B_311_1262_0 == _rtB ->
B_311_295_0_c ) ; _rtB -> B_311_2982_0 = ( _rtB -> B_311_1262_0 == _rtB ->
B_311_296_0 ) ; _rtB -> B_311_2983_0 = ( _rtB -> B_311_2981_0 || _rtB ->
B_311_2982_0 ) ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB ->
B_311_2984_0 = ( ( rtb_B_311_2980_0 != 0.0 ) && _rtB -> B_311_2983_0 ) ; _rtB
-> B_311_2986_0 = ( _rtB -> B_311_2984_0 == _rtB -> B_311_294_0_p ) ; if ( (
_rtB -> B_311_2986_0 <= 0.0 ) && ( _rtDW ->
DiscreteTimeIntegrator2_PrevResetState == 1 ) ) { _rtDW ->
DiscreteTimeIntegrator2_DSTATE = _rtP -> P_2471 ; } _rtB -> B_311_2987_0 =
_rtDW -> DiscreteTimeIntegrator2_DSTATE ; } i = ssIsSampleHit ( S , 5 , 0 ) ;
if ( i != 0 ) { rtPrevAction = _rtDW -> SwitchCase1_ActiveSubsystem ; if (
ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtB -> B_311_1262_0 < 0.0 ) {
rtb_B_299_3_0 = muDoubleScalarCeil ( _rtB -> B_311_1262_0 ) ; } else {
rtb_B_299_3_0 = muDoubleScalarFloor ( _rtB -> B_311_1262_0 ) ; } if (
muDoubleScalarIsNaN ( rtb_B_299_3_0 ) || muDoubleScalarIsInf ( rtb_B_299_3_0
) ) { rtb_B_299_3_0 = 0.0 ; } else { rtb_B_299_3_0 = muDoubleScalarRem (
rtb_B_299_3_0 , 4.294967296E+9 ) ; } switch ( rtb_B_299_3_0 < 0.0 ? - (
int32_T ) ( uint32_T ) - rtb_B_299_3_0 : ( int32_T ) ( uint32_T )
rtb_B_299_3_0 ) { case 2 : rtAction = 0 ; break ; case 3 : rtAction = 1 ;
break ; default : rtAction = 2 ; break ; } _rtDW ->
SwitchCase1_ActiveSubsystem = rtAction ; } else { rtAction = _rtDW ->
SwitchCase1_ActiveSubsystem ; } if ( rtPrevAction != rtAction ) { switch (
rtPrevAction ) { case 0 :
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2_Disable ( S ) ;
break ; case 1 :
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2_Disable ( S ) ;
break ; case 2 : break ; } } switch ( rtAction ) { case 0 : if ( rtAction !=
rtPrevAction ) {
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2_Enable ( S ) ; }
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2 ( S , _rtB ->
B_311_2987_0 , & _rtB -> B_311_2991_0 , & _rtB -> SwitchCaseActionSubsystem2
) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( _rtDW ->
SwitchCaseActionSubsystem2 . SwitchCaseActionSubsystem2_SubsysRanBC ) ; }
break ; case 1 : if ( rtAction != rtPrevAction ) {
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2_Enable ( S ) ; }
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2 ( S , _rtB ->
B_311_2987_0 , & _rtB -> B_311_2991_0 , & _rtB -> SwitchCaseActionSubsystem3
) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( _rtDW ->
SwitchCaseActionSubsystem3 . SwitchCaseActionSubsystem2_SubsysRanBC ) ; }
break ; case 2 : break ; } if ( ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtB ->
B_311_2991_0 && ( _rtZCE -> CounterAST_Trig_ZCE != 1 ) ) { _rtB -> B_219_1_0
= _rtDW -> Memory_PreviousInput_mz + _rtB -> B_219_0_0 ; _rtDW ->
Memory_PreviousInput_mz = _rtB -> B_219_1_0 ; _rtDW -> CounterAST_SubsysRanBC
= 4 ; } _rtZCE -> CounterAST_Trig_ZCE = _rtB -> B_311_2991_0 ; } if ( _rtB ->
B_219_1_0 > _rtP -> P_2472 ) { _rtB -> B_311_2993_0 = _rtP -> P_2472 ; } else
if ( _rtB -> B_219_1_0 < _rtP -> P_2473 ) { _rtB -> B_311_2993_0 = _rtP ->
P_2473 ; } else { _rtB -> B_311_2993_0 = _rtB -> B_219_1_0 ; } } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { for ( i = 0 ; i < 3 ; i ++ ) {
B_311_2995_0 [ i ] = _rtP -> P_2475 * ( _rtP -> P_2474 [ i + 6 ] *
B_311_1349_0_idx_2 + ( _rtP -> P_2474 [ i + 3 ] * B_311_1349_0_idx_1 + _rtP
-> P_2474 [ i ] * B_311_1349_0_idx_0 ) ) ; } B_311_2996_0 = _rtDW ->
DiscreteTimeIntegrator2_DSTATE_h ;
Auto_microgrid_forecasting_020822_Subsystem1_h ( S , _rtB -> B_311_361_0 , &
B_311_2995_0 [ 0 ] , B_311_2996_0 , & _rtB -> Subsystem1_hv , & _rtDW ->
Subsystem1_hv ) ; Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( S ,
_rtB -> B_311_363_0 , & B_311_2995_0 [ 0 ] , B_311_2996_0 , & _rtB ->
Subsystempi2delay_b , & _rtDW -> Subsystempi2delay_b ) ; if ( _rtB ->
B_311_361_0 != 0 ) { _rtB -> B_311_2999_0 [ 0 ] = _rtB -> Subsystem1_hv .
B_241_0_0 ; _rtB -> B_311_2999_0 [ 1 ] = _rtB -> Subsystem1_hv . B_241_1_0 ;
} else { _rtB -> B_311_2999_0 [ 0 ] = _rtB -> Subsystempi2delay_b . B_240_0_0
; _rtB -> B_311_2999_0 [ 1 ] = _rtB -> Subsystempi2delay_b . B_240_1_0 ; } if
( _rtDW -> Integ4_SYSTEM_ENABLE_oa != 0 ) { _rtB -> B_311_3000_0 = _rtDW ->
Integ4_DSTATE_bm ; } else { _rtB -> B_311_3000_0 = _rtP -> P_2478 * _rtB ->
B_311_2999_0 [ 0 ] + _rtDW -> Integ4_DSTATE_bm ; } rtb_B_311_3001_0 = _rtDW
-> DiscreteTimeIntegrator1_DSTATE_d ; for ( i = 0 ; i < 3 ; i ++ ) {
rtb_B_311_35_0 = _rtP -> P_2482 [ i + 6 ] * B_311_1349_0_idx_2 + ( _rtP ->
P_2482 [ i + 3 ] * B_311_1349_0_idx_1 + _rtP -> P_2482 [ i ] *
B_311_1349_0_idx_0 ) ; B_311_3003_0 [ i ] = _rtP -> P_2483 * rtb_B_311_35_0 ;
rtb_B_311_1335_0 [ i ] = rtb_B_311_35_0 ; }
Auto_microgrid_forecasting_020822_Subsystem1_h ( S , _rtB -> B_311_352_0 , &
B_311_3003_0 [ 0 ] , B_311_2996_0 , & _rtB -> Subsystem1_h3 , & _rtDW ->
Subsystem1_h3 ) ; Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( S ,
_rtB -> B_311_354_0 , & B_311_3003_0 [ 0 ] , B_311_2996_0 , & _rtB ->
Subsystempi2delay_hh , & _rtDW -> Subsystempi2delay_hh ) ; if ( _rtB ->
B_311_352_0 != 0 ) { B_311_3006_0_idx_1 = _rtB -> Subsystem1_h3 . B_241_1_0 ;
} else { B_311_3006_0_idx_1 = _rtB -> Subsystempi2delay_hh . B_240_1_0 ; }
_rtB -> B_311_3009_0 = ( _rtP -> P_2484 * B_311_3006_0_idx_1 +
rtb_B_311_3001_0 ) + _rtB -> B_311_355_0 ; rtb_B_311_3001_0 = _rtP -> P_2485
* _rtB -> B_311_3009_0 ; if ( rtb_B_311_3001_0 > _rtP -> P_2486 ) {
rtb_B_311_3012_0 = _rtP -> P_2486 ; } else if ( rtb_B_311_3001_0 < _rtP ->
P_2487 ) { rtb_B_311_3012_0 = _rtP -> P_2487 ; } else { rtb_B_311_3012_0 =
rtb_B_311_3001_0 ; } rtb_B_311_3012_0 = 1.0 / rtb_B_311_3012_0 / 2.0e-5 ;
rtb_B_311_2980_0 = muDoubleScalarCeil ( rtb_B_311_3012_0 ) ; _rtB ->
B_311_3014_0 = _rtP -> P_2488 * rtb_B_311_2980_0 ; ssCallAccelRunBlock ( S ,
311 , 3015 , SS_CALL_MDL_OUTPUTS ) ; if ( ssGetTaskTime ( S , 2 ) >= _rtB ->
B_311_348_0 ) { rtb_B_311_2980_0 = rtb_B_311_3012_0 - rtb_B_311_2980_0 ; _rtB
-> B_311_3021_0 = ( ( _rtB -> B_311_2999_0 [ 0 ] - _rtDW ->
UnitDelay_DSTATE_df ) * rtb_B_311_2980_0 * _rtP -> P_432 + _rtB ->
B_311_2999_0 [ 0 ] ) * ( rtb_B_311_2980_0 / rtb_B_311_3012_0 ) + ( _rtB ->
B_311_3000_0 - _rtB -> B_311_3015_0 ) * rtb_B_311_3001_0 ; } else { _rtB ->
B_311_3021_0 = _rtDW -> UnitDelay1_DSTATE_fm ; } if ( _rtDW ->
Integ4_SYSTEM_ENABLE_ee != 0 ) { _rtB -> B_311_3022_0 = _rtDW ->
Integ4_DSTATE_kl ; } else { _rtB -> B_311_3022_0 = _rtP -> P_2499 * _rtB ->
B_311_2999_0 [ 1 ] + _rtDW -> Integ4_DSTATE_kl ; } if ( rtb_B_311_3001_0 >
_rtP -> P_2501 ) { rtb_B_311_3012_0 = _rtP -> P_2501 ; } else if (
rtb_B_311_3001_0 < _rtP -> P_2502 ) { rtb_B_311_3012_0 = _rtP -> P_2502 ; }
else { rtb_B_311_3012_0 = rtb_B_311_3001_0 ; } rtb_B_311_3012_0 = 1.0 /
rtb_B_311_3012_0 / 2.0e-5 ; rtb_B_311_2980_0 = muDoubleScalarCeil (
rtb_B_311_3012_0 ) ; _rtB -> B_311_3026_0 = _rtP -> P_2503 * rtb_B_311_2980_0
; ssCallAccelRunBlock ( S , 311 , 3027 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_349_0 ) { rtb_B_311_2980_0 =
rtb_B_311_3012_0 - rtb_B_311_2980_0 ; _rtB -> B_311_3033_0 = ( ( _rtB ->
B_311_2999_0 [ 1 ] - _rtDW -> UnitDelay_DSTATE_ai4 ) * rtb_B_311_2980_0 *
_rtP -> P_433 + _rtB -> B_311_2999_0 [ 1 ] ) * ( rtb_B_311_2980_0 /
rtb_B_311_3012_0 ) + ( _rtB -> B_311_3022_0 - _rtB -> B_311_3027_0 ) *
rtb_B_311_3001_0 ; } else { _rtB -> B_311_3033_0 = _rtDW ->
UnitDelay1_DSTATE_nn ; } rtb_B_311_3001_0 = ssGetTaskTime ( S , 2 ) ; if (
rtb_B_311_3001_0 > _rtP -> P_2514 ) { rtb_B_311_3012_0 = _rtB -> B_311_3021_0
* _rtB -> B_311_3021_0 + _rtB -> B_311_3033_0 * _rtB -> B_311_3033_0 ; if (
ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtDW -> Sqrt_DWORK1_o != 0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Sqrt_DWORK1_o = 0
; } rtb_B_311_2980_0 = muDoubleScalarSqrt ( rtb_B_311_3012_0 ) ; } else if (
rtb_B_311_3012_0 < 0.0 ) { rtb_B_311_2980_0 = - muDoubleScalarSqrt (
muDoubleScalarAbs ( rtb_B_311_3012_0 ) ) ; _rtDW -> Sqrt_DWORK1_o = 1 ; }
else { rtb_B_311_2980_0 = muDoubleScalarSqrt ( rtb_B_311_3012_0 ) ; } _rtB ->
B_311_3036_0 = _rtP -> P_446 * rtb_B_311_2980_0 ; } else { _rtB ->
B_311_3036_0 = _rtB -> B_311_368_0 ; } _rtB -> B_311_3038_0 = ( _rtB ->
B_311_3036_0 > _rtB -> B_311_375_0 ) ; if ( ( _rtB -> B_311_3038_0 <= 0.0 )
&& ( _rtDW -> DiscreteTimeIntegrator2_PrevResetState_g == 1 ) ) { _rtDW ->
DiscreteTimeIntegrator2_DSTATE_e = _rtP -> P_2516 ; } B_311_3040_0 = ( _rtDW
-> DiscreteTimeIntegrator2_DSTATE_e > _rtB -> B_311_365_0 ) ;
Auto_microgrid_forecasting_020822_CounterASTfmax ( S , B_311_3040_0 , & _rtB
-> CounterASTumax , & _rtDW -> CounterASTumax , & _rtZCE -> CounterASTumax )
; if ( _rtB -> CounterASTumax . B_263_1_0 > _rtP -> P_2517 ) {
rtb_B_311_3012_0 = _rtP -> P_2517 ; } else if ( _rtB -> CounterASTumax .
B_263_1_0 < _rtP -> P_2518 ) { rtb_B_311_3012_0 = _rtP -> P_2518 ; } else {
rtb_B_311_3012_0 = _rtB -> CounterASTumax . B_263_1_0 ; } _rtB ->
B_311_3044_0 = ( _rtB -> B_311_3036_0 < _rtB -> B_311_376_0 ) ; if ( ( _rtB
-> B_311_3044_0 <= 0.0 ) && ( _rtDW -> DiscreteTimeIntegrator3_PrevResetState
== 1 ) ) { _rtDW -> DiscreteTimeIntegrator3_DSTATE = _rtP -> P_2520 ; }
B_311_3046_0 = ( _rtDW -> DiscreteTimeIntegrator3_DSTATE > _rtB ->
B_311_366_0_o ) ; Auto_microgrid_forecasting_020822_CounterASTfmax ( S ,
B_311_3046_0 , & _rtB -> CounterASTumin , & _rtDW -> CounterASTumin , &
_rtZCE -> CounterASTumin ) ; if ( _rtB -> CounterASTumin . B_263_1_0 > _rtP
-> P_2521 ) { rtb_B_299_3_0 = _rtP -> P_2521 ; } else if ( _rtB ->
CounterASTumin . B_263_1_0 < _rtP -> P_2522 ) { rtb_B_299_3_0 = _rtP ->
P_2522 ; } else { rtb_B_299_3_0 = _rtB -> CounterASTumin . B_263_1_0 ; } _rtB
-> B_311_3049_0 = ( ( rtb_B_311_3012_0 != 0.0 ) || ( rtb_B_299_3_0 != 0.0 ) )
; if ( rtb_B_311_3001_0 > _rtP -> P_2523 ) { _rtB -> B_311_3051_0 = _rtP ->
P_447 * _rtB -> B_311_3009_0 * _rtP -> P_448 ; } else { _rtB -> B_311_3051_0
= _rtB -> B_311_368_0 ; } _rtB -> B_311_3052_0 = ( _rtB -> B_311_3051_0 >
_rtB -> B_311_373_0 ) ; _rtB -> B_311_3053_0 = _rtB -> B_311_3052_0 ; if ( (
_rtB -> B_311_3053_0 <= 0.0 ) && ( _rtDW ->
DiscreteTimeIntegrator1_PrevResetState_j == 1 ) ) { _rtDW ->
DiscreteTimeIntegrator1_DSTATE_o = _rtP -> P_2525 ; } _rtB -> B_311_3054_0 =
_rtDW -> DiscreteTimeIntegrator1_DSTATE_o ; _rtB -> B_311_3055_0 = ( _rtB ->
B_311_3054_0 > _rtB -> B_311_364_0 ) ;
Auto_microgrid_forecasting_020822_CounterASTfmax ( S , _rtB -> B_311_3055_0 ,
& _rtB -> CounterASTfmax , & _rtDW -> CounterASTfmax , & _rtZCE ->
CounterASTfmax ) ; if ( _rtB -> CounterASTfmax . B_263_1_0 > _rtP -> P_2526 )
{ _rtB -> B_311_3057_0 = _rtP -> P_2526 ; } else if ( _rtB -> CounterASTfmax
. B_263_1_0 < _rtP -> P_2527 ) { _rtB -> B_311_3057_0 = _rtP -> P_2527 ; }
else { _rtB -> B_311_3057_0 = _rtB -> CounterASTfmax . B_263_1_0 ; } _rtB ->
B_311_3059_0 = ( _rtB -> B_311_3051_0 < _rtB -> B_311_374_0 ) ; if ( ( _rtB
-> B_311_3059_0 <= 0.0 ) && ( _rtDW -> DiscreteTimeIntegrator4_PrevResetState
== 1 ) ) { _rtDW -> DiscreteTimeIntegrator4_DSTATE = _rtP -> P_2529 ; } _rtB
-> B_311_3061_0 = ( _rtDW -> DiscreteTimeIntegrator4_DSTATE > _rtB ->
B_311_367_0 ) ; Auto_microgrid_forecasting_020822_CounterASTfmax ( S , _rtB
-> B_311_3061_0 , & _rtB -> CounterASTfmin , & _rtDW -> CounterASTfmin , &
_rtZCE -> CounterASTfmin ) ; if ( _rtB -> CounterASTfmin . B_263_1_0 > _rtP
-> P_2530 ) { _rtB -> B_311_3063_0 = _rtP -> P_2530 ; } else if ( _rtB ->
CounterASTfmin . B_263_1_0 < _rtP -> P_2531 ) { _rtB -> B_311_3063_0 = _rtP
-> P_2531 ; } else { _rtB -> B_311_3063_0 = _rtB -> CounterASTfmin .
B_263_1_0 ; } _rtB -> B_311_3064_0 = ( ( _rtB -> B_311_3057_0 != 0.0 ) || (
_rtB -> B_311_3063_0 != 0.0 ) ) ; _rtB -> B_311_3066_0 = ( ( ! ( _rtB ->
B_311_2993_0 != 0.0 ) ) && ( ( ! _rtB -> B_311_3049_0 ) && ( ! _rtB ->
B_311_3064_0 ) ) ) ; ssCallAccelRunBlock ( S , 311 , 3067 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_311_3068_0 = _rtDW -> UnitDelay_DSTATE_js ;
_rtB -> B_311_3069_0 = _rtDW -> dw_delay_DSTATE_c ; rtb_B_311_3001_0 = _rtP
-> P_2534 * _rtB -> B_311_3069_0 - _rtDW -> dw_predict_DSTATE_p ;
rtb_B_311_3012_0 = _rtB -> B_311_423_0 + rtb_B_311_3001_0 ; _rtB ->
B_311_3074_0 = _rtP -> P_2536 * rtb_B_311_3012_0 ; } i = ssIsSampleHit ( S ,
4 , 0 ) ; if ( i != 0 ) { i = ssIsSampleHit ( S , 4 , 0 ) ; if ( i != 0 ) {
rtPrevAction = _rtDW -> SwitchCase_ActiveSubsystem_c ; if ( _rtB ->
B_311_1262_0 < 0.0 ) { rtb_B_299_3_0 = muDoubleScalarCeil ( _rtB ->
B_311_1262_0 ) ; } else { rtb_B_299_3_0 = muDoubleScalarFloor ( _rtB ->
B_311_1262_0 ) ; } if ( muDoubleScalarIsNaN ( rtb_B_299_3_0 ) ||
muDoubleScalarIsInf ( rtb_B_299_3_0 ) ) { rtb_B_299_3_0 = 0.0 ; } else {
rtb_B_299_3_0 = muDoubleScalarRem ( rtb_B_299_3_0 , 4.294967296E+9 ) ; }
switch ( rtb_B_299_3_0 < 0.0 ? - ( int32_T ) ( uint32_T ) - rtb_B_299_3_0 : (
int32_T ) ( uint32_T ) rtb_B_299_3_0 ) { case 1 : rtAction = 0 ; break ; case
2 : case 3 : rtAction = 1 ; break ; case 4 : rtAction = 2 ; break ; case 5 :
rtAction = 3 ; break ; default : rtAction = 4 ; break ; } _rtDW ->
SwitchCase_ActiveSubsystem_c = rtAction ; if ( ( rtPrevAction != rtAction )
&& ( rtPrevAction == 0 ) ) { ssCallAccelRunBlock ( S , 206 , 0 ,
SS_CALL_RTW_GENERATED_DISABLE ) ; ssCallAccelRunBlock ( S , 204 , 0 ,
SS_CALL_RTW_GENERATED_DISABLE ) ; } switch ( rtAction ) { case 0 : if (
rtAction != rtPrevAction ) { if ( ssGetTaskTime ( S , 4 ) != ssGetTStart ( S
) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
ssCallAccelRunBlock ( S , 206 , 0 , SS_CALL_RTW_GENERATED_ENABLE ) ;
ssCallAccelRunBlock ( S , 204 , 0 , SS_CALL_RTW_GENERATED_ENABLE ) ; } i =
ssIsSampleHit ( S , 4 , 0 ) ; if ( i != 0 ) { _rtB -> B_208_1_0 = _rtP ->
P_360 * _rtB -> B_311_2019_0 - _rtB -> B_311_3068_0 ; if ( _rtDW ->
UnitDelay_DSTATE_jb ) { _rtB -> B_208_5_0_n = ( _rtB -> B_208_1_0 >= _rtB ->
B_208_5_0 ) ; } else { _rtB -> B_208_5_0_n = _rtDW -> ICic_PreviousInput_b ;
} if ( ssGetTaskTime ( S , 4 ) > _rtP -> P_361 ) { _rtB -> B_208_7_0 = _rtB
-> B_208_5_0_n ; } else { _rtB -> B_208_7_0 = _rtB -> B_208_0_0 ; }
ssCallAccelRunBlock ( S , 208 , 8 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 208 , 9 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_207_0_0
= _rtDW -> Delay3_DSTATE_j ; _rtB -> B_207_1_0 = _rtDW -> Delay2_DSTATE_f ;
_rtB -> B_207_2_0 = _rtDW -> Delay1_DSTATE_n ; ssCallAccelRunBlock ( S , 206
, 0 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 207 , 4 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 207 , 5 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 207 , 6 ,
SS_CALL_MDL_OUTPUTS ) ; rtb_B_311_2980_0 = _rtP -> P_362 ; _rtB -> B_208_14_0
= ( _rtP -> P_362 - _rtB -> B_208_11_0 [ 0 ] <= _rtB -> B_208_2_0 ) ;
rtb_B_311_251_0 = _rtP -> P_363 * _rtB -> B_311_3068_0 ; _rtB -> B_208_16_0 =
_rtP -> P_364 * rtb_B_311_251_0 ; if ( _rtDW -> Integrator_IC_LOADING_i != 0
) { _rtDW -> Integrator_DSTATE_kt = _rtB -> B_208_16_0 ; if ( _rtDW ->
Integrator_DSTATE_kt >= _rtP -> P_366 ) { _rtDW -> Integrator_DSTATE_kt =
_rtP -> P_366 ; } else { if ( _rtDW -> Integrator_DSTATE_kt <= _rtP -> P_367
) { _rtDW -> Integrator_DSTATE_kt = _rtP -> P_367 ; } } } if ( _rtB ->
B_208_14_0 || ( _rtDW -> Integrator_PrevResetState_c != 0 ) ) { _rtDW ->
Integrator_DSTATE_kt = _rtB -> B_208_16_0 ; if ( _rtDW ->
Integrator_DSTATE_kt >= _rtP -> P_366 ) { _rtDW -> Integrator_DSTATE_kt =
_rtP -> P_366 ; } else { if ( _rtDW -> Integrator_DSTATE_kt <= _rtP -> P_367
) { _rtDW -> Integrator_DSTATE_kt = _rtP -> P_367 ; } } } if ( _rtDW ->
Integrator_DSTATE_kt >= _rtP -> P_366 ) { _rtDW -> Integrator_DSTATE_kt =
_rtP -> P_366 ; } else { if ( _rtDW -> Integrator_DSTATE_kt <= _rtP -> P_367
) { _rtDW -> Integrator_DSTATE_kt = _rtP -> P_367 ; } } if ( _rtDW ->
Integrator_DSTATE_kt > _rtP -> P_368 ) { _rtB -> B_208_18_0 = _rtP -> P_368 ;
} else if ( _rtDW -> Integrator_DSTATE_kt < _rtP -> P_369 ) { _rtB ->
B_208_18_0 = _rtP -> P_369 ; } else { _rtB -> B_208_18_0 = _rtDW ->
Integrator_DSTATE_kt ; } _rtB -> B_208_19_0 = _rtB -> B_208_18_0 * _rtP ->
P_370 ; _rtB -> B_208_21_0 = _rtB -> B_208_19_0 - _rtDW -> UD_DSTATE_k ; _rtB
-> B_208_22_0 = rtb_B_311_251_0 - _rtB -> B_208_3_0 ; _rtDW ->
DiscreteTransferFcn_tmp = ( _rtB -> B_208_22_0 - _rtP -> P_373 [ 1 ] * _rtDW
-> DiscreteTransferFcn_states ) / _rtP -> P_373 [ 0 ] ; _rtB -> B_208_23_0 =
_rtP -> P_372 [ 0 ] * _rtDW -> DiscreteTransferFcn_tmp + _rtP -> P_372 [ 1 ]
* _rtDW -> DiscreteTransferFcn_states ; if ( ! ( _rtB -> B_208_7_0 != 0.0 ) )
{ _rtB -> B_208_26_0 = _rtB -> B_311_3074_0 ; } else { _rtB -> B_208_26_0 =
_rtDW -> ICic_PreviousInput ; } ssCallAccelRunBlock ( S , 204 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; if ( _rtB -> B_212_1_0 > _rtP -> P_376 ) { _rtB ->
B_208_29_0 = _rtB -> B_206_0_1 ; } else if ( _rtB -> B_204_0_1 > _rtP ->
P_353 ) { _rtB -> B_208_29_0 = _rtP -> P_353 ; } else if ( _rtB -> B_204_0_1
< _rtP -> P_354 ) { _rtB -> B_208_29_0 = _rtP -> P_354 ; } else { _rtB ->
B_208_29_0 = _rtB -> B_204_0_1 ; } ssCallAccelRunBlock ( S , 208 , 30 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_208_31_0 = ( _rtB -> B_208_29_0 <= _rtB ->
B_208_1_0_a ) ; ssCallAccelRunBlock ( S , 208 , 32 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 208 , 33 , SS_CALL_MDL_OUTPUTS ) ; if ( _rtB ->
B_311_3074_0 > _rtP -> P_377 ) { rtb_B_299_3_0 = _rtP -> P_377 ; } else if (
_rtB -> B_311_3074_0 < _rtP -> P_378 ) { rtb_B_299_3_0 = _rtP -> P_378 ; }
else { rtb_B_299_3_0 = _rtB -> B_311_3074_0 ; } B_311_1965_0_idx_0 = _rtB ->
B_208_29_0 / rtb_B_299_3_0 ; ssCallAccelRunBlock ( S , 208 , 36 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 208 , 37 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 208 , 38 ,
SS_CALL_MDL_OUTPUTS ) ; rtb_B_311_50_0 = muDoubleScalarMax ( _rtB ->
B_208_11_0 [ 0 ] , rtb_B_311_2980_0 ) ; _rtB -> B_208_42_0 = 1.0 / ( ( real_T
) ( rtb_B_311_50_0 == 0.0 ) * 2.2204460492503131e-16 + rtb_B_311_50_0 ) * (
_rtB -> B_208_16_0 - _rtB -> B_208_18_0 ) ; ssCallAccelRunBlock ( S , 208 ,
43 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_208_44_0 = ! _rtB -> B_208_31_0 ;
ssCallAccelRunBlock ( S , 208 , 45 , SS_CALL_MDL_OUTPUTS ) ; if ( _rtB ->
B_208_44_0 ) { _rtB -> B_208_47_0 = ! _rtB -> B_208_5_0_n ; } else { _rtB ->
B_208_47_0 = ! ( _rtB -> B_208_4_0 != 0.0 ) ; } rtb_B_311_251_0 =
B_311_1965_0_idx_0 - _rtDW -> PrevY_g ; if ( rtb_B_311_251_0 > _rtP -> P_379
) { rtb_B_311_2980_0 = _rtDW -> PrevY_g + _rtP -> P_379 ; } else if (
rtb_B_311_251_0 < _rtP -> P_380 ) { rtb_B_311_2980_0 = _rtDW -> PrevY_g +
_rtP -> P_380 ; } else { rtb_B_311_2980_0 = B_311_1965_0_idx_0 ; } _rtDW ->
PrevY_g = rtb_B_311_2980_0 ; if ( rtb_B_311_2980_0 > _rtP -> P_382 ) { _rtB
-> B_212_12_0 = _rtP -> P_382 ; } else if ( rtb_B_311_2980_0 < _rtP -> P_383
) { _rtB -> B_212_12_0 = _rtP -> P_383 ; } else { _rtB -> B_212_12_0 =
rtb_B_311_2980_0 ; } _rtB -> B_212_18_0 = _rtB -> B_208_6_0 ; _rtB ->
B_212_5_0 = _rtB -> B_208_6_0 ; } srUpdateBC ( _rtDW ->
InertialResponsetodf_SubsysRanBC ) ; break ; case 1 : if ( rtAction !=
rtPrevAction ) { if ( ssGetTaskTime ( S , 4 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } i = ssIsSampleHit ( S
, 4 , 0 ) ; if ( i != 0 ) { rtb_B_311_2980_0 = _rtP -> P_391 * _rtB ->
B_311_2019_0 ; rtb_B_311_2980_0 = ( rtb_B_311_2980_0 - _rtB -> B_311_3068_0 )
/ rtb_B_311_2980_0 ; if ( rtb_B_311_2980_0 > _rtP -> P_393 ) {
rtb_B_311_2980_0 -= _rtP -> P_393 ; } else if ( rtb_B_311_2980_0 >= _rtP ->
P_392 ) { rtb_B_311_2980_0 = 0.0 ; } else { rtb_B_311_2980_0 -= _rtP -> P_392
; } rtb_B_311_35_0 = _rtP -> P_394 * rtb_B_311_2980_0 - _rtB -> B_209_0_0 ;
if ( rtb_B_311_35_0 > _rtP -> P_395 ) { _rtB -> B_212_5_0 = _rtP -> P_395 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_396 ) { _rtB -> B_212_5_0 = _rtP ->
P_396 ; } else { _rtB -> B_212_5_0 = rtb_B_311_35_0 ; } _rtB -> B_212_18_0 =
_rtB -> B_209_1_0 ; _rtB -> B_212_12_0 = _rtB -> B_209_1_0 ; } srUpdateBC (
_rtDW -> PFR_SubsysRanBC ) ; break ; case 2 : if ( rtAction != rtPrevAction )
{ if ( ssGetTaskTime ( S , 4 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } i = ssIsSampleHit ( S
, 4 , 0 ) ; if ( i != 0 ) { _rtB -> B_212_5_0 = _rtB -> B_311_1263_0 [ 0 ] -
_rtB -> B_210_0_0 ; _rtB -> B_212_18_0 = _rtB -> B_311_1263_0 [ 1 ] ; _rtB ->
B_212_12_0 = _rtB -> B_210_1_0 ; } srUpdateBC ( _rtDW -> PQ_SubsysRanBC ) ;
break ; case 3 : if ( rtAction != rtPrevAction ) { if ( ssGetTaskTime ( S , 4
) != ssGetTStart ( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
} } i = ssIsSampleHit ( S , 4 , 0 ) ; if ( i != 0 ) { _rtB -> B_212_18_0 =
_rtB -> B_211_0_0 ; _rtB -> B_212_5_0 = _rtB -> B_211_0_0 ; _rtB ->
B_212_12_0 = _rtB -> B_211_0_0 ; } srUpdateBC ( _rtDW -> Baseline_SubsysRanBC
) ; break ; } if ( ssGetTaskTime ( S , 4 ) > _rtP -> P_403 ) {
rtb_B_311_2980_0 = _rtB -> B_212_5_0 + _rtB -> B_212_4_0 ; } else {
rtb_B_311_2980_0 = _rtB -> B_212_5_0_d ; } if ( rtb_B_311_2980_0 > _rtP ->
P_404 ) { _rtB -> B_212_9_0 = _rtP -> P_404 ; } else if ( rtb_B_311_2980_0 <
_rtP -> P_405 ) { _rtB -> B_212_9_0 = _rtP -> P_405 ; } else { _rtB ->
B_212_9_0 = rtb_B_311_2980_0 ; } ssCallAccelRunBlock ( S , 212 , 10 ,
SS_CALL_MDL_OUTPUTS ) ; { { const char * errMsg = ( NULL ) ; void *
excelLoader = ( void * ) _rtDW -> FromSpreadsheet_PWORK_d . ExcelLoader ;
void * TempDoubleOutput = ( void * ) & _rtDW -> FromSpreadsheet_RWORK_f .
TempDoubleOutput ; if ( excelLoader != ( NULL ) && ( ssIsMajorTimeStep ( S )
|| ! 0 ) ) { void * y = & B_212_11_0 ; real_T t = ssGetTaskTime ( S , 4 ) ;
errMsg = rtwExcelLoaderGetOutput ( & TempDoubleOutput , excelLoader , t ,
ssIsMajorTimeStep ( S ) ) ; if ( errMsg != ( NULL ) ) { ssSetErrorStatus ( S
, errMsg ) ; return ; } B_212_11_0 = _rtDW -> FromSpreadsheet_RWORK_f .
TempDoubleOutput ; } } } if ( _rtP -> P_4279 == 1 ) { _rtB -> B_212_14_0 =
B_212_11_0 ; } else { _rtB -> B_212_14_0 = _rtB -> B_212_12_0 + _rtB ->
B_212_2_0 ; } _rtB -> B_212_15_0 = _rtB -> B_212_14_0 * ( real_T ) _rtB ->
B_311_3066_0 ; ssCallAccelRunBlock ( S , 212 , 16 , SS_CALL_MDL_OUTPUTS ) ;
_rtB -> B_212_17_0 = _rtB -> B_212_9_0 * ( real_T ) _rtB -> B_311_3066_0 ; if
( ssGetTaskTime ( S , 4 ) > _rtP -> P_408 ) { rtb_B_299_3_0 = _rtB ->
B_212_18_0 ; } else { rtb_B_299_3_0 = _rtB -> B_212_3_0 ; } _rtB ->
B_212_21_0 = rtb_B_299_3_0 * ( real_T ) _rtB -> B_311_3066_0 ; } } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { rtb_B_311_2980_0 = _rtDW ->
UnitDelay6_DSTATE ; _rtB -> B_311_3077_0 = _rtP -> P_2538 * _rtDW ->
UnitDelay6_DSTATE ; _rtB -> B_311_3078_0 = _rtDW -> UnitDelay7_DSTATE ;
ssCallAccelRunBlock ( S , 311 , 3079 , SS_CALL_MDL_OUTPUTS ) ;
rtb_B_311_3082_0 = _rtP -> P_2540 [ 0 ] * rtb_B_311_8_0 [ 0 ] * rtb_B_311_5_0
[ 1 ] + _rtP -> P_2540 [ 1 ] * rtb_B_311_8_0 [ 1 ] * rtb_B_311_5_0 [ 0 ] ;
rtb_B_311_3086_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_ni ;
rtb_B_311_3087_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_ni - _rtB ->
B_311_3074_0 ; _rtB -> B_311_3089_0 = _rtP -> P_2544 * _rtDW ->
DiscreteTimeIntegrator1_DSTATE_b + _rtP -> P_2547 * rtb_B_311_3087_0 ;
rtb_B_311_3090_0 = _rtP -> P_2548 * _rtB -> B_311_3089_0 ; _rtB ->
B_311_3091_0 [ 0 ] = _rtP -> P_2541 * rtb_B_311_3082_0 * _rtP -> P_2549 ;
_rtB -> B_311_3091_0 [ 1 ] = _rtP -> P_2549 * rtb_B_311_3090_0 ;
rtb_B_311_3090_0 *= _rtB -> B_311_3074_0 ; _rtB -> B_311_3095_0 [ 0 ] = _rtP
-> P_2551 * rtb_B_311_3090_0 ; _rtB -> B_311_3095_0 [ 1 ] = rtb_B_311_3012_0
* rtb_B_311_3082_0 * _rtP -> P_2550 * _rtP -> P_2551 ; _rtB -> B_311_3096_0 =
_rtDW -> UnitDelay8_DSTATE ; _rtB -> B_311_3097_0 [ 0 ] = _rtP -> P_2553 *
rtb_B_311_2980_0 ; _rtB -> B_311_3097_0 [ 1 ] = _rtP -> P_2553 * _rtB ->
B_311_3078_0 ; } ssCallAccelRunBlock ( S , 311 , 3098 , SS_CALL_MDL_OUTPUTS )
; i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_3100_0 = (
( _rtB -> B_155_5_9 != 0.0 ) && _rtB -> B_311_3066_0 ) ; if ( _rtB ->
B_311_284_0 >= _rtP -> P_2557 ) { rtb_B_311_3103_0 = _rtB -> B_311_3100_0 ; }
else { rtb_B_311_3103_0 = rt_Lookup ( _rtP -> P_2555 , 4 , ssGetTaskTime ( S
, 2 ) , _rtP -> P_2556 ) ; } if ( _rtP -> P_2554 >= _rtP -> P_2558 ) { if (
_rtB -> B_311_285_0_i >= _rtP -> P_570 ) { _rtB -> B_311_3107_0 =
rtb_B_311_3103_0 ; } else { _rtB -> B_311_3107_0 = _rtB -> B_311_288_0 ; } }
else { _rtB -> B_311_3107_0 = rt_Lookup ( _rtP -> P_568 , 5 , ssGetTaskTime (
S , 2 ) , _rtP -> P_569 ) ; } if ( _rtP -> P_2559 >= _rtP -> P_2560 ) { if (
_rtB -> B_311_286_0 >= _rtP -> P_573 ) { _rtB -> B_311_3115_0 =
rtb_B_311_3103_0 ; } else { _rtB -> B_311_3115_0 = _rtB -> B_311_288_0 ; } }
else { _rtB -> B_311_3115_0 = rt_Lookup ( _rtP -> P_571 , 5 , ssGetTaskTime (
S , 2 ) , _rtP -> P_572 ) ; } if ( _rtP -> P_2561 >= _rtP -> P_2562 ) { if (
_rtB -> B_311_287_0 >= _rtP -> P_576 ) { _rtB -> B_311_3123_0 =
rtb_B_311_3103_0 ; } else { _rtB -> B_311_3123_0 = _rtB -> B_311_288_0 ; } }
else { _rtB -> B_311_3123_0 = rt_Lookup ( _rtP -> P_574 , 5 , ssGetTaskTime (
S , 2 ) , _rtP -> P_575 ) ; } rtb_B_311_3103_0 = _rtP -> P_2563 * _rtB ->
B_311_152_0 [ 191 ] * _rtP -> P_2564 ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_cm
!= 0 ) { _rtB -> B_311_3143_0 = _rtDW -> Integ4_DSTATE_m ; } else { _rtB ->
B_311_3143_0 = _rtP -> P_2565 * _rtB -> B_311_1246_0 + _rtDW ->
Integ4_DSTATE_m ; } _rtB -> B_311_3144_0 = _rtP -> P_2567 ;
ssCallAccelRunBlock ( S , 311 , 3145 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_3144_0 ) { _rtB -> B_311_3151_0 = (
_rtB -> B_311_3143_0 - _rtB -> B_311_3145_0 ) * _rtP -> P_2577 + ( _rtP ->
P_351 * _rtB -> B_311_1246_0 - _rtP -> P_352 * _rtDW -> UnitDelay_DSTATE_hq )
; } else { _rtB -> B_311_3151_0 = _rtDW -> UnitDelay1_DSTATE_hy ; }
B_311_1349_0_idx_0 = _rtP -> P_2579 * _rtB -> B_311_152_0 [ 192 ] * _rtP ->
P_2582 ; B_311_1349_0_idx_1 = _rtP -> P_2580 * _rtB -> B_311_152_0 [ 193 ] *
_rtP -> P_2582 ; B_311_1349_0_idx_2 = _rtP -> P_2581 * _rtB -> B_311_152_0 [
194 ] * _rtP -> P_2582 ; rtb_B_311_1335_0 [ 0 ] = _rtP -> P_2583 * _rtB ->
B_311_152_0 [ 109 ] * _rtP -> P_2586 ; rtb_B_311_1335_0 [ 1 ] = _rtP ->
P_2584 * _rtB -> B_311_152_0 [ 110 ] * _rtP -> P_2586 ; rtb_B_311_1335_0 [ 2
] = _rtP -> P_2585 * _rtB -> B_311_152_0 [ 111 ] * _rtP -> P_2586 ;
B_311_1283_0_idx_0 = _rtP -> P_2587 * _rtB -> B_311_152_0 [ 195 ] * _rtP ->
P_2590 ; B_311_1283_0_idx_1 = _rtP -> P_2588 * _rtB -> B_311_152_0 [ 196 ] *
_rtP -> P_2590 ; B_311_1283_0_idx_2 = _rtP -> P_2589 * _rtB -> B_311_152_0 [
197 ] * _rtP -> P_2590 ; rtb_B_311_3231_0 = _rtDW ->
DiscreteTimeIntegrator_DSTATE_m3l ; rtb_B_311_3233_0 = _rtDW ->
Delay_x1_DSTATE_d ; rtb_B_311_3235_0 = _rtDW -> Delay_x2_DSTATE_h ; _rtB ->
B_311_3238_0 = ( _rtP -> P_2597 * _rtDW -> Delay_x1_DSTATE_d + _rtP -> P_2599
* _rtDW -> Delay_x2_DSTATE_h ) + _rtP -> P_2595 * _rtB -> B_311_3074_0 ; if (
_rtB -> B_311_3238_0 >= _rtP -> P_2600 ) { if ( _rtB -> B_311_3238_0 > _rtP
-> P_414 ) { rtb_B_299_3_0 = _rtP -> P_414 ; } else if ( _rtB -> B_311_3238_0
< _rtP -> P_415 ) { rtb_B_299_3_0 = _rtP -> P_415 ; } else { rtb_B_299_3_0 =
_rtB -> B_311_3238_0 ; } rtb_B_311_3240_0 = _rtB -> B_311_293_0_g /
rtb_B_299_3_0 ; } else { rtb_B_311_3240_0 = _rtB -> B_311_292_0_f ; }
rtb_B_311_3251_0 = _rtP -> P_2608 * muDoubleScalarRem ( _rtDW -> theta_DSTATE
, 6.2831853071795862 ) * _rtP -> P_2609 ; rtb_B_311_3253_0 = _rtDW ->
Delay_x1_DSTATE_b ; rtb_B_311_3255_0 = _rtDW -> Delay_x2_DSTATE_f ;
B_311_3260_0 = muDoubleScalarMod ( ( ( _rtP -> P_2612 * _rtDW ->
Delay_x1_DSTATE_b + _rtP -> P_2614 * _rtDW -> Delay_x2_DSTATE_f ) + _rtP ->
P_2610 * rtb_B_311_3251_0 ) * _rtP -> P_2615 , _rtB -> B_311_337_0 ) ;
rtb_B_311_3261_0 = muDoubleScalarSin ( B_311_3260_0 ) ; rtb_B_311_3262_0 =
muDoubleScalarCos ( B_311_3260_0 ) ; B_311_3242_0_idx_0 = _rtDW ->
Delay_x1_DSTATE_c [ 0 ] ; B_311_3244_0_idx_0 = _rtDW -> Delay_x2_DSTATE_i [ 0
] ; B_311_1062_0_idx_0 = ( _rtP -> P_2603 * _rtDW -> Delay_x1_DSTATE_c [ 0 ]
+ _rtP -> P_2605 * _rtDW -> Delay_x2_DSTATE_i [ 0 ] ) + _rtP -> P_2601 *
rtb_B_311_1335_0 [ 0 ] ; B_311_3265_0_idx_0 = _rtDW -> Delay_x1_DSTATE_i [ 0
] ; B_311_3267_0_idx_0 = _rtDW -> Delay_x2_DSTATE_fo [ 0 ] ; rtb_B_311_3335_0
= ( _rtP -> P_2618 * _rtDW -> Delay_x1_DSTATE_i [ 0 ] + _rtP -> P_2620 *
_rtDW -> Delay_x2_DSTATE_fo [ 0 ] ) + _rtP -> P_2616 * B_311_1349_0_idx_0 ;
B_311_3242_0_idx_1 = _rtDW -> Delay_x1_DSTATE_c [ 1 ] ; B_311_3244_0_idx_1 =
_rtDW -> Delay_x2_DSTATE_i [ 1 ] ; B_311_1062_0_idx_1 = ( _rtP -> P_2603 *
_rtDW -> Delay_x1_DSTATE_c [ 1 ] + _rtP -> P_2605 * _rtDW ->
Delay_x2_DSTATE_i [ 1 ] ) + _rtP -> P_2601 * rtb_B_311_1335_0 [ 1 ] ;
B_311_3265_0_idx_1 = _rtDW -> Delay_x1_DSTATE_i [ 1 ] ; B_311_3267_0_idx_1 =
_rtDW -> Delay_x2_DSTATE_fo [ 1 ] ; rtb_B_311_3278_0 = ( _rtP -> P_2618 *
_rtDW -> Delay_x1_DSTATE_i [ 1 ] + _rtP -> P_2620 * _rtDW ->
Delay_x2_DSTATE_fo [ 1 ] ) + _rtP -> P_2616 * B_311_1349_0_idx_1 ;
B_311_3242_0_idx_2 = _rtDW -> Delay_x1_DSTATE_c [ 2 ] ; B_311_3244_0_idx_2 =
_rtDW -> Delay_x2_DSTATE_i [ 2 ] ; B_311_3265_0_idx_2 = _rtDW ->
Delay_x1_DSTATE_i [ 2 ] ; B_311_3267_0_idx_2 = _rtDW -> Delay_x2_DSTATE_fo [
2 ] ; rtb_B_311_3279_0 = ( _rtP -> P_2618 * _rtDW -> Delay_x1_DSTATE_i [ 2 ]
+ _rtP -> P_2620 * _rtDW -> Delay_x2_DSTATE_fo [ 2 ] ) + _rtP -> P_2616 *
B_311_1349_0_idx_2 ; for ( i = 0 ; i < 3 ; i ++ ) { B_311_3272_0 [ i ] = _rtP
-> P_2622 * ( _rtP -> P_2621 [ i + 6 ] * rtb_B_311_3279_0 + ( _rtP -> P_2621
[ i + 3 ] * rtb_B_311_3278_0 + _rtP -> P_2621 [ i ] * rtb_B_311_3335_0 ) ) ;
} Auto_microgrid_forecasting_020822_Subsystem1_h ( S , _rtB -> B_311_399_0 ,
& B_311_3272_0 [ 0 ] , B_311_3260_0 , & _rtB -> Subsystem1_n , & _rtDW ->
Subsystem1_n ) ; Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( S ,
_rtB -> B_311_401_0 , & B_311_3272_0 [ 0 ] , B_311_3260_0 , & _rtB ->
Subsystempi2delay_e , & _rtDW -> Subsystempi2delay_e ) ; if ( _rtB ->
B_311_399_0 != 0 ) { B_311_1965_0_idx_0 = _rtB -> Subsystem1_n . B_241_0_0 ;
B_311_1965_0_idx_1 = _rtB -> Subsystem1_n . B_241_1_0 ; } else {
B_311_1965_0_idx_0 = _rtB -> Subsystempi2delay_e . B_240_0_0 ;
B_311_1965_0_idx_1 = _rtB -> Subsystempi2delay_e . B_240_1_0 ; } if ( _rtB ->
B_311_3238_0 > _rtP -> P_2623 ) { rtb_B_311_3278_0 = _rtP -> P_2623 ; } else
if ( _rtB -> B_311_3238_0 < _rtP -> P_2624 ) { rtb_B_311_3278_0 = _rtP ->
P_2624 ; } else { rtb_B_311_3278_0 = _rtB -> B_311_3238_0 ; }
rtb_B_311_3279_0 = ( ( ( 2.0 * B_311_1062_0_idx_0 + B_311_1062_0_idx_1 ) *
rtb_B_311_3262_0 + 1.7320508075688772 * B_311_1062_0_idx_1 * rtb_B_311_3261_0
) / 3.0 - B_311_1965_0_idx_1 * _rtB -> B_311_385_0 ) / rtb_B_311_3278_0 ;
rtb_B_311_3261_0 = ( ( ( 2.0 * B_311_1062_0_idx_0 + B_311_1062_0_idx_1 ) *
rtb_B_311_3261_0 + - 1.7320508075688772 * B_311_1062_0_idx_1 *
rtb_B_311_3262_0 ) / 3.0 - B_311_1965_0_idx_0 * _rtB -> B_311_386_0_l ) / (
_rtP -> P_2625 * rtb_B_311_3278_0 ) ; rtb_B_311_68_0 = muDoubleScalarPower (
rtb_B_311_3279_0 , 2.0 ) + muDoubleScalarPower ( rtb_B_311_3261_0 , 2.0 ) ;
if ( rtb_B_311_68_0 < 0.0 ) { rtb_B_311_3279_0 = - muDoubleScalarSqrt ( -
rtb_B_311_68_0 ) ; } else { rtb_B_311_3279_0 = muDoubleScalarSqrt (
rtb_B_311_68_0 ) ; } rtb_B_311_3261_0 = _rtDW -> Delay_x_DSTATE ;
rtb_B_311_3240_0 -= _rtP -> P_2626 * rtb_B_311_3279_0 + _rtP -> P_2628 *
_rtDW -> Delay_x_DSTATE ; _rtB -> B_311_3292_0 = _rtP -> P_2630 *
rtb_B_311_3240_0 ; rtb_B_311_35_0 = _rtP -> P_2629 * rtb_B_311_3240_0 +
rtb_B_311_3231_0 ; if ( rtb_B_311_35_0 > _rtP -> P_2631 ) { _rtB ->
B_311_3294_0 = _rtP -> P_2631 ; } else if ( rtb_B_311_35_0 < _rtP -> P_2632 )
{ _rtB -> B_311_3294_0 = _rtP -> P_2632 ; } else { _rtB -> B_311_3294_0 =
rtb_B_311_35_0 ; } ssCallAccelRunBlock ( S , 311 , 3295 , SS_CALL_MDL_OUTPUTS
) ; _rtB -> B_311_3296_0 = _rtP -> P_2633 ; ssCallAccelRunBlock ( S , 311 ,
3297 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_311_3298_0 = _rtDW ->
Delay_DSTATE_dv ; _rtB -> B_311_3299_0 = _rtDW -> Delay1_DSTATE ;
ssCallAccelRunBlock ( S , 311 , 3300 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 3301 , SS_CALL_MDL_OUTPUTS ) ;
B_311_3303_0_idx_0 = _rtDW -> Delay_x1_DSTATE_l [ 0 ] ; B_311_3305_0_idx_0 =
_rtDW -> Delay_x2_DSTATE_o [ 0 ] ; rtb_B_311_3271_0 [ 0 ] = ( _rtP -> P_2638
* _rtDW -> Delay_x1_DSTATE_l [ 0 ] + _rtP -> P_2640 * _rtDW ->
Delay_x2_DSTATE_o [ 0 ] ) + _rtP -> P_2636 * B_311_1283_0_idx_0 ;
B_311_3303_0_idx_1 = _rtDW -> Delay_x1_DSTATE_l [ 1 ] ; B_311_3305_0_idx_1 =
_rtDW -> Delay_x2_DSTATE_o [ 1 ] ; rtb_B_311_3271_0 [ 1 ] = ( _rtP -> P_2638
* _rtDW -> Delay_x1_DSTATE_l [ 1 ] + _rtP -> P_2640 * _rtDW ->
Delay_x2_DSTATE_o [ 1 ] ) + _rtP -> P_2636 * B_311_1283_0_idx_1 ;
B_311_3303_0_idx_2 = _rtDW -> Delay_x1_DSTATE_l [ 2 ] ; B_311_3305_0_idx_2 =
_rtDW -> Delay_x2_DSTATE_o [ 2 ] ; rtb_B_311_3271_0 [ 2 ] = ( _rtP -> P_2638
* _rtDW -> Delay_x1_DSTATE_l [ 2 ] + _rtP -> P_2640 * _rtDW ->
Delay_x2_DSTATE_o [ 2 ] ) + _rtP -> P_2636 * B_311_1283_0_idx_2 ; for ( i = 0
; i < 3 ; i ++ ) { B_311_3310_0 [ i ] = _rtP -> P_2642 * ( _rtP -> P_2641 [ i
+ 6 ] * rtb_B_311_3271_0 [ 2 ] + ( _rtP -> P_2641 [ i + 3 ] *
rtb_B_311_3271_0 [ 1 ] + _rtP -> P_2641 [ i ] * rtb_B_311_3271_0 [ 0 ] ) ) ;
} _rtB -> B_311_3312_0 = ( _rtDW -> DiscreteTimeIntegrator_DSTATE_nt > _rtP
-> P_2643 ) ; if ( _rtDW -> Initial_FirstOutputTime_k1 ) { _rtDW ->
Initial_FirstOutputTime_k1 = false ; _rtB -> B_311_3314_0 = _rtP -> P_2644 ;
} else { _rtB -> B_311_3314_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_nt -
_rtP -> P_2643 ; } if ( _rtB -> B_311_3312_0 && ( _rtDW ->
DiscreteTimeIntegrator_PrevResetState_nb <= 0 ) ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_nt = _rtB -> B_311_3314_0 ; } B_311_3315_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_nt ;
Auto_microgrid_forecasting_020822_Subsystem1_h ( S , _rtB -> B_311_394_0_i ,
& B_311_3310_0 [ 0 ] , B_311_3315_0 , & _rtB -> Subsystem1_c , & _rtDW ->
Subsystem1_c ) ; Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( S ,
_rtB -> B_311_396_0 , & B_311_3310_0 [ 0 ] , B_311_3315_0 , & _rtB ->
Subsystempi2delay_oq , & _rtDW -> Subsystempi2delay_oq ) ; if ( _rtB ->
B_311_394_0_i != 0 ) { _rtB -> B_311_3318_0 [ 0 ] = _rtB -> Subsystem1_c .
B_241_0_0 ; _rtB -> B_311_3318_0 [ 1 ] = _rtB -> Subsystem1_c . B_241_1_0 ; }
else { _rtB -> B_311_3318_0 [ 0 ] = _rtB -> Subsystempi2delay_oq . B_240_0_0
; _rtB -> B_311_3318_0 [ 1 ] = _rtB -> Subsystempi2delay_oq . B_240_1_0 ; }
_rtB -> B_311_3321_0 = _rtDW -> Delay2_DSTATE * ( real_T ) _rtB ->
B_311_3066_0 * _rtP -> P_2647 ; _rtB -> B_311_3324_0 = ( real_T ) _rtB ->
B_311_3066_0 * _rtDW -> Delay3_DSTATE * _rtP -> P_2649 ; ssCallAccelRunBlock
( S , 311 , 3325 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_311_3326_0 = _rtP ->
P_2650 * _rtDW -> DiscreteTransferFcn3_states ; rtb_B_311_3231_0 = _rtDW ->
Delay_x1_DSTATE_m ; rtb_B_311_3240_0 = _rtDW -> Delay_x2_DSTATE_n ; _rtB ->
B_311_3333_0 = ( _rtP -> P_2655 * _rtDW -> Delay_x1_DSTATE_m + _rtP -> P_2657
* _rtDW -> Delay_x2_DSTATE_n ) + _rtP -> P_2653 * _rtB -> B_212_21_0 ; if (
_rtDW -> DiscreteTransferFcn1_icLoad != 0 ) { _rtDW ->
DiscreteTransferFcn1_states = _rtB -> B_311_3333_0 ; _rtDW ->
DiscreteTransferFcn1_icLoad = 0U ; } rtb_B_311_3335_0 = _rtP -> P_2660 *
_rtDW -> DiscreteTransferFcn4_states ; rtb_B_311_3262_0 = _rtDW ->
Delay_x1_DSTATE_cl ; rtb_B_311_3278_0 = _rtDW -> Delay_x2_DSTATE_hg ;
rtb_B_311_3347_0 = ( _rtP -> P_2658 * _rtDW -> DiscreteTransferFcn1_states -
( ( _rtP -> P_2665 * _rtDW -> Delay_x1_DSTATE_cl + _rtP -> P_2667 * _rtDW ->
Delay_x2_DSTATE_hg ) + _rtP -> P_2663 * rtb_B_311_3335_0 ) ) * _rtP -> P_2668
* ( real_T ) ! _rtB -> B_311_2984_0 * ( real_T ) _rtB -> B_311_3066_0 ;
rtb_B_311_3348_0 = _rtP -> P_2669 * rtb_B_311_3347_0 ; rtb_B_311_3349_0 =
_rtDW -> DiscreteIntegrator_DSTATE + rtb_B_311_3348_0 ; rtb_B_311_3334_0 =
_rtP -> P_2670 * _rtB -> B_311_3299_0 ; if ( ( ( rtb_B_311_3349_0 >= _rtB ->
B_311_3299_0 ) && ( rtb_B_311_3347_0 > Auto_microgrid_forecasting_020822_rtC
( S ) -> B_311_7_0 ) ) || ( ( rtb_B_311_3349_0 <= rtb_B_311_3334_0 ) && (
rtb_B_311_3347_0 < Auto_microgrid_forecasting_020822_rtC ( S ) -> B_311_8_0 )
) ) { _rtB -> B_311_3359_0 = _rtB -> B_311_325_0 ; } else { _rtB ->
B_311_3359_0 = _rtP -> P_2671 * rtb_B_311_3347_0 ; } rtb_B_311_44_0 = ( _rtDW
-> DiscreteIntegrator_DSTATE >= _rtB -> B_311_3299_0 ) ; rtb_B_311_69_0 = (
_rtDW -> DiscreteIntegrator_DSTATE <= rtb_B_311_3334_0 ) ; rtb_B_311_3349_0 =
_rtP -> P_2673 * _rtP -> P_2672 ; _rtB -> B_311_3364_0 = ( _rtB ->
B_311_2982_0 && _rtB -> B_311_2984_0 ) ; if ( _rtB -> B_311_3364_0 || ( _rtDW
-> DiscreteTimeIntegrator3_PrevResetState_l != 0 ) ) { _rtDW ->
DiscreteTimeIntegrator3_DSTATE_c = _rtP -> P_2675 ; } if ( _rtDW ->
DiscreteTimeIntegrator3_DSTATE_c >= _rtP -> P_2676 ) { _rtDW ->
DiscreteTimeIntegrator3_DSTATE_c = _rtP -> P_2676 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator3_DSTATE_c <= _rtP -> P_2677 ) { _rtDW ->
DiscreteTimeIntegrator3_DSTATE_c = _rtP -> P_2677 ; } } rtb_B_311_3366_0 = (
_rtDW -> DiscreteTimeIntegrator3_DSTATE_c > _rtB -> B_311_297_0 ) ;
rtb_B_311_3347_0 = ( real_T ) rtb_B_311_3366_0 - _rtDW -> UnitDelay_DSTATE_dh
; if ( ! ( rtb_B_311_3347_0 > rtb_B_311_3349_0 ) ) { rtb_B_311_3349_0 = _rtP
-> P_424 * _rtP -> P_2679 ; if ( ! ( rtb_B_311_3347_0 < rtb_B_311_3349_0 ) )
{ rtb_B_311_3349_0 = rtb_B_311_3347_0 ; } } _rtB -> B_311_3374_0 =
rtb_B_311_3349_0 + _rtDW -> UnitDelay_DSTATE_dh ; rtb_B_311_3375_0 = ( _rtB
-> B_311_3374_0 < _rtB -> B_311_299_0 ) ; _rtB -> B_311_3376_0 = (
rtb_B_311_44_0 || rtb_B_311_69_0 || rtb_B_311_3375_0 ) ; rtb_B_311_3377_0 = !
rtb_B_311_3375_0 ; _rtB -> B_311_3380_0 = _rtDW -> Delay_DSTATE_io ;
rtb_B_311_3347_0 = _rtP -> P_2681 * _rtB -> B_311_3380_0 - _rtDW ->
Delay1_DSTATE_b ; if ( _rtDW -> IC_FirstOutputTime ) { _rtDW ->
IC_FirstOutputTime = false ; _rtB -> B_311_3386_0 = _rtP -> P_2683 ; } else {
_rtB -> B_311_3386_0 = ( _rtB -> B_311_3299_0 * ( real_T ) rtb_B_311_44_0 * (
real_T ) rtb_B_311_3377_0 + ( rtb_B_311_3377_0 ? ( real_T ) rtb_B_311_69_0 :
0.0 ) * rtb_B_311_3334_0 ) + rtb_B_311_3347_0 * ( real_T ) rtb_B_311_3375_0 ;
} if ( _rtB -> B_311_3376_0 ) { _rtDW -> DiscreteIntegrator_DSTATE = _rtB ->
B_311_3386_0 ; } _rtB -> B_311_3387_0 = _rtP -> P_2684 * _rtB -> B_311_3359_0
+ _rtDW -> DiscreteIntegrator_DSTATE ; rtb_B_311_3348_0 += _rtB ->
B_311_3387_0 ; if ( rtb_B_311_3348_0 > _rtB -> B_311_3299_0 ) { _rtB ->
B_311_3391_0 = _rtB -> B_311_3299_0 ; } else if ( rtb_B_311_3348_0 <
rtb_B_311_3334_0 ) { _rtB -> B_311_3391_0 = rtb_B_311_3334_0 ; } else { _rtB
-> B_311_3391_0 = rtb_B_311_3348_0 ; } ssCallAccelRunBlock ( S , 311 , 3392 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_311_3393_0 = ( _rtB -> B_311_2981_0 && _rtB
-> B_311_2984_0 ) ; B_311_3394_0 = _rtDW -> UnitDelay_DSTATE_le ;
Auto_microgrid_forecasting_020822_POSITIVEEdge ( S , _rtB -> B_311_322_0_c [
0 ] , _rtB -> B_311_3393_0 , B_311_3394_0 , & _rtB -> POSITIVEEdge_eg , &
_rtDW -> POSITIVEEdge_eg ) ; Auto_microgrid_forecasting_020822_NEGATIVEEdge (
S , _rtB -> B_311_322_0_c [ 1 ] , _rtB -> B_311_3393_0 , B_311_3394_0 , &
_rtB -> NEGATIVEEdge_m , & _rtDW -> NEGATIVEEdge_m ) ; _rtB -> B_311_3397_0 =
( _rtB -> POSITIVEEdge_eg . B_221_0_0 || _rtB -> NEGATIVEEdge_m . B_220_0_0 )
; rtb_B_311_3348_0 = _rtP -> P_2686 * _rtP -> P_2685 ; rtb_B_311_3334_0 = (
real_T ) rtb_B_311_3366_0 - _rtDW -> UnitDelay_DSTATE_i ; if ( ! (
rtb_B_311_3334_0 > rtb_B_311_3348_0 ) ) { rtb_B_311_3348_0 = _rtP -> P_423 *
_rtP -> P_2688 ; if ( ! ( rtb_B_311_3334_0 < rtb_B_311_3348_0 ) ) {
rtb_B_311_3348_0 = rtb_B_311_3334_0 ; } } _rtB -> B_311_3406_0 =
rtb_B_311_3348_0 + _rtDW -> UnitDelay_DSTATE_i ; rtb_B_311_3408_0 = _rtP ->
P_2689 * _rtDW -> DiscreteTransferFcn2_states ; _rtDW -> Relay3_Mode = ( (
rtb_B_311_3408_0 >= _rtP -> P_2692 ) || ( ( ! ( rtb_B_311_3408_0 <= _rtP ->
P_2693 ) ) && _rtDW -> Relay3_Mode ) ) ; if ( _rtDW -> Relay3_Mode ) {
rtb_B_299_3_0 = _rtP -> P_2694 ; } else { rtb_B_299_3_0 = _rtP -> P_2695 ; }
_rtB -> B_311_3411_0 = ( ( ( _rtB -> B_311_3406_0 < _rtB -> B_311_298_0 ) &&
( rtb_B_299_3_0 != 0.0 ) ) || _rtB -> B_311_3364_0 ) ; ssCallAccelRunBlock (
S , 311 , 3412 , SS_CALL_MDL_OUTPUTS ) ; rtb_B_311_3349_0 = (
rtb_B_311_3408_0 - _rtP -> P_2696 ) * _rtP -> P_2697 ; for ( i = 0 ; i < 3 ;
i ++ ) { B_311_3417_0 [ i ] = _rtP -> P_2699 * ( _rtP -> P_2698 [ i + 6 ] *
rtb_B_311_2877_0 [ 2 ] + ( _rtP -> P_2698 [ i + 3 ] * rtb_B_311_2877_0 [ 1 ]
+ _rtP -> P_2698 [ i ] * rtb_B_311_2877_0 [ 0 ] ) ) ; }
Auto_microgrid_forecasting_020822_Subsystem1_h ( S , _rtB -> B_311_389_0 , &
B_311_3417_0 [ 0 ] , B_311_3315_0 , & _rtB -> Subsystem1_l , & _rtDW ->
Subsystem1_l ) ; Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( S ,
_rtB -> B_311_391_0 , & B_311_3417_0 [ 0 ] , B_311_3315_0 , & _rtB ->
Subsystempi2delay_oi , & _rtDW -> Subsystempi2delay_oi ) ; if ( _rtB ->
B_311_389_0 != 0 ) { B_311_1965_0_idx_0 = _rtB -> Subsystem1_l . B_241_0_0 ;
B_311_1965_0_idx_1 = _rtB -> Subsystem1_l . B_241_1_0 ; } else {
B_311_1965_0_idx_0 = _rtB -> Subsystempi2delay_oi . B_240_0_0 ;
B_311_1965_0_idx_1 = _rtB -> Subsystempi2delay_oi . B_240_1_0 ; }
rtb_B_311_3409_0 = _rtDW -> UnitDelay_DSTATE_fb ; rtb_B_311_3408_0 = _rtDW ->
UnitDelay_DSTATE_fb / _rtB -> B_311_377_0_h ; rtb_B_311_3426_0 = _rtB ->
B_311_3321_0 - _rtB -> B_311_3318_0 [ 0 ] ; rtb_B_311_3427_0 = _rtB ->
B_311_3324_0 - _rtB -> B_311_3318_0 [ 1 ] ; rtb_B_311_35_0 = _rtP -> P_2701 *
rtb_B_311_3426_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_nh [ 0 ] ;
rtb_B_311_47_0 = _rtP -> P_2701 * rtb_B_311_3427_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_nh [ 1 ] ; if ( rtb_B_311_35_0 > _rtP -> P_2706
) { rtb_B_311_35_0 = _rtP -> P_2706 ; } else { if ( rtb_B_311_35_0 < _rtP ->
P_2707 ) { rtb_B_311_35_0 = _rtP -> P_2707 ; } } rtb_B_311_3432_0 = ( ( _rtB
-> B_311_305_0_m * rtb_B_311_3408_0 * _rtB -> B_311_3324_0 +
B_311_1965_0_idx_0 ) - _rtB -> B_311_301_0 * _rtB -> B_311_3321_0 ) -
rtb_B_311_35_0 ; if ( rtb_B_311_47_0 > _rtP -> P_2706 ) { rtb_B_311_47_0 =
_rtP -> P_2706 ; } else { if ( rtb_B_311_47_0 < _rtP -> P_2707 ) {
rtb_B_311_47_0 = _rtP -> P_2707 ; } } rtb_B_311_3436_0 = ( (
B_311_1965_0_idx_1 - _rtB -> B_311_3324_0 * _rtB -> B_311_303_0 ) - _rtB ->
B_311_3321_0 * _rtB -> B_311_306_0_m * rtb_B_311_3408_0 ) - rtb_B_311_47_0 ;
rtb_B_311_3408_0 = _rtDW -> Delay_x1_DSTATE_h ; rtb_B_311_3334_0 = _rtDW ->
Delay_x2_DSTATE_d ; _rtB -> B_311_3444_0 = ( _rtP -> P_2710 * _rtDW ->
Delay_x1_DSTATE_h + _rtP -> P_2712 * _rtDW -> Delay_x2_DSTATE_d ) + _rtP ->
P_2708 * rtb_B_299_1_0 ; if ( _rtB -> B_311_3444_0 > _rtP -> P_2713 ) {
rtb_B_299_3_0 = _rtP -> P_2713 ; } else if ( _rtB -> B_311_3444_0 < _rtP ->
P_2714 ) { rtb_B_299_3_0 = _rtP -> P_2714 ; } else { rtb_B_299_3_0 = _rtB ->
B_311_3444_0 ; } rtb_B_311_3348_0 = muDoubleScalarHypot ( rtb_B_311_3432_0 ,
rtb_B_311_3436_0 ) * _rtB -> B_311_323_0_f / rtb_B_299_3_0 ; if (
rtb_B_311_3348_0 > _rtP -> P_2715 ) { rtb_B_311_3348_0 = _rtP -> P_2715 ; }
else { if ( rtb_B_311_3348_0 < _rtP -> P_2716 ) { rtb_B_311_3348_0 = _rtP ->
P_2716 ; } } } i = ssIsSampleHit ( S , 5 , 0 ) ; if ( i != 0 ) { _rtB ->
B_311_3448_0 = ( _rtB -> B_311_2981_0 && _rtB -> B_311_2977_0 ) ; } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { rtb_B_311_35_0 = _rtB ->
B_311_3296_0 - _rtB -> B_311_2972_0 ; if ( rtb_B_311_35_0 > _rtP -> P_2717 )
{ rtb_B_311_35_0 = _rtP -> P_2717 ; } else { if ( rtb_B_311_35_0 < _rtP ->
P_2718 ) { rtb_B_311_35_0 = _rtP -> P_2718 ; } } rtb_B_311_3453_0 = _rtP ->
P_2719 * rtb_B_311_35_0 * _rtP -> P_2720 * ( real_T ) _rtB -> B_311_3448_0 ;
rtb_B_311_35_0 = _rtP -> P_2721 - _rtB -> B_311_2972_0 ; if ( rtb_B_311_35_0
> _rtP -> P_2722 ) { rtb_B_311_35_0 = _rtP -> P_2722 ; } else { if (
rtb_B_311_35_0 < _rtP -> P_2723 ) { rtb_B_311_35_0 = _rtP -> P_2723 ; } }
rtb_B_311_3458_0 = _rtP -> P_2724 * rtb_B_311_35_0 * _rtP -> P_2725 ; } i =
ssIsSampleHit ( S , 5 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_3459_0 = ( _rtB
-> B_311_2976_0 && _rtB -> B_311_2981_0 ) ; } i = ssIsSampleHit ( S , 2 , 0 )
; if ( i != 0 ) { _rtB -> B_311_3461_0 = rtb_B_311_3458_0 * ( real_T ) _rtB
-> B_311_3459_0 + rtb_B_311_3453_0 ; rtb_B_311_3436_0 = muDoubleScalarAtan2 (
rtb_B_311_3436_0 , rtb_B_311_3432_0 ) ; rtb_B_311_3463_0 = _rtP -> P_2726 ;
rtb_B_311_3464_0 = _rtP -> P_2727 ; rtb_B_311_3465_0 = _rtP -> P_2728 ;
rtb_B_311_2980_0 = _rtP -> P_2729 ; rtb_B_311_3458_0 = _rtP -> P_2730 ;
rtb_B_311_3453_0 = _rtP -> P_2731 ; rtb_B_311_3469_0 = _rtDW ->
Delayd1_DSTATE [ 0 ] ; rtb_B_311_3432_0 = _rtDW -> Delayq1_DSTATE [ 0 ] ;
_rtB -> B_311_3471_0 [ 0 ] = _rtP -> P_2734 * rtb_B_311_3426_0 ; _rtB ->
B_311_3471_0 [ 1 ] = _rtP -> P_2734 * rtb_B_311_3427_0 ; B_311_3472_0 = _rtDW
-> UnitDelay_DSTATE_kf ; Auto_microgrid_forecasting_020822_POSITIVEEdge ( S ,
_rtB -> B_311_312_0 [ 0 ] , _rtB -> B_311_3364_0 , B_311_3472_0 , & _rtB ->
POSITIVEEdge , & _rtDW -> POSITIVEEdge ) ;
Auto_microgrid_forecasting_020822_NEGATIVEEdge ( S , _rtB -> B_311_312_0 [ 1
] , _rtB -> B_311_3364_0 , B_311_3472_0 , & _rtB -> NEGATIVEEdge , & _rtDW ->
NEGATIVEEdge ) ; rtb_B_311_44_0 = ( _rtB -> POSITIVEEdge . B_221_0_0 || _rtB
-> NEGATIVEEdge . B_220_0_0 ) ; B_311_3476_0 = _rtDW -> UnitDelay_DSTATE_lv ;
Auto_microgrid_forecasting_020822_POSITIVEEdge ( S , _rtB -> B_311_317_0 [ 0
] , _rtB -> B_311_3393_0 , B_311_3476_0 , & _rtB -> POSITIVEEdge_e , & _rtDW
-> POSITIVEEdge_e ) ; Auto_microgrid_forecasting_020822_NEGATIVEEdge ( S ,
_rtB -> B_311_317_0 [ 1 ] , _rtB -> B_311_3393_0 , B_311_3476_0 , & _rtB ->
NEGATIVEEdge_p , & _rtDW -> NEGATIVEEdge_p ) ; _rtB -> B_311_3480_0 = ( _rtB
-> B_311_3397_0 || _rtB -> B_311_3411_0 ) ; _rtB -> B_311_3481_0 = _rtB ->
B_311_3391_0 + _rtB -> B_311_3326_0 ; rtb_B_311_3349_0 *= ( real_T ) _rtB ->
B_311_3066_0 ; rtb_B_311_3426_0 = _rtP -> P_2735 * rtb_B_311_3349_0 ;
rtb_B_311_3427_0 = _rtDW -> DiscreteIntegrator_DSTATE_l + rtb_B_311_3426_0 ;
if ( ( ( rtb_B_311_3427_0 >= _rtB -> B_311_3298_0 ) && ( rtb_B_311_3349_0 >
Auto_microgrid_forecasting_020822_rtC ( S ) -> B_311_5_0 ) ) || ( (
rtb_B_311_3427_0 <= rtb_B_311_2980_0 ) && ( rtb_B_311_3349_0 <
Auto_microgrid_forecasting_020822_rtC ( S ) -> B_311_6_0 ) ) ) { _rtB ->
B_311_3493_0 = _rtB -> B_311_324_0_p ; } else { _rtB -> B_311_3493_0 = _rtP
-> P_2736 * rtb_B_311_3349_0 ; } rtb_B_311_69_0 = ( _rtDW ->
DiscreteIntegrator_DSTATE_l >= _rtB -> B_311_3298_0 ) ; rtb_B_311_3366_0 = (
_rtDW -> DiscreteIntegrator_DSTATE_l <= rtb_B_311_2980_0 ) ; _rtB ->
B_311_3496_0 = ( rtb_B_311_69_0 || rtb_B_311_3366_0 || _rtB -> B_311_3480_0 )
; rtb_B_311_3377_0 = ! _rtB -> B_311_3480_0 ; _rtB -> B_311_3500_0 = _rtDW ->
Delay_DSTATE_fp ; rtb_B_311_3349_0 = _rtP -> P_2738 * _rtB -> B_311_3500_0 -
_rtDW -> Delay1_DSTATE_a ; if ( _rtB -> POSITIVEEdge_e . B_221_0_0 || _rtB ->
NEGATIVEEdge_p . B_220_0_0 ) { _rtB -> B_311_3505_0 = rtb_B_311_3469_0 ; }
else { _rtB -> B_311_3505_0 = _rtDW -> ICic_DSTATE ; } if ( _rtDW ->
IC_FirstOutputTime_n ) { _rtDW -> IC_FirstOutputTime_n = false ; _rtB ->
B_311_3509_0 = _rtP -> P_2742 ; } else { if ( _rtB -> B_311_1262_0 > _rtP ->
P_2741 ) { rtb_B_299_3_0 = rtb_B_311_3349_0 ; } else { rtb_B_299_3_0 = _rtB
-> B_311_3505_0 ; } _rtB -> B_311_3509_0 = ( _rtB -> B_311_3298_0 * ( real_T
) rtb_B_311_69_0 * ( real_T ) rtb_B_311_3377_0 + ( rtb_B_311_3377_0 ? (
real_T ) rtb_B_311_3366_0 : 0.0 ) * rtb_B_311_2980_0 ) + rtb_B_299_3_0 * (
real_T ) _rtB -> B_311_3480_0 ; } if ( _rtB -> B_311_3496_0 ) { _rtDW ->
DiscreteIntegrator_DSTATE_l = _rtB -> B_311_3509_0 ; } _rtB -> B_311_3510_0 =
_rtP -> P_2743 * _rtB -> B_311_3493_0 + _rtDW -> DiscreteIntegrator_DSTATE_l
; rtb_B_311_3426_0 += _rtB -> B_311_3510_0 ; if ( rtb_B_311_3426_0 > _rtB ->
B_311_3298_0 ) { _rtB -> B_311_3514_0 = _rtB -> B_311_3298_0 ; } else if (
rtb_B_311_3426_0 < rtb_B_311_2980_0 ) { _rtB -> B_311_3514_0 =
rtb_B_311_2980_0 ; } else { _rtB -> B_311_3514_0 = rtb_B_311_3426_0 ; }
ssCallAccelRunBlock ( S , 311 , 3515 , SS_CALL_MDL_OUTPUTS ) ;
rtb_B_311_3426_0 = rtb_B_311_3348_0 * muDoubleScalarCos ( rtb_B_311_3436_0 )
; rtb_B_311_3348_0 *= muDoubleScalarSin ( rtb_B_311_3436_0 ) ; if (
rtb_B_311_44_0 ) { _rtB -> B_311_3519_0 = rtb_B_311_3469_0 ; _rtB ->
B_311_3522_0 = rtb_B_311_3432_0 ; } else { _rtB -> B_311_3519_0 = _rtDW ->
ICic_DSTATE_e ; _rtB -> B_311_3522_0 = _rtDW -> ICic_DSTATE_m ; } _rtB ->
B_311_3520_0 = _rtB -> B_311_3519_0 * _rtB -> B_311_3406_0 ; _rtB ->
B_311_3523_0 = _rtB -> B_311_3522_0 * _rtB -> B_311_3374_0 ; if ( _rtB ->
B_311_3514_0 > _rtB -> B_311_3298_0 ) { _rtB -> B_311_3526_0 = _rtB ->
B_311_3298_0 ; } else if ( _rtB -> B_311_3514_0 < rtb_B_311_3458_0 ) { _rtB
-> B_311_3526_0 = rtb_B_311_3458_0 ; } else { _rtB -> B_311_3526_0 = _rtB ->
B_311_3514_0 ; } if ( _rtB -> B_311_3481_0 > _rtB -> B_311_3299_0 ) { _rtB ->
B_311_3529_0 = _rtB -> B_311_3299_0 ; } else { rtb_B_311_3458_0 = _rtP ->
P_425 * _rtB -> B_311_3299_0 ; if ( _rtB -> B_311_3481_0 < rtb_B_311_3458_0 )
{ _rtB -> B_311_3529_0 = rtb_B_311_3458_0 ; } else { _rtB -> B_311_3529_0 =
_rtB -> B_311_3481_0 ; } } _rtB -> B_311_3537_0 = ( _rtP -> P_2746 *
rtb_B_311_3262_0 + _rtP -> P_2747 * rtb_B_311_3278_0 ) + _rtP -> P_2750 *
rtb_B_311_3335_0 ; _rtB -> B_311_3539_0 = ( _rtP -> P_2748 * rtb_B_311_3262_0
+ _rtP -> P_2749 * rtb_B_311_3278_0 ) + _rtP -> P_2751 * rtb_B_311_3335_0 ;
if ( _rtB -> B_311_3411_0 ) { _rtB -> B_311_3541_0 = rtb_B_311_3349_0 ; }
else { if ( _rtB -> B_311_3393_0 ) { rtb_B_311_3335_0 = rtb_B_311_3453_0 *
rtb_B_311_3453_0 - _rtB -> B_311_3481_0 * _rtB -> B_311_3481_0 ; if (
rtb_B_311_3335_0 > _rtP -> P_418 ) { rtb_B_311_3335_0 = _rtP -> P_418 ; }
else { if ( rtb_B_311_3335_0 < _rtP -> P_419 ) { rtb_B_311_3335_0 = _rtP ->
P_419 ; } } if ( ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtDW -> Sqrt1_DWORK1
!= 0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW ->
Sqrt1_DWORK1 = 0 ; } rtb_B_311_3464_0 = muDoubleScalarSqrt ( rtb_B_311_3335_0
) ; } else if ( rtb_B_311_3335_0 < 0.0 ) { rtb_B_311_3464_0 = -
muDoubleScalarSqrt ( muDoubleScalarAbs ( rtb_B_311_3335_0 ) ) ; _rtDW ->
Sqrt1_DWORK1 = 1 ; } else { rtb_B_311_3464_0 = muDoubleScalarSqrt (
rtb_B_311_3335_0 ) ; } } _rtB -> B_311_3541_0 = rtb_B_311_3464_0 ; } if (
rtb_B_311_3375_0 ) { _rtB -> B_311_3544_0 = muDoubleScalarAbs (
rtb_B_311_3347_0 ) ; } else { if ( ! _rtB -> B_311_3393_0 ) {
rtb_B_311_3335_0 = rtb_B_311_3463_0 * rtb_B_311_3463_0 - _rtB -> B_311_3514_0
* _rtB -> B_311_3514_0 ; if ( rtb_B_311_3335_0 > _rtP -> P_416 ) {
rtb_B_311_3335_0 = _rtP -> P_416 ; } else { if ( rtb_B_311_3335_0 < _rtP ->
P_417 ) { rtb_B_311_3335_0 = _rtP -> P_417 ; } } if ( ssIsMajorTimeStep ( S )
!= 0 ) { if ( _rtDW -> Sqrt2_DWORK1 != 0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Sqrt2_DWORK1 = 0
; } rtb_B_311_3465_0 = muDoubleScalarSqrt ( rtb_B_311_3335_0 ) ; } else if (
rtb_B_311_3335_0 < 0.0 ) { rtb_B_311_3465_0 = - muDoubleScalarSqrt (
muDoubleScalarAbs ( rtb_B_311_3335_0 ) ) ; _rtDW -> Sqrt2_DWORK1 = 1 ; } else
{ rtb_B_311_3465_0 = muDoubleScalarSqrt ( rtb_B_311_3335_0 ) ; } } _rtB ->
B_311_3544_0 = rtb_B_311_3465_0 ; } if ( _rtB -> B_311_333_0_o > 0 ) { _rtB
-> B_237_0_0 = rtb_B_311_3426_0 * muDoubleScalarCos ( B_311_3315_0 ) -
rtb_B_311_3348_0 * muDoubleScalarSin ( B_311_3315_0 ) ; _rtB -> B_237_1_0 =
rtb_B_311_3426_0 * muDoubleScalarSin ( B_311_3315_0 ) + rtb_B_311_3348_0 *
muDoubleScalarCos ( B_311_3315_0 ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) {
srUpdateBC ( _rtDW -> Subsystem1_SubsysRanBC ) ; } } if ( _rtB ->
B_311_335_0_k > 0 ) { _rtB -> B_236_0_0 = rtb_B_311_3426_0 *
muDoubleScalarSin ( B_311_3315_0 ) + rtb_B_311_3348_0 * muDoubleScalarCos (
B_311_3315_0 ) ; _rtB -> B_236_1_0 = - rtb_B_311_3426_0 * muDoubleScalarCos (
B_311_3315_0 ) + rtb_B_311_3348_0 * muDoubleScalarSin ( B_311_3315_0 ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( _rtDW ->
Subsystempi2delay_SubsysRanBC ) ; } } if ( _rtB -> B_311_333_0_o != 0 ) {
rtb_B_311_3335_0 = _rtB -> B_237_0_0 ; rtb_B_311_3278_0 = _rtB -> B_237_1_0 ;
} else { rtb_B_311_3335_0 = _rtB -> B_236_0_0 ; rtb_B_311_3278_0 = _rtB ->
B_236_1_0 ; } for ( i = 0 ; i < 3 ; i ++ ) { _rtB -> B_311_3549_0 [ i ] = 0.0
; _rtB -> B_311_3549_0 [ i ] += _rtP -> P_2752 [ i ] * rtb_B_311_3335_0 ;
_rtB -> B_311_3549_0 [ i ] += _rtP -> P_2752 [ i + 3 ] * rtb_B_311_3278_0 ;
_rtB -> B_311_3549_0 [ i ] += _rtP -> P_2752 [ i + 6 ] * _rtB -> B_311_330_0
; _rtB -> B_311_3550_0 [ i ] = _rtP -> P_2753 * rtb_B_311_2877_0 [ i ] ; _rtB
-> B_311_3551_0 [ i ] = _rtP -> P_2754 * rtb_B_311_3271_0 [ i ] ; } for ( i =
0 ; i < 3 ; i ++ ) { B_311_3553_0 [ i ] = _rtP -> P_2756 * ( _rtP -> P_2755 [
i + 6 ] * _rtB -> B_311_3550_0 [ 2 ] + ( _rtP -> P_2755 [ i + 3 ] * _rtB ->
B_311_3550_0 [ 1 ] + _rtP -> P_2755 [ i ] * _rtB -> B_311_3550_0 [ 0 ] ) ) ;
} Auto_microgrid_forecasting_020822_Subsystem1_h ( S , _rtB -> B_311_340_0 ,
& B_311_3553_0 [ 0 ] , B_311_3315_0 , & _rtB -> Subsystem1_h , & _rtDW ->
Subsystem1_h ) ; Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( S ,
_rtB -> B_311_342_0 , & B_311_3553_0 [ 0 ] , B_311_3315_0 , & _rtB ->
Subsystempi2delay_p , & _rtDW -> Subsystempi2delay_p ) ; if ( _rtB ->
B_311_340_0 != 0 ) { _rtB -> B_311_3556_0 [ 0 ] = _rtB -> Subsystem1_h .
B_241_0_0 ; _rtB -> B_311_3556_0 [ 1 ] = _rtB -> Subsystem1_h . B_241_1_0 ; }
else { _rtB -> B_311_3556_0 [ 0 ] = _rtB -> Subsystempi2delay_p . B_240_0_0 ;
_rtB -> B_311_3556_0 [ 1 ] = _rtB -> Subsystempi2delay_p . B_240_1_0 ; } if (
_rtDW -> Integ4_SYSTEM_ENABLE_fs != 0 ) { _rtB -> B_311_3557_0 = _rtDW ->
Integ4_DSTATE_j2 ; } else { _rtB -> B_311_3557_0 = _rtP -> P_2757 * _rtB ->
B_311_3556_0 [ 0 ] + _rtDW -> Integ4_DSTATE_j2 ; } if ( rtb_B_311_3409_0 >
_rtP -> P_2759 ) { rtb_B_311_3262_0 = _rtP -> P_2759 ; } else if (
rtb_B_311_3409_0 < _rtP -> P_2760 ) { rtb_B_311_3262_0 = _rtP -> P_2760 ; }
else { rtb_B_311_3262_0 = rtb_B_311_3409_0 ; } rtb_B_311_3335_0 = 1.0 /
rtb_B_311_3262_0 / 2.0e-5 ; rtb_B_311_3262_0 = muDoubleScalarCeil (
rtb_B_311_3335_0 ) ; _rtB -> B_311_3561_0 = _rtP -> P_2761 * rtb_B_311_3262_0
; ssCallAccelRunBlock ( S , 311 , 3562 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) < _rtP -> P_2763 ) { rtb_B_299_3_0 = _rtP -> P_2764 ;
} else { rtb_B_299_3_0 = _rtP -> P_2765 ; } if ( rtb_B_299_3_0 >= _rtP ->
P_2767 ) { rtb_B_311_3262_0 = rtb_B_311_3335_0 - rtb_B_311_3262_0 ; _rtB ->
B_311_3567_0 = ( ( _rtB -> B_311_3556_0 [ 0 ] - _rtDW -> UnitDelay_DSTATE_d0
) * rtb_B_311_3262_0 * _rtP -> P_429 + _rtB -> B_311_3556_0 [ 0 ] ) * (
rtb_B_311_3262_0 / rtb_B_311_3335_0 ) + ( _rtB -> B_311_3557_0 - _rtB ->
B_311_3562_0 ) * rtb_B_311_3409_0 ; } else { _rtB -> B_311_3567_0 = _rtDW ->
UnitDelay_DSTATE_bs ; } if ( _rtDW -> Integ4_SYSTEM_ENABLE_lu != 0 ) { _rtB
-> B_311_3568_0 = _rtDW -> Integ4_DSTATE_d ; } else { _rtB -> B_311_3568_0 =
_rtP -> P_2768 * _rtB -> B_311_3556_0 [ 1 ] + _rtDW -> Integ4_DSTATE_d ; } if
( rtb_B_311_3409_0 > _rtP -> P_2770 ) { rtb_B_311_3262_0 = _rtP -> P_2770 ; }
else if ( rtb_B_311_3409_0 < _rtP -> P_2771 ) { rtb_B_311_3262_0 = _rtP ->
P_2771 ; } else { rtb_B_311_3262_0 = rtb_B_311_3409_0 ; } rtb_B_311_3335_0 =
1.0 / rtb_B_311_3262_0 / 2.0e-5 ; rtb_B_311_3262_0 = muDoubleScalarCeil (
rtb_B_311_3335_0 ) ; _rtB -> B_311_3572_0 = _rtP -> P_2772 * rtb_B_311_3262_0
; ssCallAccelRunBlock ( S , 311 , 3573 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) < _rtP -> P_2774 ) { rtb_B_299_3_0 = _rtP -> P_2775 ;
} else { rtb_B_299_3_0 = _rtP -> P_2776 ; } if ( rtb_B_299_3_0 >= _rtP ->
P_2778 ) { rtb_B_311_3262_0 = rtb_B_311_3335_0 - rtb_B_311_3262_0 ; _rtB ->
B_311_3578_0 = ( ( _rtB -> B_311_3556_0 [ 1 ] - _rtDW -> UnitDelay_DSTATE_db
) * rtb_B_311_3262_0 * _rtP -> P_428 + _rtB -> B_311_3556_0 [ 1 ] ) * (
rtb_B_311_3262_0 / rtb_B_311_3335_0 ) + ( _rtB -> B_311_3568_0 - _rtB ->
B_311_3573_0 ) * rtb_B_311_3409_0 ; } else { _rtB -> B_311_3578_0 = _rtDW ->
UnitDelay_DSTATE_e3 ; } rtb_B_311_3335_0 = muDoubleScalarHypot ( _rtB ->
B_311_3567_0 , _rtB -> B_311_3578_0 ) ; for ( i = 0 ; i < 3 ; i ++ ) {
B_311_3581_0 [ i ] = _rtP -> P_2780 * ( _rtP -> P_2779 [ i + 6 ] * _rtB ->
B_311_3551_0 [ 2 ] + ( _rtP -> P_2779 [ i + 3 ] * _rtB -> B_311_3551_0 [ 1 ]
+ _rtP -> P_2779 [ i ] * _rtB -> B_311_3551_0 [ 0 ] ) ) ; }
Auto_microgrid_forecasting_020822_Subsystem1_h ( S , _rtB -> B_311_345_0 , &
B_311_3581_0 [ 0 ] , B_311_3315_0 , & _rtB -> Subsystem1_hd , & _rtDW ->
Subsystem1_hd ) ; Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( S ,
_rtB -> B_311_347_0 , & B_311_3581_0 [ 0 ] , B_311_3315_0 , & _rtB ->
Subsystempi2delay_c , & _rtDW -> Subsystempi2delay_c ) ; if ( _rtB ->
B_311_345_0 != 0 ) { _rtB -> B_311_3584_0 [ 0 ] = _rtB -> Subsystem1_hd .
B_241_0_0 ; _rtB -> B_311_3584_0 [ 1 ] = _rtB -> Subsystem1_hd . B_241_1_0 ;
} else { _rtB -> B_311_3584_0 [ 0 ] = _rtB -> Subsystempi2delay_c . B_240_0_0
; _rtB -> B_311_3584_0 [ 1 ] = _rtB -> Subsystempi2delay_c . B_240_1_0 ; } if
( _rtDW -> Integ4_SYSTEM_ENABLE_c0 != 0 ) { _rtB -> B_311_3585_0 = _rtDW ->
Integ4_DSTATE_d2 ; } else { _rtB -> B_311_3585_0 = _rtP -> P_2781 * _rtB ->
B_311_3584_0 [ 0 ] + _rtDW -> Integ4_DSTATE_d2 ; } if ( rtb_B_311_3409_0 >
_rtP -> P_2783 ) { rtb_B_311_3262_0 = _rtP -> P_2783 ; } else if (
rtb_B_311_3409_0 < _rtP -> P_2784 ) { rtb_B_311_3262_0 = _rtP -> P_2784 ; }
else { rtb_B_311_3262_0 = rtb_B_311_3409_0 ; } rtb_B_311_3262_0 = 1.0 /
rtb_B_311_3262_0 / 2.0e-5 ; rtb_B_311_3278_0 = muDoubleScalarCeil (
rtb_B_311_3262_0 ) ; _rtB -> B_311_3589_0 = _rtP -> P_2785 * rtb_B_311_3278_0
; ssCallAccelRunBlock ( S , 311 , 3590 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) < _rtP -> P_2787 ) { rtb_B_299_3_0 = _rtP -> P_2788 ;
} else { rtb_B_299_3_0 = _rtP -> P_2789 ; } if ( rtb_B_299_3_0 >= _rtP ->
P_2791 ) { rtb_B_311_3278_0 = rtb_B_311_3262_0 - rtb_B_311_3278_0 ; _rtB ->
B_311_3595_0 = ( ( _rtB -> B_311_3584_0 [ 0 ] - _rtDW -> UnitDelay_DSTATE_di
) * rtb_B_311_3278_0 * _rtP -> P_431 + _rtB -> B_311_3584_0 [ 0 ] ) * (
rtb_B_311_3278_0 / rtb_B_311_3262_0 ) + ( _rtB -> B_311_3585_0 - _rtB ->
B_311_3590_0 ) * rtb_B_311_3409_0 ; } else { _rtB -> B_311_3595_0 = _rtDW ->
UnitDelay_DSTATE_lk ; } if ( _rtDW -> Integ4_SYSTEM_ENABLE_cw != 0 ) { _rtB
-> B_311_3596_0 = _rtDW -> Integ4_DSTATE_d1 ; } else { _rtB -> B_311_3596_0 =
_rtP -> P_2792 * _rtB -> B_311_3584_0 [ 1 ] + _rtDW -> Integ4_DSTATE_d1 ; }
if ( rtb_B_311_3409_0 > _rtP -> P_2794 ) { rtb_B_311_3262_0 = _rtP -> P_2794
; } else if ( rtb_B_311_3409_0 < _rtP -> P_2795 ) { rtb_B_311_3262_0 = _rtP
-> P_2795 ; } else { rtb_B_311_3262_0 = rtb_B_311_3409_0 ; } rtb_B_311_3262_0
= 1.0 / rtb_B_311_3262_0 / 2.0e-5 ; rtb_B_311_3278_0 = muDoubleScalarCeil (
rtb_B_311_3262_0 ) ; _rtB -> B_311_3600_0 = _rtP -> P_2796 * rtb_B_311_3278_0
; ssCallAccelRunBlock ( S , 311 , 3601 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) < _rtP -> P_2798 ) { rtb_B_299_3_0 = _rtP -> P_2799 ;
} else { rtb_B_299_3_0 = _rtP -> P_2800 ; } if ( rtb_B_299_3_0 >= _rtP ->
P_2802 ) { rtb_B_311_3278_0 = rtb_B_311_3262_0 - rtb_B_311_3278_0 ; _rtB ->
B_311_3606_0 = ( ( _rtB -> B_311_3584_0 [ 1 ] - _rtDW -> UnitDelay_DSTATE_ax
) * rtb_B_311_3278_0 * _rtP -> P_430 + _rtB -> B_311_3584_0 [ 1 ] ) * (
rtb_B_311_3278_0 / rtb_B_311_3262_0 ) + ( _rtB -> B_311_3596_0 - _rtB ->
B_311_3601_0 ) * rtb_B_311_3409_0 ; } else { _rtB -> B_311_3606_0 = _rtDW ->
UnitDelay_DSTATE_mh ; } rtb_B_311_3262_0 = rtb_B_311_3335_0 *
muDoubleScalarHypot ( _rtB -> B_311_3595_0 , _rtB -> B_311_3606_0 ) ;
rtb_B_311_3278_0 = ( _rtP -> P_2803 * muDoubleScalarAtan2 ( _rtB ->
B_311_3578_0 , _rtB -> B_311_3567_0 ) - _rtP -> P_2804 * muDoubleScalarAtan2
( _rtB -> B_311_3606_0 , _rtB -> B_311_3595_0 ) ) * _rtP -> P_2805 ; _rtB ->
B_311_3619_0 [ 0 ] = rtb_B_311_3262_0 * muDoubleScalarCos ( rtb_B_311_3278_0
) * _rtP -> P_2806 ; _rtB -> B_311_3619_0 [ 1 ] = muDoubleScalarSin (
rtb_B_311_3278_0 ) * rtb_B_311_3262_0 * _rtP -> P_2806 ; rtb_B_311_3335_0 /=
_rtB -> B_311_336_0_e ; rtb_B_311_3335_0 *= rtb_B_311_3335_0 ; _rtB ->
B_311_3624_0 = ( _rtP -> P_2807 * rtb_B_311_3335_0 + _rtB -> B_311_3619_0 [ 0
] ) * _rtP -> P_2808 ; ssCallAccelRunBlock ( S , 311 , 3625 ,
SS_CALL_MDL_OUTPUTS ) ; rtb_B_311_3335_0 *= _rtP -> P_2809 ; _rtB ->
B_311_3627_0 = _rtP -> P_2810 * B_311_3006_0_idx_1 ; ssCallAccelRunBlock ( S
, 311 , 3628 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 311 , 3629 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 311 , 3630 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 311 , 3631 ,
SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 311 , 3632 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_311_3640_0 [ 0 ] = ( _rtP -> P_2811 *
B_311_2872_0_idx_0 + _rtP -> P_2812 * B_311_2874_0_idx_0 ) + _rtP -> P_2815 *
B_311_2829_0_idx_0 ; _rtB -> B_311_3642_0 [ 0 ] = ( _rtP -> P_2813 *
B_311_2872_0_idx_0 + _rtP -> P_2814 * B_311_2874_0_idx_0 ) + _rtP -> P_2816 *
B_311_2829_0_idx_0 ; _rtB -> B_311_3650_0 [ 0 ] = ( _rtP -> P_2817 *
B_311_3303_0_idx_0 + _rtP -> P_2818 * B_311_3305_0_idx_0 ) + _rtP -> P_2821 *
B_311_1283_0_idx_0 ; _rtB -> B_311_3652_0 [ 0 ] = ( _rtP -> P_2819 *
B_311_3303_0_idx_0 + _rtP -> P_2820 * B_311_3305_0_idx_0 ) + _rtP -> P_2822 *
B_311_1283_0_idx_0 ; _rtB -> B_311_3660_0 [ 0 ] = ( _rtP -> P_2823 *
B_311_3242_0_idx_0 + _rtP -> P_2824 * B_311_3244_0_idx_0 ) + _rtP -> P_2827 *
rtb_B_311_1335_0 [ 0 ] ; _rtB -> B_311_3662_0 [ 0 ] = ( _rtP -> P_2825 *
B_311_3242_0_idx_0 + _rtP -> P_2826 * B_311_3244_0_idx_0 ) + _rtP -> P_2828 *
rtb_B_311_1335_0 [ 0 ] ; _rtB -> B_311_3670_0 [ 0 ] = ( _rtP -> P_2829 *
B_311_3265_0_idx_0 + _rtP -> P_2830 * B_311_3267_0_idx_0 ) + _rtP -> P_2833 *
B_311_1349_0_idx_0 ; _rtB -> B_311_3672_0 [ 0 ] = ( _rtP -> P_2831 *
B_311_3265_0_idx_0 + _rtP -> P_2832 * B_311_3267_0_idx_0 ) + _rtP -> P_2834 *
B_311_1349_0_idx_0 ; _rtB -> B_311_3640_0 [ 1 ] = ( _rtP -> P_2811 *
B_311_2872_0_idx_1 + _rtP -> P_2812 * B_311_2874_0_idx_1 ) + _rtP -> P_2815 *
B_311_2829_0_idx_1 ; _rtB -> B_311_3642_0 [ 1 ] = ( _rtP -> P_2813 *
B_311_2872_0_idx_1 + _rtP -> P_2814 * B_311_2874_0_idx_1 ) + _rtP -> P_2816 *
B_311_2829_0_idx_1 ; _rtB -> B_311_3650_0 [ 1 ] = ( _rtP -> P_2817 *
B_311_3303_0_idx_1 + _rtP -> P_2818 * B_311_3305_0_idx_1 ) + _rtP -> P_2821 *
B_311_1283_0_idx_1 ; _rtB -> B_311_3652_0 [ 1 ] = ( _rtP -> P_2819 *
B_311_3303_0_idx_1 + _rtP -> P_2820 * B_311_3305_0_idx_1 ) + _rtP -> P_2822 *
B_311_1283_0_idx_1 ; _rtB -> B_311_3660_0 [ 1 ] = ( _rtP -> P_2823 *
B_311_3242_0_idx_1 + _rtP -> P_2824 * B_311_3244_0_idx_1 ) + _rtP -> P_2827 *
rtb_B_311_1335_0 [ 1 ] ; _rtB -> B_311_3662_0 [ 1 ] = ( _rtP -> P_2825 *
B_311_3242_0_idx_1 + _rtP -> P_2826 * B_311_3244_0_idx_1 ) + _rtP -> P_2828 *
rtb_B_311_1335_0 [ 1 ] ; _rtB -> B_311_3670_0 [ 1 ] = ( _rtP -> P_2829 *
B_311_3265_0_idx_1 + _rtP -> P_2830 * B_311_3267_0_idx_1 ) + _rtP -> P_2833 *
B_311_1349_0_idx_1 ; _rtB -> B_311_3672_0 [ 1 ] = ( _rtP -> P_2831 *
B_311_3265_0_idx_1 + _rtP -> P_2832 * B_311_3267_0_idx_1 ) + _rtP -> P_2834 *
B_311_1349_0_idx_1 ; _rtB -> B_311_3640_0 [ 2 ] = ( _rtP -> P_2811 *
B_311_2872_0_idx_2 + _rtP -> P_2812 * B_311_2874_0_idx_2 ) + _rtP -> P_2815 *
B_311_2829_0_idx_2 ; _rtB -> B_311_3642_0 [ 2 ] = ( _rtP -> P_2813 *
B_311_2872_0_idx_2 + _rtP -> P_2814 * B_311_2874_0_idx_2 ) + _rtP -> P_2816 *
B_311_2829_0_idx_2 ; _rtB -> B_311_3650_0 [ 2 ] = ( _rtP -> P_2817 *
B_311_3303_0_idx_2 + _rtP -> P_2818 * B_311_3305_0_idx_2 ) + _rtP -> P_2821 *
B_311_1283_0_idx_2 ; _rtB -> B_311_3652_0 [ 2 ] = ( _rtP -> P_2819 *
B_311_3303_0_idx_2 + _rtP -> P_2820 * B_311_3305_0_idx_2 ) + _rtP -> P_2822 *
B_311_1283_0_idx_2 ; _rtB -> B_311_3660_0 [ 2 ] = ( _rtP -> P_2823 *
B_311_3242_0_idx_2 + _rtP -> P_2824 * B_311_3244_0_idx_2 ) + _rtP -> P_2827 *
rtb_B_311_1335_0 [ 2 ] ; _rtB -> B_311_3662_0 [ 2 ] = ( _rtP -> P_2825 *
B_311_3242_0_idx_2 + _rtP -> P_2826 * B_311_3244_0_idx_2 ) + _rtP -> P_2828 *
rtb_B_311_1335_0 [ 2 ] ; _rtB -> B_311_3670_0 [ 2 ] = ( _rtP -> P_2829 *
B_311_3265_0_idx_2 + _rtP -> P_2830 * B_311_3267_0_idx_2 ) + _rtP -> P_2833 *
B_311_1349_0_idx_2 ; _rtB -> B_311_3672_0 [ 2 ] = ( _rtP -> P_2831 *
B_311_3265_0_idx_2 + _rtP -> P_2832 * B_311_3267_0_idx_2 ) + _rtP -> P_2834 *
B_311_1349_0_idx_2 ; _rtB -> B_311_3682_0 = ( _rtP -> P_2836 * _rtDW ->
Delay_x1_DSTATE_de + _rtP -> P_2838 * _rtDW -> Delay_x2_DSTATE_j ) + _rtP ->
P_2841 * rtb_B_311_3103_0 ; _rtB -> B_311_3684_0 = ( _rtP -> P_2839 * _rtDW
-> Delay_x1_DSTATE_de + _rtP -> P_2840 * _rtDW -> Delay_x2_DSTATE_j ) + _rtP
-> P_2842 * rtb_B_311_3103_0 ; _rtB -> B_311_3689_0 = ( _rtP -> P_2844 *
_rtDW -> Delay_x1_DSTATE_de + _rtP -> P_2845 * _rtDW -> Delay_x2_DSTATE_j ) +
_rtP -> P_2843 * rtb_B_311_3103_0 ; _rtB -> B_311_3697_0 = ( _rtP -> P_2846 *
rtb_B_311_3408_0 + _rtP -> P_2847 * rtb_B_311_3334_0 ) + _rtP -> P_2850 *
rtb_B_299_1_0 ; _rtB -> B_311_3699_0 = ( _rtP -> P_2848 * rtb_B_311_3408_0 +
_rtP -> P_2849 * rtb_B_311_3334_0 ) + _rtP -> P_2851 * rtb_B_299_1_0 ; _rtB
-> B_311_3707_0 = ( _rtP -> P_2852 * rtb_B_311_3231_0 + _rtP -> P_2853 *
rtb_B_311_3240_0 ) + _rtP -> P_2856 * _rtB -> B_212_21_0 ; _rtB ->
B_311_3709_0 = ( _rtP -> P_2854 * rtb_B_311_3231_0 + _rtP -> P_2855 *
rtb_B_311_3240_0 ) + _rtP -> P_2857 * _rtB -> B_212_21_0 ; _rtB ->
B_311_3717_0 = ( _rtP -> P_2858 * rtb_B_311_3233_0 + _rtP -> P_2859 *
rtb_B_311_3235_0 ) + _rtP -> P_2862 * _rtB -> B_311_3074_0 ; _rtB ->
B_311_3719_0 = ( _rtP -> P_2860 * rtb_B_311_3233_0 + _rtP -> P_2861 *
rtb_B_311_3235_0 ) + _rtP -> P_2863 * _rtB -> B_311_3074_0 ; _rtB ->
B_311_3727_0 = ( _rtP -> P_2864 * rtb_B_311_3253_0 + _rtP -> P_2865 *
rtb_B_311_3255_0 ) + _rtP -> P_2868 * rtb_B_311_3251_0 ; _rtB -> B_311_3729_0
= ( _rtP -> P_2866 * rtb_B_311_3253_0 + _rtP -> P_2867 * rtb_B_311_3255_0 ) +
_rtP -> P_2869 * rtb_B_311_3251_0 ; _rtB -> B_311_3730_0 = _rtP -> P_2870 ; i
= ssIsSampleHit ( S , 2 , 0 ) ; if ( ( i != 0 ) && ( ssIsMajorTimeStep ( S )
!= 0 ) ) { if ( _rtB -> B_311_3730_0 > 0.0 ) { if ( ! _rtDW ->
AutomaticGainControl_MODE_g ) { if ( ssGetTaskTime ( S , 2 ) != ssGetTStart (
S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtDW ->
Integ4_SYSTEM_ENABLE_lr = 1U ; _rtDW -> Integ4_SYSTEM_ENABLE_nz = 1U ; _rtDW
-> AutomaticGainControl_MODE_g = true ; } } else { if ( _rtDW ->
AutomaticGainControl_MODE_g ) { ssSetBlockStateForSolverChangedAtMajorStep (
S ) ; _rtDW -> Integ4_DSTATE_l5 = _rtB -> B_271_5_0 ; _rtDW ->
Integ4_DSTATE_cy = _rtB -> B_271_17_0 ; _rtDW -> AutomaticGainControl_MODE_g
= false ; } } } if ( _rtDW -> AutomaticGainControl_MODE_g ) { i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { for ( i = 0 ; i < 3 ; i ++ ) {
B_271_1_0 [ i ] = _rtP -> P_453 * ( _rtP -> P_452 [ i + 6 ] *
rtb_B_311_2877_0 [ 2 ] + ( _rtP -> P_452 [ i + 3 ] * rtb_B_311_2877_0 [ 1 ] +
_rtP -> P_452 [ i ] * rtb_B_311_2877_0 [ 0 ] ) ) ; }
Auto_microgrid_forecasting_020822_Subsystem1_h ( S , _rtB -> B_271_4_0_c , &
B_271_1_0 [ 0 ] , B_311_3315_0 , & _rtB -> Subsystem1_ny , & _rtDW ->
Subsystem1_ny ) ; Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( S ,
_rtB -> B_271_6_0 , & B_271_1_0 [ 0 ] , B_311_3315_0 , & _rtB ->
Subsystempi2delay_p3 , & _rtDW -> Subsystempi2delay_p3 ) ; if ( _rtB ->
B_271_4_0_c != 0 ) { _rtB -> B_271_4_0 [ 0 ] = _rtB -> Subsystem1_ny .
B_241_0_0 ; _rtB -> B_271_4_0 [ 1 ] = _rtB -> Subsystem1_ny . B_241_1_0 ; }
else { _rtB -> B_271_4_0 [ 0 ] = _rtB -> Subsystempi2delay_p3 . B_240_0_0 ;
_rtB -> B_271_4_0 [ 1 ] = _rtB -> Subsystempi2delay_p3 . B_240_1_0 ; } if (
_rtDW -> Integ4_SYSTEM_ENABLE_lr != 0 ) { _rtB -> B_271_5_0 = _rtDW ->
Integ4_DSTATE_l5 ; } else { _rtB -> B_271_5_0 = _rtP -> P_454 * _rtB ->
B_271_4_0 [ 0 ] + _rtDW -> Integ4_DSTATE_l5 ; } if ( rtb_B_311_3409_0 > _rtP
-> P_456 ) { rtb_B_311_3262_0 = _rtP -> P_456 ; } else if ( rtb_B_311_3409_0
< _rtP -> P_457 ) { rtb_B_311_3262_0 = _rtP -> P_457 ; } else {
rtb_B_311_3262_0 = rtb_B_311_3409_0 ; } rtb_B_311_3233_0 = 1.0 /
rtb_B_311_3262_0 / 2.0e-5 ; rtb_B_311_3235_0 = muDoubleScalarCeil (
rtb_B_311_3233_0 ) ; _rtB -> B_271_9_0 = _rtP -> P_458 * rtb_B_311_3235_0 ;
ssCallAccelRunBlock ( S , 271 , 10 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_271_0_0 ) { rtb_B_311_3235_0 =
rtb_B_311_3233_0 - rtb_B_311_3235_0 ; _rtB -> B_271_16_0 = ( ( _rtB ->
B_271_4_0 [ 0 ] - _rtDW -> UnitDelay_DSTATE_ew ) * rtb_B_311_3235_0 * _rtP ->
P_449 + _rtB -> B_271_4_0 [ 0 ] ) * ( rtb_B_311_3235_0 / rtb_B_311_3233_0 ) +
( _rtB -> B_271_5_0 - _rtB -> B_271_10_0 ) * rtb_B_311_3409_0 ; } else { _rtB
-> B_271_16_0 = _rtDW -> UnitDelay1_DSTATE_cc ; } if ( _rtDW ->
Integ4_SYSTEM_ENABLE_nz != 0 ) { _rtB -> B_271_17_0 = _rtDW ->
Integ4_DSTATE_cy ; } else { _rtB -> B_271_17_0 = _rtP -> P_469 * _rtB ->
B_271_4_0 [ 1 ] + _rtDW -> Integ4_DSTATE_cy ; } if ( rtb_B_311_3409_0 > _rtP
-> P_471 ) { rtb_B_311_3262_0 = _rtP -> P_471 ; } else if ( rtb_B_311_3409_0
< _rtP -> P_472 ) { rtb_B_311_3262_0 = _rtP -> P_472 ; } else {
rtb_B_311_3262_0 = rtb_B_311_3409_0 ; } rtb_B_311_3233_0 = 1.0 /
rtb_B_311_3262_0 / 2.0e-5 ; rtb_B_311_3235_0 = muDoubleScalarCeil (
rtb_B_311_3233_0 ) ; _rtB -> B_271_21_0 = _rtP -> P_473 * rtb_B_311_3235_0 ;
ssCallAccelRunBlock ( S , 271 , 22 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_271_1_0 ) { rtb_B_311_3235_0 =
rtb_B_311_3233_0 - rtb_B_311_3235_0 ; _rtB -> B_271_28_0 = ( ( _rtB ->
B_271_4_0 [ 1 ] - _rtDW -> UnitDelay_DSTATE_ne ) * rtb_B_311_3235_0 * _rtP ->
P_450 + _rtB -> B_271_4_0 [ 1 ] ) * ( rtb_B_311_3235_0 / rtb_B_311_3233_0 ) +
( _rtB -> B_271_17_0 - _rtB -> B_271_22_0 ) * rtb_B_311_3409_0 ; } else {
_rtB -> B_271_28_0 = _rtDW -> UnitDelay1_DSTATE_nvr ; } rtb_B_311_35_0 =
muDoubleScalarHypot ( _rtB -> B_271_16_0 , _rtB -> B_271_28_0 ) ; if (
rtb_B_311_35_0 > _rtP -> P_484 ) { rtb_B_311_35_0 = _rtP -> P_484 ; } else {
if ( rtb_B_311_35_0 < _rtP -> P_485 ) { rtb_B_311_35_0 = _rtP -> P_485 ; } }
_rtB -> B_271_32_0 = 1.0 / rtb_B_311_35_0 ; } if ( ssIsMajorTimeStep ( S ) !=
0 ) { srUpdateBC ( _rtDW -> AutomaticGainControl_SubsysRanBC_d ) ; } } for (
i = 0 ; i < 3 ; i ++ ) { B_311_3006_0_idx_1 = _rtP -> P_2871 [ i + 6 ] *
rtb_B_311_2877_0 [ 2 ] + ( _rtP -> P_2871 [ i + 3 ] * rtb_B_311_2877_0 [ 1 ]
+ _rtP -> P_2871 [ i ] * rtb_B_311_2877_0 [ 0 ] ) ; B_311_3733_0 [ i ] = _rtP
-> P_2872 * B_311_3006_0_idx_1 ; rtb_B_311_3271_0 [ i ] = B_311_3006_0_idx_1
; } Auto_microgrid_forecasting_020822_Subsystem1_h ( S , _rtB -> B_311_382_0
, & B_311_3733_0 [ 0 ] , B_311_3315_0 , & _rtB -> Subsystem1_d , & _rtDW ->
Subsystem1_d ) ; Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( S ,
_rtB -> B_311_384_0 , & B_311_3733_0 [ 0 ] , B_311_3315_0 , & _rtB ->
Subsystempi2delay_nr , & _rtDW -> Subsystempi2delay_nr ) ; if ( _rtB ->
B_311_382_0 != 0 ) { _rtB -> B_311_3736_0 [ 0 ] = _rtB -> Subsystem1_d .
B_241_0_0 ; _rtB -> B_311_3736_0 [ 1 ] = _rtB -> Subsystem1_d . B_241_1_0 ; }
else { _rtB -> B_311_3736_0 [ 0 ] = _rtB -> Subsystempi2delay_nr . B_240_0_0
; _rtB -> B_311_3736_0 [ 1 ] = _rtB -> Subsystempi2delay_nr . B_240_1_0 ; }
if ( _rtDW -> Integ4_SYSTEM_ENABLE_br != 0 ) { _rtB -> B_311_3737_0 = _rtDW
-> Integ4_DSTATE_j2b ; } else { _rtB -> B_311_3737_0 = _rtP -> P_2873 * _rtB
-> B_311_3736_0 [ 1 ] + _rtDW -> Integ4_DSTATE_j2b ; } if ( rtb_B_311_3409_0
> _rtP -> P_2875 ) { rtb_B_311_3262_0 = _rtP -> P_2875 ; } else if (
rtb_B_311_3409_0 < _rtP -> P_2876 ) { rtb_B_311_3262_0 = _rtP -> P_2876 ; }
else { rtb_B_311_3262_0 = rtb_B_311_3409_0 ; } rtb_B_311_3233_0 = 1.0 /
rtb_B_311_3262_0 / 2.0e-5 ; rtb_B_311_3235_0 = muDoubleScalarCeil (
rtb_B_311_3233_0 ) ; _rtB -> B_311_3741_0 = _rtP -> P_2877 * rtb_B_311_3235_0
; ssCallAccelRunBlock ( S , 311 , 3742 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_379_0 ) { rtb_B_311_3235_0 =
rtb_B_311_3233_0 - rtb_B_311_3235_0 ; _rtB -> B_311_3748_0 = ( ( _rtB ->
B_311_3736_0 [ 1 ] - _rtDW -> UnitDelay_DSTATE_hc ) * rtb_B_311_3235_0 * _rtP
-> P_491 + _rtB -> B_311_3736_0 [ 1 ] ) * ( rtb_B_311_3235_0 /
rtb_B_311_3233_0 ) + ( _rtB -> B_311_3737_0 - _rtB -> B_311_3742_0 ) *
rtb_B_311_3409_0 ; } else { _rtB -> B_311_3748_0 = _rtDW ->
UnitDelay1_DSTATE_jv ; } _rtB -> B_311_3749_0 = _rtB -> B_311_3748_0 * _rtB
-> B_271_32_0 ; _rtDW -> DiscreteDerivative_tmp = ( _rtB -> B_311_3749_0 -
_rtP -> P_2889 [ 1 ] * _rtDW -> DiscreteDerivative_states ) / _rtP -> P_2889
[ 0 ] ; rtb_B_311_35_0 = ( _rtP -> P_2895 * _rtB -> B_311_3749_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_gt ) + ( _rtP -> P_2888 [ 0 ] * _rtDW ->
DiscreteDerivative_tmp + _rtP -> P_2888 [ 1 ] * _rtDW ->
DiscreteDerivative_states ) ; if ( rtb_B_311_35_0 > _rtP -> P_2896 ) { _rtB
-> B_311_3754_0 = _rtP -> P_2896 ; } else if ( rtb_B_311_35_0 < _rtP ->
P_2897 ) { _rtB -> B_311_3754_0 = _rtP -> P_2897 ; } else { _rtB ->
B_311_3754_0 = rtb_B_311_35_0 ; } rtb_B_311_3750_0 = _rtP -> P_2898 * _rtB ->
B_311_3754_0 ; rtb_B_311_251_0 = rtb_B_311_3750_0 - _rtDW -> PrevY ; if (
rtb_B_311_251_0 > _rtP -> P_2899 ) { rtb_B_311_3750_0 = _rtDW -> PrevY + _rtP
-> P_2899 ; } else { if ( rtb_B_311_251_0 < _rtP -> P_2900 ) {
rtb_B_311_3750_0 = _rtDW -> PrevY + _rtP -> P_2900 ; } } _rtDW -> PrevY =
rtb_B_311_3750_0 ; _rtB -> B_311_3766_0 = ( _rtP -> P_2903 * _rtDW ->
Delay_x1_DSTATE_j + _rtP -> P_2905 * _rtDW -> Delay_x2_DSTATE_ht ) + _rtP ->
P_2908 * rtb_B_311_3750_0 ; _rtB -> B_311_3768_0 = ( _rtP -> P_2906 * _rtDW
-> Delay_x1_DSTATE_j + _rtP -> P_2907 * _rtDW -> Delay_x2_DSTATE_ht ) + _rtP
-> P_2909 * rtb_B_311_3750_0 ; _rtB -> B_311_3773_0 = ( _rtP -> P_2911 *
_rtDW -> Delay_x1_DSTATE_j + _rtP -> P_2912 * _rtDW -> Delay_x2_DSTATE_ht ) +
_rtP -> P_2910 * rtb_B_311_3750_0 ; _rtB -> B_311_3776_0 = _rtP -> P_2913 *
rtb_B_311_3261_0 + _rtP -> P_2914 * rtb_B_311_3279_0 ; _rtB -> B_311_3778_0 =
( _rtB -> B_311_3619_0 [ 1 ] - rtb_B_311_3335_0 ) * _rtP -> P_2915 ; if (
_rtB -> B_311_3624_0 > _rtP -> P_2916 ) { rtb_B_299_3_0 = _rtP -> P_2916 ; }
else if ( _rtB -> B_311_3624_0 < _rtP -> P_2917 ) { rtb_B_299_3_0 = _rtP ->
P_2917 ; } else { rtb_B_299_3_0 = _rtB -> B_311_3624_0 ; } rtb_B_311_3469_0 =
_rtP -> P_2918 * rtb_B_299_3_0 ; if ( rtb_B_311_3469_0 >= _rtP -> P_2919 ) {
rtb_B_311_3750_0 = _rtB -> B_311_405_0 ; } else { rtb_B_311_3750_0 = ( -
0.5551 * muDoubleScalarPower ( rtb_B_311_3469_0 , 2.0 ) + 1.183 *
rtb_B_311_3469_0 ) + 0.425 ; } rtb_B_311_3465_0 = _rtDW -> Delay_x_DSTATE_l ;
_rtB -> B_311_3786_0 = _rtP -> P_2920 * rtb_B_311_3750_0 + _rtP -> P_2922 *
_rtDW -> Delay_x_DSTATE_l ; _rtB -> B_311_3787_0 = _rtB -> B_311_3238_0 -
_rtB -> B_311_3786_0 ; rtb_B_311_3463_0 = _rtP -> P_2923 * _rtB ->
B_311_3787_0 + _rtDW -> Integrator_DSTATE_j1 ; if ( rtb_B_311_3463_0 > _rtP
-> P_2926 ) { rtb_B_311_3464_0 = _rtP -> P_2926 ; } else if (
rtb_B_311_3463_0 < _rtP -> P_2927 ) { rtb_B_311_3464_0 = _rtP -> P_2927 ; }
else { rtb_B_311_3464_0 = rtb_B_311_3463_0 ; } } i = ssIsSampleHit ( S , 4 ,
0 ) ; if ( i != 0 ) { if ( _rtB -> B_311_290_0 >= _rtP -> P_2928 ) { _rtB ->
B_311_3792_0 = _rtB -> B_212_15_0 ; } else { _rtB -> B_311_3792_0 = _rtB ->
B_311_291_0 ; } _rtB -> B_311_3793_0 = ( _rtB -> B_311_3792_0 <= _rtB ->
B_311_402_0 ) ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { if ( _rtB
-> B_311_3793_0 ) { _rtB -> B_311_3794_0 = rtb_B_311_3464_0 ; } else { _rtB
-> B_311_3794_0 = _rtB -> B_311_3792_0 ; } rtb_B_311_3795_0 = _rtB ->
B_311_3794_0 - _rtB -> B_311_3689_0 ; rtb_B_311_35_0 = _rtP -> P_2929 *
rtb_B_311_3795_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_mc ; if (
rtb_B_311_35_0 > _rtP -> P_2934 ) { _rtB -> B_311_3799_0 = _rtP -> P_2934 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_2935 ) { _rtB -> B_311_3799_0 = _rtP ->
P_2935 ; } else { _rtB -> B_311_3799_0 = rtb_B_311_35_0 ; }
ssCallAccelRunBlock ( S , 311 , 3800 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_311_3801_0 = _rtDW -> Delay1_DSTATE_m ; ssCallAccelRunBlock ( S , 282 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; rtb_B_311_3805_0 = ( _rtB -> B_311_3238_0 - _rtB ->
B_311_3786_0 ) * _rtP -> P_2937 ; if ( rtb_B_311_3805_0 > _rtP -> P_2938 ) {
rtb_B_311_3805_0 = _rtP -> P_2938 ; } else { if ( rtb_B_311_3805_0 < _rtP ->
P_2939 ) { rtb_B_311_3805_0 = _rtP -> P_2939 ; } } } i = ssIsSampleHit ( S ,
4 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_3806_0 = muDoubleScalarMin ( _rtB ->
B_212_17_0 , _rtB -> B_311_290_0 ) ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if (
i != 0 ) { rtb_B_311_3807_0 = rtb_B_311_3469_0 - _rtB -> B_311_3806_0 ;
rtb_B_311_35_0 = _rtP -> P_2940 * rtb_B_311_3807_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_hk ; if ( rtb_B_311_35_0 > _rtP -> P_2945 ) {
rtb_B_311_35_0 = _rtP -> P_2945 ; } else { if ( rtb_B_311_35_0 < _rtP ->
P_2946 ) { rtb_B_311_35_0 = _rtP -> P_2946 ; } } _rtB -> B_311_3812_0 =
rtb_B_311_3805_0 + rtb_B_311_35_0 ; } if ( ( _rtDW -> LastMajorTimeA_a >=
ssGetTaskTime ( S , 0 ) ) && ( _rtDW -> LastMajorTimeB_h >= ssGetTaskTime ( S
, 0 ) ) ) { _rtB -> B_311_3813_0 = _rtB -> B_311_3812_0 ; } else { if ( ( (
_rtDW -> LastMajorTimeA_a < _rtDW -> LastMajorTimeB_h ) && ( _rtDW ->
LastMajorTimeB_h < ssGetTaskTime ( S , 0 ) ) ) || ( ( _rtDW ->
LastMajorTimeA_a >= _rtDW -> LastMajorTimeB_h ) && ( _rtDW ->
LastMajorTimeA_a >= ssGetTaskTime ( S , 0 ) ) ) ) { rtb_B_311_62_0 =
ssGetTaskTime ( S , 0 ) - _rtDW -> LastMajorTimeB_h ; B_311_1965_0_idx_0 =
_rtDW -> PrevYB_f ; } else { rtb_B_311_62_0 = ssGetTaskTime ( S , 0 ) - _rtDW
-> LastMajorTimeA_a ; B_311_1965_0_idx_0 = _rtDW -> PrevYA_o ; }
rtb_B_311_2022_0 = rtb_B_311_62_0 * _rtP -> P_2947 ; rtb_B_311_251_0 = _rtB
-> B_311_3812_0 - B_311_1965_0_idx_0 ; if ( rtb_B_311_251_0 >
rtb_B_311_2022_0 ) { _rtB -> B_311_3813_0 = B_311_1965_0_idx_0 +
rtb_B_311_2022_0 ; } else { rtb_B_311_62_0 *= _rtP -> P_2948 ; if (
rtb_B_311_251_0 < rtb_B_311_62_0 ) { _rtB -> B_311_3813_0 =
B_311_1965_0_idx_0 + rtb_B_311_62_0 ; } else { _rtB -> B_311_3813_0 = _rtB ->
B_311_3812_0 ; } } } if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW ->
upitch_max_MODE = _rtB -> B_311_3813_0 >= _rtP -> P_2949 ? 1 : _rtB ->
B_311_3813_0 > _rtP -> P_2950 ? 0 : - 1 ; } _rtB -> B_311_3814_0 = _rtDW ->
upitch_max_MODE == 1 ? _rtP -> P_2949 : _rtDW -> upitch_max_MODE == - 1 ?
_rtP -> P_2950 : _rtB -> B_311_3813_0 ; i = ssIsSampleHit ( S , 2 , 0 ) ; if
( i != 0 ) { rtb_B_311_3815_0 = _rtP -> P_2951 * _rtB -> B_311_3814_0 ;
rtb_B_311_3805_0 = _rtDW -> Delay_x_DSTATE_a ; rtb_B_311_3279_0 = _rtP ->
P_2953 * _rtDW -> Delay_x_DSTATE_a ; rtPrevAction = _rtDW ->
SwitchCase1_ActiveSubsystem_h ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if (
_rtB -> B_282_0_1 < 0.0 ) { rtb_B_299_3_0 = muDoubleScalarCeil ( _rtB ->
B_282_0_1 ) ; } else { rtb_B_299_3_0 = muDoubleScalarFloor ( _rtB ->
B_282_0_1 ) ; } if ( muDoubleScalarIsNaN ( rtb_B_299_3_0 ) ||
muDoubleScalarIsInf ( rtb_B_299_3_0 ) ) { rtb_B_299_3_0 = 0.0 ; } else {
rtb_B_299_3_0 = muDoubleScalarRem ( rtb_B_299_3_0 , 4.294967296E+9 ) ; }
switch ( rtb_B_299_3_0 < 0.0 ? - ( int32_T ) ( uint32_T ) - rtb_B_299_3_0 : (
int32_T ) ( uint32_T ) rtb_B_299_3_0 ) { case 1 : rtAction = 0 ; break ; case
2 : rtAction = 1 ; break ; case 3 : rtAction = 2 ; break ; default : rtAction
= 3 ; break ; } _rtDW -> SwitchCase1_ActiveSubsystem_h = rtAction ; } else {
rtAction = _rtDW -> SwitchCase1_ActiveSubsystem_h ; } switch ( rtAction ) {
case 0 : if ( rtAction != rtPrevAction ) { if ( ssGetTaskTime ( S , 2 ) !=
ssGetTStart ( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } }
i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { if ( _rtB -> B_283_2_0 >
_rtP -> P_492 ) { _rtB -> B_311_3823_0 = _rtP -> P_492 ; } else if ( _rtB ->
B_283_2_0 < _rtP -> P_493 ) { _rtB -> B_311_3823_0 = _rtP -> P_493 ; } else {
_rtB -> B_311_3823_0 = _rtB -> B_283_2_0 ; } } if ( ssIsMajorTimeStep ( S )
!= 0 ) { srUpdateBC ( _rtDW -> Startup1_SubsysRanBC ) ; } break ; case 1 : if
( rtAction != rtPrevAction ) { if ( ssGetTaskTime ( S , 2 ) != ssGetTStart (
S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } _rtB ->
B_311_3823_0 = rtb_B_311_3815_0 + rtb_B_311_3279_0 ; if ( ssIsMajorTimeStep (
S ) != 0 ) { srUpdateBC ( _rtDW -> Operational1_SubsysRanBC ) ; } break ;
case 2 : if ( rtAction != rtPrevAction ) { if ( ssGetTaskTime ( S , 2 ) !=
ssGetTStart ( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } }
i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_3823_0 = _rtB
-> B_285_0_0 ; } if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( _rtDW ->
Shutdown1_SubsysRanBC ) ; } break ; case 3 : break ; } ssCallAccelRunBlock (
S , 311 , 3824 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_311_3825_0 = _rtP ->
P_2955 * rtb_B_311_3807_0 ; _rtB -> B_311_3826_0 = _rtP -> P_2956 *
rtb_B_311_3795_0 ; _rtB -> B_311_3829_0 = _rtP -> P_2957 * rtb_B_311_3805_0 +
_rtP -> P_2958 * _rtB -> B_311_3814_0 ; _rtB -> B_311_3832_0 = _rtP -> P_2959
* rtb_B_311_3465_0 + _rtP -> P_2960 * rtb_B_311_3750_0 ; _rtB -> B_311_3836_0
= ( rtb_B_311_3464_0 - rtb_B_311_3463_0 ) * _rtP -> P_2961 + _rtP -> P_2962 *
_rtB -> B_311_3787_0 ; rtb_B_311_3815_0 = _rtDW -> UnitDelay2_DSTATE_nh ; }
if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Avoiddivbyzero_MODE = _rtB ->
B_311_1246_0 >= _rtP -> P_2964 ? 1 : _rtB -> B_311_1246_0 > _rtP -> P_2965 ?
0 : - 1 ; } rtb_B_311_3795_0 = _rtDW -> Avoiddivbyzero_MODE == 1 ? _rtP ->
P_2964 : _rtDW -> Avoiddivbyzero_MODE == - 1 ? _rtP -> P_2965 : _rtB ->
B_311_1246_0 ; i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { if (
rtb_B_311_3086_0 > _rtP -> P_2966 ) { _rtB -> B_311_3839_0 = _rtP -> P_2966 ;
} else if ( rtb_B_311_3086_0 < _rtP -> P_2967 ) { _rtB -> B_311_3839_0 = _rtP
-> P_2967 ; } else { _rtB -> B_311_3839_0 = rtb_B_311_3086_0 ; } } _rtB ->
B_311_3846_0 = ( look2_binlxpw ( 70.199999999999989 * _rtB -> B_311_3839_0 /
rtb_B_311_3795_0 , _rtB -> B_311_3096_0 , _rtP -> P_2969 , _rtP -> P_2970 ,
_rtP -> P_2968 , _rtP -> P_4253 , 29U ) * ( 830.42626737941316 *
muDoubleScalarPower ( rtb_B_311_3795_0 , 3.0 ) ) * _rtP -> P_2971 / _rtB ->
B_311_3839_0 - _rtB -> B_311_3089_0 ) * _rtP -> P_2972 ; i = ssIsSampleHit (
S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_3847_0 = _rtP -> P_2973 *
rtb_B_311_3087_0 ; _rtB -> B_311_3852_0 = _rtP -> P_2974 * rtb_B_311_3103_0 *
rtb_B_298_4_0 ; ssCallAccelRunBlock ( S , 311 , 3853 , SS_CALL_MDL_OUTPUTS )
; B_311_1283_0_idx_0 *= _rtP -> P_2975 ; _rtB -> B_311_3860_0 = ( ( _rtB ->
B_311_25_0 * B_311_1283_0_idx_0 - _rtP -> P_2975 * B_311_1283_0_idx_2 * _rtB
-> B_311_31_0 ) / rtb_B_299_1_0 + _rtB -> B_311_3852_0 ) * _rtP -> P_2976 ;
_rtB -> B_311_3862_0 = ( _rtDW -> DiscreteTimeIntegrator_DSTATE_oc > _rtP ->
P_2977 ) ; if ( _rtDW -> Initial_FirstOutputTime_ey ) { _rtDW ->
Initial_FirstOutputTime_ey = false ; _rtB -> B_311_3864_0 = _rtP -> P_2978 ;
} else { _rtB -> B_311_3864_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_oc -
_rtP -> P_2977 ; } if ( _rtB -> B_311_3862_0 && ( _rtDW ->
DiscreteTimeIntegrator_PrevResetState_ox <= 0 ) ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_oc = _rtB -> B_311_3864_0 ; } B_311_3865_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_oc ; _rtB -> B_311_3866_0 = _rtP ->
P_2980 ; i = ssIsSampleHit ( S , 2 , 0 ) ; if ( ( i != 0 ) && (
ssIsMajorTimeStep ( S ) != 0 ) ) { if ( _rtB -> B_311_3866_0 > 0.0 ) { if ( !
_rtDW -> AutomaticGainControl_MODE ) { if ( ssGetTaskTime ( S , 2 ) !=
ssGetTStart ( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
_rtDW -> Integ4_SYSTEM_ENABLE_kb = 1U ; _rtDW -> Integ4_SYSTEM_ENABLE_hc = 1U
; _rtDW -> AutomaticGainControl_MODE = true ; } } else { if ( _rtDW ->
AutomaticGainControl_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; _rtDW -> Integ4_DSTATE_f5 = _rtB -> B_292_5_0 ; _rtDW -> Integ4_DSTATE_p5
= _rtB -> B_292_17_0 ; _rtDW -> AutomaticGainControl_MODE = false ; } } } if
( _rtDW -> AutomaticGainControl_MODE ) { i = ssIsSampleHit ( S , 2 , 0 ) ; if
( i != 0 ) { for ( i = 0 ; i < 3 ; i ++ ) { B_292_1_0 [ i ] = _rtP -> P_501 *
( _rtP -> P_500 [ i + 6 ] * B_311_2829_0_idx_2 + ( _rtP -> P_500 [ i + 3 ] *
B_311_2829_0_idx_1 + _rtP -> P_500 [ i ] * B_311_2829_0_idx_0 ) ) ; }
Auto_microgrid_forecasting_020822_Subsystem1_h ( S , _rtB -> B_292_4_0_m , &
B_292_1_0 [ 0 ] , B_311_3865_0 , & _rtB -> Subsystem1_al , & _rtDW ->
Subsystem1_al ) ; Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( S ,
_rtB -> B_292_6_0 , & B_292_1_0 [ 0 ] , B_311_3865_0 , & _rtB ->
Subsystempi2delay_ba , & _rtDW -> Subsystempi2delay_ba ) ; if ( _rtB ->
B_292_4_0_m != 0 ) { _rtB -> B_292_4_0 [ 0 ] = _rtB -> Subsystem1_al .
B_241_0_0 ; _rtB -> B_292_4_0 [ 1 ] = _rtB -> Subsystem1_al . B_241_1_0 ; }
else { _rtB -> B_292_4_0 [ 0 ] = _rtB -> Subsystempi2delay_ba . B_240_0_0 ;
_rtB -> B_292_4_0 [ 1 ] = _rtB -> Subsystempi2delay_ba . B_240_1_0 ; } if (
_rtDW -> Integ4_SYSTEM_ENABLE_kb != 0 ) { _rtB -> B_292_5_0 = _rtDW ->
Integ4_DSTATE_f5 ; } else { _rtB -> B_292_5_0 = _rtP -> P_502 * _rtB ->
B_292_4_0 [ 0 ] + _rtDW -> Integ4_DSTATE_f5 ; } if ( _rtB -> B_311_3068_0 >
_rtP -> P_504 ) { rtb_B_299_3_0 = _rtP -> P_504 ; } else if ( _rtB ->
B_311_3068_0 < _rtP -> P_505 ) { rtb_B_299_3_0 = _rtP -> P_505 ; } else {
rtb_B_299_3_0 = _rtB -> B_311_3068_0 ; } rtb_B_311_3086_0 = 1.0 /
rtb_B_299_3_0 / 2.0e-5 ; rtb_B_311_3087_0 = muDoubleScalarCeil (
rtb_B_311_3086_0 ) ; _rtB -> B_292_9_0 = _rtP -> P_506 * rtb_B_311_3087_0 ;
ssCallAccelRunBlock ( S , 292 , 10 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_292_0_0 ) { rtb_B_311_3087_0 =
rtb_B_311_3086_0 - rtb_B_311_3087_0 ; _rtB -> B_292_16_0 = ( ( _rtB ->
B_292_4_0 [ 0 ] - _rtDW -> UnitDelay_DSTATE_m4 ) * rtb_B_311_3087_0 * _rtP ->
P_497 + _rtB -> B_292_4_0 [ 0 ] ) * ( rtb_B_311_3087_0 / rtb_B_311_3086_0 ) +
( _rtB -> B_292_5_0 - _rtB -> B_292_10_0 ) * _rtB -> B_311_3068_0 ; } else {
_rtB -> B_292_16_0 = _rtDW -> UnitDelay1_DSTATE_bj ; } if ( _rtDW ->
Integ4_SYSTEM_ENABLE_hc != 0 ) { _rtB -> B_292_17_0 = _rtDW ->
Integ4_DSTATE_p5 ; } else { _rtB -> B_292_17_0 = _rtP -> P_517 * _rtB ->
B_292_4_0 [ 1 ] + _rtDW -> Integ4_DSTATE_p5 ; } if ( _rtB -> B_311_3068_0 >
_rtP -> P_519 ) { rtb_B_299_3_0 = _rtP -> P_519 ; } else if ( _rtB ->
B_311_3068_0 < _rtP -> P_520 ) { rtb_B_299_3_0 = _rtP -> P_520 ; } else {
rtb_B_299_3_0 = _rtB -> B_311_3068_0 ; } rtb_B_311_3086_0 = 1.0 /
rtb_B_299_3_0 / 2.0e-5 ; rtb_B_311_3087_0 = muDoubleScalarCeil (
rtb_B_311_3086_0 ) ; _rtB -> B_292_21_0 = _rtP -> P_521 * rtb_B_311_3087_0 ;
ssCallAccelRunBlock ( S , 292 , 22 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_292_1_0 ) { rtb_B_311_3087_0 =
rtb_B_311_3086_0 - rtb_B_311_3087_0 ; _rtB -> B_292_28_0 = ( ( _rtB ->
B_292_4_0 [ 1 ] - _rtDW -> UnitDelay_DSTATE_m1 ) * rtb_B_311_3087_0 * _rtP ->
P_498 + _rtB -> B_292_4_0 [ 1 ] ) * ( rtb_B_311_3087_0 / rtb_B_311_3086_0 ) +
( _rtB -> B_292_17_0 - _rtB -> B_292_22_0 ) * _rtB -> B_311_3068_0 ; } else {
_rtB -> B_292_28_0 = _rtDW -> UnitDelay1_DSTATE_nv ; } rtb_B_311_35_0 =
muDoubleScalarHypot ( _rtB -> B_292_16_0 , _rtB -> B_292_28_0 ) ; if (
rtb_B_311_35_0 > _rtP -> P_532 ) { rtb_B_311_35_0 = _rtP -> P_532 ; } else {
if ( rtb_B_311_35_0 < _rtP -> P_533 ) { rtb_B_311_35_0 = _rtP -> P_533 ; } }
_rtB -> B_292_32_0 = 1.0 / rtb_B_311_35_0 ; } if ( ssIsMajorTimeStep ( S ) !=
0 ) { srUpdateBC ( _rtDW -> AutomaticGainControl_SubsysRanBC ) ; } } for ( i
= 0 ; i < 3 ; i ++ ) { B_311_3006_0_idx_1 = _rtP -> P_2981 [ i + 6 ] *
B_311_2829_0_idx_2 + ( _rtP -> P_2981 [ i + 3 ] * B_311_2829_0_idx_1 + _rtP
-> P_2981 [ i ] * B_311_2829_0_idx_0 ) ; B_311_3869_0 [ i ] = _rtP -> P_2982
* B_311_3006_0_idx_1 ; rtb_B_311_3271_0 [ i ] = B_311_3006_0_idx_1 ; }
Auto_microgrid_forecasting_020822_Subsystem1_h ( S , _rtB -> B_311_411_0_o ,
& B_311_3869_0 [ 0 ] , B_311_3865_0 , & _rtB -> Subsystem1_f , & _rtDW ->
Subsystem1_f ) ; Auto_microgrid_forecasting_020822_Subsystempi2delay_p ( S ,
_rtB -> B_311_413_0 , & B_311_3869_0 [ 0 ] , B_311_3865_0 , & _rtB ->
Subsystempi2delay_a , & _rtDW -> Subsystempi2delay_a ) ; if ( _rtB ->
B_311_411_0_o != 0 ) { _rtB -> B_311_3872_0 [ 0 ] = _rtB -> Subsystem1_f .
B_241_0_0 ; _rtB -> B_311_3872_0 [ 1 ] = _rtB -> Subsystem1_f . B_241_1_0 ; }
else { _rtB -> B_311_3872_0 [ 0 ] = _rtB -> Subsystempi2delay_a . B_240_0_0 ;
_rtB -> B_311_3872_0 [ 1 ] = _rtB -> Subsystempi2delay_a . B_240_1_0 ; } if (
_rtDW -> Integ4_SYSTEM_ENABLE_c3 != 0 ) { _rtB -> B_311_3873_0 = _rtDW ->
Integ4_DSTATE_ce ; } else { _rtB -> B_311_3873_0 = _rtP -> P_2983 * _rtB ->
B_311_3872_0 [ 1 ] + _rtDW -> Integ4_DSTATE_ce ; } if ( _rtB -> B_311_3068_0
> _rtP -> P_2985 ) { rtb_B_299_3_0 = _rtP -> P_2985 ; } else if ( _rtB ->
B_311_3068_0 < _rtP -> P_2986 ) { rtb_B_299_3_0 = _rtP -> P_2986 ; } else {
rtb_B_299_3_0 = _rtB -> B_311_3068_0 ; } rtb_B_311_3086_0 = 1.0 /
rtb_B_299_3_0 / 2.0e-5 ; rtb_B_311_3087_0 = muDoubleScalarCeil (
rtb_B_311_3086_0 ) ; _rtB -> B_311_3877_0 = _rtP -> P_2987 * rtb_B_311_3087_0
; ssCallAccelRunBlock ( S , 311 , 3878 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_408_0 ) { rtb_B_311_3087_0 =
rtb_B_311_3086_0 - rtb_B_311_3087_0 ; _rtB -> B_311_3884_0 = ( ( _rtB ->
B_311_3872_0 [ 1 ] - _rtDW -> UnitDelay_DSTATE_jz ) * rtb_B_311_3087_0 * _rtP
-> P_539 + _rtB -> B_311_3872_0 [ 1 ] ) * ( rtb_B_311_3087_0 /
rtb_B_311_3086_0 ) + ( _rtB -> B_311_3873_0 - _rtB -> B_311_3878_0 ) * _rtB
-> B_311_3068_0 ; } else { _rtB -> B_311_3884_0 = _rtDW ->
UnitDelay1_DSTATE_ky ; } _rtB -> B_311_3885_0 = _rtB -> B_311_3884_0 * _rtB
-> B_292_32_0 ; _rtDW -> DiscreteDerivative_tmp_m = ( _rtB -> B_311_3885_0 -
_rtP -> P_2999 [ 1 ] * _rtDW -> DiscreteDerivative_states_l ) / _rtP ->
P_2999 [ 0 ] ; rtb_B_311_35_0 = ( _rtP -> P_3005 * _rtB -> B_311_3885_0 +
_rtDW -> DiscreteTimeIntegrator_DSTATE_o5 ) + ( _rtP -> P_2998 [ 0 ] * _rtDW
-> DiscreteDerivative_tmp_m + _rtP -> P_2998 [ 1 ] * _rtDW ->
DiscreteDerivative_states_l ) ; if ( rtb_B_311_35_0 > _rtP -> P_3006 ) { _rtB
-> B_311_3890_0 = _rtP -> P_3006 ; } else if ( rtb_B_311_35_0 < _rtP ->
P_3007 ) { _rtB -> B_311_3890_0 = _rtP -> P_3007 ; } else { _rtB ->
B_311_3890_0 = rtb_B_311_35_0 ; } rtb_B_311_3886_0 = _rtP -> P_3008 * _rtB ->
B_311_3890_0 ; rtb_B_311_251_0 = rtb_B_311_3886_0 - _rtDW -> PrevY_d ; if (
rtb_B_311_251_0 > _rtP -> P_3009 ) { rtb_B_311_3886_0 = _rtDW -> PrevY_d +
_rtP -> P_3009 ; } else { if ( rtb_B_311_251_0 < _rtP -> P_3010 ) {
rtb_B_311_3886_0 = _rtDW -> PrevY_d + _rtP -> P_3010 ; } } _rtDW -> PrevY_d =
rtb_B_311_3886_0 ; _rtB -> B_311_3902_0 = ( _rtP -> P_3013 * _rtDW ->
Delay_x1_DSTATE_d4 + _rtP -> P_3015 * _rtDW -> Delay_x2_DSTATE_c ) + _rtP ->
P_3018 * rtb_B_311_3886_0 ; _rtB -> B_311_3904_0 = ( _rtP -> P_3016 * _rtDW
-> Delay_x1_DSTATE_d4 + _rtP -> P_3017 * _rtDW -> Delay_x2_DSTATE_c ) + _rtP
-> P_3019 * rtb_B_311_3886_0 ; _rtB -> B_311_3909_0 = ( _rtP -> P_3021 *
_rtDW -> Delay_x1_DSTATE_d4 + _rtP -> P_3022 * _rtDW -> Delay_x2_DSTATE_c ) +
_rtP -> P_3020 * rtb_B_311_3886_0 ; _rtB -> B_311_3910_0 = _rtP -> P_3023 *
rtb_B_311_3815_0 ; B_311_2829_0_idx_0 = _rtP -> P_3024 * _rtB -> B_311_152_0
[ 37 ] ; B_311_3006_0_idx_1 = _rtP -> P_3024 * _rtB -> B_311_152_0 [ 38 ] ;
_rtB -> B_311_3912_0 = ( ( 2.0 * B_311_2829_0_idx_0 + B_311_3006_0_idx_1 ) *
rtb_B_311_4_1 + 1.7320508075688772 * B_311_3006_0_idx_1 * rtb_B_311_4_0 ) *
0.33333333333333331 ; _rtB -> B_311_3913_0 = ( ( 2.0 * B_311_2829_0_idx_0 +
B_311_3006_0_idx_1 ) * rtb_B_311_4_0 + - 1.7320508075688772 *
B_311_3006_0_idx_1 * rtb_B_311_4_1 ) * 0.33333333333333331 ; if (
ssGetTaskTime ( S , 2 ) >= _rtP -> P_3026 ) { rtb_B_300_0_0 [ 0 ] = _rtB ->
B_311_3912_0 + _rtDW -> voltages_DSTATE [ 0 ] ; rtb_B_300_0_0 [ 1 ] = _rtB ->
B_311_3913_0 + _rtDW -> voltages_DSTATE [ 1 ] ; rtb_B_300_0_0 [ 2 ] = _rtB ->
B_311_3910_0 + _rtDW -> voltages_DSTATE [ 2 ] ; rtb_B_300_0_0 [ 3 ] = _rtB ->
B_311_422_0 [ 0 ] + _rtDW -> voltages_DSTATE [ 3 ] ; rtb_B_300_0_0 [ 4 ] =
_rtB -> B_311_422_0 [ 1 ] + _rtDW -> voltages_DSTATE [ 4 ] ; memcpy ( &
rtb_B_300_4_0 [ 0 ] , & _rtB -> B_311_419_0 [ 0 ] , 25U * sizeof ( real_T ) )
; rtb_B_300_4_0 [ 5 ] = rtb_B_311_3012_0 ; rtb_B_300_4_0 [ 1 ] = _rtP ->
P_565 * rtb_B_311_3012_0 ; rtb_B_311_44_0 = ( _rtB -> B_311_415_0 >= _rtP ->
P_564 ) ; for ( i = 0 ; i < 25 ; i ++ ) { if ( rtb_B_311_44_0 ) {
rtb_B_299_3_0 = _rtB -> B_299_18_0 [ i ] ; } else { rtb_B_299_3_0 = _rtB ->
B_311_418_0 [ i ] ; } rtb_B_311_4_0 = ( ( 0.0 - rtb_B_300_4_0 [ i ] ) -
rtb_B_299_3_0 ) * _rtP -> P_566 ; B_300_1_0 [ i ] = _rtB -> B_311_421_0 [ i ]
- rtb_B_311_4_0 ; rtb_B_300_4_0 [ i ] = rtb_B_311_4_0 ; } rt_invd5x5_snf (
B_300_1_0 , rtb_B_300_1_0 ) ; for ( i = 0 ; i < 25 ; i ++ ) { rtb_B_300_4_0 [
i ] += _rtB -> B_311_421_0 [ i ] ; } for ( i = 0 ; i < 5 ; i ++ ) {
B_299_15_0 [ i ] = 0.0 ; tmp [ i ] = 0.0 ; for ( i_0 = 0 ; i_0 < 5 ; i_0 ++ )
{ B_300_1_0 [ i + 5 * i_0 ] = 0.0 ; for ( i_1 = 0 ; i_1 < 5 ; i_1 ++ ) {
B_300_1_0 [ i + 5 * i_0 ] += rtb_B_300_1_0 [ 5 * i_1 + i ] * rtb_B_300_4_0 [
5 * i_0 + i_1 ] ; } B_299_15_0 [ i ] += B_300_1_0 [ 5 * i_0 + i ] *
rtb_B_311_5_0 [ i_0 ] ; tmp [ i ] += rtb_B_300_1_0 [ 5 * i_0 + i ] * _rtP ->
P_567 * rtb_B_300_0_0 [ i_0 ] ; } _rtB -> B_311_3917_0 [ i ] = B_299_15_0 [ i
] + tmp [ i ] ; } } else { for ( i = 0 ; i < 5 ; i ++ ) { _rtB ->
B_311_3917_0 [ i ] = rtb_B_311_5_0 [ i ] ; } } _rtB -> B_311_3922_0 = ( (
_rtP -> P_3027 * rtb_B_311_3090_0 / rtb_B_311_3012_0 - rtb_B_311_3082_0 ) -
_rtP -> P_3028 * rtb_B_311_3012_0 ) * _rtP -> P_3029 ; if ( _rtDW ->
Rotorspeeddeviationdw_SYSTEM_ENABLE != 0 ) { _rtB -> B_311_3923_0 = _rtDW ->
Rotorspeeddeviationdw_DSTATE ; } else { _rtB -> B_311_3923_0 = _rtP -> P_3030
* _rtB -> B_311_3922_0 + _rtDW -> Rotorspeeddeviationdw_DSTATE ; } _rtB ->
B_311_3924_0 = _rtP -> P_3032 * rtb_B_311_3001_0 ; _rtB -> B_311_3925_0 =
_rtP -> P_3033 * rtb_B_311_3012_0 ; if ( _rtB -> B_311_425_0_m >= _rtP ->
P_3037 ) { rtb_B_311_4_0 = _rtB -> B_311_430_0 ; } else { rtb_B_311_4_0 =
rt_Lookup ( _rtP -> P_3035 , 4 , ssGetTaskTime ( S , 2 ) , _rtP -> P_3036 ) ;
} if ( _rtP -> P_3034 >= _rtP -> P_3038 ) { if ( _rtB -> B_311_426_0 >= _rtP
-> P_338 ) { _rtB -> B_311_4018_0 = rtb_B_311_4_0 ; } else { _rtB ->
B_311_4018_0 = _rtB -> B_311_429_0 ; } } else { _rtB -> B_311_4018_0 =
rt_Lookup ( _rtP -> P_336 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_337 ) ; }
if ( _rtP -> P_3039 >= _rtP -> P_3040 ) { if ( _rtB -> B_311_427_0 >= _rtP ->
P_341 ) { _rtB -> B_311_4026_0 = rtb_B_311_4_0 ; } else { _rtB ->
B_311_4026_0 = _rtB -> B_311_429_0 ; } } else { _rtB -> B_311_4026_0 =
rt_Lookup ( _rtP -> P_339 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_340 ) ; }
if ( _rtP -> P_3041 >= _rtP -> P_3042 ) { if ( _rtB -> B_311_428_0 >= _rtP ->
P_344 ) { _rtB -> B_311_4034_0 = rtb_B_311_4_0 ; } else { _rtB ->
B_311_4034_0 = _rtB -> B_311_429_0 ; } } else { _rtB -> B_311_4034_0 =
rt_Lookup ( _rtP -> P_342 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_343 ) ; }
rtb_B_311_3886_0 = _rtP -> P_3043 ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if (
i != 0 ) { _rtDW -> Step1_MODE = ( ssGetTaskTime ( S , 1 ) >= _rtP -> P_3044
) ; _rtDW -> Step2_MODE_l = ( ssGetTaskTime ( S , 1 ) >= _rtP -> P_3047 ) ; i
= ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { if ( _rtDW -> Step1_MODE == 1
) { rtb_B_299_3_0 = _rtP -> P_3046 ; } else { rtb_B_299_3_0 = _rtP -> P_3045
; } if ( _rtDW -> Step2_MODE_l == 1 ) { B_311_401_0_idx_1 = _rtP -> P_3049 ;
} else { B_311_401_0_idx_1 = _rtP -> P_3048 ; } _rtDW ->
RateTransition8_Buffer = rtb_B_299_3_0 + B_311_401_0_idx_1 ; } } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_4062_0 = _rtDW ->
RateTransition8_Buffer ; if ( _rtB -> B_311_431_0 >= _rtP -> P_3052 ) {
rtb_B_311_4_0 = _rtB -> B_311_4062_0 ; } else { rtb_B_311_4_0 = rt_Lookup (
_rtP -> P_3050 , 4 , ssGetTaskTime ( S , 2 ) , _rtP -> P_3051 ) ; } if (
rtb_B_311_3886_0 >= _rtP -> P_3053 ) { if ( _rtB -> B_311_432_0_m >= _rtP ->
P_133 ) { _rtB -> B_311_4069_0 = rtb_B_311_4_0 ; } else { _rtB ->
B_311_4069_0 = _rtB -> B_311_435_0 ; } } else { _rtB -> B_311_4069_0 =
rt_Lookup ( _rtP -> P_131 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_132 ) ; }
if ( _rtP -> P_3054 >= _rtP -> P_3055 ) { if ( _rtB -> B_311_433_0 >= _rtP ->
P_136 ) { _rtB -> B_311_4077_0 = rtb_B_311_4_0 ; } else { _rtB ->
B_311_4077_0 = _rtB -> B_311_435_0 ; } } else { _rtB -> B_311_4077_0 =
rt_Lookup ( _rtP -> P_134 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_135 ) ; }
if ( _rtP -> P_3056 >= _rtP -> P_3057 ) { if ( _rtB -> B_311_434_0 >= _rtP ->
P_139 ) { _rtB -> B_311_4085_0 = rtb_B_311_4_0 ; } else { _rtB ->
B_311_4085_0 = _rtB -> B_311_435_0 ; } } else { _rtB -> B_311_4085_0 =
rt_Lookup ( _rtP -> P_137 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_138 ) ; }
} i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_4096_0 [ 0
] = _rtDW -> UnitDelay_DSTATE_an [ 0 ] ; B_311_4097_0_idx_0 = _rtDW ->
UnitDelay_DSTATE_flb [ 0 ] ; _rtB -> B_311_4096_0 [ 1 ] = _rtDW ->
UnitDelay_DSTATE_an [ 1 ] ; B_311_4097_0_idx_1 = _rtDW ->
UnitDelay_DSTATE_flb [ 1 ] ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0
) { _rtB -> B_311_4101_0 [ 0 ] = _rtP -> P_3060 * _rtB -> B_311_152_0 [ 103 ]
* _rtP -> P_3063 ; _rtB -> B_311_4101_0 [ 1 ] = _rtP -> P_3061 * _rtB ->
B_311_152_0 [ 104 ] * _rtP -> P_3063 ; _rtB -> B_311_4101_0 [ 2 ] = _rtP ->
P_3062 * _rtB -> B_311_152_0 [ 105 ] * _rtP -> P_3063 ; } i = ssIsSampleHit (
S , 3 , 0 ) ; if ( i != 0 ) { rtb_B_311_50_0 = _rtP -> P_3064 * _rtB ->
B_311_4101_0 [ 0 ] ; rtb_B_311_68_0 = _rtP -> P_3064 * _rtB -> B_311_4101_0 [
1 ] ; rtb_B_311_51_0 = _rtP -> P_3064 * _rtB -> B_311_4101_0 [ 2 ] ;
rtb_B_311_4106_0 = ( 0.0 - _rtDW -> UnitDelay2_DSTATE_nc [ 0 ] * _rtB ->
B_311_461_0 ) - _rtDW -> UnitDelay2_DSTATE_nc [ 1 ] * _rtB -> B_311_462_0 ;
rtb_B_311_4_0 = _rtDW -> UnitDelay2_DSTATE_nc [ 0 ] * _rtB -> B_311_462_0 -
_rtDW -> UnitDelay2_DSTATE_nc [ 1 ] * _rtB -> B_311_461_0 ; _rtB ->
B_311_4112_0 = ( ( ( 0.0 - rtb_B_311_4106_0 ) - _rtDW -> UnitDelay2_DSTATE_nc
[ 0 ] ) * rtb_B_311_51_0 + ( rtb_B_311_50_0 * _rtDW -> UnitDelay2_DSTATE_nc [
0 ] + rtb_B_311_68_0 * rtb_B_311_4106_0 ) ) * 0.66666666666666663 ; _rtB ->
B_311_4113_0 = ( ( ( 0.0 - rtb_B_311_4_0 ) - _rtDW -> UnitDelay2_DSTATE_nc [
1 ] ) * rtb_B_311_51_0 + ( rtb_B_311_50_0 * _rtDW -> UnitDelay2_DSTATE_nc [ 1
] + rtb_B_311_68_0 * rtb_B_311_4_0 ) ) * 0.66666666666666663 ; if ( _rtP ->
P_4290 == 1 ) { _rtB -> B_311_4114_0 [ 0 ] = B_311_4097_0_idx_0 ; _rtB ->
B_311_4114_0 [ 1 ] = B_311_4097_0_idx_1 ; } else { _rtB -> B_311_4114_0 [ 0 ]
= _rtB -> B_311_4112_0 ; _rtB -> B_311_4114_0 [ 1 ] = _rtB -> B_311_4113_0 ;
} rtb_B_311_4_0 = _rtB -> B_311_4096_0 [ 0 ] - _rtB -> B_311_4114_0 [ 0 ] ;
B_311_4115_0_idx_1 = _rtB -> B_311_4096_0 [ 1 ] - _rtB -> B_311_4114_0 [ 1 ]
; rtb_B_311_4106_0 = _rtB -> B_311_440_0 * rtb_B_311_4_0 ; _rtB ->
B_311_4117_0 = rtb_B_311_4_0 / _rtB -> B_311_442_0_h ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Step1_MODE_p = ( ssGetTaskTime ( S , 1
) >= _rtP -> P_3066 ) ; if ( _rtDW -> Step1_MODE_p == 1 ) { _rtB ->
B_311_4118_0 = _rtP -> P_3068 ; } else { _rtB -> B_311_4118_0 = _rtP ->
P_3067 ; } } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { _rtB ->
B_311_4120_0 = ( ( _rtB -> B_311_4062_0 != 0.0 ) && ( ! ( _rtB ->
B_311_4118_0 != 0.0 ) ) ) ; rtb_B_311_3375_0 = ! _rtB -> B_311_4120_0 ; _rtB
-> B_311_4122_0 = ! rtb_B_311_3375_0 ; B_311_4123_0_idx_0 = _rtDW ->
UnitDelay1_DSTATE_gm [ 0 ] ; B_311_4123_0_idx_1 = _rtDW ->
UnitDelay1_DSTATE_gm [ 1 ] ; _rtB -> B_311_4124_0 = _rtDW ->
UnitDelay1_DSTATE_gm [ 0 ] - rtb_B_311_4106_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_IC_LOADING_kw != 0 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_c = _rtB -> B_311_4124_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_c >= _rtP -> P_3071 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_c = _rtP -> P_3071 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_c <= _rtP -> P_3072 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_c = _rtP -> P_3072 ; } } _rtB -> B_311_4125_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_c ; } else if ( _rtB -> B_311_4122_0
|| ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_d != 0 ) ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_c = _rtB -> B_311_4124_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_c >= _rtP -> P_3071 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_c = _rtP -> P_3071 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_c <= _rtP -> P_3072 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_c = _rtP -> P_3072 ; } } _rtB -> B_311_4125_0 =
_rtDW -> DiscreteTimeIntegrator_DSTATE_c ; } else { _rtB -> B_311_4125_0 =
_rtP -> P_3070 * _rtB -> B_311_4117_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_c ; } if ( _rtB -> B_311_4125_0 >= _rtP ->
P_3071 ) { _rtB -> B_311_4125_0 = _rtP -> P_3071 ; } else { if ( _rtB ->
B_311_4125_0 <= _rtP -> P_3072 ) { _rtB -> B_311_4125_0 = _rtP -> P_3072 ; }
} rtb_B_311_35_0 = rtb_B_311_4106_0 + _rtB -> B_311_4125_0 ; if (
rtb_B_311_35_0 > _rtP -> P_3073 ) { _rtB -> B_311_4127_0 = _rtP -> P_3073 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_3074 ) { _rtB -> B_311_4127_0 = _rtP ->
P_3074 ; } else { _rtB -> B_311_4127_0 = rtb_B_311_35_0 ; } rtb_B_311_4106_0
= _rtB -> B_311_443_0 * B_311_4115_0_idx_1 ; _rtB -> B_311_4129_0 =
B_311_4115_0_idx_1 / _rtB -> B_311_445_0 ; _rtB -> B_311_4130_0 = !
rtb_B_311_3375_0 ; _rtB -> B_311_4131_0 = _rtDW -> UnitDelay1_DSTATE_gm [ 1 ]
- rtb_B_311_4106_0 ; if ( _rtDW -> DiscreteTimeIntegrator_IC_LOADING_nm != 0
) { _rtDW -> DiscreteTimeIntegrator_DSTATE_m4 = _rtB -> B_311_4131_0 ; if (
_rtDW -> DiscreteTimeIntegrator_DSTATE_m4 >= _rtP -> P_3076 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_m4 = _rtP -> P_3076 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_m4 <= _rtP -> P_3077 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_m4 = _rtP -> P_3077 ; } } _rtB -> B_311_4132_0
= _rtDW -> DiscreteTimeIntegrator_DSTATE_m4 ; } else if ( _rtB ->
B_311_4130_0 || ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_ii != 0 ) )
{ _rtDW -> DiscreteTimeIntegrator_DSTATE_m4 = _rtB -> B_311_4131_0 ; if (
_rtDW -> DiscreteTimeIntegrator_DSTATE_m4 >= _rtP -> P_3076 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_m4 = _rtP -> P_3076 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_m4 <= _rtP -> P_3077 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_m4 = _rtP -> P_3077 ; } } _rtB -> B_311_4132_0
= _rtDW -> DiscreteTimeIntegrator_DSTATE_m4 ; } else { _rtB -> B_311_4132_0 =
_rtP -> P_3075 * _rtB -> B_311_4129_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_m4 ; } if ( _rtB -> B_311_4132_0 >= _rtP ->
P_3076 ) { _rtB -> B_311_4132_0 = _rtP -> P_3076 ; } else { if ( _rtB ->
B_311_4132_0 <= _rtP -> P_3077 ) { _rtB -> B_311_4132_0 = _rtP -> P_3077 ; }
} rtb_B_311_35_0 = rtb_B_311_4106_0 + _rtB -> B_311_4132_0 ; if (
rtb_B_311_35_0 > _rtP -> P_3078 ) { _rtB -> B_311_4134_0 = _rtP -> P_3078 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_3079 ) { _rtB -> B_311_4134_0 = _rtP ->
P_3079 ; } else { _rtB -> B_311_4134_0 = rtb_B_311_35_0 ; }
ssCallAccelRunBlock ( S , 311 , 4135 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_311_4137_0 = _rtP -> P_3081 * _rtDW -> UnitDelay_DSTATE_ir ; } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_4141_0 [ 0 ] =
_rtP -> P_3082 * _rtB -> B_311_152_0 [ 106 ] * _rtP -> P_3085 ; _rtB ->
B_311_4141_0 [ 1 ] = _rtP -> P_3083 * _rtB -> B_311_152_0 [ 107 ] * _rtP ->
P_3085 ; _rtB -> B_311_4141_0 [ 2 ] = _rtP -> P_3084 * _rtB -> B_311_152_0 [
108 ] * _rtP -> P_3085 ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) {
_rtB -> B_311_1589_0 [ 0 ] = _rtP -> P_3086 * _rtB -> B_311_4141_0 [ 0 ] ;
_rtB -> B_311_1589_0 [ 1 ] = _rtP -> P_3086 * _rtB -> B_311_4141_0 [ 1 ] ;
_rtB -> B_311_1589_0 [ 2 ] = _rtP -> P_3086 * _rtB -> B_311_4141_0 [ 2 ] ;
B_311_4097_0_idx_1 = _rtDW -> UnitDelay_DSTATE_c4 [ 1 ] ; rtb_B_311_4106_0 =
( 0.0 - _rtDW -> UnitDelay_DSTATE_c4 [ 0 ] * _rtB -> B_311_457_0 ) - _rtDW ->
UnitDelay_DSTATE_c4 [ 1 ] * _rtB -> B_311_458_0_k ; rtb_B_311_4_0 = _rtDW ->
UnitDelay_DSTATE_c4 [ 0 ] * _rtB -> B_311_458_0_k - _rtDW ->
UnitDelay_DSTATE_c4 [ 1 ] * _rtB -> B_311_457_0 ; rtb_B_311_3082_0 = ( 0.0 -
rtb_B_311_4_0 ) - _rtDW -> UnitDelay_DSTATE_c4 [ 1 ] ; _rtB -> B_311_4152_0 =
( ( ( 0.0 - rtb_B_311_4106_0 ) - _rtDW -> UnitDelay_DSTATE_c4 [ 0 ] ) * _rtB
-> B_311_1589_0 [ 2 ] + ( _rtB -> B_311_1589_0 [ 0 ] * _rtDW ->
UnitDelay_DSTATE_c4 [ 0 ] + _rtB -> B_311_1589_0 [ 1 ] * rtb_B_311_4106_0 ) )
* 0.66666666666666663 ; { B_311_4153_0 = ( _rtP -> P_3090 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_pd [ 0 ] + ( _rtP -> P_3090 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_pd [ 1 ] ; B_311_4153_0 += _rtP -> P_3091 * _rtB ->
B_311_4152_0 ; } _rtB -> B_311_4154_0 = ( ( _rtB -> B_311_1589_0 [ 0 ] *
B_311_4097_0_idx_1 + _rtB -> B_311_1589_0 [ 1 ] * rtb_B_311_4_0 ) + _rtB ->
B_311_1589_0 [ 2 ] * rtb_B_311_3082_0 ) * 0.66666666666666663 ; {
B_311_4155_0 = ( _rtP -> P_3095 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_n [ 0 ] + ( _rtP -> P_3095 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_n [ 1 ] ; B_311_4155_0 += _rtP -> P_3096 * _rtB ->
B_311_4154_0 ; } B_311_4097_0_idx_1 = B_311_4153_0 * B_311_4153_0 +
B_311_4155_0 * B_311_4155_0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if (
_rtDW -> Sqrt_DWORK1_l != 0 ) { ssSetBlockStateForSolverChangedAtMajorStep (
S ) ; _rtDW -> Sqrt_DWORK1_l = 0 ; } _rtB -> B_311_4159_0 =
muDoubleScalarSqrt ( B_311_4097_0_idx_1 ) ; } else if ( B_311_4097_0_idx_1 <
0.0 ) { _rtB -> B_311_4159_0 = - muDoubleScalarSqrt ( muDoubleScalarAbs (
B_311_4097_0_idx_1 ) ) ; _rtDW -> Sqrt_DWORK1_l = 1 ; } else { _rtB ->
B_311_4159_0 = muDoubleScalarSqrt ( B_311_4097_0_idx_1 ) ; } } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_4163_0 [ 0 ] =
_rtP -> P_3098 * _rtB -> B_311_152_0 [ 188 ] * _rtP -> P_3101 ; _rtB ->
B_311_4163_0 [ 1 ] = _rtP -> P_3099 * _rtB -> B_311_152_0 [ 189 ] * _rtP ->
P_3101 ; _rtB -> B_311_4163_0 [ 2 ] = _rtP -> P_3100 * _rtB -> B_311_152_0 [
190 ] * _rtP -> P_3101 ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) {
_rtB -> B_311_1571_0 [ 0 ] = _rtP -> P_3102 * _rtB -> B_311_4163_0 [ 0 ] ;
_rtB -> B_311_1571_0 [ 1 ] = _rtP -> P_3102 * _rtB -> B_311_4163_0 [ 1 ] ;
_rtB -> B_311_1571_0 [ 2 ] = _rtP -> P_3102 * _rtB -> B_311_4163_0 [ 2 ] ;
B_311_4097_0_idx_1 = ( 0.0 - _rtDW -> UnitDelay1_DSTATE_hvi [ 0 ] * _rtB ->
B_311_459_0_p ) - _rtDW -> UnitDelay1_DSTATE_hvi [ 1 ] * _rtB -> B_311_460_0
; rtb_B_311_4106_0 = _rtDW -> UnitDelay1_DSTATE_hvi [ 0 ] * _rtB ->
B_311_460_0 - _rtDW -> UnitDelay1_DSTATE_hvi [ 1 ] * _rtB -> B_311_459_0_p ;
_rtB -> B_311_4174_0 = ( ( ( 0.0 - B_311_4097_0_idx_1 ) - _rtDW ->
UnitDelay1_DSTATE_hvi [ 0 ] ) * _rtB -> B_311_1571_0 [ 2 ] + ( _rtB ->
B_311_1571_0 [ 0 ] * _rtDW -> UnitDelay1_DSTATE_hvi [ 0 ] + _rtB ->
B_311_1571_0 [ 1 ] * B_311_4097_0_idx_1 ) ) * 0.66666666666666663 ; _rtB ->
B_311_4175_0 = ( ( ( 0.0 - rtb_B_311_4106_0 ) - _rtDW ->
UnitDelay1_DSTATE_hvi [ 1 ] ) * _rtB -> B_311_1571_0 [ 2 ] + ( _rtB ->
B_311_1571_0 [ 0 ] * _rtDW -> UnitDelay1_DSTATE_hvi [ 1 ] + _rtB ->
B_311_1571_0 [ 1 ] * rtb_B_311_4106_0 ) ) * 0.66666666666666663 ; _rtB ->
B_311_4176_0 [ 0 ] = _rtP -> P_3104 [ 0 ] * _rtB -> B_311_4174_0 ; _rtB ->
B_311_4176_0 [ 1 ] = _rtP -> P_3104 [ 1 ] * _rtB -> B_311_4175_0 ; } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_4177_0 = _rtP ->
P_3105 ; _rtB -> B_311_4178_0 = _rtP -> P_3106 ; } i = ssIsSampleHit ( S , 3
, 0 ) ; if ( i != 0 ) { _rtB -> B_311_4179_0 [ 0 ] = _rtB -> B_311_4177_0 -
_rtB -> B_311_4137_0 ; _rtB -> B_311_4179_0 [ 1 ] = _rtB -> B_311_4178_0 -
_rtB -> B_311_4159_0 ; _rtB -> B_311_4179_0 [ 2 ] = _rtB -> B_311_468_0 [ 0 ]
- _rtB -> B_311_4176_0 [ 0 ] ; _rtB -> B_311_4179_0 [ 3 ] = _rtB ->
B_311_468_0 [ 1 ] - _rtB -> B_311_4176_0 [ 1 ] ; { B_311_4180_0 = ( _rtP ->
P_3109 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_lg [ 0 ] + ( _rtP ->
P_3109 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_lg [ 1 ] ; B_311_4180_0
+= _rtP -> P_3110 * _rtB -> B_311_4179_0 [ 2 ] ; } rtb_B_311_4181_0 =
B_311_4180_0 * _rtB -> B_311_436_0 ; { B_311_4182_0 = ( _rtP -> P_3114 [ 0 ]
) * _rtDW -> DiscreteStateSpace_DSTATE_b2 [ 0 ] + ( _rtP -> P_3114 [ 1 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_b2 [ 1 ] ; B_311_4182_0 += _rtP -> P_3115
* _rtB -> B_311_4179_0 [ 3 ] ; } if ( B_311_4182_0 > _rtP -> P_3118 ) {
rtb_B_299_3_0 = B_311_4182_0 - _rtP -> P_3118 ; } else if ( B_311_4182_0 >=
_rtP -> P_3117 ) { rtb_B_299_3_0 = 0.0 ; } else { rtb_B_299_3_0 =
B_311_4182_0 - _rtP -> P_3117 ; } rtb_B_311_4186_0 = B_311_4182_0 * _rtB ->
B_311_437_0 + rtb_B_299_3_0 * _rtB -> B_311_469_0 ; { B_311_4187_0 = _rtP ->
P_3121 * _rtDW -> DiscreteStateSpace_DSTATE_oa ; B_311_4187_0 += _rtP ->
P_3122 * _rtB -> B_311_4179_0 [ 0 ] ; } rtb_B_311_4188_0 = B_311_4187_0 *
_rtB -> B_311_436_0 ; { B_311_4189_0 = _rtP -> P_3126 * _rtDW ->
DiscreteStateSpace_DSTATE_ow ; B_311_4189_0 += _rtP -> P_3127 * _rtB ->
B_311_4179_0 [ 1 ] ; } rtb_B_311_4190_0 = B_311_4189_0 * _rtB -> B_311_437_0
; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_4191_0 =
_rtP -> P_3129 ; _rtB -> B_311_4192_0 = _rtP -> P_3130 ; } i = ssIsSampleHit
( S , 3 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_4193_0 [ 0 ] = rtb_B_311_4181_0
+ _rtB -> B_311_4177_0 ; _rtB -> B_311_4193_0 [ 1 ] = rtb_B_311_4186_0 + _rtB
-> B_311_4178_0 ; _rtB -> B_311_4193_0 [ 2 ] = rtb_B_311_4188_0 + _rtB ->
B_311_4191_0 ; _rtB -> B_311_4193_0 [ 3 ] = rtb_B_311_4190_0 + _rtB ->
B_311_4192_0 ; if ( _rtB -> B_311_4193_0 [ 1 ] > _rtP -> P_3131 ) { _rtB ->
B_311_4194_0 = _rtP -> P_3131 ; } else if ( _rtB -> B_311_4193_0 [ 1 ] < _rtP
-> P_3132 ) { _rtB -> B_311_4194_0 = _rtP -> P_3132 ; } else { _rtB ->
B_311_4194_0 = _rtB -> B_311_4193_0 [ 1 ] ; } if ( _rtB -> B_311_4120_0 ) {
_rtB -> B_311_4196_0 [ 0 ] = B_311_4123_0_idx_0 ; _rtB -> B_311_4196_0 [ 1 ]
= B_311_4123_0_idx_1 ; } else { B_311_4123_0_idx_0 = _rtB -> B_311_4194_0 -
_rtB -> B_311_4096_0 [ 0 ] ; B_311_4123_0_idx_1 = _rtB -> B_311_438_0 - _rtB
-> B_311_4096_0 [ 1 ] ; if ( B_311_4123_0_idx_0 > _rtB -> B_311_446_0 [ 0 ] )
{ B_311_4123_0_idx_0 = _rtB -> B_311_446_0 [ 0 ] ; } else { if (
B_311_4123_0_idx_0 < _rtB -> B_311_447_0 [ 0 ] ) { B_311_4123_0_idx_0 = _rtB
-> B_311_447_0 [ 0 ] ; } } _rtB -> B_311_4196_0 [ 0 ] = B_311_4123_0_idx_0 +
_rtB -> B_311_4096_0 [ 0 ] ; if ( B_311_4123_0_idx_1 > _rtB -> B_311_446_0 [
1 ] ) { B_311_4123_0_idx_1 = _rtB -> B_311_446_0 [ 1 ] ; } else { if (
B_311_4123_0_idx_1 < _rtB -> B_311_447_0 [ 1 ] ) { B_311_4123_0_idx_1 = _rtB
-> B_311_447_0 [ 1 ] ; } } _rtB -> B_311_4196_0 [ 1 ] = B_311_4123_0_idx_1 +
_rtB -> B_311_4096_0 [ 1 ] ; } B_311_4123_0_idx_0 = _rtDW ->
UnitDelay_DSTATE_e1 [ 0 ] ; B_311_4123_0_idx_1 = _rtDW -> UnitDelay_DSTATE_e1
[ 1 ] ; { B_311_4198_0 = ( _rtP -> P_3136 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_bt [ 0 ] + ( _rtP -> P_3136 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_bt [ 1 ] ; B_311_4198_0 += _rtP -> P_3137 * _rtB ->
B_311_4174_0 ; } { B_311_4199_0 = ( _rtP -> P_3141 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_fm [ 0 ] + ( _rtP -> P_3141 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_fm [ 1 ] ; B_311_4199_0 += _rtP -> P_3142 * _rtB ->
B_311_4175_0 ; } B_311_4097_0_idx_0 = B_311_4123_0_idx_0 - B_311_4198_0 ;
B_311_4097_0_idx_1 = B_311_4123_0_idx_1 - B_311_4199_0 ; rtb_B_311_4181_0 =
_rtB -> B_311_449_0_c * B_311_4097_0_idx_0 ; _rtB -> B_311_4202_0 =
B_311_4097_0_idx_0 / _rtB -> B_311_451_0 ; _rtB -> B_311_4203_0 = ! _rtB ->
B_311_4120_0 ; rtb_B_311_4_0 = _rtDW -> UnitDelay_DSTATE_ff [ 0 ] ;
B_311_4115_0_idx_1 = _rtDW -> UnitDelay_DSTATE_ff [ 1 ] ; { B_311_4205_0 = (
_rtP -> P_3147 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_h [ 0 ] + ( _rtP
-> P_3147 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_h [ 1 ] ; B_311_4205_0
+= _rtP -> P_3148 * _rtB -> B_311_4112_0 ; } { B_311_4206_0 = ( _rtP ->
P_3152 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_cw [ 0 ] + ( _rtP ->
P_3152 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_cw [ 1 ] ; B_311_4206_0
+= _rtP -> P_3153 * _rtB -> B_311_4113_0 ; } _rtB -> B_311_4209_0 = (
rtb_B_311_4_0 - _rtP -> P_3155 * B_311_4205_0 ) - rtb_B_311_4181_0 ; if (
_rtDW -> DiscreteTimeIntegrator_IC_LOADING_m != 0 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ei = _rtB -> B_311_4209_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_ei >= _rtP -> P_3157 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ei = _rtP -> P_3157 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_ei <= _rtP -> P_3158 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ei = _rtP -> P_3158 ; } } _rtB -> B_311_4210_0
= _rtDW -> DiscreteTimeIntegrator_DSTATE_ei ; } else if ( _rtB ->
B_311_4203_0 || ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_j != 0 ) ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE_ei = _rtB -> B_311_4209_0 ; if ( _rtDW
-> DiscreteTimeIntegrator_DSTATE_ei >= _rtP -> P_3157 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ei = _rtP -> P_3157 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_ei <= _rtP -> P_3158 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ei = _rtP -> P_3158 ; } } _rtB -> B_311_4210_0
= _rtDW -> DiscreteTimeIntegrator_DSTATE_ei ; } else { _rtB -> B_311_4210_0 =
_rtP -> P_3156 * _rtB -> B_311_4202_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_ei ; } if ( _rtB -> B_311_4210_0 >= _rtP ->
P_3157 ) { _rtB -> B_311_4210_0 = _rtP -> P_3157 ; } else { if ( _rtB ->
B_311_4210_0 <= _rtP -> P_3158 ) { _rtB -> B_311_4210_0 = _rtP -> P_3158 ; }
} rtb_B_311_35_0 = rtb_B_311_4181_0 + _rtB -> B_311_4210_0 ; if (
rtb_B_311_35_0 > _rtP -> P_3159 ) { _rtB -> B_311_4212_0 = _rtP -> P_3159 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_3160 ) { _rtB -> B_311_4212_0 = _rtP ->
P_3160 ; } else { _rtB -> B_311_4212_0 = rtb_B_311_35_0 ; } rtb_B_311_4181_0
= _rtB -> B_311_452_0 * B_311_4097_0_idx_1 ; _rtB -> B_311_4214_0 =
B_311_4097_0_idx_1 / _rtB -> B_311_454_0 ; _rtB -> B_311_4215_0 = ! _rtB ->
B_311_4120_0 ; _rtB -> B_311_4216_0 = ( B_311_4115_0_idx_1 - _rtP -> P_3155 *
B_311_4206_0 ) - rtb_B_311_4181_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_IC_LOADING_pq != 0 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_dw = _rtB -> B_311_4216_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_dw >= _rtP -> P_3162 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_dw = _rtP -> P_3162 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_dw <= _rtP -> P_3163 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_dw = _rtP -> P_3163 ; } } _rtB -> B_311_4217_0
= _rtDW -> DiscreteTimeIntegrator_DSTATE_dw ; } else if ( _rtB ->
B_311_4215_0 || ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_l != 0 ) ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE_dw = _rtB -> B_311_4216_0 ; if ( _rtDW
-> DiscreteTimeIntegrator_DSTATE_dw >= _rtP -> P_3162 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_dw = _rtP -> P_3162 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_dw <= _rtP -> P_3163 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_dw = _rtP -> P_3163 ; } } _rtB -> B_311_4217_0
= _rtDW -> DiscreteTimeIntegrator_DSTATE_dw ; } else { _rtB -> B_311_4217_0 =
_rtP -> P_3161 * _rtB -> B_311_4214_0 + _rtDW ->
DiscreteTimeIntegrator_DSTATE_dw ; } if ( _rtB -> B_311_4217_0 >= _rtP ->
P_3162 ) { _rtB -> B_311_4217_0 = _rtP -> P_3162 ; } else { if ( _rtB ->
B_311_4217_0 <= _rtP -> P_3163 ) { _rtB -> B_311_4217_0 = _rtP -> P_3163 ; }
} rtb_B_311_35_0 = rtb_B_311_4181_0 + _rtB -> B_311_4217_0 ; if (
rtb_B_311_35_0 > _rtP -> P_3164 ) { _rtB -> B_311_4219_0 = _rtP -> P_3164 ; }
else if ( rtb_B_311_35_0 < _rtP -> P_3165 ) { _rtB -> B_311_4219_0 = _rtP ->
P_3165 ; } else { _rtB -> B_311_4219_0 = rtb_B_311_35_0 ; } _rtB ->
B_311_4221_0 [ 0 ] = _rtP -> P_3166 * B_311_4205_0 + _rtB -> B_311_4212_0 ;
_rtB -> B_311_4221_0 [ 1 ] = _rtP -> P_3166 * B_311_4206_0 + _rtB ->
B_311_4219_0 ; ssCallAccelRunBlock ( S , 311 , 4222 , SS_CALL_MDL_OUTPUTS ) ;
if ( _rtB -> B_311_4193_0 [ 2 ] > _rtB -> B_311_484_0 ) { rtb_B_311_4181_0 =
_rtB -> B_311_484_0 ; } else if ( _rtB -> B_311_4193_0 [ 2 ] < _rtB ->
B_311_485_0 ) { rtb_B_311_4181_0 = _rtB -> B_311_485_0 ; } else {
rtb_B_311_4181_0 = _rtB -> B_311_4193_0 [ 2 ] ; } rtb_B_311_47_0 =
rtb_B_311_4181_0 ; rtb_B_311_4181_0 = muDoubleScalarAtan2 ( _rtB ->
B_311_4193_0 [ 3 ] , rtb_B_311_4181_0 ) ; rtb_B_311_35_0 =
muDoubleScalarHypot ( rtb_B_311_47_0 , _rtB -> B_311_4193_0 [ 3 ] ) ;
rtb_B_311_47_0 = muDoubleScalarHypot ( rtb_B_311_47_0 , _rtB -> B_311_4193_0
[ 3 ] ) ; if ( rtb_B_311_35_0 > _rtP -> P_3167 ) { rtb_B_311_35_0 = _rtP ->
P_3167 ; } else { if ( rtb_B_311_35_0 < _rtP -> P_3168 ) { rtb_B_311_35_0 =
_rtP -> P_3168 ; } } _rtB -> B_311_4231_0 = rtb_B_311_35_0 *
muDoubleScalarCos ( rtb_B_311_4181_0 ) ; if ( rtb_B_311_47_0 > _rtP -> P_3167
) { rtb_B_311_47_0 = _rtP -> P_3167 ; } else { if ( rtb_B_311_47_0 < _rtP ->
P_3168 ) { rtb_B_311_47_0 = _rtP -> P_3168 ; } } _rtB -> B_311_4231_1 =
rtb_B_311_47_0 * muDoubleScalarSin ( rtb_B_311_4181_0 ) ; if ( ! _rtB ->
B_311_4120_0 ) { _rtB -> B_311_4233_0 [ 0 ] = _rtB -> B_311_4174_0 ; _rtB ->
B_311_4233_0 [ 1 ] = _rtB -> B_311_4175_0 ; } else { B_311_4097_0_idx_0 =
_rtB -> B_311_4231_0 - B_311_4123_0_idx_0 ; B_311_4097_0_idx_1 = _rtB ->
B_311_4231_1 - B_311_4123_0_idx_1 ; if ( B_311_4097_0_idx_0 > _rtB ->
B_311_455_0 [ 0 ] ) { B_311_4097_0_idx_0 = _rtB -> B_311_455_0 [ 0 ] ; } else
{ if ( B_311_4097_0_idx_0 < _rtB -> B_311_456_0 [ 0 ] ) { B_311_4097_0_idx_0
= _rtB -> B_311_456_0 [ 0 ] ; } } _rtB -> B_311_4233_0 [ 0 ] =
B_311_4097_0_idx_0 + B_311_4123_0_idx_0 ; if ( B_311_4097_0_idx_1 > _rtB ->
B_311_455_0 [ 1 ] ) { B_311_4097_0_idx_1 = _rtB -> B_311_455_0 [ 1 ] ; } else
{ if ( B_311_4097_0_idx_1 < _rtB -> B_311_456_0 [ 1 ] ) { B_311_4097_0_idx_1
= _rtB -> B_311_456_0 [ 1 ] ; } } _rtB -> B_311_4233_0 [ 1 ] =
B_311_4097_0_idx_1 + B_311_4123_0_idx_1 ; } _rtB -> B_311_4235_0 = ( _rtB ->
B_311_1589_0 [ 0 ] * _rtB -> B_311_1571_0 [ 0 ] + _rtB -> B_311_1589_0 [ 1 ]
* _rtB -> B_311_1571_0 [ 1 ] ) + _rtB -> B_311_1589_0 [ 2 ] * _rtB ->
B_311_1571_0 [ 2 ] ; _rtB -> B_311_4241_0 = ( ( ( _rtB -> B_311_1589_0 [ 1 ]
- _rtB -> B_311_1589_0 [ 2 ] ) * _rtB -> B_311_1571_0 [ 0 ] + ( _rtB ->
B_311_1589_0 [ 2 ] - _rtB -> B_311_1589_0 [ 0 ] ) * _rtB -> B_311_1571_0 [ 1
] ) + ( _rtB -> B_311_1589_0 [ 0 ] - _rtB -> B_311_1589_0 [ 1 ] ) * _rtB ->
B_311_1571_0 [ 2 ] ) * _rtP -> P_3169 ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_jqs != 0 ) { _rtB -> B_311_4242_0 [ 0 ] = _rtDW ->
Integ4_DSTATE_o5 [ 0 ] ; _rtB -> B_311_4242_0 [ 1 ] = _rtDW ->
Integ4_DSTATE_o5 [ 1 ] ; } else { _rtB -> B_311_4242_0 [ 0 ] = _rtP -> P_3170
* _rtB -> B_311_4235_0 + _rtDW -> Integ4_DSTATE_o5 [ 0 ] ; _rtB ->
B_311_4242_0 [ 1 ] = _rtP -> P_3170 * _rtB -> B_311_4241_0 + _rtDW ->
Integ4_DSTATE_o5 [ 1 ] ; } _rtB -> B_311_4243_0 = _rtP -> P_3172 ;
ssCallAccelRunBlock ( S , 311 , 4244 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 3 ) < _rtP -> P_3175 ) { rtb_B_299_3_0 = _rtP -> P_3176 ;
} else { rtb_B_299_3_0 = _rtP -> P_3177 ; } if ( rtb_B_299_3_0 >= _rtP ->
P_3179 ) { B_311_4123_0_idx_0 = ( _rtB -> B_311_4242_0 [ 0 ] - _rtB ->
B_311_4244_0 [ 0 ] ) * _rtP -> P_3173 + ( _rtP -> P_141 * _rtB ->
B_311_4235_0 - _rtP -> P_140 * _rtDW -> UnitDelay_DSTATE_no [ 0 ] ) ;
B_311_4123_0_idx_1 = ( _rtB -> B_311_4242_0 [ 1 ] - _rtB -> B_311_4244_0 [ 1
] ) * _rtP -> P_3173 + ( _rtP -> P_141 * _rtB -> B_311_4241_0 - _rtP -> P_140
* _rtDW -> UnitDelay_DSTATE_no [ 1 ] ) ; } else { B_311_4123_0_idx_0 = _rtP
-> P_3178 ; B_311_4123_0_idx_1 = _rtP -> P_3178 ; } _rtB -> B_311_4251_0 [ 0
] = _rtP -> P_3180 * B_311_4123_0_idx_0 ; _rtB -> B_311_4251_0 [ 1 ] = _rtP
-> P_3180 * B_311_4123_0_idx_1 ; ssCallAccelRunBlock ( S , 311 , 4252 ,
SS_CALL_MDL_OUTPUTS ) ; if ( _rtB -> B_311_4120_0 ) { _rtB -> B_311_4253_0 [
0 ] = _rtB -> B_311_4221_0 [ 0 ] ; _rtB -> B_311_4253_0 [ 1 ] = _rtB ->
B_311_4221_0 [ 1 ] ; } else { _rtB -> B_311_4253_0 [ 0 ] = _rtB ->
B_311_4127_0 ; _rtB -> B_311_4253_0 [ 1 ] = _rtB -> B_311_4134_0 ; }
ssCallAccelRunBlock ( S , 311 , 4254 , SS_CALL_MDL_OUTPUTS ) ;
B_311_4123_0_idx_0 = _rtDW -> UnitDelay1_DSTATE_pf ; _rtB -> B_311_4256_0 = !
( _rtB -> B_311_4062_0 != 0.0 ) ; _rtB -> B_311_4257_0 = _rtDW ->
UnitDelay1_DSTATE_c4 ; rtb_B_311_4181_0 = muDoubleScalarSin ( _rtB ->
B_311_4257_0 ) ; rtb_B_311_4186_0 = muDoubleScalarCos ( _rtB -> B_311_4257_0
) ; rtb_B_311_4188_0 = ( 0.0 - rtb_B_311_4181_0 * _rtB -> B_311_481_0 ) -
rtb_B_311_4186_0 * _rtB -> B_311_482_0 ; rtb_B_311_4190_0 = rtb_B_311_4181_0
* _rtB -> B_311_482_0 - rtb_B_311_4186_0 * _rtB -> B_311_481_0 ; _rtB ->
B_311_4268_0 = ( ( ( 0.0 - rtb_B_311_4190_0 ) - rtb_B_311_4186_0 ) * _rtB ->
B_311_1589_0 [ 2 ] + ( _rtB -> B_311_1589_0 [ 0 ] * rtb_B_311_4186_0 + _rtB
-> B_311_1589_0 [ 1 ] * rtb_B_311_4190_0 ) ) * 0.66666666666666663 ;
rtb_B_311_4181_0 = ( ( ( 0.0 - rtb_B_311_4188_0 ) - rtb_B_311_4181_0 ) * _rtB
-> B_311_1589_0 [ 2 ] + ( _rtB -> B_311_1589_0 [ 0 ] * rtb_B_311_4181_0 +
_rtB -> B_311_1589_0 [ 1 ] * rtb_B_311_4188_0 ) ) * 0.66666666666666663 ;
rtb_B_311_4181_0 = ( ( _rtB -> B_311_470_0 <= _rtB -> B_311_4268_0 ) && (
_rtB -> B_311_4268_0 <= _rtB -> B_311_471_0 ) && ( ( _rtB -> B_311_472_0 <=
rtb_B_311_4181_0 ) && ( rtb_B_311_4181_0 <= _rtB -> B_311_473_0 ) ) ) ;
rtb_B_311_4186_0 = _rtDW -> Delay_DSTATE_je ; _rtB -> B_311_4280_0 = ( ( (
rtb_B_311_4181_0 != 0.0 ) == ( _rtDW -> Delay_DSTATE_je != 0.0 ) ) || ( _rtB
-> B_311_474_0 != 0.0 ) ) ; _rtB -> B_311_4282_0 = ( _rtB -> B_311_477_0 && (
_rtDW -> Delay_DSTATE_je != 0.0 ) ) ; if ( _rtDW ->
DiscreteTimeIntegrator_IC_LOADING_h != 0 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff = _rtB -> B_311_4282_0 ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff >= _rtP -> P_3185 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff = _rtP -> P_3185 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff <= _rtP -> P_3186 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff = _rtP -> P_3186 ; } } } if ( _rtB ->
B_311_4280_0 || ( _rtDW -> DiscreteTimeIntegrator_PrevResetState_dg != 0 ) )
{ _rtDW -> DiscreteTimeIntegrator_DSTATE_ff = _rtB -> B_311_4282_0 ; if (
_rtDW -> DiscreteTimeIntegrator_DSTATE_ff >= _rtP -> P_3185 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff = _rtP -> P_3185 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff <= _rtP -> P_3186 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff = _rtP -> P_3186 ; } } } if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff >= _rtP -> P_3185 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff = _rtP -> P_3185 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff <= _rtP -> P_3186 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff = _rtP -> P_3186 ; } } _rtDW -> Relay_Mode_a
= ( ( _rtDW -> DiscreteTimeIntegrator_DSTATE_ff >= _rtP -> P_3187 ) || ( ( !
( _rtDW -> DiscreteTimeIntegrator_DSTATE_ff <= _rtP -> P_3188 ) ) && _rtDW ->
Relay_Mode_a ) ) ; if ( _rtDW -> Relay_Mode_a ) { _rtB -> B_311_4284_0 = _rtP
-> P_3189 ; } else { _rtB -> B_311_4284_0 = _rtP -> P_3190 ; } _rtB ->
B_311_4289_0 = ( ( _rtB -> B_311_4256_0 && ( _rtB -> B_311_4284_0 != 0.0 ) )
|| ( ( ! _rtB -> B_311_4256_0 ) && ( ! ( _rtB -> B_311_4118_0 != 0.0 ) ) ) )
; if ( ! _rtB -> B_311_4289_0 ) { _rtB -> B_311_4291_0 = _rtDW ->
UnitDelay1_DSTATE_pf ; } else { _rtB -> B_311_4291_0 = _rtB -> B_311_4257_0 ;
} rtb_B_311_4190_0 = _rtB -> B_311_4291_0 + _rtP -> P_3191 ; rtb_B_311_4188_0
= muDoubleScalarSin ( rtb_B_311_4190_0 ) ; rtb_B_311_4106_0 =
muDoubleScalarCos ( rtb_B_311_4190_0 ) ; rtb_B_311_4_0 = ( 0.0 -
rtb_B_311_4188_0 * _rtB -> B_311_466_0_p ) - rtb_B_311_4106_0 * _rtB ->
B_311_467_0 ; rtb_B_311_4_1 = rtb_B_311_4188_0 * _rtB -> B_311_467_0 -
rtb_B_311_4106_0 * _rtB -> B_311_466_0_p ; rtb_B_311_4190_0 = ( _rtB ->
B_311_4253_0 [ 0 ] * rtb_B_311_4188_0 + _rtB -> B_311_4253_0 [ 1 ] *
rtb_B_311_4106_0 ) + _rtB -> B_311_463_0 ; B_311_4097_0_idx_1 = ( _rtB ->
B_311_4253_0 [ 0 ] * rtb_B_311_4_0 + _rtB -> B_311_4253_0 [ 1 ] *
rtb_B_311_4_1 ) + _rtB -> B_311_463_0 ; rtb_B_311_4188_0 = ( ( ( 0.0 -
rtb_B_311_4_0 ) - rtb_B_311_4188_0 ) * _rtB -> B_311_4253_0 [ 0 ] + ( ( 0.0 -
rtb_B_311_4_1 ) - rtb_B_311_4106_0 ) * _rtB -> B_311_4253_0 [ 1 ] ) + _rtB ->
B_311_463_0 ; if ( rtb_B_311_4190_0 > _rtB -> B_311_464_0 ) {
rtb_B_311_4190_0 = _rtB -> B_311_464_0 ; } else { if ( rtb_B_311_4190_0 <
_rtB -> B_311_465_0 ) { rtb_B_311_4190_0 = _rtB -> B_311_465_0 ; } } _rtB ->
B_311_4310_0 [ 0 ] = _rtP -> P_3192 * rtb_B_311_4190_0 ; if (
B_311_4097_0_idx_1 > _rtB -> B_311_464_0 ) { B_311_4097_0_idx_1 = _rtB ->
B_311_464_0 ; } else { if ( B_311_4097_0_idx_1 < _rtB -> B_311_465_0 ) {
B_311_4097_0_idx_1 = _rtB -> B_311_465_0 ; } } _rtB -> B_311_4310_0 [ 1 ] =
_rtP -> P_3192 * B_311_4097_0_idx_1 ; if ( rtb_B_311_4188_0 > _rtB ->
B_311_464_0 ) { rtb_B_311_4188_0 = _rtB -> B_311_464_0 ; } else { if (
rtb_B_311_4188_0 < _rtB -> B_311_465_0 ) { rtb_B_311_4188_0 = _rtB ->
B_311_465_0 ; } } _rtB -> B_311_4310_0 [ 2 ] = _rtP -> P_3192 *
rtb_B_311_4188_0 ; ssCallAccelRunBlock ( S , 311 , 4311 , SS_CALL_MDL_OUTPUTS
) ; ssCallAccelRunBlock ( S , 311 , 4312 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 4313 , SS_CALL_MDL_OUTPUTS ) ; if ( _rtB ->
B_311_4193_0 [ 0 ] > _rtP -> P_3193 ) { _rtB -> B_311_4314_0 = _rtP -> P_3193
; } else if ( _rtB -> B_311_4193_0 [ 0 ] < _rtP -> P_3194 ) { _rtB ->
B_311_4314_0 = _rtP -> P_3194 ; } else { _rtB -> B_311_4314_0 = _rtB ->
B_311_4193_0 [ 0 ] ; } ssCallAccelRunBlock ( S , 311 , 4315 ,
SS_CALL_MDL_OUTPUTS ) ; if ( rtb_B_311_4186_0 >= _rtP -> P_3196 ) {
rtb_B_311_4186_0 = _rtB -> B_311_475_0 ; } else { rtb_B_311_4186_0 = _rtB ->
B_311_476_0 ; } _rtB -> B_311_4318_0 = ( rtb_B_311_4181_0 + _rtP -> P_3195 )
* rtb_B_311_4186_0 ; rtb_B_311_35_0 = _rtB -> B_311_478_0 * _rtB ->
B_311_4268_0 + _rtDW -> DiscreteTimeIntegrator_DSTATE_b ; if ( rtb_B_311_35_0
> _rtP -> P_3201 ) { _rtB -> B_311_4322_0 = _rtP -> P_3201 ; } else if (
rtb_B_311_35_0 < _rtP -> P_3202 ) { _rtB -> B_311_4322_0 = _rtP -> P_3202 ; }
else { _rtB -> B_311_4322_0 = rtb_B_311_35_0 ; } ssCallAccelRunBlock ( S ,
311 , 4323 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_311_4324_0 = _rtB ->
B_311_4268_0 * _rtB -> B_311_479_0_p ; _rtB -> B_311_4325_0 = _rtP -> P_3203
* _rtB -> B_311_4322_0 ; { _rtB -> B_311_4326_0 = ( _rtP -> P_3206 [ 0 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_gv [ 0 ] + ( _rtP -> P_3206 [ 1 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_gv [ 1 ] ; _rtB -> B_311_4326_0 += _rtP ->
P_3207 * _rtB -> B_311_4325_0 ; } _rtB -> B_311_4331_0 = ( _rtP -> P_3210 *
_rtB -> B_311_4322_0 - ( real_T ) ( _rtB -> B_311_4257_0 >= _rtB ->
B_311_480_0 ) * _rtP -> P_3209 ) + _rtB -> B_311_4257_0 ; _rtB ->
B_311_4332_0 = muDoubleScalarSin ( _rtB -> B_311_4291_0 ) ; _rtB ->
B_311_4333_0 = muDoubleScalarCos ( _rtB -> B_311_4291_0 ) ; if ( _rtB ->
B_311_4289_0 ) { _rtB -> B_311_4335_0 = _rtB -> B_311_4257_0 ; } else { _rtB
-> B_311_4335_0 = ( _rtP -> P_142 * _rtB -> B_311_4314_0 * _rtP -> P_143 - (
real_T ) ( B_311_4123_0_idx_0 >= _rtB -> B_311_483_0_a ) * _rtP -> P_144 ) +
B_311_4123_0_idx_0 ; } ssCallAccelRunBlock ( S , 311 , 4336 ,
SS_CALL_MDL_OUTPUTS ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) {
_rtB -> B_311_4433_0 = _rtDW -> Memory_PreviousInput_c ; } i = ssIsSampleHit
( S , 2 , 0 ) ; if ( i != 0 ) { _rtB -> B_311_4434_0 = ssGetTaskTime ( S , 2
) ; } if ( _rtB -> B_311_4434_0 > _rtP -> P_3212 ) { _rtB -> B_311_4435_0 =
_rtB -> B_311_4433_0 ; } else { _rtB -> B_311_4435_0 = _rtB -> B_311_512_0_o
; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( ssIsMajorTimeStep
( S ) != 0 ) { _rtDW -> Compare_Mode = ( _rtB -> B_311_4435_0 <= _rtB ->
B_311_514_0 ) ; } _rtB -> B_311_4437_0 = _rtDW -> Compare_Mode ; } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { if ( ( _rtB -> B_311_4437_0 <=
0.0 ) && ( _rtDW -> DiscreteTimeIntegrator2_PrevResetState_a == 1 ) ) { _rtDW
-> DiscreteTimeIntegrator2_DSTATE_h1 = _rtP -> P_3214 ; } rtb_B_311_4439_0 =
( _rtDW -> DiscreteTimeIntegrator2_DSTATE_h1 > _rtB -> B_311_509_0 ) ; } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( ssIsMajorTimeStep ( S ) !=
0 ) { _rtDW -> Compare_Mode_g = ( _rtB -> B_311_4435_0 >= _rtB -> B_311_515_0
) ; } _rtB -> B_311_4441_0 = _rtDW -> Compare_Mode_g ; } i = ssIsSampleHit (
S , 2 , 0 ) ; if ( i != 0 ) { if ( ( _rtB -> B_311_4441_0 <= 0.0 ) && ( _rtDW
-> DiscreteTimeIntegrator1_PrevResetState_e == 1 ) ) { _rtDW ->
DiscreteTimeIntegrator1_DSTATE_m = _rtP -> P_3216 ; } _rtB -> B_311_4444_0 =
( ( ! rtb_B_311_4439_0 ) && ( ! ( _rtDW -> DiscreteTimeIntegrator1_DSTATE_m >
_rtB -> B_311_508_0 ) ) ) ; if ( _rtDW -> systemEnable_j != 0 ) { _rtDW ->
lastSin_fu = muDoubleScalarSin ( _rtP -> P_3219 * ssGetTaskTime ( S , 2 ) ) ;
_rtDW -> lastCos_gi = muDoubleScalarCos ( _rtP -> P_3219 * ssGetTaskTime ( S
, 2 ) ) ; _rtDW -> systemEnable_j = 0 ; } _rtB -> B_311_4446_0 = ( ( ( _rtDW
-> lastSin_fu * _rtP -> P_3223 + _rtDW -> lastCos_gi * _rtP -> P_3222 ) *
_rtP -> P_3221 + ( _rtDW -> lastCos_gi * _rtP -> P_3223 - _rtDW -> lastSin_fu
* _rtP -> P_3222 ) * _rtP -> P_3220 ) * _rtP -> P_3217 + _rtP -> P_3218 ) *
_rtB -> B_311_483_0 [ 0 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_m != 0 ) {
_rtB -> B_311_4447_0 = _rtDW -> Integ4_DSTATE_pu ; } else { _rtB ->
B_311_4447_0 = _rtP -> P_3224 * _rtB -> B_311_4446_0 + _rtDW ->
Integ4_DSTATE_pu ; } _rtB -> B_311_4448_0 = _rtP -> P_3226 ;
ssCallAccelRunBlock ( S , 311 , 4449 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_4448_0 ) { _rtB -> B_311_4456_0 = (
_rtB -> B_311_4447_0 - _rtB -> B_311_4449_0 ) * _rtP -> P_3235 + ( _rtP ->
P_211 * _rtB -> B_311_4446_0 - _rtP -> P_210 * _rtDW -> UnitDelay_DSTATE_mz )
; } else { _rtB -> B_311_4456_0 = _rtDW -> UnitDelay1_DSTATE_k2 ; } if (
_rtDW -> systemEnable_hc != 0 ) { _rtDW -> lastSin_ev = muDoubleScalarSin (
_rtP -> P_3240 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_ll =
muDoubleScalarCos ( _rtP -> P_3240 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_hc = 0 ; } _rtB -> B_311_4458_0 = ( ( ( _rtDW -> lastSin_ev *
_rtP -> P_3244 + _rtDW -> lastCos_ll * _rtP -> P_3243 ) * _rtP -> P_3242 + (
_rtDW -> lastCos_ll * _rtP -> P_3244 - _rtDW -> lastSin_ev * _rtP -> P_3243 )
* _rtP -> P_3241 ) * _rtP -> P_3238 + _rtP -> P_3239 ) * _rtB -> B_311_483_0
[ 0 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_br3 != 0 ) { _rtB -> B_311_4459_0
= _rtDW -> Integ4_DSTATE_gp ; } else { _rtB -> B_311_4459_0 = _rtP -> P_3245
* _rtB -> B_311_4458_0 + _rtDW -> Integ4_DSTATE_gp ; } _rtB -> B_311_4460_0 =
_rtP -> P_3247 ; ssCallAccelRunBlock ( S , 311 , 4461 , SS_CALL_MDL_OUTPUTS )
; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_4460_0 ) { _rtB ->
B_311_4468_0 = ( _rtB -> B_311_4459_0 - _rtB -> B_311_4461_0 ) * _rtP ->
P_3256 + ( _rtP -> P_209 * _rtB -> B_311_4458_0 - _rtP -> P_208 * _rtDW ->
UnitDelay_DSTATE_bf ) ; } else { _rtB -> B_311_4468_0 = _rtDW ->
UnitDelay1_DSTATE_ezs ; } B_311_4123_0_idx_0 = _rtP -> P_3259 *
muDoubleScalarAtan2 ( _rtB -> B_311_4468_0 , _rtB -> B_311_4456_0 ) * _rtP ->
P_3260 ; B_311_4473_0 . re = muDoubleScalarHypot ( _rtB -> B_311_4456_0 ,
_rtB -> B_311_4468_0 ) * muDoubleScalarCos ( B_311_4123_0_idx_0 ) ;
B_311_4473_0 . im = muDoubleScalarHypot ( _rtB -> B_311_4456_0 , _rtB ->
B_311_4468_0 ) * muDoubleScalarSin ( B_311_4123_0_idx_0 ) ; if ( _rtDW ->
systemEnable_jg != 0 ) { _rtDW -> lastSin_nb = muDoubleScalarSin ( _rtP ->
P_3263 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_h = muDoubleScalarCos (
_rtP -> P_3263 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_jg = 0 ; }
_rtB -> B_311_4475_0 = ( ( ( _rtDW -> lastSin_nb * _rtP -> P_3267 + _rtDW ->
lastCos_h * _rtP -> P_3266 ) * _rtP -> P_3265 + ( _rtDW -> lastCos_h * _rtP
-> P_3267 - _rtDW -> lastSin_nb * _rtP -> P_3266 ) * _rtP -> P_3264 ) * _rtP
-> P_3261 + _rtP -> P_3262 ) * _rtB -> B_311_483_0 [ 1 ] ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_b3 != 0 ) { _rtB -> B_311_4476_0 = _rtDW ->
Integ4_DSTATE_gj ; } else { _rtB -> B_311_4476_0 = _rtP -> P_3268 * _rtB ->
B_311_4475_0 + _rtDW -> Integ4_DSTATE_gj ; } _rtB -> B_311_4477_0 = _rtP ->
P_3270 ; ssCallAccelRunBlock ( S , 311 , 4478 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_4477_0 ) { _rtB -> B_311_4485_0 = (
_rtB -> B_311_4476_0 - _rtB -> B_311_4478_0 ) * _rtP -> P_3279 + ( _rtP ->
P_215 * _rtB -> B_311_4475_0 - _rtP -> P_214 * _rtDW -> UnitDelay_DSTATE_mm )
; } else { _rtB -> B_311_4485_0 = _rtDW -> UnitDelay1_DSTATE_oe ; } if (
_rtDW -> systemEnable_oz != 0 ) { _rtDW -> lastSin_e0 = muDoubleScalarSin (
_rtP -> P_3284 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_ow =
muDoubleScalarCos ( _rtP -> P_3284 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_oz = 0 ; } _rtB -> B_311_4487_0 = ( ( ( _rtDW -> lastSin_e0 *
_rtP -> P_3288 + _rtDW -> lastCos_ow * _rtP -> P_3287 ) * _rtP -> P_3286 + (
_rtDW -> lastCos_ow * _rtP -> P_3288 - _rtDW -> lastSin_e0 * _rtP -> P_3287 )
* _rtP -> P_3285 ) * _rtP -> P_3282 + _rtP -> P_3283 ) * _rtB -> B_311_483_0
[ 1 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_cq != 0 ) { _rtB -> B_311_4488_0 =
_rtDW -> Integ4_DSTATE_i ; } else { _rtB -> B_311_4488_0 = _rtP -> P_3289 *
_rtB -> B_311_4487_0 + _rtDW -> Integ4_DSTATE_i ; } _rtB -> B_311_4489_0 =
_rtP -> P_3291 ; ssCallAccelRunBlock ( S , 311 , 4490 , SS_CALL_MDL_OUTPUTS )
; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_4489_0 ) { _rtB ->
B_311_4497_0 = ( _rtB -> B_311_4488_0 - _rtB -> B_311_4490_0 ) * _rtP ->
P_3300 + ( _rtP -> P_213 * _rtB -> B_311_4487_0 - _rtP -> P_212 * _rtDW ->
UnitDelay_DSTATE_jj ) ; } else { _rtB -> B_311_4497_0 = _rtDW ->
UnitDelay1_DSTATE_nb ; } B_311_4123_0_idx_0 = _rtP -> P_3303 *
muDoubleScalarAtan2 ( _rtB -> B_311_4497_0 , _rtB -> B_311_4485_0 ) * _rtP ->
P_3304 ; B_311_4502_0 . re = muDoubleScalarHypot ( _rtB -> B_311_4485_0 ,
_rtB -> B_311_4497_0 ) * muDoubleScalarCos ( B_311_4123_0_idx_0 ) ;
B_311_4502_0 . im = muDoubleScalarHypot ( _rtB -> B_311_4485_0 , _rtB ->
B_311_4497_0 ) * muDoubleScalarSin ( B_311_4123_0_idx_0 ) ; if ( _rtDW ->
systemEnable_ch != 0 ) { _rtDW -> lastSin_h = muDoubleScalarSin ( _rtP ->
P_3307 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_ls = muDoubleScalarCos
( _rtP -> P_3307 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_ch = 0 ;
} _rtB -> B_311_4504_0 = ( ( ( _rtDW -> lastSin_h * _rtP -> P_3311 + _rtDW ->
lastCos_ls * _rtP -> P_3310 ) * _rtP -> P_3309 + ( _rtDW -> lastCos_ls * _rtP
-> P_3311 - _rtDW -> lastSin_h * _rtP -> P_3310 ) * _rtP -> P_3308 ) * _rtP
-> P_3305 + _rtP -> P_3306 ) * _rtB -> B_311_483_0 [ 2 ] ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_mg != 0 ) { _rtB -> B_311_4505_0 = _rtDW ->
Integ4_DSTATE_go ; } else { _rtB -> B_311_4505_0 = _rtP -> P_3312 * _rtB ->
B_311_4504_0 + _rtDW -> Integ4_DSTATE_go ; } _rtB -> B_311_4506_0 = _rtP ->
P_3314 ; ssCallAccelRunBlock ( S , 311 , 4507 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_4506_0 ) { _rtB -> B_311_4514_0 = (
_rtB -> B_311_4505_0 - _rtB -> B_311_4507_0 ) * _rtP -> P_3323 + ( _rtP ->
P_219 * _rtB -> B_311_4504_0 - _rtP -> P_218 * _rtDW -> UnitDelay_DSTATE_lkz
) ; } else { _rtB -> B_311_4514_0 = _rtDW -> UnitDelay1_DSTATE_m2 ; } if (
_rtDW -> systemEnable_at != 0 ) { _rtDW -> lastSin_nj = muDoubleScalarSin (
_rtP -> P_3328 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_pn =
muDoubleScalarCos ( _rtP -> P_3328 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_at = 0 ; } _rtB -> B_311_4516_0 = ( ( ( _rtDW -> lastSin_nj *
_rtP -> P_3332 + _rtDW -> lastCos_pn * _rtP -> P_3331 ) * _rtP -> P_3330 + (
_rtDW -> lastCos_pn * _rtP -> P_3332 - _rtDW -> lastSin_nj * _rtP -> P_3331 )
* _rtP -> P_3329 ) * _rtP -> P_3326 + _rtP -> P_3327 ) * _rtB -> B_311_483_0
[ 2 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_lh != 0 ) { _rtB -> B_311_4517_0 =
_rtDW -> Integ4_DSTATE_iz ; } else { _rtB -> B_311_4517_0 = _rtP -> P_3333 *
_rtB -> B_311_4516_0 + _rtDW -> Integ4_DSTATE_iz ; } _rtB -> B_311_4518_0 =
_rtP -> P_3335 ; ssCallAccelRunBlock ( S , 311 , 4519 , SS_CALL_MDL_OUTPUTS )
; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_4518_0 ) { _rtB ->
B_311_4526_0 = ( _rtB -> B_311_4517_0 - _rtB -> B_311_4519_0 ) * _rtP ->
P_3344 + ( _rtP -> P_217 * _rtB -> B_311_4516_0 - _rtP -> P_216 * _rtDW ->
UnitDelay_DSTATE_gc ) ; } else { _rtB -> B_311_4526_0 = _rtDW ->
UnitDelay1_DSTATE_bc ; } B_311_4123_0_idx_0 = _rtP -> P_3347 *
muDoubleScalarAtan2 ( _rtB -> B_311_4526_0 , _rtB -> B_311_4514_0 ) * _rtP ->
P_3348 ; B_311_4531_0 . re = muDoubleScalarHypot ( _rtB -> B_311_4514_0 ,
_rtB -> B_311_4526_0 ) * muDoubleScalarCos ( B_311_4123_0_idx_0 ) ;
B_311_4531_0 . im = muDoubleScalarHypot ( _rtB -> B_311_4514_0 , _rtB ->
B_311_4526_0 ) * muDoubleScalarSin ( B_311_4123_0_idx_0 ) ;
Auto_microgrid_forecasting_020822_NegSeqComputation ( S , _rtB -> B_311_505_0
, B_311_4473_0 , B_311_4502_0 , B_311_4531_0 , & _rtB -> PosSeqComputation ,
& _rtDW -> PosSeqComputation , & _rtP -> PosSeqComputation ) ;
Auto_microgrid_forecasting_020822_NegSeqComputation ( S , _rtB -> B_311_506_0
, B_311_4473_0 , B_311_4502_0 , B_311_4531_0 , & _rtB -> NegSeqComputation ,
& _rtDW -> NegSeqComputation , & _rtP -> NegSeqComputation ) ;
Auto_microgrid_forecasting_020822_ZeroSeqComputation ( S , _rtB ->
B_311_507_0 , B_311_4473_0 , B_311_4502_0 , B_311_4531_0 , & _rtB ->
ZeroSeqComputation , & _rtDW -> ZeroSeqComputation , & _rtP ->
ZeroSeqComputation ) ; B_311_4535_0_idx_0 = muDoubleScalarHypot ( _rtB ->
PosSeqComputation . B_122_2_0 . re , _rtB -> PosSeqComputation . B_122_2_0 .
im ) ; if ( ssGetTaskTime ( S , 2 ) > _rtP -> P_3349 ) { B_311_4123_0_idx_0 =
_rtP -> P_298 * B_311_4535_0_idx_0 ; } else { B_311_4123_0_idx_0 = _rtB ->
B_311_513_0_b ; } _rtB -> B_311_4540_0 = ( B_311_4123_0_idx_0 <= _rtB ->
B_311_516_0 ) ; if ( ( _rtB -> B_311_4540_0 <= 0.0 ) && ( _rtDW ->
DiscreteTimeIntegrator4_PrevResetState_o == 1 ) ) { _rtDW ->
DiscreteTimeIntegrator4_DSTATE_m = _rtP -> P_3351 ; } _rtB -> B_311_4544_0 =
( B_311_4123_0_idx_0 >= _rtB -> B_311_517_0 ) ; if ( ( _rtB -> B_311_4544_0
<= 0.0 ) && ( _rtDW -> DiscreteTimeIntegrator3_PrevResetState_g == 1 ) ) {
_rtDW -> DiscreteTimeIntegrator3_DSTATE_d = _rtP -> P_3353 ; } _rtB ->
B_311_4547_0 = ( ( ! ( _rtDW -> DiscreteTimeIntegrator4_DSTATE_m > _rtB ->
B_311_511_0 ) ) && ( ! ( _rtDW -> DiscreteTimeIntegrator3_DSTATE_d > _rtB ->
B_311_510_0 ) ) ) ; _rtB -> B_311_4548_0 = ( _rtB -> B_311_4444_0 && _rtB ->
B_311_4547_0 && ( _rtB -> B_311_1259_0 != 0.0 ) ) ; ssCallAccelRunBlock ( S ,
311 , 4549 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_311_4550_0 = ssGetTaskTime ( S
, 2 ) ; } if ( _rtB -> B_311_4550_0 > _rtP -> P_3354 ) { _rtB -> B_311_4551_0
= _rtB -> B_311_4433_0 ; } else { _rtB -> B_311_4551_0 = _rtB -> B_311_522_0
; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( ssIsMajorTimeStep
( S ) != 0 ) { _rtDW -> Compare_Mode_p = ( _rtB -> B_311_4551_0 <= _rtB ->
B_311_524_0 ) ; } _rtB -> B_311_4553_0 = _rtDW -> Compare_Mode_p ; } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { if ( ( _rtB -> B_311_4553_0 <=
0.0 ) && ( _rtDW -> DiscreteTimeIntegrator2_PrevResetState_n == 1 ) ) { _rtDW
-> DiscreteTimeIntegrator2_DSTATE_hg = _rtP -> P_3356 ; } rtb_B_311_4555_0 =
( _rtDW -> DiscreteTimeIntegrator2_DSTATE_hg > _rtB -> B_311_519_0 ) ; } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( ssIsMajorTimeStep ( S ) !=
0 ) { _rtDW -> Compare_Mode_n = ( _rtB -> B_311_4551_0 >= _rtB -> B_311_525_0
) ; } _rtB -> B_311_4557_0 = _rtDW -> Compare_Mode_n ; } i = ssIsSampleHit (
S , 2 , 0 ) ; if ( i != 0 ) { if ( ( _rtB -> B_311_4557_0 <= 0.0 ) && ( _rtDW
-> DiscreteTimeIntegrator1_PrevResetState_je == 1 ) ) { _rtDW ->
DiscreteTimeIntegrator1_DSTATE_dd = _rtP -> P_3358 ; } _rtB -> B_311_4560_0 =
( ( ! rtb_B_311_4555_0 ) && ( ! ( _rtDW -> DiscreteTimeIntegrator1_DSTATE_dd
> _rtB -> B_311_518_0 ) ) ) ; if ( ssGetTaskTime ( S , 2 ) > _rtP -> P_3359 )
{ B_311_4123_0_idx_0 = _rtP -> P_299 * B_311_4535_0_idx_0 ; } else {
B_311_4123_0_idx_0 = _rtB -> B_311_523_0 ; } _rtB -> B_311_4565_0 = (
B_311_4123_0_idx_0 <= _rtB -> B_311_526_0 ) ; if ( ( _rtB -> B_311_4565_0 <=
0.0 ) && ( _rtDW -> DiscreteTimeIntegrator4_PrevResetState_i == 1 ) ) { _rtDW
-> DiscreteTimeIntegrator4_DSTATE_a = _rtP -> P_3361 ; } _rtB -> B_311_4569_0
= ( B_311_4123_0_idx_0 >= _rtB -> B_311_527_0 ) ; if ( ( _rtB -> B_311_4569_0
<= 0.0 ) && ( _rtDW -> DiscreteTimeIntegrator3_PrevResetState_c == 1 ) ) {
_rtDW -> DiscreteTimeIntegrator3_DSTATE_e = _rtP -> P_3363 ; } _rtB ->
B_311_4572_0 = ( ( ! ( _rtDW -> DiscreteTimeIntegrator4_DSTATE_a > _rtB ->
B_311_521_0 ) ) && ( ! ( _rtDW -> DiscreteTimeIntegrator3_DSTATE_e > _rtB ->
B_311_520_0_a ) ) ) ; _rtB -> B_311_4573_0 = ( _rtB -> B_311_4560_0 && _rtB
-> B_311_4572_0 && ( _rtB -> B_311_1258_0 != 0.0 ) ) ; ssCallAccelRunBlock (
S , 311 , 4574 , SS_CALL_MDL_OUTPUTS ) ; if ( _rtB -> B_311_486_0 >= _rtP ->
P_3367 ) { B_311_4123_0_idx_0 = _rtB -> B_311_4548_0 ; } else {
B_311_4123_0_idx_0 = rt_Lookup ( _rtP -> P_3365 , 4 , ssGetTaskTime ( S , 2 )
, _rtP -> P_3366 ) ; } if ( _rtP -> P_3364 >= _rtP -> P_3368 ) { if ( _rtB ->
B_311_487_0 >= _rtP -> P_311 ) { _rtB -> B_311_4582_0 = B_311_4123_0_idx_0 ;
} else { _rtB -> B_311_4582_0 = _rtB -> B_311_490_0 ; } } else { _rtB ->
B_311_4582_0 = rt_Lookup ( _rtP -> P_309 , 5 , ssGetTaskTime ( S , 2 ) , _rtP
-> P_310 ) ; } if ( _rtP -> P_3369 >= _rtP -> P_3370 ) { if ( _rtB ->
B_311_488_0 >= _rtP -> P_314 ) { _rtB -> B_311_4590_0 = B_311_4123_0_idx_0 ;
} else { _rtB -> B_311_4590_0 = _rtB -> B_311_490_0 ; } } else { _rtB ->
B_311_4590_0 = rt_Lookup ( _rtP -> P_312 , 5 , ssGetTaskTime ( S , 2 ) , _rtP
-> P_313 ) ; } if ( _rtP -> P_3371 >= _rtP -> P_3372 ) { if ( _rtB ->
B_311_489_0 >= _rtP -> P_317 ) { _rtB -> B_311_4598_0 = B_311_4123_0_idx_0 ;
} else { _rtB -> B_311_4598_0 = _rtB -> B_311_490_0 ; } } else { _rtB ->
B_311_4598_0 = rt_Lookup ( _rtP -> P_315 , 5 , ssGetTaskTime ( S , 2 ) , _rtP
-> P_316 ) ; } if ( _rtB -> B_311_491_0 >= _rtP -> P_3376 ) {
B_311_4123_0_idx_0 = _rtB -> B_311_4573_0 ; } else { B_311_4123_0_idx_0 =
rt_Lookup ( _rtP -> P_3374 , 4 , ssGetTaskTime ( S , 2 ) , _rtP -> P_3375 ) ;
} if ( _rtP -> P_3373 >= _rtP -> P_3377 ) { if ( _rtB -> B_311_492_0_j >=
_rtP -> P_329 ) { _rtB -> B_311_4616_0 = B_311_4123_0_idx_0 ; } else { _rtB
-> B_311_4616_0 = _rtB -> B_311_495_0_e ; } } else { _rtB -> B_311_4616_0 =
rt_Lookup ( _rtP -> P_327 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_328 ) ; }
if ( _rtP -> P_3378 >= _rtP -> P_3379 ) { if ( _rtB -> B_311_493_0 >= _rtP ->
P_332 ) { _rtB -> B_311_4624_0 = B_311_4123_0_idx_0 ; } else { _rtB ->
B_311_4624_0 = _rtB -> B_311_495_0_e ; } } else { _rtB -> B_311_4624_0 =
rt_Lookup ( _rtP -> P_330 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_331 ) ; }
if ( _rtP -> P_3380 >= _rtP -> P_3381 ) { if ( _rtB -> B_311_494_0 >= _rtP ->
P_335 ) { _rtB -> B_311_4632_0 = B_311_4123_0_idx_0 ; } else { _rtB ->
B_311_4632_0 = _rtB -> B_311_495_0_e ; } } else { _rtB -> B_311_4632_0 =
rt_Lookup ( _rtP -> P_333 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_334 ) ; }
} i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( ssIsMajorTimeStep (
S ) != 0 ) { _rtDW -> RelationalOperator_Mode_l = ( _rtX ->
Integrator_CSTATE_op > _rtB -> B_311_498_0 ) ; } _rtB -> B_311_4643_0 = _rtDW
-> RelationalOperator_Mode_l ; } _rtB -> B_311_4644_0 = _rtX ->
Integrator_CSTATE_op - _rtB -> B_311_498_0 ; if ( ( _rtDW ->
Initial_FirstOutputTime_co == ( rtMinusInf ) ) || ( _rtDW ->
Initial_FirstOutputTime_co == ssGetTaskTime ( S , 0 ) ) ) { _rtDW ->
Initial_FirstOutputTime_co = ssGetTaskTime ( S , 0 ) ; _rtB -> B_311_4645_0 =
_rtP -> P_3382 ; } else { _rtB -> B_311_4645_0 = _rtB -> B_311_4644_0 ; } if
( ssIsMajorTimeStep ( S ) != 0 ) { rtb_B_311_3366_0 = ( _rtB -> B_311_4643_0
&& ( _rtZCE -> Integrator_Reset_ZCE_k != 1 ) ) ; _rtZCE ->
Integrator_Reset_ZCE_k = _rtB -> B_311_4643_0 ; if ( rtb_B_311_3366_0 || (
_rtDW -> Integrator_IWORK_d != 0 ) ) { _rtX -> Integrator_CSTATE_op = _rtB ->
B_311_4645_0 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } if ( _rtX ->
Integrator_CSTATE_op >= _rtP -> P_3383 ) { if ( _rtX -> Integrator_CSTATE_op
> _rtP -> P_3383 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
_rtX -> Integrator_CSTATE_op = _rtP -> P_3383 ; } else { if ( _rtX ->
Integrator_CSTATE_op <= _rtP -> P_3384 ) { if ( _rtX -> Integrator_CSTATE_op
< _rtP -> P_3384 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
_rtX -> Integrator_CSTATE_op = _rtP -> P_3384 ; } } } _rtB -> B_311_4646_0 =
_rtX -> Integrator_CSTATE_op ;
Auto_microgrid_forecasting_020822_AutomaticGainControl ( S , _rtB ->
B_311_497_0 , _rtB -> B_311_4433_0 , _rtB -> B_311_4646_0 , _rtB ->
B_311_483_0 , & _rtB -> AutomaticGainControl , & _rtDW ->
AutomaticGainControl , & _rtP -> AutomaticGainControl , & _rtX ->
AutomaticGainControl , & ( ( XDis_Auto_microgrid_forecasting_020822_T * )
ssGetContStateDisabled ( S ) ) -> AutomaticGainControl ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtX -> Integrator_CSTATE_hc >= _rtP ->
P_3387 ) { if ( _rtX -> Integrator_CSTATE_hc > _rtP -> P_3387 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtX ->
Integrator_CSTATE_hc = _rtP -> P_3387 ; } else { if ( _rtX ->
Integrator_CSTATE_hc <= _rtP -> P_3388 ) { if ( _rtX -> Integrator_CSTATE_hc
< _rtP -> P_3388 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
_rtX -> Integrator_CSTATE_hc = _rtP -> P_3388 ; } } } _rtB -> B_311_4649_0 =
_rtX -> Integrator_CSTATE_hc ; { real_T * * uBuffer = ( real_T * * ) & _rtDW
-> VariableTransportDelay_PWORK_h . TUbufferPtrs [ 0 ] ; real_T * * tBuffer =
( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_h . TUbufferPtrs [ 1 ]
; real_T * * xBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK_h . TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT
( S ) ; real_T appliedDelay ; _rtB -> B_311_4650_0 =
Auto_microgrid_forecasting_020822_acc_rt_VTDelayfindtDInterpolate ( ( (
X_Auto_microgrid_forecasting_020822_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE_p , * tBuffer , * uBuffer , * xBuffer , _rtDW
-> VariableTransportDelay_IWORK_o . CircularBufSize , _rtDW ->
VariableTransportDelay_IWORK_o . Head , _rtDW ->
VariableTransportDelay_IWORK_o . Tail , & _rtDW ->
VariableTransportDelay_IWORK_o . Last , simTime , 0.0 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
, _rtP -> P_3390 , & appliedDelay ) ; } _rtB -> B_311_4651_0 = _rtX ->
integrator_CSTATE_o ; _rtB -> B_311_4652_0 = ssGetT ( S ) ; i = ssIsSampleHit
( S , 1 , 0 ) ; if ( i != 0 ) { if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW
-> RelationalOperator_Mode_i = ( _rtB -> B_311_4652_0 >= _rtB -> B_311_499_0
) ; } _rtB -> B_311_4653_0 = _rtDW -> RelationalOperator_Mode_i ; _rtB ->
B_311_4654_0 = _rtDW -> Memory_PreviousInput_l ; } if ( _rtB -> B_311_4653_0
) { _rtB -> B_68_0_0 = _rtB -> B_311_4651_0 - _rtB -> B_311_4650_0 ; _rtB ->
B_68_1_0 = _rtB -> B_68_0_0 * _rtB -> B_311_4433_0 ; _rtB -> B_311_4656_0 =
_rtB -> B_68_1_0 ; } else { _rtB -> B_311_4656_0 = _rtB -> B_311_4654_0 ; }
_rtB -> B_311_4657_0 = _rtB -> B_311_4656_0 * _rtB -> AutomaticGainControl .
B_67_17_0 ; _rtB -> B_311_4658_0 = _rtP -> P_3393 * _rtB -> B_311_4657_0 ;
_rtB -> B_311_4659_0 = _rtP -> P_3394 * _rtB -> B_311_4657_0 ; _rtB ->
B_311_4660_0 = _rtP -> P_3395 * _rtB -> B_311_4657_0 ; _rtB -> B_311_4661_0 =
0.0 ; _rtB -> B_311_4661_0 += _rtP -> P_3397 * _rtX -> TransferFcn_CSTATE_le
; _rtB -> B_311_4661_0 += _rtP -> P_3398 * _rtB -> B_311_4660_0 ; _rtB ->
B_311_4662_0 = ( _rtB -> B_311_4658_0 + _rtB -> B_311_4649_0 ) + _rtB ->
B_311_4661_0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW ->
Saturation2_MODE_a = _rtB -> B_311_4662_0 >= _rtP -> P_3399 ? 1 : _rtB ->
B_311_4662_0 > _rtP -> P_3400 ? 0 : - 1 ; } _rtB -> B_311_4663_0 = _rtDW ->
Saturation2_MODE_a == 1 ? _rtP -> P_3399 : _rtDW -> Saturation2_MODE_a == - 1
? _rtP -> P_3400 : _rtB -> B_311_4662_0 ; _rtB -> B_311_4664_0 = _rtP ->
P_3401 * _rtB -> B_311_4663_0 ; if ( ( _rtDW -> LastMajorTimeA_m >=
ssGetTaskTime ( S , 0 ) ) && ( _rtDW -> LastMajorTimeB_l >= ssGetTaskTime ( S
, 0 ) ) ) { _rtB -> B_311_4665_0 = _rtB -> B_311_4664_0 ; } else { if ( ( (
_rtDW -> LastMajorTimeA_m < _rtDW -> LastMajorTimeB_l ) && ( _rtDW ->
LastMajorTimeB_l < ssGetTaskTime ( S , 0 ) ) ) || ( ( _rtDW ->
LastMajorTimeA_m >= _rtDW -> LastMajorTimeB_l ) && ( _rtDW ->
LastMajorTimeA_m >= ssGetTaskTime ( S , 0 ) ) ) ) { rtb_B_311_62_0 =
ssGetTaskTime ( S , 0 ) - _rtDW -> LastMajorTimeB_l ; B_311_1965_0_idx_0 =
_rtDW -> PrevYB_b ; } else { rtb_B_311_62_0 = ssGetTaskTime ( S , 0 ) - _rtDW
-> LastMajorTimeA_m ; B_311_1965_0_idx_0 = _rtDW -> PrevYA_h ; }
rtb_B_311_2022_0 = rtb_B_311_62_0 * _rtP -> P_3402 ; rtb_B_311_251_0 = _rtB
-> B_311_4664_0 - B_311_1965_0_idx_0 ; if ( rtb_B_311_251_0 >
rtb_B_311_2022_0 ) { _rtB -> B_311_4665_0 = B_311_1965_0_idx_0 +
rtb_B_311_2022_0 ; } else { rtb_B_311_62_0 *= _rtP -> P_3403 ; if (
rtb_B_311_251_0 < rtb_B_311_62_0 ) { _rtB -> B_311_4665_0 =
B_311_1965_0_idx_0 + rtb_B_311_62_0 ; } else { _rtB -> B_311_4665_0 = _rtB ->
B_311_4664_0 ; } } } _rtB -> B_311_4666_0 = _rtX -> Integrator_x1_CSTATE_a ;
_rtB -> B_311_4667_0 = _rtP -> P_3405 * _rtB -> B_311_4666_0 ; _rtB ->
B_311_4668_0 = _rtX -> Integrator_x2_CSTATE_j ; _rtB -> B_311_4669_0 = _rtP
-> P_3407 * _rtB -> B_311_4668_0 ; _rtB -> B_311_4670_0 = _rtP -> P_3408 *
_rtB -> B_311_4666_0 ; _rtB -> B_311_4671_0 = _rtP -> P_3409 * _rtB ->
B_311_4668_0 ; _rtB -> B_311_4672_0 = _rtB -> B_311_4667_0 + _rtB ->
B_311_4669_0 ; _rtB -> B_311_4673_0 = _rtB -> B_311_4670_0 + _rtB ->
B_311_4671_0 ; _rtB -> B_311_4674_0 = _rtP -> P_3410 * _rtB -> B_311_4665_0 ;
_rtB -> B_311_4675_0 = _rtB -> B_311_4672_0 + _rtB -> B_311_4674_0 ; _rtB ->
B_311_4676_0 = _rtP -> P_3411 * _rtB -> B_311_4665_0 ; _rtB -> B_311_4677_0 =
_rtB -> B_311_4673_0 + _rtB -> B_311_4676_0 ; _rtB -> B_311_4682_0 = ( _rtP
-> P_3412 * _rtB -> B_311_4666_0 + _rtP -> P_3413 * _rtB -> B_311_4668_0 ) +
_rtP -> P_3414 * _rtB -> B_311_4665_0 ; if ( _rtB -> B_311_4433_0 > _rtP ->
P_3415 ) { _rtB -> B_311_4683_0 = _rtP -> P_3415 ; } else if ( _rtB ->
B_311_4433_0 < _rtP -> P_3416 ) { _rtB -> B_311_4683_0 = _rtP -> P_3416 ; }
else { _rtB -> B_311_4683_0 = _rtB -> B_311_4433_0 ; } _rtB -> B_311_4684_0 =
1.0 / _rtB -> B_311_4683_0 ; for ( i = 0 ; i < 3 ; i ++ ) { _rtB ->
B_311_4685_0 [ i ] = 0.0 ; _rtB -> B_311_4685_0 [ i ] += _rtP -> P_3417 [ i ]
* _rtB -> B_311_483_0 [ 0 ] ; _rtB -> B_311_4685_0 [ i ] += _rtP -> P_3417 [
i + 3 ] * _rtB -> B_311_483_0 [ 1 ] ; _rtB -> B_311_4685_0 [ i ] += _rtP ->
P_3417 [ i + 6 ] * _rtB -> B_311_483_0 [ 2 ] ; _rtB -> B_311_4686_0 [ i ] =
_rtP -> P_3418 * _rtB -> B_311_4685_0 [ i ] ; }
Auto_microgrid_forecasting_020822_Subsystempi2delay ( S , _rtB -> B_311_504_0
, & _rtB -> B_311_4686_0 [ 0 ] , _rtB -> B_311_4646_0 , & _rtB ->
Subsystempi2delay , & _rtDW -> Subsystempi2delay ) ;
Auto_microgrid_forecasting_020822_Subsystem1 ( S , _rtB -> B_311_502_0 , &
_rtB -> B_311_4686_0 [ 0 ] , _rtB -> B_311_4646_0 , & _rtB -> Subsystem1 , &
_rtDW -> Subsystem1 ) ; if ( _rtB -> B_311_502_0 != 0 ) { _rtB ->
B_311_4689_0 [ 0 ] = _rtB -> Subsystem1 . B_66_0_0 ; _rtB -> B_311_4689_0 [ 1
] = _rtB -> Subsystem1 . B_66_1_0 ; } else { _rtB -> B_311_4689_0 [ 0 ] =
_rtB -> Subsystempi2delay . B_65_0_0 ; _rtB -> B_311_4689_0 [ 1 ] = _rtB ->
Subsystempi2delay . B_65_1_0 ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i !=
0 ) { ssCallAccelRunBlock ( S , 311 , 4690 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 311 , 4691 , SS_CALL_MDL_OUTPUTS ) ; }
Auto_microgrid_forecasting_020822_RMS ( S , _rtB -> B_311_530_0_c , _rtB ->
B_311_609_0 , & _rtB -> RMS , & _rtDW -> RMS , & _rtP -> RMS , & ( (
XDis_Auto_microgrid_forecasting_020822_T * ) ssGetContStateDisabled ( S ) )
-> RMS ) ; Auto_microgrid_forecasting_020822_TrueRMS ( S , _rtB ->
B_311_529_0_b , _rtB -> B_311_609_0 , & _rtB -> TrueRMS , & _rtDW -> TrueRMS
, & _rtP -> TrueRMS , & _rtX -> TrueRMS , & ( (
XDis_Auto_microgrid_forecasting_020822_T * ) ssGetContStateDisabled ( S ) )
-> TrueRMS ) ; Auto_microgrid_forecasting_020822_RMS ( S , _rtB ->
B_311_533_0 , _rtB -> B_311_609_0 , & _rtB -> RMS_j , & _rtDW -> RMS_j , &
_rtP -> RMS_j , & ( ( XDis_Auto_microgrid_forecasting_020822_T * )
ssGetContStateDisabled ( S ) ) -> RMS_j ) ;
Auto_microgrid_forecasting_020822_TrueRMS ( S , _rtB -> B_311_532_0 , _rtB ->
B_311_609_0 , & _rtB -> TrueRMS_c , & _rtDW -> TrueRMS_c , & _rtP ->
TrueRMS_c , & _rtX -> TrueRMS_c , & ( (
XDis_Auto_microgrid_forecasting_020822_T * ) ssGetContStateDisabled ( S ) )
-> TrueRMS_c ) ; i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtB ->
B_311_4697_0 = _rtDW -> Memory_PreviousInput_jb ; } i = ssIsSampleHit ( S , 2
, 0 ) ; if ( i != 0 ) { if ( _rtDW -> systemEnable_i0 != 0 ) { _rtDW ->
lastSin_m = muDoubleScalarSin ( _rtP -> P_3423 * ssGetTaskTime ( S , 2 ) ) ;
_rtDW -> lastCos_pe = muDoubleScalarCos ( _rtP -> P_3423 * ssGetTaskTime ( S
, 2 ) ) ; _rtDW -> systemEnable_i0 = 0 ; } _rtB -> B_311_4699_0 = ( ( ( _rtDW
-> lastSin_m * _rtP -> P_3427 + _rtDW -> lastCos_pe * _rtP -> P_3426 ) * _rtP
-> P_3425 + ( _rtDW -> lastCos_pe * _rtP -> P_3427 - _rtDW -> lastSin_m *
_rtP -> P_3426 ) * _rtP -> P_3424 ) * _rtP -> P_3421 + _rtP -> P_3422 ) *
_rtB -> B_311_157_0 [ 0 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_dh != 0 ) {
_rtB -> B_311_4700_0 = _rtDW -> Integ4_DSTATE_oe ; } else { _rtB ->
B_311_4700_0 = _rtP -> P_3428 * _rtB -> B_311_4699_0 + _rtDW ->
Integ4_DSTATE_oe ; } _rtB -> B_311_4701_0 = _rtP -> P_3430 ;
ssCallAccelRunBlock ( S , 311 , 4702 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_4701_0 ) { _rtB -> B_311_4709_0 = (
_rtB -> B_311_4700_0 - _rtB -> B_311_4702_0 ) * _rtP -> P_3439 + ( _rtP ->
P_235 * _rtB -> B_311_4699_0 - _rtP -> P_234 * _rtDW -> UnitDelay_DSTATE_hg )
; } else { _rtB -> B_311_4709_0 = _rtDW -> UnitDelay1_DSTATE_og ; } if (
_rtDW -> systemEnable_cq != 0 ) { _rtDW -> lastSin_d = muDoubleScalarSin (
_rtP -> P_3444 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_hr =
muDoubleScalarCos ( _rtP -> P_3444 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_cq = 0 ; } _rtB -> B_311_4711_0 = ( ( ( _rtDW -> lastSin_d *
_rtP -> P_3448 + _rtDW -> lastCos_hr * _rtP -> P_3447 ) * _rtP -> P_3446 + (
_rtDW -> lastCos_hr * _rtP -> P_3448 - _rtDW -> lastSin_d * _rtP -> P_3447 )
* _rtP -> P_3445 ) * _rtP -> P_3442 + _rtP -> P_3443 ) * _rtB -> B_311_157_0
[ 0 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_dg != 0 ) { _rtB -> B_311_4712_0 =
_rtDW -> Integ4_DSTATE_ff ; } else { _rtB -> B_311_4712_0 = _rtP -> P_3449 *
_rtB -> B_311_4711_0 + _rtDW -> Integ4_DSTATE_ff ; } _rtB -> B_311_4713_0 =
_rtP -> P_3451 ; ssCallAccelRunBlock ( S , 311 , 4714 , SS_CALL_MDL_OUTPUTS )
; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_4713_0 ) { _rtB ->
B_311_4721_0 = ( _rtB -> B_311_4712_0 - _rtB -> B_311_4714_0 ) * _rtP ->
P_3460 + ( _rtP -> P_233 * _rtB -> B_311_4711_0 - _rtP -> P_232 * _rtDW ->
UnitDelay_DSTATE_au ) ; } else { _rtB -> B_311_4721_0 = _rtDW ->
UnitDelay1_DSTATE_j4 ; } B_311_4123_0_idx_0 = _rtP -> P_3463 *
muDoubleScalarAtan2 ( _rtB -> B_311_4721_0 , _rtB -> B_311_4709_0 ) * _rtP ->
P_3464 ; B_311_4726_0 . re = muDoubleScalarHypot ( _rtB -> B_311_4709_0 ,
_rtB -> B_311_4721_0 ) * muDoubleScalarCos ( B_311_4123_0_idx_0 ) ;
B_311_4726_0 . im = muDoubleScalarHypot ( _rtB -> B_311_4709_0 , _rtB ->
B_311_4721_0 ) * muDoubleScalarSin ( B_311_4123_0_idx_0 ) ; if ( _rtDW ->
systemEnable_mm != 0 ) { _rtDW -> lastSin_nx = muDoubleScalarSin ( _rtP ->
P_3467 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_b3 = muDoubleScalarCos
( _rtP -> P_3467 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_mm = 0 ;
} _rtB -> B_311_4728_0 = ( ( ( _rtDW -> lastSin_nx * _rtP -> P_3471 + _rtDW
-> lastCos_b3 * _rtP -> P_3470 ) * _rtP -> P_3469 + ( _rtDW -> lastCos_b3 *
_rtP -> P_3471 - _rtDW -> lastSin_nx * _rtP -> P_3470 ) * _rtP -> P_3468 ) *
_rtP -> P_3465 + _rtP -> P_3466 ) * _rtB -> B_311_157_0 [ 1 ] ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_a40 != 0 ) { _rtB -> B_311_4729_0 = _rtDW ->
Integ4_DSTATE_oh ; } else { _rtB -> B_311_4729_0 = _rtP -> P_3472 * _rtB ->
B_311_4728_0 + _rtDW -> Integ4_DSTATE_oh ; } _rtB -> B_311_4730_0 = _rtP ->
P_3474 ; ssCallAccelRunBlock ( S , 311 , 4731 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_4730_0 ) { _rtB -> B_311_4738_0 = (
_rtB -> B_311_4729_0 - _rtB -> B_311_4731_0 ) * _rtP -> P_3483 + ( _rtP ->
P_239 * _rtB -> B_311_4728_0 - _rtP -> P_238 * _rtDW -> UnitDelay_DSTATE_jt )
; } else { _rtB -> B_311_4738_0 = _rtDW -> UnitDelay1_DSTATE_iw ; } if (
_rtDW -> systemEnable_lz != 0 ) { _rtDW -> lastSin_n4 = muDoubleScalarSin (
_rtP -> P_3488 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_fu =
muDoubleScalarCos ( _rtP -> P_3488 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_lz = 0 ; } _rtB -> B_311_4740_0 = ( ( ( _rtDW -> lastSin_n4 *
_rtP -> P_3492 + _rtDW -> lastCos_fu * _rtP -> P_3491 ) * _rtP -> P_3490 + (
_rtDW -> lastCos_fu * _rtP -> P_3492 - _rtDW -> lastSin_n4 * _rtP -> P_3491 )
* _rtP -> P_3489 ) * _rtP -> P_3486 + _rtP -> P_3487 ) * _rtB -> B_311_157_0
[ 1 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_g != 0 ) { _rtB -> B_311_4741_0 =
_rtDW -> Integ4_DSTATE_bmx ; } else { _rtB -> B_311_4741_0 = _rtP -> P_3493 *
_rtB -> B_311_4740_0 + _rtDW -> Integ4_DSTATE_bmx ; } _rtB -> B_311_4742_0 =
_rtP -> P_3495 ; ssCallAccelRunBlock ( S , 311 , 4743 , SS_CALL_MDL_OUTPUTS )
; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_4742_0 ) { _rtB ->
B_311_4750_0 = ( _rtB -> B_311_4741_0 - _rtB -> B_311_4743_0 ) * _rtP ->
P_3504 + ( _rtP -> P_237 * _rtB -> B_311_4740_0 - _rtP -> P_236 * _rtDW ->
UnitDelay_DSTATE_it ) ; } else { _rtB -> B_311_4750_0 = _rtDW ->
UnitDelay1_DSTATE_ov ; } B_311_4123_0_idx_0 = _rtP -> P_3507 *
muDoubleScalarAtan2 ( _rtB -> B_311_4750_0 , _rtB -> B_311_4738_0 ) * _rtP ->
P_3508 ; B_311_4755_0 . re = muDoubleScalarHypot ( _rtB -> B_311_4738_0 ,
_rtB -> B_311_4750_0 ) * muDoubleScalarCos ( B_311_4123_0_idx_0 ) ;
B_311_4755_0 . im = muDoubleScalarHypot ( _rtB -> B_311_4738_0 , _rtB ->
B_311_4750_0 ) * muDoubleScalarSin ( B_311_4123_0_idx_0 ) ; if ( _rtDW ->
systemEnable_hp != 0 ) { _rtDW -> lastSin_ew = muDoubleScalarSin ( _rtP ->
P_3511 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_pv = muDoubleScalarCos
( _rtP -> P_3511 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> systemEnable_hp = 0 ;
} _rtB -> B_311_4757_0 = ( ( ( _rtDW -> lastSin_ew * _rtP -> P_3515 + _rtDW
-> lastCos_pv * _rtP -> P_3514 ) * _rtP -> P_3513 + ( _rtDW -> lastCos_pv *
_rtP -> P_3515 - _rtDW -> lastSin_ew * _rtP -> P_3514 ) * _rtP -> P_3512 ) *
_rtP -> P_3509 + _rtP -> P_3510 ) * _rtB -> B_311_157_0 [ 2 ] ; if ( _rtDW ->
Integ4_SYSTEM_ENABLE_jl != 0 ) { _rtB -> B_311_4758_0 = _rtDW ->
Integ4_DSTATE_lf ; } else { _rtB -> B_311_4758_0 = _rtP -> P_3516 * _rtB ->
B_311_4757_0 + _rtDW -> Integ4_DSTATE_lf ; } _rtB -> B_311_4759_0 = _rtP ->
P_3518 ; ssCallAccelRunBlock ( S , 311 , 4760 , SS_CALL_MDL_OUTPUTS ) ; if (
ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_4759_0 ) { _rtB -> B_311_4767_0 = (
_rtB -> B_311_4758_0 - _rtB -> B_311_4760_0 ) * _rtP -> P_3527 + ( _rtP ->
P_243 * _rtB -> B_311_4757_0 - _rtP -> P_242 * _rtDW -> UnitDelay_DSTATE_b5 )
; } else { _rtB -> B_311_4767_0 = _rtDW -> UnitDelay1_DSTATE_n4 ; } if (
_rtDW -> systemEnable_ad != 0 ) { _rtDW -> lastSin_cz = muDoubleScalarSin (
_rtP -> P_3532 * ssGetTaskTime ( S , 2 ) ) ; _rtDW -> lastCos_kq =
muDoubleScalarCos ( _rtP -> P_3532 * ssGetTaskTime ( S , 2 ) ) ; _rtDW ->
systemEnable_ad = 0 ; } _rtB -> B_311_4769_0 = ( ( ( _rtDW -> lastSin_cz *
_rtP -> P_3536 + _rtDW -> lastCos_kq * _rtP -> P_3535 ) * _rtP -> P_3534 + (
_rtDW -> lastCos_kq * _rtP -> P_3536 - _rtDW -> lastSin_cz * _rtP -> P_3535 )
* _rtP -> P_3533 ) * _rtP -> P_3530 + _rtP -> P_3531 ) * _rtB -> B_311_157_0
[ 2 ] ; if ( _rtDW -> Integ4_SYSTEM_ENABLE_cf != 0 ) { _rtB -> B_311_4770_0 =
_rtDW -> Integ4_DSTATE_o1 ; } else { _rtB -> B_311_4770_0 = _rtP -> P_3537 *
_rtB -> B_311_4769_0 + _rtDW -> Integ4_DSTATE_o1 ; } _rtB -> B_311_4771_0 =
_rtP -> P_3539 ; ssCallAccelRunBlock ( S , 311 , 4772 , SS_CALL_MDL_OUTPUTS )
; if ( ssGetTaskTime ( S , 2 ) >= _rtB -> B_311_4771_0 ) { _rtB ->
B_311_4779_0 = ( _rtB -> B_311_4770_0 - _rtB -> B_311_4772_0 ) * _rtP ->
P_3548 + ( _rtP -> P_241 * _rtB -> B_311_4769_0 - _rtP -> P_240 * _rtDW ->
UnitDelay_DSTATE_ib ) ; } else { _rtB -> B_311_4779_0 = _rtDW ->
UnitDelay1_DSTATE_e0 ; } B_311_4123_0_idx_0 = _rtP -> P_3551 *
muDoubleScalarAtan2 ( _rtB -> B_311_4779_0 , _rtB -> B_311_4767_0 ) * _rtP ->
P_3552 ; B_311_4784_0 . re = muDoubleScalarHypot ( _rtB -> B_311_4767_0 ,
_rtB -> B_311_4779_0 ) * muDoubleScalarCos ( B_311_4123_0_idx_0 ) ;
B_311_4784_0 . im = muDoubleScalarHypot ( _rtB -> B_311_4767_0 , _rtB ->
B_311_4779_0 ) * muDoubleScalarSin ( B_311_4123_0_idx_0 ) ;
Auto_microgrid_forecasting_020822_NegSeqComputation ( S , _rtB -> B_311_543_0
, B_311_4726_0 , B_311_4755_0 , B_311_4784_0 , & _rtB -> PosSeqComputation_a
, & _rtDW -> PosSeqComputation_a , & _rtP -> PosSeqComputation_a ) ;
Auto_microgrid_forecasting_020822_NegSeqComputation ( S , _rtB -> B_311_544_0
, B_311_4726_0 , B_311_4755_0 , B_311_4784_0 , & _rtB -> NegSeqComputation_g
, & _rtDW -> NegSeqComputation_g , & _rtP -> NegSeqComputation_g ) ;
Auto_microgrid_forecasting_020822_ZeroSeqComputation ( S , _rtB ->
B_311_545_0 , B_311_4726_0 , B_311_4755_0 , B_311_4784_0 , & _rtB ->
ZeroSeqComputation_b , & _rtDW -> ZeroSeqComputation_b , & _rtP ->
ZeroSeqComputation_b ) ; rtb_B_311_3271_0 [ 0 ] = muDoubleScalarHypot ( _rtB
-> PosSeqComputation_a . B_122_2_0 . re , _rtB -> PosSeqComputation_a .
B_122_2_0 . im ) ; rtb_B_311_3271_0 [ 1 ] = muDoubleScalarHypot ( _rtB ->
NegSeqComputation_g . B_122_2_0 . re , _rtB -> NegSeqComputation_g .
B_122_2_0 . im ) ; rtb_B_311_3271_0 [ 2 ] = muDoubleScalarHypot ( _rtB ->
ZeroSeqComputation_b . B_124_1_0 . re , _rtB -> ZeroSeqComputation_b .
B_124_1_0 . im ) ; _rtB -> B_311_4789_0 = _rtP -> P_3553 * rtb_B_311_3271_0 [
0 ] ; } ssCallAccelRunBlock ( S , 311 , 4790 , SS_CALL_MDL_OUTPUTS ) ; i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { if ( ssIsMajorTimeStep ( S ) !=
0 ) { _rtDW -> RelationalOperator_Mode_h = ( _rtX -> Integrator_CSTATE_ai >
_rtB -> B_311_536_0 ) ; } _rtB -> B_311_4791_0 = _rtDW ->
RelationalOperator_Mode_h ; } _rtB -> B_311_4792_0 = _rtX ->
Integrator_CSTATE_ai - _rtB -> B_311_536_0 ; if ( ( _rtDW ->
Initial_FirstOutputTime_g == ( rtMinusInf ) ) || ( _rtDW ->
Initial_FirstOutputTime_g == ssGetTaskTime ( S , 0 ) ) ) { _rtDW ->
Initial_FirstOutputTime_g = ssGetTaskTime ( S , 0 ) ; _rtB -> B_311_4793_0 =
_rtP -> P_3554 ; } else { _rtB -> B_311_4793_0 = _rtB -> B_311_4792_0 ; } if
( ssIsMajorTimeStep ( S ) != 0 ) { rtb_B_311_3366_0 = ( _rtB -> B_311_4791_0
&& ( _rtZCE -> Integrator_Reset_ZCE_l != 1 ) ) ; _rtZCE ->
Integrator_Reset_ZCE_l = _rtB -> B_311_4791_0 ; if ( rtb_B_311_3366_0 || (
_rtDW -> Integrator_IWORK_h != 0 ) ) { _rtX -> Integrator_CSTATE_ai = _rtB ->
B_311_4793_0 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; } if ( _rtX ->
Integrator_CSTATE_ai >= _rtP -> P_3555 ) { if ( _rtX -> Integrator_CSTATE_ai
> _rtP -> P_3555 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
_rtX -> Integrator_CSTATE_ai = _rtP -> P_3555 ; } else { if ( _rtX ->
Integrator_CSTATE_ai <= _rtP -> P_3556 ) { if ( _rtX -> Integrator_CSTATE_ai
< _rtP -> P_3556 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
_rtX -> Integrator_CSTATE_ai = _rtP -> P_3556 ; } } } _rtB -> B_311_4794_0 =
_rtX -> Integrator_CSTATE_ai ;
Auto_microgrid_forecasting_020822_AutomaticGainControl ( S , _rtB ->
B_311_535_0 , _rtB -> B_311_4697_0 , _rtB -> B_311_4794_0 , _rtB ->
B_311_157_0 , & _rtB -> AutomaticGainControl_f , & _rtDW ->
AutomaticGainControl_f , & _rtP -> AutomaticGainControl_f , & _rtX ->
AutomaticGainControl_f , & ( ( XDis_Auto_microgrid_forecasting_020822_T * )
ssGetContStateDisabled ( S ) ) -> AutomaticGainControl_f ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtX -> Integrator_CSTATE_dl >= _rtP ->
P_3559 ) { if ( _rtX -> Integrator_CSTATE_dl > _rtP -> P_3559 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtX ->
Integrator_CSTATE_dl = _rtP -> P_3559 ; } else { if ( _rtX ->
Integrator_CSTATE_dl <= _rtP -> P_3560 ) { if ( _rtX -> Integrator_CSTATE_dl
< _rtP -> P_3560 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
_rtX -> Integrator_CSTATE_dl = _rtP -> P_3560 ; } } } _rtB -> B_311_4797_0 =
_rtX -> Integrator_CSTATE_dl ; { real_T * * uBuffer = ( real_T * * ) & _rtDW
-> VariableTransportDelay_PWORK_k . TUbufferPtrs [ 0 ] ; real_T * * tBuffer =
( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_k . TUbufferPtrs [ 1 ]
; real_T * * xBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK_k . TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT
( S ) ; real_T appliedDelay ; _rtB -> B_311_4798_0 =
Auto_microgrid_forecasting_020822_acc_rt_VTDelayfindtDInterpolate ( ( (
X_Auto_microgrid_forecasting_020822_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE_ht , * tBuffer , * uBuffer , * xBuffer , _rtDW
-> VariableTransportDelay_IWORK_f . CircularBufSize , _rtDW ->
VariableTransportDelay_IWORK_f . Head , _rtDW ->
VariableTransportDelay_IWORK_f . Tail , & _rtDW ->
VariableTransportDelay_IWORK_f . Last , simTime , 0.0 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
, _rtP -> P_3562 , & appliedDelay ) ; } _rtB -> B_311_4799_0 = _rtX ->
integrator_CSTATE_d ; _rtB -> B_311_4800_0 = ssGetT ( S ) ; i = ssIsSampleHit
( S , 1 , 0 ) ; if ( i != 0 ) { if ( ssIsMajorTimeStep ( S ) != 0 ) { _rtDW
-> RelationalOperator_Mode_js = ( _rtB -> B_311_4800_0 >= _rtB ->
B_311_537_0_g ) ; } _rtB -> B_311_4801_0 = _rtDW ->
RelationalOperator_Mode_js ; _rtB -> B_311_4802_0 = _rtDW ->
Memory_PreviousInput_a ; } if ( _rtB -> B_311_4801_0 ) { _rtB -> B_92_0_0 =
_rtB -> B_311_4799_0 - _rtB -> B_311_4798_0 ; _rtB -> B_92_1_0 = _rtB ->
B_92_0_0 * _rtB -> B_311_4697_0 ; _rtB -> B_311_4804_0 = _rtB -> B_92_1_0 ; }
else { _rtB -> B_311_4804_0 = _rtB -> B_311_4802_0 ; } _rtB -> B_311_4805_0 =
_rtB -> B_311_4804_0 * _rtB -> AutomaticGainControl_f . B_67_17_0 ; _rtB ->
B_311_4806_0 = _rtP -> P_3565 * _rtB -> B_311_4805_0 ; _rtB -> B_311_4807_0 =
_rtP -> P_3566 * _rtB -> B_311_4805_0 ; _rtB -> B_311_4808_0 = _rtP -> P_3567
* _rtB -> B_311_4805_0 ; _rtB -> B_311_4809_0 = 0.0 ; _rtB -> B_311_4809_0 +=
_rtP -> P_3569 * _rtX -> TransferFcn_CSTATE_a ; _rtB -> B_311_4809_0 += _rtP
-> P_3570 * _rtB -> B_311_4808_0 ; _rtB -> B_311_4810_0 = ( _rtB ->
B_311_4806_0 + _rtB -> B_311_4797_0 ) + _rtB -> B_311_4809_0 ; if (
ssIsMajorTimeStep ( S ) != 0 ) { _rtDW -> Saturation2_MODE_au = _rtB ->
B_311_4810_0 >= _rtP -> P_3571 ? 1 : _rtB -> B_311_4810_0 > _rtP -> P_3572 ?
0 : - 1 ; } _rtB -> B_311_4811_0 = _rtDW -> Saturation2_MODE_au == 1 ? _rtP
-> P_3571 : _rtDW -> Saturation2_MODE_au == - 1 ? _rtP -> P_3572 : _rtB ->
B_311_4810_0 ; _rtB -> B_311_4812_0 = _rtP -> P_3573 * _rtB -> B_311_4811_0 ;
if ( ( _rtDW -> LastMajorTimeA_n >= ssGetTaskTime ( S , 0 ) ) && ( _rtDW ->
LastMajorTimeB_cz >= ssGetTaskTime ( S , 0 ) ) ) { _rtB -> B_311_4813_0 =
_rtB -> B_311_4812_0 ; } else { if ( ( ( _rtDW -> LastMajorTimeA_n < _rtDW ->
LastMajorTimeB_cz ) && ( _rtDW -> LastMajorTimeB_cz < ssGetTaskTime ( S , 0 )
) ) || ( ( _rtDW -> LastMajorTimeA_n >= _rtDW -> LastMajorTimeB_cz ) && (
_rtDW -> LastMajorTimeA_n >= ssGetTaskTime ( S , 0 ) ) ) ) { rtb_B_311_62_0 =
ssGetTaskTime ( S , 0 ) - _rtDW -> LastMajorTimeB_cz ; B_311_1965_0_idx_0 =
_rtDW -> PrevYB_e ; } else { rtb_B_311_62_0 = ssGetTaskTime ( S , 0 ) - _rtDW
-> LastMajorTimeA_n ; B_311_1965_0_idx_0 = _rtDW -> PrevYA_c ; }
rtb_B_311_2022_0 = rtb_B_311_62_0 * _rtP -> P_3574 ; rtb_B_311_251_0 = _rtB
-> B_311_4812_0 - B_311_1965_0_idx_0 ; if ( rtb_B_311_251_0 >
rtb_B_311_2022_0 ) { _rtB -> B_311_4813_0 = B_311_1965_0_idx_0 +
rtb_B_311_2022_0 ; } else { rtb_B_311_62_0 *= _rtP -> P_3575 ; if (
rtb_B_311_251_0 < rtb_B_311_62_0 ) { _rtB -> B_311_4813_0 =
B_311_1965_0_idx_0 + rtb_B_311_62_0 ; } else { _rtB -> B_311_4813_0 = _rtB ->
B_311_4812_0 ; } } } _rtB -> B_311_4814_0 = _rtX -> Integrator_x1_CSTATE_g ;
_rtB -> B_311_4815_0 = _rtP -> P_3577 * _rtB -> B_311_4814_0 ; _rtB ->
B_311_4816_0 = _rtX -> Integrator_x2_CSTATE_jy ; _rtB -> B_311_4817_0 = _rtP
-> P_3579 * _rtB -> B_311_4816_0 ; _rtB -> B_311_4818_0 = _rtP -> P_3580 *
_rtB -> B_311_4814_0 ; _rtB -> B_311_4819_0 = _rtP -> P_3581 * _rtB ->
B_311_4816_0 ; _rtB -> B_311_4820_0 = _rtB -> B_311_4815_0 + _rtB ->
B_311_4817_0 ; _rtB -> B_311_4821_0 = _rtB -> B_311_4818_0 + _rtB ->
B_311_4819_0 ; _rtB -> B_311_4822_0 = _rtP -> P_3582 * _rtB -> B_311_4813_0 ;
_rtB -> B_311_4823_0 = _rtB -> B_311_4820_0 + _rtB -> B_311_4822_0 ; _rtB ->
B_311_4824_0 = _rtP -> P_3583 * _rtB -> B_311_4813_0 ; _rtB -> B_311_4825_0 =
_rtB -> B_311_4821_0 + _rtB -> B_311_4824_0 ; _rtB -> B_311_4830_0 = ( _rtP
-> P_3584 * _rtB -> B_311_4814_0 + _rtP -> P_3585 * _rtB -> B_311_4816_0 ) +
_rtP -> P_3586 * _rtB -> B_311_4813_0 ; if ( _rtB -> B_311_4697_0 > _rtP ->
P_3587 ) { _rtB -> B_311_4831_0 = _rtP -> P_3587 ; } else if ( _rtB ->
B_311_4697_0 < _rtP -> P_3588 ) { _rtB -> B_311_4831_0 = _rtP -> P_3588 ; }
else { _rtB -> B_311_4831_0 = _rtB -> B_311_4697_0 ; } _rtB -> B_311_4832_0 =
1.0 / _rtB -> B_311_4831_0 ; for ( i = 0 ; i < 3 ; i ++ ) { _rtB ->
B_311_4833_0 [ i ] = 0.0 ; _rtB -> B_311_4833_0 [ i ] += _rtP -> P_3589 [ i ]
* _rtB -> B_311_157_0 [ 0 ] ; _rtB -> B_311_4833_0 [ i ] += _rtP -> P_3589 [
i + 3 ] * _rtB -> B_311_157_0 [ 1 ] ; _rtB -> B_311_4833_0 [ i ] += _rtP ->
P_3589 [ i + 6 ] * _rtB -> B_311_157_0 [ 2 ] ; _rtB -> B_311_4834_0 [ i ] =
_rtP -> P_3590 * _rtB -> B_311_4833_0 [ i ] ; }
Auto_microgrid_forecasting_020822_Subsystempi2delay ( S , _rtB -> B_311_542_0
, & _rtB -> B_311_4834_0 [ 0 ] , _rtB -> B_311_4794_0 , & _rtB ->
Subsystempi2delay_nv , & _rtDW -> Subsystempi2delay_nv ) ;
Auto_microgrid_forecasting_020822_Subsystem1 ( S , _rtB -> B_311_540_0 , &
_rtB -> B_311_4834_0 [ 0 ] , _rtB -> B_311_4794_0 , & _rtB -> Subsystem1_a ,
& _rtDW -> Subsystem1_a ) ; if ( _rtB -> B_311_540_0 != 0 ) { _rtB ->
B_311_4837_0 [ 0 ] = _rtB -> Subsystem1_a . B_66_0_0 ; _rtB -> B_311_4837_0 [
1 ] = _rtB -> Subsystem1_a . B_66_1_0 ; } else { _rtB -> B_311_4837_0 [ 0 ] =
_rtB -> Subsystempi2delay_nv . B_65_0_0 ; _rtB -> B_311_4837_0 [ 1 ] = _rtB
-> Subsystempi2delay_nv . B_65_1_0 ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if (
i != 0 ) { if ( _rtB -> B_311_555_0 >= _rtP -> P_3594 ) { B_311_4123_0_idx_0
= _rtB -> B_311_562_0 ; } else { B_311_4123_0_idx_0 = rt_Lookup ( _rtP ->
P_3592 , 6 , ssGetTaskTime ( S , 2 ) , _rtP -> P_3593 ) ; } if ( _rtP ->
P_3591 >= _rtP -> P_3595 ) { _rtB -> B_311_5476_0 = ( ( B_311_4123_0_idx_0 !=
0.0 ) && _rtB -> B_311_561_0 ) ; } else { _rtB -> B_311_5476_0 = rt_Lookup (
_rtP -> P_345 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_346 ) ; } if ( _rtP
-> P_3596 >= _rtP -> P_3597 ) { _rtB -> B_311_5484_0 = ( ( B_311_4123_0_idx_0
!= 0.0 ) && _rtB -> B_311_560_0 ) ; } else { _rtB -> B_311_5484_0 = rt_Lookup
( _rtP -> P_347 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_348 ) ; } if ( _rtP
-> P_3598 >= _rtP -> P_3599 ) { _rtB -> B_311_5492_0 = ( ( B_311_4123_0_idx_0
!= 0.0 ) && _rtB -> B_311_559_0 ) ; } else { _rtB -> B_311_5492_0 = rt_Lookup
( _rtP -> P_349 , 5 , ssGetTaskTime ( S , 2 ) , _rtP -> P_350 ) ; } _rtB ->
B_311_5509_0 = _rtP -> P_3600 * rtb_B_5_3_0 ; rtb_B_311_3271_0 [ 0 ] = _rtB
-> B_311_152_0 [ 207 ] ; rtb_B_311_3271_0 [ 1 ] = _rtB -> B_311_152_0 [ 208 ]
; rtb_B_311_3271_0 [ 2 ] = B_311_104_0_idx_1 ; for ( i = 0 ; i < 5 ; i ++ ) {
rtb_B_311_8_0 [ i ] = ( ( rtb_B_311_140_0 [ i + 5 ] * _rtP -> P_3601 *
rtb_B_311_3271_0 [ 1 ] + _rtP -> P_3601 * rtb_B_311_140_0 [ i ] *
rtb_B_311_3271_0 [ 0 ] ) + rtb_B_311_140_0 [ i + 10 ] * _rtP -> P_3601 *
B_311_104_0_idx_1 ) + rtb_B_311_128_0 [ i ] ; } for ( i = 0 ; i < 5 ; i ++ )
{ rtb_B_311_5_0 [ i ] = 0.0 ; for ( i_0 = 0 ; i_0 < 5 ; i_0 ++ ) {
rtb_B_311_5_0 [ i ] += rtb_B_311_124_0 [ 5 * i_0 + i ] * rtb_B_311_8_0 [ i_0
] ; } rtb_B_311_5514_0 [ i ] = _rtP -> P_3602 * rtb_B_311_5_0 [ i ] ; } for (
i = 0 ; i < 5 ; i ++ ) { tmp [ i ] = 0.0 ; for ( i_0 = 0 ; i_0 < 5 ; i_0 ++ )
{ tmp [ i ] += _rtB -> B_311_574_0 [ 5 * i_0 + i ] * rtb_B_311_5514_0 [ i_0 ]
; } rtb_B_311_8_0 [ i ] = _rtP -> P_3603 [ i ] * tmp [ i ] ; }
B_311_104_0_idx_1 = rtb_B_311_8_0 [ 0 ] * rtb_B_311_100_0 + rtb_B_311_8_0 [ 1
] * rtb_B_0_0_0 ; rtb_B_0_0_0 = ( ( - rtb_B_311_8_0 [ 0 ] -
1.7320508075688772 * rtb_B_311_8_0 [ 1 ] ) * rtb_B_311_100_0 + (
1.7320508075688772 * rtb_B_311_8_0 [ 0 ] - rtb_B_311_8_0 [ 1 ] ) *
rtb_B_0_0_0 ) * 0.5 ; B_311_4123_0_idx_0 = rtb_B_311_133_0 [ 0 ] * _rtB ->
B_311_152_0 [ 207 ] + rtb_B_311_133_0 [ 5 ] * _rtB -> B_311_152_0 [ 208 ] ;
rtb_B_311_100_0 = rtb_B_311_133_0 [ 1 ] * _rtB -> B_311_152_0 [ 207 ] +
rtb_B_311_133_0 [ 6 ] * _rtB -> B_311_152_0 [ 208 ] ; B_311_146_0_0 [ 0 ] =
B_311_104_0_idx_1 ; B_311_146_0_0 [ 1 ] = rtb_B_0_0_0 ; B_311_146_0_0 [ 2 ] =
( 0.0 - B_311_104_0_idx_1 ) - rtb_B_0_0_0 ; B_311_146_0_0 [ 3 ] =
rtb_B_311_8_0 [ 0 ] ; B_311_146_0_0 [ 4 ] = rtb_B_311_8_0 [ 1 ] ;
B_311_146_0_0 [ 5 ] = rtb_B_311_8_0 [ 2 ] ; B_311_146_0_0 [ 6 ] =
rtb_B_311_8_0 [ 4 ] ; if ( _rtB -> B_311_571_0 ) { B_311_146_0_0 [ 7 ] =
rtb_B_311_8_0 [ 0 ] ; } else { B_311_146_0_0 [ 7 ] = _rtB -> B_311_572_0 ; }
B_311_146_0_0 [ 8 ] = rtb_B_311_8_0 [ 3 ] ; B_311_146_0_0 [ 9 ] = _rtP ->
P_3604 [ 0 ] * rtb_B_311_5514_0 [ 0 ] * _rtB -> B_311_578_0 [ 0 ] + _rtP ->
P_3604 [ 1 ] * rtb_B_311_5514_0 [ 4 ] * _rtB -> B_311_578_0 [ 0 ] ;
B_311_146_0_0 [ 10 ] = ( _rtP -> P_3605 [ 0 ] * rtb_B_311_5514_0 [ 1 ] * _rtB
-> B_311_578_0 [ 1 ] + _rtP -> P_3605 [ 1 ] * rtb_B_311_5514_0 [ 2 ] * _rtB
-> B_311_578_0 [ 1 ] ) + _rtP -> P_3605 [ 2 ] * rtb_B_311_5514_0 [ 3 ] * _rtB
-> B_311_578_0 [ 1 ] ; B_311_146_0_0 [ 11 ] = B_311_4123_0_idx_0 ;
B_311_146_0_0 [ 13 ] = _rtB -> B_311_580_0 [ 0 ] ; B_311_146_0_0 [ 12 ] =
rtb_B_311_100_0 ; B_311_146_0_0 [ 14 ] = _rtB -> B_311_580_0 [ 1 ] ;
B_311_146_0_0 [ 15 ] = muDoubleScalarAtan2 ( rtb_B_311_133_0 [ 1 ] * _rtB ->
B_311_152_0 [ 207 ] + rtb_B_311_133_0 [ 6 ] * _rtB -> B_311_152_0 [ 208 ] ,
rtb_B_311_133_0 [ 0 ] * _rtB -> B_311_152_0 [ 207 ] + rtb_B_311_133_0 [ 5 ] *
_rtB -> B_311_152_0 [ 208 ] ) * _rtP -> P_3606 ; B_311_146_0_0 [ 16 ] = (
B_311_4123_0_idx_0 * rtb_B_311_8_0 [ 0 ] + rtb_B_311_100_0 * rtb_B_311_8_0 [
1 ] ) * _rtP -> P_3607 ; B_311_146_0_0 [ 17 ] = ( B_311_4123_0_idx_0 *
rtb_B_311_8_0 [ 1 ] - rtb_B_311_100_0 * rtb_B_311_8_0 [ 0 ] ) * _rtP ->
P_3608 ; for ( i = 0 ; i < 18 ; i ++ ) { _rtB -> B_311_5540_0 [ i ] = _rtP ->
P_3609 [ i ] * B_311_146_0_0 [ i ] ; } } { real_T * * uBuffer = ( real_T * *
) & _rtDW -> ENGINETd_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> ENGINETd_PWORK . TUbufferPtrs [ 1 ] ; real_T simTime
= ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_3610 ; B_311_5541_0
= Auto_microgrid_forecasting_020822_acc_rt_TDelayInterpolate ( tMinusDelay ,
0.0 , * tBuffer , * uBuffer , _rtDW -> ENGINETd_IWORK . CircularBufSize , &
_rtDW -> ENGINETd_IWORK . Last , _rtDW -> ENGINETd_IWORK . Tail , _rtDW ->
ENGINETd_IWORK . Head , _rtP -> P_3611 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ssGetTimeOfLastOutput ( S ) == ssGetT ( S ) ) )
) ; } _rtB -> B_311_5542_0 = B_311_5541_0 * _rtB -> B_311_5509_0 ;
ssCallAccelRunBlock ( S , 311 , 5543 , SS_CALL_MDL_OUTPUTS ) ; i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { rtb_B_311_5546_0 = _rtP ->
P_3612 [ 0 ] * rtb_B_311_8_0 [ 0 ] * rtb_B_311_5514_0 [ 1 ] + _rtP -> P_3612
[ 1 ] * rtb_B_311_8_0 [ 1 ] * rtb_B_311_5514_0 [ 0 ] ; _rtB -> B_311_5548_0 =
rtb_B_5_3_0 * rtb_B_311_5546_0 * _rtP -> P_3613 ; ssCallAccelRunBlock ( S ,
311 , 5549 , SS_CALL_MDL_OUTPUTS ) ; } i = ssIsSampleHit ( S , 5 , 0 ) ; if (
i != 0 ) { ssCallAccelRunBlock ( S , 311 , 5550 , SS_CALL_MDL_OUTPUTS ) ; }
_rtB -> B_311_5551_0 = 0.0 ; _rtB -> B_311_5551_0 += _rtP -> P_3615 [ 0 ] *
_rtX -> CONTROLSYSTEM_CSTATE [ 0 ] ; _rtB -> B_311_5551_0 += _rtP -> P_3615 [
1 ] * _rtX -> CONTROLSYSTEM_CSTATE [ 1 ] ; _rtB -> B_311_5552_0 = _rtP ->
P_3616 * _rtB -> B_311_5551_0 ; if ( ssIsMajorTimeStep ( S ) != 0 ) { if (
_rtX -> Integrator_CSTATE_hq >= _rtP -> P_3618 ) { if ( _rtX ->
Integrator_CSTATE_hq > _rtP -> P_3618 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } _rtX ->
Integrator_CSTATE_hq = _rtP -> P_3618 ; } else { if ( _rtX ->
Integrator_CSTATE_hq <= _rtP -> P_3619 ) { if ( _rtX -> Integrator_CSTATE_hq
< _rtP -> P_3619 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
_rtX -> Integrator_CSTATE_hq = _rtP -> P_3619 ; } } } _rtB -> B_311_5553_0 =
_rtX -> Integrator_CSTATE_hq ; i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0
) { _rtB -> B_311_5554_0 = _rtB -> B_311_2019_0 - _rtB -> B_311_5509_0 ; }
_rtB -> B_311_5555_0 = 0.0 ; _rtB -> B_311_5555_0 += _rtP -> P_3621 * _rtX ->
TF1_CSTATE ; _rtB -> B_311_5555_0 += _rtP -> P_3622 * _rtB -> B_311_5552_0 ;
_rtB -> B_311_5556_0 = 0.0 ; _rtB -> B_311_5556_0 += _rtP -> P_3624 * _rtX ->
TF2_CSTATE ; i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
rtb_B_311_4106_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_e1 ;
B_311_4123_0_idx_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_bz ; rtb_B_0_0_0
= ( _rtDW -> DiscreteTimeIntegrator_DSTATE_e1 * look1_binlxpw ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_e1 , _rtP -> P_3636 , _rtP -> P_3635 , 2U ) +
_rtP -> P_3637 * _rtDW -> DiscreteTimeIntegrator_DSTATE_e1 ) + _rtP -> P_3638
* _rtB -> B_311_5540_0 [ 5 ] ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_bz
> _rtP -> P_3633 ) { rtb_B_299_3_0 = _rtP -> P_3633 ; } else if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_bz < _rtP -> P_3634 ) { rtb_B_299_3_0 = _rtP ->
P_3634 ; } else { rtb_B_299_3_0 = _rtDW -> DiscreteTimeIntegrator_DSTATE_bz ;
} _rtB -> B_311_5567_0 = ( rtb_B_299_3_0 - rtb_B_0_0_0 ) * _rtP -> P_3639 ;
rtb_B_311_68_0 = muDoubleScalarPower ( _rtB -> B_311_5540_0 [ 12 ] , 2.0 ) +
muDoubleScalarPower ( _rtB -> B_311_5540_0 [ 11 ] , 2.0 ) ; if (
rtb_B_311_68_0 < 0.0 ) { _rtB -> B_311_5568_0 = - muDoubleScalarSqrt ( -
rtb_B_311_68_0 ) ; } else { _rtB -> B_311_5568_0 = muDoubleScalarSqrt (
rtb_B_311_68_0 ) ; } { B_311_5569_0 = _rtP -> P_3642 * _rtDW ->
DiscreteStateSpace_DSTATE_iw ; B_311_5569_0 += _rtP -> P_3643 * _rtB ->
B_311_5568_0 ; } rtb_B_311_100_0 = _rtB -> B_311_5540_0 [ 5 ] / ( ( real_T )
( rtb_B_311_4106_0 == 0.0 ) * 2.2204460492503131e-16 + rtb_B_311_4106_0 ) *
_rtP -> P_3645 ; rtPrevAction = _rtDW -> SwitchCase_ActiveSubsystem_b ; if (
ssIsMajorTimeStep ( S ) != 0 ) { switch ( ( ( rtb_B_311_100_0 <= _rtB ->
B_311_567_0_f ) + ( rtb_B_311_100_0 < _rtB -> B_311_568_0_h ) ) + (
rtb_B_311_100_0 <= _rtB -> B_311_569_0 ) ) { case 1 : rtAction = 0 ; break ;
case 2 : rtAction = 1 ; break ; case 3 : rtAction = 2 ; break ; default :
rtAction = 3 ; break ; } _rtDW -> SwitchCase_ActiveSubsystem_b = rtAction ; }
else { rtAction = _rtDW -> SwitchCase_ActiveSubsystem_b ; } switch ( rtAction
) { case 0 : if ( rtAction != rtPrevAction ) { if ( ssGetTaskTime ( S , 2 )
!= ssGetTStart ( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
} i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { rtb_B_311_5582_0 = ( _rtB
-> B_307_0_0 - rtb_B_311_100_0 ) * _rtP -> P_577 ; } if ( ssIsMajorTimeStep (
S ) != 0 ) { srUpdateBC ( _rtDW -> u75In10_SubsysRanBC ) ; } break ; case 1 :
if ( rtAction != rtPrevAction ) { if ( ssGetTaskTime ( S , 2 ) != ssGetTStart
( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { rtb_B_311_5582_0 = _rtB ->
B_308_0_0 - rtb_B_311_100_0 * rtb_B_311_100_0 ; if ( rtb_B_311_5582_0 < 0.0 )
{ rtb_B_311_5582_0 = - muDoubleScalarSqrt ( muDoubleScalarAbs (
rtb_B_311_5582_0 ) ) ; } else { rtb_B_311_5582_0 = muDoubleScalarSqrt (
rtb_B_311_5582_0 ) ; } } if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC (
_rtDW -> u433In075_SubsysRanBC ) ; } break ; case 2 : if ( rtAction !=
rtPrevAction ) { if ( ssGetTaskTime ( S , 2 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } i = ssIsSampleHit ( S
, 2 , 0 ) ; if ( i != 0 ) { rtb_B_311_5582_0 = _rtB -> B_309_0_0 - _rtP ->
P_580 * rtb_B_311_100_0 ; } if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC
( _rtDW -> In0433_SubsysRanBC ) ; } break ; case 3 : if ( rtAction !=
rtPrevAction ) { if ( ssGetTaskTime ( S , 2 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } } i = ssIsSampleHit ( S
, 2 , 0 ) ; if ( i != 0 ) { rtb_B_311_5582_0 = _rtB -> B_310_0_0 ; } if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( _rtDW -> In10_SubsysRanBC ) ; }
break ; } _rtB -> B_311_5583_0 = rtb_B_311_4106_0 * rtb_B_311_5582_0 ;
rtb_B_311_5582_0 = _rtP -> P_3646 * rtb_B_0_0_0 ; rtb_B_311_4106_0 = ( ( _rtB
-> B_156_13_0 + _rtB -> B_311_570_0 ) - B_311_5569_0 ) - ( _rtP -> P_3647 *
rtb_B_311_5582_0 + _rtP -> P_3649 * _rtDW -> Delay_x_DSTATE_h ) ; _rtB ->
B_311_5593_0 = _rtP -> P_3651 * _rtDW -> Delay_x_DSTATE_p + _rtP -> P_3652 *
rtb_B_311_4106_0 ; _rtB -> B_311_5599_0 = ( ( _rtP -> P_3654 *
rtb_B_311_4106_0 + _rtP -> P_3653 * _rtDW -> Delay_x_DSTATE_p ) * _rtP ->
P_3655 - B_311_4123_0_idx_0 ) * _rtP -> P_3656 ; _rtB -> B_311_5602_0 = _rtP
-> P_3657 * _rtDW -> Delay_x_DSTATE_h + _rtP -> P_3658 * rtb_B_311_5582_0 ;
for ( i = 0 ; i < 25 ; i ++ ) { B_300_1_0 [ i ] = rtb_B_299_15_0 [ i ] * (
real_T ) _rtB -> B_311_577_0 + _rtB -> B_311_576_0 [ i ] ; } for ( i = 0 ; i
< 5 ; i ++ ) { for ( i_0 = 0 ; i_0 < 5 ; i_0 ++ ) { rtb_B_299_15_0 [ i + 5 *
i_0 ] = 0.0 ; for ( i_1 = 0 ; i_1 < 5 ; i_1 ++ ) { rtb_B_299_15_0 [ i + 5 *
i_0 ] += rtb_B_311_124_0 [ 5 * i_1 + i ] * B_300_1_0 [ 5 * i_0 + i_1 ] ; } }
rtb_B_311_5_0 [ i ] = 0.0 ; for ( i_0 = 0 ; i_0 < 3 ; i_0 ++ ) {
rtb_B_311_133_0 [ i + 5 * i_0 ] = 0.0 ; for ( i_1 = 0 ; i_1 < 5 ; i_1 ++ ) {
rtb_B_311_133_0 [ i + 5 * i_0 ] += rtb_B_311_124_0 [ 5 * i_1 + i ] *
rtb_B_311_140_0 [ 5 * i_0 + i_1 ] ; } rtb_B_311_5_0 [ i ] += rtb_B_311_133_0
[ 5 * i_0 + i ] * rtb_B_311_3271_0 [ i_0 ] ; } B_299_15_0 [ i ] = 0.0 ; for (
i_0 = 0 ; i_0 < 5 ; i_0 ++ ) { B_299_15_0 [ i ] += rtb_B_299_15_0 [ 5 * i_0 +
i ] * rtb_B_311_128_0 [ i_0 ] ; } _rtB -> B_311_5609_0 [ i ] = rtb_B_311_5_0
[ i ] + B_299_15_0 [ i ] ; } } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( i !=
0 ) && ( _rtDW -> RateTransition1_semaphoreTaken == 0 ) ) { _rtDW ->
RateTransition1_Buffer0 = _rtB -> B_311_5542_0 ; } i = ssIsSampleHit ( S , 2
, 0 ) ; if ( i != 0 ) { _rtDW -> RateTransition1_semaphoreTaken = 1 ;
rtb_B_311_5582_0 = _rtDW -> RateTransition1_Buffer0 ; _rtDW ->
RateTransition1_semaphoreTaken = 0 ; _rtB -> B_311_5615_0 = ( ( _rtP ->
P_3659 * rtb_B_311_5582_0 / rtb_B_5_3_0 - rtb_B_311_5546_0 ) - _rtP -> P_3660
* rtb_B_5_3_0 ) * _rtP -> P_3661 ; if ( _rtDW ->
Rotorspeeddeviationdw_SYSTEM_ENABLE_m != 0 ) { _rtB -> B_311_5616_0 = _rtDW
-> Rotorspeeddeviationdw_DSTATE_g ; } else { _rtB -> B_311_5616_0 = _rtP ->
P_3662 * _rtB -> B_311_5615_0 + _rtDW -> Rotorspeeddeviationdw_DSTATE_g ; }
_rtB -> B_311_5617_0 = _rtP -> P_3664 * rtb_B_5_7_0 ; } UNUSED_PARAMETER (
tid ) ; } static void mdlOutputsTID6 ( SimStruct * S , int_T tid ) { real_T
rtb_B_311_163_0 ; real_T rtb_B_311_178_0 ; int32_T i ;
B_Auto_microgrid_forecasting_020822_T * _rtB ;
P_Auto_microgrid_forecasting_020822_T * _rtP ;
DW_Auto_microgrid_forecasting_020822_T * _rtDW ; _rtDW = ( (
DW_Auto_microgrid_forecasting_020822_T * ) ssGetRootDWork ( S ) ) ; _rtP = (
( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) ; _rtB = (
( B_Auto_microgrid_forecasting_020822_T * ) _ssGetModelBlockIO ( S ) ) ;
memcpy ( & _rtB -> B_311_0_0 [ 0 ] , & _rtP -> P_3665 [ 0 ] , 37U * sizeof (
real_T ) ) ; _rtB -> B_311_1_0 = _rtP -> P_3666 ; _rtB -> B_311_2_0 = _rtP ->
P_3667 ; _rtB -> B_311_3_0 = _rtP -> P_3668 ; _rtB -> B_311_4_0 = _rtP ->
P_3669 ; _rtB -> B_311_5_0 = _rtP -> P_3670 ; _rtB -> B_311_6_0 = _rtP ->
P_3671 ; _rtB -> B_311_7_0 = _rtP -> P_3672 ; _rtB -> B_311_8_0 = _rtP ->
P_3673 ; _rtB -> B_311_9_0 = _rtP -> P_3674 ; _rtB -> B_311_10_0 = _rtP ->
P_3675 ; _rtB -> B_311_11_0 = _rtP -> P_3676 ; _rtB -> B_311_12_0 = _rtP ->
P_3677 ; _rtB -> B_311_13_0_m = _rtP -> P_3678 ; _rtB -> B_311_14_0_c = _rtP
-> P_3679 ; _rtB -> B_311_15_0_k = _rtP -> P_3680 ; _rtB -> B_311_16_0_c =
_rtP -> P_3681 ; _rtB -> B_311_17_0 = _rtP -> P_3682 ; _rtB -> B_311_18_0 =
_rtP -> P_3683 ; _rtB -> B_311_19_0 = _rtP -> P_3684 ; _rtB -> B_311_20_0 =
_rtP -> P_3685 ; _rtB -> B_311_21_0 = _rtP -> P_3686 ; _rtB -> B_311_22_0 =
_rtP -> P_3687 ; _rtB -> B_311_23_0 = _rtP -> P_3688 ; _rtB -> B_311_24_0 =
_rtP -> P_3689 ; _rtB -> B_311_25_0_b = _rtP -> P_3690 ; _rtB -> B_311_26_0 =
_rtP -> P_3691 ; _rtB -> B_311_27_0 = _rtP -> P_3692 ; _rtB -> B_311_28_0 =
_rtP -> P_3693 ; _rtB -> B_311_29_0 = _rtP -> P_3694 ; _rtB -> B_311_30_0 =
_rtP -> P_3695 ; _rtB -> B_311_31_0_p = _rtP -> P_3696 ; _rtB -> B_311_32_0 =
_rtP -> P_3697 ; _rtB -> B_311_33_0 = _rtP -> P_3698 ; _rtB -> B_311_34_0_c =
_rtP -> P_3699 ; _rtB -> B_311_35_0 = _rtP -> P_3700 ; _rtB -> B_311_36_0 =
_rtP -> P_3701 ; _rtB -> B_311_37_0 = _rtP -> P_3702 ; _rtB -> B_311_38_0_f =
_rtP -> P_3703 ; _rtB -> B_311_39_0 = _rtP -> P_3704 ; _rtB -> B_311_41_0_g =
_rtP -> P_3706 ; if ( _rtP -> P_3707 > _rtP -> P_3708 ) { _rtB ->
B_311_43_0_g = _rtP -> P_3708 ; } else if ( _rtP -> P_3707 < _rtP -> P_3709 )
{ _rtB -> B_311_43_0_g = _rtP -> P_3709 ; } else { _rtB -> B_311_43_0_g =
_rtP -> P_3707 ; } _rtB -> B_311_44_0 = _rtP -> P_3710 ; if ( _rtP -> P_3711
> _rtP -> P_3712 ) { _rtB -> B_311_46_0_m = _rtP -> P_3712 ; } else if ( _rtP
-> P_3711 < _rtP -> P_3713 ) { _rtB -> B_311_46_0_m = _rtP -> P_3713 ; } else
{ _rtB -> B_311_46_0_m = _rtP -> P_3711 ; } _rtB -> B_311_50_0 = _rtP ->
P_3717 ; if ( _rtP -> P_3718 > _rtP -> P_3719 ) { _rtB -> B_311_52_0 = _rtP
-> P_3719 ; } else if ( _rtP -> P_3718 < _rtP -> P_3720 ) { _rtB ->
B_311_52_0 = _rtP -> P_3720 ; } else { _rtB -> B_311_52_0 = _rtP -> P_3718 ;
} _rtB -> B_311_53_0 = _rtP -> P_3721 ; if ( _rtP -> P_3722 > _rtP -> P_3723
) { _rtB -> B_311_55_0 = _rtP -> P_3723 ; } else if ( _rtP -> P_3722 < _rtP
-> P_3724 ) { _rtB -> B_311_55_0 = _rtP -> P_3724 ; } else { _rtB ->
B_311_55_0 = _rtP -> P_3722 ; } _rtB -> B_311_58_0 = _rtP -> P_3727 ; _rtB ->
B_311_59_0 = _rtP -> P_3728 ; _rtB -> B_311_60_0 = _rtP -> P_3729 ; _rtB ->
B_311_61_0 = _rtP -> P_3730 ; _rtB -> B_311_62_0 = _rtP -> P_3731 ; _rtB ->
B_311_63_0_p = _rtP -> P_3732 ; _rtB -> B_311_64_0 = _rtP -> P_3733 ; _rtB ->
B_311_65_0 = _rtP -> P_3734 ; _rtB -> B_311_66_0 = _rtP -> P_3735 * _rtB ->
B_311_65_0 ; _rtB -> B_311_67_0 = _rtP -> P_3736 ; _rtB -> B_311_68_0 = _rtP
-> P_3737 ; _rtB -> B_311_70_0_l = _rtP -> P_3739 ; _rtB -> B_311_71_0 = _rtP
-> P_3740 ; _rtB -> B_311_72_0_j = _rtP -> P_3741 ; _rtB -> B_311_73_0_d =
_rtP -> P_3742 ; _rtB -> B_311_74_0_g = _rtP -> P_3743 ; _rtB -> B_311_75_0 =
_rtP -> P_3744 ; _rtB -> B_311_76_0_l = _rtP -> P_3745 ; _rtB -> B_311_77_0 =
_rtP -> P_3746 ; _rtB -> B_311_78_0_m = ! ( _rtB -> B_311_75_0 != 0.0 ) ;
_rtB -> B_311_79_0_d = _rtP -> P_3747 ; _rtB -> B_311_80_0_d = _rtP -> P_3748
; _rtB -> B_311_81_0 = _rtP -> P_3749 ; _rtB -> B_311_82_0 = _rtP -> P_3750 ;
_rtB -> B_311_83_0 = _rtP -> P_3751 ; _rtB -> B_311_84_0 = _rtP -> P_3752 ;
_rtB -> B_311_85_0 = _rtP -> P_3753 ; _rtB -> B_311_86_0 = _rtP -> P_3754 ;
_rtB -> B_311_87_0 = _rtP -> P_3755 ; _rtB -> B_311_88_0 = _rtP -> P_3756 ;
_rtB -> B_311_89_0 = _rtP -> P_3757 ; _rtB -> B_311_90_0 = _rtP -> P_3758 ;
_rtB -> B_311_91_0 = _rtP -> P_3759 ; _rtB -> B_311_92_0 = _rtP -> P_3760 ;
_rtB -> B_311_93_0 = _rtP -> P_3761 ; _rtB -> B_311_94_0_l = _rtP -> P_3762 ;
_rtB -> B_311_95_0 = _rtP -> P_3763 ; _rtB -> B_311_96_0 = _rtP -> P_3764 ;
_rtB -> B_311_98_0 = _rtP -> P_3766 ; if ( _rtP -> P_3767 > _rtP -> P_3768 )
{ _rtB -> B_311_100_0 = _rtP -> P_3768 ; } else if ( _rtP -> P_3767 < _rtP ->
P_3769 ) { _rtB -> B_311_100_0 = _rtP -> P_3769 ; } else { _rtB ->
B_311_100_0 = _rtP -> P_3767 ; } _rtB -> B_311_101_0 = _rtP -> P_3770 ; if (
_rtP -> P_3771 > _rtP -> P_3772 ) { _rtB -> B_311_103_0 = _rtP -> P_3772 ; }
else if ( _rtP -> P_3771 < _rtP -> P_3773 ) { _rtB -> B_311_103_0 = _rtP ->
P_3773 ; } else { _rtB -> B_311_103_0 = _rtP -> P_3771 ; } _rtB ->
B_311_107_0 = _rtP -> P_3777 ; if ( _rtP -> P_3778 > _rtP -> P_3779 ) { _rtB
-> B_311_109_0 = _rtP -> P_3779 ; } else if ( _rtP -> P_3778 < _rtP -> P_3780
) { _rtB -> B_311_109_0 = _rtP -> P_3780 ; } else { _rtB -> B_311_109_0 =
_rtP -> P_3778 ; } _rtB -> B_311_110_0 = _rtP -> P_3781 ; if ( _rtP -> P_3782
> _rtP -> P_3783 ) { _rtB -> B_311_112_0 = _rtP -> P_3783 ; } else if ( _rtP
-> P_3782 < _rtP -> P_3784 ) { _rtB -> B_311_112_0 = _rtP -> P_3784 ; } else
{ _rtB -> B_311_112_0 = _rtP -> P_3782 ; } _rtB -> B_311_115_0 = _rtP ->
P_3787 ; _rtB -> B_311_116_0 = _rtP -> P_3788 ; _rtB -> B_311_117_0 = _rtP ->
P_3789 ; _rtB -> B_311_118_0 = _rtP -> P_3790 ; _rtB -> B_311_119_0 = _rtP ->
P_3791 ; _rtB -> B_311_120_0 = _rtP -> P_3792 ; _rtB -> B_311_121_0 = _rtP ->
P_3793 ; _rtB -> B_311_122_0 = _rtP -> P_3794 ; _rtB -> B_311_123_0 = _rtP ->
P_3795 * _rtB -> B_311_122_0 ; _rtB -> B_311_124_0 = _rtP -> P_3796 ; _rtB ->
B_311_125_0 = _rtP -> P_3797 ; _rtB -> B_311_47_0 [ 0 ] = _rtP -> P_3714 [ 0
] * _rtP -> P_3705 ; _rtB -> B_311_48_0 [ 0 ] = _rtP -> P_3715 * _rtB ->
B_311_47_0 [ 0 ] ; _rtB -> B_311_56_0 [ 0 ] = _rtP -> P_3725 [ 0 ] * _rtP ->
P_3716 ; _rtB -> B_311_57_0_n [ 0 ] = _rtP -> P_3726 * _rtB -> B_311_56_0 [ 0
] ; _rtB -> B_311_69_0 [ 0 ] = _rtP -> P_3738 [ 0 ] ; _rtB -> B_311_104_0 [ 0
] = _rtP -> P_3774 [ 0 ] * _rtP -> P_3765 ; _rtB -> B_311_105_0 [ 0 ] = _rtP
-> P_3775 * _rtB -> B_311_104_0 [ 0 ] ; _rtB -> B_311_113_0_o [ 0 ] = _rtP ->
P_3785 [ 0 ] * _rtP -> P_3776 ; _rtB -> B_311_114_0 [ 0 ] = _rtP -> P_3786 *
_rtB -> B_311_113_0_o [ 0 ] ; _rtB -> B_311_126_0 [ 0 ] = _rtP -> P_3798 [ 0
] ; _rtB -> B_311_47_0 [ 1 ] = _rtP -> P_3714 [ 1 ] * _rtP -> P_3705 ; _rtB
-> B_311_48_0 [ 1 ] = _rtP -> P_3715 * _rtB -> B_311_47_0 [ 1 ] ; _rtB ->
B_311_56_0 [ 1 ] = _rtP -> P_3725 [ 1 ] * _rtP -> P_3716 ; _rtB ->
B_311_57_0_n [ 1 ] = _rtP -> P_3726 * _rtB -> B_311_56_0 [ 1 ] ; _rtB ->
B_311_69_0 [ 1 ] = _rtP -> P_3738 [ 1 ] ; _rtB -> B_311_104_0 [ 1 ] = _rtP ->
P_3774 [ 1 ] * _rtP -> P_3765 ; _rtB -> B_311_105_0 [ 1 ] = _rtP -> P_3775 *
_rtB -> B_311_104_0 [ 1 ] ; _rtB -> B_311_113_0_o [ 1 ] = _rtP -> P_3785 [ 1
] * _rtP -> P_3776 ; _rtB -> B_311_114_0 [ 1 ] = _rtP -> P_3786 * _rtB ->
B_311_113_0_o [ 1 ] ; _rtB -> B_311_126_0 [ 1 ] = _rtP -> P_3798 [ 1 ] ; _rtB
-> B_311_127_0 = _rtP -> P_3799 ; _rtB -> B_311_128_0 = _rtP -> P_3800 ; _rtB
-> B_311_129_0 = _rtP -> P_3801 ; _rtB -> B_311_130_0 = _rtP -> P_3802 ; _rtB
-> B_311_131_0 = _rtP -> P_3803 ; _rtB -> B_311_132_0 = _rtP -> P_3804 ; _rtB
-> B_311_133_0 = _rtP -> P_3805 ; _rtB -> B_311_134_0 = _rtP -> P_3806 ; _rtB
-> B_311_135_0 = ! ( _rtB -> B_311_132_0 != 0.0 ) ; _rtB -> B_311_136_0 =
_rtP -> P_3807 ; _rtB -> B_311_137_0 = _rtP -> P_3808 ; _rtB -> B_311_138_0 =
_rtP -> P_3809 ; _rtB -> B_311_139_0 = _rtP -> P_3810 ; _rtB -> B_311_140_0 =
_rtP -> P_3811 ; _rtB -> B_311_141_0 = _rtP -> P_3812 ; _rtB -> B_311_142_0 =
_rtP -> P_3813 ; _rtB -> B_311_143_0 = _rtP -> P_3814 ; _rtB -> B_311_144_0_b
= _rtP -> P_3815 ; _rtB -> B_311_145_0_n = _rtP -> P_3816 ; _rtB ->
B_311_146_0 = _rtP -> P_3817 ; _rtB -> B_311_147_0 = _rtP -> P_3818 ; _rtB ->
B_311_148_0_b = _rtP -> P_3819 ; _rtB -> B_311_149_0_l = _rtP -> P_3820 ;
_rtB -> B_311_150_0 = _rtP -> P_3821 ; _rtB -> B_311_151_0_h = _rtP -> P_3822
; _rtB -> B_311_152_0_b = _rtP -> P_3823 ; _rtB -> B_311_153_0_d = _rtP ->
P_3824 ; _rtB -> B_311_154_0 = _rtP -> P_3825 * _rtB -> B_311_93_0 ; _rtB ->
B_311_155_0 = _rtP -> P_3826 ; _rtB -> B_311_156_0 = _rtP -> P_3827 ; _rtB ->
B_311_157_0_e = _rtP -> P_3828 ; _rtB -> B_311_158_0 = _rtP -> P_3829 ; _rtB
-> B_311_159_0_b = _rtP -> P_3830 ; rtb_B_311_163_0 = _rtP -> P_3831 + _rtP
-> P_3832 ; _rtB -> B_311_169_0_j = ( ( rtb_B_311_163_0 - _rtP -> P_3833 ) *
_rtP -> P_3834 + _rtP -> P_3835 ) * _rtP -> P_3836 ; _rtB -> B_311_170_0 =
_rtP -> P_3837 ; rtb_B_311_178_0 = rtb_B_311_163_0 / _rtP -> P_3842 ; _rtB ->
B_311_186_0 = muDoubleScalarExp ( _rtP -> P_3844 - ( ( rtb_B_311_163_0 - _rtP
-> P_3839 ) * _rtP -> P_3840 + _rtP -> P_3838 ) * _rtP -> P_3841 / ( _rtP ->
P_3845 * rtb_B_311_163_0 ) ) * ( _rtP -> P_3843 * muDoubleScalarPower (
rtb_B_311_178_0 , 3.0 ) ) ; _rtB -> B_311_187_0 = _rtP -> P_3846 ; _rtB ->
B_311_188_0_f = _rtP -> P_3847 ; _rtB -> B_311_189_0_a = _rtP -> P_3848 *
rtb_B_311_178_0 ; _rtB -> B_311_190_0_j = _rtP -> P_3849 ; _rtB ->
B_311_191_0_j = _rtP -> P_3850 ; _rtB -> B_311_192_0 = _rtP -> P_3851 ; _rtB
-> B_311_193_0 = _rtP -> P_3852 ; _rtB -> B_311_194_0 = _rtP -> P_3853 ; _rtB
-> B_311_195_0 = _rtP -> P_3854 ; _rtB -> B_311_196_0 = _rtP -> P_3855 ; _rtB
-> B_311_197_0 = _rtP -> P_3856 ; _rtB -> B_311_199_0 = _rtP -> P_3858 ; _rtB
-> B_311_200_0_o = _rtP -> P_3859 ; _rtB -> B_311_201_0_n = _rtP -> P_3860 ;
_rtB -> B_311_202_0_i = _rtP -> P_3861 * _rtP -> P_3857 ; _rtB ->
B_311_203_0_a = ! ( _rtB -> B_311_197_0 != 0.0 ) ; _rtB -> B_311_204_0 = _rtP
-> P_3862 ; _rtB -> B_311_205_0 = _rtP -> P_3863 ; _rtB -> B_311_206_0 = _rtP
-> P_3864 ; _rtB -> B_311_207_0 = _rtP -> P_3865 ; _rtB -> B_311_208_0 = _rtP
-> P_3866 ; _rtB -> B_311_209_0 = _rtP -> P_3867 ; _rtB -> B_311_210_0_o =
_rtP -> P_3868 ; _rtB -> B_311_211_0 = _rtP -> P_3869 ; _rtB -> B_311_212_0 =
_rtP -> P_3870 ; _rtB -> B_311_214_0 = _rtP -> P_3872 ; _rtB -> B_311_215_0 =
_rtP -> P_3873 ; _rtB -> B_311_216_0 = _rtP -> P_3874 ; _rtB -> B_311_217_0_n
= _rtP -> P_3875 * _rtP -> P_3871 ; _rtB -> B_311_218_0_k = ! ( _rtB ->
B_311_212_0 != 0.0 ) ; _rtB -> B_311_219_0_m = _rtP -> P_3876 ; _rtB ->
B_311_220_0_c = _rtP -> P_3877 ;
Auto_microgrid_forecasting_020822_AutomaticGainControlTID6 ( S , & _rtB ->
AutomaticGainControl_c , & _rtDW -> AutomaticGainControl_c , & _rtP ->
AutomaticGainControl_c ) ; _rtB -> B_311_222_0 = _rtP -> P_3878 ; _rtB ->
B_311_223_0 = _rtP -> P_3879 ; _rtB -> B_311_224_0 = _rtP -> P_3880 ; _rtB ->
B_311_227_0_f = ( uint8_T ) ( _rtP -> P_3881 == _rtP -> P_3882 ) ; _rtB ->
B_311_229_0_i = ( uint8_T ) ( _rtP -> P_3881 == _rtP -> P_3883 ) ;
Auto_microgrid_forecasting_020822_AutomaticGainControlTID6 ( S , & _rtB ->
AutomaticGainControl_e , & _rtDW -> AutomaticGainControl_e , & _rtP ->
AutomaticGainControl_e ) ; _rtB -> B_311_231_0_m = _rtP -> P_3884 ; _rtB ->
B_311_232_0_m = _rtP -> P_3885 ; _rtB -> B_311_233_0 = _rtP -> P_3886 ; _rtB
-> B_311_236_0 = ( uint8_T ) ( _rtP -> P_3887 == _rtP -> P_3888 ) ; _rtB ->
B_311_238_0 = ( uint8_T ) ( _rtP -> P_3887 == _rtP -> P_3889 ) ; _rtB ->
B_311_239_0_j = _rtP -> P_3890 ; _rtB -> B_311_240_0 = _rtP -> P_3891 ; _rtB
-> B_311_241_0 = _rtP -> P_3892 ; _rtB -> B_311_242_0 = _rtP -> P_3893 ; _rtB
-> B_311_243_0 = _rtP -> P_3894 ; _rtB -> B_311_244_0 = _rtP -> P_3895 ; _rtB
-> B_311_245_0 = _rtP -> P_3896 ; _rtB -> B_311_246_0 = _rtP -> P_3897 ; _rtB
-> B_311_247_0 = _rtP -> P_3898 ; _rtB -> B_311_248_0 = _rtP -> P_3899 ; _rtB
-> B_311_249_0_h = _rtP -> P_3900 ; _rtB -> B_311_250_0_c = _rtP -> P_3901 ;
_rtB -> B_311_251_0 = _rtP -> P_3902 ; _rtB -> B_311_252_0 = _rtP -> P_3903 ;
_rtB -> B_311_253_0 = _rtP -> P_3904 ; _rtB -> B_311_254_0_c = _rtP -> P_3905
; _rtB -> B_311_255_0_p = _rtP -> P_3906 ; _rtB -> B_311_256_0 = _rtP ->
P_3907 ; _rtB -> B_311_257_0_p = _rtP -> P_3908 ; _rtB -> B_311_258_0 = _rtP
-> P_3909 ; _rtB -> B_311_259_0 = _rtP -> P_3910 ; _rtB -> B_311_260_0 = _rtP
-> P_3911 ; _rtB -> B_311_261_0_a = _rtP -> P_3912 ; _rtB -> B_311_262_0 =
_rtP -> P_3913 ; _rtB -> B_311_263_0_e = _rtP -> P_3914 ; _rtB ->
B_311_264_0_a = _rtP -> P_3915 ; _rtB -> B_311_265_0_a = _rtP -> P_3916 ;
_rtB -> B_311_266_0_i = _rtP -> P_3917 ; _rtB -> B_311_267_0 = _rtP -> P_3918
; _rtB -> B_311_268_0 = _rtP -> P_3919 ; _rtB -> B_311_269_0 = _rtP -> P_3920
; _rtB -> B_311_270_0 = _rtP -> P_3921 ; _rtB -> B_311_271_0 = _rtP -> P_3922
; _rtB -> B_311_272_0 = _rtP -> P_3923 ; _rtB -> B_311_273_0_l = _rtP ->
P_3924 ; _rtB -> B_311_274_0 = _rtP -> P_3925 ; _rtB -> B_311_275_0_o = _rtP
-> P_3926 ; _rtB -> B_311_276_0_o = _rtP -> P_3927 ; _rtB -> B_311_277_0_i =
_rtP -> P_3928 ; _rtB -> B_311_278_0_f = _rtP -> P_3929 ; _rtB -> B_311_279_0
= _rtP -> P_3930 ; _rtB -> B_311_280_0 = _rtP -> P_3931 ; _rtB -> B_311_281_0
= _rtP -> P_3932 ; _rtB -> B_311_282_0 = _rtP -> P_3933 ; _rtB -> B_156_0_0 =
_rtP -> P_295 ; _rtB -> B_156_1_0_p = _rtP -> P_296 ; _rtB -> B_156_2_0_m =
_rtP -> P_297 ; _rtB -> B_311_284_0 = _rtP -> P_3934 ; _rtB -> B_311_285_0_i
= _rtP -> P_3935 ; _rtB -> B_311_286_0 = _rtP -> P_3936 ; _rtB -> B_311_287_0
= _rtP -> P_3937 ; _rtB -> B_311_288_0 = _rtP -> P_3938 ; _rtB -> B_212_1_0 =
_rtP -> P_409 ; _rtB -> B_212_2_0 = _rtP -> P_410 ; _rtB -> B_212_3_0 = _rtP
-> P_411 ; _rtB -> B_212_4_0 = _rtP -> P_412 ; _rtB -> B_212_5_0_d = _rtP ->
P_413 ; _rtB -> B_208_0_0 = _rtP -> P_384 ; _rtB -> B_208_1_0_a = _rtP ->
P_385 ; _rtB -> B_208_2_0 = _rtP -> P_386 ; _rtB -> B_208_3_0 = _rtP -> P_387
; _rtB -> B_208_4_0 = _rtP -> P_388 ; _rtB -> B_208_5_0 = _rtP -> P_389 ;
_rtB -> B_208_6_0 = _rtP -> P_390 ; _rtB -> B_209_0_0 = _rtP -> P_397 ; _rtB
-> B_209_1_0 = _rtP -> P_398 ; _rtB -> B_210_0_0 = _rtP -> P_399 ; _rtB ->
B_210_1_0 = _rtP -> P_400 ; _rtB -> B_211_0_0 = _rtP -> P_401 ; _rtB ->
B_311_290_0 = _rtP -> P_3939 ; _rtB -> B_311_291_0 = _rtP -> P_3940 ; _rtB ->
B_311_292_0_f = _rtP -> P_3941 ; _rtB -> B_311_293_0_g = _rtP -> P_3942 ;
_rtB -> B_311_294_0_p = _rtP -> P_4272 ; _rtB -> B_311_295_0_c = _rtP ->
P_3943 ; _rtB -> B_311_296_0 = _rtP -> P_3944 ; _rtB -> B_311_297_0 = _rtP ->
P_3945 ; _rtB -> B_311_298_0 = _rtP -> P_3946 ; _rtB -> B_311_299_0 = _rtP ->
P_3947 ; _rtB -> B_311_300_0 = _rtP -> P_3948 ; _rtB -> B_311_301_0 = _rtP ->
P_3949 ; _rtB -> B_311_302_0_o = _rtP -> P_3950 ; _rtB -> B_311_303_0 = _rtP
-> P_3951 ; _rtB -> B_311_304_0_l = _rtP -> P_3952 ; _rtB -> B_311_305_0_m =
_rtP -> P_3953 ; _rtB -> B_311_306_0_m = _rtP -> P_3954 ; _rtB -> B_219_0_0 =
_rtP -> P_422 ; switch ( ( int32_T ) _rtP -> P_3955 ) { case 1 : _rtB ->
B_311_312_0 [ 0 ] = _rtP -> P_3956 [ 0 ] ; _rtB -> B_311_312_0 [ 1 ] = _rtP
-> P_3956 [ 1 ] ; break ; case 2 : _rtB -> B_311_312_0 [ 0 ] = _rtP -> P_3957
[ 0 ] ; _rtB -> B_311_312_0 [ 1 ] = _rtP -> P_3957 [ 1 ] ; break ; default :
_rtB -> B_311_312_0 [ 0 ] = _rtP -> P_3958 [ 0 ] ; _rtB -> B_311_312_0 [ 1 ]
= _rtP -> P_3958 [ 1 ] ; break ; } switch ( ( int32_T ) _rtP -> P_3959 ) {
case 1 : _rtB -> B_311_317_0 [ 0 ] = _rtP -> P_3960 [ 0 ] ; _rtB ->
B_311_317_0 [ 1 ] = _rtP -> P_3960 [ 1 ] ; break ; case 2 : _rtB ->
B_311_317_0 [ 0 ] = _rtP -> P_3961 [ 0 ] ; _rtB -> B_311_317_0 [ 1 ] = _rtP
-> P_3961 [ 1 ] ; break ; default : _rtB -> B_311_317_0 [ 0 ] = _rtP ->
P_3962 [ 0 ] ; _rtB -> B_311_317_0 [ 1 ] = _rtP -> P_3962 [ 1 ] ; break ; }
switch ( ( int32_T ) _rtP -> P_3963 ) { case 1 : _rtB -> B_311_322_0_c [ 0 ]
= _rtP -> P_3964 [ 0 ] ; _rtB -> B_311_322_0_c [ 1 ] = _rtP -> P_3964 [ 1 ] ;
break ; case 2 : _rtB -> B_311_322_0_c [ 0 ] = _rtP -> P_3965 [ 0 ] ; _rtB ->
B_311_322_0_c [ 1 ] = _rtP -> P_3965 [ 1 ] ; break ; default : _rtB ->
B_311_322_0_c [ 0 ] = _rtP -> P_3966 [ 0 ] ; _rtB -> B_311_322_0_c [ 1 ] =
_rtP -> P_3966 [ 1 ] ; break ; } _rtB -> B_311_323_0_f = _rtP -> P_3967 ;
_rtB -> B_311_324_0_p = _rtP -> P_3968 ; _rtB -> B_311_325_0 = _rtP -> P_3969
; _rtB -> B_311_330_0 = _rtP -> P_3970 ; _rtB -> B_311_333_0_o = ( uint8_T )
( _rtP -> P_3971 == _rtP -> P_3972 ) ; _rtB -> B_311_335_0_k = ( uint8_T ) (
_rtP -> P_3971 == _rtP -> P_3973 ) ; _rtB -> B_311_336_0_e = _rtP -> P_3974 ;
_rtB -> B_311_337_0 = _rtP -> P_3975 ; _rtB -> B_311_340_0 = ( uint8_T ) (
_rtP -> P_3976 == _rtP -> P_3977 ) ; _rtB -> B_311_342_0 = ( uint8_T ) ( _rtP
-> P_3976 == _rtP -> P_3978 ) ; _rtB -> B_311_345_0 = ( uint8_T ) ( _rtP ->
P_3979 == _rtP -> P_3980 ) ; _rtB -> B_311_347_0 = ( uint8_T ) ( _rtP ->
P_3979 == _rtP -> P_3981 ) ; _rtB -> B_311_348_0 = _rtP -> P_3982 ; _rtB ->
B_311_349_0 = _rtP -> P_3983 ; _rtB -> B_311_352_0 = ( uint8_T ) ( _rtP ->
P_3984 == _rtP -> P_3985 ) ; _rtB -> B_311_354_0 = ( uint8_T ) ( _rtP ->
P_3984 == _rtP -> P_3986 ) ; _rtB -> B_311_355_0 = _rtP -> P_3987 ; _rtB ->
B_311_356_0 = _rtP -> P_3988 ; _rtB -> B_311_357_0 = _rtP -> P_3989 ; _rtB ->
B_311_358_0 = _rtP -> P_3990 ; _rtB -> B_311_361_0 = ( uint8_T ) ( _rtP ->
P_3991 == _rtP -> P_3992 ) ; _rtB -> B_311_363_0 = ( uint8_T ) ( _rtP ->
P_3991 == _rtP -> P_3993 ) ; _rtB -> B_311_364_0 = _rtP -> P_3994 ; _rtB ->
B_311_365_0 = _rtP -> P_3995 ; _rtB -> B_311_366_0_o = _rtP -> P_3996 ; _rtB
-> B_311_367_0 = _rtP -> P_3997 ; _rtB -> B_311_368_0 = _rtP -> P_3998 ;
Auto_microgrid_forecasting_020822_CounterASTfmaxTID6 ( S , & _rtB ->
CounterASTfmax , & _rtP -> CounterASTfmax ) ;
Auto_microgrid_forecasting_020822_CounterASTfmaxTID6 ( S , & _rtB ->
CounterASTfmin , & _rtP -> CounterASTfmin ) ;
Auto_microgrid_forecasting_020822_CounterASTfmaxTID6 ( S , & _rtB ->
CounterASTumax , & _rtP -> CounterASTumax ) ;
Auto_microgrid_forecasting_020822_CounterASTfmaxTID6 ( S , & _rtB ->
CounterASTumin , & _rtP -> CounterASTumin ) ; _rtB -> B_311_373_0 = _rtP ->
P_3999 ; _rtB -> B_311_374_0 = _rtP -> P_4000 ; _rtB -> B_311_375_0 = _rtP ->
P_4001 ; _rtB -> B_311_376_0 = _rtP -> P_4002 ; _rtB -> B_311_377_0_h = _rtP
-> P_4003 ; _rtB -> B_271_0_0 = _rtP -> P_486 ; _rtB -> B_271_1_0 = _rtP ->
P_487 ; _rtB -> B_271_4_0_c = ( uint8_T ) ( _rtP -> P_488 == _rtP -> P_489 )
; _rtB -> B_271_6_0 = ( uint8_T ) ( _rtP -> P_488 == _rtP -> P_490 ) ; if (
ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( _rtDW ->
AutomaticGainControl_SubsysRanBC_d ) ; } _rtB -> B_311_379_0 = _rtP -> P_4004
; _rtB -> B_311_382_0 = ( uint8_T ) ( _rtP -> P_4005 == _rtP -> P_4006 ) ;
_rtB -> B_311_384_0 = ( uint8_T ) ( _rtP -> P_4005 == _rtP -> P_4007 ) ; _rtB
-> B_311_385_0 = _rtP -> P_4008 ; _rtB -> B_311_386_0_l = _rtP -> P_4009 ;
_rtB -> B_311_389_0 = ( uint8_T ) ( _rtP -> P_4010 == _rtP -> P_4011 ) ; _rtB
-> B_311_391_0 = ( uint8_T ) ( _rtP -> P_4010 == _rtP -> P_4012 ) ; _rtB ->
B_311_394_0_i = ( uint8_T ) ( _rtP -> P_4013 == _rtP -> P_4014 ) ; _rtB ->
B_311_396_0 = ( uint8_T ) ( _rtP -> P_4013 == _rtP -> P_4015 ) ; _rtB ->
B_311_399_0 = ( uint8_T ) ( _rtP -> P_4016 == _rtP -> P_4017 ) ; _rtB ->
B_311_401_0 = ( uint8_T ) ( _rtP -> P_4016 == _rtP -> P_4018 ) ; _rtB ->
B_311_402_0 = _rtP -> P_4019 ; _rtB -> B_311_405_0 = _rtP -> P_4020 ; _rtB ->
B_311_406_0 = _rtP -> P_4021 ; _rtB -> B_292_0_0 = _rtP -> P_534 ; _rtB ->
B_292_1_0 = _rtP -> P_535 ; _rtB -> B_292_4_0_m = ( uint8_T ) ( _rtP -> P_536
== _rtP -> P_537 ) ; _rtB -> B_292_6_0 = ( uint8_T ) ( _rtP -> P_536 == _rtP
-> P_538 ) ; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( _rtDW ->
AutomaticGainControl_SubsysRanBC ) ; } _rtB -> B_311_408_0 = _rtP -> P_4022 ;
_rtB -> B_311_411_0_o = ( uint8_T ) ( _rtP -> P_4023 == _rtP -> P_4024 ) ;
_rtB -> B_311_413_0 = ( uint8_T ) ( _rtP -> P_4023 == _rtP -> P_4025 ) ; _rtB
-> B_311_414_0 = _rtP -> P_4026 ; _rtB -> B_311_415_0 = _rtP -> P_4027 ; _rtB
-> B_311_416_0 = _rtP -> P_4028 ; _rtB -> B_299_0_0 = _rtP -> P_4257 ; _rtB
-> B_299_1_0 = _rtP -> P_4258 ; _rtB -> B_299_2_0 = _rtP -> P_558 ; _rtB ->
B_299_3_0 [ 0 ] = _rtP -> P_559 [ 0 ] ; _rtB -> B_299_3_0 [ 1 ] = _rtP ->
P_559 [ 1 ] ; _rtB -> B_298_0_0 = _rtP -> P_547 ; _rtB -> B_298_1_0 [ 0 ] =
_rtP -> P_548 [ 0 ] ; _rtB -> B_298_1_0 [ 1 ] = _rtP -> P_548 [ 1 ] ; _rtB ->
B_298_1_0 [ 2 ] = _rtP -> P_548 [ 2 ] ; _rtB -> B_299_8_0 = _rtP -> P_563 ;
memcpy ( & _rtB -> B_311_417_0 [ 0 ] , & _rtP -> P_4029 [ 0 ] , 25U * sizeof
( real_T ) ) ; memcpy ( & _rtB -> B_311_418_0 [ 0 ] , & _rtP -> P_4030 [ 0 ]
, 25U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_311_419_0 [ 0 ] , & _rtP
-> P_4031 [ 0 ] , 25U * sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_299_5_0 [
0 ] , & _rtP -> P_560 [ 0 ] , 25U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_299_6_0 [ 0 ] , & _rtP -> P_561 [ 0 ] , 25U * sizeof ( real_T ) ) ; memcpy
( & _rtB -> B_299_7_0 [ 0 ] , & _rtP -> P_562 [ 0 ] , 25U * sizeof ( real_T )
) ; memcpy ( & _rtB -> B_311_421_0 [ 0 ] , & _rtP -> P_4032 [ 0 ] , 25U *
sizeof ( real_T ) ) ; _rtB -> B_311_422_0 [ 0 ] = _rtP -> P_4033 [ 0 ] ; _rtB
-> B_311_422_0 [ 1 ] = _rtP -> P_4033 [ 1 ] ; _rtB -> B_311_423_0 = _rtP ->
P_4034 ; for ( i = 0 ; i < 6 ; i ++ ) { _rtB -> B_311_424_0_h [ i ] = _rtP ->
P_4035 [ i ] ; } _rtB -> B_311_425_0_m = _rtP -> P_4036 ; _rtB -> B_311_426_0
= _rtP -> P_4037 ; _rtB -> B_311_427_0 = _rtP -> P_4038 ; _rtB -> B_311_428_0
= _rtP -> P_4039 ; _rtB -> B_311_429_0 = _rtP -> P_4040 ; _rtB -> B_311_430_0
= _rtP -> P_4041 ; _rtB -> B_311_431_0 = _rtP -> P_4042 ; _rtB ->
B_311_432_0_m = _rtP -> P_4043 ; _rtB -> B_311_433_0 = _rtP -> P_4044 ; _rtB
-> B_311_434_0 = _rtP -> P_4045 ; _rtB -> B_311_435_0 = _rtP -> P_4046 ; _rtB
-> B_311_436_0 = _rtP -> P_4047 ; _rtB -> B_311_437_0 = _rtP -> P_4048 ; _rtB
-> B_311_438_0 = _rtP -> P_4049 ; _rtB -> B_311_440_0 = _rtP -> P_4051 ; if (
_rtP -> P_4052 > _rtP -> P_4053 ) { _rtB -> B_311_442_0_h = _rtP -> P_4053 ;
} else if ( _rtP -> P_4052 < _rtP -> P_4054 ) { _rtB -> B_311_442_0_h = _rtP
-> P_4054 ; } else { _rtB -> B_311_442_0_h = _rtP -> P_4052 ; } _rtB ->
B_311_443_0 = _rtP -> P_4055 ; if ( _rtP -> P_4056 > _rtP -> P_4057 ) { _rtB
-> B_311_445_0 = _rtP -> P_4057 ; } else if ( _rtP -> P_4056 < _rtP -> P_4058
) { _rtB -> B_311_445_0 = _rtP -> P_4058 ; } else { _rtB -> B_311_445_0 =
_rtP -> P_4056 ; } _rtB -> B_311_449_0_c = _rtP -> P_4062 ; if ( _rtP ->
P_4063 > _rtP -> P_4064 ) { _rtB -> B_311_451_0 = _rtP -> P_4064 ; } else if
( _rtP -> P_4063 < _rtP -> P_4065 ) { _rtB -> B_311_451_0 = _rtP -> P_4065 ;
} else { _rtB -> B_311_451_0 = _rtP -> P_4063 ; } _rtB -> B_311_452_0 = _rtP
-> P_4066 ; if ( _rtP -> P_4067 > _rtP -> P_4068 ) { _rtB -> B_311_454_0 =
_rtP -> P_4068 ; } else if ( _rtP -> P_4067 < _rtP -> P_4069 ) { _rtB ->
B_311_454_0 = _rtP -> P_4069 ; } else { _rtB -> B_311_454_0 = _rtP -> P_4067
; } _rtB -> B_311_457_0 = _rtP -> P_4072 ; _rtB -> B_311_458_0_k = _rtP ->
P_4073 ; _rtB -> B_311_459_0_p = _rtP -> P_4074 ; _rtB -> B_311_460_0 = _rtP
-> P_4075 ; _rtB -> B_311_461_0 = _rtP -> P_4076 ; _rtB -> B_311_462_0 = _rtP
-> P_4077 ; _rtB -> B_311_463_0 = _rtP -> P_4078 ; _rtB -> B_311_464_0 = _rtP
-> P_4079 ; _rtB -> B_311_465_0 = _rtP -> P_4080 * _rtB -> B_311_464_0 ; _rtB
-> B_311_466_0_p = _rtP -> P_4081 ; _rtB -> B_311_467_0 = _rtP -> P_4082 ;
_rtB -> B_311_446_0 [ 0 ] = _rtP -> P_4059 [ 0 ] * _rtP -> P_4050 ; _rtB ->
B_311_447_0 [ 0 ] = _rtP -> P_4060 * _rtB -> B_311_446_0 [ 0 ] ; _rtB ->
B_311_455_0 [ 0 ] = _rtP -> P_4070 [ 0 ] * _rtP -> P_4061 ; _rtB ->
B_311_456_0 [ 0 ] = _rtP -> P_4071 * _rtB -> B_311_455_0 [ 0 ] ; _rtB ->
B_311_468_0 [ 0 ] = _rtP -> P_4083 [ 0 ] ; _rtB -> B_311_446_0 [ 1 ] = _rtP
-> P_4059 [ 1 ] * _rtP -> P_4050 ; _rtB -> B_311_447_0 [ 1 ] = _rtP -> P_4060
* _rtB -> B_311_446_0 [ 1 ] ; _rtB -> B_311_455_0 [ 1 ] = _rtP -> P_4070 [ 1
] * _rtP -> P_4061 ; _rtB -> B_311_456_0 [ 1 ] = _rtP -> P_4071 * _rtB ->
B_311_455_0 [ 1 ] ; _rtB -> B_311_468_0 [ 1 ] = _rtP -> P_4083 [ 1 ] ; _rtB
-> B_311_469_0 = _rtP -> P_4084 ; _rtB -> B_311_470_0 = _rtP -> P_4085 ; _rtB
-> B_311_471_0 = _rtP -> P_4086 ; _rtB -> B_311_472_0 = _rtP -> P_4087 ; _rtB
-> B_311_473_0 = _rtP -> P_4088 ; _rtB -> B_311_474_0 = _rtP -> P_4089 ; _rtB
-> B_311_475_0 = _rtP -> P_4090 ; _rtB -> B_311_476_0 = _rtP -> P_4091 ; _rtB
-> B_311_477_0 = ! ( _rtB -> B_311_474_0 != 0.0 ) ; _rtB -> B_311_478_0 =
_rtP -> P_4092 ; _rtB -> B_311_479_0_p = _rtP -> P_4093 ; _rtB -> B_311_480_0
= _rtP -> P_4094 ; _rtB -> B_311_481_0 = _rtP -> P_4095 ; _rtB -> B_311_482_0
= _rtP -> P_4096 ; _rtB -> B_311_483_0_a = _rtP -> P_4097 ; _rtB ->
B_311_484_0 = _rtP -> P_4098 ; _rtB -> B_311_485_0 = _rtP -> P_4099 ; _rtB ->
B_311_486_0 = _rtP -> P_4100 ; _rtB -> B_311_487_0 = _rtP -> P_4101 ; _rtB ->
B_311_488_0 = _rtP -> P_4102 ; _rtB -> B_311_489_0 = _rtP -> P_4103 ; _rtB ->
B_311_490_0 = _rtP -> P_4104 ; _rtB -> B_311_491_0 = _rtP -> P_4105 ; _rtB ->
B_311_492_0_j = _rtP -> P_4106 ; _rtB -> B_311_493_0 = _rtP -> P_4107 ; _rtB
-> B_311_494_0 = _rtP -> P_4108 ; _rtB -> B_311_495_0_e = _rtP -> P_4109 ;
Auto_microgrid_forecasting_020822_AutomaticGainControlTID6 ( S , & _rtB ->
AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl ) ; _rtB -> B_311_497_0 = _rtP -> P_4110 ; _rtB ->
B_311_498_0 = _rtP -> P_4111 ; _rtB -> B_311_499_0 = _rtP -> P_4112 ; _rtB ->
B_311_502_0 = ( uint8_T ) ( _rtP -> P_4113 == _rtP -> P_4114 ) ; _rtB ->
B_311_504_0 = ( uint8_T ) ( _rtP -> P_4113 == _rtP -> P_4115 ) ; _rtB ->
B_311_505_0 = _rtP -> P_4116 ; _rtB -> B_311_506_0 = _rtP -> P_4117 ; _rtB ->
B_311_507_0 = _rtP -> P_4118 ; _rtB -> B_311_508_0 = _rtP -> P_4119 ; _rtB ->
B_311_509_0 = _rtP -> P_4120 ; _rtB -> B_311_510_0 = _rtP -> P_4121 ; _rtB ->
B_311_511_0 = _rtP -> P_4122 ; _rtB -> B_311_512_0_o = _rtP -> P_4123 ; _rtB
-> B_311_513_0_b = _rtP -> P_4124 ; _rtB -> B_311_514_0 = _rtP -> P_4125 ;
_rtB -> B_311_515_0 = _rtP -> P_4126 ; _rtB -> B_311_516_0 = _rtP -> P_4127 ;
_rtB -> B_311_517_0 = _rtP -> P_4128 ; _rtB -> B_311_518_0 = _rtP -> P_4129 ;
_rtB -> B_311_519_0 = _rtP -> P_4130 ; _rtB -> B_311_520_0_a = _rtP -> P_4131
; _rtB -> B_311_521_0 = _rtP -> P_4132 ; _rtB -> B_311_522_0 = _rtP -> P_4133
; _rtB -> B_311_523_0 = _rtP -> P_4134 ; _rtB -> B_311_524_0 = _rtP -> P_4135
; _rtB -> B_311_525_0 = _rtP -> P_4136 ; _rtB -> B_311_526_0 = _rtP -> P_4137
; _rtB -> B_311_527_0 = _rtP -> P_4138 ; _rtB -> B_311_529_0_b = ( _rtP ->
P_4139 != 0.0 ) ; _rtB -> B_311_530_0_c = ! _rtB -> B_311_529_0_b ; _rtB ->
B_311_532_0 = ( _rtP -> P_4140 != 0.0 ) ; _rtB -> B_311_533_0 = ! _rtB ->
B_311_532_0 ; Auto_microgrid_forecasting_020822_AutomaticGainControlTID6 ( S
, & _rtB -> AutomaticGainControl_f , & _rtDW -> AutomaticGainControl_f , &
_rtP -> AutomaticGainControl_f ) ; _rtB -> B_311_535_0 = _rtP -> P_4141 ;
_rtB -> B_311_536_0 = _rtP -> P_4142 ; _rtB -> B_311_537_0_g = _rtP -> P_4143
; _rtB -> B_311_540_0 = ( uint8_T ) ( _rtP -> P_4144 == _rtP -> P_4145 ) ;
_rtB -> B_311_542_0 = ( uint8_T ) ( _rtP -> P_4144 == _rtP -> P_4146 ) ; _rtB
-> B_311_543_0 = _rtP -> P_4147 ; _rtB -> B_311_544_0 = _rtP -> P_4148 ; _rtB
-> B_311_545_0 = _rtP -> P_4149 ; _rtB -> B_311_547_0 = _rtP -> P_4151 * _rtP
-> P_4150 ; _rtB -> B_311_549_0 = _rtP -> P_4153 * _rtP -> P_4152 ; _rtB ->
B_0_1_0 = _rtP -> P_11 * _rtP -> P_10 ; switch ( ( int32_T ) ( _rtP -> P_12 +
_rtP -> P_13 ) ) { case 1 : _rtB -> B_0_8_0 [ 0 ] = _rtP -> P_14 [ 0 ] ; _rtB
-> B_0_8_0 [ 1 ] = _rtP -> P_14 [ 1 ] ; _rtB -> B_0_8_0 [ 2 ] = _rtP -> P_14
[ 2 ] ; break ; case 2 : _rtB -> B_0_8_0 [ 0 ] = _rtP -> P_15 [ 0 ] ; _rtB ->
B_0_8_0 [ 1 ] = _rtP -> P_15 [ 1 ] ; _rtB -> B_0_8_0 [ 2 ] = _rtP -> P_15 [ 2
] ; break ; default : _rtB -> B_0_8_0 [ 0 ] = _rtP -> P_16 [ 0 ] ; _rtB ->
B_0_8_0 [ 1 ] = _rtP -> P_16 [ 1 ] ; _rtB -> B_0_8_0 [ 2 ] = _rtP -> P_16 [ 2
] ; break ; } _rtB -> B_0_9_0 = _rtP -> P_17 ; _rtB -> B_0_11_0 = _rtP ->
P_19 * _rtP -> P_18 ; switch ( ( int32_T ) ( _rtP -> P_20 + _rtP -> P_21 ) )
{ case 1 : _rtB -> B_0_18_0 [ 0 ] = _rtP -> P_22 [ 0 ] ; _rtB -> B_0_18_0 [ 1
] = _rtP -> P_22 [ 1 ] ; _rtB -> B_0_18_0 [ 2 ] = _rtP -> P_22 [ 2 ] ; break
; case 2 : _rtB -> B_0_18_0 [ 0 ] = _rtP -> P_23 [ 0 ] ; _rtB -> B_0_18_0 [ 1
] = _rtP -> P_23 [ 1 ] ; _rtB -> B_0_18_0 [ 2 ] = _rtP -> P_23 [ 2 ] ; break
; default : _rtB -> B_0_18_0 [ 0 ] = _rtP -> P_24 [ 0 ] ; _rtB -> B_0_18_0 [
1 ] = _rtP -> P_24 [ 1 ] ; _rtB -> B_0_18_0 [ 2 ] = _rtP -> P_24 [ 2 ] ;
break ; } _rtB -> B_0_19_0 = _rtP -> P_25 ; if ( ssIsMajorTimeStep ( S ) != 0
) { srUpdateBC ( _rtDW -> HarmonicGenerator_SubsysRanBC ) ; } _rtB ->
B_311_551_0_e [ 0 ] = _rtP -> P_4154 [ 0 ] ; _rtB -> B_311_551_0_e [ 1 ] =
_rtP -> P_4154 [ 1 ] ; _rtB -> B_311_551_0_e [ 2 ] = _rtP -> P_4154 [ 2 ] ;
_rtB -> B_5_1_0_o = _rtP -> P_48 ; _rtB -> B_5_4_0 = _rtP -> P_51 ; _rtB ->
B_5_5_0 = _rtP -> P_52 ; _rtB -> B_5_8_0_i = ( _rtP -> P_54 == _rtP -> P_47 )
; _rtB -> B_5_9_0 = ( _rtP -> P_54 == _rtP -> P_49 ) ; _rtB -> B_5_10_0 = (
_rtP -> P_54 == _rtP -> P_50 ) ; _rtB -> B_5_11_0_n = _rtP -> P_55 ; _rtB ->
B_5_12_0 = ( _rtB -> B_5_11_0_n == _rtP -> P_53 ) ; _rtB -> B_5_14_0_l = _rtP
-> P_57 ; _rtB -> B_5_17_0_p = _rtP -> P_60 ; _rtB -> B_5_18_0 = _rtP -> P_61
; _rtB -> B_5_20_0_p = _rtP -> P_63 * _rtP -> P_62 ; _rtB -> B_5_22_0 = (
_rtB -> B_5_11_0_n == _rtP -> P_59 ) ; if ( _rtB -> B_5_11_0_n == _rtP ->
P_56 ) { _rtB -> B_5_24_0 = _rtP -> P_64 ; } else { _rtB -> B_5_24_0 = _rtP
-> P_58 ; } _rtB -> B_5_25_0 = _rtP -> P_65 ; _rtB -> B_5_26_0 = _rtP -> P_66
; if ( ssIsMajorTimeStep ( S ) != 0 ) { srUpdateBC ( _rtDW ->
Signalgenerator_SubsysRanBC ) ; } _rtB -> B_311_553_0 = _rtP -> P_4155 ; _rtB
-> B_311_554_0 = _rtP -> P_4156 ; _rtB -> B_311_555_0 = _rtP -> P_4157 ; _rtB
-> B_311_559_0 = ( _rtP -> P_4160 != 0.0 ) ; _rtB -> B_311_560_0 = ( _rtP ->
P_4159 != 0.0 ) ; _rtB -> B_311_561_0 = ( _rtP -> P_4158 != 0.0 ) ; _rtB ->
B_311_562_0 = _rtP -> P_4161 ; _rtB -> B_311_567_0_f = _rtP -> P_4162 ; _rtB
-> B_311_568_0_h = _rtP -> P_4163 ; _rtB -> B_311_569_0 = _rtP -> P_4164 ;
_rtB -> B_311_570_0 = _rtP -> P_4165 ; _rtB -> B_311_571_0 = _rtP -> P_4273 ;
_rtB -> B_311_572_0 = _rtP -> P_4166 ; memcpy ( & _rtB -> B_311_573_0 [ 0 ] ,
& _rtP -> P_4167 [ 0 ] , 25U * sizeof ( real_T ) ) ; memcpy ( & _rtB ->
B_311_574_0 [ 0 ] , & _rtP -> P_4168 [ 0 ] , 25U * sizeof ( real_T ) ) ;
memcpy ( & _rtB -> B_311_575_0_e [ 0 ] , & _rtP -> P_4169 [ 0 ] , 25U *
sizeof ( real_T ) ) ; memcpy ( & _rtB -> B_311_576_0 [ 0 ] , & _rtP -> P_4170
[ 0 ] , 25U * sizeof ( real_T ) ) ; _rtB -> B_311_577_0 = _rtP -> P_4274 ;
_rtB -> B_311_578_0 [ 0 ] = _rtP -> P_4171 [ 0 ] ; _rtB -> B_311_578_0 [ 1 ]
= _rtP -> P_4171 [ 1 ] ; for ( i = 0 ; i < 5 ; i ++ ) { _rtB -> B_311_579_0 [
i << 1 ] = _rtB -> B_311_574_0 [ 5 * i ] ; _rtB -> B_311_579_0 [ ( i << 1 ) +
1 ] = _rtB -> B_311_574_0 [ 5 * i + 1 ] ; } _rtB -> B_311_580_0 [ 0 ] = _rtP
-> P_4172 [ 0 ] ; _rtB -> B_311_580_0 [ 1 ] = _rtP -> P_4172 [ 1 ] ; _rtB ->
B_311_582_0 [ 0 ] = _rtP -> P_4174 [ 0 ] ; _rtB -> B_311_582_0 [ 1 ] = _rtP
-> P_4174 [ 1 ] ; _rtB -> B_311_582_0 [ 2 ] = _rtP -> P_4174 [ 2 ] ; _rtB ->
B_311_582_0 [ 3 ] = _rtP -> P_4174 [ 3 ] ; memcpy ( & _rtB -> B_311_584_0_c [
0 ] , & _rtP -> P_4175 [ 0 ] , 15U * sizeof ( real_T ) ) ; _rtB ->
B_311_584_0_c [ 12 ] = _rtP -> P_4173 ; _rtB -> B_311_585_0_a = _rtP ->
P_4176 ; Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystemTID6 ( S
, & _rtB -> SwitchCaseActionSubsystem , & _rtP -> SwitchCaseActionSubsystem )
; Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystemTID6 ( S , &
_rtB -> SwitchCaseActionSubsystem1 , & _rtP -> SwitchCaseActionSubsystem1 ) ;
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2TID6 ( S , & _rtB
-> SwitchCaseActionSubsystem2 , & _rtP -> SwitchCaseActionSubsystem2 ) ;
Auto_microgrid_forecasting_020822_SwitchCaseActionSubsystem2TID6 ( S , & _rtB
-> SwitchCaseActionSubsystem3 , & _rtP -> SwitchCaseActionSubsystem3 ) ; _rtB
-> B_283_2_0 = _rtP -> P_495 * _rtP -> P_494 ; _rtB -> B_285_0_0 = _rtP ->
P_496 ; _rtB -> B_307_0_0 = _rtP -> P_578 ; _rtB -> B_308_0_0 = _rtP -> P_579
; _rtB -> B_309_0_0 = _rtP -> P_581 ; _rtB -> B_310_0_0 = _rtP -> P_582 ;
UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { real_T HoldSine ;
int32_T i ; B_Auto_microgrid_forecasting_020822_T * _rtB ;
P_Auto_microgrid_forecasting_020822_T * _rtP ;
X_Auto_microgrid_forecasting_020822_T * _rtX ;
DW_Auto_microgrid_forecasting_020822_T * _rtDW ; _rtDW = ( (
DW_Auto_microgrid_forecasting_020822_T * ) ssGetRootDWork ( S ) ) ; _rtX = (
( X_Auto_microgrid_forecasting_020822_T * ) ssGetContStates ( S ) ) ; _rtP =
( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) ; _rtB =
( ( B_Auto_microgrid_forecasting_020822_T * ) _ssGetModelBlockIO ( S ) ) ; i
= ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtDW ->
Rotorangledthetae_DSTATE += _rtP -> P_591 * _rtB -> B_311_3924_0 ; for ( i =
0 ; i < 5 ; i ++ ) { _rtDW -> fluxes_DSTATE [ i ] = _rtB -> B_311_3917_0 [ i
] ; } if ( _rtB -> B_311_414_0 > 0.0 ) { i = ssIsSampleHit ( S , 2 , 0 ) ; if
( i != 0 ) { _rtDW -> Lmd_sat_DSTATE = _rtB -> B_299_11_0 ; if ( _rtB ->
B_299_0_0 ) { i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtDW ->
Lmq_sat_DSTATE = _rtB -> B_298_11_0 ; } } } } } i = ssIsSampleHit ( S , 3 , 0
) ; if ( i != 0 ) { _rtDW -> RateTransition3_Buffer0 [ 0 ] = _rtB ->
B_311_1745_0 [ 0 ] ; _rtDW -> RateTransition3_Buffer0_h [ 0 ] = _rtB ->
B_311_2150_0 [ 0 ] ; _rtDW -> RateTransition3_Buffer0_b [ 0 ] = _rtB ->
B_311_4310_0 [ 0 ] ; _rtDW -> RateTransition3_Buffer0 [ 1 ] = _rtB ->
B_311_1745_0 [ 1 ] ; _rtDW -> RateTransition3_Buffer0_h [ 1 ] = _rtB ->
B_311_2150_0 [ 1 ] ; _rtDW -> RateTransition3_Buffer0_b [ 1 ] = _rtB ->
B_311_4310_0 [ 1 ] ; _rtDW -> RateTransition3_Buffer0 [ 2 ] = _rtB ->
B_311_1745_0 [ 2 ] ; _rtDW -> RateTransition3_Buffer0_h [ 2 ] = _rtB ->
B_311_2150_0 [ 2 ] ; _rtDW -> RateTransition3_Buffer0_b [ 2 ] = _rtB ->
B_311_4310_0 [ 2 ] ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE += _rtP -> P_601 * _rtB ->
B_311_3860_0 ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE >= _rtP -> P_603 )
{ _rtDW -> DiscreteTimeIntegrator_DSTATE = _rtP -> P_603 ; } else { if (
_rtDW -> DiscreteTimeIntegrator_DSTATE <= _rtP -> P_604 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE = _rtP -> P_604 ; } } _rtDW -> UnitDelay_DSTATE
[ 0 ] = _rtB -> B_311_3549_0 [ 0 ] ; _rtDW -> UnitDelay_DSTATE [ 1 ] = _rtB
-> B_311_3549_0 [ 1 ] ; _rtDW -> UnitDelay_DSTATE [ 2 ] = _rtB ->
B_311_3549_0 [ 2 ] ; _rtDW -> UnitDelay5_DSTATE = _rtB -> B_311_3799_0 ; } i
= ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW ->
itinit1_PreviousInput = _rtB -> B_311_22_0 ; } i = ssIsSampleHit ( S , 2 , 0
) ; if ( i != 0 ) { _rtDW -> Currentfilter_states = ( _rtB -> B_311_1450_0 -
_rtP -> P_613 [ 1 ] * _rtDW -> Currentfilter_states ) / _rtP -> P_613 [ 0 ] ;
} i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW ->
itinit_PreviousInput = _rtB -> B_311_1461_0 ; } i = ssIsSampleHit ( S , 2 , 0
) ; if ( i != 0 ) { _rtDW -> inti_IC_LOADING = 0U ; _rtDW -> inti_DSTATE +=
_rtP -> P_616 * _rtB -> B_311_1450_0 ; if ( _rtDW -> inti_DSTATE >= _rtP ->
P_617 ) { _rtDW -> inti_DSTATE = _rtP -> P_617 ; } else { if ( _rtDW ->
inti_DSTATE <= _rtP -> P_618 ) { _rtDW -> inti_DSTATE = _rtP -> P_618 ; } }
if ( _rtB -> B_311_40_0 > 0.0 ) { _rtDW -> inti_PrevResetState = 1 ; } else
if ( _rtB -> B_311_40_0 < 0.0 ) { _rtDW -> inti_PrevResetState = - 1 ; } else
if ( _rtB -> B_311_40_0 == 0.0 ) { _rtDW -> inti_PrevResetState = 0 ; } else
{ _rtDW -> inti_PrevResetState = 2 ; } _rtDW ->
DiscreteTimeIntegrator_DSTATE_a += _rtP -> P_621 * _rtB -> B_311_1460_0 ;
_rtDW -> Memory2_PreviousInput = _rtB -> B_311_1462_0 ; } i = ssIsSampleHit (
S , 3 , 0 ) ; if ( i != 0 ) { _rtDW -> UnitDelayToBreakTheLoop_DSTATE = _rtB
-> B_311_2543_0 ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtDW
-> UnitDelayToBreakTheLoop_DSTATE_f = _rtB -> B_311_1481_0 ; _rtDW ->
UnitDelay_DSTATE_p = _rtB -> B_311_2533_0 ; if ( _rtDW ->
Signalgenerator_MODE ) { i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE_m1 += _rtP -> P_33 * _rtB -> B_5_14_0
; _rtDW -> DiscreteTimeIntegrator1_DSTATE_i += _rtP -> P_41 * _rtB ->
B_5_20_0 ; _rtDW -> UnitDelay_DSTATE_h5 = _rtB -> B_5_11_0 ; } } _rtDW ->
Rotorangledthetae_DSTATE_g += _rtP -> P_633 * _rtB -> B_311_5617_0 ; _rtDW ->
dw_delay_DSTATE = _rtB -> B_311_5616_0 ; _rtDW -> dw_predict_DSTATE = _rtB ->
B_311_113_0 ; for ( i = 0 ; i < 5 ; i ++ ) { _rtDW -> UnitDelay_DSTATE_f [ i
] = _rtB -> B_311_5609_0 [ i ] ; } _rtDW -> UnitDelay_DSTATE_k = _rtB ->
B_311_5583_0 ; _rtDW -> UnitDelay_DSTATE_pe [ 0 ] = _rtB -> B_311_149_0 [ 0 ]
; _rtDW -> UnitDelay_DSTATE_pe [ 1 ] = _rtB -> B_311_149_0 [ 1 ] ; _rtDW ->
UnitDelay_DSTATE_pe [ 2 ] = _rtB -> B_311_149_0 [ 2 ] ; _rtDW ->
UnitDelay_DSTATE_pe [ 3 ] = _rtB -> B_311_149_0 [ 3 ] ; ssCallAccelRunBlock (
S , 311 , 152 , SS_CALL_MDL_UPDATE ) ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if
( i != 0 ) { _rtDW -> Memory_PreviousInput = _rtB -> B_311_2818_0 ; } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { HoldSine = _rtDW -> lastSin ;
_rtDW -> lastSin = _rtDW -> lastSin * _rtP -> P_712 + _rtDW -> lastCos * _rtP
-> P_711 ; _rtDW -> lastCos = _rtDW -> lastCos * _rtP -> P_712 - HoldSine *
_rtP -> P_711 ; _rtDW -> Integ4_SYSTEM_ENABLE = 0U ; _rtDW -> Integ4_DSTATE =
_rtP -> P_715 * _rtB -> B_311_159_0 + _rtB -> B_311_160_0 ;
ssCallAccelRunBlock ( S , 311 , 162 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_kk = _rtB -> B_311_159_0 ; _rtDW -> UnitDelay1_DSTATE = _rtB
-> B_311_169_0 ; HoldSine = _rtDW -> lastSin_b ; _rtDW -> lastSin_b = _rtDW
-> lastSin_b * _rtP -> P_733 + _rtDW -> lastCos_k * _rtP -> P_732 ; _rtDW ->
lastCos_k = _rtDW -> lastCos_k * _rtP -> P_733 - HoldSine * _rtP -> P_732 ;
_rtDW -> Integ4_SYSTEM_ENABLE_c = 0U ; _rtDW -> Integ4_DSTATE_g = _rtP ->
P_736 * _rtB -> B_311_171_0 + _rtB -> B_311_172_0 ; ssCallAccelRunBlock ( S ,
311 , 174 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_fl = _rtB ->
B_311_171_0 ; _rtDW -> UnitDelay1_DSTATE_h = _rtB -> B_311_181_0 ; HoldSine =
_rtDW -> lastSin_bv ; _rtDW -> lastSin_bv = _rtDW -> lastSin_bv * _rtP ->
P_756 + _rtDW -> lastCos_c * _rtP -> P_755 ; _rtDW -> lastCos_c = _rtDW ->
lastCos_c * _rtP -> P_756 - HoldSine * _rtP -> P_755 ; _rtDW ->
Integ4_SYSTEM_ENABLE_l = 0U ; _rtDW -> Integ4_DSTATE_j = _rtP -> P_759 * _rtB
-> B_311_188_0 + _rtB -> B_311_189_0 ; ssCallAccelRunBlock ( S , 311 , 191 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_fli = _rtB -> B_311_188_0 ;
_rtDW -> UnitDelay1_DSTATE_b = _rtB -> B_311_198_0 ; HoldSine = _rtDW ->
lastSin_p ; _rtDW -> lastSin_p = _rtDW -> lastSin_p * _rtP -> P_777 + _rtDW
-> lastCos_l * _rtP -> P_776 ; _rtDW -> lastCos_l = _rtDW -> lastCos_l * _rtP
-> P_777 - HoldSine * _rtP -> P_776 ; _rtDW -> Integ4_SYSTEM_ENABLE_a = 0U ;
_rtDW -> Integ4_DSTATE_n = _rtP -> P_780 * _rtB -> B_311_200_0 + _rtB ->
B_311_201_0 ; ssCallAccelRunBlock ( S , 311 , 203 , SS_CALL_MDL_UPDATE ) ;
_rtDW -> UnitDelay_DSTATE_g = _rtB -> B_311_200_0 ; _rtDW ->
UnitDelay1_DSTATE_d = _rtB -> B_311_210_0 ; HoldSine = _rtDW -> lastSin_a ;
_rtDW -> lastSin_a = _rtDW -> lastSin_a * _rtP -> P_800 + _rtDW -> lastCos_m
* _rtP -> P_799 ; _rtDW -> lastCos_m = _rtDW -> lastCos_m * _rtP -> P_800 -
HoldSine * _rtP -> P_799 ; _rtDW -> Integ4_SYSTEM_ENABLE_f = 0U ; _rtDW ->
Integ4_DSTATE_gk = _rtP -> P_803 * _rtB -> B_311_217_0 + _rtB -> B_311_218_0
; ssCallAccelRunBlock ( S , 311 , 220 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_o = _rtB -> B_311_217_0 ; _rtDW -> UnitDelay1_DSTATE_j =
_rtB -> B_311_227_0 ; HoldSine = _rtDW -> lastSin_c ; _rtDW -> lastSin_c =
_rtDW -> lastSin_c * _rtP -> P_821 + _rtDW -> lastCos_p * _rtP -> P_820 ;
_rtDW -> lastCos_p = _rtDW -> lastCos_p * _rtP -> P_821 - HoldSine * _rtP ->
P_820 ; _rtDW -> Integ4_SYSTEM_ENABLE_d = 0U ; _rtDW -> Integ4_DSTATE_h =
_rtP -> P_824 * _rtB -> B_311_229_0 + _rtB -> B_311_230_0 ;
ssCallAccelRunBlock ( S , 311 , 232 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_b = _rtB -> B_311_229_0 ; _rtDW -> UnitDelay1_DSTATE_p =
_rtB -> B_311_239_0 ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) {
_rtDW -> Memory_PreviousInput_j = _rtB -> B_311_2771_0 ; } i = ssIsSampleHit
( S , 2 , 0 ) ; if ( i != 0 ) { _rtDW -> Integrator_IC_LOADING = 0U ; _rtDW
-> Integrator_DSTATE += _rtP -> P_844 * _rtB -> B_311_2730_0 ; if ( _rtDW ->
Integrator_DSTATE >= _rtP -> P_845 ) { _rtDW -> Integrator_DSTATE = _rtP ->
P_845 ; } else { if ( _rtDW -> Integrator_DSTATE <= _rtP -> P_846 ) { _rtDW
-> Integrator_DSTATE = _rtP -> P_846 ; } } _rtDW -> Integrator_PrevResetState
= ( int8_T ) _rtB -> B_311_253_0_f ; HoldSine = _rtDW -> lastSin_i ; _rtDW ->
lastSin_i = _rtDW -> lastSin_i * _rtP -> P_857 + _rtDW -> lastCos_lq * _rtP
-> P_856 ; _rtDW -> lastCos_lq = _rtDW -> lastCos_lq * _rtP -> P_857 -
HoldSine * _rtP -> P_856 ; _rtDW -> Integ4_SYSTEM_ENABLE_h = 0U ; _rtDW ->
Integ4_DSTATE_o = _rtP -> P_860 * _rtB -> B_311_263_0 + _rtB -> B_311_264_0 ;
ssCallAccelRunBlock ( S , 311 , 266 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_c = _rtB -> B_311_263_0 ; _rtDW -> UnitDelay1_DSTATE_i =
_rtB -> B_311_273_0 ; HoldSine = _rtDW -> lastSin_av ; _rtDW -> lastSin_av =
_rtDW -> lastSin_av * _rtP -> P_878 + _rtDW -> lastCos_mv * _rtP -> P_877 ;
_rtDW -> lastCos_mv = _rtDW -> lastCos_mv * _rtP -> P_878 - HoldSine * _rtP
-> P_877 ; _rtDW -> Integ4_SYSTEM_ENABLE_k = 0U ; _rtDW -> Integ4_DSTATE_a =
_rtP -> P_881 * _rtB -> B_311_275_0 + _rtB -> B_311_276_0 ;
ssCallAccelRunBlock ( S , 311 , 278 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_a = _rtB -> B_311_275_0 ; _rtDW -> UnitDelay1_DSTATE_c =
_rtB -> B_311_285_0 ; HoldSine = _rtDW -> lastSin_n ; _rtDW -> lastSin_n =
_rtDW -> lastSin_n * _rtP -> P_901 + _rtDW -> lastCos_d * _rtP -> P_900 ;
_rtDW -> lastCos_d = _rtDW -> lastCos_d * _rtP -> P_901 - HoldSine * _rtP ->
P_900 ; _rtDW -> Integ4_SYSTEM_ENABLE_o = 0U ; _rtDW -> Integ4_DSTATE_e =
_rtP -> P_904 * _rtB -> B_311_292_0 + _rtB -> B_311_293_0 ;
ssCallAccelRunBlock ( S , 311 , 295 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_on = _rtB -> B_311_292_0 ; _rtDW -> UnitDelay1_DSTATE_l =
_rtB -> B_311_302_0 ; HoldSine = _rtDW -> lastSin_f ; _rtDW -> lastSin_f =
_rtDW -> lastSin_f * _rtP -> P_922 + _rtDW -> lastCos_n * _rtP -> P_921 ;
_rtDW -> lastCos_n = _rtDW -> lastCos_n * _rtP -> P_922 - HoldSine * _rtP ->
P_921 ; _rtDW -> Integ4_SYSTEM_ENABLE_j = 0U ; _rtDW -> Integ4_DSTATE_f =
_rtP -> P_925 * _rtB -> B_311_304_0 + _rtB -> B_311_305_0 ;
ssCallAccelRunBlock ( S , 311 , 307 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_cq = _rtB -> B_311_304_0 ; _rtDW -> UnitDelay1_DSTATE_f =
_rtB -> B_311_314_0 ; HoldSine = _rtDW -> lastSin_ak ; _rtDW -> lastSin_ak =
_rtDW -> lastSin_ak * _rtP -> P_945 + _rtDW -> lastCos_g * _rtP -> P_944 ;
_rtDW -> lastCos_g = _rtDW -> lastCos_g * _rtP -> P_945 - HoldSine * _rtP ->
P_944 ; _rtDW -> Integ4_SYSTEM_ENABLE_p = 0U ; _rtDW -> Integ4_DSTATE_k =
_rtP -> P_948 * _rtB -> B_311_321_0 + _rtB -> B_311_322_0 ;
ssCallAccelRunBlock ( S , 311 , 324 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_d = _rtB -> B_311_321_0 ; _rtDW -> UnitDelay1_DSTATE_k =
_rtB -> B_311_331_0 ; HoldSine = _rtDW -> lastSin_o ; _rtDW -> lastSin_o =
_rtDW -> lastSin_o * _rtP -> P_966 + _rtDW -> lastCos_ke * _rtP -> P_965 ;
_rtDW -> lastCos_ke = _rtDW -> lastCos_ke * _rtP -> P_966 - HoldSine * _rtP
-> P_965 ; _rtDW -> Integ4_SYSTEM_ENABLE_e = 0U ; _rtDW -> Integ4_DSTATE_jf =
_rtP -> P_969 * _rtB -> B_311_333_0 + _rtB -> B_311_334_0 ;
ssCallAccelRunBlock ( S , 311 , 336 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_pw = _rtB -> B_311_333_0 ; _rtDW -> UnitDelay1_DSTATE_a =
_rtB -> B_311_343_0 ; _rtDW -> Integrator_DSTATE_o += _rtP -> P_993 * _rtB ->
B_311_243_0 ; _rtDW -> Integrator_PrevResetState_f = ( int8_T ) _rtB ->
B_311_360_0 ; _rtDW -> Integrator_SYSTEM_ENABLE = 0U ; _rtDW ->
Integrator_DSTATE_j [ 0 ] = _rtP -> P_994 * _rtB -> B_311_369_0 [ 0 ] + _rtB
-> B_311_370_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_bd [ 0 ] = _rtB ->
B_311_370_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE [ 0 ] = _rtB -> B_311_377_0 [
0 ] ; _rtDW -> Integrator_DSTATE_j [ 1 ] = _rtP -> P_994 * _rtB ->
B_311_369_0 [ 1 ] + _rtB -> B_311_370_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_bd
[ 1 ] = _rtB -> B_311_370_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE [ 1 ] = _rtB
-> B_311_377_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_h = 0U ; _rtDW ->
Integrator_DSTATE_f [ 0 ] = _rtP -> P_1003 * _rtB -> B_311_386_0 [ 0 ] + _rtB
-> B_311_387_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_g [ 0 ] = _rtB ->
B_311_387_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_m [ 0 ] = _rtB -> B_311_394_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_f [ 1 ] = _rtP -> P_1003 * _rtB ->
B_311_386_0 [ 1 ] + _rtB -> B_311_387_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_g
[ 1 ] = _rtB -> B_311_387_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_m [ 1 ] = _rtB
-> B_311_394_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_f = 0U ; _rtDW ->
Integrator_DSTATE_c [ 0 ] = _rtP -> P_1012 * _rtB -> B_311_403_0 [ 0 ] + _rtB
-> B_311_404_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_kl [ 0 ] = _rtB ->
B_311_404_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_i [ 0 ] = _rtB -> B_311_411_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_c [ 1 ] = _rtP -> P_1012 * _rtB ->
B_311_403_0 [ 1 ] + _rtB -> B_311_404_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_kl
[ 1 ] = _rtB -> B_311_404_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_i [ 1 ] = _rtB
-> B_311_411_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_k = 0U ; _rtDW ->
Integrator_DSTATE_d [ 0 ] = _rtP -> P_1025 * _rtB -> B_311_424_0 [ 0 ] + _rtB
-> B_311_425_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_n [ 0 ] = _rtB ->
B_311_425_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_n [ 0 ] = _rtB -> B_311_432_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_d [ 1 ] = _rtP -> P_1025 * _rtB ->
B_311_424_0 [ 1 ] + _rtB -> B_311_425_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_n
[ 1 ] = _rtB -> B_311_425_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_n [ 1 ] = _rtB
-> B_311_432_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_j = 0U ; _rtDW ->
Integrator_DSTATE_jn [ 0 ] = _rtP -> P_1034 * _rtB -> B_311_441_0 [ 0 ] +
_rtB -> B_311_442_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_l2 [ 0 ] = _rtB ->
B_311_442_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_b [ 0 ] = _rtB -> B_311_449_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_jn [ 1 ] = _rtP -> P_1034 * _rtB ->
B_311_441_0 [ 1 ] + _rtB -> B_311_442_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_l2
[ 1 ] = _rtB -> B_311_442_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_b [ 1 ] = _rtB
-> B_311_449_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_l = 0U ; _rtDW ->
Integrator_DSTATE_d5 [ 0 ] = _rtP -> P_1043 * _rtB -> B_311_458_0 [ 0 ] +
_rtB -> B_311_459_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_m [ 0 ] = _rtB ->
B_311_459_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_k [ 0 ] = _rtB -> B_311_466_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_d5 [ 1 ] = _rtP -> P_1043 * _rtB ->
B_311_458_0 [ 1 ] + _rtB -> B_311_459_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_m
[ 1 ] = _rtB -> B_311_459_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_k [ 1 ] = _rtB
-> B_311_466_0 [ 1 ] ; _rtDW -> Integrator_DSTATE_a += _rtP -> P_1059 * _rtB
-> B_311_273_0_l ; _rtDW -> Integrator_PrevResetState_e = ( int8_T ) _rtB ->
B_311_486_0_h ; _rtDW -> Integrator_SYSTEM_ENABLE_fz = 0U ; _rtDW ->
Integrator_DSTATE_az [ 0 ] = _rtP -> P_1060 * _rtB -> B_311_495_0 [ 0 ] +
_rtB -> B_311_496_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_fn [ 0 ] = _rtB ->
B_311_496_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_f [ 0 ] = _rtB -> B_311_503_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_az [ 1 ] = _rtP -> P_1060 * _rtB ->
B_311_495_0 [ 1 ] + _rtB -> B_311_496_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_fn
[ 1 ] = _rtB -> B_311_496_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_f [ 1 ] = _rtB
-> B_311_503_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_p = 0U ; _rtDW ->
Integrator_DSTATE_b [ 0 ] = _rtP -> P_1069 * _rtB -> B_311_512_0 [ 0 ] + _rtB
-> B_311_513_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_ht [ 0 ] = _rtB ->
B_311_513_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_il [ 0 ] = _rtB -> B_311_520_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_b [ 1 ] = _rtP -> P_1069 * _rtB ->
B_311_512_0 [ 1 ] + _rtB -> B_311_513_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_ht
[ 1 ] = _rtB -> B_311_513_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_il [ 1 ] =
_rtB -> B_311_520_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_n = 0U ; _rtDW
-> Integrator_DSTATE_ar [ 0 ] = _rtP -> P_1078 * _rtB -> B_311_529_0 [ 0 ] +
_rtB -> B_311_530_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_ds [ 0 ] = _rtB ->
B_311_530_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_ft [ 0 ] = _rtB -> B_311_537_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_ar [ 1 ] = _rtP -> P_1078 * _rtB ->
B_311_529_0 [ 1 ] + _rtB -> B_311_530_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_ds
[ 1 ] = _rtB -> B_311_530_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_ft [ 1 ] =
_rtB -> B_311_537_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_lb = 0U ; _rtDW
-> Integrator_DSTATE_n [ 0 ] = _rtP -> P_1091 * _rtB -> B_311_550_0 [ 0 ] +
_rtB -> B_311_551_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_jl [ 0 ] = _rtB ->
B_311_551_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_nq [ 0 ] = _rtB -> B_311_558_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_n [ 1 ] = _rtP -> P_1091 * _rtB ->
B_311_550_0 [ 1 ] + _rtB -> B_311_551_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_jl
[ 1 ] = _rtB -> B_311_551_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_nq [ 1 ] =
_rtB -> B_311_558_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_m = 0U ; _rtDW
-> Integrator_DSTATE_e [ 0 ] = _rtP -> P_1100 * _rtB -> B_311_567_0 [ 0 ] +
_rtB -> B_311_568_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_l1 [ 0 ] = _rtB ->
B_311_568_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_nr [ 0 ] = _rtB -> B_311_575_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_e [ 1 ] = _rtP -> P_1100 * _rtB ->
B_311_567_0 [ 1 ] + _rtB -> B_311_568_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_l1
[ 1 ] = _rtB -> B_311_568_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_nr [ 1 ] =
_rtB -> B_311_575_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_fl = 0U ; _rtDW
-> Integrator_DSTATE_cb [ 0 ] = _rtP -> P_1109 * _rtB -> B_311_584_0 [ 0 ] +
_rtB -> B_311_585_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_hq [ 0 ] = _rtB ->
B_311_585_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_h [ 0 ] = _rtB -> B_311_592_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_cb [ 1 ] = _rtP -> P_1109 * _rtB ->
B_311_584_0 [ 1 ] + _rtB -> B_311_585_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_hq
[ 1 ] = _rtB -> B_311_585_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_h [ 1 ] = _rtB
-> B_311_592_0 [ 1 ] ; _rtDW -> Integrator_DSTATE_au += _rtP -> P_1125 * _rtB
-> B_311_248_0 ; _rtDW -> Integrator_PrevResetState_l = ( int8_T ) _rtB ->
B_311_612_0 ; _rtDW -> Integrator_SYSTEM_ENABLE_li = 0U ; _rtDW ->
Integrator_DSTATE_p [ 0 ] = _rtP -> P_1126 * _rtB -> B_311_621_0 [ 0 ] + _rtB
-> B_311_622_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_nr [ 0 ] = _rtB ->
B_311_622_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_hg [ 0 ] = _rtB -> B_311_629_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_p [ 1 ] = _rtP -> P_1126 * _rtB ->
B_311_621_0 [ 1 ] + _rtB -> B_311_622_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_nr
[ 1 ] = _rtB -> B_311_622_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_hg [ 1 ] =
_rtB -> B_311_629_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_d = 0U ; _rtDW
-> Integrator_DSTATE_ec [ 0 ] = _rtP -> P_1135 * _rtB -> B_311_638_0 [ 0 ] +
_rtB -> B_311_639_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_i3 [ 0 ] = _rtB ->
B_311_639_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_ni [ 0 ] = _rtB -> B_311_646_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_ec [ 1 ] = _rtP -> P_1135 * _rtB ->
B_311_638_0 [ 1 ] + _rtB -> B_311_639_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_i3
[ 1 ] = _rtB -> B_311_639_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_ni [ 1 ] =
_rtB -> B_311_646_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_b = 0U ; _rtDW
-> Integrator_DSTATE_h [ 0 ] = _rtP -> P_1144 * _rtB -> B_311_655_0 [ 0 ] +
_rtB -> B_311_656_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_dq [ 0 ] = _rtB ->
B_311_656_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_l [ 0 ] = _rtB -> B_311_663_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_h [ 1 ] = _rtP -> P_1144 * _rtB ->
B_311_655_0 [ 1 ] + _rtB -> B_311_656_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_dq
[ 1 ] = _rtB -> B_311_656_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_l [ 1 ] = _rtB
-> B_311_663_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_o = 0U ; _rtDW ->
Integrator_DSTATE_ff [ 0 ] = _rtP -> P_1157 * _rtB -> B_311_676_0 [ 0 ] +
_rtB -> B_311_677_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_k0 [ 0 ] = _rtB ->
B_311_677_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_hp [ 0 ] = _rtB -> B_311_684_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_ff [ 1 ] = _rtP -> P_1157 * _rtB ->
B_311_676_0 [ 1 ] + _rtB -> B_311_677_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_k0
[ 1 ] = _rtB -> B_311_677_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_hp [ 1 ] =
_rtB -> B_311_684_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_om = 0U ; _rtDW
-> Integrator_DSTATE_g [ 0 ] = _rtP -> P_1166 * _rtB -> B_311_693_0 [ 0 ] +
_rtB -> B_311_694_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_dh [ 0 ] = _rtB ->
B_311_694_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_ko [ 0 ] = _rtB -> B_311_701_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_g [ 1 ] = _rtP -> P_1166 * _rtB ->
B_311_693_0 [ 1 ] + _rtB -> B_311_694_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_dh
[ 1 ] = _rtB -> B_311_694_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_ko [ 1 ] =
_rtB -> B_311_701_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_k5 = 0U ; _rtDW
-> Integrator_DSTATE_eq [ 0 ] = _rtP -> P_1175 * _rtB -> B_311_710_0 [ 0 ] +
_rtB -> B_311_711_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_fo [ 0 ] = _rtB ->
B_311_711_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_e [ 0 ] = _rtB -> B_311_718_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_eq [ 1 ] = _rtP -> P_1175 * _rtB ->
B_311_710_0 [ 1 ] + _rtB -> B_311_711_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_fo
[ 1 ] = _rtB -> B_311_711_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_e [ 1 ] = _rtB
-> B_311_718_0 [ 1 ] ; for ( i = 0 ; i < 832 ; i ++ ) { _rtDW -> Delay_DSTATE
[ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE [ ( i + 1U ) << 1 ] ; _rtDW
-> Delay_DSTATE [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE [ (
( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_o [ ( uint32_T ) i << 1 ] =
_rtDW -> Delay_DSTATE_o [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_o [ ( (
uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_o [ ( ( i + 1U ) << 1 ) +
1U ] ; _rtDW -> Delay_DSTATE_p [ ( uint32_T ) i << 1 ] = _rtDW ->
Delay_DSTATE_p [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_p [ ( ( uint32_T )
i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_p [ ( ( i + 1U ) << 1 ) + 1U ] ;
_rtDW -> Delay_DSTATE_b [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_b [ (
i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_b [ ( ( uint32_T ) i << 1 ) + 1U ] =
_rtDW -> Delay_DSTATE_b [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW ->
Delay_DSTATE_k [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_k [ ( i + 1U )
<< 1 ] ; _rtDW -> Delay_DSTATE_k [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW ->
Delay_DSTATE_k [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_m [ (
uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_m [ ( i + 1U ) << 1 ] ; _rtDW ->
Delay_DSTATE_m [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_m [ (
( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_j [ ( uint32_T ) i << 1 ] =
_rtDW -> Delay_DSTATE_j [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_j [ ( (
uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_j [ ( ( i + 1U ) << 1 ) +
1U ] ; _rtDW -> Delay_DSTATE_d [ ( uint32_T ) i << 1 ] = _rtDW ->
Delay_DSTATE_d [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_d [ ( ( uint32_T )
i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_d [ ( ( i + 1U ) << 1 ) + 1U ] ;
_rtDW -> Delay_DSTATE_c [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_c [ (
i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_c [ ( ( uint32_T ) i << 1 ) + 1U ] =
_rtDW -> Delay_DSTATE_c [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW ->
Delay_DSTATE_oo [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_oo [ ( i + 1U
) << 1 ] ; _rtDW -> Delay_DSTATE_oo [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW
-> Delay_DSTATE_oo [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_f [ (
uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_f [ ( i + 1U ) << 1 ] ; _rtDW ->
Delay_DSTATE_f [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_f [ (
( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_a [ ( uint32_T ) i << 1 ] =
_rtDW -> Delay_DSTATE_a [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_a [ ( (
uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_a [ ( ( i + 1U ) << 1 ) +
1U ] ; _rtDW -> Delay_DSTATE_l [ ( uint32_T ) i << 1 ] = _rtDW ->
Delay_DSTATE_l [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_l [ ( ( uint32_T )
i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_l [ ( ( i + 1U ) << 1 ) + 1U ] ;
_rtDW -> Delay_DSTATE_bg [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_bg [
( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_bg [ ( ( uint32_T ) i << 1 ) + 1U ]
= _rtDW -> Delay_DSTATE_bg [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW ->
Delay_DSTATE_ky [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_ky [ ( i + 1U
) << 1 ] ; _rtDW -> Delay_DSTATE_ky [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW
-> Delay_DSTATE_ky [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_oe [
( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_oe [ ( i + 1U ) << 1 ] ; _rtDW
-> Delay_DSTATE_oe [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW ->
Delay_DSTATE_oe [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_dt [ (
uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_dt [ ( i + 1U ) << 1 ] ; _rtDW ->
Delay_DSTATE_dt [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_dt [
( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_c5 [ ( uint32_T ) i << 1 ]
= _rtDW -> Delay_DSTATE_c5 [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_c5 [ (
( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_c5 [ ( ( i + 1U ) << 1 )
+ 1U ] ; } _rtDW -> Delay_DSTATE [ 1664 ] = _rtB -> B_311_370_0 [ 0 ] ; _rtDW
-> Delay_DSTATE [ 1665 ] = _rtB -> B_311_370_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_o [ 1664 ] = _rtB -> B_311_387_0 [ 0 ] ; _rtDW -> Delay_DSTATE_o
[ 1665 ] = _rtB -> B_311_387_0 [ 1 ] ; _rtDW -> Delay_DSTATE_p [ 1664 ] =
_rtB -> B_311_404_0 [ 0 ] ; _rtDW -> Delay_DSTATE_p [ 1665 ] = _rtB ->
B_311_404_0 [ 1 ] ; _rtDW -> Delay_DSTATE_b [ 1664 ] = _rtB -> B_311_425_0 [
0 ] ; _rtDW -> Delay_DSTATE_b [ 1665 ] = _rtB -> B_311_425_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_k [ 1664 ] = _rtB -> B_311_442_0 [ 0 ] ; _rtDW -> Delay_DSTATE_k
[ 1665 ] = _rtB -> B_311_442_0 [ 1 ] ; _rtDW -> Delay_DSTATE_m [ 1664 ] =
_rtB -> B_311_459_0 [ 0 ] ; _rtDW -> Delay_DSTATE_m [ 1665 ] = _rtB ->
B_311_459_0 [ 1 ] ; _rtDW -> Delay_DSTATE_j [ 1664 ] = _rtB -> B_311_496_0 [
0 ] ; _rtDW -> Delay_DSTATE_j [ 1665 ] = _rtB -> B_311_496_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_d [ 1664 ] = _rtB -> B_311_513_0 [ 0 ] ; _rtDW -> Delay_DSTATE_d
[ 1665 ] = _rtB -> B_311_513_0 [ 1 ] ; _rtDW -> Delay_DSTATE_c [ 1664 ] =
_rtB -> B_311_530_0 [ 0 ] ; _rtDW -> Delay_DSTATE_c [ 1665 ] = _rtB ->
B_311_530_0 [ 1 ] ; _rtDW -> Delay_DSTATE_oo [ 1664 ] = _rtB -> B_311_551_0 [
0 ] ; _rtDW -> Delay_DSTATE_oo [ 1665 ] = _rtB -> B_311_551_0 [ 1 ] ; _rtDW
-> Delay_DSTATE_f [ 1664 ] = _rtB -> B_311_568_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_f [ 1665 ] = _rtB -> B_311_568_0 [ 1 ] ; _rtDW -> Delay_DSTATE_a
[ 1664 ] = _rtB -> B_311_585_0 [ 0 ] ; _rtDW -> Delay_DSTATE_a [ 1665 ] =
_rtB -> B_311_585_0 [ 1 ] ; _rtDW -> Delay_DSTATE_l [ 1664 ] = _rtB ->
B_311_622_0 [ 0 ] ; _rtDW -> Delay_DSTATE_l [ 1665 ] = _rtB -> B_311_622_0 [
1 ] ; _rtDW -> Delay_DSTATE_bg [ 1664 ] = _rtB -> B_311_639_0 [ 0 ] ; _rtDW
-> Delay_DSTATE_bg [ 1665 ] = _rtB -> B_311_639_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_ky [ 1664 ] = _rtB -> B_311_656_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_ky [ 1665 ] = _rtB -> B_311_656_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_oe [ 1664 ] = _rtB -> B_311_677_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_oe [ 1665 ] = _rtB -> B_311_677_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_dt [ 1664 ] = _rtB -> B_311_694_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_dt [ 1665 ] = _rtB -> B_311_694_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_c5 [ 1664 ] = _rtB -> B_311_711_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_c5 [ 1665 ] = _rtB -> B_311_711_0 [ 1 ] ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_e = _rtB ->
B_311_1374_0 [ 0 ] ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
_rtDW -> Integrator_DSTATE_dv += _rtP -> P_1193 * _rtB -> B_311_268_0 ; _rtDW
-> Integrator_PrevResetState_g = ( int8_T ) _rtB -> B_311_741_0 ; _rtDW ->
Integrator_SYSTEM_ENABLE_pr = 0U ; _rtDW -> Integrator_DSTATE_m [ 0 ] = _rtP
-> P_1194 * _rtB -> B_311_750_0 [ 0 ] + _rtB -> B_311_751_0 [ 0 ] ; _rtDW ->
UnitDelay1_DSTATE_e [ 0 ] = _rtB -> B_311_751_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_bw [ 0 ] = _rtB -> B_311_758_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_m [ 1 ] = _rtP -> P_1194 * _rtB -> B_311_750_0 [ 1 ] + _rtB
-> B_311_751_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_e [ 1 ] = _rtB ->
B_311_751_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_bw [ 1 ] = _rtB -> B_311_758_0
[ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_mt = 0U ; _rtDW ->
Integrator_DSTATE_aj [ 0 ] = _rtP -> P_1203 * _rtB -> B_311_767_0 [ 0 ] +
_rtB -> B_311_768_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_ie [ 0 ] = _rtB ->
B_311_768_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_a [ 0 ] = _rtB -> B_311_775_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_aj [ 1 ] = _rtP -> P_1203 * _rtB ->
B_311_767_0 [ 1 ] + _rtB -> B_311_768_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_ie
[ 1 ] = _rtB -> B_311_768_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_a [ 1 ] = _rtB
-> B_311_775_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_e = 0U ; _rtDW ->
Integrator_DSTATE_pb [ 0 ] = _rtP -> P_1212 * _rtB -> B_311_784_0 [ 0 ] +
_rtB -> B_311_785_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_i4 [ 0 ] = _rtB ->
B_311_785_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_g [ 0 ] = _rtB -> B_311_792_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_pb [ 1 ] = _rtP -> P_1212 * _rtB ->
B_311_784_0 [ 1 ] + _rtB -> B_311_785_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_i4
[ 1 ] = _rtB -> B_311_785_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_g [ 1 ] = _rtB
-> B_311_792_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_bc = 0U ; _rtDW ->
Integrator_DSTATE_jk [ 0 ] = _rtP -> P_1225 * _rtB -> B_311_805_0 [ 0 ] +
_rtB -> B_311_806_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_ej [ 0 ] = _rtB ->
B_311_806_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_ml [ 0 ] = _rtB -> B_311_813_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_jk [ 1 ] = _rtP -> P_1225 * _rtB ->
B_311_805_0 [ 1 ] + _rtB -> B_311_806_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_ej
[ 1 ] = _rtB -> B_311_806_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_ml [ 1 ] =
_rtB -> B_311_813_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_ks = 0U ; _rtDW
-> Integrator_DSTATE_hx [ 0 ] = _rtP -> P_1234 * _rtB -> B_311_822_0 [ 0 ] +
_rtB -> B_311_823_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_bm [ 0 ] = _rtB ->
B_311_823_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_hgp [ 0 ] = _rtB ->
B_311_830_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_hx [ 1 ] = _rtP -> P_1234 *
_rtB -> B_311_822_0 [ 1 ] + _rtB -> B_311_823_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_bm [ 1 ] = _rtB -> B_311_823_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_hgp [ 1 ] = _rtB -> B_311_830_0 [ 1 ] ; _rtDW ->
Integrator_SYSTEM_ENABLE_kf = 0U ; _rtDW -> Integrator_DSTATE_i [ 0 ] = _rtP
-> P_1243 * _rtB -> B_311_839_0 [ 0 ] + _rtB -> B_311_840_0 [ 0 ] ; _rtDW ->
UnitDelay1_DSTATE_id [ 0 ] = _rtB -> B_311_840_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_p [ 0 ] = _rtB -> B_311_847_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_i [ 1 ] = _rtP -> P_1243 * _rtB -> B_311_839_0 [ 1 ] + _rtB
-> B_311_840_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_id [ 1 ] = _rtB ->
B_311_840_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_p [ 1 ] = _rtB -> B_311_847_0
[ 1 ] ; _rtDW -> Integrator_DSTATE_ej += _rtP -> P_1263 * _rtB -> B_311_258_0
; _rtDW -> Integrator_PrevResetState_b = ( int8_T ) _rtB -> B_311_871_0 ;
_rtDW -> Integrator_SYSTEM_ENABLE_jw = 0U ; _rtDW -> Integrator_DSTATE_pk [ 0
] = _rtP -> P_1264 * _rtB -> B_311_880_0 [ 0 ] + _rtB -> B_311_881_0 [ 0 ] ;
_rtDW -> UnitDelay1_DSTATE_lq [ 0 ] = _rtB -> B_311_881_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_fr [ 0 ] = _rtB -> B_311_888_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_pk [ 1 ] = _rtP -> P_1264 * _rtB -> B_311_880_0 [ 1 ] +
_rtB -> B_311_881_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_lq [ 1 ] = _rtB ->
B_311_881_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_fr [ 1 ] = _rtB -> B_311_888_0
[ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_mh = 0U ; _rtDW ->
Integrator_DSTATE_o4 [ 0 ] = _rtP -> P_1273 * _rtB -> B_311_897_0 [ 0 ] +
_rtB -> B_311_898_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_o [ 0 ] = _rtB ->
B_311_898_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_p0 [ 0 ] = _rtB -> B_311_905_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_o4 [ 1 ] = _rtP -> P_1273 * _rtB ->
B_311_897_0 [ 1 ] + _rtB -> B_311_898_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_o
[ 1 ] = _rtB -> B_311_898_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_p0 [ 1 ] =
_rtB -> B_311_905_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_lih = 0U ;
_rtDW -> Integrator_DSTATE_hj [ 0 ] = _rtP -> P_1282 * _rtB -> B_311_914_0 [
0 ] + _rtB -> B_311_915_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_mc [ 0 ] = _rtB
-> B_311_915_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_h1 [ 0 ] = _rtB ->
B_311_922_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_hj [ 1 ] = _rtP -> P_1282 *
_rtB -> B_311_914_0 [ 1 ] + _rtB -> B_311_915_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_mc [ 1 ] = _rtB -> B_311_915_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_h1 [ 1 ] = _rtB -> B_311_922_0 [ 1 ] ; _rtDW ->
Integrator_SYSTEM_ENABLE_nx = 0U ; _rtDW -> Integrator_DSTATE_gj [ 0 ] = _rtP
-> P_1295 * _rtB -> B_311_935_0 [ 0 ] + _rtB -> B_311_936_0 [ 0 ] ; _rtDW ->
UnitDelay1_DSTATE_g4 [ 0 ] = _rtB -> B_311_936_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_aj [ 0 ] = _rtB -> B_311_943_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_gj [ 1 ] = _rtP -> P_1295 * _rtB -> B_311_935_0 [ 1 ] +
_rtB -> B_311_936_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_g4 [ 1 ] = _rtB ->
B_311_936_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_aj [ 1 ] = _rtB -> B_311_943_0
[ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_l2 = 0U ; _rtDW ->
Integrator_DSTATE_l [ 0 ] = _rtP -> P_1304 * _rtB -> B_311_952_0 [ 0 ] + _rtB
-> B_311_953_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_n5 [ 0 ] = _rtB ->
B_311_953_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_o [ 0 ] = _rtB -> B_311_960_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_l [ 1 ] = _rtP -> P_1304 * _rtB ->
B_311_952_0 [ 1 ] + _rtB -> B_311_953_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_n5
[ 1 ] = _rtB -> B_311_953_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_o [ 1 ] = _rtB
-> B_311_960_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_a = 0U ; _rtDW ->
Integrator_DSTATE_j3 [ 0 ] = _rtP -> P_1313 * _rtB -> B_311_969_0 [ 0 ] +
_rtB -> B_311_970_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_lqg [ 0 ] = _rtB ->
B_311_970_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_oq [ 0 ] = _rtB -> B_311_977_0
[ 0 ] ; _rtDW -> Integrator_DSTATE_j3 [ 1 ] = _rtP -> P_1313 * _rtB ->
B_311_969_0 [ 1 ] + _rtB -> B_311_970_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_lqg [ 1 ] = _rtB -> B_311_970_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_oq [ 1 ] = _rtB -> B_311_977_0 [ 1 ] ; for ( i = 0 ; i <
832 ; i ++ ) { _rtDW -> Delay_DSTATE_f0 [ ( uint32_T ) i << 1 ] = _rtDW ->
Delay_DSTATE_f0 [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_f0 [ ( ( uint32_T
) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_f0 [ ( ( i + 1U ) << 1 ) + 1U ] ;
_rtDW -> Delay_DSTATE_ca [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_ca [
( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_ca [ ( ( uint32_T ) i << 1 ) + 1U ]
= _rtDW -> Delay_DSTATE_ca [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW ->
Delay_DSTATE_e [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_e [ ( i + 1U )
<< 1 ] ; _rtDW -> Delay_DSTATE_e [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW ->
Delay_DSTATE_e [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_de [ (
uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_de [ ( i + 1U ) << 1 ] ; _rtDW ->
Delay_DSTATE_de [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_de [
( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_dj [ ( uint32_T ) i << 1 ]
= _rtDW -> Delay_DSTATE_dj [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_dj [ (
( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_dj [ ( ( i + 1U ) << 1 )
+ 1U ] ; _rtDW -> Delay_DSTATE_i [ ( uint32_T ) i << 1 ] = _rtDW ->
Delay_DSTATE_i [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_i [ ( ( uint32_T )
i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_i [ ( ( i + 1U ) << 1 ) + 1U ] ;
_rtDW -> Delay_DSTATE_bf [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_bf [
( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_bf [ ( ( uint32_T ) i << 1 ) + 1U ]
= _rtDW -> Delay_DSTATE_bf [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW ->
Delay_DSTATE_i1 [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_i1 [ ( i + 1U
) << 1 ] ; _rtDW -> Delay_DSTATE_i1 [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW
-> Delay_DSTATE_i1 [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_mn [
( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_mn [ ( i + 1U ) << 1 ] ; _rtDW
-> Delay_DSTATE_mn [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW ->
Delay_DSTATE_mn [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_f1 [ (
uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_f1 [ ( i + 1U ) << 1 ] ; _rtDW ->
Delay_DSTATE_f1 [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_f1 [
( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_p1 [ ( uint32_T ) i << 1 ]
= _rtDW -> Delay_DSTATE_p1 [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_p1 [ (
( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_p1 [ ( ( i + 1U ) << 1 )
+ 1U ] ; _rtDW -> Delay_DSTATE_cl [ ( uint32_T ) i << 1 ] = _rtDW ->
Delay_DSTATE_cl [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_cl [ ( ( uint32_T
) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_cl [ ( ( i + 1U ) << 1 ) + 1U ] ; }
_rtDW -> Delay_DSTATE_f0 [ 1664 ] = _rtB -> B_311_751_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_f0 [ 1665 ] = _rtB -> B_311_751_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_ca [ 1664 ] = _rtB -> B_311_768_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_ca [ 1665 ] = _rtB -> B_311_768_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_e [ 1664 ] = _rtB -> B_311_785_0 [ 0 ] ; _rtDW -> Delay_DSTATE_e
[ 1665 ] = _rtB -> B_311_785_0 [ 1 ] ; _rtDW -> Delay_DSTATE_de [ 1664 ] =
_rtB -> B_311_806_0 [ 0 ] ; _rtDW -> Delay_DSTATE_de [ 1665 ] = _rtB ->
B_311_806_0 [ 1 ] ; _rtDW -> Delay_DSTATE_dj [ 1664 ] = _rtB -> B_311_823_0 [
0 ] ; _rtDW -> Delay_DSTATE_dj [ 1665 ] = _rtB -> B_311_823_0 [ 1 ] ; _rtDW
-> Delay_DSTATE_i [ 1664 ] = _rtB -> B_311_840_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_i [ 1665 ] = _rtB -> B_311_840_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_bf [ 1664 ] = _rtB -> B_311_881_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_bf [ 1665 ] = _rtB -> B_311_881_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_i1 [ 1664 ] = _rtB -> B_311_898_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_i1 [ 1665 ] = _rtB -> B_311_898_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_mn [ 1664 ] = _rtB -> B_311_915_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_mn [ 1665 ] = _rtB -> B_311_915_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_f1 [ 1664 ] = _rtB -> B_311_936_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_f1 [ 1665 ] = _rtB -> B_311_936_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_p1 [ 1664 ] = _rtB -> B_311_953_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_p1 [ 1665 ] = _rtB -> B_311_953_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_cl [ 1664 ] = _rtB -> B_311_970_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_cl [ 1665 ] = _rtB -> B_311_970_0 [ 1 ] ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory1_PreviousInput = _rtB ->
B_311_3100_0 ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtDW ->
Integrator_DSTATE_k += _rtP -> P_1330 * _rtB -> B_311_253_0 ; _rtDW ->
Integrator_PrevResetState_go = ( int8_T ) _rtB -> B_311_999_0 ; _rtDW ->
Integrator_SYSTEM_ENABLE_po = 0U ; _rtDW -> Integrator_DSTATE_mm [ 0 ] = _rtP
-> P_1331 * _rtB -> B_311_1008_0 [ 0 ] + _rtB -> B_311_1009_0 [ 0 ] ; _rtDW
-> UnitDelay1_DSTATE_cs [ 0 ] = _rtB -> B_311_1009_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_at [ 0 ] = _rtB -> B_311_1016_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_mm [ 1 ] = _rtP -> P_1331 * _rtB -> B_311_1008_0 [ 1 ] +
_rtB -> B_311_1009_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_cs [ 1 ] = _rtB ->
B_311_1009_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_at [ 1 ] = _rtB ->
B_311_1016_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_i = 0U ; _rtDW ->
Integrator_DSTATE_ng [ 0 ] = _rtP -> P_1340 * _rtB -> B_311_1025_0 [ 0 ] +
_rtB -> B_311_1026_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_p2 [ 0 ] = _rtB ->
B_311_1026_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_i2 [ 0 ] = _rtB ->
B_311_1033_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_ng [ 1 ] = _rtP -> P_1340 *
_rtB -> B_311_1025_0 [ 1 ] + _rtB -> B_311_1026_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_p2 [ 1 ] = _rtB -> B_311_1026_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_i2 [ 1 ] = _rtB -> B_311_1033_0 [ 1 ] ; _rtDW ->
Integrator_SYSTEM_ENABLE_lt = 0U ; _rtDW -> Integrator_DSTATE_bw [ 0 ] = _rtP
-> P_1349 * _rtB -> B_311_1042_0 [ 0 ] + _rtB -> B_311_1043_0 [ 0 ] ; _rtDW
-> UnitDelay1_DSTATE_ez [ 0 ] = _rtB -> B_311_1043_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_c [ 0 ] = _rtB -> B_311_1050_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_bw [ 1 ] = _rtP -> P_1349 * _rtB -> B_311_1042_0 [ 1 ] +
_rtB -> B_311_1043_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_ez [ 1 ] = _rtB ->
B_311_1043_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_c [ 1 ] = _rtB ->
B_311_1050_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_a5 = 0U ; _rtDW ->
Integrator_DSTATE_ka [ 0 ] = _rtP -> P_1362 * _rtB -> B_311_1063_0 [ 0 ] +
_rtB -> B_311_1064_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_ms [ 0 ] = _rtB ->
B_311_1064_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_j [ 0 ] = _rtB ->
B_311_1071_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_ka [ 1 ] = _rtP -> P_1362 *
_rtB -> B_311_1063_0 [ 1 ] + _rtB -> B_311_1064_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_ms [ 1 ] = _rtB -> B_311_1064_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_j [ 1 ] = _rtB -> B_311_1071_0 [ 1 ] ; _rtDW ->
Integrator_SYSTEM_ENABLE_ix = 0U ; _rtDW -> Integrator_DSTATE_ow [ 0 ] = _rtP
-> P_1371 * _rtB -> B_311_1080_0 [ 0 ] + _rtB -> B_311_1081_0 [ 0 ] ; _rtDW
-> UnitDelay1_DSTATE_o5 [ 0 ] = _rtB -> B_311_1081_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_hc [ 0 ] = _rtB -> B_311_1088_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_ow [ 1 ] = _rtP -> P_1371 * _rtB -> B_311_1080_0 [ 1 ] +
_rtB -> B_311_1081_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_o5 [ 1 ] = _rtB ->
B_311_1081_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_hc [ 1 ] = _rtB ->
B_311_1088_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_dz = 0U ; _rtDW ->
Integrator_DSTATE_c1 [ 0 ] = _rtP -> P_1380 * _rtB -> B_311_1097_0 [ 0 ] +
_rtB -> B_311_1098_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_h2 [ 0 ] = _rtB ->
B_311_1098_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_pw [ 0 ] = _rtB ->
B_311_1105_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_c1 [ 1 ] = _rtP -> P_1380 *
_rtB -> B_311_1097_0 [ 1 ] + _rtB -> B_311_1098_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_h2 [ 1 ] = _rtB -> B_311_1098_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_pw [ 1 ] = _rtB -> B_311_1105_0 [ 1 ] ; for ( i = 0 ; i <
832 ; i ++ ) { _rtDW -> Delay_DSTATE_bi [ ( uint32_T ) i << 1 ] = _rtDW ->
Delay_DSTATE_bi [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_bi [ ( ( uint32_T
) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_bi [ ( ( i + 1U ) << 1 ) + 1U ] ;
_rtDW -> Delay_DSTATE_ok [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_ok [
( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_ok [ ( ( uint32_T ) i << 1 ) + 1U ]
= _rtDW -> Delay_DSTATE_ok [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW ->
Delay_DSTATE_h [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_h [ ( i + 1U )
<< 1 ] ; _rtDW -> Delay_DSTATE_h [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW ->
Delay_DSTATE_h [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_cv [ (
uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_cv [ ( i + 1U ) << 1 ] ; _rtDW ->
Delay_DSTATE_cv [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_cv [
( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_cu [ ( uint32_T ) i << 1 ]
= _rtDW -> Delay_DSTATE_cu [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_cu [ (
( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_cu [ ( ( i + 1U ) << 1 )
+ 1U ] ; _rtDW -> Delay_DSTATE_g [ ( uint32_T ) i << 1 ] = _rtDW ->
Delay_DSTATE_g [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_g [ ( ( uint32_T )
i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_g [ ( ( i + 1U ) << 1 ) + 1U ] ; }
_rtDW -> Delay_DSTATE_bi [ 1664 ] = _rtB -> B_311_1009_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_bi [ 1665 ] = _rtB -> B_311_1009_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_ok [ 1664 ] = _rtB -> B_311_1026_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_ok [ 1665 ] = _rtB -> B_311_1026_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_h [ 1664 ] = _rtB -> B_311_1043_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_h [ 1665 ] = _rtB -> B_311_1043_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_cv [ 1664 ] = _rtB -> B_311_1064_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_cv [ 1665 ] = _rtB -> B_311_1064_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_cu [ 1664 ] = _rtB -> B_311_1081_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_cu [ 1665 ] = _rtB -> B_311_1081_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_g [ 1664 ] = _rtB -> B_311_1098_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_g [ 1665 ] = _rtB -> B_311_1098_0 [ 1 ] ; } i = ssIsSampleHit (
S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory2_PreviousInput_m = _rtB ->
B_311_1322_0 [ 0 ] ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
_rtDW -> Integrator_DSTATE_eh += _rtP -> P_1396 * _rtB -> B_311_263_0_e ;
_rtDW -> Integrator_PrevResetState_h = ( int8_T ) _rtB -> B_311_1126_0 ;
_rtDW -> Integrator_SYSTEM_ENABLE_ln = 0U ; _rtDW -> Integrator_DSTATE_om [ 0
] = _rtP -> P_1397 * _rtB -> B_311_1135_0 [ 0 ] + _rtB -> B_311_1136_0 [ 0 ]
; _rtDW -> UnitDelay1_DSTATE_go [ 0 ] = _rtB -> B_311_1136_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_k5 [ 0 ] = _rtB -> B_311_1143_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_om [ 1 ] = _rtP -> P_1397 * _rtB -> B_311_1135_0 [ 1 ] +
_rtB -> B_311_1136_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_go [ 1 ] = _rtB ->
B_311_1136_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_k5 [ 1 ] = _rtB ->
B_311_1143_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_omg = 0U ; _rtDW ->
Integrator_DSTATE_bv [ 0 ] = _rtP -> P_1406 * _rtB -> B_311_1152_0 [ 0 ] +
_rtB -> B_311_1153_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_e5 [ 0 ] = _rtB ->
B_311_1153_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_gy [ 0 ] = _rtB ->
B_311_1160_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_bv [ 1 ] = _rtP -> P_1406 *
_rtB -> B_311_1152_0 [ 1 ] + _rtB -> B_311_1153_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_e5 [ 1 ] = _rtB -> B_311_1153_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_gy [ 1 ] = _rtB -> B_311_1160_0 [ 1 ] ; _rtDW ->
Integrator_SYSTEM_ENABLE_nl = 0U ; _rtDW -> Integrator_DSTATE_d5g [ 0 ] =
_rtP -> P_1415 * _rtB -> B_311_1169_0 [ 0 ] + _rtB -> B_311_1170_0 [ 0 ] ;
_rtDW -> UnitDelay1_DSTATE_h2i [ 0 ] = _rtB -> B_311_1170_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_k5t [ 0 ] = _rtB -> B_311_1177_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_d5g [ 1 ] = _rtP -> P_1415 * _rtB -> B_311_1169_0 [ 1 ] +
_rtB -> B_311_1170_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_h2i [ 1 ] = _rtB ->
B_311_1170_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_k5t [ 1 ] = _rtB ->
B_311_1177_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_jz = 0U ; _rtDW ->
Integrator_DSTATE_hw [ 0 ] = _rtP -> P_1428 * _rtB -> B_311_1190_0 [ 0 ] +
_rtB -> B_311_1191_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_en [ 0 ] = _rtB ->
B_311_1191_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_nj [ 0 ] = _rtB ->
B_311_1198_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_hw [ 1 ] = _rtP -> P_1428 *
_rtB -> B_311_1190_0 [ 1 ] + _rtB -> B_311_1191_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_en [ 1 ] = _rtB -> B_311_1191_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_nj [ 1 ] = _rtB -> B_311_1198_0 [ 1 ] ; _rtDW ->
Integrator_SYSTEM_ENABLE_hk = 0U ; _rtDW -> Integrator_DSTATE_ax [ 0 ] = _rtP
-> P_1437 * _rtB -> B_311_1207_0 [ 0 ] + _rtB -> B_311_1208_0 [ 0 ] ; _rtDW
-> UnitDelay1_DSTATE_nf [ 0 ] = _rtB -> B_311_1208_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_jd [ 0 ] = _rtB -> B_311_1215_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_ax [ 1 ] = _rtP -> P_1437 * _rtB -> B_311_1207_0 [ 1 ] +
_rtB -> B_311_1208_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_nf [ 1 ] = _rtB ->
B_311_1208_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_jd [ 1 ] = _rtB ->
B_311_1215_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_kk = 0U ; _rtDW ->
Integrator_DSTATE_il [ 0 ] = _rtP -> P_1446 * _rtB -> B_311_1224_0 [ 0 ] +
_rtB -> B_311_1225_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_hx [ 0 ] = _rtB ->
B_311_1225_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_hq [ 0 ] = _rtB ->
B_311_1232_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_il [ 1 ] = _rtP -> P_1446 *
_rtB -> B_311_1224_0 [ 1 ] + _rtB -> B_311_1225_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_hx [ 1 ] = _rtB -> B_311_1225_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_hq [ 1 ] = _rtB -> B_311_1232_0 [ 1 ] ; for ( i = 0 ; i <
832 ; i ++ ) { _rtDW -> Delay_DSTATE_h0 [ ( uint32_T ) i << 1 ] = _rtDW ->
Delay_DSTATE_h0 [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_h0 [ ( ( uint32_T
) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_h0 [ ( ( i + 1U ) << 1 ) + 1U ] ;
_rtDW -> Delay_DSTATE_mo [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_mo [
( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_mo [ ( ( uint32_T ) i << 1 ) + 1U ]
= _rtDW -> Delay_DSTATE_mo [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW ->
Delay_DSTATE_ah [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_ah [ ( i + 1U
) << 1 ] ; _rtDW -> Delay_DSTATE_ah [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW
-> Delay_DSTATE_ah [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_pi [
( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_pi [ ( i + 1U ) << 1 ] ; _rtDW
-> Delay_DSTATE_pi [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW ->
Delay_DSTATE_pi [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_hg [ (
uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_hg [ ( i + 1U ) << 1 ] ; _rtDW ->
Delay_DSTATE_hg [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_hg [
( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_f0p [ ( uint32_T ) i << 1
] = _rtDW -> Delay_DSTATE_f0p [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_f0p
[ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_f0p [ ( ( i + 1U )
<< 1 ) + 1U ] ; } _rtDW -> Delay_DSTATE_h0 [ 1664 ] = _rtB -> B_311_1136_0 [
0 ] ; _rtDW -> Delay_DSTATE_h0 [ 1665 ] = _rtB -> B_311_1136_0 [ 1 ] ; _rtDW
-> Delay_DSTATE_mo [ 1664 ] = _rtB -> B_311_1153_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_mo [ 1665 ] = _rtB -> B_311_1153_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_ah [ 1664 ] = _rtB -> B_311_1170_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_ah [ 1665 ] = _rtB -> B_311_1170_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_pi [ 1664 ] = _rtB -> B_311_1191_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_pi [ 1665 ] = _rtB -> B_311_1191_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_hg [ 1664 ] = _rtB -> B_311_1208_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_hg [ 1665 ] = _rtB -> B_311_1208_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_f0p [ 1664 ] = _rtB -> B_311_1225_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_f0p [ 1665 ] = _rtB -> B_311_1225_0 [ 1 ] ; } i = ssIsSampleHit
( S , 5 , 0 ) ; if ( i != 0 ) { i = ssIsSampleHit ( S , 5 , 0 ) ; if ( i != 0
) { _rtDW -> Integrator_DSTATE_os += _rtP -> P_262 * _rtB -> B_156_55_0 ;
_rtDW -> Integrator_IC_LOADING_l = 0U ; _rtDW -> Integrator_DSTATE_ij += _rtP
-> P_265 * _rtB -> B_156_26_0 ; if ( _rtDW -> Integrator_DSTATE_ij >= _rtP ->
P_266 ) { _rtDW -> Integrator_DSTATE_ij = _rtP -> P_266 ; } else { if ( _rtDW
-> Integrator_DSTATE_ij <= _rtP -> P_267 ) { _rtDW -> Integrator_DSTATE_ij =
_rtP -> P_267 ; } } _rtDW -> Integrator_PrevResetState_hs = ( int8_T ) _rtB
-> B_156_6_0 ; _rtDW -> Integrator_DSTATE_em += _rtP -> P_277 * _rtB ->
B_156_54_0 ; _rtDW -> Integrator_IC_LOADING_n = 0U ; _rtDW ->
Integrator_DSTATE_ho += _rtP -> P_280 * _rtB -> B_156_41_0 ; if ( _rtDW ->
Integrator_DSTATE_ho >= _rtP -> P_281 ) { _rtDW -> Integrator_DSTATE_ho =
_rtP -> P_281 ; } else { if ( _rtDW -> Integrator_DSTATE_ho <= _rtP -> P_282
) { _rtDW -> Integrator_DSTATE_ho = _rtP -> P_282 ; } } _rtDW ->
Integrator_PrevResetState_a = ( int8_T ) _rtB -> B_156_32_0 ; _rtDW ->
Integrator_IC_LOADING_h = 0U ; _rtDW -> Integrator_DSTATE_nj += _rtP -> P_287
* _rtB -> B_156_52_0 ; if ( _rtDW -> Integrator_DSTATE_nj >= _rtP -> P_288 )
{ _rtDW -> Integrator_DSTATE_nj = _rtP -> P_288 ; } else { if ( _rtDW ->
Integrator_DSTATE_nj <= _rtP -> P_289 ) { _rtDW -> Integrator_DSTATE_nj =
_rtP -> P_289 ; } } _rtDW -> Integrator_PrevResetState_m = ( int8_T ) _rtB ->
B_156_47_0 ; } } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtDW ->
Integ4_SYSTEM_ENABLE_l1 = 0U ; _rtDW -> Integ4_DSTATE_c [ 0 ] = _rtP ->
P_1469 * _rtB -> B_311_1274_0 [ 0 ] + _rtB -> B_311_1275_0 [ 0 ] ; _rtDW ->
Integ4_DSTATE_c [ 1 ] = _rtP -> P_1469 * _rtB -> B_311_1274_0 [ 1 ] + _rtB ->
B_311_1275_0 [ 1 ] ; _rtDW -> Integ4_DSTATE_c [ 2 ] = _rtP -> P_1469 * _rtB
-> B_311_1274_0 [ 2 ] + _rtB -> B_311_1275_0 [ 2 ] ; ssCallAccelRunBlock ( S
, 311 , 1277 , SS_CALL_MDL_UPDATE ) ; _rtDW -> Integ4_SYSTEM_ENABLE_d4 = 0U ;
_rtDW -> UnitDelay_DSTATE_e [ 0 ] = _rtB -> B_311_1274_0 [ 0 ] ; _rtDW ->
Integ4_DSTATE_jv [ 0 ] = _rtP -> P_1480 * _rtB -> B_311_1288_0 [ 0 ] + _rtB
-> B_311_1289_0 [ 0 ] ; _rtDW -> UnitDelay_DSTATE_e [ 1 ] = _rtB ->
B_311_1274_0 [ 1 ] ; _rtDW -> Integ4_DSTATE_jv [ 1 ] = _rtP -> P_1480 * _rtB
-> B_311_1288_0 [ 1 ] + _rtB -> B_311_1289_0 [ 1 ] ; _rtDW ->
UnitDelay_DSTATE_e [ 2 ] = _rtB -> B_311_1274_0 [ 2 ] ; _rtDW ->
Integ4_DSTATE_jv [ 2 ] = _rtP -> P_1480 * _rtB -> B_311_1288_0 [ 2 ] + _rtB
-> B_311_1289_0 [ 2 ] ; ssCallAccelRunBlock ( S , 311 , 1291 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_ku = _rtB -> B_311_2677_0 ;
_rtDW -> UnitDelay_DSTATE_fa = _rtB -> B_311_2716_0 ; _rtDW ->
UnitDelay1_DSTATE_jd = _rtB -> B_311_2682_0 ; _rtDW -> UnitDelay1_DSTATE_nl =
_rtB -> B_311_2721_0 ; _rtDW -> Memory_PreviousInput_jf = _rtB ->
B_311_1322_0 [ 0 ] ; _rtDW -> Integ4_SYSTEM_ENABLE_ks = 0U ; _rtDW ->
UnitDelay_DSTATE_ft [ 0 ] = _rtB -> B_311_1288_0 [ 0 ] ; _rtDW ->
Integ4_DSTATE_gd [ 0 ] = _rtP -> P_1497 * _rtB -> B_311_1326_0 [ 0 ] + _rtB
-> B_311_1327_0 [ 0 ] ; _rtDW -> UnitDelay_DSTATE_ft [ 1 ] = _rtB ->
B_311_1288_0 [ 1 ] ; _rtDW -> Integ4_DSTATE_gd [ 1 ] = _rtP -> P_1497 * _rtB
-> B_311_1326_0 [ 1 ] + _rtB -> B_311_1327_0 [ 1 ] ; _rtDW ->
UnitDelay_DSTATE_ft [ 2 ] = _rtB -> B_311_1288_0 [ 2 ] ; _rtDW ->
Integ4_DSTATE_gd [ 2 ] = _rtP -> P_1497 * _rtB -> B_311_1326_0 [ 2 ] + _rtB
-> B_311_1327_0 [ 2 ] ; ssCallAccelRunBlock ( S , 311 , 1329 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> Integ4_SYSTEM_ENABLE_b = 0U ; _rtDW ->
UnitDelay_DSTATE_ca [ 0 ] = _rtB -> B_311_1326_0 [ 0 ] ; _rtDW ->
Integ4_DSTATE_ow [ 0 ] = _rtP -> P_1508 * _rtB -> B_311_1340_0 [ 0 ] + _rtB
-> B_311_1341_0 [ 0 ] ; _rtDW -> UnitDelay_DSTATE_ca [ 1 ] = _rtB ->
B_311_1326_0 [ 1 ] ; _rtDW -> Integ4_DSTATE_ow [ 1 ] = _rtP -> P_1508 * _rtB
-> B_311_1340_0 [ 1 ] + _rtB -> B_311_1341_0 [ 1 ] ; _rtDW ->
UnitDelay_DSTATE_ca [ 2 ] = _rtB -> B_311_1326_0 [ 2 ] ; _rtDW ->
Integ4_DSTATE_ow [ 2 ] = _rtP -> P_1508 * _rtB -> B_311_1340_0 [ 2 ] + _rtB
-> B_311_1341_0 [ 2 ] ; ssCallAccelRunBlock ( S , 311 , 1343 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_al [ 0 ] = _rtB ->
B_311_1340_0 [ 0 ] ; _rtDW -> UnitDelay_DSTATE_al [ 1 ] = _rtB ->
B_311_1340_0 [ 1 ] ; _rtDW -> UnitDelay_DSTATE_al [ 2 ] = _rtB ->
B_311_1340_0 [ 2 ] ; _rtDW -> UnitDelay_DSTATE_n = _rtB -> B_311_2592_0 ;
_rtDW -> UnitDelay_DSTATE_j = _rtB -> B_311_2631_0 ; _rtDW ->
UnitDelay1_DSTATE_jdb = _rtB -> B_311_2597_0 ; _rtDW -> UnitDelay1_DSTATE_kc
= _rtB -> B_311_2636_0 ; _rtDW -> Memory_PreviousInput_mv = _rtB ->
B_311_1374_0 [ 0 ] ; } i = ssIsSampleHit ( S , 5 , 0 ) ; if ( i != 0 ) {
_rtDW -> RateTransition8_Buffer0 = _rtB -> B_311_1260_0 ; } i = ssIsSampleHit
( S , 3 , 0 ) ; if ( i != 0 ) { _rtDW -> UnitDelay_DSTATE_p4 [ 0 ] = _rtB ->
B_311_1560_0 ; _rtDW -> UnitDelay_DSTATE_p4 [ 1 ] = _rtB -> B_311_1567_0 ;
_rtDW -> UnitDelay2_DSTATE_pk [ 0 ] = _rtB -> B_311_1767_0 ; _rtDW ->
UnitDelay2_DSTATE_pk [ 1 ] = _rtB -> B_311_1768_0 ; _rtDW ->
UnitDelay_DSTATE_be [ 0 ] = _rtB -> B_311_1622_0 [ 0 ] ; _rtDW ->
UnitDelay1_DSTATE_hn [ 0 ] = _rtB -> B_311_1688_0 [ 0 ] ; _rtDW ->
UnitDelay_DSTATE_be [ 1 ] = _rtB -> B_311_1622_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_hn [ 1 ] = _rtB -> B_311_1688_0 [ 1 ] ; _rtDW ->
DiscreteTimeIntegrator_IC_LOADING = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_e = _rtB -> B_311_1558_0 ; _rtDW ->
DiscreteTimeIntegrator_PrevResetState = ( int8_T ) _rtB -> B_311_1555_0 ;
_rtDW -> DiscreteTimeIntegrator_IC_LOADING_f = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_m = _rtB -> B_311_1565_0 ; _rtDW ->
DiscreteTimeIntegrator_PrevResetState_n = ( int8_T ) _rtB -> B_311_1563_0 ;
_rtDW -> UnitDelay_DSTATE_m = _rtB -> B_311_1761_0 ; _rtDW ->
UnitDelay_DSTATE_aw [ 0 ] = _rtB -> B_311_1767_0 ; _rtDW ->
UnitDelay_DSTATE_aw [ 1 ] = _rtB -> B_311_1768_0 ; { real_T xnew [ 2 ] ; xnew
[ 0 ] = ( _rtP -> P_1593 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE [ 0 ] +
( _rtP -> P_1593 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE [ 1 ] ; xnew [
0 ] += ( _rtP -> P_1594 [ 0 ] ) * _rtB -> B_311_1581_0 ; xnew [ 1 ] = ( _rtP
-> P_1593 [ 2 ] ) * _rtDW -> DiscreteStateSpace_DSTATE [ 0 ] + ( _rtP ->
P_1593 [ 3 ] ) * _rtDW -> DiscreteStateSpace_DSTATE [ 1 ] ; xnew [ 1 ] += (
_rtP -> P_1594 [ 1 ] ) * _rtB -> B_311_1581_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } { real_T
xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_1598 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_c [ 0 ] + ( _rtP -> P_1598 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_c [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1599 [ 0 ] ) *
_rtB -> B_311_1583_0 ; xnew [ 1 ] = ( _rtP -> P_1598 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_c [ 0 ] + ( _rtP -> P_1598 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_c [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1599 [ 1 ] ) *
_rtB -> B_311_1583_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_c [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> UnitDelay1_DSTATE_gw [ 0 ] = _rtB -> B_311_1767_0 ; _rtDW ->
UnitDelay1_DSTATE_gw [ 1 ] = _rtB -> B_311_1768_0 ; } i = ssIsSampleHit ( S ,
2 , 0 ) ; if ( i != 0 ) { _rtDW -> DiscreteTimeIntegrator1_DSTATE += _rtP ->
P_1606 * _rtB -> B_311_2731_0 ; _rtDW ->
DiscreteTimeIntegrator1_PrevResetState = ( int8_T ) _rtB -> B_311_1604_0 ; }
i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) { { real_T xnew [ 2 ] ; xnew
[ 0 ] = ( _rtP -> P_1609 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_f [ 0 ]
+ ( _rtP -> P_1609 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_f [ 1 ] ;
xnew [ 0 ] += ( _rtP -> P_1610 [ 0 ] ) * _rtB -> B_311_1607_0 [ 2 ] ; xnew [
1 ] = ( _rtP -> P_1609 [ 2 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_f [ 0 ] +
( _rtP -> P_1609 [ 3 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_f [ 1 ] ; xnew
[ 1 ] += ( _rtP -> P_1610 [ 1 ] ) * _rtB -> B_311_1607_0 [ 2 ] ; ( void )
memcpy ( & _rtDW -> DiscreteStateSpace_DSTATE_f [ 0 ] , xnew , sizeof (
real_T ) * 2 ) ; } { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_1614 [ 0 ]
) * _rtDW -> DiscreteStateSpace_DSTATE_g [ 0 ] + ( _rtP -> P_1614 [ 1 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_g [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1615 [
0 ] ) * _rtB -> B_311_1607_0 [ 3 ] ; xnew [ 1 ] = ( _rtP -> P_1614 [ 2 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_g [ 0 ] + ( _rtP -> P_1614 [ 3 ] ) * _rtDW
-> DiscreteStateSpace_DSTATE_g [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1615 [ 1 ] )
* _rtB -> B_311_1607_0 [ 3 ] ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_g [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } {
real_T xnew [ 1 ] ; xnew [ 0 ] = _rtP -> P_1621 * _rtDW ->
DiscreteStateSpace_DSTATE_p ; xnew [ 0 ] += _rtP -> P_1622 * _rtB ->
B_311_1607_0 [ 0 ] ; ( void ) memcpy ( & _rtDW -> DiscreteStateSpace_DSTATE_p
, xnew , sizeof ( real_T ) * 1 ) ; } { real_T xnew [ 1 ] ; xnew [ 0 ] = _rtP
-> P_1626 * _rtDW -> DiscreteStateSpace_DSTATE_ce ; xnew [ 0 ] += _rtP ->
P_1627 * _rtB -> B_311_1607_0 [ 1 ] ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_ce , xnew , sizeof ( real_T ) * 1 ) ; } _rtDW ->
UnitDelay_DSTATE_nw [ 0 ] = _rtB -> B_311_1659_0 [ 0 ] ; _rtDW ->
UnitDelay_DSTATE_nw [ 1 ] = _rtB -> B_311_1659_0 [ 1 ] ; { real_T xnew [ 2 ]
; xnew [ 0 ] = ( _rtP -> P_1634 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_k [ 0 ] + ( _rtP -> P_1634 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_k [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1635 [ 0 ] ) *
_rtB -> B_311_1599_0 ; xnew [ 1 ] = ( _rtP -> P_1634 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_k [ 0 ] + ( _rtP -> P_1634 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_k [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1635 [ 1 ] ) *
_rtB -> B_311_1599_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_k [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } {
real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_1639 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_j [ 0 ] + ( _rtP -> P_1639 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_j [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1640 [ 0 ] ) *
_rtB -> B_311_1600_0 ; xnew [ 1 ] = ( _rtP -> P_1639 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_j [ 0 ] + ( _rtP -> P_1639 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_j [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1640 [ 1 ] ) *
_rtB -> B_311_1600_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_j [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> UnitDelay_DSTATE_ay [ 0 ] = _rtB -> B_311_1688_0 [ 0 ] ; _rtDW ->
UnitDelay_DSTATE_ay [ 1 ] = _rtB -> B_311_1688_0 [ 1 ] ; { real_T xnew [ 2 ]
; xnew [ 0 ] = ( _rtP -> P_1645 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_ph [ 0 ] + ( _rtP -> P_1645 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_ph [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1646 [ 0 ] ) *
_rtB -> B_311_1581_0 ; xnew [ 1 ] = ( _rtP -> P_1645 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_ph [ 0 ] + ( _rtP -> P_1645 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_ph [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1646 [ 1 ] ) *
_rtB -> B_311_1581_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_ph [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } {
real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_1650 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_d [ 0 ] + ( _rtP -> P_1650 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_d [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1651 [ 0 ] ) *
_rtB -> B_311_1583_0 ; xnew [ 1 ] = ( _rtP -> P_1650 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_d [ 0 ] + ( _rtP -> P_1650 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_d [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1651 [ 1 ] ) *
_rtB -> B_311_1583_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_d [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> DiscreteTimeIntegrator_IC_LOADING_n = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_p = _rtB -> B_311_1636_0 ; _rtDW ->
DiscreteTimeIntegrator_PrevResetState_nq = ( int8_T ) _rtB -> B_311_1629_0 ;
_rtDW -> DiscreteTimeIntegrator_IC_LOADING_p = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_n = _rtB -> B_311_1643_0 ; _rtDW ->
DiscreteTimeIntegrator_PrevResetState_i = ( int8_T ) _rtB -> B_311_1641_0 ; {
real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_1671 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_g1 [ 0 ] + ( _rtP -> P_1671 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_g1 [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1672 [ 0 ] ) *
_rtB -> B_311_1599_0 ; xnew [ 1 ] = ( _rtP -> P_1671 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_g1 [ 0 ] + ( _rtP -> P_1671 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_g1 [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1672 [ 1 ] ) *
_rtB -> B_311_1599_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_g1 [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } {
real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_1676 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_b [ 0 ] + ( _rtP -> P_1676 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_b [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1677 [ 0 ] ) *
_rtB -> B_311_1600_0 ; xnew [ 1 ] = ( _rtP -> P_1676 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_b [ 0 ] + ( _rtP -> P_1676 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_b [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1677 [ 1 ] ) *
_rtB -> B_311_1600_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_b [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> Integ4_SYSTEM_ENABLE_i = 0U ; _rtDW -> Integ4_DSTATE_p [ 0 ] = _rtP ->
P_1682 * _rtB -> B_311_1667_0 + _rtB -> B_311_1674_0 [ 0 ] ; _rtDW ->
Integ4_DSTATE_p [ 1 ] = _rtP -> P_1682 * _rtB -> B_311_1673_0 + _rtB ->
B_311_1674_0 [ 1 ] ; ssCallAccelRunBlock ( S , 311 , 1676 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_pv [ 0 ] = _rtB ->
B_311_1667_0 ; _rtDW -> UnitDelay_DSTATE_pv [ 1 ] = _rtB -> B_311_1673_0 ;
_rtDW -> UnitDelay1_DSTATE_pi = _rtB -> B_311_1770_0 ; _rtDW ->
UnitDelay1_DSTATE_cg = _rtB -> B_311_1766_0 ; _rtDW -> Delay_DSTATE_en = _rtB
-> B_311_1719_0 ; _rtDW -> DiscreteTimeIntegrator_IC_LOADING_d = 0U ; _rtDW
-> DiscreteTimeIntegrator_DSTATE_h += _rtP -> P_1697 * _rtB -> B_311_1753_0 ;
if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_h >= _rtP -> P_1698 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_h = _rtP -> P_1698 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_h <= _rtP -> P_1699 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_h = _rtP -> P_1699 ; } } _rtDW ->
DiscreteTimeIntegrator_PrevResetState_o = ( int8_T ) _rtB -> B_311_1715_0 ;
_rtDW -> DiscreteTimeIntegrator_DSTATE_l += _rtP -> P_1710 * _rtB ->
B_311_1759_0 ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_l >= _rtP ->
P_1712 ) { _rtDW -> DiscreteTimeIntegrator_DSTATE_l = _rtP -> P_1712 ; } else
{ if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_l <= _rtP -> P_1713 ) { _rtDW
-> DiscreteTimeIntegrator_DSTATE_l = _rtP -> P_1713 ; } } { real_T xnew [ 2 ]
; xnew [ 0 ] = ( _rtP -> P_1717 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_be [ 0 ] + ( _rtP -> P_1717 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_be [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1718 [ 0 ] ) *
_rtB -> B_311_1760_0 ; xnew [ 1 ] = ( _rtP -> P_1717 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_be [ 0 ] + ( _rtP -> P_1717 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_be [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1718 [ 1 ] ) *
_rtB -> B_311_1760_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_be [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } } i =
ssIsSampleHit ( S , 5 , 0 ) ; if ( i != 0 ) { _rtDW ->
RateTransition8_Buffer0_m = _rtB -> B_311_1266_0 ; } i = ssIsSampleHit ( S ,
3 , 0 ) ; if ( i != 0 ) { _rtDW -> UnitDelay_DSTATE_h [ 0 ] = _rtB ->
B_311_1969_0 ; _rtDW -> UnitDelay_DSTATE_h [ 1 ] = _rtB -> B_311_1976_0 ;
_rtDW -> UnitDelay2_DSTATE_bb [ 0 ] = _rtB -> B_311_2173_0 ; _rtDW ->
UnitDelay2_DSTATE_bb [ 1 ] = _rtB -> B_311_2174_0 ; _rtDW ->
UnitDelay_DSTATE_g3 [ 0 ] = _rtB -> B_311_2036_0 [ 0 ] ; _rtDW ->
UnitDelay1_DSTATE_d5 [ 0 ] = _rtB -> B_311_2093_0 [ 0 ] ; _rtDW ->
UnitDelay_DSTATE_g3 [ 1 ] = _rtB -> B_311_2036_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_d5 [ 1 ] = _rtB -> B_311_2093_0 [ 1 ] ; _rtDW ->
DiscreteTimeIntegrator_IC_LOADING_do = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_pi = _rtB -> B_311_1967_0 ; _rtDW ->
DiscreteTimeIntegrator_PrevResetState_g = ( int8_T ) _rtB -> B_311_1964_0 ;
_rtDW -> DiscreteTimeIntegrator_IC_LOADING_k = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_o = _rtB -> B_311_1974_0 ; _rtDW ->
DiscreteTimeIntegrator_PrevResetState_k = ( int8_T ) _rtB -> B_311_1972_0 ;
_rtDW -> UnitDelay_DSTATE_cd = _rtB -> B_311_2167_0 ; _rtDW ->
UnitDelay_DSTATE_hf [ 0 ] = _rtB -> B_311_2173_0 ; _rtDW ->
UnitDelay_DSTATE_hf [ 1 ] = _rtB -> B_311_2174_0 ; { real_T xnew [ 2 ] ; xnew
[ 0 ] = ( _rtP -> P_1777 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_f5 [ 0
] + ( _rtP -> P_1777 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_f5 [ 1 ] ;
xnew [ 0 ] += ( _rtP -> P_1778 [ 0 ] ) * _rtB -> B_311_1994_0 ; xnew [ 1 ] =
( _rtP -> P_1777 [ 2 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_f5 [ 0 ] + (
_rtP -> P_1777 [ 3 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_f5 [ 1 ] ; xnew [
1 ] += ( _rtP -> P_1778 [ 1 ] ) * _rtB -> B_311_1994_0 ; ( void ) memcpy ( &
_rtDW -> DiscreteStateSpace_DSTATE_f5 [ 0 ] , xnew , sizeof ( real_T ) * 2 )
; } { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_1782 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_gl [ 0 ] + ( _rtP -> P_1782 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_gl [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1783 [ 0 ] ) *
_rtB -> B_311_1996_0 ; xnew [ 1 ] = ( _rtP -> P_1782 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_gl [ 0 ] + ( _rtP -> P_1782 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_gl [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1783 [ 1 ] ) *
_rtB -> B_311_1996_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_gl [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> UnitDelay1_DSTATE_n0 [ 0 ] = _rtB -> B_311_2173_0 ; _rtDW ->
UnitDelay1_DSTATE_n0 [ 1 ] = _rtB -> B_311_2174_0 ; { real_T xnew [ 2 ] ;
xnew [ 0 ] = ( _rtP -> P_1795 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_cy
[ 0 ] + ( _rtP -> P_1795 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_cy [ 1
] ; xnew [ 0 ] += ( _rtP -> P_1796 [ 0 ] ) * _rtB -> B_311_2020_0 [ 2 ] ;
xnew [ 1 ] = ( _rtP -> P_1795 [ 2 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_cy
[ 0 ] + ( _rtP -> P_1795 [ 3 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_cy [ 1
] ; xnew [ 1 ] += ( _rtP -> P_1796 [ 1 ] ) * _rtB -> B_311_2020_0 [ 2 ] ; (
void ) memcpy ( & _rtDW -> DiscreteStateSpace_DSTATE_cy [ 0 ] , xnew , sizeof
( real_T ) * 2 ) ; } { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_1800 [ 0
] ) * _rtDW -> DiscreteStateSpace_DSTATE_m [ 0 ] + ( _rtP -> P_1800 [ 1 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_m [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1801 [
0 ] ) * _rtB -> B_311_2020_0 [ 3 ] ; xnew [ 1 ] = ( _rtP -> P_1800 [ 2 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_m [ 0 ] + ( _rtP -> P_1800 [ 3 ] ) * _rtDW
-> DiscreteStateSpace_DSTATE_m [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1801 [ 1 ] )
* _rtB -> B_311_2020_0 [ 3 ] ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_m [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } {
real_T xnew [ 1 ] ; xnew [ 0 ] = _rtP -> P_1807 * _rtDW ->
DiscreteStateSpace_DSTATE_pf ; xnew [ 0 ] += _rtP -> P_1808 * _rtB ->
B_311_2020_0 [ 0 ] ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_pf , xnew , sizeof ( real_T ) * 1 ) ; } { real_T
xnew [ 1 ] ; xnew [ 0 ] = _rtP -> P_1812 * _rtDW ->
DiscreteStateSpace_DSTATE_l ; xnew [ 0 ] += _rtP -> P_1813 * _rtB ->
B_311_2020_0 [ 1 ] ; ( void ) memcpy ( & _rtDW -> DiscreteStateSpace_DSTATE_l
, xnew , sizeof ( real_T ) * 1 ) ; } _rtDW -> UnitDelay_DSTATE_om [ 0 ] =
_rtB -> B_311_2073_0 [ 0 ] ; _rtDW -> UnitDelay_DSTATE_om [ 1 ] = _rtB ->
B_311_2073_0 [ 1 ] ; { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_1820 [ 0
] ) * _rtDW -> DiscreteStateSpace_DSTATE_bi [ 0 ] + ( _rtP -> P_1820 [ 1 ] )
* _rtDW -> DiscreteStateSpace_DSTATE_bi [ 1 ] ; xnew [ 0 ] += ( _rtP ->
P_1821 [ 0 ] ) * _rtB -> B_311_2016_0 ; xnew [ 1 ] = ( _rtP -> P_1820 [ 2 ] )
* _rtDW -> DiscreteStateSpace_DSTATE_bi [ 0 ] + ( _rtP -> P_1820 [ 3 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_bi [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1821
[ 1 ] ) * _rtB -> B_311_2016_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_bi [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } {
real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_1825 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_i [ 0 ] + ( _rtP -> P_1825 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_i [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1826 [ 0 ] ) *
_rtB -> B_311_2017_0 ; xnew [ 1 ] = ( _rtP -> P_1825 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_i [ 0 ] + ( _rtP -> P_1825 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_i [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1826 [ 1 ] ) *
_rtB -> B_311_2017_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_i [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> UnitDelay_DSTATE_dz [ 0 ] = _rtB -> B_311_2093_0 [ 0 ] ; _rtDW ->
UnitDelay_DSTATE_dz [ 1 ] = _rtB -> B_311_2093_0 [ 1 ] ; { real_T xnew [ 2 ]
; xnew [ 0 ] = ( _rtP -> P_1831 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_fl [ 0 ] + ( _rtP -> P_1831 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_fl [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1832 [ 0 ] ) *
_rtB -> B_311_1955_0 ; xnew [ 1 ] = ( _rtP -> P_1831 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_fl [ 0 ] + ( _rtP -> P_1831 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_fl [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1832 [ 1 ] ) *
_rtB -> B_311_1955_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_fl [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } {
real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_1836 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_o [ 0 ] + ( _rtP -> P_1836 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_o [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1837 [ 0 ] ) *
_rtB -> B_311_1956_0 ; xnew [ 1 ] = ( _rtP -> P_1836 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_o [ 0 ] + ( _rtP -> P_1836 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_o [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1837 [ 1 ] ) *
_rtB -> B_311_1956_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_o [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> DiscreteTimeIntegrator_IC_LOADING_g = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_k = _rtB -> B_311_2050_0 ; _rtDW ->
DiscreteTimeIntegrator_PrevResetState_g1 = ( int8_T ) _rtB -> B_311_2043_0 ;
_rtDW -> DiscreteTimeIntegrator_IC_LOADING_o = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_j = _rtB -> B_311_2057_0 ; _rtDW ->
DiscreteTimeIntegrator_PrevResetState_k3 = ( int8_T ) _rtB -> B_311_2055_0 ;
_rtDW -> Integ4_SYSTEM_ENABLE_kx = 0U ; _rtDW -> Integ4_DSTATE_ey [ 0 ] =
_rtP -> P_1856 * _rtB -> B_311_2075_0 + _rtB -> B_311_2082_0 [ 0 ] ; _rtDW ->
Integ4_DSTATE_ey [ 1 ] = _rtP -> P_1856 * _rtB -> B_311_2081_0 + _rtB ->
B_311_2082_0 [ 1 ] ; ssCallAccelRunBlock ( S , 311 , 2084 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_gj [ 0 ] = _rtB ->
B_311_2075_0 ; _rtDW -> UnitDelay_DSTATE_gj [ 1 ] = _rtB -> B_311_2081_0 ;
_rtDW -> UnitDelay1_DSTATE_o0 = _rtB -> B_311_2176_0 ; _rtDW ->
UnitDelay1_DSTATE_ft = _rtB -> B_311_2172_0 ; _rtDW -> Delay_DSTATE_cw = _rtB
-> B_311_2124_0 ; _rtDW -> DiscreteTimeIntegrator_IC_LOADING_dn = 0U ; _rtDW
-> DiscreteTimeIntegrator_DSTATE_a5 += _rtP -> P_1870 * _rtB -> B_311_2159_0
; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_a5 >= _rtP -> P_1871 ) { _rtDW
-> DiscreteTimeIntegrator_DSTATE_a5 = _rtP -> P_1871 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 <= _rtP -> P_1872 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_a5 = _rtP -> P_1872 ; } } _rtDW ->
DiscreteTimeIntegrator_PrevResetState_gz = ( int8_T ) _rtB -> B_311_2120_0 ;
_rtDW -> DiscreteTimeIntegrator_DSTATE_g += _rtP -> P_1883 * _rtB ->
B_311_2165_0 ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_g >= _rtP ->
P_1885 ) { _rtDW -> DiscreteTimeIntegrator_DSTATE_g = _rtP -> P_1885 ; } else
{ if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_g <= _rtP -> P_1886 ) { _rtDW
-> DiscreteTimeIntegrator_DSTATE_g = _rtP -> P_1886 ; } } { real_T xnew [ 2 ]
; xnew [ 0 ] = ( _rtP -> P_1890 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_pq [ 0 ] + ( _rtP -> P_1890 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_pq [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_1891 [ 0 ] ) *
_rtB -> B_311_2166_0 ; xnew [ 1 ] = ( _rtP -> P_1890 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_pq [ 0 ] + ( _rtP -> P_1890 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_pq [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_1891 [ 1 ] ) *
_rtB -> B_311_2166_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_pq [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } } i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtDW -> Integrator_DSTATE_lv
+= _rtP -> P_1900 * _rtB -> B_311_148_0_b ; _rtDW ->
Integrator_PrevResetState_f3 = ( int8_T ) _rtB -> B_311_2180_0 ; _rtDW ->
Integrator_SYSTEM_ENABLE_c = 0U ; _rtDW -> Integrator_DSTATE_j5 [ 0 ] = _rtP
-> P_1901 * _rtB -> B_311_2189_0 [ 0 ] + _rtB -> B_311_2190_0 [ 0 ] ; _rtDW
-> UnitDelay1_DSTATE_et [ 0 ] = _rtB -> B_311_2190_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_ne [ 0 ] = _rtB -> B_311_2197_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_j5 [ 1 ] = _rtP -> P_1901 * _rtB -> B_311_2189_0 [ 1 ] +
_rtB -> B_311_2190_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_et [ 1 ] = _rtB ->
B_311_2190_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_ne [ 1 ] = _rtB ->
B_311_2197_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_j5 = 0U ; _rtDW ->
Integrator_DSTATE_og [ 0 ] = _rtP -> P_1910 * _rtB -> B_311_2206_0 [ 0 ] +
_rtB -> B_311_2207_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_p2c [ 0 ] = _rtB ->
B_311_2207_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_ew [ 0 ] = _rtB ->
B_311_2214_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_og [ 1 ] = _rtP -> P_1910 *
_rtB -> B_311_2206_0 [ 1 ] + _rtB -> B_311_2207_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_p2c [ 1 ] = _rtB -> B_311_2207_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_ew [ 1 ] = _rtB -> B_311_2214_0 [ 1 ] ; _rtDW ->
Integrator_SYSTEM_ENABLE_lk = 0U ; _rtDW -> Integrator_DSTATE_gv [ 0 ] = _rtP
-> P_1919 * _rtB -> B_311_2223_0 [ 0 ] + _rtB -> B_311_2224_0 [ 0 ] ; _rtDW
-> UnitDelay1_DSTATE_ow [ 0 ] = _rtB -> B_311_2224_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_d [ 0 ] = _rtB -> B_311_2231_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_gv [ 1 ] = _rtP -> P_1919 * _rtB -> B_311_2223_0 [ 1 ] +
_rtB -> B_311_2224_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_ow [ 1 ] = _rtB ->
B_311_2224_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_d [ 1 ] = _rtB ->
B_311_2231_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_o4 = 0U ; _rtDW ->
Integrator_DSTATE_ko [ 0 ] = _rtP -> P_1932 * _rtB -> B_311_2244_0 [ 0 ] +
_rtB -> B_311_2245_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_kp [ 0 ] = _rtB ->
B_311_2245_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_nw [ 0 ] = _rtB ->
B_311_2252_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_ko [ 1 ] = _rtP -> P_1932 *
_rtB -> B_311_2244_0 [ 1 ] + _rtB -> B_311_2245_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_kp [ 1 ] = _rtB -> B_311_2245_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_nw [ 1 ] = _rtB -> B_311_2252_0 [ 1 ] ; _rtDW ->
Integrator_SYSTEM_ENABLE_ef = 0U ; _rtDW -> Integrator_DSTATE_pki [ 0 ] =
_rtP -> P_1941 * _rtB -> B_311_2261_0 [ 0 ] + _rtB -> B_311_2262_0 [ 0 ] ;
_rtDW -> UnitDelay1_DSTATE_j1 [ 0 ] = _rtB -> B_311_2262_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_e2 [ 0 ] = _rtB -> B_311_2269_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_pki [ 1 ] = _rtP -> P_1941 * _rtB -> B_311_2261_0 [ 1 ] +
_rtB -> B_311_2262_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_j1 [ 1 ] = _rtB ->
B_311_2262_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_e2 [ 1 ] = _rtB ->
B_311_2269_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_b3 = 0U ; _rtDW ->
Integrator_DSTATE_bz [ 0 ] = _rtP -> P_1950 * _rtB -> B_311_2278_0 [ 0 ] +
_rtB -> B_311_2279_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_gu [ 0 ] = _rtB ->
B_311_2279_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_cd [ 0 ] = _rtB ->
B_311_2286_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_bz [ 1 ] = _rtP -> P_1950 *
_rtB -> B_311_2278_0 [ 1 ] + _rtB -> B_311_2279_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_gu [ 1 ] = _rtB -> B_311_2279_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_cd [ 1 ] = _rtB -> B_311_2286_0 [ 1 ] ; _rtDW ->
Integrator_DSTATE_d4 += _rtP -> P_1962 * _rtB -> B_311_153_0_d ; _rtDW ->
Integrator_PrevResetState_p = ( int8_T ) _rtB -> B_311_2304_0 ; _rtDW ->
Integrator_SYSTEM_ENABLE_mu = 0U ; _rtDW -> Integrator_DSTATE_px [ 0 ] = _rtP
-> P_1963 * _rtB -> B_311_2313_0 [ 0 ] + _rtB -> B_311_2314_0 [ 0 ] ; _rtDW
-> UnitDelay1_DSTATE_dqh [ 0 ] = _rtB -> B_311_2314_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_b5 [ 0 ] = _rtB -> B_311_2321_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_px [ 1 ] = _rtP -> P_1963 * _rtB -> B_311_2313_0 [ 1 ] +
_rtB -> B_311_2314_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_dqh [ 1 ] = _rtB ->
B_311_2314_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_b5 [ 1 ] = _rtB ->
B_311_2321_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_n2 = 0U ; _rtDW ->
Integrator_DSTATE_ms [ 0 ] = _rtP -> P_1972 * _rtB -> B_311_2330_0 [ 0 ] +
_rtB -> B_311_2331_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_im [ 0 ] = _rtB ->
B_311_2331_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_j2 [ 0 ] = _rtB ->
B_311_2338_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_ms [ 1 ] = _rtP -> P_1972 *
_rtB -> B_311_2330_0 [ 1 ] + _rtB -> B_311_2331_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_im [ 1 ] = _rtB -> B_311_2331_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_j2 [ 1 ] = _rtB -> B_311_2338_0 [ 1 ] ; _rtDW ->
Integrator_SYSTEM_ENABLE_iz = 0U ; _rtDW -> Integrator_DSTATE_gp [ 0 ] = _rtP
-> P_1981 * _rtB -> B_311_2347_0 [ 0 ] + _rtB -> B_311_2348_0 [ 0 ] ; _rtDW
-> UnitDelay1_DSTATE_fg [ 0 ] = _rtB -> B_311_2348_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_fn [ 0 ] = _rtB -> B_311_2355_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_gp [ 1 ] = _rtP -> P_1981 * _rtB -> B_311_2347_0 [ 1 ] +
_rtB -> B_311_2348_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_fg [ 1 ] = _rtB ->
B_311_2348_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_fn [ 1 ] = _rtB ->
B_311_2355_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_g = 0U ; _rtDW ->
Integrator_DSTATE_lq [ 0 ] = _rtP -> P_1990 * _rtB -> B_311_2364_0 [ 0 ] +
_rtB -> B_311_2365_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_ew [ 0 ] = _rtB ->
B_311_2365_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_lw [ 0 ] = _rtB ->
B_311_2372_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_lq [ 1 ] = _rtP -> P_1990 *
_rtB -> B_311_2364_0 [ 1 ] + _rtB -> B_311_2365_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_ew [ 1 ] = _rtB -> B_311_2365_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_lw [ 1 ] = _rtB -> B_311_2372_0 [ 1 ] ; _rtDW ->
Integrator_SYSTEM_ENABLE_ju = 0U ; _rtDW -> Integrator_DSTATE_ik [ 0 ] = _rtP
-> P_1999 * _rtB -> B_311_2381_0 [ 0 ] + _rtB -> B_311_2382_0 [ 0 ] ; _rtDW
-> UnitDelay1_DSTATE_fw [ 0 ] = _rtB -> B_311_2382_0 [ 0 ] ; _rtDW ->
UnitDelay2_DSTATE_mi [ 0 ] = _rtB -> B_311_2389_0 [ 0 ] ; _rtDW ->
Integrator_DSTATE_ik [ 1 ] = _rtP -> P_1999 * _rtB -> B_311_2381_0 [ 1 ] +
_rtB -> B_311_2382_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_fw [ 1 ] = _rtB ->
B_311_2382_0 [ 1 ] ; _rtDW -> UnitDelay2_DSTATE_mi [ 1 ] = _rtB ->
B_311_2389_0 [ 1 ] ; _rtDW -> Integrator_SYSTEM_ENABLE_ek = 0U ; _rtDW ->
Integrator_DSTATE_ba [ 0 ] = _rtP -> P_2008 * _rtB -> B_311_2398_0 [ 0 ] +
_rtB -> B_311_2399_0 [ 0 ] ; _rtDW -> UnitDelay1_DSTATE_ly [ 0 ] = _rtB ->
B_311_2399_0 [ 0 ] ; _rtDW -> UnitDelay2_DSTATE_mg [ 0 ] = _rtB ->
B_311_2406_0 [ 0 ] ; _rtDW -> Integrator_DSTATE_ba [ 1 ] = _rtP -> P_2008 *
_rtB -> B_311_2398_0 [ 1 ] + _rtB -> B_311_2399_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_ly [ 1 ] = _rtB -> B_311_2399_0 [ 1 ] ; _rtDW ->
UnitDelay2_DSTATE_mg [ 1 ] = _rtB -> B_311_2406_0 [ 1 ] ; for ( i = 0 ; i <
832 ; i ++ ) { _rtDW -> Delay_DSTATE_jl [ ( uint32_T ) i << 1 ] = _rtDW ->
Delay_DSTATE_jl [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_jl [ ( ( uint32_T
) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_jl [ ( ( i + 1U ) << 1 ) + 1U ] ;
_rtDW -> Delay_DSTATE_dk [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_dk [
( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_dk [ ( ( uint32_T ) i << 1 ) + 1U ]
= _rtDW -> Delay_DSTATE_dk [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW ->
Delay_DSTATE_fs [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_fs [ ( i + 1U
) << 1 ] ; _rtDW -> Delay_DSTATE_fs [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW
-> Delay_DSTATE_fs [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_mr [
( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_mr [ ( i + 1U ) << 1 ] ; _rtDW
-> Delay_DSTATE_mr [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW ->
Delay_DSTATE_mr [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_pr [ (
uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_pr [ ( i + 1U ) << 1 ] ; _rtDW ->
Delay_DSTATE_pr [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_pr [
( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_cvb [ ( uint32_T ) i << 1
] = _rtDW -> Delay_DSTATE_cvb [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_cvb
[ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_cvb [ ( ( i + 1U )
<< 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_ob [ ( uint32_T ) i << 1 ] = _rtDW ->
Delay_DSTATE_ob [ ( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_ob [ ( ( uint32_T
) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_ob [ ( ( i + 1U ) << 1 ) + 1U ] ;
_rtDW -> Delay_DSTATE_h4 [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_h4 [
( i + 1U ) << 1 ] ; _rtDW -> Delay_DSTATE_h4 [ ( ( uint32_T ) i << 1 ) + 1U ]
= _rtDW -> Delay_DSTATE_h4 [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW ->
Delay_DSTATE_oox [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_oox [ ( i +
1U ) << 1 ] ; _rtDW -> Delay_DSTATE_oox [ ( ( uint32_T ) i << 1 ) + 1U ] =
_rtDW -> Delay_DSTATE_oox [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW ->
Delay_DSTATE_lw [ ( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_lw [ ( i + 1U
) << 1 ] ; _rtDW -> Delay_DSTATE_lw [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW
-> Delay_DSTATE_lw [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_ft [
( uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_ft [ ( i + 1U ) << 1 ] ; _rtDW
-> Delay_DSTATE_ft [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW ->
Delay_DSTATE_ft [ ( ( i + 1U ) << 1 ) + 1U ] ; _rtDW -> Delay_DSTATE_cr [ (
uint32_T ) i << 1 ] = _rtDW -> Delay_DSTATE_cr [ ( i + 1U ) << 1 ] ; _rtDW ->
Delay_DSTATE_cr [ ( ( uint32_T ) i << 1 ) + 1U ] = _rtDW -> Delay_DSTATE_cr [
( ( i + 1U ) << 1 ) + 1U ] ; } _rtDW -> Delay_DSTATE_jl [ 1664 ] = _rtB ->
B_311_2190_0 [ 0 ] ; _rtDW -> Delay_DSTATE_jl [ 1665 ] = _rtB -> B_311_2190_0
[ 1 ] ; _rtDW -> Delay_DSTATE_dk [ 1664 ] = _rtB -> B_311_2207_0 [ 0 ] ;
_rtDW -> Delay_DSTATE_dk [ 1665 ] = _rtB -> B_311_2207_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_fs [ 1664 ] = _rtB -> B_311_2224_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_fs [ 1665 ] = _rtB -> B_311_2224_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_mr [ 1664 ] = _rtB -> B_311_2245_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_mr [ 1665 ] = _rtB -> B_311_2245_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_pr [ 1664 ] = _rtB -> B_311_2262_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_pr [ 1665 ] = _rtB -> B_311_2262_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_cvb [ 1664 ] = _rtB -> B_311_2279_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_cvb [ 1665 ] = _rtB -> B_311_2279_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_ob [ 1664 ] = _rtB -> B_311_2314_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_ob [ 1665 ] = _rtB -> B_311_2314_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_h4 [ 1664 ] = _rtB -> B_311_2331_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_h4 [ 1665 ] = _rtB -> B_311_2331_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_oox [ 1664 ] = _rtB -> B_311_2348_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_oox [ 1665 ] = _rtB -> B_311_2348_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_lw [ 1664 ] = _rtB -> B_311_2365_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_lw [ 1665 ] = _rtB -> B_311_2365_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_ft [ 1664 ] = _rtB -> B_311_2382_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_ft [ 1665 ] = _rtB -> B_311_2382_0 [ 1 ] ; _rtDW ->
Delay_DSTATE_cr [ 1664 ] = _rtB -> B_311_2399_0 [ 0 ] ; _rtDW ->
Delay_DSTATE_cr [ 1665 ] = _rtB -> B_311_2399_0 [ 1 ] ; _rtDW ->
Integ4_SYSTEM_ENABLE_jr = 0U ; _rtDW -> Integ4_DSTATE_b = _rtP -> P_2019 *
_rtB -> B_311_2569_0 + _rtB -> B_311_2570_0 ; ssCallAccelRunBlock ( S , 311 ,
2572 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_f4 = _rtB ->
B_311_2569_0 ; _rtDW -> DiscreteTimeIntegrator_DSTATE_pk += _rtP -> P_2030 *
_rtB -> B_311_2593_0 ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_pk >= _rtP
-> P_2032 ) { _rtDW -> DiscreteTimeIntegrator_DSTATE_pk = _rtP -> P_2032 ; }
else { if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_pk <= _rtP -> P_2033 ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE_pk = _rtP -> P_2033 ; } } _rtDW ->
UD_DSTATE = _rtB -> B_311_2582_0 ; _rtDW -> UnitDelay_DSTATE_l = _rtB ->
B_311_2591_0 ; { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_2043 [ 0 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_fa [ 0 ] + ( _rtP -> P_2043 [ 1 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_fa [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_2044
[ 0 ] ) * _rtB -> B_311_2591_0 ; xnew [ 1 ] = ( _rtP -> P_2043 [ 2 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_fa [ 0 ] + ( _rtP -> P_2043 [ 3 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_fa [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_2044
[ 1 ] ) * _rtB -> B_311_2591_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_fa [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> Integ4_SYSTEM_ENABLE_ev = 0U ; _rtDW -> Integ4_DSTATE_at = _rtP -> P_2050
* _rtB -> B_311_2608_0 + _rtB -> B_311_2609_0 ; ssCallAccelRunBlock ( S , 311
, 2611 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_ji = _rtB ->
B_311_2608_0 ; _rtDW -> DiscreteTimeIntegrator_DSTATE_m3 += _rtP -> P_2061 *
_rtB -> B_311_2632_0 ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_m3 >= _rtP
-> P_2063 ) { _rtDW -> DiscreteTimeIntegrator_DSTATE_m3 = _rtP -> P_2063 ; }
else { if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_m3 <= _rtP -> P_2064 ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE_m3 = _rtP -> P_2064 ; } } _rtDW ->
UD_DSTATE_i = _rtB -> B_311_2621_0 ; _rtDW -> UnitDelay_DSTATE_jq = _rtB ->
B_311_2630_0 ; { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_2074 [ 0 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_in [ 0 ] + ( _rtP -> P_2074 [ 1 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_in [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_2075
[ 0 ] ) * _rtB -> B_311_2630_0 ; xnew [ 1 ] = ( _rtP -> P_2074 [ 2 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_in [ 0 ] + ( _rtP -> P_2074 [ 3 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_in [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_2075
[ 1 ] ) * _rtB -> B_311_2630_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_in [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> Integ4_SYSTEM_ENABLE_fy = 0U ; _rtDW -> Integ4_DSTATE_ko = _rtP -> P_2081
* _rtB -> B_311_2654_0 + _rtB -> B_311_2655_0 ; ssCallAccelRunBlock ( S , 311
, 2657 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_gg = _rtB ->
B_311_2654_0 ; _rtDW -> DiscreteTimeIntegrator_DSTATE_d += _rtP -> P_2092 *
_rtB -> B_311_2678_0 ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_d >= _rtP
-> P_2094 ) { _rtDW -> DiscreteTimeIntegrator_DSTATE_d = _rtP -> P_2094 ; }
else { if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_d <= _rtP -> P_2095 ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE_d = _rtP -> P_2095 ; } } _rtDW ->
UD_DSTATE_m = _rtB -> B_311_2667_0 ; _rtDW -> UnitDelay_DSTATE_ob = _rtB ->
B_311_2676_0 ; { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_2105 [ 0 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_gw [ 0 ] + ( _rtP -> P_2105 [ 1 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_gw [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_2106
[ 0 ] ) * _rtB -> B_311_2676_0 ; xnew [ 1 ] = ( _rtP -> P_2105 [ 2 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_gw [ 0 ] + ( _rtP -> P_2105 [ 3 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_gw [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_2106
[ 1 ] ) * _rtB -> B_311_2676_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_gw [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> Integ4_SYSTEM_ENABLE_n = 0U ; _rtDW -> Integ4_DSTATE_cj = _rtP -> P_2112 *
_rtB -> B_311_2693_0 + _rtB -> B_311_2694_0 ; ssCallAccelRunBlock ( S , 311 ,
2696 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_aq = _rtB ->
B_311_2693_0 ; _rtDW -> DiscreteTimeIntegrator_DSTATE_f += _rtP -> P_2123 *
_rtB -> B_311_2717_0 ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_f >= _rtP
-> P_2125 ) { _rtDW -> DiscreteTimeIntegrator_DSTATE_f = _rtP -> P_2125 ; }
else { if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_f <= _rtP -> P_2126 ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE_f = _rtP -> P_2126 ; } } _rtDW ->
UD_DSTATE_p = _rtB -> B_311_2706_0 ; _rtDW -> UnitDelay_DSTATE_ar = _rtB ->
B_311_2715_0 ; { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_2136 [ 0 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_ie [ 0 ] + ( _rtP -> P_2136 [ 1 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_ie [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_2137
[ 0 ] ) * _rtB -> B_311_2715_0 ; xnew [ 1 ] = ( _rtP -> P_2136 [ 2 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_ie [ 0 ] + ( _rtP -> P_2136 [ 3 ] ) *
_rtDW -> DiscreteStateSpace_DSTATE_ie [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_2137
[ 1 ] ) * _rtB -> B_311_2715_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_ie [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } }
_rtDW -> Integrator_IWORK = 0 ; if ( _rtX -> Integrator_CSTATE_d == _rtP ->
P_2145 ) { switch ( _rtDW -> Integrator_MODE ) { case 3 : if ( _rtB ->
B_311_2752_0 < 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
_rtDW -> Integrator_MODE = 1 ; } break ; case 1 : if ( _rtB -> B_311_2752_0
>= 0.0 ) { _rtDW -> Integrator_MODE = 3 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; default :
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( _rtB -> B_311_2752_0
< 0.0 ) { _rtDW -> Integrator_MODE = 1 ; } else { _rtDW -> Integrator_MODE =
3 ; } break ; } } else if ( _rtX -> Integrator_CSTATE_d == _rtP -> P_2146 ) {
switch ( _rtDW -> Integrator_MODE ) { case 4 : if ( _rtB -> B_311_2752_0 >
0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW ->
Integrator_MODE = 2 ; } break ; case 2 : if ( _rtB -> B_311_2752_0 <= 0.0 ) {
_rtDW -> Integrator_MODE = 4 ; ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; } break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if
( _rtB -> B_311_2752_0 > 0.0 ) { _rtDW -> Integrator_MODE = 2 ; } else {
_rtDW -> Integrator_MODE = 4 ; } break ; } } else { _rtDW -> Integrator_MODE
= 0 ; } Auto_microgrid_forecasting_020822_AutomaticGainControl_Update ( S , &
_rtB -> AutomaticGainControl_c , & _rtDW -> AutomaticGainControl_c , & _rtP
-> AutomaticGainControl_c , & ( ( X_Auto_microgrid_forecasting_020822_T * )
ssGetContStates ( S ) ) -> AutomaticGainControl_c ) ; if ( _rtX ->
Integrator_CSTATE_c == _rtP -> P_2149 ) { switch ( _rtDW -> Integrator_MODE_j
) { case 3 : if ( _rtB -> B_311_2748_0 < 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_j
= 1 ; } break ; case 1 : if ( _rtB -> B_311_2748_0 >= 0.0 ) { _rtDW ->
Integrator_MODE_j = 3 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_311_2748_0 < 0.0 ) { _rtDW -> Integrator_MODE_j = 1 ; } else {
_rtDW -> Integrator_MODE_j = 3 ; } break ; } } else if ( _rtX ->
Integrator_CSTATE_c == _rtP -> P_2150 ) { switch ( _rtDW -> Integrator_MODE_j
) { case 4 : if ( _rtB -> B_311_2748_0 > 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_j
= 2 ; } break ; case 2 : if ( _rtB -> B_311_2748_0 <= 0.0 ) { _rtDW ->
Integrator_MODE_j = 4 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_311_2748_0 > 0.0 ) { _rtDW -> Integrator_MODE_j = 2 ; } else {
_rtDW -> Integrator_MODE_j = 4 ; } break ; } } else { _rtDW ->
Integrator_MODE_j = 0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> VariableTransportDelay_PWORK . TUbufferPtrs [ 1 ] ;
real_T * * xBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK .
TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
VariableTransportDelay_IWORK . Head = ( ( _rtDW ->
VariableTransportDelay_IWORK . Head < ( _rtDW -> VariableTransportDelay_IWORK
. CircularBufSize - 1 ) ) ? ( _rtDW -> VariableTransportDelay_IWORK . Head +
1 ) : 0 ) ; if ( _rtDW -> VariableTransportDelay_IWORK . Head == _rtDW ->
VariableTransportDelay_IWORK . Tail ) { if ( !
Auto_microgrid_forecasting_020822_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW
-> VariableTransportDelay_IWORK . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK . Tail , & _rtDW -> VariableTransportDelay_IWORK
. Head , & _rtDW -> VariableTransportDelay_IWORK . Last , simTime - _rtP ->
P_2151 , tBuffer , uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 1 , &
_rtDW -> VariableTransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus
( S , "vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [
_rtDW -> VariableTransportDelay_IWORK . Head ] = simTime ; ( * uBuffer ) [
_rtDW -> VariableTransportDelay_IWORK . Head ] = _rtB -> B_311_2740_0 ; ( *
xBuffer ) [ _rtDW -> VariableTransportDelay_IWORK . Head ] = ( (
X_Auto_microgrid_forecasting_020822_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i !=
0 ) { _rtDW -> Memory_PreviousInput_n = _rtB -> B_311_2745_0 ; } if ( _rtDW
-> LastMajorTimeA == ( rtInf ) ) { _rtDW -> LastMajorTimeA = ssGetTaskTime (
S , 0 ) ; _rtDW -> PrevYA = _rtB -> B_311_2754_0 ; } else if ( _rtDW ->
LastMajorTimeB == ( rtInf ) ) { _rtDW -> LastMajorTimeB = ssGetTaskTime ( S ,
0 ) ; _rtDW -> PrevYB = _rtB -> B_311_2754_0 ; } else if ( _rtDW ->
LastMajorTimeA < _rtDW -> LastMajorTimeB ) { _rtDW -> LastMajorTimeA =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA = _rtB -> B_311_2754_0 ; } else {
_rtDW -> LastMajorTimeB = ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYB = _rtB ->
B_311_2754_0 ; } _rtDW -> Integrator_IWORK_o = 0 ; if ( _rtX ->
Integrator_CSTATE_mn == _rtP -> P_2182 ) { switch ( _rtDW ->
Integrator_MODE_f ) { case 3 : if ( _rtB -> B_311_2799_0 < 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_f
= 1 ; } break ; case 1 : if ( _rtB -> B_311_2799_0 >= 0.0 ) { _rtDW ->
Integrator_MODE_f = 3 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_311_2799_0 < 0.0 ) { _rtDW -> Integrator_MODE_f = 1 ; } else {
_rtDW -> Integrator_MODE_f = 3 ; } break ; } } else if ( _rtX ->
Integrator_CSTATE_mn == _rtP -> P_2183 ) { switch ( _rtDW ->
Integrator_MODE_f ) { case 4 : if ( _rtB -> B_311_2799_0 > 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_f
= 2 ; } break ; case 2 : if ( _rtB -> B_311_2799_0 <= 0.0 ) { _rtDW ->
Integrator_MODE_f = 4 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_311_2799_0 > 0.0 ) { _rtDW -> Integrator_MODE_f = 2 ; } else {
_rtDW -> Integrator_MODE_f = 4 ; } break ; } } else { _rtDW ->
Integrator_MODE_f = 0 ; }
Auto_microgrid_forecasting_020822_AutomaticGainControl_Update ( S , & _rtB ->
AutomaticGainControl_e , & _rtDW -> AutomaticGainControl_e , & _rtP ->
AutomaticGainControl_e , & ( ( X_Auto_microgrid_forecasting_020822_T * )
ssGetContStates ( S ) ) -> AutomaticGainControl_e ) ; if ( _rtX ->
Integrator_CSTATE_dx == _rtP -> P_2186 ) { switch ( _rtDW ->
Integrator_MODE_c ) { case 3 : if ( _rtB -> B_311_2795_0 < 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_c
= 1 ; } break ; case 1 : if ( _rtB -> B_311_2795_0 >= 0.0 ) { _rtDW ->
Integrator_MODE_c = 3 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_311_2795_0 < 0.0 ) { _rtDW -> Integrator_MODE_c = 1 ; } else {
_rtDW -> Integrator_MODE_c = 3 ; } break ; } } else if ( _rtX ->
Integrator_CSTATE_dx == _rtP -> P_2187 ) { switch ( _rtDW ->
Integrator_MODE_c ) { case 4 : if ( _rtB -> B_311_2795_0 > 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW -> Integrator_MODE_c
= 2 ; } break ; case 2 : if ( _rtB -> B_311_2795_0 <= 0.0 ) { _rtDW ->
Integrator_MODE_c = 4 ; ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; }
break ; default : ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if (
_rtB -> B_311_2795_0 > 0.0 ) { _rtDW -> Integrator_MODE_c = 2 ; } else {
_rtDW -> Integrator_MODE_c = 4 ; } break ; } } else { _rtDW ->
Integrator_MODE_c = 0 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK_m . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_m . TUbufferPtrs [ 1 ] ;
real_T * * xBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_m
. TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
VariableTransportDelay_IWORK_e . Head = ( ( _rtDW ->
VariableTransportDelay_IWORK_e . Head < ( _rtDW ->
VariableTransportDelay_IWORK_e . CircularBufSize - 1 ) ) ? ( _rtDW ->
VariableTransportDelay_IWORK_e . Head + 1 ) : 0 ) ; if ( _rtDW ->
VariableTransportDelay_IWORK_e . Head == _rtDW ->
VariableTransportDelay_IWORK_e . Tail ) { if ( !
Auto_microgrid_forecasting_020822_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW
-> VariableTransportDelay_IWORK_e . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK_e . Tail , & _rtDW ->
VariableTransportDelay_IWORK_e . Head , & _rtDW ->
VariableTransportDelay_IWORK_e . Last , simTime - _rtP -> P_2188 , tBuffer ,
uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 1 , & _rtDW ->
VariableTransportDelay_IWORK_e . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_e . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_e . Head ] = _rtB -> B_311_2787_0 ; ( * xBuffer
) [ _rtDW -> VariableTransportDelay_IWORK_e . Head ] = ( (
X_Auto_microgrid_forecasting_020822_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE_h ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i
!= 0 ) { _rtDW -> Memory_PreviousInput_m = _rtB -> B_311_2792_0 ; } if (
_rtDW -> LastMajorTimeA_o == ( rtInf ) ) { _rtDW -> LastMajorTimeA_o =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_d = _rtB -> B_311_2801_0 ; } else
if ( _rtDW -> LastMajorTimeB_c == ( rtInf ) ) { _rtDW -> LastMajorTimeB_c =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYB_m = _rtB -> B_311_2801_0 ; } else
if ( _rtDW -> LastMajorTimeA_o < _rtDW -> LastMajorTimeB_c ) { _rtDW ->
LastMajorTimeA_o = ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_d = _rtB ->
B_311_2801_0 ; } else { _rtDW -> LastMajorTimeB_c = ssGetTaskTime ( S , 0 ) ;
_rtDW -> PrevYB_m = _rtB -> B_311_2801_0 ; } i = ssIsSampleHit ( S , 2 , 0 )
; if ( i != 0 ) { HoldSine = _rtDW -> lastSin_nh ; _rtDW -> lastSin_nh =
_rtDW -> lastSin_nh * _rtP -> P_2226 + _rtDW -> lastCos_e * _rtP -> P_2225 ;
_rtDW -> lastCos_e = _rtDW -> lastCos_e * _rtP -> P_2226 - HoldSine * _rtP ->
P_2225 ; _rtDW -> Integ4_SYSTEM_ENABLE_di = 0U ; _rtDW -> Integ4_DSTATE_ku =
_rtP -> P_2229 * _rtB -> B_311_2831_0 + _rtB -> B_311_2832_0 ;
ssCallAccelRunBlock ( S , 311 , 2834 , SS_CALL_MDL_UPDATE ) ; HoldSine =
_rtDW -> lastSin_pc ; _rtDW -> lastSin_pc = _rtDW -> lastSin_pc * _rtP ->
P_2244 + _rtDW -> lastCos_l5 * _rtP -> P_2243 ; _rtDW -> lastCos_l5 = _rtDW
-> lastCos_l5 * _rtP -> P_2244 - HoldSine * _rtP -> P_2243 ; _rtDW ->
Integ4_SYSTEM_ENABLE_er = 0U ; _rtDW -> Integ4_DSTATE_a4 = _rtP -> P_2247 *
_rtB -> B_311_2836_0 + _rtB -> B_311_2837_0 ; ssCallAccelRunBlock ( S , 311 ,
2839 , SS_CALL_MDL_UPDATE ) ; HoldSine = _rtDW -> lastSin_px ; _rtDW ->
lastSin_px = _rtDW -> lastSin_px * _rtP -> P_2262 + _rtDW -> lastCos_g5 *
_rtP -> P_2261 ; _rtDW -> lastCos_g5 = _rtDW -> lastCos_g5 * _rtP -> P_2262 -
HoldSine * _rtP -> P_2261 ; _rtDW -> Integ4_SYSTEM_ENABLE_lc = 0U ; _rtDW ->
Integ4_DSTATE_jk = _rtP -> P_2265 * _rtB -> B_311_2841_0 + _rtB ->
B_311_2842_0 ; ssCallAccelRunBlock ( S , 311 , 2844 , SS_CALL_MDL_UPDATE ) ;
HoldSine = _rtDW -> lastSin_cf ; _rtDW -> lastSin_cf = _rtDW -> lastSin_cf *
_rtP -> P_2280 + _rtDW -> lastCos_pu * _rtP -> P_2279 ; _rtDW -> lastCos_pu =
_rtDW -> lastCos_pu * _rtP -> P_2280 - HoldSine * _rtP -> P_2279 ; _rtDW ->
Integ4_SYSTEM_ENABLE_a4 = 0U ; _rtDW -> Integ4_DSTATE_ek = _rtP -> P_2283 *
_rtB -> B_311_2846_0 + _rtB -> B_311_2847_0 ; ssCallAccelRunBlock ( S , 311 ,
2849 , SS_CALL_MDL_UPDATE ) ; HoldSine = _rtDW -> lastSin_af ; _rtDW ->
lastSin_af = _rtDW -> lastSin_af * _rtP -> P_2298 + _rtDW -> lastCos_gl *
_rtP -> P_2297 ; _rtDW -> lastCos_gl = _rtDW -> lastCos_gl * _rtP -> P_2298 -
HoldSine * _rtP -> P_2297 ; _rtDW -> Integ4_SYSTEM_ENABLE_nk = 0U ; _rtDW ->
Integ4_DSTATE_cjp = _rtP -> P_2301 * _rtB -> B_311_2851_0 + _rtB ->
B_311_2852_0 ; ssCallAccelRunBlock ( S , 311 , 2854 , SS_CALL_MDL_UPDATE ) ;
HoldSine = _rtDW -> lastSin_ac ; _rtDW -> lastSin_ac = _rtDW -> lastSin_ac *
_rtP -> P_2316 + _rtDW -> lastCos_f * _rtP -> P_2315 ; _rtDW -> lastCos_f =
_rtDW -> lastCos_f * _rtP -> P_2316 - HoldSine * _rtP -> P_2315 ; _rtDW ->
Integ4_SYSTEM_ENABLE_pz = 0U ; _rtDW -> Integ4_DSTATE_bd = _rtP -> P_2319 *
_rtB -> B_311_2856_0 + _rtB -> B_311_2857_0 ; ssCallAccelRunBlock ( S , 311 ,
2859 , SS_CALL_MDL_UPDATE ) ; _rtDW -> Delay_x1_DSTATE [ 0 ] = _rtB ->
B_311_3640_0 [ 0 ] ; _rtDW -> Delay_x2_DSTATE [ 0 ] = _rtB -> B_311_3642_0 [
0 ] ; _rtDW -> Delay_x1_DSTATE [ 1 ] = _rtB -> B_311_3640_0 [ 1 ] ; _rtDW ->
Delay_x2_DSTATE [ 1 ] = _rtB -> B_311_3642_0 [ 1 ] ; _rtDW -> Delay_x1_DSTATE
[ 2 ] = _rtB -> B_311_3640_0 [ 2 ] ; _rtDW -> Delay_x2_DSTATE [ 2 ] = _rtB ->
B_311_3642_0 [ 2 ] ; HoldSine = _rtDW -> lastSin_j ; _rtDW -> lastSin_j =
_rtDW -> lastSin_j * _rtP -> P_2341 + _rtDW -> lastCos_lu * _rtP -> P_2340 ;
_rtDW -> lastCos_lu = _rtDW -> lastCos_lu * _rtP -> P_2341 - HoldSine * _rtP
-> P_2340 ; _rtDW -> Integ4_SYSTEM_ENABLE_d5 = 0U ; _rtDW -> Integ4_DSTATE_en
= _rtP -> P_2344 * _rtB -> B_311_2881_0 + _rtB -> B_311_2882_0 ;
ssCallAccelRunBlock ( S , 311 , 2884 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_fw = _rtB -> B_311_2881_0 ; _rtDW -> UnitDelay1_DSTATE_ou =
_rtB -> B_311_2891_0 ; HoldSine = _rtDW -> lastSin_c4 ; _rtDW -> lastSin_c4 =
_rtDW -> lastSin_c4 * _rtP -> P_2362 + _rtDW -> lastCos_b * _rtP -> P_2361 ;
_rtDW -> lastCos_b = _rtDW -> lastCos_b * _rtP -> P_2362 - HoldSine * _rtP ->
P_2361 ; _rtDW -> Integ4_SYSTEM_ENABLE_jq = 0U ; _rtDW -> Integ4_DSTATE_og =
_rtP -> P_2365 * _rtB -> B_311_2893_0 + _rtB -> B_311_2894_0 ;
ssCallAccelRunBlock ( S , 311 , 2896 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_cl = _rtB -> B_311_2893_0 ; _rtDW -> UnitDelay1_DSTATE_pb =
_rtB -> B_311_2903_0 ; HoldSine = _rtDW -> lastSin_e ; _rtDW -> lastSin_e =
_rtDW -> lastSin_e * _rtP -> P_2385 + _rtDW -> lastCos_a * _rtP -> P_2384 ;
_rtDW -> lastCos_a = _rtDW -> lastCos_a * _rtP -> P_2385 - HoldSine * _rtP ->
P_2384 ; _rtDW -> Integ4_SYSTEM_ENABLE_l2 = 0U ; _rtDW -> Integ4_DSTATE_bl =
_rtP -> P_2388 * _rtB -> B_311_2910_0 + _rtB -> B_311_2911_0 ;
ssCallAccelRunBlock ( S , 311 , 2913 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_a4 = _rtB -> B_311_2910_0 ; _rtDW -> UnitDelay1_DSTATE_f5 =
_rtB -> B_311_2920_0 ; HoldSine = _rtDW -> lastSin_ck ; _rtDW -> lastSin_ck =
_rtDW -> lastSin_ck * _rtP -> P_2406 + _rtDW -> lastCos_et * _rtP -> P_2405 ;
_rtDW -> lastCos_et = _rtDW -> lastCos_et * _rtP -> P_2406 - HoldSine * _rtP
-> P_2405 ; _rtDW -> Integ4_SYSTEM_ENABLE_b4 = 0U ; _rtDW -> Integ4_DSTATE_a2
= _rtP -> P_2409 * _rtB -> B_311_2922_0 + _rtB -> B_311_2923_0 ;
ssCallAccelRunBlock ( S , 311 , 2925 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_ai = _rtB -> B_311_2922_0 ; _rtDW -> UnitDelay1_DSTATE_p1 =
_rtB -> B_311_2932_0 ; HoldSine = _rtDW -> lastSin_aca ; _rtDW -> lastSin_aca
= _rtDW -> lastSin_aca * _rtP -> P_2429 + _rtDW -> lastCos_fr * _rtP ->
P_2428 ; _rtDW -> lastCos_fr = _rtDW -> lastCos_fr * _rtP -> P_2429 -
HoldSine * _rtP -> P_2428 ; _rtDW -> Integ4_SYSTEM_ENABLE_ke = 0U ; _rtDW ->
Integ4_DSTATE_l = _rtP -> P_2432 * _rtB -> B_311_2939_0 + _rtB ->
B_311_2940_0 ; ssCallAccelRunBlock ( S , 311 , 2942 , SS_CALL_MDL_UPDATE ) ;
_rtDW -> UnitDelay_DSTATE_nwk = _rtB -> B_311_2939_0 ; _rtDW ->
UnitDelay1_DSTATE_hv = _rtB -> B_311_2949_0 ; HoldSine = _rtDW -> lastSin_af0
; _rtDW -> lastSin_af0 = _rtDW -> lastSin_af0 * _rtP -> P_2450 + _rtDW ->
lastCos_o * _rtP -> P_2449 ; _rtDW -> lastCos_o = _rtDW -> lastCos_o * _rtP
-> P_2450 - HoldSine * _rtP -> P_2449 ; _rtDW -> Integ4_SYSTEM_ENABLE_nb = 0U
; _rtDW -> Integ4_DSTATE_kv = _rtP -> P_2453 * _rtB -> B_311_2951_0 + _rtB ->
B_311_2952_0 ; ssCallAccelRunBlock ( S , 311 , 2954 , SS_CALL_MDL_UPDATE ) ;
_rtDW -> UnitDelay_DSTATE_ck = _rtB -> B_311_2951_0 ; _rtDW ->
UnitDelay1_DSTATE_b3 = _rtB -> B_311_2961_0 ; _rtDW ->
DiscreteTimeIntegrator2_DSTATE += _rtP -> P_2470 * _rtB -> B_311_2986_0 ; if
( _rtB -> B_311_2986_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator2_PrevResetState = 1 ; } else if ( _rtB -> B_311_2986_0
< 0.0 ) { _rtDW -> DiscreteTimeIntegrator2_PrevResetState = - 1 ; } else if (
_rtB -> B_311_2986_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator2_PrevResetState = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator2_PrevResetState = 2 ; } _rtDW ->
DiscreteTimeIntegrator2_DSTATE_h += _rtP -> P_2476 * _rtB -> B_311_3009_0 ;
_rtDW -> Integ4_SYSTEM_ENABLE_oa = 0U ; _rtDW -> Integ4_DSTATE_bm = _rtP ->
P_2478 * _rtB -> B_311_2999_0 [ 0 ] + _rtB -> B_311_3000_0 ; _rtDW ->
DiscreteTimeIntegrator1_DSTATE_d += _rtP -> P_2480 * _rtB -> B_311_3627_0 ;
ssCallAccelRunBlock ( S , 311 , 3015 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_df = _rtB -> B_311_2999_0 [ 0 ] ; _rtDW ->
UnitDelay1_DSTATE_fm = _rtB -> B_311_3021_0 ; _rtDW ->
Integ4_SYSTEM_ENABLE_ee = 0U ; _rtDW -> Integ4_DSTATE_kl = _rtP -> P_2499 *
_rtB -> B_311_2999_0 [ 1 ] + _rtB -> B_311_3022_0 ; ssCallAccelRunBlock ( S ,
311 , 3027 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_ai4 = _rtB ->
B_311_2999_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_nn = _rtB -> B_311_3033_0 ;
_rtDW -> DiscreteTimeIntegrator2_DSTATE_e += _rtP -> P_2515 * _rtB ->
B_311_3038_0 ; if ( _rtB -> B_311_3038_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator2_PrevResetState_g = 1 ; } else if ( _rtB ->
B_311_3038_0 < 0.0 ) { _rtDW -> DiscreteTimeIntegrator2_PrevResetState_g = -
1 ; } else if ( _rtB -> B_311_3038_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator2_PrevResetState_g = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator2_PrevResetState_g = 2 ; } _rtDW ->
DiscreteTimeIntegrator3_DSTATE += _rtP -> P_2519 * _rtB -> B_311_3044_0 ; if
( _rtB -> B_311_3044_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator3_PrevResetState = 1 ; } else if ( _rtB -> B_311_3044_0
< 0.0 ) { _rtDW -> DiscreteTimeIntegrator3_PrevResetState = - 1 ; } else if (
_rtB -> B_311_3044_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator3_PrevResetState = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator3_PrevResetState = 2 ; } _rtDW ->
DiscreteTimeIntegrator1_DSTATE_o += _rtP -> P_2524 * _rtB -> B_311_3053_0 ;
if ( _rtB -> B_311_3053_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator1_PrevResetState_j = 1 ; } else if ( _rtB ->
B_311_3053_0 < 0.0 ) { _rtDW -> DiscreteTimeIntegrator1_PrevResetState_j = -
1 ; } else if ( _rtB -> B_311_3053_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator1_PrevResetState_j = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator1_PrevResetState_j = 2 ; } _rtDW ->
DiscreteTimeIntegrator4_DSTATE += _rtP -> P_2528 * _rtB -> B_311_3059_0 ; if
( _rtB -> B_311_3059_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator4_PrevResetState = 1 ; } else if ( _rtB -> B_311_3059_0
< 0.0 ) { _rtDW -> DiscreteTimeIntegrator4_PrevResetState = - 1 ; } else if (
_rtB -> B_311_3059_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator4_PrevResetState = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator4_PrevResetState = 2 ; } _rtDW -> UnitDelay_DSTATE_js =
_rtB -> B_311_3909_0 ; _rtDW -> dw_delay_DSTATE_c = _rtB -> B_311_3923_0 ;
_rtDW -> dw_predict_DSTATE_p = _rtB -> B_311_3069_0 ; } i = ssIsSampleHit ( S
, 4 , 0 ) ; if ( i != 0 ) { i = ssIsSampleHit ( S , 4 , 0 ) ; if ( ( i != 0 )
&& ( _rtDW -> SwitchCase_ActiveSubsystem_c == 0 ) ) { i = ssIsSampleHit ( S ,
4 , 0 ) ; if ( i != 0 ) { _rtDW -> UnitDelay_DSTATE_jb = _rtB -> B_208_47_0 ;
_rtDW -> ICic_PreviousInput_b = _rtB -> B_208_5_0_n ; _rtDW ->
Delay3_DSTATE_j = _rtB -> B_206_0_2 ; _rtDW -> Delay2_DSTATE_f = _rtB ->
B_206_0_3 ; _rtDW -> Delay1_DSTATE_n = _rtB -> B_206_0_4 ; _rtDW ->
Integrator_IC_LOADING_i = 0U ; _rtDW -> Integrator_DSTATE_kt += _rtP -> P_365
* _rtB -> B_208_42_0 ; if ( _rtDW -> Integrator_DSTATE_kt >= _rtP -> P_366 )
{ _rtDW -> Integrator_DSTATE_kt = _rtP -> P_366 ; } else { if ( _rtDW ->
Integrator_DSTATE_kt <= _rtP -> P_367 ) { _rtDW -> Integrator_DSTATE_kt =
_rtP -> P_367 ; } } _rtDW -> Integrator_PrevResetState_c = ( int8_T ) _rtB ->
B_208_14_0 ; _rtDW -> UD_DSTATE_k = _rtB -> B_208_19_0 ; _rtDW ->
DiscreteTransferFcn_states = _rtDW -> DiscreteTransferFcn_tmp ; _rtDW ->
ICic_PreviousInput = _rtB -> B_208_26_0 ; } } } i = ssIsSampleHit ( S , 2 , 0
) ; if ( i != 0 ) { _rtDW -> UnitDelay6_DSTATE = _rtB -> B_311_3624_0 ; _rtDW
-> UnitDelay7_DSTATE = _rtB -> B_311_3778_0 ; _rtDW ->
DiscreteTimeIntegrator1_DSTATE_b += _rtP -> P_2542 * _rtB -> B_311_3847_0 ;
_rtDW -> DiscreteTimeIntegrator_DSTATE_ni += _rtP -> P_2545 * _rtB ->
B_311_3846_0 ; _rtDW -> UnitDelay8_DSTATE = _rtB -> B_311_3823_0 ; _rtDW ->
Integ4_SYSTEM_ENABLE_cm = 0U ; _rtDW -> Integ4_DSTATE_m = _rtP -> P_2565 *
_rtB -> B_311_1246_0 + _rtB -> B_311_3143_0 ; ssCallAccelRunBlock ( S , 311 ,
3145 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_hq = _rtB ->
B_311_1246_0 ; _rtDW -> UnitDelay1_DSTATE_hy = _rtB -> B_311_3151_0 ; _rtDW
-> DiscreteTimeIntegrator_DSTATE_m3l += _rtP -> P_2591 * _rtB -> B_311_3292_0
; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_m3l >= _rtP -> P_2593 ) { _rtDW
-> DiscreteTimeIntegrator_DSTATE_m3l = _rtP -> P_2593 ; } else { if ( _rtDW
-> DiscreteTimeIntegrator_DSTATE_m3l <= _rtP -> P_2594 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_m3l = _rtP -> P_2594 ; } } _rtDW ->
Delay_x1_DSTATE_d = _rtB -> B_311_3717_0 ; _rtDW -> Delay_x2_DSTATE_h = _rtB
-> B_311_3719_0 ; _rtDW -> theta_DSTATE += _rtP -> P_2606 * _rtB ->
B_311_3925_0 ; _rtDW -> Delay_x1_DSTATE_b = _rtB -> B_311_3727_0 ; _rtDW ->
Delay_x2_DSTATE_f = _rtB -> B_311_3729_0 ; _rtDW -> Delay_x_DSTATE = _rtB ->
B_311_3776_0 ; _rtDW -> Delay_DSTATE_dv = _rtB -> B_311_3541_0 ; _rtDW ->
Delay1_DSTATE = _rtB -> B_311_3544_0 ; _rtDW -> Delay_x1_DSTATE_c [ 0 ] =
_rtB -> B_311_3660_0 [ 0 ] ; _rtDW -> Delay_x2_DSTATE_i [ 0 ] = _rtB ->
B_311_3662_0 [ 0 ] ; _rtDW -> Delay_x1_DSTATE_i [ 0 ] = _rtB -> B_311_3670_0
[ 0 ] ; _rtDW -> Delay_x2_DSTATE_fo [ 0 ] = _rtB -> B_311_3672_0 [ 0 ] ;
_rtDW -> Delay_x1_DSTATE_l [ 0 ] = _rtB -> B_311_3650_0 [ 0 ] ; _rtDW ->
Delay_x2_DSTATE_o [ 0 ] = _rtB -> B_311_3652_0 [ 0 ] ; _rtDW ->
Delay_x1_DSTATE_c [ 1 ] = _rtB -> B_311_3660_0 [ 1 ] ; _rtDW ->
Delay_x2_DSTATE_i [ 1 ] = _rtB -> B_311_3662_0 [ 1 ] ; _rtDW ->
Delay_x1_DSTATE_i [ 1 ] = _rtB -> B_311_3670_0 [ 1 ] ; _rtDW ->
Delay_x2_DSTATE_fo [ 1 ] = _rtB -> B_311_3672_0 [ 1 ] ; _rtDW ->
Delay_x1_DSTATE_l [ 1 ] = _rtB -> B_311_3650_0 [ 1 ] ; _rtDW ->
Delay_x2_DSTATE_o [ 1 ] = _rtB -> B_311_3652_0 [ 1 ] ; _rtDW ->
Delay_x1_DSTATE_c [ 2 ] = _rtB -> B_311_3660_0 [ 2 ] ; _rtDW ->
Delay_x2_DSTATE_i [ 2 ] = _rtB -> B_311_3662_0 [ 2 ] ; _rtDW ->
Delay_x1_DSTATE_i [ 2 ] = _rtB -> B_311_3670_0 [ 2 ] ; _rtDW ->
Delay_x2_DSTATE_fo [ 2 ] = _rtB -> B_311_3672_0 [ 2 ] ; _rtDW ->
Delay_x1_DSTATE_l [ 2 ] = _rtB -> B_311_3650_0 [ 2 ] ; _rtDW ->
Delay_x2_DSTATE_o [ 2 ] = _rtB -> B_311_3652_0 [ 2 ] ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_nt += _rtP -> P_2645 * _rtB -> B_311_3754_0 ;
_rtDW -> DiscreteTimeIntegrator_PrevResetState_nb = ( int8_T ) _rtB ->
B_311_3312_0 ; _rtDW -> Delay2_DSTATE = _rtB -> B_311_3526_0 ; _rtDW ->
Delay3_DSTATE = _rtB -> B_311_3529_0 ; _rtDW -> DiscreteTransferFcn3_states =
( _rtB -> B_311_3461_0 - _rtP -> P_2651 [ 1 ] * _rtDW ->
DiscreteTransferFcn3_states ) / _rtP -> P_2651 [ 0 ] ; _rtDW ->
Delay_x1_DSTATE_m = _rtB -> B_311_3707_0 ; _rtDW -> Delay_x2_DSTATE_n = _rtB
-> B_311_3709_0 ; _rtDW -> DiscreteTransferFcn1_icLoad = 0U ; _rtDW ->
DiscreteTransferFcn1_states = ( _rtB -> B_311_3333_0 - _rtP -> P_2659 [ 1 ] *
_rtDW -> DiscreteTransferFcn1_states ) / _rtP -> P_2659 [ 0 ] ; _rtDW ->
DiscreteTransferFcn4_states = ( _rtB -> B_311_3078_0 - _rtP -> P_2661 [ 1 ] *
_rtDW -> DiscreteTransferFcn4_states ) / _rtP -> P_2661 [ 0 ] ; _rtDW ->
Delay_x1_DSTATE_cl = _rtB -> B_311_3537_0 ; _rtDW -> Delay_x2_DSTATE_hg =
_rtB -> B_311_3539_0 ; _rtDW -> DiscreteTimeIntegrator3_DSTATE_c += _rtP ->
P_2674 * _rtB -> B_311_302_0_o ; if ( _rtDW ->
DiscreteTimeIntegrator3_DSTATE_c >= _rtP -> P_2676 ) { _rtDW ->
DiscreteTimeIntegrator3_DSTATE_c = _rtP -> P_2676 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator3_DSTATE_c <= _rtP -> P_2677 ) { _rtDW ->
DiscreteTimeIntegrator3_DSTATE_c = _rtP -> P_2677 ; } } _rtDW ->
DiscreteTimeIntegrator3_PrevResetState_l = ( int8_T ) _rtB -> B_311_3364_0 ;
_rtDW -> UnitDelay_DSTATE_dh = _rtB -> B_311_3374_0 ; _rtDW ->
Delay_DSTATE_io = _rtB -> B_311_3523_0 ; _rtDW -> Delay1_DSTATE_b = _rtB ->
B_311_3380_0 ; _rtDW -> DiscreteIntegrator_DSTATE = _rtP -> P_2684 * _rtB ->
B_311_3359_0 + _rtB -> B_311_3387_0 ; _rtDW -> UnitDelay_DSTATE_le = _rtB ->
B_311_3393_0 ; _rtDW -> UnitDelay_DSTATE_i = _rtB -> B_311_3406_0 ; _rtDW ->
DiscreteTransferFcn2_states = ( _rtB -> B_311_3444_0 - _rtP -> P_2690 [ 1 ] *
_rtDW -> DiscreteTransferFcn2_states ) / _rtP -> P_2690 [ 0 ] ; _rtDW ->
UnitDelay_DSTATE_fb = _rtB -> B_311_3773_0 ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_nh [ 0 ] += _rtP -> P_2702 * _rtB ->
B_311_3471_0 [ 0 ] ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_nh [ 0 ] >=
_rtP -> P_2704 ) { _rtDW -> DiscreteTimeIntegrator_DSTATE_nh [ 0 ] = _rtP ->
P_2704 ; } else { if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_nh [ 0 ] <=
_rtP -> P_2705 ) { _rtDW -> DiscreteTimeIntegrator_DSTATE_nh [ 0 ] = _rtP ->
P_2705 ; } } _rtDW -> DiscreteTimeIntegrator_DSTATE_nh [ 1 ] += _rtP ->
P_2702 * _rtB -> B_311_3471_0 [ 1 ] ; if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_nh [ 1 ] >= _rtP -> P_2704 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_nh [ 1 ] = _rtP -> P_2704 ; } else { if ( _rtDW
-> DiscreteTimeIntegrator_DSTATE_nh [ 1 ] <= _rtP -> P_2705 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_nh [ 1 ] = _rtP -> P_2705 ; } } _rtDW ->
Delay_x1_DSTATE_h = _rtB -> B_311_3697_0 ; _rtDW -> Delay_x2_DSTATE_d = _rtB
-> B_311_3699_0 ; for ( i = 0 ; i < 1665 ; i ++ ) { _rtDW -> Delayd1_DSTATE [
( uint32_T ) i ] = _rtDW -> Delayd1_DSTATE [ i + 1U ] ; _rtDW ->
Delayq1_DSTATE [ ( uint32_T ) i ] = _rtDW -> Delayq1_DSTATE [ i + 1U ] ; }
_rtDW -> Delayd1_DSTATE [ 1665 ] = _rtB -> B_311_3514_0 ; _rtDW ->
Delayq1_DSTATE [ 1665 ] = _rtB -> B_311_3481_0 ; _rtDW -> UnitDelay_DSTATE_kf
= _rtB -> B_311_3364_0 ; _rtDW -> UnitDelay_DSTATE_lv = _rtB -> B_311_3393_0
; _rtDW -> Delay_DSTATE_fp = _rtB -> B_311_3520_0 ; _rtDW -> Delay1_DSTATE_a
= _rtB -> B_311_3500_0 ; _rtDW -> ICic_DSTATE = _rtB -> B_311_3505_0 ; _rtDW
-> DiscreteIntegrator_DSTATE_l = _rtP -> P_2743 * _rtB -> B_311_3493_0 + _rtB
-> B_311_3510_0 ; _rtDW -> ICic_DSTATE_e = _rtB -> B_311_3519_0 ; _rtDW ->
ICic_DSTATE_m = _rtB -> B_311_3522_0 ; _rtDW -> Integ4_SYSTEM_ENABLE_fs = 0U
; _rtDW -> Integ4_DSTATE_j2 = _rtP -> P_2757 * _rtB -> B_311_3556_0 [ 0 ] +
_rtB -> B_311_3557_0 ; ssCallAccelRunBlock ( S , 311 , 3562 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_d0 = _rtB -> B_311_3556_0 [
0 ] ; _rtDW -> UnitDelay_DSTATE_bs = _rtB -> B_311_3567_0 ; _rtDW ->
Integ4_SYSTEM_ENABLE_lu = 0U ; _rtDW -> Integ4_DSTATE_d = _rtP -> P_2768 *
_rtB -> B_311_3556_0 [ 1 ] + _rtB -> B_311_3568_0 ; ssCallAccelRunBlock ( S ,
311 , 3573 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_db = _rtB ->
B_311_3556_0 [ 1 ] ; _rtDW -> UnitDelay_DSTATE_e3 = _rtB -> B_311_3578_0 ;
_rtDW -> Integ4_SYSTEM_ENABLE_c0 = 0U ; _rtDW -> Integ4_DSTATE_d2 = _rtP ->
P_2781 * _rtB -> B_311_3584_0 [ 0 ] + _rtB -> B_311_3585_0 ;
ssCallAccelRunBlock ( S , 311 , 3590 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_di = _rtB -> B_311_3584_0 [ 0 ] ; _rtDW ->
UnitDelay_DSTATE_lk = _rtB -> B_311_3595_0 ; _rtDW -> Integ4_SYSTEM_ENABLE_cw
= 0U ; _rtDW -> Integ4_DSTATE_d1 = _rtP -> P_2792 * _rtB -> B_311_3584_0 [ 1
] + _rtB -> B_311_3596_0 ; ssCallAccelRunBlock ( S , 311 , 3601 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_ax = _rtB -> B_311_3584_0 [
1 ] ; _rtDW -> UnitDelay_DSTATE_mh = _rtB -> B_311_3606_0 ; _rtDW ->
Delay_x1_DSTATE_de = _rtB -> B_311_3682_0 ; _rtDW -> Delay_x2_DSTATE_j = _rtB
-> B_311_3684_0 ; if ( _rtDW -> AutomaticGainControl_MODE_g ) { i =
ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtDW ->
Integ4_SYSTEM_ENABLE_lr = 0U ; _rtDW -> Integ4_DSTATE_l5 = _rtP -> P_454 *
_rtB -> B_271_4_0 [ 0 ] + _rtB -> B_271_5_0 ; ssCallAccelRunBlock ( S , 271 ,
10 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_ew = _rtB -> B_271_4_0
[ 0 ] ; _rtDW -> UnitDelay1_DSTATE_cc = _rtB -> B_271_16_0 ; _rtDW ->
Integ4_SYSTEM_ENABLE_nz = 0U ; _rtDW -> Integ4_DSTATE_cy = _rtP -> P_469 *
_rtB -> B_271_4_0 [ 1 ] + _rtB -> B_271_17_0 ; ssCallAccelRunBlock ( S , 271
, 22 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_ne = _rtB ->
B_271_4_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_nvr = _rtB -> B_271_28_0 ; } }
_rtDW -> Integ4_SYSTEM_ENABLE_br = 0U ; _rtDW -> Integ4_DSTATE_j2b = _rtP ->
P_2873 * _rtB -> B_311_3736_0 [ 1 ] + _rtB -> B_311_3737_0 ;
ssCallAccelRunBlock ( S , 311 , 3742 , SS_CALL_MDL_UPDATE ) ; _rtDW ->
UnitDelay_DSTATE_hc = _rtB -> B_311_3736_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_jv = _rtB -> B_311_3748_0 ; _rtDW ->
DiscreteDerivative_states = _rtDW -> DiscreteDerivative_tmp ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_gt += _rtP -> P_2891 * _rtB -> B_311_3749_0 ;
if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_gt >= _rtP -> P_2893 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_gt = _rtP -> P_2893 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_gt <= _rtP -> P_2894 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_gt = _rtP -> P_2894 ; } } _rtDW ->
Delay_x1_DSTATE_j = _rtB -> B_311_3766_0 ; _rtDW -> Delay_x2_DSTATE_ht = _rtB
-> B_311_3768_0 ; _rtDW -> Delay_x_DSTATE_l = _rtB -> B_311_3832_0 ; _rtDW ->
Integrator_DSTATE_j1 += _rtP -> P_2924 * _rtB -> B_311_3836_0 ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_mc += _rtP -> P_2930 * _rtB -> B_311_3826_0 ;
if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_mc >= _rtP -> P_2932 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_mc = _rtP -> P_2932 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_mc <= _rtP -> P_2933 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_mc = _rtP -> P_2933 ; } } _rtDW ->
Delay1_DSTATE_m = _rtB -> B_282_0_1 ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_hk += _rtP -> P_2941 * _rtB -> B_311_3825_0 ;
if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_hk >= _rtP -> P_2943 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_hk = _rtP -> P_2943 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_hk <= _rtP -> P_2944 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_hk = _rtP -> P_2944 ; } } } if ( _rtDW ->
LastMajorTimeA_a == ( rtInf ) ) { _rtDW -> LastMajorTimeA_a = ssGetTaskTime (
S , 0 ) ; _rtDW -> PrevYA_o = _rtB -> B_311_3813_0 ; } else if ( _rtDW ->
LastMajorTimeB_h == ( rtInf ) ) { _rtDW -> LastMajorTimeB_h = ssGetTaskTime (
S , 0 ) ; _rtDW -> PrevYB_f = _rtB -> B_311_3813_0 ; } else if ( _rtDW ->
LastMajorTimeA_a < _rtDW -> LastMajorTimeB_h ) { _rtDW -> LastMajorTimeA_a =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_o = _rtB -> B_311_3813_0 ; } else {
_rtDW -> LastMajorTimeB_h = ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYB_f =
_rtB -> B_311_3813_0 ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
_rtDW -> Delay_x_DSTATE_a = _rtB -> B_311_3829_0 ; _rtDW ->
UnitDelay2_DSTATE_nh = _rtB -> B_311_3294_0 ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_oc += _rtP -> P_2979 * _rtB -> B_311_3890_0 ;
_rtDW -> DiscreteTimeIntegrator_PrevResetState_ox = ( int8_T ) _rtB ->
B_311_3862_0 ; if ( _rtDW -> AutomaticGainControl_MODE ) { i = ssIsSampleHit
( S , 2 , 0 ) ; if ( i != 0 ) { _rtDW -> Integ4_SYSTEM_ENABLE_kb = 0U ; _rtDW
-> Integ4_DSTATE_f5 = _rtP -> P_502 * _rtB -> B_292_4_0 [ 0 ] + _rtB ->
B_292_5_0 ; ssCallAccelRunBlock ( S , 292 , 10 , SS_CALL_MDL_UPDATE ) ; _rtDW
-> UnitDelay_DSTATE_m4 = _rtB -> B_292_4_0 [ 0 ] ; _rtDW ->
UnitDelay1_DSTATE_bj = _rtB -> B_292_16_0 ; _rtDW -> Integ4_SYSTEM_ENABLE_hc
= 0U ; _rtDW -> Integ4_DSTATE_p5 = _rtP -> P_517 * _rtB -> B_292_4_0 [ 1 ] +
_rtB -> B_292_17_0 ; ssCallAccelRunBlock ( S , 292 , 22 , SS_CALL_MDL_UPDATE
) ; _rtDW -> UnitDelay_DSTATE_m1 = _rtB -> B_292_4_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_nv = _rtB -> B_292_28_0 ; } } _rtDW ->
Integ4_SYSTEM_ENABLE_c3 = 0U ; _rtDW -> Integ4_DSTATE_ce = _rtP -> P_2983 *
_rtB -> B_311_3872_0 [ 1 ] + _rtB -> B_311_3873_0 ; ssCallAccelRunBlock ( S ,
311 , 3878 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_jz = _rtB ->
B_311_3872_0 [ 1 ] ; _rtDW -> UnitDelay1_DSTATE_ky = _rtB -> B_311_3884_0 ;
_rtDW -> DiscreteDerivative_states_l = _rtDW -> DiscreteDerivative_tmp_m ;
_rtDW -> DiscreteTimeIntegrator_DSTATE_o5 += _rtP -> P_3001 * _rtB ->
B_311_3885_0 ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_o5 >= _rtP ->
P_3003 ) { _rtDW -> DiscreteTimeIntegrator_DSTATE_o5 = _rtP -> P_3003 ; }
else { if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_o5 <= _rtP -> P_3004 ) {
_rtDW -> DiscreteTimeIntegrator_DSTATE_o5 = _rtP -> P_3004 ; } } _rtDW ->
Delay_x1_DSTATE_d4 = _rtB -> B_311_3902_0 ; _rtDW -> Delay_x2_DSTATE_c = _rtB
-> B_311_3904_0 ; _rtDW -> voltages_DSTATE [ 0 ] = _rtB -> B_311_3912_0 ;
_rtDW -> voltages_DSTATE [ 1 ] = _rtB -> B_311_3913_0 ; _rtDW ->
voltages_DSTATE [ 2 ] = _rtB -> B_311_3910_0 ; _rtDW -> voltages_DSTATE [ 3 ]
= _rtB -> B_311_422_0 [ 0 ] ; _rtDW -> voltages_DSTATE [ 4 ] = _rtB ->
B_311_422_0 [ 1 ] ; _rtDW -> Rotorspeeddeviationdw_SYSTEM_ENABLE = 0U ; _rtDW
-> Rotorspeeddeviationdw_DSTATE = _rtP -> P_3030 * _rtB -> B_311_3922_0 +
_rtB -> B_311_3923_0 ; } i = ssIsSampleHit ( S , 3 , 0 ) ; if ( i != 0 ) {
_rtDW -> UnitDelay_DSTATE_flb [ 0 ] = _rtB -> B_311_4127_0 ; _rtDW ->
UnitDelay_DSTATE_flb [ 1 ] = _rtB -> B_311_4134_0 ; _rtDW ->
UnitDelay2_DSTATE_nc [ 0 ] = _rtB -> B_311_4332_0 ; _rtDW ->
UnitDelay2_DSTATE_nc [ 1 ] = _rtB -> B_311_4333_0 ; _rtDW ->
UnitDelay_DSTATE_an [ 0 ] = _rtB -> B_311_4196_0 [ 0 ] ; _rtDW ->
UnitDelay1_DSTATE_gm [ 0 ] = _rtB -> B_311_4253_0 [ 0 ] ; _rtDW ->
UnitDelay_DSTATE_an [ 1 ] = _rtB -> B_311_4196_0 [ 1 ] ; _rtDW ->
UnitDelay1_DSTATE_gm [ 1 ] = _rtB -> B_311_4253_0 [ 1 ] ; _rtDW ->
DiscreteTimeIntegrator_IC_LOADING_kw = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_c = _rtB -> B_311_4125_0 ; _rtDW ->
DiscreteTimeIntegrator_PrevResetState_d = ( int8_T ) _rtB -> B_311_4122_0 ;
_rtDW -> DiscreteTimeIntegrator_IC_LOADING_nm = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_m4 = _rtB -> B_311_4132_0 ; _rtDW ->
DiscreteTimeIntegrator_PrevResetState_ii = ( int8_T ) _rtB -> B_311_4130_0 ;
_rtDW -> UnitDelay_DSTATE_ir = _rtB -> B_311_4326_0 ; _rtDW ->
UnitDelay_DSTATE_c4 [ 0 ] = _rtB -> B_311_4332_0 ; _rtDW ->
UnitDelay_DSTATE_c4 [ 1 ] = _rtB -> B_311_4333_0 ; { real_T xnew [ 2 ] ; xnew
[ 0 ] = ( _rtP -> P_3088 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_pd [ 0
] + ( _rtP -> P_3088 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_pd [ 1 ] ;
xnew [ 0 ] += ( _rtP -> P_3089 [ 0 ] ) * _rtB -> B_311_4152_0 ; xnew [ 1 ] =
( _rtP -> P_3088 [ 2 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_pd [ 0 ] + (
_rtP -> P_3088 [ 3 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_pd [ 1 ] ; xnew [
1 ] += ( _rtP -> P_3089 [ 1 ] ) * _rtB -> B_311_4152_0 ; ( void ) memcpy ( &
_rtDW -> DiscreteStateSpace_DSTATE_pd [ 0 ] , xnew , sizeof ( real_T ) * 2 )
; } { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_3093 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_n [ 0 ] + ( _rtP -> P_3093 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_n [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_3094 [ 0 ] ) *
_rtB -> B_311_4154_0 ; xnew [ 1 ] = ( _rtP -> P_3093 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_n [ 0 ] + ( _rtP -> P_3093 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_n [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_3094 [ 1 ] ) *
_rtB -> B_311_4154_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_n [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> UnitDelay1_DSTATE_hvi [ 0 ] = _rtB -> B_311_4332_0 ; _rtDW ->
UnitDelay1_DSTATE_hvi [ 1 ] = _rtB -> B_311_4333_0 ; { real_T xnew [ 2 ] ;
xnew [ 0 ] = ( _rtP -> P_3107 [ 0 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_lg
[ 0 ] + ( _rtP -> P_3107 [ 1 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_lg [ 1
] ; xnew [ 0 ] += ( _rtP -> P_3108 [ 0 ] ) * _rtB -> B_311_4179_0 [ 2 ] ;
xnew [ 1 ] = ( _rtP -> P_3107 [ 2 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_lg
[ 0 ] + ( _rtP -> P_3107 [ 3 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_lg [ 1
] ; xnew [ 1 ] += ( _rtP -> P_3108 [ 1 ] ) * _rtB -> B_311_4179_0 [ 2 ] ; (
void ) memcpy ( & _rtDW -> DiscreteStateSpace_DSTATE_lg [ 0 ] , xnew , sizeof
( real_T ) * 2 ) ; } { real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_3112 [ 0
] ) * _rtDW -> DiscreteStateSpace_DSTATE_b2 [ 0 ] + ( _rtP -> P_3112 [ 1 ] )
* _rtDW -> DiscreteStateSpace_DSTATE_b2 [ 1 ] ; xnew [ 0 ] += ( _rtP ->
P_3113 [ 0 ] ) * _rtB -> B_311_4179_0 [ 3 ] ; xnew [ 1 ] = ( _rtP -> P_3112 [
2 ] ) * _rtDW -> DiscreteStateSpace_DSTATE_b2 [ 0 ] + ( _rtP -> P_3112 [ 3 ]
) * _rtDW -> DiscreteStateSpace_DSTATE_b2 [ 1 ] ; xnew [ 1 ] += ( _rtP ->
P_3113 [ 1 ] ) * _rtB -> B_311_4179_0 [ 3 ] ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_b2 [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } {
real_T xnew [ 1 ] ; xnew [ 0 ] = _rtP -> P_3119 * _rtDW ->
DiscreteStateSpace_DSTATE_oa ; xnew [ 0 ] += _rtP -> P_3120 * _rtB ->
B_311_4179_0 [ 0 ] ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_oa , xnew , sizeof ( real_T ) * 1 ) ; } { real_T
xnew [ 1 ] ; xnew [ 0 ] = _rtP -> P_3124 * _rtDW ->
DiscreteStateSpace_DSTATE_ow ; xnew [ 0 ] += _rtP -> P_3125 * _rtB ->
B_311_4179_0 [ 1 ] ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_ow , xnew , sizeof ( real_T ) * 1 ) ; } _rtDW ->
UnitDelay_DSTATE_e1 [ 0 ] = _rtB -> B_311_4233_0 [ 0 ] ; _rtDW ->
UnitDelay_DSTATE_e1 [ 1 ] = _rtB -> B_311_4233_0 [ 1 ] ; { real_T xnew [ 2 ]
; xnew [ 0 ] = ( _rtP -> P_3134 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_bt [ 0 ] + ( _rtP -> P_3134 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_bt [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_3135 [ 0 ] ) *
_rtB -> B_311_4174_0 ; xnew [ 1 ] = ( _rtP -> P_3134 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_bt [ 0 ] + ( _rtP -> P_3134 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_bt [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_3135 [ 1 ] ) *
_rtB -> B_311_4174_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_bt [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } {
real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_3139 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_fm [ 0 ] + ( _rtP -> P_3139 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_fm [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_3140 [ 0 ] ) *
_rtB -> B_311_4175_0 ; xnew [ 1 ] = ( _rtP -> P_3139 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_fm [ 0 ] + ( _rtP -> P_3139 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_fm [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_3140 [ 1 ] ) *
_rtB -> B_311_4175_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_fm [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> UnitDelay_DSTATE_ff [ 0 ] = _rtB -> B_311_4253_0 [ 0 ] ; _rtDW ->
UnitDelay_DSTATE_ff [ 1 ] = _rtB -> B_311_4253_0 [ 1 ] ; { real_T xnew [ 2 ]
; xnew [ 0 ] = ( _rtP -> P_3145 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_h [ 0 ] + ( _rtP -> P_3145 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_h [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_3146 [ 0 ] ) *
_rtB -> B_311_4112_0 ; xnew [ 1 ] = ( _rtP -> P_3145 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_h [ 0 ] + ( _rtP -> P_3145 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_h [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_3146 [ 1 ] ) *
_rtB -> B_311_4112_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_h [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } {
real_T xnew [ 2 ] ; xnew [ 0 ] = ( _rtP -> P_3150 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_cw [ 0 ] + ( _rtP -> P_3150 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_cw [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_3151 [ 0 ] ) *
_rtB -> B_311_4113_0 ; xnew [ 1 ] = ( _rtP -> P_3150 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_cw [ 0 ] + ( _rtP -> P_3150 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_cw [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_3151 [ 1 ] ) *
_rtB -> B_311_4113_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_cw [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } _rtDW
-> DiscreteTimeIntegrator_IC_LOADING_m = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_ei = _rtB -> B_311_4210_0 ; _rtDW ->
DiscreteTimeIntegrator_PrevResetState_j = ( int8_T ) _rtB -> B_311_4203_0 ;
_rtDW -> DiscreteTimeIntegrator_IC_LOADING_pq = 0U ; _rtDW ->
DiscreteTimeIntegrator_DSTATE_dw = _rtB -> B_311_4217_0 ; _rtDW ->
DiscreteTimeIntegrator_PrevResetState_l = ( int8_T ) _rtB -> B_311_4215_0 ;
_rtDW -> Integ4_SYSTEM_ENABLE_jqs = 0U ; _rtDW -> Integ4_DSTATE_o5 [ 0 ] =
_rtP -> P_3170 * _rtB -> B_311_4235_0 + _rtB -> B_311_4242_0 [ 0 ] ; _rtDW ->
Integ4_DSTATE_o5 [ 1 ] = _rtP -> P_3170 * _rtB -> B_311_4241_0 + _rtB ->
B_311_4242_0 [ 1 ] ; ssCallAccelRunBlock ( S , 311 , 4244 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_no [ 0 ] = _rtB ->
B_311_4235_0 ; _rtDW -> UnitDelay_DSTATE_no [ 1 ] = _rtB -> B_311_4241_0 ;
_rtDW -> UnitDelay1_DSTATE_pf = _rtB -> B_311_4335_0 ; _rtDW ->
UnitDelay1_DSTATE_c4 = _rtB -> B_311_4331_0 ; _rtDW -> Delay_DSTATE_je = _rtB
-> B_311_4284_0 ; _rtDW -> DiscreteTimeIntegrator_IC_LOADING_h = 0U ; _rtDW
-> DiscreteTimeIntegrator_DSTATE_ff += _rtP -> P_3184 * _rtB -> B_311_4318_0
; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_ff >= _rtP -> P_3185 ) { _rtDW
-> DiscreteTimeIntegrator_DSTATE_ff = _rtP -> P_3185 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff <= _rtP -> P_3186 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_ff = _rtP -> P_3186 ; } } _rtDW ->
DiscreteTimeIntegrator_PrevResetState_dg = ( int8_T ) _rtB -> B_311_4280_0 ;
_rtDW -> DiscreteTimeIntegrator_DSTATE_b += _rtP -> P_3197 * _rtB ->
B_311_4324_0 ; if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_b >= _rtP ->
P_3199 ) { _rtDW -> DiscreteTimeIntegrator_DSTATE_b = _rtP -> P_3199 ; } else
{ if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_b <= _rtP -> P_3200 ) { _rtDW
-> DiscreteTimeIntegrator_DSTATE_b = _rtP -> P_3200 ; } } { real_T xnew [ 2 ]
; xnew [ 0 ] = ( _rtP -> P_3204 [ 0 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_gv [ 0 ] + ( _rtP -> P_3204 [ 1 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_gv [ 1 ] ; xnew [ 0 ] += ( _rtP -> P_3205 [ 0 ] ) *
_rtB -> B_311_4325_0 ; xnew [ 1 ] = ( _rtP -> P_3204 [ 2 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_gv [ 0 ] + ( _rtP -> P_3204 [ 3 ] ) * _rtDW ->
DiscreteStateSpace_DSTATE_gv [ 1 ] ; xnew [ 1 ] += ( _rtP -> P_3205 [ 1 ] ) *
_rtB -> B_311_4325_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_gv [ 0 ] , xnew , sizeof ( real_T ) * 2 ) ; } } i =
ssIsSampleHit ( S , 1 , 0 ) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_c
= _rtB -> B_311_4682_0 ; } i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) {
_rtDW -> DiscreteTimeIntegrator2_DSTATE_h1 += _rtP -> P_3213 * _rtB ->
B_311_4437_0 ; if ( _rtB -> B_311_4437_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator2_PrevResetState_a = 1 ; } else if ( _rtB ->
B_311_4437_0 < 0.0 ) { _rtDW -> DiscreteTimeIntegrator2_PrevResetState_a = -
1 ; } else if ( _rtB -> B_311_4437_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator2_PrevResetState_a = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator2_PrevResetState_a = 2 ; } _rtDW ->
DiscreteTimeIntegrator1_DSTATE_m += _rtP -> P_3215 * _rtB -> B_311_4441_0 ;
if ( _rtB -> B_311_4441_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator1_PrevResetState_e = 1 ; } else if ( _rtB ->
B_311_4441_0 < 0.0 ) { _rtDW -> DiscreteTimeIntegrator1_PrevResetState_e = -
1 ; } else if ( _rtB -> B_311_4441_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator1_PrevResetState_e = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator1_PrevResetState_e = 2 ; } HoldSine = _rtDW ->
lastSin_fu ; _rtDW -> lastSin_fu = _rtDW -> lastSin_fu * _rtP -> P_3221 +
_rtDW -> lastCos_gi * _rtP -> P_3220 ; _rtDW -> lastCos_gi = _rtDW ->
lastCos_gi * _rtP -> P_3221 - HoldSine * _rtP -> P_3220 ; _rtDW ->
Integ4_SYSTEM_ENABLE_m = 0U ; _rtDW -> Integ4_DSTATE_pu = _rtP -> P_3224 *
_rtB -> B_311_4446_0 + _rtB -> B_311_4447_0 ; ssCallAccelRunBlock ( S , 311 ,
4449 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_mz = _rtB ->
B_311_4446_0 ; _rtDW -> UnitDelay1_DSTATE_k2 = _rtB -> B_311_4456_0 ;
HoldSine = _rtDW -> lastSin_ev ; _rtDW -> lastSin_ev = _rtDW -> lastSin_ev *
_rtP -> P_3242 + _rtDW -> lastCos_ll * _rtP -> P_3241 ; _rtDW -> lastCos_ll =
_rtDW -> lastCos_ll * _rtP -> P_3242 - HoldSine * _rtP -> P_3241 ; _rtDW ->
Integ4_SYSTEM_ENABLE_br3 = 0U ; _rtDW -> Integ4_DSTATE_gp = _rtP -> P_3245 *
_rtB -> B_311_4458_0 + _rtB -> B_311_4459_0 ; ssCallAccelRunBlock ( S , 311 ,
4461 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_bf = _rtB ->
B_311_4458_0 ; _rtDW -> UnitDelay1_DSTATE_ezs = _rtB -> B_311_4468_0 ;
HoldSine = _rtDW -> lastSin_nb ; _rtDW -> lastSin_nb = _rtDW -> lastSin_nb *
_rtP -> P_3265 + _rtDW -> lastCos_h * _rtP -> P_3264 ; _rtDW -> lastCos_h =
_rtDW -> lastCos_h * _rtP -> P_3265 - HoldSine * _rtP -> P_3264 ; _rtDW ->
Integ4_SYSTEM_ENABLE_b3 = 0U ; _rtDW -> Integ4_DSTATE_gj = _rtP -> P_3268 *
_rtB -> B_311_4475_0 + _rtB -> B_311_4476_0 ; ssCallAccelRunBlock ( S , 311 ,
4478 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_mm = _rtB ->
B_311_4475_0 ; _rtDW -> UnitDelay1_DSTATE_oe = _rtB -> B_311_4485_0 ;
HoldSine = _rtDW -> lastSin_e0 ; _rtDW -> lastSin_e0 = _rtDW -> lastSin_e0 *
_rtP -> P_3286 + _rtDW -> lastCos_ow * _rtP -> P_3285 ; _rtDW -> lastCos_ow =
_rtDW -> lastCos_ow * _rtP -> P_3286 - HoldSine * _rtP -> P_3285 ; _rtDW ->
Integ4_SYSTEM_ENABLE_cq = 0U ; _rtDW -> Integ4_DSTATE_i = _rtP -> P_3289 *
_rtB -> B_311_4487_0 + _rtB -> B_311_4488_0 ; ssCallAccelRunBlock ( S , 311 ,
4490 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_jj = _rtB ->
B_311_4487_0 ; _rtDW -> UnitDelay1_DSTATE_nb = _rtB -> B_311_4497_0 ;
HoldSine = _rtDW -> lastSin_h ; _rtDW -> lastSin_h = _rtDW -> lastSin_h *
_rtP -> P_3309 + _rtDW -> lastCos_ls * _rtP -> P_3308 ; _rtDW -> lastCos_ls =
_rtDW -> lastCos_ls * _rtP -> P_3309 - HoldSine * _rtP -> P_3308 ; _rtDW ->
Integ4_SYSTEM_ENABLE_mg = 0U ; _rtDW -> Integ4_DSTATE_go = _rtP -> P_3312 *
_rtB -> B_311_4504_0 + _rtB -> B_311_4505_0 ; ssCallAccelRunBlock ( S , 311 ,
4507 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_lkz = _rtB ->
B_311_4504_0 ; _rtDW -> UnitDelay1_DSTATE_m2 = _rtB -> B_311_4514_0 ;
HoldSine = _rtDW -> lastSin_nj ; _rtDW -> lastSin_nj = _rtDW -> lastSin_nj *
_rtP -> P_3330 + _rtDW -> lastCos_pn * _rtP -> P_3329 ; _rtDW -> lastCos_pn =
_rtDW -> lastCos_pn * _rtP -> P_3330 - HoldSine * _rtP -> P_3329 ; _rtDW ->
Integ4_SYSTEM_ENABLE_lh = 0U ; _rtDW -> Integ4_DSTATE_iz = _rtP -> P_3333 *
_rtB -> B_311_4516_0 + _rtB -> B_311_4517_0 ; ssCallAccelRunBlock ( S , 311 ,
4519 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_gc = _rtB ->
B_311_4516_0 ; _rtDW -> UnitDelay1_DSTATE_bc = _rtB -> B_311_4526_0 ; _rtDW
-> DiscreteTimeIntegrator4_DSTATE_m += _rtP -> P_3350 * _rtB -> B_311_4540_0
; if ( _rtB -> B_311_4540_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator4_PrevResetState_o = 1 ; } else if ( _rtB ->
B_311_4540_0 < 0.0 ) { _rtDW -> DiscreteTimeIntegrator4_PrevResetState_o = -
1 ; } else if ( _rtB -> B_311_4540_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator4_PrevResetState_o = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator4_PrevResetState_o = 2 ; } _rtDW ->
DiscreteTimeIntegrator3_DSTATE_d += _rtP -> P_3352 * _rtB -> B_311_4544_0 ;
if ( _rtB -> B_311_4544_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator3_PrevResetState_g = 1 ; } else if ( _rtB ->
B_311_4544_0 < 0.0 ) { _rtDW -> DiscreteTimeIntegrator3_PrevResetState_g = -
1 ; } else if ( _rtB -> B_311_4544_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator3_PrevResetState_g = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator3_PrevResetState_g = 2 ; } _rtDW ->
DiscreteTimeIntegrator2_DSTATE_hg += _rtP -> P_3355 * _rtB -> B_311_4553_0 ;
if ( _rtB -> B_311_4553_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator2_PrevResetState_n = 1 ; } else if ( _rtB ->
B_311_4553_0 < 0.0 ) { _rtDW -> DiscreteTimeIntegrator2_PrevResetState_n = -
1 ; } else if ( _rtB -> B_311_4553_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator2_PrevResetState_n = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator2_PrevResetState_n = 2 ; } _rtDW ->
DiscreteTimeIntegrator1_DSTATE_dd += _rtP -> P_3357 * _rtB -> B_311_4557_0 ;
if ( _rtB -> B_311_4557_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator1_PrevResetState_je = 1 ; } else if ( _rtB ->
B_311_4557_0 < 0.0 ) { _rtDW -> DiscreteTimeIntegrator1_PrevResetState_je = -
1 ; } else if ( _rtB -> B_311_4557_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator1_PrevResetState_je = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator1_PrevResetState_je = 2 ; } _rtDW ->
DiscreteTimeIntegrator4_DSTATE_a += _rtP -> P_3360 * _rtB -> B_311_4565_0 ;
if ( _rtB -> B_311_4565_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator4_PrevResetState_i = 1 ; } else if ( _rtB ->
B_311_4565_0 < 0.0 ) { _rtDW -> DiscreteTimeIntegrator4_PrevResetState_i = -
1 ; } else if ( _rtB -> B_311_4565_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator4_PrevResetState_i = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator4_PrevResetState_i = 2 ; } _rtDW ->
DiscreteTimeIntegrator3_DSTATE_e += _rtP -> P_3362 * _rtB -> B_311_4569_0 ;
if ( _rtB -> B_311_4569_0 > 0.0 ) { _rtDW ->
DiscreteTimeIntegrator3_PrevResetState_c = 1 ; } else if ( _rtB ->
B_311_4569_0 < 0.0 ) { _rtDW -> DiscreteTimeIntegrator3_PrevResetState_c = -
1 ; } else if ( _rtB -> B_311_4569_0 == 0.0 ) { _rtDW ->
DiscreteTimeIntegrator3_PrevResetState_c = 0 ; } else { _rtDW ->
DiscreteTimeIntegrator3_PrevResetState_c = 2 ; } } _rtDW ->
Integrator_IWORK_d = 0 ; if ( _rtX -> Integrator_CSTATE_op == _rtP -> P_3383
) { switch ( _rtDW -> Integrator_MODE_e ) { case 3 : if ( _rtB ->
B_311_4663_0 < 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
_rtDW -> Integrator_MODE_e = 1 ; } break ; case 1 : if ( _rtB -> B_311_4663_0
>= 0.0 ) { _rtDW -> Integrator_MODE_e = 3 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; default :
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( _rtB -> B_311_4663_0
< 0.0 ) { _rtDW -> Integrator_MODE_e = 1 ; } else { _rtDW ->
Integrator_MODE_e = 3 ; } break ; } } else if ( _rtX -> Integrator_CSTATE_op
== _rtP -> P_3384 ) { switch ( _rtDW -> Integrator_MODE_e ) { case 4 : if (
_rtB -> B_311_4663_0 > 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; _rtDW -> Integrator_MODE_e = 2 ; } break ; case 2 : if ( _rtB ->
B_311_4663_0 <= 0.0 ) { _rtDW -> Integrator_MODE_e = 4 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; default :
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( _rtB -> B_311_4663_0
> 0.0 ) { _rtDW -> Integrator_MODE_e = 2 ; } else { _rtDW ->
Integrator_MODE_e = 4 ; } break ; } } else { _rtDW -> Integrator_MODE_e = 0 ;
} Auto_microgrid_forecasting_020822_AutomaticGainControl_Update ( S , & _rtB
-> AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl , & ( ( X_Auto_microgrid_forecasting_020822_T * )
ssGetContStates ( S ) ) -> AutomaticGainControl ) ; if ( _rtX ->
Integrator_CSTATE_hc == _rtP -> P_3387 ) { switch ( _rtDW ->
Integrator_MODE_jv ) { case 3 : if ( _rtB -> B_311_4659_0 < 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW ->
Integrator_MODE_jv = 1 ; } break ; case 1 : if ( _rtB -> B_311_4659_0 >= 0.0
) { _rtDW -> Integrator_MODE_jv = 3 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; default :
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( _rtB -> B_311_4659_0
< 0.0 ) { _rtDW -> Integrator_MODE_jv = 1 ; } else { _rtDW ->
Integrator_MODE_jv = 3 ; } break ; } } else if ( _rtX -> Integrator_CSTATE_hc
== _rtP -> P_3388 ) { switch ( _rtDW -> Integrator_MODE_jv ) { case 4 : if (
_rtB -> B_311_4659_0 > 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; _rtDW -> Integrator_MODE_jv = 2 ; } break ; case 2 : if ( _rtB ->
B_311_4659_0 <= 0.0 ) { _rtDW -> Integrator_MODE_jv = 4 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; default :
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( _rtB -> B_311_4659_0
> 0.0 ) { _rtDW -> Integrator_MODE_jv = 2 ; } else { _rtDW ->
Integrator_MODE_jv = 4 ; } break ; } } else { _rtDW -> Integrator_MODE_jv = 0
; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK_h . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_h . TUbufferPtrs [ 1 ] ;
real_T * * xBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_h
. TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
VariableTransportDelay_IWORK_o . Head = ( ( _rtDW ->
VariableTransportDelay_IWORK_o . Head < ( _rtDW ->
VariableTransportDelay_IWORK_o . CircularBufSize - 1 ) ) ? ( _rtDW ->
VariableTransportDelay_IWORK_o . Head + 1 ) : 0 ) ; if ( _rtDW ->
VariableTransportDelay_IWORK_o . Head == _rtDW ->
VariableTransportDelay_IWORK_o . Tail ) { if ( !
Auto_microgrid_forecasting_020822_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW
-> VariableTransportDelay_IWORK_o . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK_o . Tail , & _rtDW ->
VariableTransportDelay_IWORK_o . Head , & _rtDW ->
VariableTransportDelay_IWORK_o . Last , simTime - _rtP -> P_3389 , tBuffer ,
uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 1 , & _rtDW ->
VariableTransportDelay_IWORK_o . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_o . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_o . Head ] = _rtB -> B_311_4651_0 ; ( * xBuffer
) [ _rtDW -> VariableTransportDelay_IWORK_o . Head ] = ( (
X_Auto_microgrid_forecasting_020822_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE_p ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i
!= 0 ) { _rtDW -> Memory_PreviousInput_l = _rtB -> B_311_4656_0 ; } if (
_rtDW -> LastMajorTimeA_m == ( rtInf ) ) { _rtDW -> LastMajorTimeA_m =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_h = _rtB -> B_311_4665_0 ; } else
if ( _rtDW -> LastMajorTimeB_l == ( rtInf ) ) { _rtDW -> LastMajorTimeB_l =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYB_b = _rtB -> B_311_4665_0 ; } else
if ( _rtDW -> LastMajorTimeA_m < _rtDW -> LastMajorTimeB_l ) { _rtDW ->
LastMajorTimeA_m = ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_h = _rtB ->
B_311_4665_0 ; } else { _rtDW -> LastMajorTimeB_l = ssGetTaskTime ( S , 0 ) ;
_rtDW -> PrevYB_b = _rtB -> B_311_4665_0 ; }
Auto_microgrid_forecasting_020822_TrueRMS_Update ( S , & _rtB -> TrueRMS , &
_rtDW -> TrueRMS , & _rtP -> TrueRMS ) ;
Auto_microgrid_forecasting_020822_TrueRMS_Update ( S , & _rtB -> TrueRMS_c ,
& _rtDW -> TrueRMS_c , & _rtP -> TrueRMS_c ) ; i = ssIsSampleHit ( S , 1 , 0
) ; if ( i != 0 ) { _rtDW -> Memory_PreviousInput_jb = _rtB -> B_311_4830_0 ;
} i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { HoldSine = _rtDW ->
lastSin_m ; _rtDW -> lastSin_m = _rtDW -> lastSin_m * _rtP -> P_3425 + _rtDW
-> lastCos_pe * _rtP -> P_3424 ; _rtDW -> lastCos_pe = _rtDW -> lastCos_pe *
_rtP -> P_3425 - HoldSine * _rtP -> P_3424 ; _rtDW -> Integ4_SYSTEM_ENABLE_dh
= 0U ; _rtDW -> Integ4_DSTATE_oe = _rtP -> P_3428 * _rtB -> B_311_4699_0 +
_rtB -> B_311_4700_0 ; ssCallAccelRunBlock ( S , 311 , 4702 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_hg = _rtB -> B_311_4699_0 ;
_rtDW -> UnitDelay1_DSTATE_og = _rtB -> B_311_4709_0 ; HoldSine = _rtDW ->
lastSin_d ; _rtDW -> lastSin_d = _rtDW -> lastSin_d * _rtP -> P_3446 + _rtDW
-> lastCos_hr * _rtP -> P_3445 ; _rtDW -> lastCos_hr = _rtDW -> lastCos_hr *
_rtP -> P_3446 - HoldSine * _rtP -> P_3445 ; _rtDW -> Integ4_SYSTEM_ENABLE_dg
= 0U ; _rtDW -> Integ4_DSTATE_ff = _rtP -> P_3449 * _rtB -> B_311_4711_0 +
_rtB -> B_311_4712_0 ; ssCallAccelRunBlock ( S , 311 , 4714 ,
SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_au = _rtB -> B_311_4711_0 ;
_rtDW -> UnitDelay1_DSTATE_j4 = _rtB -> B_311_4721_0 ; HoldSine = _rtDW ->
lastSin_nx ; _rtDW -> lastSin_nx = _rtDW -> lastSin_nx * _rtP -> P_3469 +
_rtDW -> lastCos_b3 * _rtP -> P_3468 ; _rtDW -> lastCos_b3 = _rtDW ->
lastCos_b3 * _rtP -> P_3469 - HoldSine * _rtP -> P_3468 ; _rtDW ->
Integ4_SYSTEM_ENABLE_a40 = 0U ; _rtDW -> Integ4_DSTATE_oh = _rtP -> P_3472 *
_rtB -> B_311_4728_0 + _rtB -> B_311_4729_0 ; ssCallAccelRunBlock ( S , 311 ,
4731 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_jt = _rtB ->
B_311_4728_0 ; _rtDW -> UnitDelay1_DSTATE_iw = _rtB -> B_311_4738_0 ;
HoldSine = _rtDW -> lastSin_n4 ; _rtDW -> lastSin_n4 = _rtDW -> lastSin_n4 *
_rtP -> P_3490 + _rtDW -> lastCos_fu * _rtP -> P_3489 ; _rtDW -> lastCos_fu =
_rtDW -> lastCos_fu * _rtP -> P_3490 - HoldSine * _rtP -> P_3489 ; _rtDW ->
Integ4_SYSTEM_ENABLE_g = 0U ; _rtDW -> Integ4_DSTATE_bmx = _rtP -> P_3493 *
_rtB -> B_311_4740_0 + _rtB -> B_311_4741_0 ; ssCallAccelRunBlock ( S , 311 ,
4743 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_it = _rtB ->
B_311_4740_0 ; _rtDW -> UnitDelay1_DSTATE_ov = _rtB -> B_311_4750_0 ;
HoldSine = _rtDW -> lastSin_ew ; _rtDW -> lastSin_ew = _rtDW -> lastSin_ew *
_rtP -> P_3513 + _rtDW -> lastCos_pv * _rtP -> P_3512 ; _rtDW -> lastCos_pv =
_rtDW -> lastCos_pv * _rtP -> P_3513 - HoldSine * _rtP -> P_3512 ; _rtDW ->
Integ4_SYSTEM_ENABLE_jl = 0U ; _rtDW -> Integ4_DSTATE_lf = _rtP -> P_3516 *
_rtB -> B_311_4757_0 + _rtB -> B_311_4758_0 ; ssCallAccelRunBlock ( S , 311 ,
4760 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_b5 = _rtB ->
B_311_4757_0 ; _rtDW -> UnitDelay1_DSTATE_n4 = _rtB -> B_311_4767_0 ;
HoldSine = _rtDW -> lastSin_cz ; _rtDW -> lastSin_cz = _rtDW -> lastSin_cz *
_rtP -> P_3534 + _rtDW -> lastCos_kq * _rtP -> P_3533 ; _rtDW -> lastCos_kq =
_rtDW -> lastCos_kq * _rtP -> P_3534 - HoldSine * _rtP -> P_3533 ; _rtDW ->
Integ4_SYSTEM_ENABLE_cf = 0U ; _rtDW -> Integ4_DSTATE_o1 = _rtP -> P_3537 *
_rtB -> B_311_4769_0 + _rtB -> B_311_4770_0 ; ssCallAccelRunBlock ( S , 311 ,
4772 , SS_CALL_MDL_UPDATE ) ; _rtDW -> UnitDelay_DSTATE_ib = _rtB ->
B_311_4769_0 ; _rtDW -> UnitDelay1_DSTATE_e0 = _rtB -> B_311_4779_0 ; } _rtDW
-> Integrator_IWORK_h = 0 ; if ( _rtX -> Integrator_CSTATE_ai == _rtP ->
P_3555 ) { switch ( _rtDW -> Integrator_MODE_p ) { case 3 : if ( _rtB ->
B_311_4811_0 < 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
_rtDW -> Integrator_MODE_p = 1 ; } break ; case 1 : if ( _rtB -> B_311_4811_0
>= 0.0 ) { _rtDW -> Integrator_MODE_p = 3 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; default :
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( _rtB -> B_311_4811_0
< 0.0 ) { _rtDW -> Integrator_MODE_p = 1 ; } else { _rtDW ->
Integrator_MODE_p = 3 ; } break ; } } else if ( _rtX -> Integrator_CSTATE_ai
== _rtP -> P_3556 ) { switch ( _rtDW -> Integrator_MODE_p ) { case 4 : if (
_rtB -> B_311_4811_0 > 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; _rtDW -> Integrator_MODE_p = 2 ; } break ; case 2 : if ( _rtB ->
B_311_4811_0 <= 0.0 ) { _rtDW -> Integrator_MODE_p = 4 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; default :
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( _rtB -> B_311_4811_0
> 0.0 ) { _rtDW -> Integrator_MODE_p = 2 ; } else { _rtDW ->
Integrator_MODE_p = 4 ; } break ; } } else { _rtDW -> Integrator_MODE_p = 0 ;
} Auto_microgrid_forecasting_020822_AutomaticGainControl_Update ( S , & _rtB
-> AutomaticGainControl_f , & _rtDW -> AutomaticGainControl_f , & _rtP ->
AutomaticGainControl_f , & ( ( X_Auto_microgrid_forecasting_020822_T * )
ssGetContStates ( S ) ) -> AutomaticGainControl_f ) ; if ( _rtX ->
Integrator_CSTATE_dl == _rtP -> P_3559 ) { switch ( _rtDW ->
Integrator_MODE_e0 ) { case 3 : if ( _rtB -> B_311_4807_0 < 0.0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; _rtDW ->
Integrator_MODE_e0 = 1 ; } break ; case 1 : if ( _rtB -> B_311_4807_0 >= 0.0
) { _rtDW -> Integrator_MODE_e0 = 3 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; default :
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( _rtB -> B_311_4807_0
< 0.0 ) { _rtDW -> Integrator_MODE_e0 = 1 ; } else { _rtDW ->
Integrator_MODE_e0 = 3 ; } break ; } } else if ( _rtX -> Integrator_CSTATE_dl
== _rtP -> P_3560 ) { switch ( _rtDW -> Integrator_MODE_e0 ) { case 4 : if (
_rtB -> B_311_4807_0 > 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; _rtDW -> Integrator_MODE_e0 = 2 ; } break ; case 2 : if ( _rtB ->
B_311_4807_0 <= 0.0 ) { _rtDW -> Integrator_MODE_e0 = 4 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; default :
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( _rtB -> B_311_4807_0
> 0.0 ) { _rtDW -> Integrator_MODE_e0 = 2 ; } else { _rtDW ->
Integrator_MODE_e0 = 4 ; } break ; } } else { _rtDW -> Integrator_MODE_e0 = 0
; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
VariableTransportDelay_PWORK_k . TUbufferPtrs [ 0 ] ; real_T * * tBuffer = (
real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_k . TUbufferPtrs [ 1 ] ;
real_T * * xBuffer = ( real_T * * ) & _rtDW -> VariableTransportDelay_PWORK_k
. TUbufferPtrs [ 2 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
VariableTransportDelay_IWORK_f . Head = ( ( _rtDW ->
VariableTransportDelay_IWORK_f . Head < ( _rtDW ->
VariableTransportDelay_IWORK_f . CircularBufSize - 1 ) ) ? ( _rtDW ->
VariableTransportDelay_IWORK_f . Head + 1 ) : 0 ) ; if ( _rtDW ->
VariableTransportDelay_IWORK_f . Head == _rtDW ->
VariableTransportDelay_IWORK_f . Tail ) { if ( !
Auto_microgrid_forecasting_020822_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW
-> VariableTransportDelay_IWORK_f . CircularBufSize , & _rtDW ->
VariableTransportDelay_IWORK_f . Tail , & _rtDW ->
VariableTransportDelay_IWORK_f . Head , & _rtDW ->
VariableTransportDelay_IWORK_f . Last , simTime - _rtP -> P_3561 , tBuffer ,
uBuffer , xBuffer , ( boolean_T ) 0 , ( boolean_T ) 1 , & _rtDW ->
VariableTransportDelay_IWORK_f . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_f . Head ] = simTime ; ( * uBuffer ) [ _rtDW ->
VariableTransportDelay_IWORK_f . Head ] = _rtB -> B_311_4799_0 ; ( * xBuffer
) [ _rtDW -> VariableTransportDelay_IWORK_f . Head ] = ( (
X_Auto_microgrid_forecasting_020822_T * ) ssGetContStates ( S ) ) ->
VariableTransportDelay_CSTATE_ht ; } i = ssIsSampleHit ( S , 1 , 0 ) ; if ( i
!= 0 ) { _rtDW -> Memory_PreviousInput_a = _rtB -> B_311_4804_0 ; } if (
_rtDW -> LastMajorTimeA_n == ( rtInf ) ) { _rtDW -> LastMajorTimeA_n =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_c = _rtB -> B_311_4813_0 ; } else
if ( _rtDW -> LastMajorTimeB_cz == ( rtInf ) ) { _rtDW -> LastMajorTimeB_cz =
ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYB_e = _rtB -> B_311_4813_0 ; } else
if ( _rtDW -> LastMajorTimeA_n < _rtDW -> LastMajorTimeB_cz ) { _rtDW ->
LastMajorTimeA_n = ssGetTaskTime ( S , 0 ) ; _rtDW -> PrevYA_c = _rtB ->
B_311_4813_0 ; } else { _rtDW -> LastMajorTimeB_cz = ssGetTaskTime ( S , 0 )
; _rtDW -> PrevYB_e = _rtB -> B_311_4813_0 ; } { real_T * * uBuffer = (
real_T * * ) & _rtDW -> ENGINETd_PWORK . TUbufferPtrs [ 0 ] ; real_T * *
tBuffer = ( real_T * * ) & _rtDW -> ENGINETd_PWORK . TUbufferPtrs [ 1 ] ;
real_T simTime = ssGetT ( S ) ; _rtDW -> ENGINETd_IWORK . Head = ( ( _rtDW ->
ENGINETd_IWORK . Head < ( _rtDW -> ENGINETd_IWORK . CircularBufSize - 1 ) ) ?
( _rtDW -> ENGINETd_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW -> ENGINETd_IWORK .
Head == _rtDW -> ENGINETd_IWORK . Tail ) { if ( !
Auto_microgrid_forecasting_020822_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW
-> ENGINETd_IWORK . CircularBufSize , & _rtDW -> ENGINETd_IWORK . Tail , &
_rtDW -> ENGINETd_IWORK . Head , & _rtDW -> ENGINETd_IWORK . Last , simTime -
_rtP -> P_3610 , tBuffer , uBuffer , ( NULL ) , ( boolean_T ) 0 , false , &
_rtDW -> ENGINETd_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"tdelay memory allocation error" ) ; return ; } } ( * tBuffer ) [ _rtDW ->
ENGINETd_IWORK . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> ENGINETd_IWORK .
Head ] = _rtB -> B_311_5553_0 ; } if ( _rtX -> Integrator_CSTATE_hq == _rtP
-> P_3618 ) { switch ( _rtDW -> Integrator_MODE_i ) { case 3 : if ( _rtB ->
B_311_5556_0 < 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
_rtDW -> Integrator_MODE_i = 1 ; } break ; case 1 : if ( _rtB -> B_311_5556_0
>= 0.0 ) { _rtDW -> Integrator_MODE_i = 3 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; default :
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( _rtB -> B_311_5556_0
< 0.0 ) { _rtDW -> Integrator_MODE_i = 1 ; } else { _rtDW ->
Integrator_MODE_i = 3 ; } break ; } } else if ( _rtX -> Integrator_CSTATE_hq
== _rtP -> P_3619 ) { switch ( _rtDW -> Integrator_MODE_i ) { case 4 : if (
_rtB -> B_311_5556_0 > 0.0 ) { ssSetBlockStateForSolverChangedAtMajorStep ( S
) ; _rtDW -> Integrator_MODE_i = 2 ; } break ; case 2 : if ( _rtB ->
B_311_5556_0 <= 0.0 ) { _rtDW -> Integrator_MODE_i = 4 ;
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } break ; default :
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; if ( _rtB -> B_311_5556_0
> 0.0 ) { _rtDW -> Integrator_MODE_i = 2 ; } else { _rtDW ->
Integrator_MODE_i = 4 ; } break ; } } else { _rtDW -> Integrator_MODE_i = 0 ;
} i = ssIsSampleHit ( S , 2 , 0 ) ; if ( i != 0 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_e1 += _rtP -> P_3625 * _rtB -> B_311_5567_0 ;
if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_e1 >= _rtP -> P_3627 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_e1 = _rtP -> P_3627 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_e1 <= _rtP -> P_3628 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_e1 = _rtP -> P_3628 ; } } _rtDW ->
DiscreteTimeIntegrator_DSTATE_bz += _rtP -> P_3629 * _rtB -> B_311_5599_0 ;
if ( _rtDW -> DiscreteTimeIntegrator_DSTATE_bz >= _rtP -> P_3631 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_bz = _rtP -> P_3631 ; } else { if ( _rtDW ->
DiscreteTimeIntegrator_DSTATE_bz <= _rtP -> P_3632 ) { _rtDW ->
DiscreteTimeIntegrator_DSTATE_bz = _rtP -> P_3632 ; } } { real_T xnew [ 1 ] ;
xnew [ 0 ] = _rtP -> P_3640 * _rtDW -> DiscreteStateSpace_DSTATE_iw ; xnew [
0 ] += _rtP -> P_3641 * _rtB -> B_311_5568_0 ; ( void ) memcpy ( & _rtDW ->
DiscreteStateSpace_DSTATE_iw , xnew , sizeof ( real_T ) * 1 ) ; } _rtDW ->
Delay_x_DSTATE_h = _rtB -> B_311_5602_0 ; _rtDW -> Delay_x_DSTATE_p = _rtB ->
B_311_5593_0 ; _rtDW -> Rotorspeeddeviationdw_SYSTEM_ENABLE_m = 0U ; _rtDW ->
Rotorspeeddeviationdw_DSTATE_g = _rtP -> P_3662 * _rtB -> B_311_5615_0 + _rtB
-> B_311_5616_0 ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID6 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) {
B_Auto_microgrid_forecasting_020822_T * _rtB ;
P_Auto_microgrid_forecasting_020822_T * _rtP ;
X_Auto_microgrid_forecasting_020822_T * _rtX ;
XDot_Auto_microgrid_forecasting_020822_T * _rtXdot ;
XDis_Auto_microgrid_forecasting_020822_T * _rtXdis ;
DW_Auto_microgrid_forecasting_020822_T * _rtDW ; _rtDW = ( (
DW_Auto_microgrid_forecasting_020822_T * ) ssGetRootDWork ( S ) ) ; _rtXdis =
( ( XDis_Auto_microgrid_forecasting_020822_T * ) ssGetContStateDisabled ( S )
) ; _rtXdot = ( ( XDot_Auto_microgrid_forecasting_020822_T * ) ssGetdX ( S )
) ; _rtX = ( ( X_Auto_microgrid_forecasting_020822_T * ) ssGetContStates ( S
) ) ; _rtP = ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S
) ) ; _rtB = ( ( B_Auto_microgrid_forecasting_020822_T * ) _ssGetModelBlockIO
( S ) ) ; _rtXdot -> Integrator_CSTATE_g = _rtB -> B_311_87_0 ; _rtXdot ->
Integrator_CSTATE [ 0 ] = _rtB -> B_311_1775_0 [ 0 ] ; _rtXdot ->
Integrator_CSTATE_h [ 0 ] = _rtB -> B_311_1781_0 [ 0 ] ; _rtXdot ->
Integrator_CSTATE_o [ 0 ] = _rtB -> B_311_1783_0 [ 0 ] ; _rtXdot ->
Integrator_CSTATE_i [ 0 ] = _rtB -> B_311_1785_0 [ 0 ] ; _rtXdot ->
Integrator_CSTATE_a [ 0 ] = _rtB -> B_311_1787_0 [ 0 ] ; _rtXdot ->
Integrator_CSTATE_m [ 0 ] = _rtB -> B_311_1789_0 [ 0 ] ; _rtXdot ->
Integrator_CSTATE [ 1 ] = _rtB -> B_311_1775_0 [ 1 ] ; _rtXdot ->
Integrator_CSTATE_h [ 1 ] = _rtB -> B_311_1781_0 [ 1 ] ; _rtXdot ->
Integrator_CSTATE_o [ 1 ] = _rtB -> B_311_1783_0 [ 1 ] ; _rtXdot ->
Integrator_CSTATE_i [ 1 ] = _rtB -> B_311_1785_0 [ 1 ] ; _rtXdot ->
Integrator_CSTATE_a [ 1 ] = _rtB -> B_311_1787_0 [ 1 ] ; _rtXdot ->
Integrator_CSTATE_m [ 1 ] = _rtB -> B_311_1789_0 [ 1 ] ; if ( ( _rtDW ->
Integrator_MODE != 3 ) && ( _rtDW -> Integrator_MODE != 4 ) ) { _rtXdot ->
Integrator_CSTATE_d = _rtB -> B_311_2752_0 ; _rtXdis -> Integrator_CSTATE_d =
false ; } else { _rtXdot -> Integrator_CSTATE_d = 0.0 ; _rtXdis ->
Integrator_CSTATE_d = ( ( _rtDW -> Integrator_MODE == 3 ) || ( _rtDW ->
Integrator_MODE == 4 ) || _rtXdis -> Integrator_CSTATE_d ) ; }
Auto_microgrid_forecasting_020822_AutomaticGainControl_Deriv ( S , & _rtB ->
AutomaticGainControl_c , & _rtDW -> AutomaticGainControl_c , & _rtP ->
AutomaticGainControl_c , & _rtXdot -> AutomaticGainControl_c ) ; if ( ( _rtDW
-> Integrator_MODE_j != 3 ) && ( _rtDW -> Integrator_MODE_j != 4 ) ) {
_rtXdot -> Integrator_CSTATE_c = _rtB -> B_311_2748_0 ; _rtXdis ->
Integrator_CSTATE_c = false ; } else { _rtXdot -> Integrator_CSTATE_c = 0.0 ;
_rtXdis -> Integrator_CSTATE_c = ( ( _rtDW -> Integrator_MODE_j == 3 ) || (
_rtDW -> Integrator_MODE_j == 4 ) || _rtXdis -> Integrator_CSTATE_c ) ; } {
real_T instantDelay ; instantDelay = _rtB -> B_311_2773_0 ; if ( instantDelay
> _rtP -> P_2151 ) { instantDelay = _rtP -> P_2151 ; } if ( instantDelay <
0.0 ) { ( ( XDot_Auto_microgrid_forecasting_020822_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE = 0 ; } else { ( (
XDot_Auto_microgrid_forecasting_020822_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE = 1.0 / instantDelay ; } } _rtXdot ->
integrator_CSTATE = _rtB -> B_311_2778_0 [ 1 ] ; _rtXdot ->
TransferFcn_CSTATE = 0.0 ; _rtXdot -> TransferFcn_CSTATE += _rtP -> P_2158 *
_rtX -> TransferFcn_CSTATE ; _rtXdot -> TransferFcn_CSTATE += _rtB ->
B_311_2749_0 ; _rtXdot -> Integrator_x1_CSTATE = _rtB -> B_311_2764_0 ;
_rtXdot -> Integrator_x2_CSTATE = _rtB -> B_311_2766_0 ; if ( ( _rtDW ->
Integrator_MODE_f != 3 ) && ( _rtDW -> Integrator_MODE_f != 4 ) ) { _rtXdot
-> Integrator_CSTATE_mn = _rtB -> B_311_2799_0 ; _rtXdis ->
Integrator_CSTATE_mn = false ; } else { _rtXdot -> Integrator_CSTATE_mn = 0.0
; _rtXdis -> Integrator_CSTATE_mn = ( ( _rtDW -> Integrator_MODE_f == 3 ) ||
( _rtDW -> Integrator_MODE_f == 4 ) || _rtXdis -> Integrator_CSTATE_mn ) ; }
Auto_microgrid_forecasting_020822_AutomaticGainControl_Deriv ( S , & _rtB ->
AutomaticGainControl_e , & _rtDW -> AutomaticGainControl_e , & _rtP ->
AutomaticGainControl_e , & _rtXdot -> AutomaticGainControl_e ) ; if ( ( _rtDW
-> Integrator_MODE_c != 3 ) && ( _rtDW -> Integrator_MODE_c != 4 ) ) {
_rtXdot -> Integrator_CSTATE_dx = _rtB -> B_311_2795_0 ; _rtXdis ->
Integrator_CSTATE_dx = false ; } else { _rtXdot -> Integrator_CSTATE_dx = 0.0
; _rtXdis -> Integrator_CSTATE_dx = ( ( _rtDW -> Integrator_MODE_c == 3 ) ||
( _rtDW -> Integrator_MODE_c == 4 ) || _rtXdis -> Integrator_CSTATE_dx ) ; }
{ real_T instantDelay ; instantDelay = _rtB -> B_311_2820_0 ; if (
instantDelay > _rtP -> P_2188 ) { instantDelay = _rtP -> P_2188 ; } if (
instantDelay < 0.0 ) { ( ( XDot_Auto_microgrid_forecasting_020822_T * )
ssGetdX ( S ) ) -> VariableTransportDelay_CSTATE_h = 0 ; } else { ( (
XDot_Auto_microgrid_forecasting_020822_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE_h = 1.0 / instantDelay ; } } _rtXdot ->
integrator_CSTATE_j = _rtB -> B_311_2825_0 [ 1 ] ; _rtXdot ->
TransferFcn_CSTATE_l = 0.0 ; _rtXdot -> TransferFcn_CSTATE_l += _rtP ->
P_2195 * _rtX -> TransferFcn_CSTATE_l ; _rtXdot -> TransferFcn_CSTATE_l +=
_rtB -> B_311_2796_0 ; _rtXdot -> Integrator_x1_CSTATE_n = _rtB ->
B_311_2811_0 ; _rtXdot -> Integrator_x2_CSTATE_p = _rtB -> B_311_2813_0 ; if
( ( _rtDW -> Integrator_MODE_e != 3 ) && ( _rtDW -> Integrator_MODE_e != 4 )
) { _rtXdot -> Integrator_CSTATE_op = _rtB -> B_311_4663_0 ; _rtXdis ->
Integrator_CSTATE_op = false ; } else { _rtXdot -> Integrator_CSTATE_op = 0.0
; _rtXdis -> Integrator_CSTATE_op = ( ( _rtDW -> Integrator_MODE_e == 3 ) ||
( _rtDW -> Integrator_MODE_e == 4 ) || _rtXdis -> Integrator_CSTATE_op ) ; }
Auto_microgrid_forecasting_020822_AutomaticGainControl_Deriv ( S , & _rtB ->
AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl , & _rtXdot -> AutomaticGainControl ) ; if ( ( _rtDW ->
Integrator_MODE_jv != 3 ) && ( _rtDW -> Integrator_MODE_jv != 4 ) ) { _rtXdot
-> Integrator_CSTATE_hc = _rtB -> B_311_4659_0 ; _rtXdis ->
Integrator_CSTATE_hc = false ; } else { _rtXdot -> Integrator_CSTATE_hc = 0.0
; _rtXdis -> Integrator_CSTATE_hc = ( ( _rtDW -> Integrator_MODE_jv == 3 ) ||
( _rtDW -> Integrator_MODE_jv == 4 ) || _rtXdis -> Integrator_CSTATE_hc ) ; }
{ real_T instantDelay ; instantDelay = _rtB -> B_311_4684_0 ; if (
instantDelay > _rtP -> P_3389 ) { instantDelay = _rtP -> P_3389 ; } if (
instantDelay < 0.0 ) { ( ( XDot_Auto_microgrid_forecasting_020822_T * )
ssGetdX ( S ) ) -> VariableTransportDelay_CSTATE_p = 0 ; } else { ( (
XDot_Auto_microgrid_forecasting_020822_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE_p = 1.0 / instantDelay ; } } _rtXdot ->
integrator_CSTATE_o = _rtB -> B_311_4689_0 [ 1 ] ; _rtXdot ->
TransferFcn_CSTATE_le = 0.0 ; _rtXdot -> TransferFcn_CSTATE_le += _rtP ->
P_3396 * _rtX -> TransferFcn_CSTATE_le ; _rtXdot -> TransferFcn_CSTATE_le +=
_rtB -> B_311_4660_0 ; _rtXdot -> Integrator_x1_CSTATE_a = _rtB ->
B_311_4675_0 ; _rtXdot -> Integrator_x2_CSTATE_j = _rtB -> B_311_4677_0 ;
_rtXdot -> integrator_CSTATE_e [ 0 ] = _rtB -> B_311_609_0 [ 0 ] ; _rtXdot ->
integrator_CSTATE_e [ 1 ] = _rtB -> B_311_609_0 [ 1 ] ; _rtXdot ->
integrator_CSTATE_e [ 2 ] = _rtB -> B_311_609_0 [ 2 ] ;
Auto_microgrid_forecasting_020822_RMS_Deriv ( S , & _rtB -> RMS , & _rtDW ->
RMS , & _rtXdot -> RMS ) ; Auto_microgrid_forecasting_020822_TrueRMS_Deriv (
S , & _rtB -> TrueRMS , & _rtDW -> TrueRMS , & _rtXdot -> TrueRMS ) ;
Auto_microgrid_forecasting_020822_RMS_Deriv ( S , & _rtB -> RMS_j , & _rtDW
-> RMS_j , & _rtXdot -> RMS_j ) ;
Auto_microgrid_forecasting_020822_TrueRMS_Deriv ( S , & _rtB -> TrueRMS_c , &
_rtDW -> TrueRMS_c , & _rtXdot -> TrueRMS_c ) ; if ( ( _rtDW ->
Integrator_MODE_p != 3 ) && ( _rtDW -> Integrator_MODE_p != 4 ) ) { _rtXdot
-> Integrator_CSTATE_ai = _rtB -> B_311_4811_0 ; _rtXdis ->
Integrator_CSTATE_ai = false ; } else { _rtXdot -> Integrator_CSTATE_ai = 0.0
; _rtXdis -> Integrator_CSTATE_ai = ( ( _rtDW -> Integrator_MODE_p == 3 ) ||
( _rtDW -> Integrator_MODE_p == 4 ) || _rtXdis -> Integrator_CSTATE_ai ) ; }
Auto_microgrid_forecasting_020822_AutomaticGainControl_Deriv ( S , & _rtB ->
AutomaticGainControl_f , & _rtDW -> AutomaticGainControl_f , & _rtP ->
AutomaticGainControl_f , & _rtXdot -> AutomaticGainControl_f ) ; if ( ( _rtDW
-> Integrator_MODE_e0 != 3 ) && ( _rtDW -> Integrator_MODE_e0 != 4 ) ) {
_rtXdot -> Integrator_CSTATE_dl = _rtB -> B_311_4807_0 ; _rtXdis ->
Integrator_CSTATE_dl = false ; } else { _rtXdot -> Integrator_CSTATE_dl = 0.0
; _rtXdis -> Integrator_CSTATE_dl = ( ( _rtDW -> Integrator_MODE_e0 == 3 ) ||
( _rtDW -> Integrator_MODE_e0 == 4 ) || _rtXdis -> Integrator_CSTATE_dl ) ; }
{ real_T instantDelay ; instantDelay = _rtB -> B_311_4832_0 ; if (
instantDelay > _rtP -> P_3561 ) { instantDelay = _rtP -> P_3561 ; } if (
instantDelay < 0.0 ) { ( ( XDot_Auto_microgrid_forecasting_020822_T * )
ssGetdX ( S ) ) -> VariableTransportDelay_CSTATE_ht = 0 ; } else { ( (
XDot_Auto_microgrid_forecasting_020822_T * ) ssGetdX ( S ) ) ->
VariableTransportDelay_CSTATE_ht = 1.0 / instantDelay ; } } _rtXdot ->
integrator_CSTATE_d = _rtB -> B_311_4837_0 [ 1 ] ; _rtXdot ->
TransferFcn_CSTATE_a = 0.0 ; _rtXdot -> TransferFcn_CSTATE_a += _rtP ->
P_3568 * _rtX -> TransferFcn_CSTATE_a ; _rtXdot -> TransferFcn_CSTATE_a +=
_rtB -> B_311_4808_0 ; _rtXdot -> Integrator_x1_CSTATE_g = _rtB ->
B_311_4823_0 ; _rtXdot -> Integrator_x2_CSTATE_jy = _rtB -> B_311_4825_0 ;
_rtXdot -> CONTROLSYSTEM_CSTATE [ 0 ] = 0.0 ; _rtXdot -> CONTROLSYSTEM_CSTATE
[ 0 ] += _rtP -> P_3614 [ 0 ] * _rtX -> CONTROLSYSTEM_CSTATE [ 0 ] ; _rtXdot
-> CONTROLSYSTEM_CSTATE [ 1 ] = 0.0 ; _rtXdot -> CONTROLSYSTEM_CSTATE [ 0 ]
+= _rtP -> P_3614 [ 1 ] * _rtX -> CONTROLSYSTEM_CSTATE [ 1 ] ; _rtXdot ->
CONTROLSYSTEM_CSTATE [ 1 ] += _rtX -> CONTROLSYSTEM_CSTATE [ 0 ] ; _rtXdot ->
CONTROLSYSTEM_CSTATE [ 0 ] += _rtB -> B_311_5554_0 ; if ( ( _rtDW ->
Integrator_MODE_i != 3 ) && ( _rtDW -> Integrator_MODE_i != 4 ) ) { _rtXdot
-> Integrator_CSTATE_hq = _rtB -> B_311_5556_0 ; _rtXdis ->
Integrator_CSTATE_hq = false ; } else { _rtXdot -> Integrator_CSTATE_hq = 0.0
; _rtXdis -> Integrator_CSTATE_hq = ( ( _rtDW -> Integrator_MODE_i == 3 ) ||
( _rtDW -> Integrator_MODE_i == 4 ) || _rtXdis -> Integrator_CSTATE_hq ) ; }
_rtXdot -> TF1_CSTATE = 0.0 ; _rtXdot -> TF1_CSTATE += _rtP -> P_3620 * _rtX
-> TF1_CSTATE ; _rtXdot -> TF1_CSTATE += _rtB -> B_311_5552_0 ; _rtXdot ->
TF2_CSTATE = 0.0 ; _rtXdot -> TF2_CSTATE += _rtP -> P_3623 * _rtX ->
TF2_CSTATE ; _rtXdot -> TF2_CSTATE += _rtB -> B_311_5555_0 ; }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) {
B_Auto_microgrid_forecasting_020822_T * _rtB ;
P_Auto_microgrid_forecasting_020822_T * _rtP ;
X_Auto_microgrid_forecasting_020822_T * _rtX ;
ZCV_Auto_microgrid_forecasting_020822_T * _rtZCSV ;
DW_Auto_microgrid_forecasting_020822_T * _rtDW ; _rtDW = ( (
DW_Auto_microgrid_forecasting_020822_T * ) ssGetRootDWork ( S ) ) ; _rtZCSV =
( ( ZCV_Auto_microgrid_forecasting_020822_T * ) ssGetSolverZcSignalVector ( S
) ) ; _rtX = ( ( X_Auto_microgrid_forecasting_020822_T * ) ssGetContStates (
S ) ) ; _rtP = ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp (
S ) ) ; _rtB = ( ( B_Auto_microgrid_forecasting_020822_T * )
_ssGetModelBlockIO ( S ) ) ; _rtZCSV -> Saturation_UprLim_ZC = _rtB ->
B_311_1248_0 - _rtP -> P_1455 ; _rtZCSV -> Saturation_LwrLim_ZC = _rtB ->
B_311_1248_0 - _rtP -> P_1456 ; _rtZCSV -> Saturation1_UprLim_ZC = _rtB ->
B_311_1250_0 - _rtP -> P_1457 ; _rtZCSV -> Saturation1_LwrLim_ZC = _rtB ->
B_311_1250_0 - _rtP -> P_1458 ; _rtZCSV -> Step2_StepTime_ZC = ssGetT ( S ) -
_rtP -> P_1541 ; _rtZCSV -> RelationalOperator_RelopInput_ZC = _rtX ->
Integrator_CSTATE_d - _rtB -> B_311_223_0 ; if ( ( _rtDW -> Integrator_MODE
== 1 ) && ( _rtX -> Integrator_CSTATE_d >= _rtP -> P_2145 ) ) { _rtZCSV ->
Integrator_IntgUpLimit_ZC = 0.0 ; } else { _rtZCSV ->
Integrator_IntgUpLimit_ZC = _rtX -> Integrator_CSTATE_d - _rtP -> P_2145 ; }
if ( ( _rtDW -> Integrator_MODE == 2 ) && ( _rtX -> Integrator_CSTATE_d <=
_rtP -> P_2146 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC = 0.0 ; } else {
_rtZCSV -> Integrator_IntgLoLimit_ZC = _rtX -> Integrator_CSTATE_d - _rtP ->
P_2146 ; } if ( ( _rtDW -> Integrator_MODE == 3 ) || ( _rtDW ->
Integrator_MODE == 4 ) ) { _rtZCSV -> Integrator_LeaveSaturate_ZC = _rtB ->
B_311_2752_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC = 0.0 ; }
_rtZCSV -> HitCrossing_HitNoOutput_ZC = _rtB -> B_311_2735_0 - _rtP -> P_2147
; Auto_microgrid_forecasting_020822_AutomaticGainControl_ZC ( S , & _rtB ->
AutomaticGainControl_c , & _rtDW -> AutomaticGainControl_c , & _rtP ->
AutomaticGainControl_c , & _rtZCSV -> AutomaticGainControl_c ) ; if ( ( _rtDW
-> Integrator_MODE_j == 1 ) && ( _rtX -> Integrator_CSTATE_c >= _rtP ->
P_2149 ) ) { _rtZCSV -> Integrator_IntgUpLimit_ZC_k = 0.0 ; } else { _rtZCSV
-> Integrator_IntgUpLimit_ZC_k = _rtX -> Integrator_CSTATE_c - _rtP -> P_2149
; } if ( ( _rtDW -> Integrator_MODE_j == 2 ) && ( _rtX -> Integrator_CSTATE_c
<= _rtP -> P_2150 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC_m = 0.0 ; } else
{ _rtZCSV -> Integrator_IntgLoLimit_ZC_m = _rtX -> Integrator_CSTATE_c - _rtP
-> P_2150 ; } if ( ( _rtDW -> Integrator_MODE_j == 3 ) || ( _rtDW ->
Integrator_MODE_j == 4 ) ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_j = _rtB
-> B_311_2748_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_j = 0.0 ; }
_rtZCSV -> RelationalOperator_RelopInput_ZC_h = _rtB -> B_311_2741_0 - _rtB
-> B_311_224_0 ; _rtZCSV -> Saturation2_UprLim_ZC = _rtB -> B_311_2751_0 -
_rtP -> P_2161 ; _rtZCSV -> Saturation2_LwrLim_ZC = _rtB -> B_311_2751_0 -
_rtP -> P_2162 ; _rtZCSV -> RelationalOperator_RelopInput_ZC_a = _rtX ->
Integrator_CSTATE_mn - _rtB -> B_311_232_0_m ; if ( ( _rtDW ->
Integrator_MODE_f == 1 ) && ( _rtX -> Integrator_CSTATE_mn >= _rtP -> P_2182
) ) { _rtZCSV -> Integrator_IntgUpLimit_ZC_b = 0.0 ; } else { _rtZCSV ->
Integrator_IntgUpLimit_ZC_b = _rtX -> Integrator_CSTATE_mn - _rtP -> P_2182 ;
} if ( ( _rtDW -> Integrator_MODE_f == 2 ) && ( _rtX -> Integrator_CSTATE_mn
<= _rtP -> P_2183 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC_j = 0.0 ; } else
{ _rtZCSV -> Integrator_IntgLoLimit_ZC_j = _rtX -> Integrator_CSTATE_mn -
_rtP -> P_2183 ; } if ( ( _rtDW -> Integrator_MODE_f == 3 ) || ( _rtDW ->
Integrator_MODE_f == 4 ) ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_c = _rtB
-> B_311_2799_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_c = 0.0 ; }
_rtZCSV -> HitCrossing_HitNoOutput_ZC_o = _rtB -> B_311_2782_0 - _rtP ->
P_2184 ; Auto_microgrid_forecasting_020822_AutomaticGainControl_ZC ( S , &
_rtB -> AutomaticGainControl_e , & _rtDW -> AutomaticGainControl_e , & _rtP
-> AutomaticGainControl_e , & _rtZCSV -> AutomaticGainControl_e ) ; if ( (
_rtDW -> Integrator_MODE_c == 1 ) && ( _rtX -> Integrator_CSTATE_dx >= _rtP
-> P_2186 ) ) { _rtZCSV -> Integrator_IntgUpLimit_ZC_n = 0.0 ; } else {
_rtZCSV -> Integrator_IntgUpLimit_ZC_n = _rtX -> Integrator_CSTATE_dx - _rtP
-> P_2186 ; } if ( ( _rtDW -> Integrator_MODE_c == 2 ) && ( _rtX ->
Integrator_CSTATE_dx <= _rtP -> P_2187 ) ) { _rtZCSV ->
Integrator_IntgLoLimit_ZC_mi = 0.0 ; } else { _rtZCSV ->
Integrator_IntgLoLimit_ZC_mi = _rtX -> Integrator_CSTATE_dx - _rtP -> P_2187
; } if ( ( _rtDW -> Integrator_MODE_c == 3 ) || ( _rtDW -> Integrator_MODE_c
== 4 ) ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_h = _rtB -> B_311_2795_0 ;
} else { _rtZCSV -> Integrator_LeaveSaturate_ZC_h = 0.0 ; } _rtZCSV ->
RelationalOperator_RelopInput_ZC_c = _rtB -> B_311_2788_0 - _rtB ->
B_311_233_0 ; _rtZCSV -> Saturation2_UprLim_ZC_e = _rtB -> B_311_2798_0 -
_rtP -> P_2198 ; _rtZCSV -> Saturation2_LwrLim_ZC_o = _rtB -> B_311_2798_0 -
_rtP -> P_2199 ; _rtZCSV -> upitch_max_UprLim_ZC = _rtB -> B_311_3813_0 -
_rtP -> P_2949 ; _rtZCSV -> upitch_max_LwrLim_ZC = _rtB -> B_311_3813_0 -
_rtP -> P_2950 ; _rtZCSV -> Avoiddivbyzero_UprLim_ZC = _rtB -> B_311_1246_0 -
_rtP -> P_2964 ; _rtZCSV -> Avoiddivbyzero_LwrLim_ZC = _rtB -> B_311_1246_0 -
_rtP -> P_2965 ; _rtZCSV -> Step1_StepTime_ZC = ssGetT ( S ) - _rtP -> P_3044
; _rtZCSV -> Step2_StepTime_ZC_p = ssGetT ( S ) - _rtP -> P_3047 ; _rtZCSV ->
Step1_StepTime_ZC_g = ssGetT ( S ) - _rtP -> P_3066 ; _rtZCSV ->
Compare_RelopInput_ZC = _rtB -> B_311_4435_0 - _rtB -> B_311_514_0 ; _rtZCSV
-> Compare_RelopInput_ZC_h = _rtB -> B_311_4435_0 - _rtB -> B_311_515_0 ;
_rtZCSV -> Compare_RelopInput_ZC_i = _rtB -> B_311_4551_0 - _rtB ->
B_311_524_0 ; _rtZCSV -> Compare_RelopInput_ZC_hj = _rtB -> B_311_4551_0 -
_rtB -> B_311_525_0 ; _rtZCSV -> RelationalOperator_RelopInput_ZC_p = _rtX ->
Integrator_CSTATE_op - _rtB -> B_311_498_0 ; if ( ( _rtDW ->
Integrator_MODE_e == 1 ) && ( _rtX -> Integrator_CSTATE_op >= _rtP -> P_3383
) ) { _rtZCSV -> Integrator_IntgUpLimit_ZC_d = 0.0 ; } else { _rtZCSV ->
Integrator_IntgUpLimit_ZC_d = _rtX -> Integrator_CSTATE_op - _rtP -> P_3383 ;
} if ( ( _rtDW -> Integrator_MODE_e == 2 ) && ( _rtX -> Integrator_CSTATE_op
<= _rtP -> P_3384 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC_p = 0.0 ; } else
{ _rtZCSV -> Integrator_IntgLoLimit_ZC_p = _rtX -> Integrator_CSTATE_op -
_rtP -> P_3384 ; } if ( ( _rtDW -> Integrator_MODE_e == 3 ) || ( _rtDW ->
Integrator_MODE_e == 4 ) ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_a = _rtB
-> B_311_4663_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_a = 0.0 ; }
_rtZCSV -> HitCrossing_HitNoOutput_ZC_b = _rtB -> B_311_4646_0 - _rtP ->
P_3385 ; Auto_microgrid_forecasting_020822_AutomaticGainControl_ZC ( S , &
_rtB -> AutomaticGainControl , & _rtDW -> AutomaticGainControl , & _rtP ->
AutomaticGainControl , & _rtZCSV -> AutomaticGainControl ) ; if ( ( _rtDW ->
Integrator_MODE_jv == 1 ) && ( _rtX -> Integrator_CSTATE_hc >= _rtP -> P_3387
) ) { _rtZCSV -> Integrator_IntgUpLimit_ZC_e = 0.0 ; } else { _rtZCSV ->
Integrator_IntgUpLimit_ZC_e = _rtX -> Integrator_CSTATE_hc - _rtP -> P_3387 ;
} if ( ( _rtDW -> Integrator_MODE_jv == 2 ) && ( _rtX -> Integrator_CSTATE_hc
<= _rtP -> P_3388 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC_b = 0.0 ; } else
{ _rtZCSV -> Integrator_IntgLoLimit_ZC_b = _rtX -> Integrator_CSTATE_hc -
_rtP -> P_3388 ; } if ( ( _rtDW -> Integrator_MODE_jv == 3 ) || ( _rtDW ->
Integrator_MODE_jv == 4 ) ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_m = _rtB
-> B_311_4659_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_m = 0.0 ; }
_rtZCSV -> RelationalOperator_RelopInput_ZC_n = _rtB -> B_311_4652_0 - _rtB
-> B_311_499_0 ; _rtZCSV -> Saturation2_UprLim_ZC_n = _rtB -> B_311_4662_0 -
_rtP -> P_3399 ; _rtZCSV -> Saturation2_LwrLim_ZC_g = _rtB -> B_311_4662_0 -
_rtP -> P_3400 ; Auto_microgrid_forecasting_020822_TrueRMS_ZC ( S , & _rtB ->
TrueRMS , & _rtDW -> TrueRMS , & _rtP -> TrueRMS , & _rtZCSV -> TrueRMS ) ;
Auto_microgrid_forecasting_020822_TrueRMS_ZC ( S , & _rtB -> TrueRMS_c , &
_rtDW -> TrueRMS_c , & _rtP -> TrueRMS_c , & _rtZCSV -> TrueRMS_c ) ; _rtZCSV
-> RelationalOperator_RelopInput_ZC_g = _rtX -> Integrator_CSTATE_ai - _rtB
-> B_311_536_0 ; if ( ( _rtDW -> Integrator_MODE_p == 1 ) && ( _rtX ->
Integrator_CSTATE_ai >= _rtP -> P_3555 ) ) { _rtZCSV ->
Integrator_IntgUpLimit_ZC_o = 0.0 ; } else { _rtZCSV ->
Integrator_IntgUpLimit_ZC_o = _rtX -> Integrator_CSTATE_ai - _rtP -> P_3555 ;
} if ( ( _rtDW -> Integrator_MODE_p == 2 ) && ( _rtX -> Integrator_CSTATE_ai
<= _rtP -> P_3556 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC_ba = 0.0 ; }
else { _rtZCSV -> Integrator_IntgLoLimit_ZC_ba = _rtX -> Integrator_CSTATE_ai
- _rtP -> P_3556 ; } if ( ( _rtDW -> Integrator_MODE_p == 3 ) || ( _rtDW ->
Integrator_MODE_p == 4 ) ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_i = _rtB
-> B_311_4811_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_i = 0.0 ; }
_rtZCSV -> HitCrossing_HitNoOutput_ZC_h = _rtB -> B_311_4794_0 - _rtP ->
P_3557 ; Auto_microgrid_forecasting_020822_AutomaticGainControl_ZC ( S , &
_rtB -> AutomaticGainControl_f , & _rtDW -> AutomaticGainControl_f , & _rtP
-> AutomaticGainControl_f , & _rtZCSV -> AutomaticGainControl_f ) ; if ( (
_rtDW -> Integrator_MODE_e0 == 1 ) && ( _rtX -> Integrator_CSTATE_dl >= _rtP
-> P_3559 ) ) { _rtZCSV -> Integrator_IntgUpLimit_ZC_m = 0.0 ; } else {
_rtZCSV -> Integrator_IntgUpLimit_ZC_m = _rtX -> Integrator_CSTATE_dl - _rtP
-> P_3559 ; } if ( ( _rtDW -> Integrator_MODE_e0 == 2 ) && ( _rtX ->
Integrator_CSTATE_dl <= _rtP -> P_3560 ) ) { _rtZCSV ->
Integrator_IntgLoLimit_ZC_jf = 0.0 ; } else { _rtZCSV ->
Integrator_IntgLoLimit_ZC_jf = _rtX -> Integrator_CSTATE_dl - _rtP -> P_3560
; } if ( ( _rtDW -> Integrator_MODE_e0 == 3 ) || ( _rtDW ->
Integrator_MODE_e0 == 4 ) ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_h5 =
_rtB -> B_311_4807_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_h5 =
0.0 ; } _rtZCSV -> RelationalOperator_RelopInput_ZC_cn = _rtB -> B_311_4800_0
- _rtB -> B_311_537_0_g ; _rtZCSV -> Saturation2_UprLim_ZC_i = _rtB ->
B_311_4810_0 - _rtP -> P_3571 ; _rtZCSV -> Saturation2_LwrLim_ZC_j = _rtB ->
B_311_4810_0 - _rtP -> P_3572 ; if ( ( _rtDW -> Integrator_MODE_i == 1 ) && (
_rtX -> Integrator_CSTATE_hq >= _rtP -> P_3618 ) ) { _rtZCSV ->
Integrator_IntgUpLimit_ZC_f = 0.0 ; } else { _rtZCSV ->
Integrator_IntgUpLimit_ZC_f = _rtX -> Integrator_CSTATE_hq - _rtP -> P_3618 ;
} if ( ( _rtDW -> Integrator_MODE_i == 2 ) && ( _rtX -> Integrator_CSTATE_hq
<= _rtP -> P_3619 ) ) { _rtZCSV -> Integrator_IntgLoLimit_ZC_a = 0.0 ; } else
{ _rtZCSV -> Integrator_IntgLoLimit_ZC_a = _rtX -> Integrator_CSTATE_hq -
_rtP -> P_3619 ; } if ( ( _rtDW -> Integrator_MODE_i == 3 ) || ( _rtDW ->
Integrator_MODE_i == 4 ) ) { _rtZCSV -> Integrator_LeaveSaturate_ZC_f = _rtB
-> B_311_5556_0 ; } else { _rtZCSV -> Integrator_LeaveSaturate_ZC_f = 0.0 ; }
} static void mdlInitializeSizes ( SimStruct * S ) { ssSetChecksumVal ( S , 0
, 1625030792U ) ; ssSetChecksumVal ( S , 1 , 3328423878U ) ; ssSetChecksumVal
( S , 2 , 381066423U ) ; ssSetChecksumVal ( S , 3 , 2328195899U ) ; { mxArray
* slVerStructMat = NULL ; mxArray * slStrMat = mxCreateString ( "simulink" )
; char slVerChar [ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat ,
1 , & slStrMat , "ver" ) ; if ( status == 0 ) { mxArray * slVerMat =
mxGetField ( slVerStructMat , 0 , "Version" ) ; if ( slVerMat == NULL ) {
status = 1 ; } else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; }
} mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "10.1" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != sizeof ( DW_Auto_microgrid_forecasting_020822_T ) ) { ssSetErrorStatus
( S , "Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file." ) ; } if ( ssGetSizeofGlobalBlockIO ( S
) != sizeof ( B_Auto_microgrid_forecasting_020822_T ) ) { ssSetErrorStatus (
S , "Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file." ) ; } { int ssSizeofParams ;
ssGetSizeofParams ( S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof (
P_Auto_microgrid_forecasting_020822_T ) ) { static char msg [ 256 ] ; sprintf
( msg , "Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file." ) ; } } _ssSetModelRtp ( S , ( real_T *
) & Auto_microgrid_forecasting_020822_rtDefaultP ) ; _ssSetConstBlockIO ( S ,
& Auto_microgrid_forecasting_020822_rtInvariant ) ; rt_InitInfAndNaN ( sizeof
( real_T ) ) ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp (
S ) ) -> P_68 = rtMinusInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_266 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_267 =
rtMinusInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S
) ) -> P_268 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_269 = rtMinusInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_281 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_282 = rtMinusInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_283 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_284 =
rtMinusInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S
) ) -> P_288 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_289 = rtMinusInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_290 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_291 = rtMinusInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_366 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_367 =
rtMinusInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S
) ) -> P_368 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_369 = rtMinusInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_414 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_484 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_532 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_603 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_618 = rtMinusInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_845 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_846 =
rtMinusInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S
) ) -> P_847 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_848 = rtMinusInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_2145 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_2146 = rtMinusInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_2149 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_2161 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_2182 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_2183 = rtMinusInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_2186 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_2198 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_2623 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_2679 =
rtMinusInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S
) ) -> P_2688 = rtMinusInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_2893 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_2896 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_2916 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_2964 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_2966 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_3003 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_3006 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_3383 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_3384 = rtMinusInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_3387 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_3399 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_3555 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_3556 = rtMinusInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_3559 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_3571 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_3627 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_3708 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_3712 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_3719 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_3723 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_3768 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_3772 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_3779 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_3783 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_4053 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> P_4057 =
rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) )
-> P_4064 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * )
ssGetModelRtp ( S ) ) -> P_4068 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) -> TrueRMS_c
. P_7 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp (
S ) ) -> TrueRMS . P_7 = rtInf ; ( ( P_Auto_microgrid_forecasting_020822_T *
) ssGetModelRtp ( S ) ) -> AutomaticGainControl_f . P_9 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) ->
AutomaticGainControl_e . P_9 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) ->
AutomaticGainControl_c . P_9 = rtInf ; ( (
P_Auto_microgrid_forecasting_020822_T * ) ssGetModelRtp ( S ) ) ->
AutomaticGainControl . P_9 = rtInf ; } static void mdlInitializeSampleTimes (
SimStruct * S ) { { SimStruct * childS ; SysOutputFcn * callSysFcns ; childS
= ssGetSFunction ( S , 0 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 1 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 2 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 3 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 4 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 5 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; childS =
ssGetSFunction ( S , 8 ) ; callSysFcns = ssGetCallSystemOutputFcnList (
childS ) ; callSysFcns [ 3 + 0 ] = ( SysOutputFcn ) ( NULL ) ; }
slAccRegPrmChangeFcn ( S , mdlOutputsTID6 ) ; } static void mdlTerminate (
SimStruct * S ) { }
#include "simulink.c"
