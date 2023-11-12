set_project("NazaraUtils")

add_rules("mode.coverage", "mode.debug", "mode.releasedbg", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

-- Sanitizers
local sanitizers = {
	asan = "address",
	lsan = "leak",
	tsan = "thread",
}

for opt, policy in table.orderpairs(sanitizers) do
	option(opt, { description = "Enable " .. opt, default = false })

	if has_config(opt) then
		set_policy("build.sanitizer." .. policy, true)
	end
end

add_includedirs("include")
set_languages("c89", "cxx17")
set_rundir("./bin/$(plat)_$(arch)_$(mode)")
set_targetdir("./bin/$(plat)_$(arch)_$(mode)")
set_warnings("allextra")

if is_mode("debug") then
    add_defines("NAZARA_DEBUG")
end

target("NazaraUtils", function ()
    set_kind("headeronly")
    set_group("Library")
    add_headerfiles("include/(NazaraUtils/**.hpp)")
    add_headerfiles("include/(NazaraUtils/**.inl)")
end)

includes("bench/*.lua")
includes("tests/*.lua")
