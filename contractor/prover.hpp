#pragma once
#include "prover_common.ipp"

#include "prover_clever.ipp"
#include "prover_clever_lazy.ipp"
#include "prover_simple.ipp"

//////////////////////////////////////////////////////////////////////////////
// default prover
//////////////////////////////////////////////////////////////////////////////
#ifndef CONTRACTOR_DEFAULT_PROVER
#define CONTRACTOR_DEFAULT_PROVER contractor::prove_clever_lazy
#endif

//////////////////////////////////////////////////////////////////////////////
// CONTRACTOR_PROVE, CONTRACTOR_PROVE_WEIGHTED - prove with default prover
//////////////////////////////////////////////////////////////////////////////
#define CONTRACTOR_PROVE(n, lhs, rhs)                                          \
  ([] {                                                                        \
    auto ieq = CONTRACTOR_IEQ(n, lhs, rhs);                                    \
    auto pfn = CONTRACTOR_DEFAULT_PROVER(ieq);                                 \
    return pfn;                                                                \
  })()
#define CONTRACTOR_PROVE_WEIGHTED(n, lhs, rhs, lhs_weights, rhs_weights)       \
  ([] {                                                                        \
    auto ieq = CONTRACTOR_WEIGHTED_IEQ(n, lhs, rhs, lhs_weights, rhs_weights); \
    auto pfn = CONTRACTOR_DEFAULT_PROVER(ieq);                                 \
    return pfn;                                                                \
  })()
