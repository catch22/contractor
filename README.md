The Holographic Contractor [![Build Status](https://app.travis-ci.com/catch22/contractor.svg)](https://app.travis-ci.com/github/catch22/contractor)
==========================

In a [recent paper](http://arxiv.org/abs/1505.07839), we have described a combinatorial method for proving holographic entropy inequalities based on finding contractions of the hypercube.
`contractor` is an optimized C++ implementation of the greedy algorithm that we sketched in the paper for finding these "proofs by contractions".


Basic Usage
-----------

Proving one of the new cyclic inequalities derived in our paper is as simple as in the following [example](example.cpp):

~~~cpp
#include <contractor/contractor.hpp>

int main() {
    try {
        // Prove ABC + BCD + CDE + DEA + EAB >= AB + BC + CD + DE + EA + ABCDE
        auto fn = CONTRACTOR_PROVE(5, ("ABC", "BCD", "CDE", "DEA", "EAB"),
                                   ("AB", "BC", "CD", "DE", "EA", "ABCDE"));
        std::cout << "Success!" << std::endl;

        // Print contraction
        for (auto e : fn) {
            std::cout << e.x << " | " << e.y << " | " << e.annotation << std::endl;
        }

        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

~~~


Installation
------------

To compile `contractor` and run the above example, install [cmake](https://cmake.org) and run

~~~bash
cmake -E make_directory build
cmake -E chdir build cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release   # to (re)build

cmake -E chdir build ./example         # to run example
cmake -E chdir build make test         # to run unit tests
~~~


Attribution
-----------

If you find `contractor` useful for your research, please consider citing our paper:

~~~bibtex
@article{contractor,
  author  = {Bao, N. and Nezami, S. and Ooguri, H. and Stoica, B. and Sully, J. and Walter, M.},
  title   = {The holographic entropy cone},
  year    = {2015},
  journal = {Journal of High Energy Physics},
  volume  = {09},
  pages   = {130},
  eprint  = {1505.07839},
  doi     = {10.1007/JHEP09(2015)130},
  note    = {Software available at \url{https://github.com/catch22/contractor/}.},
}
~~~

Copyright 2015, [Michael Walter](michael.walter@stanford.edu).
