#include <gsl_math.h>
#include <gsl_cblas.h>
#include "cblas.h"
#include "error_cblas_l3.h"

void
cblas_zsymm (const enum CBLAS_ORDER Order, const enum CBLAS_SIDE Side,
             const enum CBLAS_UPLO Uplo, const int M, const int N,
             const void *alpha, const void *A, const int lda, const void *B,
             const int ldb, const void *beta, void *C, const int ldc)
{
#define BASE double
#include "source_symm_c.h"
#undef BASE
}
