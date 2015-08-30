namespace contractor {

//////////////////////////////////////////////////////////////////////////////
// prove_simple -- naive "textbook" implementation of our greedy algorithm
// (recomputes possible function values at each iteration)
//////////////////////////////////////////////////////////////////////////////
template <typename Inequality> void prove_simple(const Inequality &ieq) {
    using function_type = partial_function<Inequality>;
    using defined_entry = typename partial_function<Inequality>::defined_entry;
    using ys_all = typename partial_function<Inequality>::ys_all;

    // start with occurrence table
    CONTRACTOR_LOG("prove_simple(): " << ieq);
    function_type pfn{ieq};

    // greedily extend function...
    bool made_choices = false;
    while(true) {
        CONTRACTOR_LOG("... " << pfn.undefined.size()
                              << " undefined function values remaining");

#ifdef DEBUG
        if(!is_contraction(ieq, pfn.defined))
            throw std::runtime_error("internal error: no longer a contraction");
#endif

        // done?
        if(pfn.undefined.empty())
            break;

        // attempt to find unique extension first
        bool found_unique_extension = false;
        for(auto it = pfn.undefined.begin(); it != pfn.undefined.end(); ++it) {
            // find all function values compatible with current function table
            auto &e = *it;
            e.ys_options.clear();
            for(auto y : ys_all()) {
                bool ok = true;
                for(auto f : pfn.defined)
                    if(ieq.hamming_rhs(y, f.y) > ieq.hamming_lhs(e.x, f.x)) {
                        ok = false;
                        break;
                    }
                if(ok)
                    e.ys_options.push_back(y);
            }

            // no option?
            if(e.ys_options.empty()) {
                if(made_choices) {
                    CONTRACTOR_LOG("cannot extend further (got stuck): x = "
                                   << e.x.str_lsb_first());
                    throw std::runtime_error(
                        "cannot extend further (got stuck)");
                } else {
                    CONTRACTOR_LOG("cannot extend further (before making any "
                                   "choices): x = "
                                   << e.x.str_lsb_first());
                    throw std::runtime_error(
                        "cannot extend further (before making any choices)");
                }
            }

            // unique option?
            if(e.ys_options.size() == 1) {
                defined_entry f{e.x, e.ys_options[0],
                                made_choices ? "unique after choices"
                                             : "unique"};
                pfn.defined.push_back(f);
                pfn.undefined.erase(it);
                found_unique_extension = true;
                break;
            }
        }
        if(found_unique_extension)
            continue;

        // make an arbitrary choice..
        made_choices = true;
        auto e = pfn.undefined[0];

        // ..and add it to function table
        std::string annotation = "CHOICE! options were:";
        for(auto y : e.ys_options)
            annotation += " " + y.str_lsb_first();
        defined_entry f{e.x, e.ys_options[0], annotation};
        pfn.defined.push_back(f);
        pfn.undefined.erase(pfn.undefined.begin());
        CONTRACTOR_LOG("    made a choice: x = " << f.x.str_lsb_first()
                                                 << ", f(x) = "
                                                 << f.y.str_lsb_first());
    }

    // verify one last time that we have indeed constructed a contraction
    if(!is_contraction(ieq, pfn.defined))
        throw std::runtime_error(
            "internal error: constructed a non-contraction");
}
}
