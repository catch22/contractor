#ifndef CONTRACTOR_HPP
#define CONTRACTOR_HPP

#ifndef CONTRACTOR_LOG
#ifdef DEBUG
#define CONTRACTOR_LOG(what) std::cerr << what << std::endl
#else
#define CONTRACTOR_LOG(what)
#endif
#endif

#include "bitset.hpp"
#include "subsystem.hpp"
#include "inequality.hpp"
#include "prover.hpp"

#endif
