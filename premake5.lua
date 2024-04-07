include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"
workspace "Adran"
	architecture "x86_64"
	startproject "AdranEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Adran/vendor/GLFW"
	include "Adran/vendor/Glad"
	include "Adran/vendor/imgui"
	include "Adran/vendor/yaml-cpp"
group ""

include "Adran"
include "AdranEditor"
include "SandBox"