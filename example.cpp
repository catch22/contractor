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
