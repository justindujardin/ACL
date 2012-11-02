//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef CORE_UTIL_REFLECTION_H
#define CORE_UTIL_REFLECTION_H

// Make sure there aren't any lingering macro's defined... Normally only a
// problem if windows.h is included before this.
#undef min
#undef max

#include "core/types.h"

//-----------------------------------------------------------------------------

namespace ACLib
{

   // These helper defines are undefined at the end of this file.
#define TypeInfoTemplate(name,result) \
   template<typename T> struct name { static const bool True = result, False = !True; }
#define TypeInfoPartialSpec(name,type,result) \
   template<> struct name<type> { static const bool True = result, False = !True; }
#define TypeInfoPartialSpec1(name,type,spec,result) \
   template<type> struct name<spec> { static const bool True = result, False = !True; }
#define TypeInfoPartialSpec2(name,type1,type2,spec,result) \
   template<type1,type2> struct name<spec> { static const bool True = result, False = !True; }


   //-----------------------------------------------------------------------------

   //-----------------------------------------------------------------------------
   struct TrueType {
      static const bool True = true;
      static const bool False = false;
   };
   struct FalseType {
      static const bool True = false;
      static const bool False = true;
   };

   //-----------------------------------------------------------------------------
   /// Used to convert int or bool template arguments into types that can be
   /// used to specialize other templates.
   ///@ingroup UtilReflection
   template<int V>
   struct IntToType {
      enum { Value = V };
   };

   //-----------------------------------------------------------------------------
   /// Type selection. Selects type T or U based on the bool flag
   ///@ingroup UtilReflection
   template<bool flag, typename T, typename U>
   struct Select {
      typedef T ValueType;
   };
   template<typename T, typename U>
   struct Select<false,T,U> {
      typedef U ValueType;
   };


   //-----------------------------------------------------------------------------
   // type_limits<T>
   /// Defines min and max as the smallest and largest possible values which
   /// can be stored by this type.
   ///@ingroup UtilReflection
   template<typename T>
   struct NumericLimits
   {
      static const bool IsInteger = false;     ///< Is integer or floating point?
      static const bool IsExact = false;       ///< Represents exact value or not?
      static int min() { return 0; }            ///< Smallest possible value for this type
      static int max() { return 0; }            ///< Largest possible value for this type
      static int epsilon() { return 0; }        ///< Difference between 1 and the least value greater than 1 possible
   };

#define TypeLimitTemplate(Type,Min,Max,Integer,Epsilon) \
   template<> struct NumericLimits<Type> { \
   static const bool IsInteger = Integer,IsExact = IsInteger; \
   static Type min() { return Min; } \
   static Type max() { return Max; } \
   static Type epsilon() { return Epsilon; } \
   };

   TypeLimitTemplate(S8,-128,+127,true,0);
   TypeLimitTemplate(S16,-32768,+32767,true,0);
   TypeLimitTemplate(S32,-2147483647L-1,+2147483647L,true,0);
   //TypeLimitTemplate(S64,-ACL_CONSTANT_S64(9223372036854775807)-1,ACL_CONSTANT_S64(+9223372036854775807),true,0);
   TypeLimitTemplate(U8, 0,+255U,true,0);
   TypeLimitTemplate(U16,0,+65535U,true,0);
   TypeLimitTemplate(U32,0,+4294967295U,true,0);
   //TypeLimitTemplate(U64,0,ACL_CONSTANT_U64(+18446744073709551615),true,0);

   TypeLimitTemplate(F32,1.17549435e-38F,3.4028234e+38F,false,1.19209290e-07F);
   TypeLimitTemplate(F64,2.2250738585072014e-308,1.7976931348623157e+308,false,2.2204460492503131e-16);
#undef TypeLimitTemplate


   //-----------------------------------------------------------------------------
   /// Defines class traits.
   /// Class traits include:
   ///   <dl>
   ///      <dt>IsPod<dd>POD classes are "Plain Old Data". They have trivial constructors,
   ///         destructors and assignment operators as well as no complex data member.
   ///      <dt>HasTrivialConstructor<dd>This constructor on this class does
   ///         nothing and can be skipped.
   ///      <dt>HasTrivialDestructor<dd>This destructor on this class does
   ///         nothing and can be skipped.
   ///      <dt>HasTrivialAssign<dd>This class can be assigned using memcpy instead of
   ///         invoking the assignment operator.
   ///      <dt>HasTrivialCopy<dd>This class doesn't care where it's located in memory
   ///         and can be moved using memcpy.
   ///   </dl>
   ///@ingroup UtilReflection
   template<typename T>
   struct ClassTraits
   {
      static const bool IsPod = false;
      static const bool HasTrivialConstructor  = false;
      static const bool HasTrivialDestructor = false;
      static const bool HasTrivialAssign = false;
      static const bool HasTrivialCopy = false;
   };

   //-----------------------------------------------------------------------------
   // IteratorTraits<T>
   /// Defines iterator traits.
   /// Iterator traits include:
   ///   <dl>
   ///      <dt>ValueType<dd>The type which is being iterated over.
   ///      <dt>Pointer<dd>A pointer to the ValueType.
   ///      <dt>Reference<dd>A reference to the ValueType.
   ///   </dl>
   ///@ingroup UtilReflection
   template<typename T>
   struct IteratorTraits {
      typedef typename T::ValueType    ValueType;
      typedef typename T::Pointer      Pointer;
      typedef typename T::Reference    Reference;
   };

   template<typename T>
   struct IteratorTraits<T*> {
      typedef T                        ValueType;
      typedef T*                       Pointer;
      typedef T&                       Reference;
   };

   template<typename T>
   struct IteratorTraits<const T*> {
      typedef T                        ValueType;
      typedef const T*                 Pointer;
      typedef const T&                 Reference;
   };


   //-----------------------------------------------------------------------------
   // BooleanType<T>
   /// Used to convert true/false values into TrueType & FalseType
   ///@ingroup UtilReflection
   template<bool>
   struct BooleanType {
      typedef FalseType ValueType;
   };
   template<> struct BooleanType<true> {
      typedef TrueType ValueType;
   };

   //-----------------------------------------------------------------------------
   // IsConvertible<T,U>
   namespace Private
   {
      template<typename T>
      struct IsConvertible_aux {
         struct anything {
            template <typename U> anything(const volatile U&);
            template <typename U> anything(U&);
         };
         typedef U16 Yes;
         typedef U32 No;
         static Yes test(T,int);
         static No  test(anything,...);
      };
   }

   /// Evaluates to true if type T is convertible to U
   ///@ingroup UtilReflection
   template<typename T,typename U>
   struct IsConvertible {
      static T from;
      static const bool True =
         sizeof(Private::IsConvertible_aux<U>::test(from,0)) ==
         sizeof(typename Private::IsConvertible_aux<U>::Yes);
      static const bool False = !True;
   };

   //-----------------------------------------------------------------------------
   // IsSame<T,U>
   /// Evaluates to true if the types are the same.
   ///@ingroup UtilReflection
   template<typename T,typename U> struct IsSame
   { static const bool True = false, False = true; };
   template<typename T> struct IsSame<T,T>
   { static const bool True = true, False = false; };

   //-----------------------------------------------------------------------------
   // IsIntegral<T>
   /// Evaluates to true if the type is one of the integral types.
   ///@ingroup UtilReflection
   TypeInfoTemplate(IsIntegral,false);
   TypeInfoPartialSpec(IsIntegral,S8,true);
   TypeInfoPartialSpec(IsIntegral,U8,true);
   TypeInfoPartialSpec(IsIntegral,S16,true);
   TypeInfoPartialSpec(IsIntegral,U16,true);
   TypeInfoPartialSpec(IsIntegral,S32,true);
   TypeInfoPartialSpec(IsIntegral,U32,true);
   TypeInfoPartialSpec(IsIntegral,S64,true);
   TypeInfoPartialSpec(IsIntegral,U64,true);

   //-----------------------------------------------------------------------------
   // IsFloat<T>
   /// Evaluates to true if the type is either an F32 or F64
   ///@ingroup UtilReflection
   TypeInfoTemplate(IsFloat,false);
   TypeInfoPartialSpec(IsFloat,F32,true);
   TypeInfoPartialSpec(IsFloat,F64,true);

   //-----------------------------------------------------------------------------
   // IsArithmetic<T>
   /// Evaluates to true if the type is either an integral or floating point type
   ///@ingroup UtilReflection
   template<typename T> struct IsArithmetic {
      static const bool True =
         IsIntegral<T>::True ||
         IsFloat<T>::True;
      static const bool False = !True;
   };

   //-----------------------------------------------------------------------------
   // IsVoid<T>
   /// Evaluates to true if the type is type "void"
   ///@ingroup UtilReflection
   TypeInfoTemplate(IsVoid,false);
   TypeInfoPartialSpec(IsVoid,void,true);

   //-----------------------------------------------------------------------------
   // IsArray<T>
   /// Evaluates to true if the type is an array
   ///@ingroup UtilReflection
   TypeInfoTemplate(IsArray,false);
   TypeInfoPartialSpec2(IsArray,typename T,size_t N,T[N],true);
   TypeInfoPartialSpec2(IsArray,typename T,size_t N,const T[N],true);
   TypeInfoPartialSpec2(IsArray,typename T,size_t N,volatile T[N],true);
   TypeInfoPartialSpec2(IsArray,typename T,size_t N,const volatile T[N],true);

   //-----------------------------------------------------------------------------
   // IsReference<T>
   /// Evaluates to true if the type is a Reference
   ///@ingroup UtilReflection
   TypeInfoTemplate(IsReference,false);
   TypeInfoPartialSpec1(IsReference,typename T,T&,true);

   //-----------------------------------------------------------------------------
   // IsMemberPointer<T>
   /// Evaluates to true if T is a pointer to a data member or member function
   ///@ingroup UtilReflection
   TypeInfoTemplate(IsMemberPointer,false);
   TypeInfoPartialSpec2(IsMemberPointer,typename T,typename U,U T::*,true);

   //-----------------------------------------------------------------------------
   /// Evaluates to true if T is a function, or if T is an abstract type
   ///@ingroup UtilReflection
   template<typename T>
   struct IsFunction {
      typedef U16 Yes;
      typedef U32 No;
      template <typename U> static Yes test(...);
      template <typename U> static No test(U (*)[1]);
      // Public interface.
      static const bool True = sizeof(test<T>(0)) == sizeof(Yes);
      static const bool False = !True;
   };

   TypeInfoPartialSpec1(IsFunction,typename T,T&,false);
   TypeInfoPartialSpec(IsFunction,void,false);
   TypeInfoPartialSpec(IsFunction,void const,false);

   template<typename T>
   struct IsAbstract : public IsFunction<T>
   {
   };

   //-----------------------------------------------------------------------------
   // IsPointer<T>
   namespace Private
   {
      TypeInfoTemplate(IsPointer_aux,false);
      TypeInfoPartialSpec1(IsPointer_aux,typename T,T*,true);
      TypeInfoPartialSpec1(IsPointer_aux,typename T,T* const,true);
      TypeInfoPartialSpec1(IsPointer_aux,typename T,T* volatile,true);
      TypeInfoPartialSpec1(IsPointer_aux,typename T,T* const volatile,true);
   }

   /// Evaluates to true if T is a pointer type. This includes function pointers
   /// but excludes member pointers
   ///@ingroup UtilReflection
   template<typename T> struct IsPointer {
      static const bool True =
         Private::IsPointer_aux<T>::True &&
         IsMemberPointer<T>::False;
      static const bool False = !True;
   };

   //-----------------------------------------------------------------------------
   // IsEnum
   namespace Private {
      // Split convertible test out to avoid compiler warnings when testing to see if
      // floats are enums.
      template<typename T,bool> struct IsEnumConvertible {
         static const bool True = false;
      };
      template<typename T> struct IsEnumConvertible<T,false> {
         static const bool True = IsConvertible<T,int>::True;
      };
   }

   /// Evaluates to true if the type is an enum
   ///@ingroup UtilReflection
   template<typename T> struct IsEnum {
      //   static const bool True = false;
      static const bool True = Private::IsEnumConvertible<T,
         IsArithmetic<T>::True ||
         IsPointer<T>::True ||
         IsFunction<T>::True ||
         IsReference<T>::True
      >::True;
      static const bool False = !True;
   };

   //-----------------------------------------------------------------------------
   // IsScalar
   /// Evaluates to true if the type is an arithmetic, enum or pointer type
   ///@ingroup UtilReflection
   template<typename T>
   struct IsScalar {
      static const bool True =
         IsArithmetic<T>::True ||
         IsEnum<T>::True ||
         IsPointer<T>::True ||
         IsMemberPointer<T>::True;
      static const bool False = !True;
   };

   //-----------------------------------------------------------------------------
   // IsPod
   /// Evaluates to true if the type is "Plain Old Data"
   ///@ingroup UtilReflection
   template<typename T>
   struct IsPod
   {
      static const bool True =
         IsScalar<T>::True ||
         IsVoid<T>::True ||
         ClassTraits<T>::IsPod;
      static const bool False = !True;
   };
   template<typename T, size_t N>
   struct IsPod<T[N]>: IsPod<T> {};

   //-----------------------------------------------------------------------------
   // HasTrivialConstructor<T>
   /// Evaluates to true if calls to the type's constructor can be skipped
   ///@ingroup UtilReflection
   template<typename T> struct HasTrivialConstructor
   {
      static const bool True =
         IsPod<T>::True ||
         ClassTraits<T>::HasTrivialConstructor;
      static const bool False = !True;
   };

   // HasTrivialDestructor<T>
   /// Evaluates to true if calls to the type's destructor can be skipped
   ///@ingroup UtilReflection
   template<typename T> struct HasTrivialDestructor
   {
      static const bool True =
         IsPod<T>::True ||
         ClassTraits<T>::HasTrivialDestructor;
      static const bool False = !True;
   };

   // HasTrivialAssign<T>
   /// Evaluates to true if the type can by copied using memcpy instead of
   /// the assigment operator
   ///@ingroup UtilReflection
   template<typename T> struct HasTrivialAssign
   {
      static const bool True =
         IsPod<T>::True ||
         ClassTraits<T>::HasTrivialAssign;
      static const bool False = !True;
   };

   // HasTrivialCopy<T>
   /// Evaluates to true if the type can by moved in memory using memcpy
   ///@ingroup UtilReflection
   template<typename T> struct HasTrivialCopy
   {
      static const bool True =
         IsPod<T>::True ||
         ClassTraits<T>::HasTrivialCopy;
      static const bool False = !True;
   };

   //-----------------------------------------------------------------------------
   // unqualified_type
   namespace Private {
      template<typename T> struct UnqualifiedPtr { typedef T ValueType; };
      template<typename T> struct UnqualifiedPtr<T*> { typedef T ValueType; };
      template<typename T> struct UnqualifiedPtr<const T*> { typedef T ValueType; };
      template<typename T> struct UnqualifiedPtr<volatile T*> { typedef T ValueType; };
      template<typename T> struct UnqualifiedPtr<const volatile T*> { typedef T ValueType; };
   };

   /// Defines ValueType as the base type minus const and volatile modifiers
   ///@ingroup UtilReflection
   template<typename T> struct RemoveCV
   {
      typedef T ValueType;
   };
   template<typename T> struct RemoveCV<const T> { typedef T ValueType; };
   template<typename T> struct RemoveCV<volatile T> { typedef T ValueType; };
   template<typename T> struct RemoveCV<const volatile T> { typedef T ValueType; };
   template<typename T,size_t N> struct RemoveCV<T const[N]> { typedef T ValueType[N]; };
   template<typename T,size_t N> struct RemoveCV<T volatile[N]> { typedef T ValueType[N]; };
   template<typename T,size_t N> struct RemoveCV<T const volatile[N]> { typedef T ValueType[N]; };

   /// Detects if the given type is const.
   template<typename T> struct IsConst
   {
      static const bool True = false;
      static const bool False = true;
   };

   template<typename T> struct IsConst<const T>
   {
      static const bool True = true;
      static const bool False = false;
   };

   template<typename T, size_t N> struct IsConst<const T[N]>
   {
      static const bool True = true;
      static const bool False = false;
   };

   // Get rid of helper macros
#undef TypeInfoTemplate
#undef TypeInfoPartialSpec
#undef TypeInfoPartialSpec1
#undef TypeInfoPartialSpec2


   //-----------------------------------------------------------------------------

   template<typename T>
   struct TypeTraits
   {
      typedef typename T::ValueType ValueType;
   };
#define TypeTraitSpec(T) \
   template<> struct TypeTraits<T> { typedef T ValueType; }
   TypeTraitSpec(S8);
   TypeTraitSpec(S16);
   TypeTraitSpec(S32);
   TypeTraitSpec(S64);
   TypeTraitSpec(U8);
   TypeTraitSpec(U16);
   TypeTraitSpec(U32);
   TypeTraitSpec(U64);
   TypeTraitSpec(F32);
   TypeTraitSpec(F64);
#undef TypeTraitSpec


   //-----------------------------------------------------------------------------
   //-----------------------------------------------------------------------------

   /// Computes the number of elements in a static array. 
   template <typename T, size_t N>
   inline U32 ArraySize(T (&a)[N]) {
      return N;
   }

   /// Computes the number of elements in a static array.
   /// This is a type safe macro that will can be used in places where
   /// the ArraySize template cannot be evaluated.
   namespace Private {
      template <typename T, size_t N>
      char ( &_ArraySize( T (&)[N] ))[N];
   }
#define ACLibArraySize(array) sizeof(Private::_ArraySize(array))


} // Namespace

#endif





