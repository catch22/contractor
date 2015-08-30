#include <vector>

namespace contractor {

//////////////////////////////////////////////////////////////////////////////
// occurrence_table -- for given inequality
//////////////////////////////////////////////////////////////////////////////
template <typename Inequality> struct occurrence_table {
    enum {
        N = Inequality::num_parties,
        L = Inequality::num_lhs,
        R = Inequality::num_rhs
    };
    occurrence_table(const Inequality &ieq) {
        // set nontrivial occurrence vectors for left-hand side
        for(size_t n = 0; n < N; ++n)
            for(size_t l = 0; l < L; ++l)
                lhs[n].set(l, ieq.lhs[l].bitset[n]);

        // set nontrivial occurrence vectors for right-hand side
        for(size_t n = 0; n < N; ++n)
            for(size_t r = 0; r < R; ++r)
                rhs[n].set(r, ieq.rhs[r].bitset[n]);

        // check that there is no inconsistency
        for(size_t n1 = 0; n1 <= N; ++n1) {
            for(size_t n2 = n1 + 1; n2 <= N; ++n2) {
                const auto x1 = lhs[n1], x2 = lhs[n2];
                const auto y1 = rhs[n1], y2 = rhs[n2];
                // incompatible assignment to same x value?
                if(x1 == x2 && y1 != y2) {
                    CONTRACTOR_LOG("occurrence table contains two incompatible "
                                   "assignments for x = "
                                   << x1.str_lsb_first()
                                   << ": f(x) = " << y1.str_lsb_first()
                                   << " vs. f(x) = " << y2.str_lsb_first());
                    throw std::runtime_error("occurrence table inconsistent");
                }

                // not a contraction?
                if(ieq.hamming_rhs(y1, y2) > ieq.hamming_lhs(x1, x2)) {
                    CONTRACTOR_LOG("occurrence table does not contract x1 = "
                                   << x1.str_lsb_first()
                                   << " vs x2 = " << x2.str_lsb_first()
                                   << ": f(x1) = " << y1.str_lsb_first()
                                   << " vs. f(x2) = " << y2.str_lsb_first());
                    throw std::runtime_error(
                        "occurrence table is not a contraction");
                }
            }
        }
    }

    std::array<bitset<L>, N + 1> lhs;
    std::array<bitset<R>, N + 1> rhs;
};

//////////////////////////////////////////////////////////////////////////////
// is_contraction
//////////////////////////////////////////////////////////////////////////////
template <typename Inequality, typename Function>
bool is_contraction(const Inequality &ieq, const Function &fn) {
    for(auto e : fn) {
        for(auto f : fn) {
            if(ieq.hamming_rhs(e.y, f.y) > ieq.hamming_lhs(e.x, f.x)) {
                CONTRACTOR_LOG("function table does not contract x1 = "
                               << e.x.str_lsb_first()
                               << " vs x2 = " << f.x.str_lsb_first()
                               << ": f(x1) = " << e.y.str_lsb_first()
                               << " vs. f(x2) = " << f.y.str_lsb_first());
                return false;
            }
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////
// partial_function - partially defined function of Hamming cubes (corresopnding
// to given Inequality)
//////////////////////////////////////////////////////////////////////////////
template <typename Inequality> struct partial_function {
    enum { L = Inequality::num_lhs, R = Inequality::num_rhs };

    struct defined_entry {
        bitset<L> x;
        bitset<R> y;
        std::string annotation;
    };

    struct undefined_entry {
        bitset<L> x;
        std::vector<bitset<R>> ys_options;
    };

    typedef bitsets<R> ys_all;

    // initialize by mapping occurence vectors onto each other (and leaving all
    // other function values undefined)
    partial_function(const Inequality &ieq) {
        enum { N = Inequality::num_parties };
        const occurrence_table<Inequality> occ(ieq);

        for(auto x : bitsets<L>()) {
            // is x an occurrence vector?
            size_t n;
            for(n = 0; n <= N; ++n)
                if(occ.lhs[n] == x)
                    break;

            // if yes, define f(x) = y
            if(n <= N) {
                defined_entry e{occ.lhs[n], occ.rhs[n], ""};
                e.annotation.push_back(n < N ? 'A' + n : '*');
                defined.push_back(e);
            } else {
                undefined_entry e{x, {}};
                undefined.push_back(e);
            }
        }
    }

    std::vector<defined_entry> defined;
    std::vector<undefined_entry> undefined;
};
}
