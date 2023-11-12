option("tests", { default = false, description = "Enable unit tests"})
option("tests_cxx_version", { default = "cxx17", values = {"cxx17", "cxx20", "cxxlatest"}, description = "C++ version of unit tests" })

if has_config("tests") then
	if has_config("asan") then
		add_defines("CATCH_CONFIG_NO_WINDOWS_SEH")
		add_defines("CATCH_CONFIG_NO_POSIX_SIGNALS")
	end

	add_requires("catch2 3")

    target("UnitTests", function ()
        set_kind("binary")
        set_group("Tests")
        add_headerfiles("src/**.hpp")
        add_files("src/**.cpp")
		add_includedirs("src")
		set_languages(get_config("tests_cxx_version"))

		add_deps("NazaraUtils")
        add_packages("catch2")
	end)
end
