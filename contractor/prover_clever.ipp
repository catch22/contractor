namespace contractor {

//////////////////////////////////////////////////////////////////////////////
// prove_clever -- precompute all possible values for the unknown f(x)'s, then
// update as we learn new function values (instead of recomputing each time)
//////////////////////////////////////////////////////////////////////////////
template <typename Inequality>
function<Inequality> prove_clever(const Inequality &ieq) {
  using function_type = partial_function<Inequality>;
  using defined_entry = typename partial_function<Inequality>::defined_entry;
  using ys_all = typename partial_function<Inequality>::ys_all;

  // start with occurrence table
  CONTRACTOR_LOG("prove_clever(): " << ieq);
  function_type pfn{ieq};

  // determine all options for undefined function values
  CONTRACTOR_LOG("... precomputing all possible f(x)");
  for (auto it = pfn.undefined.begin(); it != pfn.undefined.end(); ++it) {
    // find all possible function values compatible with the
    // constraints:
    auto &e = *it;
    for (auto y : ys_all()) {
      // check all constraints
      bool ok = true;
      for (auto f : pfn.defined) {
        if (ieq.hamming_rhs(y, f.y) > ieq.hamming_lhs(e.x, f.x)) {
          ok = false;
          break;
        }
      }
      if (ok)
        e.ys_options.push_back(y);
    }

    // no extension?
    if (e.ys_options.empty()) {
      CONTRACTOR_LOG("cannot extend initial data: x = " << e.x);
      throw std::runtime_error("cannot extend initial data to a "
                               "contraction (before process has even "
                               "started)");
    }
  }

  // greedily extend function...
  bool made_choices = false;
  while (!pfn.undefined.empty()) {
    CONTRACTOR_LOG("... " << pfn.undefined.size()
                          << " undefined function values remaining");

#ifdef DEBUG
    if (!is_contraction(ieq, pfn.defined))
      throw std::runtime_error("internal error: no longer a contraction");
#endif

    // attempt to find unique extension first
    bool found_unique_extension = false;
    defined_entry new_entry;
    for (auto it = pfn.undefined.begin(); it != pfn.undefined.end(); ++it) {
      auto e = *it;
      if (e.ys_options.size() == 1) {
        new_entry =
            defined_entry{e.x, e.ys_options[0],
                          made_choices ? "unique after choices" : "unique"};
        pfn.defined.push_back(new_entry);
        pfn.undefined.erase(it);
        found_unique_extension = true;
        break;
      }
    }

    if (!found_unique_extension) {
      // make an arbitrary choice..
      auto e = pfn.undefined[0];
      made_choices = true;

      // ..and add it to function table
      std::string annotation = "CHOICE! options were:";
      for (auto y : e.ys_options)
        annotation += " " + y.str_lsb_first();
      new_entry = defined_entry{e.x, e.ys_options[0], annotation};
      pfn.defined.push_back(new_entry);
      pfn.undefined.erase(pfn.undefined.begin());
      CONTRACTOR_LOG("    made a choice: x = " << new_entry.x
                                               << ", f(x) = " << new_entry.y);
    }

    // update options for undefined function values
    for (auto it = pfn.undefined.begin(); it != pfn.undefined.end(); ++it) {
      // find all function values compatible with current function table
      auto &e = *it;
      std::vector<bitset<function_type::R>> new_ys_options;
      for (auto y : e.ys_options) {
        if (ieq.hamming_rhs(y, new_entry.y) <=
            ieq.hamming_lhs(e.x, new_entry.x))
          new_ys_options.push_back(y);
      }
      e.ys_options = new_ys_options;

      // no extension?
      if (e.ys_options.empty()) {
        if (made_choices) {
          CONTRACTOR_LOG("cannot extend further (got stuck): x = " << e.x);
          throw std::runtime_error("cannot extend further (got stuck)");
        } else {
          CONTRACTOR_LOG("cannot extend further (before making any "
                         "choices): x = "
                         << e.x);
          throw std::runtime_error(
              "cannot extend further (before making any choices)");
        }
      }
    }
  }

  // verify one last time that we have indeed constructed a contraction
  if (!is_contraction(ieq, pfn.defined))
    throw std::runtime_error("internal error: constructed a non-contraction");
  return pfn.defined;
}
} // namespace contractor
