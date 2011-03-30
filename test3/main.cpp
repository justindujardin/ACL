#include "../lib/core/torque.h"
#include "../lib/core/util/str.h"
#include "../lib/core/containers/tVector.h"
#include "../lib/core/strings/stringFunctions.h"
#include <conio.h>


#ifdef TORQUE_OS_WIN32
#include <windows.h> // for unicode call testing
#endif

//-----------------------------------------------------------------------------
S32 main(void)
{
   dPrintf("Application Core Library - Strings\r\n");

   //-----------------------------------------------------------------------------
   // Strings
   //-----------------------------------------------------------------------------
   String str1("Hello World");
   String str2 = String::ToLower(str1);

   dPrintf("Case Sensitive Comparing (%s) to (%s)\r\n", str1.c_str(), str2.c_str());
   dPrintf( "Result : %s\r\n", (str1 == str2) ? "Strings Identical" : "Strings Different");

   dPrintf("Case Insensitive Comparing (%s) to (%s)\r\n", str1.c_str(), str2.c_str());
   dPrintf( "Result : %s\r\n", str1.equal(str2, String::NoCase) ? "Strings Identical" : "Strings Different");


   //-----------------------------------------------------------------------------
   // vector of strings to test string construct/destruct in place
   //-----------------------------------------------------------------------------
   Vector<String> strVec;
   strVec.push_back("test");
   strVec.increment();
   strVec.last() = "Big brown box?";
   strVec.push_front("lolBBQ");

   for( S32 i = 0; i < strVec.size(); i++ )
      dPrintf("strVec[%d] = %s\r\n", i, strVec[i].c_str());

   //-----------------------------------------------------------------------------
   // Unicode Strings
   //-----------------------------------------------------------------------------
#ifdef TORQUE_OS_WIN32
   HWND deskWnd = ::GetForegroundWindow();  

   // get text in UTF16 buffer
   UTF16 unicodeWindowText[256];
   ::GetWindowTextW(deskWnd,(LPWSTR)&unicodeWindowText,256);

   // Convert to UTF8 to print
   String windowText(unicodeWindowText);
   dPrintf("Foreground Window Text = %s", windowText.c_str());
#endif

   //-----------------------------------------------------------------------------
   // Done
   dPrintf("\r\n\r\nPress any key to quit\r\n");
   _getch();
   return 0;
}