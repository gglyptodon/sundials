/*
 * -----------------------------------------------------------------
 * $Revision$
 * $Date$
 * ----------------------------------------------------------------- 
 * Programmer: Radu Serban @ LLNL
 * -----------------------------------------------------------------
 * LLNS Copyright Start
 * Copyright (c) 2014, Lawrence Livermore National Security
 * This work was performed under the auspices of the U.S. Department 
 * of Energy by Lawrence Livermore National Laboratory in part under 
 * Contract W-7405-Eng-48 and in part under Contract DE-AC52-07NA27344.
 * Produced at the Lawrence Livermore National Laboratory.
 * All rights reserved.
 * For details, see the LICENSE file.
 * LLNS Copyright End
 * -----------------------------------------------------------------
 * Fortran/C interface routines for KINSOL/KINBAND, for the case
 * of a user-supplied Jacobian approximation routine.
 * -----------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

#include "fkinsol.h"     /* standard interfaces and global vars.*/
#include "kinsol_impl.h" /* definition of KINMem type           */

#include <kinsol/kinsol_lapack.h>

/*
 * ----------------------------------------------------------------
 * prototypes of the user-supplied fortran routines
 * ----------------------------------------------------------------
 */

#ifdef __cplusplus  /* wrapper to enable C++ usage */
extern "C" {
#endif

extern void FK_BJAC(sunindextype*, sunindextype*, sunindextype*, sunindextype*,
                    realtype*, realtype*,
                    realtype*,
                    realtype*, realtype*, int*);

#ifdef __cplusplus
}
#endif

/*
 * ----------------------------------------------------------------
 * Function : FKIN_LAPACKBANDSETJAC
 * ----------------------------------------------------------------
 */

void FKIN_LAPACKBANDSETJAC(int *flag, int *ier)
{
  if (*flag == 0) {
    *ier = KINDlsSetBandJacFn(KIN_kinmem, NULL);
  }
  else {
    *ier = KINDlsSetBandJacFn(KIN_kinmem, FKINLapackBandJac);
  }

  return;
}

/*
 * ----------------------------------------------------------------
 * Function : FKINLapackBandJac
 * ----------------------------------------------------------------
 * C function FKINLapackBandJac interfaces between KINSOL and a
 * Fortran subroutine FKBJAC for solution of a linear system with
 * band Jacobian approximation using Lapack functions.
 * Addresses are passed to FKBJAC for the banded Jacobian and 
 * vector data.
 * Auxiliary data is assumed to be communicated by common blocks.
 * ----------------------------------------------------------------
 */

int FKINLapackBandJac(sunindextype N, sunindextype mupper, sunindextype mlower,
                      N_Vector uu, N_Vector fval, 
                      DlsMat J, void *user_data,
                      N_Vector vtemp1, N_Vector vtemp2)
{
  realtype *uu_data, *fval_data, *jacdata, *v1_data, *v2_data;
  sunindextype eband;
  int ier;

  /* Initialize all pointers to NULL */
  uu_data = fval_data = jacdata = v1_data = v2_data = NULL;

  /* NOTE: The user-supplied routine should set ier to an
     appropriate value, but we preset the value to zero
     (meaning SUCCESS) so the user need only reset the
     value if an error occurred */
  ier = 0;

  /* Get pointers to vector data */
  uu_data   = N_VGetArrayPointer(uu);
  fval_data = N_VGetArrayPointer(fval);
  v1_data   = N_VGetArrayPointer(vtemp1);
  v2_data   = N_VGetArrayPointer(vtemp2);

  eband = (J->s_mu) + mlower + 1;
  jacdata = BAND_COL(J,0) - mupper;

  /* Call user-supplied routine */
  FK_BJAC(&N, &mupper, &mlower, &eband,
          uu_data, fval_data, 
          jacdata,
          v1_data, v2_data, &ier);

  return(ier);
}


