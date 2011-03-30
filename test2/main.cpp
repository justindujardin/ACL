#include "../lib/core/types/types.h"
#include "../lib/core/util/delegate.h"
#include "../lib/core/util/tSignal.h"
#include<iostream>

//-----------------------------------------------------------------------------
// Delegate Example
//-----------------------------------------------------------------------------
typedef Delegate<void (S32)> SomeIntDelegate;

class MethodDelegateUsage
{
public:
   MethodDelegateUsage() {};

   void memberFunctionAsDelegate(S32 in)
   {
      std::cout << "member function given value " << in << std::endl;
   }
};

void functionAsDelegate(S32 in)
{
   std::cout << "simple function given value " << in << std::endl;
}

//-----------------------------------------------------------------------------
// Signal Example
//-----------------------------------------------------------------------------
typedef Signal<void(S32)> SomeIntSignal;


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
S32 main(void)
{
   std::cout << "Application Core Library - Delegate/Signal Test" << std::endl;

   //-----------------------------------------------------------------------------
   // Delegates 
   SomeIntDelegate theDelegate;

   // simple function 
   theDelegate.bind(&functionAsDelegate);
   std::cout << "Delegate bound to simple function" << std::endl;
   theDelegate(1337);

   // Member function on object
   MethodDelegateUsage mduObject;
   theDelegate.bind(&mduObject, &MethodDelegateUsage::memberFunctionAsDelegate);
   std::cout << "Delegate bound to member function on object" << std::endl;
   theDelegate(1337);


   //-----------------------------------------------------------------------------
   // Signals
   SomeIntSignal theSignal;
   std::cout << std::endl << "Signal test binding both previous delegates." << std::endl;
   std::cout << "*member function should trigger first even though added second, because of ordering parameter" << std::endl;
 
   // simple function
   theSignal.notify(&functionAsDelegate);
   // Member function on object (should trigger first because of last order arg)
   theSignal.notify(&mduObject, &MethodDelegateUsage::memberFunctionAsDelegate, 0.1f);

   // bang!
   theSignal.trigger(2048);

   // Done
   std::cout << std::endl << std::endl << "Press enter key to exit" << std::endl;
   std::cin.get();
   return 0;
}