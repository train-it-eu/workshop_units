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
    double value_{};

  public:
    constexpr quantity() = default;
    constexpr quantity(const quantity& q) = default;
    constexpr explicit quantity(const double& v): value_(v) {}

    constexpr quantity& operator=(const quantity& other) = default;

    [[nodiscard]] constexpr double count() const noexcept { return value_; }

    [[nodiscard]] static constexpr quantity zero() { return quantity(0); }
    [[nodiscard]] static constexpr quantity min() { return quantity(std::numeric_limits<double>::lowest()); }
    [[nodiscard]] static constexpr quantity max() { return quantity(std::numeric_limits<double>::max()); }

    [[nodiscard]] constexpr quantity operator+() const { return *this; }
    [[nodiscard]] constexpr quantity operator-() const { return quantity(-count()); }

    constexpr quantity& operator+=(const quantity& q)
    {
      value_ += q.count();
      return *this;
    }
    constexpr quantity& operator-=(const quantity& q)
    {
      value_ -= q.count();
      return *this;
    }
    constexpr quantity& operator*=(const double& v)
    {
      value_ *= v;
      return *this;
    }
    constexpr quantity& operator/=(const double& v)
    {
      value_ /= v;
      return *this;
    }

    [[nodiscard]] friend constexpr quantity operator+(const quantity& lhs, const quantity& rhs)
    {
      return quantity(lhs.count() + rhs.count());
    }
    [[nodiscard]] friend constexpr quantity operator-(const quantity& lhs, const quantity& rhs)
    {
      return quantity(lhs.count() - rhs.count());
    }
    [[nodiscard]] friend constexpr quantity operator*(const quantity& q, const double& v)
    {
      return quantity(q.count() * v);
    }
    [[nodiscard]] friend constexpr quantity operator*(const double& v, const quantity& q) { return q * v; }
    [[nodiscard]] friend constexpr quantity operator/(const quantity& q, const double& v)
    {
      return quantity(q.count() / v);
    }
    [[nodiscard]] friend constexpr double operator/(const quantity& lhs, const quantity& rhs)
    {
      return lhs.count() / rhs.count();
    }

    [[nodiscard]] friend constexpr bool operator==(const quantity& lhs, const quantity& rhs)
    {
      return lhs.count() == rhs.count();
    }
    [[nodiscard]] friend constexpr bool operator!=(const quantity& lhs, const quantity& rhs) { return !(lhs == rhs); }
    [[nodiscard]] friend constexpr bool operator<(const quantity& lhs, const quantity& rhs)
    {
      return lhs.count() < rhs.count();
    }
    [[nodiscard]] friend constexpr bool operator<=(const quantity& lhs, const quantity& rhs) { return !(rhs < lhs); }
    [[nodiscard]] friend constexpr bool operator>(const quantity& lhs, const quantity& rhs) { return rhs < lhs; }
    [[nodiscard]] friend constexpr bool operator>=(const quantity& lhs, const quantity& rhs) { return !(lhs < rhs); }
  };

}  // namespace units