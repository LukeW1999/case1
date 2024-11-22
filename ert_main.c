/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: ert_main.c
 *
 * Code generated for Simulink model 'triplex_12B'.
 *
 * Model version                  : 1.60
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Fri Nov  1 12:49:54 2024
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM 10
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include <stddef.h>
#include <stdio.h>            /* This example main program uses printf/fflush */
#include "triplex_12B.h"               /* Model header file */
#include <assert.h>
#include <math.h>       /* Add this for fabs, fmin, fmax */

// Add ESBMC function declarations
double nondet_double(void);
int nondet_int(void);

/*
 * Associating rt_OneStep with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function rt_OneStep is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from rt_OneStep.
 */
void rt_OneStep(void);
void rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model */
  triplex_12B_step();

  /* Get model outputs here */

  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

/*
 * The example main function illustrates what is required by your
 * application code to initialize, execute, and terminate the generated code.
 * Attaching rt_OneStep to a real-time clock is target specific. This example
 * illustrates how you do this relative to initializing the model.
 */

// Track previous values for temporal properties
typedef struct {
    real_T prev_sel_val;
    int32_T prev_FC;
    boolean_T miscompare_detected;
    int32_T miscompare_duration;
} MonitorState;

MonitorState monitor_state = {0.0, 0, false, 0};

// Helper functions for requirements checking
boolean_T is_miscompare(real_T ia, real_T ib, real_T ic, real_T Tlevel) {
    return (fabs(ia - ib) > Tlevel) || 
           (fabs(ib - ic) > Tlevel) || 
           (fabs(ia - ic) > Tlevel);
}

boolean_T is_mid_value(int sel_val, int ia, int ib, int ic) {
    int min_val = (ia < ib) ? ((ia < ic) ? ia : ic) : ((ib < ic) ? ib : ic);
    int max_val = (ia > ib) ? ((ia > ic) ? ia : ic) : ((ib > ic) ? ib : ic);
    int mid_val;
    
    // Find the middle value
    if (ia != min_val && ia != max_val) mid_val = ia;
    else if (ib != min_val && ib != max_val) mid_val = ib;
    else mid_val = ic;
    
    return sel_val == mid_val;
}

real_T compute_gca(real_T ia, real_T ib, real_T ic, int32_T FC) {
    if (FC == 1) return (ia + ib) / 2.0;
    if (FC == 2) return (ia + ic) / 2.0;
    if (FC == 4) return (ib + ic) / 2.0;
    return 0.0;
}

void check_requirements(void) {    
  /*
    // RM-001: Miscompare detection and failure reporting
    if (is_miscompare(rtU.ia, rtU.ib, rtU.ic, rtU.Tlevel)) {
        if (!monitor_state.miscompare_detected) {
            monitor_state.miscompare_detected = true;
            monitor_state.miscompare_duration = 0;
        }
        monitor_state.miscompare_duration++;
        
        if (monitor_state.miscompare_duration > rtU.PClimit) {
            __ESBMC_assert(rtY.FC != 0, "RM-001: Failure should be reported after persistent miscompare");
        }
    } else {
        monitor_state.miscompare_detected = false;
        monitor_state.miscompare_duration = 0;
    }

    // RM-002: Mid-value selection in no-fail state

*/
    if (rtY.FC == 0) {
        __ESBMC_assert(is_mid_value(rtY.sel_val, rtU.ia, rtU.ib, rtU.ic), 
                       "RM-002: Selected value must be mid-value in no-fail state");
    }

    /*
    // RM-003: Good channel average in single-fail state
    if (rtY.FC == 1 || rtY.FC == 2 || rtY.FC == 4) {
        real_T expected_gca = compute_gca(rtU.ia, rtU.ib, rtU.ic, rtY.FC);
        __ESBMC_assert(fabs(rtY.sel_val - expected_gca) < 0.001, 
                       "RM-003: Selected value must be good channel average in single-fail state");
    }

    // RM-004: Value preservation during second failure in progress
    if (rtY.PC > 0 && rtY.FC != 0) {
        __ESBMC_assert(rtY.sel_val == monitor_state.prev_sel_val,
                       "RM-004: Selected value must remain unchanged during second failure in progress");
    }
    */  
    // Update previous values
    monitor_state.prev_sel_val = rtY.sel_val;
    monitor_state.prev_FC = rtY.FC;
}

int_T main(int_T argc, const char *argv[]) {
    (void)(argc);
    (void)(argv);

    // Initialize model
    triplex_12B_initialize();

    // Initialize inputs with integers instead of doubles
    rtU.ia = nondet_int();
    rtU.ib = nondet_int();
    rtU.ic = nondet_int();
    rtU.Tlevel = nondet_int();
    rtU.PClimit = nondet_int();

    // Modify bounds for integer inputs
    __ESBMC_assume(rtU.ia >= -1000 && rtU.ia <= 1000);
    __ESBMC_assume(rtU.ib >= -1000 && rtU.ib <= 1000);
    __ESBMC_assume(rtU.ic >= -1000 && rtU.ic <= 1000);
    __ESBMC_assume(rtU.Tlevel > 0 && rtU.Tlevel <= 100);
    __ESBMC_assume(rtU.PClimit > 0 && rtU.PClimit < 1000);

    // Run for a bounded number of steps
    for (int i = 0; i < 10; i++) {
        rt_OneStep();
        check_requirements();
    }

    return 0;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
