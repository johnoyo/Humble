-- main solution file.
workspace "HumbleGameEngine"
    architecture "x64"

    configurations 
    { 
        "Debug",
        "Release" 
    }

-- Variable to hold output directory.
outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

-- Engine project.
project "Humble"
    location "Humble"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    -- Directories for binary and intermediate files.
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    { 
        "%{prj.name}/src/**.h", 
        "%{prj.name}/src/**.cpp"
    }

    -- Include directories.
    includedirs
    {
        "Dependencies/GLFW/include",
        "Dependencies/GLEW/include",
        "Dependencies/Fmod/include",
        "Humble/src/vendor"
    }

    libdirs
    {
        "Dependencies/GLFW/lib-vc2019/x64",
        "Dependencies/GLEW/lib/Release/x64",
        "Dependencies/Fmod/lib/x64",
    }

    links
    {
        "fmod_vc.lib",
        "glew32.lib",
        "glfw3.lib",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "HBL_PLATFORM_WINDOWS",
            "HBL_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/SampleApp")
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"

    filter { "system:windows", "configurations:Debug" }
        buildoptions "/MDd"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MD"

-- Application project
project "SampleApp"
    location "SampleApp"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    -- Directories for binary and intermediate files.
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    { 
        "%{prj.name}/src/**.h", 
        "%{prj.name}/src/**.cpp"
    }

    -- Include directories.
    includedirs
    {
        "Humble/src",
        "Humble/src/vendor",
        "Dependencies/GLFW/include",
        "Dependencies/GLEW/include",
        "Dependencies/Fmod/include"
    }

    links
    {
        -- Add reference to engine project, so when the game gets built, the engine builds as well.
        "Humble"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "HBL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "on"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "on"

    filter { "system:windows", "configurations:Debug" }
        buildoptions "/MDd"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MD"