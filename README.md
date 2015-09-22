The Holographic Contractor [![Build Status](https://travis-ci.org/catch22/contractor.svg)](https://travis-ci.org/catch22/contractor)
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
        CONTRACTOR_PROVE(5, ("ABC", "BCD", "CDE", "DEA", "EAB"),
                         ("AB", "BC", "CD", "DE", "EA", "ABCDE"));
        std::cout << "Success!" << std::endl;
    } catch(const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

~~~


Installation
------------

To compile `contractor` and run the above example, install [premake5](https://premake.github.io) and run

~~~bash
premake5 gmake
make -C build config=release example && bin/release/example
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
