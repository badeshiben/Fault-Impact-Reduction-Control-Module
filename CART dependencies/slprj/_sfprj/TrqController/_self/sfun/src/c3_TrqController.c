/* Include files */

#include "blascompat32.h"
#include "TrqController_sfun.h"
#include "c3_TrqController.h"
#include "mwmathutil.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance.chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance.instanceNumber)
#include "TrqController_sfun_debug_macros.h"

/* Type Definitions */

/* Named Constants */
#define c3_IN_NO_ACTIVE_CHILD          (0)

/* Variable Declarations */

/* Variable Definitions */
static SFc3_TrqControllerInstanceStruct chartInstance;

/* Function Declarations */
static void initialize_c3_TrqController(void);
static void initialize_params_c3_TrqController(void);
static void enable_c3_TrqController(void);
static void disable_c3_TrqController(void);
static void c3_update_debugger_state_c3_TrqController(void);
static const mxArray *get_sim_state_c3_TrqController(void);
static void set_sim_state_c3_TrqController(const mxArray *c3_st);
static void finalize_c3_TrqController(void);
static void sf_c3_TrqController(void);
static void c3_c3_TrqController(void);
static void init_script_number_translation(uint32_T c3_machineNumber, uint32_T
  c3_chartNumber);
static real_T c3_power(real_T c3_a, real_T c3_b);
static void c3_eml_error(void);
static const mxArray *c3_sf_marshall(void *c3_chartInstance, void *c3_u);
static void c3_info_helper(c3_ResolvedFunctionInfo c3_info[33]);
static const mxArray *c3_b_sf_marshall(void *c3_chartInstance, void *c3_u);
static void init_dsm_address_info(void);

/* Function Definitions */
static void initialize_c3_TrqController(void)
{
  _sfTime_ = (real_T)ssGetT(chartInstance.S);
  chartInstance.c3_is_active_c3_TrqController = 0U;
}

static void initialize_params_c3_TrqController(void)
{
}

static void enable_c3_TrqController(void)
{
  _sfTime_ = (real_T)ssGetT(chartInstance.S);
}

static void disable_c3_TrqController(void)
{
  _sfTime_ = (real_T)ssGetT(chartInstance.S);
}

static void c3_update_debugger_state_c3_TrqController(void)
{
}

static const mxArray *get_sim_state_c3_TrqController(void)
{
  const mxArray *c3_st = NULL;
  const mxArray *c3_y = NULL;
  real_T c3_u;
  const mxArray *c3_b_y = NULL;
  uint8_T c3_b_u;
  const mxArray *c3_c_y = NULL;
  real_T *c3_genTorque;
  c3_genTorque = (real_T *)ssGetOutputPortSignal(chartInstance.S, 1);
  c3_st = NULL;
  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_createcellarray(2));
  c3_u = *c3_genTorque;
  c3_b_y = NULL;
  sf_mex_assign(&c3_b_y, sf_mex_create("y", &c3_u, 0, 0U, 0U, 0U, 0));
  sf_mex_setcell(c3_y, 0, c3_b_y);
  c3_b_u = chartInstance.c3_is_active_c3_TrqController;
  c3_c_y = NULL;
  sf_mex_assign(&c3_c_y, sf_mex_create("y", &c3_b_u, 3, 0U, 0U, 0U, 0));
  sf_mex_setcell(c3_y, 1, c3_c_y);
  sf_mex_assign(&c3_st, c3_y);
  return c3_st;
}

static void set_sim_state_c3_TrqController(const mxArray *c3_st)
{
  const mxArray *c3_u;
  const mxArray *c3_genTorque;
  real_T c3_d0;
  real_T c3_y;
  const mxArray *c3_b_is_active_c3_TrqController;
  uint8_T c3_u0;
  uint8_T c3_b_y;
  real_T *c3_b_genTorque;
  c3_b_genTorque = (real_T *)ssGetOutputPortSignal(chartInstance.S, 1);
  chartInstance.c3_doneDoubleBufferReInit = true;
  c3_u = sf_mex_dup(c3_st);
  c3_genTorque = sf_mex_dup(sf_mex_getcell(c3_u, 0));
  sf_mex_import("genTorque", sf_mex_dup(c3_genTorque), &c3_d0, 1, 0, 0U, 0, 0U,
                0);
  c3_y = c3_d0;
  sf_mex_destroy(&c3_genTorque);
  *c3_b_genTorque = c3_y;
  c3_b_is_active_c3_TrqController = sf_mex_dup(sf_mex_getcell(c3_u, 1));
  sf_mex_import("is_active_c3_TrqController", sf_mex_dup
                (c3_b_is_active_c3_TrqController), &c3_u0, 1, 3, 0U, 0, 0U, 0);
  c3_b_y = c3_u0;
  sf_mex_destroy(&c3_b_is_active_c3_TrqController);
  chartInstance.c3_is_active_c3_TrqController = c3_b_y;
  sf_mex_destroy(&c3_u);
  c3_update_debugger_state_c3_TrqController();
  sf_mex_destroy(&c3_st);
}

static void finalize_c3_TrqController(void)
{
}

static void sf_c3_TrqController(void)
{
  int32_T c3_previousEvent;
  real_T *c3_W_hss;
  real_T *c3_genTorque;
  c3_genTorque = (real_T *)ssGetOutputPortSignal(chartInstance.S, 1);
  c3_W_hss = (real_T *)ssGetInputPortSignal(chartInstance.S, 0);
  _sfTime_ = (real_T)ssGetT(chartInstance.S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG,0);
  _SFD_DATA_RANGE_CHECK(*c3_W_hss, 0U);
  _SFD_DATA_RANGE_CHECK(*c3_genTorque, 1U);
  c3_previousEvent = _sfEvent_;
  _sfEvent_ = CALL_EVENT;
  c3_c3_TrqController();
  _sfEvent_ = c3_previousEvent;
  sf_debug_check_for_state_inconsistency(_TrqControllerMachineNumber_,
    chartInstance.chartNumber, chartInstance.instanceNumber);
}

static void c3_c3_TrqController(void)
{
  real_T c3_W_hss;
  real_T c3_nargout = 1.0;
  real_T c3_nargin = 1.0;
  real_T c3_K;
  real_T c3_MAXRPM;
  real_T c3_RATIO;
  real_T c3_MAXTORQUE;
  real_T c3_TSRZERO;
  real_T c3_CPZERO;
  real_T c3_RADIUS;
  real_T c3_DENSITY;
  real_T c3_genTorque;
  real_T c3_a;
  real_T c3_A;
  real_T c3_x;
  real_T c3_b_x;
  real_T c3_c_x;
  real_T c3_b_a;
  real_T c3_b;
  real_T c3_b_b;
  real_T c3_y;
  real_T c3_c_b;
  real_T *c3_b_genTorque;
  real_T *c3_b_W_hss;
  c3_b_genTorque = (real_T *)ssGetOutputPortSignal(chartInstance.S, 1);
  c3_b_W_hss = (real_T *)ssGetInputPortSignal(chartInstance.S, 0);
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG,0);
  c3_W_hss = *c3_b_W_hss;
  sf_debug_symbol_scope_push(12U, 0U);
  sf_debug_symbol_scope_add("nargout", &c3_nargout, c3_sf_marshall);
  sf_debug_symbol_scope_add("nargin", &c3_nargin, c3_sf_marshall);
  sf_debug_symbol_scope_add("K", &c3_K, c3_sf_marshall);
  sf_debug_symbol_scope_add("MAXRPM", &c3_MAXRPM, c3_sf_marshall);
  sf_debug_symbol_scope_add("RATIO", &c3_RATIO, c3_sf_marshall);
  sf_debug_symbol_scope_add("MAXTORQUE", &c3_MAXTORQUE, c3_sf_marshall);
  sf_debug_symbol_scope_add("TSRZERO", &c3_TSRZERO, c3_sf_marshall);
  sf_debug_symbol_scope_add("CPZERO", &c3_CPZERO, c3_sf_marshall);
  sf_debug_symbol_scope_add("RADIUS", &c3_RADIUS, c3_sf_marshall);
  sf_debug_symbol_scope_add("DENSITY", &c3_DENSITY, c3_sf_marshall);
  sf_debug_symbol_scope_add("genTorque", &c3_genTorque, c3_sf_marshall);
  sf_debug_symbol_scope_add("W_hss", &c3_W_hss, c3_sf_marshall);
  CV_EML_FCN(0, 0);

  /*  This function adapted by Paul Fleming from one written by JW van Wingerden */
  /* Currently does not contain any checks on pitch angle for fear */
  /* future advanced codes will trip this accidentally */
  /*  */
  /* Also does not contain region 1.5 as documented in Jonkman */
  /* Also does not contain "optimal tracking rotor code" */
  /* See unused Torque Controller Block for example of pitch influence */
  /* Define constants */
  _SFD_EML_CALL(0,11);
  c3_DENSITY = 1.0;

  /* Measured 10/21/10 */
  _SFD_EML_CALL(0,12);
  c3_RADIUS = 20.0;
  _SFD_EML_CALL(0,13);
  c3_CPZERO = 0.4658;
  _SFD_EML_CALL(0,14);
  c3_TSRZERO = 7.1;

  /* Higher TSR to get rid of awkward saturation region 2.5 */
  _SFD_EML_CALL(0,15);
  c3_MAXTORQUE = 3.52437E+003;
  _SFD_EML_CALL(0,16);
  c3_RATIO = 43.164;
  _SFD_EML_CALL(0,17);
  c3_MAXRPM = 1600.0;

  /* lowered RPM for stability */
  /* Calculate k */
  _SFD_EML_CALL(0,20);
  c3_K = 6.5417552767794268E+003;

  /* Region 2 */
  _SFD_EML_CALL(0,24);
  c3_a = c3_W_hss;
  c3_A = c3_a * 3.1415926535897931E+000;
  c3_x = c3_A;
  c3_b_x = c3_x;
  c3_c_x = c3_b_x;
  c3_b_a = c3_c_x / 30.0;
  c3_b = c3_power(c3_b_a, 2.0);
  c3_genTorque = 8.1344696250385318E-002 * c3_b;

  /* Region 2.5 Code */
  _SFD_EML_CALL(0,27);
  c3_b_b = c3_W_hss - 1504.0;
  c3_y = 44.054625 * c3_b_b;
  if (CV_EML_IF(0, 0, c3_y > c3_genTorque)) {
    _SFD_EML_CALL(0,28);
    c3_c_b = c3_W_hss - 1504.0;
    c3_genTorque = 44.054625 * c3_c_b;
  }

  /* Region 3 code */
  _SFD_EML_CALL(0,32);
  if (CV_EML_IF(0, 1, c3_W_hss >= 1584.0)) {
    _SFD_EML_CALL(0,33);
    c3_genTorque = c3_MAXTORQUE;
  }

  /* Region 1 */
  _SFD_EML_CALL(0,39);
  if (CV_EML_IF(0, 2, c3_W_hss < 4.3164E+002)) {
    _SFD_EML_CALL(0,40);
    c3_genTorque = 0.0;
  }

  /* m1_5= K*520^2/120;          %Region 2.5 slope (Nm/rpm) */
  /* c1_5 = -K*520^2/120*400;     %Region 2.5 offset (Nm) */
  /* elseif (W_hss > 400)                    %Region 1.5 */
  /*     genTorque = m1_5 * W_hss + c1_5; */
  _SFD_EML_CALL(0,-40);
  sf_debug_symbol_scope_pop();
  *c3_b_genTorque = c3_genTorque;
  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG,0);
}

static void init_script_number_translation(uint32_T c3_machineNumber, uint32_T
  c3_chartNumber)
{
}

static real_T c3_power(real_T c3_a, real_T c3_b)
{
  real_T c3_ak;
  real_T c3_bk;
  real_T c3_x;
  real_T c3_b_x;
  c3_ak = c3_a;
  c3_bk = c3_b;
  if (c3_ak < 0.0) {
    c3_x = c3_bk;
    c3_b_x = c3_x;
    c3_b_x = muDoubleScalarFloor(c3_b_x);
    if (c3_b_x != c3_bk) {
      c3_eml_error();
      goto label_1;
    }
  }

 label_1:
  ;
  return muDoubleScalarPower(c3_ak, c3_bk);
}

static void c3_eml_error(void)
{
  int32_T c3_i0;
  static char_T c3_cv0[102] = { 'D', 'o', 'm', 'a', 'i', 'n', ' ', 'e', 'r', 'r',
    'o', 'r', '.', ' ', 'T', 'o', ' ', 'c', 'o', 'm',
    'p', 'u', 't', 'e', ' ', 'c', 'o', 'm', 'p', 'l', 'e', 'x', ' ', 'r', 'e',
    's', 'u', 'l', 't', 's',
    ',', ' ', 'm', 'a', 'k', 'e', ' ', 'a', 't', ' ', 'l', 'e', 'a', 's', 't',
    ' ', 'o', 'n', 'e', ' ',
    'i', 'n', 'p', 'u', 't', ' ', 'c', 'o', 'm', 'p', 'l', 'e', 'x', ',', ' ',
    'e', '.', 'g', '.', ' ',
    '\'', 'p', 'o', 'w', 'e', 'r', '(', 'c', 'o', 'm', 'p', 'l', 'e', 'x', '(',
    'a', ')', ',', 'b', ')',
    '\'', '.' };

  char_T c3_u[102];
  const mxArray *c3_y = NULL;
  int32_T c3_i1;
  static char_T c3_cv1[32] = { 'E', 'm', 'b', 'e', 'd', 'd', 'e', 'd', 'M', 'A',
    'T', 'L', 'A', 'B', ':', 'p', 'o', 'w', 'e', 'r', ':'
    , 'd', 'o', 'm', 'a', 'i', 'n', 'E', 'r', 'r', 'o', 'r' };

  char_T c3_b_u[32];
  const mxArray *c3_b_y = NULL;
  for (c3_i0 = 0; c3_i0 < 102; c3_i0 = c3_i0 + 1) {
    c3_u[c3_i0] = c3_cv0[c3_i0];
  }

  c3_y = NULL;
  sf_mex_assign(&c3_y, sf_mex_create("y", &c3_u, 10, 0U, 1U, 0U, 2, 1, 102));
  for (c3_i1 = 0; c3_i1 < 32; c3_i1 = c3_i1 + 1) {
    c3_b_u[c3_i1] = c3_cv1[c3_i1];
  }

  c3_b_y = NULL;
  sf_mex_assign(&c3_b_y, sf_mex_create("y", &c3_b_u, 10, 0U, 1U, 0U, 2, 1, 32));
  sf_mex_call_debug("error", 0U, 2U, 14, c3_b_y, 14, c3_y);
}

static const mxArray *c3_sf_marshall(void *c3_chartInstance, void *c3_u)
{
  const mxArray *c3_y = NULL;
  real_T c3_b_u;
  const mxArray *c3_b_y = NULL;
  c3_y = NULL;
  c3_b_u = *((real_T *)c3_u);
  c3_b_y = NULL;
  sf_mex_assign(&c3_b_y, sf_mex_create("y", &c3_b_u, 0, 0U, 0U, 0U, 0));
  sf_mex_assign(&c3_y, c3_b_y);
  return c3_y;
}

const mxArray *sf_c3_TrqController_get_eml_resolved_functions_info(void)
{
  const mxArray *c3_nameCaptureInfo = NULL;
  c3_ResolvedFunctionInfo c3_info[33];
  const mxArray *c3_m0 = NULL;
  int32_T c3_i2;
  c3_ResolvedFunctionInfo *c3_r0;
  c3_nameCaptureInfo = NULL;
  c3_info_helper(c3_info);
  sf_mex_assign(&c3_m0, sf_mex_createstruct("nameCaptureInfo", 1, 33));
  for (c3_i2 = 0; c3_i2 < 33; c3_i2 = c3_i2 + 1) {
    c3_r0 = &c3_info[c3_i2];
    sf_mex_addfield(c3_m0, sf_mex_create("nameCaptureInfo", c3_r0->context, 15,
      0U, 0U, 0U, 2, 1, strlen(c3_r0->context)), "context",
                    "nameCaptureInfo", c3_i2);
    sf_mex_addfield(c3_m0, sf_mex_create("nameCaptureInfo", c3_r0->name, 15, 0U,
      0U, 0U, 2, 1, strlen(c3_r0->name)), "name",
                    "nameCaptureInfo", c3_i2);
    sf_mex_addfield(c3_m0, sf_mex_create("nameCaptureInfo", c3_r0->dominantType,
      15, 0U, 0U, 0U, 2, 1, strlen(c3_r0->dominantType)),
                    "dominantType", "nameCaptureInfo", c3_i2);
    sf_mex_addfield(c3_m0, sf_mex_create("nameCaptureInfo", c3_r0->resolved, 15,
      0U, 0U, 0U, 2, 1, strlen(c3_r0->resolved)), "resolved"
                    , "nameCaptureInfo", c3_i2);
    sf_mex_addfield(c3_m0, sf_mex_create("nameCaptureInfo", &c3_r0->fileLength,
      7, 0U, 0U, 0U, 0), "fileLength", "nameCaptureInfo",
                    c3_i2);
    sf_mex_addfield(c3_m0, sf_mex_create("nameCaptureInfo", &c3_r0->fileTime1, 7,
      0U, 0U, 0U, 0), "fileTime1", "nameCaptureInfo", c3_i2);
    sf_mex_addfield(c3_m0, sf_mex_create("nameCaptureInfo", &c3_r0->fileTime2, 7,
      0U, 0U, 0U, 0), "fileTime2", "nameCaptureInfo", c3_i2);
  }

  sf_mex_assign(&c3_nameCaptureInfo, c3_m0);
  return c3_nameCaptureInfo;
}

static void c3_info_helper(c3_ResolvedFunctionInfo c3_info[33])
{
  c3_info[0].context = "";
  c3_info[0].name = "pi";
  c3_info[0].dominantType = "";
  c3_info[0].resolved = "[B]pi";
  c3_info[0].fileLength = 0U;
  c3_info[0].fileTime1 = 0U;
  c3_info[0].fileTime2 = 0U;
  c3_info[1].context = "";
  c3_info[1].name = "mtimes";
  c3_info[1].dominantType = "double";
  c3_info[1].resolved = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c3_info[1].fileLength = 3302U;
  c3_info[1].fileTime1 = 1242779694U;
  c3_info[1].fileTime2 = 0U;
  c3_info[2].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c3_info[2].name = "nargin";
  c3_info[2].dominantType = "";
  c3_info[2].resolved = "[B]nargin";
  c3_info[2].fileLength = 0U;
  c3_info[2].fileTime1 = 0U;
  c3_info[2].fileTime2 = 0U;
  c3_info[3].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c3_info[3].name = "gt";
  c3_info[3].dominantType = "double";
  c3_info[3].resolved = "[B]gt";
  c3_info[3].fileLength = 0U;
  c3_info[3].fileTime1 = 0U;
  c3_info[3].fileTime2 = 0U;
  c3_info[4].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c3_info[4].name = "isa";
  c3_info[4].dominantType = "double";
  c3_info[4].resolved = "[B]isa";
  c3_info[4].fileLength = 0U;
  c3_info[4].fileTime1 = 0U;
  c3_info[4].fileTime2 = 0U;
  c3_info[5].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c3_info[5].name = "isinteger";
  c3_info[5].dominantType = "double";
  c3_info[5].resolved = "[B]isinteger";
  c3_info[5].fileLength = 0U;
  c3_info[5].fileTime1 = 0U;
  c3_info[5].fileTime2 = 0U;
  c3_info[6].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c3_info[6].name = "isscalar";
  c3_info[6].dominantType = "double";
  c3_info[6].resolved = "[B]isscalar";
  c3_info[6].fileLength = 0U;
  c3_info[6].fileTime1 = 0U;
  c3_info[6].fileTime2 = 0U;
  c3_info[7].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c3_info[7].name = "strcmp";
  c3_info[7].dominantType = "char";
  c3_info[7].resolved = "[B]strcmp";
  c3_info[7].fileLength = 0U;
  c3_info[7].fileTime1 = 0U;
  c3_info[7].fileTime2 = 0U;
  c3_info[8].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c3_info[8].name = "size";
  c3_info[8].dominantType = "double";
  c3_info[8].resolved = "[B]size";
  c3_info[8].fileLength = 0U;
  c3_info[8].fileTime1 = 0U;
  c3_info[8].fileTime2 = 0U;
  c3_info[9].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c3_info[9].name = "eq";
  c3_info[9].dominantType = "double";
  c3_info[9].resolved = "[B]eq";
  c3_info[9].fileLength = 0U;
  c3_info[9].fileTime1 = 0U;
  c3_info[9].fileTime2 = 0U;
  c3_info[10].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c3_info[10].name = "class";
  c3_info[10].dominantType = "double";
  c3_info[10].resolved = "[B]class";
  c3_info[10].fileLength = 0U;
  c3_info[10].fileTime1 = 0U;
  c3_info[10].fileTime2 = 0U;
  c3_info[11].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mtimes.m";
  c3_info[11].name = "not";
  c3_info[11].dominantType = "logical";
  c3_info[11].resolved = "[B]not";
  c3_info[11].fileLength = 0U;
  c3_info[11].fileTime1 = 0U;
  c3_info[11].fileTime2 = 0U;
  c3_info[12].context = "";
  c3_info[12].name = "mpower";
  c3_info[12].dominantType = "double";
  c3_info[12].resolved = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m";
  c3_info[12].fileLength = 3710U;
  c3_info[12].fileTime1 = 1238463090U;
  c3_info[12].fileTime2 = 0U;
  c3_info[13].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m";
  c3_info[13].name = "ndims";
  c3_info[13].dominantType = "double";
  c3_info[13].resolved = "[B]ndims";
  c3_info[13].fileLength = 0U;
  c3_info[13].fileTime1 = 0U;
  c3_info[13].fileTime2 = 0U;
  c3_info[14].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mpower.m";
  c3_info[14].name = "power";
  c3_info[14].dominantType = "double";
  c3_info[14].resolved = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m";
  c3_info[14].fileLength = 5380U;
  c3_info[14].fileTime1 = 1228122698U;
  c3_info[14].fileTime2 = 0U;
  c3_info[15].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m";
  c3_info[15].name = "eml_scalar_eg";
  c3_info[15].dominantType = "double";
  c3_info[15].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c3_info[15].fileLength = 3068U;
  c3_info[15].fileTime1 = 1240290810U;
  c3_info[15].fileTime2 = 0U;
  c3_info[16].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m/any_enums";
  c3_info[16].name = "false";
  c3_info[16].dominantType = "";
  c3_info[16].resolved = "[B]false";
  c3_info[16].fileLength = 0U;
  c3_info[16].fileTime1 = 0U;
  c3_info[16].fileTime2 = 0U;
  c3_info[17].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m";
  c3_info[17].name = "isstruct";
  c3_info[17].dominantType = "double";
  c3_info[17].resolved = "[B]isstruct";
  c3_info[17].fileLength = 0U;
  c3_info[17].fileTime1 = 0U;
  c3_info[17].fileTime2 = 0U;
  c3_info[18].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m/zerosum";
  c3_info[18].name = "cast";
  c3_info[18].dominantType = "double";
  c3_info[18].resolved = "[B]cast";
  c3_info[18].fileLength = 0U;
  c3_info[18].fileTime1 = 0U;
  c3_info[18].fileTime2 = 0U;
  c3_info[19].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m/zerosum";
  c3_info[19].name = "plus";
  c3_info[19].dominantType = "double";
  c3_info[19].resolved = "[B]plus";
  c3_info[19].fileLength = 0U;
  c3_info[19].fileTime1 = 0U;
  c3_info[19].fileTime2 = 0U;
  c3_info[20].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalar_eg.m/allreal";
  c3_info[20].name = "isreal";
  c3_info[20].dominantType = "double";
  c3_info[20].resolved = "[B]isreal";
  c3_info[20].fileLength = 0U;
  c3_info[20].fileTime1 = 0U;
  c3_info[20].fileTime2 = 0U;
  c3_info[21].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m";
  c3_info[21].name = "eml_scalexp_alloc";
  c3_info[21].dominantType = "double";
  c3_info[21].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m";
  c3_info[21].fileLength = 808U;
  c3_info[21].fileTime1 = 1230523500U;
  c3_info[21].fileTime2 = 0U;
  c3_info[22].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_scalexp_alloc.m";
  c3_info[22].name = "minus";
  c3_info[22].dominantType = "double";
  c3_info[22].resolved = "[B]minus";
  c3_info[22].fileLength = 0U;
  c3_info[22].fileTime1 = 0U;
  c3_info[22].fileTime2 = 0U;
  c3_info[23].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m";
  c3_info[23].name = "lt";
  c3_info[23].dominantType = "double";
  c3_info[23].resolved = "[B]lt";
  c3_info[23].fileLength = 0U;
  c3_info[23].fileTime1 = 0U;
  c3_info[23].fileTime2 = 0U;
  c3_info[24].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m";
  c3_info[24].name = "eml_scalar_floor";
  c3_info[24].dominantType = "double";
  c3_info[24].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/elfun/eml_scalar_floor.m";
  c3_info[24].fileLength = 260U;
  c3_info[24].fileTime1 = 1209359590U;
  c3_info[24].fileTime2 = 0U;
  c3_info[25].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m";
  c3_info[25].name = "ne";
  c3_info[25].dominantType = "double";
  c3_info[25].resolved = "[B]ne";
  c3_info[25].fileLength = 0U;
  c3_info[25].fileTime1 = 0U;
  c3_info[25].fileTime2 = 0U;
  c3_info[26].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/power.m";
  c3_info[26].name = "eml_error";
  c3_info[26].dominantType = "char";
  c3_info[26].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_error.m";
  c3_info[26].fileLength = 315U;
  c3_info[26].fileTime1 = 1213955546U;
  c3_info[26].fileTime2 = 0U;
  c3_info[27].context = "";
  c3_info[27].name = "mrdivide";
  c3_info[27].dominantType = "double";
  c3_info[27].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.m";
  c3_info[27].fileLength = 800U;
  c3_info[27].fileTime1 = 1238463092U;
  c3_info[27].fileTime2 = 0U;
  c3_info[28].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.m";
  c3_info[28].name = "ge";
  c3_info[28].dominantType = "double";
  c3_info[28].resolved = "[B]ge";
  c3_info[28].fileLength = 0U;
  c3_info[28].fileTime1 = 0U;
  c3_info[28].fileTime2 = 0U;
  c3_info[29].context =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/mrdivide.m";
  c3_info[29].name = "rdivide";
  c3_info[29].dominantType = "double";
  c3_info[29].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c3_info[29].fileLength = 620U;
  c3_info[29].fileTime1 = 1213955566U;
  c3_info[29].fileTime2 = 0U;
  c3_info[30].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c3_info[30].name = "isempty";
  c3_info[30].dominantType = "double";
  c3_info[30].resolved = "[B]isempty";
  c3_info[30].fileLength = 0U;
  c3_info[30].fileTime1 = 0U;
  c3_info[30].fileTime2 = 0U;
  c3_info[31].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c3_info[31].name = "eml_warning";
  c3_info[31].dominantType = "char";
  c3_info[31].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_warning.m";
  c3_info[31].fileLength = 262U;
  c3_info[31].fileTime1 = 1236286078U;
  c3_info[31].fileTime2 = 0U;
  c3_info[32].context = "[ILX]$matlabroot$/toolbox/eml/lib/matlab/ops/rdivide.m";
  c3_info[32].name = "eml_div";
  c3_info[32].dominantType = "double";
  c3_info[32].resolved =
    "[ILX]$matlabroot$/toolbox/eml/lib/matlab/eml/eml_div.m";
  c3_info[32].fileLength = 4269U;
  c3_info[32].fileTime1 = 1228122626U;
  c3_info[32].fileTime2 = 0U;
}

static const mxArray *c3_b_sf_marshall(void *c3_chartInstance, void *c3_u)
{
  const mxArray *c3_y = NULL;
  boolean_T c3_b_u;
  const mxArray *c3_b_y = NULL;
  c3_y = NULL;
  c3_b_u = *((boolean_T *)c3_u);
  c3_b_y = NULL;
  sf_mex_assign(&c3_b_y, sf_mex_create("y", &c3_b_u, 11, 0U, 0U, 0U, 0));
  sf_mex_assign(&c3_y, c3_b_y);
  return c3_y;
}

static void init_dsm_address_info(void)
{
}

/* SFunction Glue Code */
void sf_c3_TrqController_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(75350754U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(2996353432U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(261889525U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(1413489104U);
}

mxArray *sf_c3_TrqController_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,4,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateDoubleMatrix(4,1,mxREAL);
    double *pr = mxGetPr(mxChecksum);
    pr[0] = (double)(1956993932U);
    pr[1] = (double)(966155688U);
    pr[2] = (double)(2223715995U);
    pr[3] = (double)(2776086088U);
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,1,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,1,3,dataFields);

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,0,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,0,"type",mxType);
    }

    mxSetField(mxData,0,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  return(mxAutoinheritanceInfo);
}

static mxArray *sf_get_sim_state_info_c3_TrqController(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  char *infoEncStr[] = {
    "100 S1x2'type','srcId','name','auxInfo'{{M[1],M[6],T\"genTorque\",},{M[8],M[0],T\"is_active_c3_TrqController\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 2, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c3_TrqController_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (_TrqControllerMachineNumber_,
           3,
           1,
           1,
           2,
           0,
           0,
           0,
           0,
           0,
           &(chartInstance.chartNumber),
           &(chartInstance.instanceNumber),
           ssGetPath(S),
           (void *)S);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          init_script_number_translation(_TrqControllerMachineNumber_,
            chartInstance.chartNumber);
          sf_debug_set_chart_disable_implicit_casting
            (_TrqControllerMachineNumber_,chartInstance.chartNumber,1);
          sf_debug_set_chart_event_thresholds(_TrqControllerMachineNumber_,
            chartInstance.chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,"W_hss",0,
                              (MexFcnForType)c3_sf_marshall);
          _SFD_SET_DATA_PROPS(1,2,0,1,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,
                              "genTorque",0,(MexFcnForType)c3_sf_marshall);
          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of EML Model Coverage */
        _SFD_CV_INIT_EML(0,1,3,0,0,0,0,0);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,1224);
        _SFD_CV_INIT_EML_IF(0,0,795,856,-1,923);
        _SFD_CV_INIT_EML_IF(0,1,940,968,-1,999);
        _SFD_CV_INIT_EML_IF(0,2,1013,1036,-1,1059);
        _SFD_TRANS_COV_WTS(0,0,0,1,0);
        if (chartAlreadyPresent==0) {
          _SFD_TRANS_COV_MAPS(0,
                              0,NULL,NULL,
                              0,NULL,NULL,
                              1,NULL,NULL,
                              0,NULL,NULL);
        }

        {
          real_T *c3_W_hss;
          real_T *c3_genTorque;
          c3_genTorque = (real_T *)ssGetOutputPortSignal(chartInstance.S, 1);
          c3_W_hss = (real_T *)ssGetInputPortSignal(chartInstance.S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, c3_W_hss);
          _SFD_SET_DATA_VALUE_PTR(1U, c3_genTorque);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(_TrqControllerMachineNumber_,
        chartInstance.chartNumber,chartInstance.instanceNumber);
    }
  }
}

static void sf_opaque_initialize_c3_TrqController(void *chartInstanceVar)
{
  chart_debug_initialization(chartInstance.S,0);
  initialize_params_c3_TrqController();
  initialize_c3_TrqController();
}

static void sf_opaque_enable_c3_TrqController(void *chartInstanceVar)
{
  enable_c3_TrqController();
}

static void sf_opaque_disable_c3_TrqController(void *chartInstanceVar)
{
  disable_c3_TrqController();
}

static void sf_opaque_gateway_c3_TrqController(void *chartInstanceVar)
{
  sf_c3_TrqController();
}

static mxArray* sf_opaque_get_sim_state_c3_TrqController(void *chartInstanceVar)
{
  mxArray *st = (mxArray *) get_sim_state_c3_TrqController();
  return st;
}

static void sf_opaque_set_sim_state_c3_TrqController(void *chartInstanceVar,
  const mxArray *st)
{
  set_sim_state_c3_TrqController(sf_mex_dup(st));
}

static void sf_opaque_terminate_c3_TrqController(void *chartInstanceVar)
{
  if (sim_mode_is_rtw_gen(chartInstance.S) || sim_mode_is_external
      (chartInstance.S)) {
    sf_clear_rtw_identifier(chartInstance.S);
  }

  finalize_c3_TrqController();
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c3_TrqController(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    initialize_params_c3_TrqController();
  }
}

static void mdlSetWorkWidths_c3_TrqController(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable("TrqController","TrqController",3);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop("TrqController","TrqController",3,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop("TrqController",
      "TrqController",3,"gatewayCannotBeInlinedMultipleTimes"));
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,"TrqController","TrqController",3,1);
      sf_mark_chart_reusable_outputs(S,"TrqController","TrqController",3,1);
    }

    sf_set_rtw_dwork_info(S,"TrqController","TrqController",3);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
    ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  }

  ssSetChecksum0(S,(3237972787U));
  ssSetChecksum1(S,(2001025645U));
  ssSetChecksum2(S,(1218872411U));
  ssSetChecksum3(S,(479662804U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
}

static void mdlRTW_c3_TrqController(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    sf_write_symbol_mapping(S, "TrqController", "TrqController",3);
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c3_TrqController(SimStruct *S)
{
  chartInstance.chartInfo.chartInstance = NULL;
  chartInstance.chartInfo.isEMLChart = 1;
  chartInstance.chartInfo.chartInitialized = 0;
  chartInstance.chartInfo.sFunctionGateway = sf_opaque_gateway_c3_TrqController;
  chartInstance.chartInfo.initializeChart =
    sf_opaque_initialize_c3_TrqController;
  chartInstance.chartInfo.terminateChart = sf_opaque_terminate_c3_TrqController;
  chartInstance.chartInfo.enableChart = sf_opaque_enable_c3_TrqController;
  chartInstance.chartInfo.disableChart = sf_opaque_disable_c3_TrqController;
  chartInstance.chartInfo.getSimState = sf_opaque_get_sim_state_c3_TrqController;
  chartInstance.chartInfo.setSimState = sf_opaque_set_sim_state_c3_TrqController;
  chartInstance.chartInfo.getSimStateInfo =
    sf_get_sim_state_info_c3_TrqController;
  chartInstance.chartInfo.zeroCrossings = NULL;
  chartInstance.chartInfo.outputs = NULL;
  chartInstance.chartInfo.derivatives = NULL;
  chartInstance.chartInfo.mdlRTW = mdlRTW_c3_TrqController;
  chartInstance.chartInfo.mdlStart = mdlStart_c3_TrqController;
  chartInstance.chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c3_TrqController;
  chartInstance.chartInfo.extModeExec = NULL;
  chartInstance.chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance.chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance.chartInfo.storeCurrentConfiguration = NULL;
  chartInstance.S = S;
  ssSetUserData(S,(void *)(&(chartInstance.chartInfo)));/* register the chart instance with simstruct */
  if (!sim_mode_is_rtw_gen(S)) {
    init_dsm_address_info();
  }

  chart_debug_initialization(S,1);
}

void c3_TrqController_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c3_TrqController(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c3_TrqController(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c3_TrqController(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c3_TrqController_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
