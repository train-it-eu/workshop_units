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

#include "base_dimensions.h"
#include "quantity.h"

namespace units {

  using dimension_length = make_dimension<exp<base_dim_length, 1>>; 

  using millimetre = unit<dimension_length, std::milli>;
  using metre = unit<dimension_length, std::ratio<1>>;
  using kilometre = unit<dimension_length, std::kilo>;

  inline namespace literals {

    // mm
    constexpr auto operator""_mm(unsigned long long l) { return quantity<millimetre, std::int64_t>(l); }
    constexpr auto operator""_mm(long double l) { return quantity<millimetre, long double>(l); }

    // m
    constexpr auto operator""_m(unsigned long long l) { return quantity<metre, std::int64_t>(l); }
    constexpr auto operator""_m(long double l) { return quantity<metre, long double>(l); }

    // km
    constexpr auto operator""_km(unsigned long long l) { return quantity<kilometre, std::int64_t>(l); }
    constexpr auto operator""_km(long double l) { return quantity<kilometre, long double>(l); }

  }  // namespace literals

}  // namespace units
