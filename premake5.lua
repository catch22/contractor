solution "contractor"
    flags {"FatalCompileWarnings"}
    warnings "Extra"
    includedirs {"."}
    targetdir "bin/%{cfg.buildcfg}"
    location "build"

    configurations {"debug", "debug-simple", "debug-clever", "debug-clever-lazy", "release"}
    filter "configurations:debug*"
        defines {"DEBUG"}
        flags {"Symbols"}
    configuration "release"
        defines {"NDEBUG"}
        optimize "On"

    configuration "debug-simple"
        defines {"CONTRACTOR_DEFAULT_PROVER=contractor::prove_simple"}
    configuration "debug-clever"
        defines {"CONTRACTOR_DEFAULT_PROVER=contractor::prove_clever"}
    configuration "debug-clever-lazy"
        defines {"CONTRACTOR_DEFAULT_PROVER=contractor::prove_clever_lazy"}

    configuration "gmake"
        buildoptions { "-std=c++11" }
        targetextension ""

project "example"
    kind "ConsoleApp"
    language "C++"
    files {"include/*.hpp", "include/*.inc", "example.cpp"}

project "test"
    kind "ConsoleApp"
    language "C++"
    files {"include/*.hpp", "include/*.inc", "test/*.cpp"}
