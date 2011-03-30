#include "../lib/core/types/types.h"
#include "../lib/core/containers/tVector.h"
#include "../lib/core/containers/tList.h"
#include "../lib/core/containers/tPair.h"
#include<iostream>

//-----------------------------------------------------------------------------
// Basic Containers/Types integration
//-----------------------------------------------------------------------------
S32 main(void)
{
   // Vector
   Vector<S32> intVec;
   intVec.push_back(1337);
   intVec.increment();
   intVec.last() = 12;
   intVec.push_front(141);

   std::cout << "Application Core Libarary - Container Test" << std::endl;
   std::cout << std::endl << std::endl << "Vector<S32> intVec[" << intVec.size() << "];" << std::endl << std::endl;
   for( S32 i = 0; i < intVec.size(); i++ )
      std::cout << "intVec[" << i << "] = " << intVec[i] << std::endl;

   // List
   Torque::List<const char *> nameList;
   static const char *names[] = { "bill", "patricia", "rodney", "colbert", "robert", NULL };
   S32 i = 0;
   const char *nameIter = names[i];
   while(nameIter != NULL)
   {
      nameList.pushBack(nameIter);
      nameIter = names[i++];
   }
   std::cout << std::endl << std::endl << "Torque::List<const char*> nameList;" << std::endl << std::endl;
   for( i = 0; i < (S32)nameList.getSize(); i++ )
   {
      const char* theName = nameList[i];
      std::cout << "[" << theName << "]" << std::endl;
   }

   // Pair
   Pair<S32,F32> intFloat = MakePair<S32,F32>(1337,1.337f);
   std::cout << std::endl << std::endl << "Torque::Pair<S32,F32> intFloat = MakePair<S32,F32>(1337,1.337f);" << std::endl << std::endl;
   std::cout << "intFloat.first = " << intFloat.first << std::endl; 
   std::cout << "intFloat.second = " << intFloat.second << std::endl;

   // Done
   std::cout << std::endl << std::endl << "Press enter key to exit" << std::endl;
   std::cin.get();
   return 0;
}