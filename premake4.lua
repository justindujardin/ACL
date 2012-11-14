-- A solution contains projects, and defines the available configurations
solution "ApplicationCoreLibrary"
   configurations { "Debug", "Release" }
   platforms { "x32" }
   location ( _OPTIONS["to"] )
   configuration "Debug"
      objdir "build/obj_debug"
      targetdir "bin/debug"
 
   configuration "Release"
      objdir "build/obj_release"
      targetdir "bin/release"
 
   -- The gtest runner application (this is first so that it becomes the default runnable project in vstudio solutions)
   project "acltests"
      targetname  "acltests"
      kind        "ConsoleApp"
      language    "C++"
      flags       { "No64BitChecks", "StaticRuntime" } 
      links       { "acl", "gtest" }
      includedirs { "acl", "lib/gtest/include" }
      files       { "tests/testMain.cpp", "tests/**Tests.cpp", "tests/platform/fixtures/**.cpp","tests/platform/fixtures/**.h" }
 
      configuration "Debug"
         defines  { "ACL_DEBUG" }        
         flags    { "Symbols" }
 
      configuration "Release"
         flags    { "Optimize" }    

      configuration "macosx"
         links    { "Cocoa.framework", "CoreServices.framework" }

      configuration "windows"
         defines  { "UNICODE" }
         links    { "winmm" }

      configuration "linux"
         links       { "pthread", "dl", "rt" }
         

   -- The google test static lib
   project "gtest"
      kind        "StaticLib"     
      language    "C++"
      flags       { "No64BitChecks", "StaticRuntime" } 
      files       { "lib/gtest/src/**.cc" }
      excludes    { "lib/gtest/src/gtest-all.cc", "lib/gtest/src/gtest_main.cc" }
      includedirs { "lib/gtest/include", "lib/gtest" }
      configuration "Debug"
         flags    { "Symbols" }
 
      configuration "Release"
         flags    { "Optimize" }    


   -- The ACL static lib
   project "acl"
      targetname  "acl"
      kind        "StaticLib"
      language    "C++"
      flags       { "No64BitChecks", "StaticRuntime", "ExtraWarnings" } 
      includedirs { "acl" }
      files       { "acl/**.h", "acl/**.cpp" }

      configuration "Debug"
         defines  { "ACL_DEBUG" }
         flags    { "Symbols" }
 
      configuration "Release"
         flags    { "Optimize" }    

      configuration "windows"
         defines  { "UNICODE" }
         links    { "winmm" }
         excludes { 
            "acl/platform/impls/mac/**", 
            "acl/platform/impls/posix/**",
            "acl/platform/impls/linux/**"
         }

      configuration "macosx"
         excludes    { 
            "acl/platform/impls/win32/**",
            "acl/platform/impls/linux/**"
         }
         files       { "acl/**.mm" }
         links       { "Cocoa.framework", "CoreServices.framework" }

      configuration "linux"
         excludes { 
            "acl/platform/impls/mac/**", 
            "acl/platform/impls/win32/**" 
         }
         links { "pthread", "dl", "rt" }

      configuration { "macosx", "gmake" }
         buildoptions { "-mmacosx-version-min=10.4" }
         linkoptions  { "-mmacosx-version-min=10.4" }             

--
-- A more thorough cleanup.
--
if _ACTION == "clean" then
   os.rmdir("build")
end

--
-- Use the --to=path option to control where the project files get generated. I use
-- this to create project files for each supported toolset, each in their own folder,
-- in preparation for deployment.
--

newoption {
   trigger = "to",
   value   = "path",
   description = "Set the output location for the generated files"
}

--
-- Specify the default project types per OS
--
function defaultaction(osName, actionName)
   if os.is(osName) then
      _ACTION = _ACTION or actionName
   end
end
defaultaction("windows", "vs2010")
defaultaction("macosx", "xcode3")
defaultaction("linux", "gmake")