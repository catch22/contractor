#include <contractor/contractor.hpp>
#include "catch.hpp"

using contractor::subsystem;
using contractor::inequality;

//////////////////////////////////////////////////////////////////////////////
// Prove a single cyclic inequality
// (N parties, LBody-terms on the LHS, RBody-terms on the RHS)
//////////////////////////////////////////////////////////////////////////////
template <size_t N, size_t LBody, size_t RBody> void prove_cyclic() {
    // generate the left-hand side
    std::array<subsystem<N>, N> lhs;
    for(size_t start = 0; start < N; ++start) {
        std::string term;
        for(size_t i = 0; i < LBody; ++i)
            term.push_back('A' + (start + i) % N);
        lhs[start] = subsystem<N>(term.c_str());
    }

    // generate the right-hand side
    std::array<subsystem<N>, N + 1> rhs;
    if(RBody > 0) {
        for(size_t start = 0; start < N; ++start) {
            std::string term;
            for(size_t i = 0; i < RBody; ++i)
                term.push_back('A' + (LBody + start + i) % N);
            rhs[start] = subsystem<N>(term.c_str());
        }
    }
    {
        std::string term;
        for(size_t i = 0; i < N; ++i)
            term.push_back('A' + i);
        rhs[N] = subsystem<N>(term.c_str());
    }

    // build inequality
    inequality<N, N, N + 1> ieq = {lhs, rhs};

    // test our expectations
    bool expect_success = LBody > RBody && LBody + RBody <= N;
    if(expect_success)
        CONTRACTOR_DEFAULT_PROVER(ieq);
    else
        REQUIRE_THROWS(CONTRACTOR_DEFAULT_PROVER(ieq));
}

//////////////////////////////////////////////////////////////////////////////
// prove all cyclic inequalities on NBegin <= N < Nend parties
//////////////////////////////////////////////////////////////////////////////
template <size_t Nbegin, size_t Nend> void prove_all_cyclic();

namespace prove_all_cyclic_impl {

template <size_t N, size_t Nend> struct for_all_N;
template <size_t N, size_t LBody> struct for_all_LBody;
template <size_t N, size_t LBody, size_t RBody> struct for_all_RBody;

// loop over all N
template <size_t N, size_t Nend> struct for_all_N {
    static void go() {
        for_all_LBody<N, 1>::go();
        for_all_N<N + 1, Nend>::go();
    }
};

template <size_t Nend> struct for_all_N<Nend, Nend> {
    static void go() {}
};

// loop over all LBody
template <size_t N, size_t LBody> struct for_all_LBody {
    static void go() {
        for_all_RBody<N, LBody, 0>::go();
        for_all_LBody<N, LBody + 1>::go();
    }
};

template <size_t N> struct for_all_LBody<N, N> {
    static void go() {}
};

// loop over all RBody
template <size_t N, size_t LBody, size_t RBody> struct for_all_RBody {
    static void go() {
        prove_cyclic<N, LBody, RBody>();
        for_all_RBody<N, LBody, RBody + 1>::go();
    }
};

template <size_t N, size_t LBody> struct for_all_RBody<N, LBody, N> {
    static void go() {}
};
}

template <size_t Nbegin, size_t Nend> void prove_all_cyclic() {
    prove_all_cyclic_impl::for_all_N<Nbegin, Nend>::go();
}

TEST_CASE("Cyclic inequalities") {
    SECTION("Five-party of 3-2 type") { prove_cyclic<5, 3, 2>(); }
    SECTION("All on two to five parties") { prove_all_cyclic<2, 6>(); }
}
