set_project("NazaraUtils")

add_rules("mode.asan", "mode.coverage", "mode.debug", "mode.releasedbg", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

add_includedirs("include")
set_languages("c89", "cxx17")
set_rundir("./bin/$(plat)_$(arch)_$(mode)")
set_targetdir("./bin/$(plat)_$(arch)_$(mode)")
set_warnings("allextra")

if is_mode("debug") then
    add_defines("NAZARA_DEBUG")
end

target("NazaraUtils")
    set_kind("headeronly")
    set_group("Library")
    add_headerfiles("include/(NazaraUtils/**.hpp)")
    add_headerfiles("include/(NazaraUtils/**.inl)")

includes("tests/*.lua")
