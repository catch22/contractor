#pragma once

#ifndef CONTRACTOR_LOG
#ifdef DEBUG
#define CONTRACTOR_LOG(what) std::cerr << what << std::endl
#else
#define CONTRACTOR_LOG(what)
#endif
#endif

#include "bitset.hpp"
#include "inequality.hpp"
#include "prover.hpp"
#include "subsystem.hpp"
