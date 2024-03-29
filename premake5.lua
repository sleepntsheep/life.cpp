-- premake5.lua
workspace "life"
configurations { "Debug", "Release", "Mingw" }

newaction {
    trigger     = "clean",
    description = "clean the software",
    execute     = function ()
        print("clean the build...")
        os.rmdir("./build")
        os.rmdir("Debug")
        os.rmdir("Release")
        os.rmdir("main.o")
        print("done.")
    end
}

project "life"
kind "WindowedApp"
language "C++"
targetdir "%{cfg.buildcfg}"
cppdialect "c++20"

files { "src/*.hpp", "src/*.cpp" }

buildoptions { "-Wall", "-Wextra", "-pedantic" }
links { "SDL2", "SDL2_ttf" }

filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"


