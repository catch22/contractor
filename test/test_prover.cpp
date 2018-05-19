#include "catch.hpp"
#include <contractor/contractor.hpp>

TEST_CASE("Subadditivity and Araki-Lieb") {
  CONTRACTOR_PROVE(3, ("A", "B"), ("AB"));
  CONTRACTOR_PROVE(3, ("A", "ABC"), ("BC"));
  CONTRACTOR_PROVE(3, ("BC", "ABC"), ("A"));
}

TEST_CASE("Initial conditions that are multiply defined") {
  REQUIRE_THROWS(CONTRACTOR_PROVE(2, ("A"), ("B")));
  REQUIRE_THROWS(CONTRACTOR_PROVE(2, ("A"), ("AB")));
  CONTRACTOR_PROVE(2, ("AB"), ("AB"));
}

TEST_CASE("Initial condition that is not a contraction") {
  // REQUIRE_THROWS(CONTRACTOR_PROVE(2, ("A", "B"), ("A", "B", "B")));
}

TEST_CASE("Strong subadditivity and weak monotonicity") {
  CONTRACTOR_PROVE(3, ("AB", "BC"), ("B", "ABC"));
  CONTRACTOR_PROVE(3, ("AB", "BC"), ("A", "C"));
}

TEST_CASE("Monogamy of the mutual information") {
  CONTRACTOR_PROVE(3, ("AB", "BC", "CA"), ("A", "B", "C", "ABC"));
}

TEST_CASE("Most new five-party inequalities") {
  SECTION("Cyclic") {
    CONTRACTOR_PROVE(5, ("ABC", "BCD", "CDE", "DEA", "EAB"),
                     ("AB", "BC", "CD", "DE", "EA", "ABCDE"));
  }
  SECTION("A1") {
    CONTRACTOR_PROVE_WEIGHTED(
        5, ("ABC", "ABD", "ABE", "ACD", "ADE", "BCE", "BDE"),
        ("ABCD", "ABCE", "ABDE", "AB", "AC", "AD", "BC", "BE", "DE"),
        (2, 1, 1, 1, 1, 1, 1), (1, 1, 1, 1, 1, 1, 1, 1, 1));
    CONTRACTOR_PROVE(
        5, ("ABC", "ABC", "ABD", "ABE", "ACD", "ADE", "BCE", "BDE"),
        ("ABCD", "ABCE", "ABDE", "AB", "AC", "AD", "BC", "BE", "DE"));
  }
  SECTION("A3") {
    CONTRACTOR_PROVE(
        5, ("ABC", "ABD", "ABE", "ACD", "ACE", "ADE", "BCE", "BDE", "CDE"),
        ("AB", "ABCE", "ABDE", "AC", "ACDE", "AD", "BCD", "BE", "CE", "DE"));
  }
}

TEST_CASE("Last new five-party inequalities", "[.slow]") {
  SECTION("A4") {
    CONTRACTOR_PROVE(5, ("ABC", "ABD", "ABE", "ACD", "ACE", "BC", "DE"),
                     ("AB", "ABCD", "ABCE", "AC", "ADE", "B", "C", "D", "E"));
  }
  SECTION("B2") {
    CONTRACTOR_PROVE_WEIGHTED(
        5,
        ("ABC", "ABD", "ABE", "ACD", "ACE", "ADE", "BCD", "BCE", "BDE", "CDE"),
        ("AB", "AB", "ABCD", "ABCD", "ABCE", "ABCE", "ABDE", "AC", "AC", "ACDE",
         "AD", "AE", "BC", "BD", "BD", "CE", "CE", "DE"),
        (3, 3, 1, 1, 3, 1, 1, 1, 1, 1),
        (1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));
  }
}

TEST_CASE("Various failures", "[.slow]") {
  SECTION("A5") {
    // failure after one choice has been made
    REQUIRE_THROWS(CONTRACTOR_PROVE_WEIGHTED(
        5, ("ABC", "ABD", "ABE", "ACD", "ACE", "BC", "BE", "CD", "CE", "DE"),
        ("AB", "AB", "ABCD", "ABCD", "ABCE", "ABCE", "AC", "AC", "ADE", "B",
         "B", "B", "BCE", "C", "C", "C", "C", "CDE", "D", "D", "D", "E", "E",
         "E", "E"),
        (3, 2, 1, 1, 2, 2, 2, 2, 2, 2),
        (1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1)));
  }
  SECTION("A6") {
    // all get stuck without any choices made
    REQUIRE_THROWS(CONTRACTOR_PROVE_WEIGHTED(
        5, ("ABC", "ABD", "ACE", "ADE", "BCDE", "BE", "CD"),
        ("A", "ABCDE", "ABE", "ACD", "B", "BCD", "BCE", "BDE", "C", "CDE", "D",
         "E"),
        (2, 2, 2, 2, 3, 3, 3), (3, 3, 1, 1, 3, 1, 1, 1, 3, 1, 3, 3)));
    REQUIRE_THROWS(CONTRACTOR_PROVE_WEIGHTED(
        5, ("ABC", "ABD", "ACE", "ADE", "BCDE", "BE", "CD"),
        ("A", "A", "A", "ABCDE", "ABCDE", "ABCDE", "ABE", "ACD", "B", "B", "B",
         "BCD", "BCE", "BDE", "C", "C", "C", "CDE", "D", "D", "D", "E", "E",
         "E"),
        (2, 2, 2, 2, 3, 3, 3),
        (1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1)));
    REQUIRE_THROWS(CONTRACTOR_PROVE(
        5,
        ("ABC", "ABC", "ABD", "ABD", "ACE", "ACE", "ADE", "ADE", "BCDE", "BCDE",
         "BCDE", "BE", "BE", "BE", "CD", "CD", "CD"),
        ("A", "A", "A", "ABCDE", "ABCDE", "ABCDE", "ABE", "ACD", "B", "B", "B",
         "BCD", "BCE", "BDE", "C", "C", "C", "CDE", "D", "D", "D", "E", "E",
         "E")));
  }
  SECTION("B1a") {
    // all get stuck without any choices made
    REQUIRE_THROWS(CONTRACTOR_PROVE_WEIGHTED(
        5, ("ABC", "ABD", "ACE", "AD", "ADE", "BC", "BCDE", "BE", "CD"),
        ("A", "ABCD", "ABCDE", "ABE", "ACD", "B", "BCD", "BCE", "C", "CDE", "D",
         "E"),
        (2, 2, 2, 1, 1, 1, 2, 2, 4), (3, 1, 2, 1, 1, 3, 1, 1, 4, 1, 4, 2)));
    REQUIRE_THROWS(CONTRACTOR_PROVE_WEIGHTED(
        5, ("ABC", "ABD", "ACE", "AD", "ADE", "BC", "BCDE", "BE", "CD"),
        ("A", "A", "A", "ABCD", "ABCDE", "ABCDE", "ABE", "ACD", "B", "B", "B",
         "BCD", "BCE", "C", "C", "C", "C", "CDE", "D", "D", "D", "D", "E", "E"),
        (2, 2, 2, 1, 1, 1, 2, 2, 4),
        (1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
         1)));
    REQUIRE_THROWS(CONTRACTOR_PROVE(
        5,
        ("ABC", "ABC", "ABD", "ABD", "ACE", "ACE", "AD", "ADE", "BC", "BCDE",
         "BCDE", "BE", "BE", "CD", "CD", "CD", "CD"),
        ("A", "A", "A", "ABCD", "ABCDE", "ABCDE", "ABE", "ACD", "B", "B", "B",
         "BCD", "BCE", "C", "C", "C", "C", "CDE", "D", "D", "D", "D", "E",
         "E")));
  }
  SECTION("B1b") {
    // all get stuck without any choices made
    REQUIRE_THROWS(CONTRACTOR_PROVE_WEIGHTED(
        5, ("ABC", "ABD", "ACE", "ADE", "BC", "BCDE", "BE", "CD"),
        ("A", "ABCD", "ABCDE", "ABE", "AC", "B", "BCD", "BCE", "C", "CDE", "D",
         "E"),
        (2, 2, 2, 1, 1, 2, 2, 3), (2, 1, 2, 1, 1, 3, 1, 1, 3, 1, 3, 2)));
    REQUIRE_THROWS(CONTRACTOR_PROVE_WEIGHTED(
        5, ("ABC", "ABD", "ACE", "ADE", "BC", "BCDE", "BE", "CD"),
        ("A", "A", "ABCD", "ABCDE", "ABCDE", "ABE", "AC", "B", "B", "B", "BCD",
         "BCE", "C", "C", "C", "CDE", "D", "D", "D", "E", "E"),
        (2, 2, 2, 1, 1, 2, 2, 3),
        (1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)));
    REQUIRE_THROWS(CONTRACTOR_PROVE(
        5,
        ("ABC", "ABC", "ABD", "ABD", "ACE", "ACE", "ADE", "BC", "BCDE", "BCDE",
         "BE", "BE", "CD", "CD", "CD"),
        ("A", "A", "ABCD", "ABCDE", "ABCDE", "ABE", "AC", "B", "B", "B", "BCD",
         "BCE", "C", "C", "C", "CDE", "D", "D", "D", "E", "E")));
  }
  SECTION("B1c") {
    // all get stuck without any choices made
    REQUIRE_THROWS(CONTRACTOR_PROVE_WEIGHTED(
        5, ("ABC", "ABD", "ACE", "ADE", "BCDE", "BE", "CD"),
        ("A", "ABCD", "ABCDE", "ABE", "AC", "B", "BCE", "BD", "C", "CDE", "D",
         "E"),
        (2, 2, 2, 1, 2, 2, 2), (2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 2)));
    REQUIRE_THROWS(CONTRACTOR_PROVE_WEIGHTED(
        5, ("ABC", "ABD", "ACE", "ADE", "BCDE", "BE", "CD"),
        ("A", "A", "ABCD", "ABCDE", "ABCDE", "ABE", "AC", "B", "B", "BCE", "BD",
         "C", "C", "CDE", "D", "D", "E", "E"),
        (2, 2, 2, 1, 2, 2, 2),
        (1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)));
    REQUIRE_THROWS(CONTRACTOR_PROVE(
        5,
        ("ABC", "ABC", "ABD", "ABD", "ACE", "ACE", "ADE", "BCDE", "BCDE", "BE",
         "BE", "CD", "CD"),
        ("A", "A", "ABCD", "ABCDE", "ABCDE", "ABE", "AC", "B", "B", "BCE", "BD",
         "C", "C", "CDE", "D", "D", "E", "E")));
  }
}
