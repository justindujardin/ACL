-- A solution contains projects, and defines the available configurations
solution "ApplicationCoreLibrary"
   configurations { "Debug", "Release" }
   platforms { "x32" }
 
   -- The google test static lib
   project "gtest"
      kind        "StaticLib"     
      language    "C++"
      defines     { "GTEST_HAS_PTHREAD=0" }
      files       { "lib/gtest/src/gtest-all.cc" }
      includedirs { "lib/gtest/include", "lib/gtest" }

   -- The ACL static lib
   project "acl"
      targetname  "acl"
      kind        "StaticLib"
      language    "C++"
      flags       { "No64BitChecks", "ExtraWarnings", "StaticRuntime" } 
      includedirs { "acl" }
      files       { "acl/**.h", "acl/**.cpp" }

      configuration "Debug"
         defines  { "DEBUG" }
         flags    { "Symbols" }
 
      configuration "Release"
         defines  { "NDEBUG" }
         flags    { "Optimize" }    

      configuration "macosx"
         excludes    { "acl/platform/impls/win32/**.*" }
         files       { "acl/**.mm" }
         links       { "Cocoa.framework", "CoreServices.framework" }
         
      configuration { "macosx", "gmake" }
         buildoptions { "-mmacosx-version-min=10.4" }
         linkoptions  { "-mmacosx-version-min=10.4" }             

   -- The gtest runner application
   project "acltests"
      targetname  "acltests"
      kind        "ConsoleApp"
      language    "C++"
      flags       { "No64BitChecks", "ExtraWarnings", "StaticRuntime" } 
      links       { "acl", "gtest" }
      includedirs { "acl", "lib/gtest/include" }
      files       { "tests/testMain.cpp", "tests/**Tests.cpp" }
      excludes    { "tests/platform/**" }
 
      configuration "Debug"
         defines  { "DEBUG" }        
         flags    { "Symbols" }
 
      configuration "Release"
         defines  { "NDEBUG" }
         flags    { "Optimize" }    

      configuration "macosx"
         links    { "Cocoa.framework", "CoreServices.framework" }

--
-- A more thorough cleanup.
--
if _ACTION == "clean" then
   os.rmdir("bin")
   os.rmdir("build")
end

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