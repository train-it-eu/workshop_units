// The MIT License (MIT)
//
// Copyright (c) 2018 Train IT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <type_traits>

namespace units {

  // push_front

  namespace detail {

    template<typename List, typename... Types>
    struct type_list_push_front_impl;

    template<template<typename...> typename List, typename... OldTypes, typename... NewTypes>
    struct type_list_push_front_impl<List<OldTypes...>, NewTypes...> {
      using type = List<NewTypes..., OldTypes...>;
    };

  }

  template<typename List, typename... Types>
  using type_list_push_front = typename detail::type_list_push_front_impl<List, Types...>::type;

  // push_back

  namespace detail {

    template<typename List, typename... Types>
    struct type_list_push_back_impl;

    template<template<typename...> typename List, typename... OldTypes, typename... NewTypes>
    struct type_list_push_back_impl<List<OldTypes...>, NewTypes...> {
      using type = List<OldTypes..., NewTypes...>;
    };

  }

  template<typename List, typename... Types>
  using type_list_push_back = typename detail::type_list_push_back_impl<List, Types...>::type;

  // split

  namespace detail {

    template<template<typename...> typename List, std::size_t Idx, std::size_t N, typename... Types>
    struct split_impl;

    template<template<typename...> typename List, std::size_t Idx, std::size_t N>
    struct split_impl<List, Idx, N> {
      using first_list = List<>;
      using second_list = List<>;
    };

    template<template<typename...> typename List, std::size_t Idx, std::size_t N, typename T, typename... Rest>
    struct split_impl<List, Idx, N, T, Rest...> : split_impl<List, Idx + 1, N, Rest...> {
      using base = split_impl<List, Idx + 1, N, Rest...>;
      using first_list = std::conditional_t<Idx < N,
                                            typename type_list_push_front_impl<typename base::first_list, T>::type,
                                            typename base::first_list>;
      using second_list = std::conditional_t<Idx < N,
                                             typename base::second_list,
                                             typename type_list_push_front_impl<typename base::second_list, T>::type>;
    };

  }  // namespace detail

  template<typename List, std::size_t N>
  struct type_list_split;

  template<template<typename...> typename List, std::size_t N, typename... Types>
  struct type_list_split<List<Types...>, N> {
    static_assert(N <= sizeof...(Types), "Invalid index provided");
    using split = detail::split_impl<List, 0, N, Types...>;
    using first_list = typename split::first_list;
    using second_list = typename split::second_list;
  };

}  // namespace units
