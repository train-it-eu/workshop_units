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
#include "time.h"

namespace units {

  using dimension_frequency = make_dimension<exp<base_dim_time, -1>>;

  using millihertz = unit<dimension_frequency, std::milli>;
  using hertz = unit<dimension_frequency, std::ratio<1>>;
  using kilohertz = unit<dimension_frequency, std::kilo>;
  using megahertz = unit<dimension_frequency, std::mega>;
  using gigahertz = unit<dimension_frequency, std::giga>;
  using terahertz = unit<dimension_frequency, std::tera>;

  inline namespace literals {

    // mHz
    constexpr auto operator""_mHz(unsigned long long l) { return quantity<millihertz, std::int64_t>(l); }
    constexpr auto operator""_mHz(long double l) { return quantity<millihertz, long double>(l); }

    // Hz
    constexpr auto operator""_Hz(unsigned long long l) { return quantity<hertz, std::int64_t>(l); }
    constexpr auto operator""_Hz(long double l) { return quantity<hertz, long double>(l); }

    // kHz
    constexpr auto operator""_kHz(unsigned long long l) { return quantity<kilohertz, std::int64_t>(l); }
    constexpr auto operator""_kHz(long double l) { return quantity<kilohertz, long double>(l); }

    // MHz
    constexpr auto operator""_MHz(unsigned long long l) { return quantity<megahertz, std::int64_t>(l); }
    constexpr auto operator""_MHz(long double l) { return quantity<megahertz, long double>(l); }

    // GHz
    constexpr auto operator""_GHz(unsigned long long l) { return quantity<gigahertz, std::int64_t>(l); }
    constexpr auto operator""_GHz(long double l) { return quantity<gigahertz, long double>(l); }

    // THz
    constexpr auto operator""_THz(unsigned long long l) { return quantity<terahertz, std::int64_t>(l); }
    constexpr auto operator""_THz(long double l) { return quantity<terahertz, long double>(l); }

  }  // namespace literals

}  // namespace units
