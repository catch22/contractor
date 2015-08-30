solution "contractor"
    flags {"FatalCompileWarnings"}
    warnings "Extra"
    includedirs {"."}
    targetdir "bin/%{cfg.buildcfg}"

    configurations {"debug", "release"}
    configuration "debug"
        defines {"DEBUG"}
        flags {"Symbols"}
    configuration "release"
        defines {"NDEBUG"}
        optimize "On"

    configuration "gmake"
        buildoptions { "-std=c++11" }

project "example"
    kind "ConsoleApp"
    language "C++"
    files {"include/*.hpp", "include/*.inc", "example.cpp"}

project "test"
    kind "ConsoleApp"
    language "C++"
    files {"include/*.hpp", "include/*.inc", "test/*.cpp"}
