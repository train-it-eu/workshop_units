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
#include <type_traits>

// Requires

template<bool B>
using Requires = std::enable_if_t<B, bool>;

namespace units {

  // is_quantity

  template<typename Rep>
  class quantity;

  template<typename T>
  inline constexpr bool is_quantity = false;

  template<typename Rep>
  inline constexpr bool is_quantity<quantity<Rep>> = true;

  template<typename Rep = double>
  class quantity {
    Rep value_{};

  public:
    using rep = Rep;
    static_assert(!is_quantity<Rep>, "rep cannot be a quantity");

    constexpr quantity() = default;
    constexpr quantity(const quantity& q) = default;

    template<typename Rep2,
             Requires<!is_quantity<Rep2>> = true>
    constexpr explicit quantity(const Rep2& v): value_(static_cast<rep>(v)) {}

    constexpr quantity& operator=(const quantity& other) = default;

    [[nodiscard]] constexpr Rep count() const noexcept { return value_; }

    [[nodiscard]] static constexpr quantity zero() { return quantity(0); }
    [[nodiscard]] static constexpr quantity min() { return quantity(std::numeric_limits<Rep>::lowest()); }
    [[nodiscard]] static constexpr quantity max() { return quantity(std::numeric_limits<Rep>::max()); }

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
    constexpr quantity& operator*=(const Rep& v)
    {
      value_ *= v;
      return *this;
    }
    constexpr quantity& operator/=(const Rep& v)
    {
      value_ /= v;
      return *this;
    }

    template<typename Rep2>
    [[nodiscard]] friend constexpr auto operator+(const quantity& lhs, const quantity<Rep2>& rhs)
        -> quantity<decltype(std::declval<Rep>() + std::declval<Rep2>())>
    {
      using ret = quantity<decltype(lhs.count() + rhs.count())>;
      return ret(lhs.count() + rhs.count());
    }

    template<typename Rep2>
    [[nodiscard]] friend constexpr auto operator-(const quantity& lhs, const quantity<Rep2>& rhs)
        -> quantity<decltype(std::declval<Rep>() - std::declval<Rep2>())>
    {
      using ret = quantity<decltype(lhs.count() - rhs.count())>;
      return ret(lhs.count() - rhs.count());
    }

    template<typename Rep2,
             Requires<!is_quantity<Rep2>> = true>
    [[nodiscard]] friend constexpr auto operator*(const quantity& q, const Rep2& v)
        -> quantity<decltype(std::declval<Rep>() * std::declval<Rep2>())>
    {
      using ret = quantity<decltype(q.count() * v)>;
      return ret(q.count() * v);
    }

    template<typename Rep2,
             Requires<!is_quantity<Rep2>> = true>
    [[nodiscard]] friend constexpr auto operator*(const Rep2& v, const quantity& q) -> decltype(q * v)
    {
      return q * v;
    }

    template<typename Rep2,
             Requires<!is_quantity<Rep2>> = true>
    [[nodiscard]] friend constexpr auto operator/(const quantity& q, const Rep2& v)
        -> quantity<decltype(std::declval<Rep>() / std::declval<Rep2>())>
    {
      using ret = quantity<decltype(q.count() / v)>;
      return ret(q.count() / v);
    }

    template<typename Rep2>
    [[nodiscard]] friend constexpr auto operator/(const quantity& lhs, const quantity<Rep2>& rhs)
        -> decltype(std::declval<Rep>() / std::declval<Rep2>())
    {
      return lhs.count() / rhs.count();
    }

    template<typename Rep2>
    [[nodiscard]] friend constexpr bool operator==(const quantity& lhs, const quantity<Rep2>& rhs)
    {
      return lhs.count() == rhs.count();
    }

    template<typename Rep2>
    [[nodiscard]] friend constexpr bool operator!=(const quantity& lhs, const quantity<Rep2>& rhs)
    {
      return !(lhs == rhs);
    }

    template<typename Rep2>
    [[nodiscard]] friend constexpr bool operator<(const quantity& lhs, const quantity<Rep2>& rhs)
    {
      return lhs.count() < rhs.count();
    }

    template<typename Rep2>
    [[nodiscard]] friend constexpr bool operator<=(const quantity& lhs, const quantity<Rep2>& rhs)
    {
      return !(rhs < lhs);
    }

    template<typename Rep2>
    [[nodiscard]] friend constexpr bool operator>(const quantity& lhs, const quantity<Rep2>& rhs)
    {
      return rhs < lhs;
    }

    template<typename Rep2>
    [[nodiscard]] friend constexpr bool operator>=(const quantity& lhs, const quantity<Rep2>& rhs)
    {
      return !(lhs < rhs);
    }
  };

}  // namespace units
