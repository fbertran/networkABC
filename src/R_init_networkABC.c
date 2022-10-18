//
// 2019-02-19. Author F. Bertrand <frederic.bertrand@utt.fr>
// Copyright (c) Universite de Strasbourg and CNRS
//

#include <stdlib.h> // for NULL
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include "abc_R.h" // for NULL
#include "network_random_R.h" // for NULL

#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}

static R_NativePrimitiveArgType C_abc_t[] = {
  REALSXP, INTSXP, INTSXP, INTSXP, REALSXP, REALSXP, INTSXP, INTSXP, INTSXP, INTSXP, REALSXP, REALSXP, REALSXP, INTSXP
};

static R_NativePrimitiveArgType C_run_t[] = {
  INTSXP, REALSXP, INTSXP
};

static const R_CMethodDef cMethods[] = {
  {"C_abc", (DL_FUNC) &C_abc, 14, C_abc_t},
  {"C_run", (DL_FUNC) &C_run, 3, C_run_t},
  {NULL, NULL, 0, NULL}
};

void R_init_networkABC(DllInfo * info)
{
  R_registerRoutines(info, cMethods, NULL, NULL, NULL);
  R_useDynamicSymbols(info, FALSE);
  R_forceSymbols(info, TRUE);
}


