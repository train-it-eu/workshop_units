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

#include "dimension.h"
#include <ratio>
#include <type_traits>

namespace units {

  // is_ratio

  template<typename T>
  inline constexpr bool is_ratio = false;

  template<intmax_t Num, intmax_t Den>
  inline constexpr bool is_ratio<std::ratio<Num, Den>> = true;

  // unit

  template<typename Dimension, typename Ratio>
  struct unit {
    using dimension = Dimension;
    using ratio = Ratio;

    static_assert(is_dimension<dimension>, "dimension must be a specialization of units::dimension");
    static_assert(is_ratio<ratio>, "ratio must be a specialization of std::ratio");
    static_assert(ratio::num * ratio::den > 0, "ratio must be positive");
  };

  // is_unit

  template<typename T>
  inline constexpr bool is_unit = false;

  template<typename Dimension, typename Ratio>
  inline constexpr bool is_unit<unit<Dimension, Ratio>> = true;

}  // namespace units
