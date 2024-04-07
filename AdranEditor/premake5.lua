project "AdranEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Adran/vendor/spdlog/include",
		"%{wks.location}/Adran/src",
		"%{wks.location}/Adran/vendor",
		"%{IncludeDir.entt}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.assimp}"
	}


	links
	{
		"Adran"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"AR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "AR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AR_DIST"
		runtime "Release"
		optimize "on"