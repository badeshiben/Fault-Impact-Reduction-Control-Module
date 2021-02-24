/* Include files */

#include <stddef.h>
#include "blas.h"
#include "CART3_PMSG_sfun.h"
#include "c7_CART3_PMSG.h"
#define CHARTINSTANCE_CHARTNUMBER      (chartInstance->chartNumber)
#define CHARTINSTANCE_INSTANCENUMBER   (chartInstance->instanceNumber)
#include "CART3_PMSG_sfun_debug_macros.h"
#define _SF_MEX_LISTEN_FOR_CTRL_C(S)   sf_mex_listen_for_ctrl_c(sfGlobalDebugInstanceStruct,S);

/* Type Definitions */

/* Named Constants */
#define CALL_EVENT                     (-1)

/* Variable Declarations */

/* Variable Definitions */
static real_T _sfTime_;
static const char * c7_debug_family_names[11] = { "nargin", "nargout",
  "DCctrl_ref", "RSC_Pwr", "act_in", "term_in", "Vdc", "GSCpwr_ref", "DC_ref",
  "initFlag", "Pre_mppt_pwr" };

/* Function Declarations */
static void initialize_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct
  *chartInstance);
static void initialize_params_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct
  *chartInstance);
static void enable_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct *chartInstance);
static void disable_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct *chartInstance);
static void c7_update_debugger_state_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct
  *chartInstance);
static const mxArray *get_sim_state_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct *
  chartInstance);
static void set_sim_state_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct
  *chartInstance, const mxArray *c7_st);
static void finalize_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct *chartInstance);
static void sf_gateway_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct
  *chartInstance);
static void initSimStructsc7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct
  *chartInstance);
static void init_script_number_translation(uint32_T c7_machineNumber, uint32_T
  c7_chartNumber, uint32_T c7_instanceNumber);
static const mxArray *c7_sf_marshallOut(void *chartInstanceVoid, void *c7_inData);
static real_T c7_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct *chartInstance,
  const mxArray *c7_b_Pre_mppt_pwr, const char_T *c7_identifier);
static real_T c7_b_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct *chartInstance,
  const mxArray *c7_u, const emlrtMsgIdentifier *c7_parentId);
static void c7_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c7_mxArrayInData, const char_T *c7_varName, void *c7_outData);
static const mxArray *c7_b_sf_marshallOut(void *chartInstanceVoid, void
  *c7_inData);
static real_T c7_c_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct *chartInstance,
  const mxArray *c7_b_initFlag, const char_T *c7_identifier);
static real_T c7_d_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct *chartInstance,
  const mxArray *c7_u, const emlrtMsgIdentifier *c7_parentId);
static void c7_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c7_mxArrayInData, const char_T *c7_varName, void *c7_outData);
static const mxArray *c7_c_sf_marshallOut(void *chartInstanceVoid, void
  *c7_inData);
static real_T c7_e_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct *chartInstance,
  const mxArray *c7_DC_ref, const char_T *c7_identifier);
static real_T c7_f_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct *chartInstance,
  const mxArray *c7_u, const emlrtMsgIdentifier *c7_parentId);
static void c7_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c7_mxArrayInData, const char_T *c7_varName, void *c7_outData);
static const mxArray *c7_d_sf_marshallOut(void *chartInstanceVoid, void
  *c7_inData);
static const mxArray *c7_e_sf_marshallOut(void *chartInstanceVoid, void
  *c7_inData);
static int32_T c7_g_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct
  *chartInstance, const mxArray *c7_u, const emlrtMsgIdentifier *c7_parentId);
static void c7_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c7_mxArrayInData, const char_T *c7_varName, void *c7_outData);
static uint8_T c7_h_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct
  *chartInstance, const mxArray *c7_b_is_active_c7_CART3_PMSG, const char_T
  *c7_identifier);
static uint8_T c7_i_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct
  *chartInstance, const mxArray *c7_u, const emlrtMsgIdentifier *c7_parentId);
static void init_dsm_address_info(SFc7_CART3_PMSGInstanceStruct *chartInstance);

/* Function Definitions */
static void initialize_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct
  *chartInstance)
{
  chartInstance->c7_sfEvent = CALL_EVENT;
  _sfTime_ = sf_get_time(chartInstance->S);
  chartInstance->c7_initFlag_not_empty = false;
  chartInstance->c7_Pre_mppt_pwr_not_empty = false;
  chartInstance->c7_is_active_c7_CART3_PMSG = 0U;
}

static void initialize_params_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void enable_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void disable_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct *chartInstance)
{
  _sfTime_ = sf_get_time(chartInstance->S);
}

static void c7_update_debugger_state_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static const mxArray *get_sim_state_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct *
  chartInstance)
{
  const mxArray *c7_st;
  const mxArray *c7_y = NULL;
  real_T c7_hoistedGlobal;
  real_T c7_u;
  const mxArray *c7_b_y = NULL;
  real_T c7_b_hoistedGlobal;
  real_T c7_b_u;
  const mxArray *c7_c_y = NULL;
  real_T c7_c_hoistedGlobal;
  real_T c7_c_u;
  const mxArray *c7_d_y = NULL;
  real_T c7_d_hoistedGlobal;
  real_T c7_d_u;
  const mxArray *c7_e_y = NULL;
  uint8_T c7_e_hoistedGlobal;
  uint8_T c7_e_u;
  const mxArray *c7_f_y = NULL;
  real_T *c7_DC_ref;
  real_T *c7_GSCpwr_ref;
  c7_DC_ref = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c7_GSCpwr_ref = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  c7_st = NULL;
  c7_st = NULL;
  c7_y = NULL;
  sf_mex_assign(&c7_y, sf_mex_createcellmatrix(5, 1), false);
  c7_hoistedGlobal = *c7_DC_ref;
  c7_u = c7_hoistedGlobal;
  c7_b_y = NULL;
  sf_mex_assign(&c7_b_y, sf_mex_create("y", &c7_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c7_y, 0, c7_b_y);
  c7_b_hoistedGlobal = *c7_GSCpwr_ref;
  c7_b_u = c7_b_hoistedGlobal;
  c7_c_y = NULL;
  sf_mex_assign(&c7_c_y, sf_mex_create("y", &c7_b_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c7_y, 1, c7_c_y);
  c7_c_hoistedGlobal = chartInstance->c7_Pre_mppt_pwr;
  c7_c_u = c7_c_hoistedGlobal;
  c7_d_y = NULL;
  if (!chartInstance->c7_Pre_mppt_pwr_not_empty) {
    sf_mex_assign(&c7_d_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0),
                  false);
  } else {
    sf_mex_assign(&c7_d_y, sf_mex_create("y", &c7_c_u, 0, 0U, 0U, 0U, 0), false);
  }

  sf_mex_setcell(c7_y, 2, c7_d_y);
  c7_d_hoistedGlobal = chartInstance->c7_initFlag;
  c7_d_u = c7_d_hoistedGlobal;
  c7_e_y = NULL;
  if (!chartInstance->c7_initFlag_not_empty) {
    sf_mex_assign(&c7_e_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0),
                  false);
  } else {
    sf_mex_assign(&c7_e_y, sf_mex_create("y", &c7_d_u, 0, 0U, 0U, 0U, 0), false);
  }

  sf_mex_setcell(c7_y, 3, c7_e_y);
  c7_e_hoistedGlobal = chartInstance->c7_is_active_c7_CART3_PMSG;
  c7_e_u = c7_e_hoistedGlobal;
  c7_f_y = NULL;
  sf_mex_assign(&c7_f_y, sf_mex_create("y", &c7_e_u, 3, 0U, 0U, 0U, 0), false);
  sf_mex_setcell(c7_y, 4, c7_f_y);
  sf_mex_assign(&c7_st, c7_y, false);
  return c7_st;
}

static void set_sim_state_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct
  *chartInstance, const mxArray *c7_st)
{
  const mxArray *c7_u;
  real_T *c7_DC_ref;
  real_T *c7_GSCpwr_ref;
  c7_DC_ref = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c7_GSCpwr_ref = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  chartInstance->c7_doneDoubleBufferReInit = true;
  c7_u = sf_mex_dup(c7_st);
  *c7_DC_ref = c7_e_emlrt_marshallIn(chartInstance, sf_mex_dup(sf_mex_getcell
    (c7_u, 0)), "DC_ref");
  *c7_GSCpwr_ref = c7_e_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c7_u, 1)), "GSCpwr_ref");
  chartInstance->c7_Pre_mppt_pwr = c7_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c7_u, 2)), "Pre_mppt_pwr");
  chartInstance->c7_initFlag = c7_c_emlrt_marshallIn(chartInstance, sf_mex_dup
    (sf_mex_getcell(c7_u, 3)), "initFlag");
  chartInstance->c7_is_active_c7_CART3_PMSG = c7_h_emlrt_marshallIn
    (chartInstance, sf_mex_dup(sf_mex_getcell(c7_u, 4)),
     "is_active_c7_CART3_PMSG");
  sf_mex_destroy(&c7_u);
  c7_update_debugger_state_c7_CART3_PMSG(chartInstance);
  sf_mex_destroy(&c7_st);
}

static void finalize_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

static void sf_gateway_c7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct
  *chartInstance)
{
  real_T c7_hoistedGlobal;
  real_T c7_b_hoistedGlobal;
  boolean_T c7_c_hoistedGlobal;
  real_T c7_d_hoistedGlobal;
  real_T c7_e_hoistedGlobal;
  real_T c7_DCctrl_ref;
  real_T c7_RSC_Pwr;
  boolean_T c7_act_in;
  real_T c7_term_in;
  real_T c7_Vdc;
  uint32_T c7_debug_family_var_map[11];
  real_T c7_nargin = 5.0;
  real_T c7_nargout = 2.0;
  real_T c7_GSCpwr_ref;
  real_T c7_DC_ref;
  real_T *c7_b_DCctrl_ref;
  real_T *c7_b_GSCpwr_ref;
  real_T *c7_b_RSC_Pwr;
  boolean_T *c7_b_act_in;
  real_T *c7_b_term_in;
  real_T *c7_b_Vdc;
  real_T *c7_b_DC_ref;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  c7_b_DC_ref = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
  c7_b_Vdc = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
  c7_b_term_in = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
  c7_b_act_in = (boolean_T *)ssGetInputPortSignal(chartInstance->S, 2);
  c7_b_RSC_Pwr = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
  c7_b_GSCpwr_ref = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
  c7_b_DCctrl_ref = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
  _SFD_SYMBOL_SCOPE_PUSH(0U, 0U);
  _sfTime_ = sf_get_time(chartInstance->S);
  _SFD_CC_CALL(CHART_ENTER_SFUNCTION_TAG, 6U, chartInstance->c7_sfEvent);
  _SFD_DATA_RANGE_CHECK(*c7_b_DCctrl_ref, 0U);
  chartInstance->c7_sfEvent = CALL_EVENT;
  _SFD_CC_CALL(CHART_ENTER_DURING_FUNCTION_TAG, 6U, chartInstance->c7_sfEvent);
  c7_hoistedGlobal = *c7_b_DCctrl_ref;
  c7_b_hoistedGlobal = *c7_b_RSC_Pwr;
  c7_c_hoistedGlobal = *c7_b_act_in;
  c7_d_hoistedGlobal = *c7_b_term_in;
  c7_e_hoistedGlobal = *c7_b_Vdc;
  c7_DCctrl_ref = c7_hoistedGlobal;
  c7_RSC_Pwr = c7_b_hoistedGlobal;
  c7_act_in = c7_c_hoistedGlobal;
  c7_term_in = c7_d_hoistedGlobal;
  c7_Vdc = c7_e_hoistedGlobal;
  _SFD_SYMBOL_SCOPE_PUSH_EML(0U, 11U, 11U, c7_debug_family_names,
    c7_debug_family_var_map);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c7_nargin, 0U, c7_c_sf_marshallOut,
    c7_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c7_nargout, 1U, c7_c_sf_marshallOut,
    c7_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c7_DCctrl_ref, 2U, c7_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c7_RSC_Pwr, 3U, c7_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c7_act_in, 4U, c7_d_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c7_term_in, 5U, c7_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML(&c7_Vdc, 6U, c7_c_sf_marshallOut);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c7_GSCpwr_ref, 7U, c7_c_sf_marshallOut,
    c7_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&c7_DC_ref, 8U, c7_c_sf_marshallOut,
    c7_c_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&chartInstance->c7_initFlag, 9U,
    c7_b_sf_marshallOut, c7_b_sf_marshallIn);
  _SFD_SYMBOL_SCOPE_ADD_EML_IMPORTABLE(&chartInstance->c7_Pre_mppt_pwr, 10U,
    c7_sf_marshallOut, c7_sf_marshallIn);
  CV_EML_FCN(0, 0);
  _SFD_EML_CALL(0U, chartInstance->c7_sfEvent, 3);
  _SFD_EML_CALL(0U, chartInstance->c7_sfEvent, 4);
  _SFD_EML_CALL(0U, chartInstance->c7_sfEvent, 5);
  if (CV_EML_IF(0, 1, 0, !chartInstance->c7_initFlag_not_empty)) {
    _SFD_EML_CALL(0U, chartInstance->c7_sfEvent, 6);
    chartInstance->c7_initFlag = 1.0;
    chartInstance->c7_initFlag_not_empty = true;
    _SFD_EML_CALL(0U, chartInstance->c7_sfEvent, 7);
    chartInstance->c7_Pre_mppt_pwr = 0.0;
    chartInstance->c7_Pre_mppt_pwr_not_empty = true;
  }

  _SFD_EML_CALL(0U, chartInstance->c7_sfEvent, 10);
  c7_GSCpwr_ref = c7_DCctrl_ref;
  _SFD_EML_CALL(0U, chartInstance->c7_sfEvent, 11);
  c7_DC_ref = 3000.0;
  _SFD_EML_CALL(0U, chartInstance->c7_sfEvent, 13);
  guard2 = false;
  if (CV_EML_COND(0, 1, 0, (real_T)c7_act_in == 1.0)) {
    if (CV_EML_COND(0, 1, 1, chartInstance->c7_Pre_mppt_pwr == 0.0)) {
      CV_EML_MCDC(0, 1, 0, true);
      CV_EML_IF(0, 1, 1, true);
      _SFD_EML_CALL(0U, chartInstance->c7_sfEvent, 14);
      chartInstance->c7_Pre_mppt_pwr = c7_RSC_Pwr;
    } else {
      guard2 = true;
    }
  } else {
    guard2 = true;
  }

  if (guard2 == true) {
    CV_EML_MCDC(0, 1, 0, false);
    CV_EML_IF(0, 1, 1, false);
  }

  _SFD_EML_CALL(0U, chartInstance->c7_sfEvent, 17);
  guard1 = false;
  if (CV_EML_COND(0, 1, 2, c7_term_in == 1.0)) {
    if (CV_EML_COND(0, 1, 3, c7_RSC_Pwr == chartInstance->c7_Pre_mppt_pwr)) {
      CV_EML_MCDC(0, 1, 1, true);
      CV_EML_IF(0, 1, 2, true);
      _SFD_EML_CALL(0U, chartInstance->c7_sfEvent, 18);
      c7_GSCpwr_ref = chartInstance->c7_Pre_mppt_pwr;
    } else {
      guard1 = true;
    }
  } else {
    guard1 = true;
  }

  if (guard1 == true) {
    CV_EML_MCDC(0, 1, 1, false);
    CV_EML_IF(0, 1, 2, false);
  }

  _SFD_EML_CALL(0U, chartInstance->c7_sfEvent, -18);
  _SFD_SYMBOL_SCOPE_POP();
  *c7_b_GSCpwr_ref = c7_GSCpwr_ref;
  *c7_b_DC_ref = c7_DC_ref;
  _SFD_CC_CALL(EXIT_OUT_OF_FUNCTION_TAG, 6U, chartInstance->c7_sfEvent);
  _SFD_SYMBOL_SCOPE_POP();
  _SFD_CHECK_FOR_STATE_INCONSISTENCY(_CART3_PMSGMachineNumber_,
    chartInstance->chartNumber, chartInstance->instanceNumber);
  _SFD_DATA_RANGE_CHECK(*c7_b_GSCpwr_ref, 1U);
  _SFD_DATA_RANGE_CHECK(*c7_b_RSC_Pwr, 2U);
  _SFD_DATA_RANGE_CHECK((real_T)*c7_b_act_in, 3U);
  _SFD_DATA_RANGE_CHECK(*c7_b_term_in, 4U);
  _SFD_DATA_RANGE_CHECK(*c7_b_Vdc, 5U);
  _SFD_DATA_RANGE_CHECK(*c7_b_DC_ref, 6U);
}

static void initSimStructsc7_CART3_PMSG(SFc7_CART3_PMSGInstanceStruct
  *chartInstance)
{
  (void)chartInstance;
}

static void init_script_number_translation(uint32_T c7_machineNumber, uint32_T
  c7_chartNumber, uint32_T c7_instanceNumber)
{
  (void)c7_machineNumber;
  (void)c7_chartNumber;
  (void)c7_instanceNumber;
}

static const mxArray *c7_sf_marshallOut(void *chartInstanceVoid, void *c7_inData)
{
  const mxArray *c7_mxArrayOutData = NULL;
  real_T c7_u;
  const mxArray *c7_y = NULL;
  SFc7_CART3_PMSGInstanceStruct *chartInstance;
  chartInstance = (SFc7_CART3_PMSGInstanceStruct *)chartInstanceVoid;
  c7_mxArrayOutData = NULL;
  c7_u = *(real_T *)c7_inData;
  c7_y = NULL;
  if (!chartInstance->c7_Pre_mppt_pwr_not_empty) {
    sf_mex_assign(&c7_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0), false);
  } else {
    sf_mex_assign(&c7_y, sf_mex_create("y", &c7_u, 0, 0U, 0U, 0U, 0), false);
  }

  sf_mex_assign(&c7_mxArrayOutData, c7_y, false);
  return c7_mxArrayOutData;
}

static real_T c7_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct *chartInstance,
  const mxArray *c7_b_Pre_mppt_pwr, const char_T *c7_identifier)
{
  real_T c7_y;
  emlrtMsgIdentifier c7_thisId;
  c7_thisId.fIdentifier = c7_identifier;
  c7_thisId.fParent = NULL;
  c7_y = c7_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c7_b_Pre_mppt_pwr),
    &c7_thisId);
  sf_mex_destroy(&c7_b_Pre_mppt_pwr);
  return c7_y;
}

static real_T c7_b_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct *chartInstance,
  const mxArray *c7_u, const emlrtMsgIdentifier *c7_parentId)
{
  real_T c7_y;
  real_T c7_d0;
  if (mxIsEmpty(c7_u)) {
    chartInstance->c7_Pre_mppt_pwr_not_empty = false;
  } else {
    chartInstance->c7_Pre_mppt_pwr_not_empty = true;
    sf_mex_import(c7_parentId, sf_mex_dup(c7_u), &c7_d0, 1, 0, 0U, 0, 0U, 0);
    c7_y = c7_d0;
  }

  sf_mex_destroy(&c7_u);
  return c7_y;
}

static void c7_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c7_mxArrayInData, const char_T *c7_varName, void *c7_outData)
{
  const mxArray *c7_b_Pre_mppt_pwr;
  const char_T *c7_identifier;
  emlrtMsgIdentifier c7_thisId;
  real_T c7_y;
  SFc7_CART3_PMSGInstanceStruct *chartInstance;
  chartInstance = (SFc7_CART3_PMSGInstanceStruct *)chartInstanceVoid;
  c7_b_Pre_mppt_pwr = sf_mex_dup(c7_mxArrayInData);
  c7_identifier = c7_varName;
  c7_thisId.fIdentifier = c7_identifier;
  c7_thisId.fParent = NULL;
  c7_y = c7_b_emlrt_marshallIn(chartInstance, sf_mex_dup(c7_b_Pre_mppt_pwr),
    &c7_thisId);
  sf_mex_destroy(&c7_b_Pre_mppt_pwr);
  *(real_T *)c7_outData = c7_y;
  sf_mex_destroy(&c7_mxArrayInData);
}

static const mxArray *c7_b_sf_marshallOut(void *chartInstanceVoid, void
  *c7_inData)
{
  const mxArray *c7_mxArrayOutData = NULL;
  real_T c7_u;
  const mxArray *c7_y = NULL;
  SFc7_CART3_PMSGInstanceStruct *chartInstance;
  chartInstance = (SFc7_CART3_PMSGInstanceStruct *)chartInstanceVoid;
  c7_mxArrayOutData = NULL;
  c7_u = *(real_T *)c7_inData;
  c7_y = NULL;
  if (!chartInstance->c7_initFlag_not_empty) {
    sf_mex_assign(&c7_y, sf_mex_create("y", NULL, 0, 0U, 1U, 0U, 2, 0, 0), false);
  } else {
    sf_mex_assign(&c7_y, sf_mex_create("y", &c7_u, 0, 0U, 0U, 0U, 0), false);
  }

  sf_mex_assign(&c7_mxArrayOutData, c7_y, false);
  return c7_mxArrayOutData;
}

static real_T c7_c_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct *chartInstance,
  const mxArray *c7_b_initFlag, const char_T *c7_identifier)
{
  real_T c7_y;
  emlrtMsgIdentifier c7_thisId;
  c7_thisId.fIdentifier = c7_identifier;
  c7_thisId.fParent = NULL;
  c7_y = c7_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c7_b_initFlag),
    &c7_thisId);
  sf_mex_destroy(&c7_b_initFlag);
  return c7_y;
}

static real_T c7_d_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct *chartInstance,
  const mxArray *c7_u, const emlrtMsgIdentifier *c7_parentId)
{
  real_T c7_y;
  real_T c7_d1;
  if (mxIsEmpty(c7_u)) {
    chartInstance->c7_initFlag_not_empty = false;
  } else {
    chartInstance->c7_initFlag_not_empty = true;
    sf_mex_import(c7_parentId, sf_mex_dup(c7_u), &c7_d1, 1, 0, 0U, 0, 0U, 0);
    c7_y = c7_d1;
  }

  sf_mex_destroy(&c7_u);
  return c7_y;
}

static void c7_b_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c7_mxArrayInData, const char_T *c7_varName, void *c7_outData)
{
  const mxArray *c7_b_initFlag;
  const char_T *c7_identifier;
  emlrtMsgIdentifier c7_thisId;
  real_T c7_y;
  SFc7_CART3_PMSGInstanceStruct *chartInstance;
  chartInstance = (SFc7_CART3_PMSGInstanceStruct *)chartInstanceVoid;
  c7_b_initFlag = sf_mex_dup(c7_mxArrayInData);
  c7_identifier = c7_varName;
  c7_thisId.fIdentifier = c7_identifier;
  c7_thisId.fParent = NULL;
  c7_y = c7_d_emlrt_marshallIn(chartInstance, sf_mex_dup(c7_b_initFlag),
    &c7_thisId);
  sf_mex_destroy(&c7_b_initFlag);
  *(real_T *)c7_outData = c7_y;
  sf_mex_destroy(&c7_mxArrayInData);
}

static const mxArray *c7_c_sf_marshallOut(void *chartInstanceVoid, void
  *c7_inData)
{
  const mxArray *c7_mxArrayOutData = NULL;
  real_T c7_u;
  const mxArray *c7_y = NULL;
  SFc7_CART3_PMSGInstanceStruct *chartInstance;
  chartInstance = (SFc7_CART3_PMSGInstanceStruct *)chartInstanceVoid;
  c7_mxArrayOutData = NULL;
  c7_u = *(real_T *)c7_inData;
  c7_y = NULL;
  sf_mex_assign(&c7_y, sf_mex_create("y", &c7_u, 0, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c7_mxArrayOutData, c7_y, false);
  return c7_mxArrayOutData;
}

static real_T c7_e_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct *chartInstance,
  const mxArray *c7_DC_ref, const char_T *c7_identifier)
{
  real_T c7_y;
  emlrtMsgIdentifier c7_thisId;
  c7_thisId.fIdentifier = c7_identifier;
  c7_thisId.fParent = NULL;
  c7_y = c7_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c7_DC_ref), &c7_thisId);
  sf_mex_destroy(&c7_DC_ref);
  return c7_y;
}

static real_T c7_f_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct *chartInstance,
  const mxArray *c7_u, const emlrtMsgIdentifier *c7_parentId)
{
  real_T c7_y;
  real_T c7_d2;
  (void)chartInstance;
  sf_mex_import(c7_parentId, sf_mex_dup(c7_u), &c7_d2, 1, 0, 0U, 0, 0U, 0);
  c7_y = c7_d2;
  sf_mex_destroy(&c7_u);
  return c7_y;
}

static void c7_c_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c7_mxArrayInData, const char_T *c7_varName, void *c7_outData)
{
  const mxArray *c7_DC_ref;
  const char_T *c7_identifier;
  emlrtMsgIdentifier c7_thisId;
  real_T c7_y;
  SFc7_CART3_PMSGInstanceStruct *chartInstance;
  chartInstance = (SFc7_CART3_PMSGInstanceStruct *)chartInstanceVoid;
  c7_DC_ref = sf_mex_dup(c7_mxArrayInData);
  c7_identifier = c7_varName;
  c7_thisId.fIdentifier = c7_identifier;
  c7_thisId.fParent = NULL;
  c7_y = c7_f_emlrt_marshallIn(chartInstance, sf_mex_dup(c7_DC_ref), &c7_thisId);
  sf_mex_destroy(&c7_DC_ref);
  *(real_T *)c7_outData = c7_y;
  sf_mex_destroy(&c7_mxArrayInData);
}

static const mxArray *c7_d_sf_marshallOut(void *chartInstanceVoid, void
  *c7_inData)
{
  const mxArray *c7_mxArrayOutData = NULL;
  boolean_T c7_u;
  const mxArray *c7_y = NULL;
  SFc7_CART3_PMSGInstanceStruct *chartInstance;
  chartInstance = (SFc7_CART3_PMSGInstanceStruct *)chartInstanceVoid;
  c7_mxArrayOutData = NULL;
  c7_u = *(boolean_T *)c7_inData;
  c7_y = NULL;
  sf_mex_assign(&c7_y, sf_mex_create("y", &c7_u, 11, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c7_mxArrayOutData, c7_y, false);
  return c7_mxArrayOutData;
}

const mxArray *sf_c7_CART3_PMSG_get_eml_resolved_functions_info(void)
{
  const mxArray *c7_nameCaptureInfo = NULL;
  c7_nameCaptureInfo = NULL;
  sf_mex_assign(&c7_nameCaptureInfo, sf_mex_create("nameCaptureInfo", NULL, 0,
    0U, 1U, 0U, 2, 0, 1), false);
  return c7_nameCaptureInfo;
}

static const mxArray *c7_e_sf_marshallOut(void *chartInstanceVoid, void
  *c7_inData)
{
  const mxArray *c7_mxArrayOutData = NULL;
  int32_T c7_u;
  const mxArray *c7_y = NULL;
  SFc7_CART3_PMSGInstanceStruct *chartInstance;
  chartInstance = (SFc7_CART3_PMSGInstanceStruct *)chartInstanceVoid;
  c7_mxArrayOutData = NULL;
  c7_u = *(int32_T *)c7_inData;
  c7_y = NULL;
  sf_mex_assign(&c7_y, sf_mex_create("y", &c7_u, 6, 0U, 0U, 0U, 0), false);
  sf_mex_assign(&c7_mxArrayOutData, c7_y, false);
  return c7_mxArrayOutData;
}

static int32_T c7_g_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct
  *chartInstance, const mxArray *c7_u, const emlrtMsgIdentifier *c7_parentId)
{
  int32_T c7_y;
  int32_T c7_i0;
  (void)chartInstance;
  sf_mex_import(c7_parentId, sf_mex_dup(c7_u), &c7_i0, 1, 6, 0U, 0, 0U, 0);
  c7_y = c7_i0;
  sf_mex_destroy(&c7_u);
  return c7_y;
}

static void c7_d_sf_marshallIn(void *chartInstanceVoid, const mxArray
  *c7_mxArrayInData, const char_T *c7_varName, void *c7_outData)
{
  const mxArray *c7_b_sfEvent;
  const char_T *c7_identifier;
  emlrtMsgIdentifier c7_thisId;
  int32_T c7_y;
  SFc7_CART3_PMSGInstanceStruct *chartInstance;
  chartInstance = (SFc7_CART3_PMSGInstanceStruct *)chartInstanceVoid;
  c7_b_sfEvent = sf_mex_dup(c7_mxArrayInData);
  c7_identifier = c7_varName;
  c7_thisId.fIdentifier = c7_identifier;
  c7_thisId.fParent = NULL;
  c7_y = c7_g_emlrt_marshallIn(chartInstance, sf_mex_dup(c7_b_sfEvent),
    &c7_thisId);
  sf_mex_destroy(&c7_b_sfEvent);
  *(int32_T *)c7_outData = c7_y;
  sf_mex_destroy(&c7_mxArrayInData);
}

static uint8_T c7_h_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct
  *chartInstance, const mxArray *c7_b_is_active_c7_CART3_PMSG, const char_T
  *c7_identifier)
{
  uint8_T c7_y;
  emlrtMsgIdentifier c7_thisId;
  c7_thisId.fIdentifier = c7_identifier;
  c7_thisId.fParent = NULL;
  c7_y = c7_i_emlrt_marshallIn(chartInstance, sf_mex_dup
    (c7_b_is_active_c7_CART3_PMSG), &c7_thisId);
  sf_mex_destroy(&c7_b_is_active_c7_CART3_PMSG);
  return c7_y;
}

static uint8_T c7_i_emlrt_marshallIn(SFc7_CART3_PMSGInstanceStruct
  *chartInstance, const mxArray *c7_u, const emlrtMsgIdentifier *c7_parentId)
{
  uint8_T c7_y;
  uint8_T c7_u0;
  (void)chartInstance;
  sf_mex_import(c7_parentId, sf_mex_dup(c7_u), &c7_u0, 1, 3, 0U, 0, 0U, 0);
  c7_y = c7_u0;
  sf_mex_destroy(&c7_u);
  return c7_y;
}

static void init_dsm_address_info(SFc7_CART3_PMSGInstanceStruct *chartInstance)
{
  (void)chartInstance;
}

/* SFunction Glue Code */
#ifdef utFree
#undef utFree
#endif

#ifdef utMalloc
#undef utMalloc
#endif

#ifdef __cplusplus

extern "C" void *utMalloc(size_t size);
extern "C" void utFree(void*);

#else

extern void *utMalloc(size_t size);
extern void utFree(void*);

#endif

void sf_c7_CART3_PMSG_get_check_sum(mxArray *plhs[])
{
  ((real_T *)mxGetPr((plhs[0])))[0] = (real_T)(3698382473U);
  ((real_T *)mxGetPr((plhs[0])))[1] = (real_T)(133292534U);
  ((real_T *)mxGetPr((plhs[0])))[2] = (real_T)(2307801190U);
  ((real_T *)mxGetPr((plhs[0])))[3] = (real_T)(2469232602U);
}

mxArray *sf_c7_CART3_PMSG_get_autoinheritance_info(void)
{
  const char *autoinheritanceFields[] = { "checksum", "inputs", "parameters",
    "outputs", "locals" };

  mxArray *mxAutoinheritanceInfo = mxCreateStructMatrix(1,1,5,
    autoinheritanceFields);

  {
    mxArray *mxChecksum = mxCreateString("8l3wbj0pUCsoUnroJTcdVD");
    mxSetField(mxAutoinheritanceInfo,0,"checksum",mxChecksum);
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,5,3,dataFields);

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

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,2,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(1));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,2,"type",mxType);
    }

    mxSetField(mxData,2,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,3,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,3,"type",mxType);
    }

    mxSetField(mxData,3,"complexity",mxCreateDoubleScalar(0));

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,4,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,4,"type",mxType);
    }

    mxSetField(mxData,4,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"inputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"parameters",mxCreateDoubleMatrix(0,0,
                mxREAL));
  }

  {
    const char *dataFields[] = { "size", "type", "complexity" };

    mxArray *mxData = mxCreateStructMatrix(1,2,3,dataFields);

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

    {
      mxArray *mxSize = mxCreateDoubleMatrix(1,2,mxREAL);
      double *pr = mxGetPr(mxSize);
      pr[0] = (double)(1);
      pr[1] = (double)(1);
      mxSetField(mxData,1,"size",mxSize);
    }

    {
      const char *typeFields[] = { "base", "fixpt" };

      mxArray *mxType = mxCreateStructMatrix(1,1,2,typeFields);
      mxSetField(mxType,0,"base",mxCreateDoubleScalar(10));
      mxSetField(mxType,0,"fixpt",mxCreateDoubleMatrix(0,0,mxREAL));
      mxSetField(mxData,1,"type",mxType);
    }

    mxSetField(mxData,1,"complexity",mxCreateDoubleScalar(0));
    mxSetField(mxAutoinheritanceInfo,0,"outputs",mxData);
  }

  {
    mxSetField(mxAutoinheritanceInfo,0,"locals",mxCreateDoubleMatrix(0,0,mxREAL));
  }

  return(mxAutoinheritanceInfo);
}

mxArray *sf_c7_CART3_PMSG_third_party_uses_info(void)
{
  mxArray * mxcell3p = mxCreateCellMatrix(1,0);
  return(mxcell3p);
}

mxArray *sf_c7_CART3_PMSG_updateBuildInfo_args_info(void)
{
  mxArray *mxBIArgs = mxCreateCellMatrix(1,0);
  return mxBIArgs;
}

static const mxArray *sf_get_sim_state_info_c7_CART3_PMSG(void)
{
  const char *infoFields[] = { "chartChecksum", "varInfo" };

  mxArray *mxInfo = mxCreateStructMatrix(1, 1, 2, infoFields);
  const char *infoEncStr[] = {
    "100 S1x5'type','srcId','name','auxInfo'{{M[1],M[11],T\"DC_ref\",},{M[1],M[5],T\"GSCpwr_ref\",},{M[4],M[0],T\"Pre_mppt_pwr\",S'l','i','p'{{M1x2[159 171],M[0],}}},{M[4],M[0],T\"initFlag\",S'l','i','p'{{M1x2[94 102],M[0],}}},{M[8],M[0],T\"is_active_c7_CART3_PMSG\",}}"
  };

  mxArray *mxVarInfo = sf_mex_decode_encoded_mx_struct_array(infoEncStr, 5, 10);
  mxArray *mxChecksum = mxCreateDoubleMatrix(1, 4, mxREAL);
  sf_c7_CART3_PMSG_get_check_sum(&mxChecksum);
  mxSetField(mxInfo, 0, infoFields[0], mxChecksum);
  mxSetField(mxInfo, 0, infoFields[1], mxVarInfo);
  return mxInfo;
}

static void chart_debug_initialization(SimStruct *S, unsigned int
  fullDebuggerInitialization)
{
  if (!sim_mode_is_rtw_gen(S)) {
    SFc7_CART3_PMSGInstanceStruct *chartInstance;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    chartInstance = (SFc7_CART3_PMSGInstanceStruct *) chartInfo->chartInstance;
    if (ssIsFirstInitCond(S) && fullDebuggerInitialization==1) {
      /* do this only if simulation is starting */
      {
        unsigned int chartAlreadyPresent;
        chartAlreadyPresent = sf_debug_initialize_chart
          (sfGlobalDebugInstanceStruct,
           _CART3_PMSGMachineNumber_,
           7,
           1,
           1,
           0,
           7,
           0,
           0,
           0,
           0,
           0,
           &(chartInstance->chartNumber),
           &(chartInstance->instanceNumber),
           (void *)S);

        /* Each instance must initialize ist own list of scripts */
        init_script_number_translation(_CART3_PMSGMachineNumber_,
          chartInstance->chartNumber,chartInstance->instanceNumber);
        if (chartAlreadyPresent==0) {
          /* this is the first instance */
          sf_debug_set_chart_disable_implicit_casting
            (sfGlobalDebugInstanceStruct,_CART3_PMSGMachineNumber_,
             chartInstance->chartNumber,1);
          sf_debug_set_chart_event_thresholds(sfGlobalDebugInstanceStruct,
            _CART3_PMSGMachineNumber_,
            chartInstance->chartNumber,
            0,
            0,
            0);
          _SFD_SET_DATA_PROPS(0,1,1,0,"DCctrl_ref");
          _SFD_SET_DATA_PROPS(1,2,0,1,"GSCpwr_ref");
          _SFD_SET_DATA_PROPS(2,1,1,0,"RSC_Pwr");
          _SFD_SET_DATA_PROPS(3,1,1,0,"act_in");
          _SFD_SET_DATA_PROPS(4,1,1,0,"term_in");
          _SFD_SET_DATA_PROPS(5,1,1,0,"Vdc");
          _SFD_SET_DATA_PROPS(6,2,0,1,"DC_ref");
          _SFD_STATE_INFO(0,0,2);
          _SFD_CH_SUBSTATE_COUNT(0);
          _SFD_CH_SUBSTATE_DECOMP(0);
        }

        _SFD_CV_INIT_CHART(0,0,0,0);

        {
          _SFD_CV_INIT_STATE(0,0,0,0,0,0,NULL,NULL);
        }

        _SFD_CV_INIT_TRANS(0,0,NULL,NULL,0,NULL);

        /* Initialization of MATLAB Function Model Coverage */
        _SFD_CV_INIT_EML(0,1,1,3,0,0,0,0,0,4,2);
        _SFD_CV_INIT_EML_FCN(0,0,"eML_blk_kernel",0,-1,458);
        _SFD_CV_INIT_EML_IF(0,1,0,200,220,-1,277);
        _SFD_CV_INIT_EML_IF(0,1,1,316,349,-1,379);
        _SFD_CV_INIT_EML_IF(0,1,2,381,421,-1,454);

        {
          static int condStart[] = { 320, 333 };

          static int condEnd[] = { 329, 348 };

          static int pfixExpr[] = { 0, 1, -3 };

          _SFD_CV_INIT_EML_MCDC(0,1,0,319,349,2,0,&(condStart[0]),&(condEnd[0]),
                                3,&(pfixExpr[0]));
        }

        {
          static int condStart[] = { 385, 399 };

          static int condEnd[] = { 395, 420 };

          static int pfixExpr[] = { 0, 1, -3 };

          _SFD_CV_INIT_EML_MCDC(0,1,1,384,421,2,2,&(condStart[0]),&(condEnd[0]),
                                3,&(pfixExpr[0]));
        }

        _SFD_SET_DATA_COMPILED_PROPS(0,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c7_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(1,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c7_c_sf_marshallOut,(MexInFcnForType)c7_c_sf_marshallIn);
        _SFD_SET_DATA_COMPILED_PROPS(2,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c7_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(3,SF_UINT8,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c7_d_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(4,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c7_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(5,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c7_c_sf_marshallOut,(MexInFcnForType)NULL);
        _SFD_SET_DATA_COMPILED_PROPS(6,SF_DOUBLE,0,NULL,0,0,0,0.0,1.0,0,0,
          (MexFcnForType)c7_c_sf_marshallOut,(MexInFcnForType)c7_c_sf_marshallIn);

        {
          real_T *c7_DCctrl_ref;
          real_T *c7_GSCpwr_ref;
          real_T *c7_RSC_Pwr;
          boolean_T *c7_act_in;
          real_T *c7_term_in;
          real_T *c7_Vdc;
          real_T *c7_DC_ref;
          c7_DC_ref = (real_T *)ssGetOutputPortSignal(chartInstance->S, 2);
          c7_Vdc = (real_T *)ssGetInputPortSignal(chartInstance->S, 4);
          c7_term_in = (real_T *)ssGetInputPortSignal(chartInstance->S, 3);
          c7_act_in = (boolean_T *)ssGetInputPortSignal(chartInstance->S, 2);
          c7_RSC_Pwr = (real_T *)ssGetInputPortSignal(chartInstance->S, 1);
          c7_GSCpwr_ref = (real_T *)ssGetOutputPortSignal(chartInstance->S, 1);
          c7_DCctrl_ref = (real_T *)ssGetInputPortSignal(chartInstance->S, 0);
          _SFD_SET_DATA_VALUE_PTR(0U, c7_DCctrl_ref);
          _SFD_SET_DATA_VALUE_PTR(1U, c7_GSCpwr_ref);
          _SFD_SET_DATA_VALUE_PTR(2U, c7_RSC_Pwr);
          _SFD_SET_DATA_VALUE_PTR(3U, c7_act_in);
          _SFD_SET_DATA_VALUE_PTR(4U, c7_term_in);
          _SFD_SET_DATA_VALUE_PTR(5U, c7_Vdc);
          _SFD_SET_DATA_VALUE_PTR(6U, c7_DC_ref);
        }
      }
    } else {
      sf_debug_reset_current_state_configuration(sfGlobalDebugInstanceStruct,
        _CART3_PMSGMachineNumber_,chartInstance->chartNumber,
        chartInstance->instanceNumber);
    }
  }
}

static const char* sf_get_instance_specialization(void)
{
  return "rt3nmgq5AIntJCVa98exhF";
}

static void sf_opaque_initialize_c7_CART3_PMSG(void *chartInstanceVar)
{
  chart_debug_initialization(((SFc7_CART3_PMSGInstanceStruct*) chartInstanceVar
    )->S,0);
  initialize_params_c7_CART3_PMSG((SFc7_CART3_PMSGInstanceStruct*)
    chartInstanceVar);
  initialize_c7_CART3_PMSG((SFc7_CART3_PMSGInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_enable_c7_CART3_PMSG(void *chartInstanceVar)
{
  enable_c7_CART3_PMSG((SFc7_CART3_PMSGInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_disable_c7_CART3_PMSG(void *chartInstanceVar)
{
  disable_c7_CART3_PMSG((SFc7_CART3_PMSGInstanceStruct*) chartInstanceVar);
}

static void sf_opaque_gateway_c7_CART3_PMSG(void *chartInstanceVar)
{
  sf_gateway_c7_CART3_PMSG((SFc7_CART3_PMSGInstanceStruct*) chartInstanceVar);
}

extern const mxArray* sf_internal_get_sim_state_c7_CART3_PMSG(SimStruct* S)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[4];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_raw2high");
  prhs[1] = mxCreateDoubleScalar(ssGetSFuncBlockHandle(S));
  prhs[2] = (mxArray*) get_sim_state_c7_CART3_PMSG
    ((SFc7_CART3_PMSGInstanceStruct*)chartInfo->chartInstance);/* raw sim ctx */
  prhs[3] = (mxArray*) sf_get_sim_state_info_c7_CART3_PMSG();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 4, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  mxDestroyArray(prhs[3]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_raw2high'.\n");
  }

  return plhs[0];
}

extern void sf_internal_set_sim_state_c7_CART3_PMSG(SimStruct* S, const mxArray *
  st)
{
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
  ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
  mxArray *plhs[1] = { NULL };

  mxArray *prhs[3];
  int mxError = 0;
  prhs[0] = mxCreateString("chart_simctx_high2raw");
  prhs[1] = mxDuplicateArray(st);      /* high level simctx */
  prhs[2] = (mxArray*) sf_get_sim_state_info_c7_CART3_PMSG();/* state var info */
  mxError = sf_mex_call_matlab(1, plhs, 3, prhs, "sfprivate");
  mxDestroyArray(prhs[0]);
  mxDestroyArray(prhs[1]);
  mxDestroyArray(prhs[2]);
  if (mxError || plhs[0] == NULL) {
    sf_mex_error_message("Stateflow Internal Error: \nError calling 'chart_simctx_high2raw'.\n");
  }

  set_sim_state_c7_CART3_PMSG((SFc7_CART3_PMSGInstanceStruct*)
    chartInfo->chartInstance, mxDuplicateArray(plhs[0]));
  mxDestroyArray(plhs[0]);
}

static const mxArray* sf_opaque_get_sim_state_c7_CART3_PMSG(SimStruct* S)
{
  return sf_internal_get_sim_state_c7_CART3_PMSG(S);
}

static void sf_opaque_set_sim_state_c7_CART3_PMSG(SimStruct* S, const mxArray
  *st)
{
  sf_internal_set_sim_state_c7_CART3_PMSG(S, st);
}

static void sf_opaque_terminate_c7_CART3_PMSG(void *chartInstanceVar)
{
  if (chartInstanceVar!=NULL) {
    SimStruct *S = ((SFc7_CART3_PMSGInstanceStruct*) chartInstanceVar)->S;
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
      sf_clear_rtw_identifier(S);
      unload_CART3_PMSG_optimization_info();
    }

    finalize_c7_CART3_PMSG((SFc7_CART3_PMSGInstanceStruct*) chartInstanceVar);
    utFree((void *)chartInstanceVar);
    if (crtInfo != NULL) {
      utFree((void *)crtInfo);
    }

    ssSetUserData(S,NULL);
  }
}

static void sf_opaque_init_subchart_simstructs(void *chartInstanceVar)
{
  initSimStructsc7_CART3_PMSG((SFc7_CART3_PMSGInstanceStruct*) chartInstanceVar);
}

extern unsigned int sf_machine_global_initializer_called(void);
static void mdlProcessParameters_c7_CART3_PMSG(SimStruct *S)
{
  int i;
  for (i=0;i<ssGetNumRunTimeParams(S);i++) {
    if (ssGetSFcnParamTunable(S,i)) {
      ssUpdateDlgParamAsRunTimeParam(S,i);
    }
  }

  if (sf_machine_global_initializer_called()) {
    ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)(ssGetUserData(S));
    ChartInfoStruct * chartInfo = (ChartInfoStruct *)(crtInfo->instanceInfo);
    initialize_params_c7_CART3_PMSG((SFc7_CART3_PMSGInstanceStruct*)
      (chartInfo->chartInstance));
  }
}

static void mdlSetWorkWidths_c7_CART3_PMSG(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S) || sim_mode_is_external(S)) {
    mxArray *infoStruct = load_CART3_PMSG_optimization_info();
    int_T chartIsInlinable =
      (int_T)sf_is_chart_inlinable(sf_get_instance_specialization(),infoStruct,7);
    ssSetStateflowIsInlinable(S,chartIsInlinable);
    ssSetRTWCG(S,sf_rtw_info_uint_prop(sf_get_instance_specialization(),
                infoStruct,7,"RTWCG"));
    ssSetEnableFcnIsTrivial(S,1);
    ssSetDisableFcnIsTrivial(S,1);
    ssSetNotMultipleInlinable(S,sf_rtw_info_uint_prop
      (sf_get_instance_specialization(),infoStruct,7,
       "gatewayCannotBeInlinedMultipleTimes"));
    sf_update_buildInfo(sf_get_instance_specialization(),infoStruct,7);
    if (chartIsInlinable) {
      ssSetInputPortOptimOpts(S, 0, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 1, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 2, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 3, SS_REUSABLE_AND_LOCAL);
      ssSetInputPortOptimOpts(S, 4, SS_REUSABLE_AND_LOCAL);
      sf_mark_chart_expressionable_inputs(S,sf_get_instance_specialization(),
        infoStruct,7,5);
      sf_mark_chart_reusable_outputs(S,sf_get_instance_specialization(),
        infoStruct,7,2);
    }

    {
      unsigned int outPortIdx;
      for (outPortIdx=1; outPortIdx<=2; ++outPortIdx) {
        ssSetOutputPortOptimizeInIR(S, outPortIdx, 1U);
      }
    }

    {
      unsigned int inPortIdx;
      for (inPortIdx=0; inPortIdx < 5; ++inPortIdx) {
        ssSetInputPortOptimizeInIR(S, inPortIdx, 1U);
      }
    }

    sf_set_rtw_dwork_info(S,sf_get_instance_specialization(),infoStruct,7);
    ssSetHasSubFunctions(S,!(chartIsInlinable));
  } else {
  }

  ssSetOptions(S,ssGetOptions(S)|SS_OPTION_WORKS_WITH_CODE_REUSE);
  ssSetChecksum0(S,(2476400648U));
  ssSetChecksum1(S,(3313934409U));
  ssSetChecksum2(S,(789651747U));
  ssSetChecksum3(S,(2018061278U));
  ssSetmdlDerivatives(S, NULL);
  ssSetExplicitFCSSCtrl(S,1);
  ssSupportsMultipleExecInstances(S,1);
}

static void mdlRTW_c7_CART3_PMSG(SimStruct *S)
{
  if (sim_mode_is_rtw_gen(S)) {
    ssWriteRTWStrParam(S, "StateflowChartType", "Embedded MATLAB");
  }
}

static void mdlStart_c7_CART3_PMSG(SimStruct *S)
{
  SFc7_CART3_PMSGInstanceStruct *chartInstance;
  ChartRunTimeInfo * crtInfo = (ChartRunTimeInfo *)utMalloc(sizeof
    (ChartRunTimeInfo));
  chartInstance = (SFc7_CART3_PMSGInstanceStruct *)utMalloc(sizeof
    (SFc7_CART3_PMSGInstanceStruct));
  memset(chartInstance, 0, sizeof(SFc7_CART3_PMSGInstanceStruct));
  if (chartInstance==NULL) {
    sf_mex_error_message("Could not allocate memory for chart instance.");
  }

  chartInstance->chartInfo.chartInstance = chartInstance;
  chartInstance->chartInfo.isEMLChart = 1;
  chartInstance->chartInfo.chartInitialized = 0;
  chartInstance->chartInfo.sFunctionGateway = sf_opaque_gateway_c7_CART3_PMSG;
  chartInstance->chartInfo.initializeChart = sf_opaque_initialize_c7_CART3_PMSG;
  chartInstance->chartInfo.terminateChart = sf_opaque_terminate_c7_CART3_PMSG;
  chartInstance->chartInfo.enableChart = sf_opaque_enable_c7_CART3_PMSG;
  chartInstance->chartInfo.disableChart = sf_opaque_disable_c7_CART3_PMSG;
  chartInstance->chartInfo.getSimState = sf_opaque_get_sim_state_c7_CART3_PMSG;
  chartInstance->chartInfo.setSimState = sf_opaque_set_sim_state_c7_CART3_PMSG;
  chartInstance->chartInfo.getSimStateInfo = sf_get_sim_state_info_c7_CART3_PMSG;
  chartInstance->chartInfo.zeroCrossings = NULL;
  chartInstance->chartInfo.outputs = NULL;
  chartInstance->chartInfo.derivatives = NULL;
  chartInstance->chartInfo.mdlRTW = mdlRTW_c7_CART3_PMSG;
  chartInstance->chartInfo.mdlStart = mdlStart_c7_CART3_PMSG;
  chartInstance->chartInfo.mdlSetWorkWidths = mdlSetWorkWidths_c7_CART3_PMSG;
  chartInstance->chartInfo.extModeExec = NULL;
  chartInstance->chartInfo.restoreLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.restoreBeforeLastMajorStepConfiguration = NULL;
  chartInstance->chartInfo.storeCurrentConfiguration = NULL;
  chartInstance->chartInfo.debugInstance = sfGlobalDebugInstanceStruct;
  chartInstance->S = S;
  crtInfo->instanceInfo = (&(chartInstance->chartInfo));
  crtInfo->isJITEnabled = false;
  ssSetUserData(S,(void *)(crtInfo));  /* register the chart instance with simstruct */
  init_dsm_address_info(chartInstance);
  if (!sim_mode_is_rtw_gen(S)) {
  }

  sf_opaque_init_subchart_simstructs(chartInstance->chartInfo.chartInstance);
  chart_debug_initialization(S,1);
}

void c7_CART3_PMSG_method_dispatcher(SimStruct *S, int_T method, void *data)
{
  switch (method) {
   case SS_CALL_MDL_START:
    mdlStart_c7_CART3_PMSG(S);
    break;

   case SS_CALL_MDL_SET_WORK_WIDTHS:
    mdlSetWorkWidths_c7_CART3_PMSG(S);
    break;

   case SS_CALL_MDL_PROCESS_PARAMETERS:
    mdlProcessParameters_c7_CART3_PMSG(S);
    break;

   default:
    /* Unhandled method */
    sf_mex_error_message("Stateflow Internal Error:\n"
                         "Error calling c7_CART3_PMSG_method_dispatcher.\n"
                         "Can't handle method %d.\n", method);
    break;
  }
}
