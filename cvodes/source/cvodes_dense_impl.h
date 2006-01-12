/*
 * -----------------------------------------------------------------
 * $Revision: 1.3 $
 * $Date: 2006-01-12 22:53:38 $
 * ----------------------------------------------------------------- 
 * Programmer(s): Radu Serban @ LLNL
 * -----------------------------------------------------------------
 * Copyright (c) 2005, The Regents of the University of California.
 * Produced at the Lawrence Livermore National Laboratory.
 * All rights reserved.
 * For details, see sundials/cvodes/LICENSE.
 * -----------------------------------------------------------------
 * Implementation header file for the dense linear solver, CVDENSE.
 * -----------------------------------------------------------------
 */

#ifndef _CVSDENSE_IMPL_H
#define _CVSDENSE_IMPL_H

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

#include "cvodes_dense.h"

  /*
   * -----------------------------------------------------------------
   * Types : CVDenseMemRec, CVDenseMem                             
   * -----------------------------------------------------------------
   * The type CVDenseMem is pointer to a CVDenseMemRec.
   * This structure contains CVDense solver-specific data.
   *
   * CVDense attaches such a structure to the lmem field of CVodeMem
   * -----------------------------------------------------------------
   */

  typedef struct {

    long int d_n;       /* problem dimension                      */

    CVDenseJacFn d_jac; /* jac = Jacobian routine to be called    */

    DenseMat d_M;       /* M = I - gamma J, gamma = h / l1        */
  
    long int *d_pivots; /* pivots = pivot array for PM = LU   */
  
    DenseMat d_savedJ;  /* savedJ = old Jacobian                  */
  
    long int  d_nstlj;  /* nstlj = nst at last Jacobian eval.     */
  
    long int d_nje;     /* nje = no. of calls to jac              */

    long int d_nfeD;    /* nfeD = no. of calls to f due to
                           difference quotient approximation of J */
  
    void *d_J_data;     /* J_data is passed to jac                */

    int d_last_flag;    /* last error return flag */
  
  } CVDenseMemRec, *CVDenseMem;


  /*
   * -----------------------------------------------------------------
   * Types : CVDenseMemRecB, CVDenseMemB       
   * -----------------------------------------------------------------
   * CVDenseB attaches such a structure to the lmemB filed of CVadjMem
   * -----------------------------------------------------------------
   */

  typedef struct {

    CVDenseJacFnB d_djacB;
    void *d_jac_dataB;

  } CVDenseMemRecB, *CVDenseMemB;

  /*
   * -----------------------------------------------------------------
   * Error Messages 
   * -----------------------------------------------------------------
   */

#define _CVDENSE_         "CVDense-- "
#define MSGDS_CVMEM_NULL  _CVDENSE_ "Integrator memory is NULL.\n\n"
#define MSGDS_BAD_NVECTOR _CVDENSE_ "A required vector operation is not implemented.\n\n"
#define MSGDS_MEM_FAIL    _CVDENSE_ "A memory request failed.\n\n"

#define MSGDS_SETGET_CVMEM_NULL "CVDenseSet*/CVDenseGet*-- Integrator memory is NULL.\n\n"

#define MSGDS_SETGET_LMEM_NULL "CVDenseSet*/CVDenseGet*-- cvdense memory is NULL.\n\n"

#ifdef __cplusplus
}
#endif

#endif
