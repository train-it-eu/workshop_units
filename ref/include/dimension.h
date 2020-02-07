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

#include "type_list.h"

namespace units {

  // dim_id

  template<int UniqueValue>
  using dim_id = std::integral_constant<int, UniqueValue>;

  // dim_id_less

  template<typename D1, typename D2>
  struct dim_id_less : std::bool_constant<D1::value < D2::value> {};

  // exp

  template<typename BaseDimension, int Value>
  struct exp {
    using dimension = BaseDimension;
    static constexpr int value = Value;
  };

  // exp_less

  template<typename E1, typename E2>
  struct exp_less : dim_id_less<typename E1::dimension, typename E2::dimension> {};

  // exp_invert

  namespace detail {

    template<typename Exponent>
    struct exp_invert_impl;

    template<typename BaseDimension, int Value>
    struct exp_invert_impl<exp<BaseDimension, Value>> {
      using type = exp<BaseDimension, -Value>;
    };

  }

  template<typename Exponent>
  using exp_invert = typename detail::exp_invert_impl<Exponent>::type;

  // dimension

  template<typename... Exponents>
  struct dimension;

  // is_dimension

  template<typename T>
  inline constexpr bool is_dimension = false;

  template<typename... Exponents>
  inline constexpr bool is_dimension<dimension<Exponents...>> = true;

  // make_dimension
  namespace detail {

    template<typename D>
    struct dim_consolidate;

    template<>
    struct dim_consolidate<dimension<>> {
      using type = dimension<>;
    };

    template<typename E>
    struct dim_consolidate<dimension<E>> {
      using type = dimension<E>;
    };

    template<typename E1, typename... ERest>
    struct dim_consolidate<dimension<E1, ERest...>> {
      using type = type_list_push_front<typename dim_consolidate<dimension<ERest...>>::type, E1>;
    };

    template<typename D, int V1, int V2, typename... ERest>
    struct dim_consolidate<dimension<exp<D, V1>, exp<D, V2>, ERest...>> {
      using type = std::conditional_t<V1 + V2 == 0, typename dim_consolidate<dimension<ERest...>>::type,
                                      typename dim_consolidate<dimension<exp<D, V1 + V2>, ERest...>>::type>;
    };

  }

  template<typename... Es>
  using make_dimension = typename detail::dim_consolidate<type_list_sort<dimension<Es...>, exp_less>>::type;

}  // namespace units
