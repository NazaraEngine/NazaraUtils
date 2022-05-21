option("tests")
	set_default(false)
	set_showmenu(true)
	set_description("Build unit tests")

option_end()

if has_config("tests") then
	if is_mode("asan") then
		add_defines("CATCH_CONFIG_NO_WINDOWS_SEH")
		add_defines("CATCH_CONFIG_NO_POSIX_SIGNALS")
	end

	add_requires("catch2")

    target("NazaraUtilsTest")
        set_kind("binary")
        set_group("Tests")
        add_headerfiles("src/**.hpp")
        add_files("src/**.cpp")

		add_deps("NazaraUtils")
        add_packages("catch2")
end