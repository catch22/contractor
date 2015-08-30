#include <contractor/contractor.hpp>

int main() {
    try {
        CONTRACTOR_PROVE(5, ("ABC", "BCD", "CDE", "DEA", "EAB"),
                         ("AB", "BC", "CD", "DE", "EA", "ABCDE"));
        std::cout << "Success!" << std::endl;
    } catch(const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
