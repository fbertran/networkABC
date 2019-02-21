//
// 2019-02-19. Author F. Bertrand <fbertran@math.unistra.fr>
// Copyright (c) Universite de Strasbourg and CNRS
//

#include <stdlib.h> // for NULL
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include "abc_R.h" // for NULL
#include "network_random_R.h" // for NULL

#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}

static R_NativePrimitiveArgType abc_t[] = {
  REALSXP, INTSXP, INTSXP, INTSXP, REALSXP, REALSXP, INTSXP, INTSXP, INTSXP, INTSXP, REALSXP, REALSXP, REALSXP, INTSXP
};

static R_NativePrimitiveArgType run_t[] = {
  INTSXP, REALSXP, INTSXP
};

static const R_CMethodDef cMethods[] = {
  {"abc", (DL_FUNC) &abc, 14, abc_t},
  {"run", (DL_FUNC) &run, 3, run_t},
  {NULL, NULL, 0, NULL}
};

void R_init_networkABC(DllInfo * info)
{
  R_registerRoutines(info, cMethods, NULL, NULL, NULL);
  R_useDynamicSymbols(info, FALSE);
//  R_forceSymbols(info, TRUE);
}


