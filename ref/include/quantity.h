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

#include <limits>

namespace units {

  class quantity {
    double value_;

  public:
    constexpr quantity();
    constexpr quantity(const quantity& q);
    constexpr explicit quantity(const double& v);

    constexpr quantity& operator=(const quantity& other);

    [[nodiscard]] constexpr double count() const noexcept;

    [[nodiscard]] static constexpr quantity zero();
    [[nodiscard]] static constexpr quantity min();
    [[nodiscard]] static constexpr quantity max();

    [[nodiscard]] constexpr quantity operator+() const;
    [[nodiscard]] constexpr quantity operator-() const;

    constexpr quantity& operator+=(const quantity& q);
    constexpr quantity& operator-=(const quantity& q);
    constexpr quantity& operator*=(const double& v);
    constexpr quantity& operator/=(const double& v);

    [[nodiscard]] friend constexpr quantity operator+(const quantity& lhs, const quantity& rhs);
    [[nodiscard]] friend constexpr quantity operator-(const quantity& lhs, const quantity& rhs);
    [[nodiscard]] friend constexpr quantity operator*(const quantity& q, const double& v);
    [[nodiscard]] friend constexpr quantity operator*(const double& v, const quantity& q);
    [[nodiscard]] friend constexpr quantity operator/(const quantity& q, const double& v);
    [[nodiscard]] friend constexpr double operator/(const quantity& lhs, const quantity& rhs);

    [[nodiscard]] friend constexpr bool operator==(const quantity& lhs, const quantity& rhs);
    [[nodiscard]] friend constexpr bool operator!=(const quantity& lhs, const quantity& rhs);
    [[nodiscard]] friend constexpr bool operator<(const quantity& lhs, const quantity& rhs);
    [[nodiscard]] friend constexpr bool operator<=(const quantity& lhs, const quantity& rhs);
    [[nodiscard]] friend constexpr bool operator>(const quantity& lhs, const quantity& rhs);
    [[nodiscard]] friend constexpr bool operator>=(const quantity& lhs, const quantity& rhs);
  };

}  // namespace units
