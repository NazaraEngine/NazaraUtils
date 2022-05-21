set_project("NazaraUtils")

add_rules("mode.asan", "mode.coverage", "mode.debug", "mode.releasedbg", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

add_includedirs("include")
set_languages("c89", "cxx17")
set_rundir("./bin/$(plat)_$(arch)_$(mode)")
set_symbols("debug", "hidden")
set_targetdir("./bin/$(plat)_$(arch)_$(mode)")
set_warnings("allextra")

target("NazaraUtils")
    set_kind("headeronly")
    set_group("Library")
    add_headerfiles("include/(Nazara/Utils/**.hpp)")
    add_headerfiles("include/(Nazara/Utils/**.inl)")

includes("tests/*.lua")
