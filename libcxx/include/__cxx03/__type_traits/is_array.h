//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___CXX03___TYPE_TRAITS_IS_ARRAY_H
#define _LIBCPP___CXX03___TYPE_TRAITS_IS_ARRAY_H

#include <__cxx03/__config>
#include <__cxx03/__type_traits/integral_constant.h>
#include <__cxx03/cstddef>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if __has_builtin(__is_array) &&                                                                                       \
    (!defined(_LIBCPP_COMPILER_CLANG_BASED) || (defined(_LIBCPP_CLANG_VER) && _LIBCPP_CLANG_VER >= 1900))

template <class _Tp>
struct _LIBCPP_TEMPLATE_VIS is_array : _BoolConstant<__is_array(_Tp)> {};

#else

template <class _Tp>
struct _LIBCPP_TEMPLATE_VIS is_array : public false_type {};
template <class _Tp>
struct _LIBCPP_TEMPLATE_VIS is_array<_Tp[]> : public true_type {};
template <class _Tp, size_t _Np>
struct _LIBCPP_TEMPLATE_VIS is_array<_Tp[_Np]> : public true_type {};

#endif // __has_builtin(__is_array)

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___CXX03___TYPE_TRAITS_IS_ARRAY_H
