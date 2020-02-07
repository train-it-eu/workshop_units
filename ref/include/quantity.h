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

#include "common_ratio.h"
#include "unit.h"
#include <limits>
#include <type_traits>

// Requires

template<bool B>
using Requires = std::enable_if_t<B, bool>;

namespace units {

  // treat_as_floating_point

  template<class Rep>
  inline constexpr bool treat_as_floating_point = std::is_floating_point_v<Rep>;

  // quantity_values

  template<typename Rep>
  struct quantity_values {
    static constexpr Rep zero() { return Rep(0); }
    static constexpr Rep max() { return std::numeric_limits<Rep>::max(); }
    static constexpr Rep min() { return std::numeric_limits<Rep>::lowest(); }
  };

  // is_quantity

  template<typename Unit, typename Rep>
  class quantity;

  template<typename T>
  inline constexpr bool is_quantity = false;

  template<typename Rep, class Ratio>
  inline constexpr bool is_quantity<quantity<Rep, Ratio>> = true;

  // quantity_cast

  namespace detail {

    template<typename To, typename CRatio, typename CRep, bool NumIsOne = false, bool DenIsOne = false>
    struct quantity_cast_impl {
      template<typename Unit, typename Rep>
      static constexpr To cast(const quantity<Unit, Rep>& q)
      {
        return To(static_cast<typename To::rep>(static_cast<CRep>(q.count()) * static_cast<CRep>(CRatio::num) /
                                                static_cast<CRep>(CRatio::den)));
      }
    };

    template<typename To, typename CRatio, typename CRep>
    struct quantity_cast_impl<To, CRatio, CRep, true, true> {
      template<typename Unit, typename Rep>
      static constexpr To cast(const quantity<Unit, Rep>& q)
      {
        return To(static_cast<typename To::rep>(q.count()));
      }
    };

    template<typename To, typename CRatio, typename CRep>
    struct quantity_cast_impl<To, CRatio, CRep, true, false> {
      template<typename Unit, typename Rep>
      static constexpr To cast(const quantity<Unit, Rep>& q)
      {
        return To(static_cast<typename To::rep>(static_cast<CRep>(q.count()) / static_cast<CRep>(CRatio::den)));
      }
    };

    template<typename To, typename CRatio, typename CRep>
    struct quantity_cast_impl<To, CRatio, CRep, false, true> {
      template<typename Unit, typename Rep>
      static constexpr To cast(const quantity<Unit, Rep>& q)
      {
        return To(static_cast<typename To::rep>(static_cast<CRep>(q.count()) * static_cast<CRep>(CRatio::num)));
      }
    };

  }  // namespace detail

  template<typename To, typename Unit, typename Rep,
           Requires<is_quantity<To>> = true>
  [[nodiscard]] constexpr To quantity_cast(const quantity<Unit, Rep>& q)
  {
    using c_ratio = std::ratio_divide<typename Unit::ratio, typename To::unit::ratio>;
    using c_rep = std::common_type_t<typename To::rep, Rep, intmax_t>;
    using cast = detail::quantity_cast_impl<To, c_ratio, c_rep, c_ratio::num == 1, c_ratio::den == 1>;
    return cast::cast(q);
  }

  // common_quantity

  template<typename Q1, typename Q2, typename Rep = std::common_type_t<typename Q1::rep, typename Q2::rep>>
  using common_quantity = quantity<unit<typename Q1::unit::dimension,
                                        common_ratio<typename Q1::unit::ratio, typename Q2::unit::ratio>>,
                                   Rep>;

  template<typename Q1, typename Q2>
  inline constexpr bool same_dim = false;

  template<typename D, typename R1, typename R2>
  inline constexpr bool same_dim<unit<D, R1>, unit<D, R2>> = true;

  // quantity

  template<typename Unit, typename Rep = double>
  class quantity {
    Rep value_{};

  public:
    using unit = Unit;
    using rep = Rep;

    static_assert(is_unit<Unit>, "Unit should be an instantiation of units::unit");
    static_assert(!is_quantity<Rep>, "rep cannot be a quantity");

    constexpr quantity() = default;
    constexpr quantity(const quantity& q) = default;

    template<typename Rep2,
             Requires<!is_quantity<Rep2> &&
                      std::is_convertible_v<Rep2, rep> &&
                      (treat_as_floating_point<rep> || !treat_as_floating_point<Rep2>)> = true>
    constexpr explicit quantity(const Rep2& v): value_(static_cast<rep>(v)) {}

    template<typename Unit2, typename Rep2,
             Requires<same_dim<unit, Unit2> &&
                      std::is_convertible_v<Rep2, rep> &&
                      (treat_as_floating_point<rep> ||
                       (std::ratio_divide<typename Unit2::ratio, typename unit::ratio>::den == 1 && !treat_as_floating_point<Rep2>))> = true>
    constexpr quantity(const quantity<Unit2, Rep2>& q) : value_(quantity_cast<quantity>(q).count()) {}

    constexpr quantity& operator=(const quantity& other) = default;

    [[nodiscard]] constexpr Rep count() const noexcept { return value_; }

    [[nodiscard]] static constexpr quantity zero() { return quantity(quantity_values<Rep>::zero()); }
    [[nodiscard]] static constexpr quantity min() { return quantity(quantity_values<Rep>::min()); }
    [[nodiscard]] static constexpr quantity max() { return quantity(quantity_values<Rep>::max()); }

    [[nodiscard]] constexpr quantity operator+() const { return *this; }
    [[nodiscard]] constexpr quantity operator-() const { return quantity(-count()); }

    constexpr quantity& operator++()
    {
      ++value_;
      return *this;
    }

    [[nodiscard]] constexpr quantity operator++(int) { return quantity(value_++); }

    constexpr quantity& operator--()
    {
      --value_;
      return *this;
    }

    [[nodiscard]] constexpr quantity operator--(int) { return quantity(value_--); }

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
    template<typename T = Rep,
             Requires<!treat_as_floating_point<T>> = true>
    constexpr quantity& operator%=(const quantity& q)
    {
      value_ %= q.count();
      return *this;
    }
    template<typename T = Rep,
             Requires<!treat_as_floating_point<T>> = true>
    constexpr quantity& operator%=(const Rep& v)
    {
      value_ %= v;
      return *this;
    }

    template<typename Unit2, typename Rep2,
             Requires<same_dim<unit, Unit2>> = true>
    [[nodiscard]] friend constexpr auto operator+(const quantity& lhs, const quantity<Unit2, Rep2>& rhs)
        -> common_quantity<quantity, quantity<Unit2, Rep2>, decltype(std::declval<Rep>() + std::declval<Rep2>())>
    {
      using ret = common_quantity<quantity, quantity<Unit2, Rep2>, decltype(lhs.count() + rhs.count())>;
      return ret(ret(lhs).count() + ret(rhs).count());
    }

    template<typename Unit2, typename Rep2,
             Requires<same_dim<unit, Unit2>> = true>
    [[nodiscard]] friend constexpr auto operator-(const quantity& lhs, const quantity<Unit2, Rep2>& rhs)
        -> common_quantity<quantity, quantity<Unit2, Rep2>, decltype(std::declval<Rep>() - std::declval<Rep2>())>
    {
      using ret = common_quantity<quantity, quantity<Unit2, Rep2>, decltype(lhs.count() - rhs.count())>;
      return ret(ret(lhs).count() - ret(rhs).count());
    }

    template<typename Rep2,
             Requires<!is_quantity<Rep2>> = true>
    [[nodiscard]] friend constexpr auto operator*(const quantity& q, const Rep2& v)
        -> quantity<Unit, decltype(std::declval<Rep>() * std::declval<Rep2>())>
    {
      using ret = quantity<Unit, decltype(q.count() * v)>;
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
        -> quantity<Unit, decltype(std::declval<Rep>() / std::declval<Rep2>())>
    {
      using ret = quantity<Unit, decltype(q.count() / v)>;
      return ret(q.count() / v);
    }

    template<typename Rep2,
             Requires<!is_quantity<Rep2>> = true>
    [[nodiscard]] friend constexpr auto operator/(const Rep2& v, const quantity& q)
        -> quantity<units::unit<dim_invert<typename unit::dimension>, typename unit::ratio>, decltype(std::declval<Rep2>() / std::declval<Rep>())>
    {
      using ret = quantity<units::unit<dim_invert<typename unit::dimension>, typename unit::ratio>, decltype(v / q.count())>;
      return ret(v / q.count());
    }

    template<typename Unit2, typename Rep2,
             Requires<same_dim<unit, Unit2>> = true>
    [[nodiscard]] friend constexpr auto operator/(const quantity& lhs, const quantity<Unit2, Rep2>& rhs)
        -> decltype(std::declval<Rep>() / std::declval<Rep2>())
    {
      using cq = common_quantity<quantity, quantity<Unit2, Rep2>, decltype(lhs.count() / rhs.count())>;
      return cq(lhs).count() / cq(rhs).count();
    }

    template<typename Rep2, typename T = Rep,
             Requires<!is_quantity<Rep2> &&
                      !treat_as_floating_point<T> &&
                      !treat_as_floating_point<Rep2>> = true>
    [[nodiscard]] friend constexpr quantity operator%(const quantity& q, const Rep2& v)
    {
      return quantity(q.count() % v);
    }

    template<typename Unit2, typename Rep2, typename T = Rep,
             Requires<same_dim<unit, Unit2> &&
                      !treat_as_floating_point<T> &&
                      !treat_as_floating_point<Rep2>> = true>
    [[nodiscard]] friend constexpr auto operator%(const quantity& lhs, const quantity<Unit2, Rep2>& rhs)
        -> common_quantity<quantity, quantity<Unit2, Rep2>, decltype(std::declval<Rep>() % std::declval<Rep2>())>
    {
      using ret = common_quantity<quantity, quantity<Unit2, Rep2>, decltype(lhs.count() % rhs.count())>;
      return ret(ret(lhs).count() % ret(rhs).count());
    }

    template<typename Unit2, typename Rep2,
             Requires<same_dim<unit, Unit2>> = true>
    [[nodiscard]] friend constexpr bool operator==(const quantity& lhs, const quantity<Unit2, Rep2>& rhs)
    {
      using cq = common_quantity<quantity, quantity<Unit2, Rep2>>;
      return cq(lhs).count() == cq(rhs).count();
    }

    template<typename Unit2, typename Rep2,
             Requires<same_dim<unit, Unit2>> = true>
    [[nodiscard]] friend constexpr bool operator!=(const quantity& lhs, const quantity<Unit2, Rep2>& rhs)
    {
      return !(lhs == rhs);
    }

    template<typename Unit2, typename Rep2,
             Requires<same_dim<unit, Unit2>> = true>
    [[nodiscard]] friend constexpr bool operator<(const quantity& lhs, const quantity<Unit2, Rep2>& rhs)
    {
      using cq = common_quantity<quantity, quantity<Unit2, Rep2>>;
      return cq(lhs).count() < cq(rhs).count();
    }

    template<typename Unit2, typename Rep2,
             Requires<same_dim<unit, Unit2>> = true>
    [[nodiscard]] friend constexpr bool operator<=(const quantity& lhs, const quantity<Unit2, Rep2>& rhs)
    {
      return !(rhs < lhs);
    }

    template<typename Unit2, typename Rep2,
             Requires<same_dim<unit, Unit2>> = true>
    [[nodiscard]] friend constexpr bool operator>(const quantity& lhs, const quantity<Unit2, Rep2>& rhs)
    {
      return rhs < lhs;
    }

    template<typename Unit2, typename Rep2,
             Requires<same_dim<unit, Unit2>> = true>
    [[nodiscard]] friend constexpr bool operator>=(const quantity& lhs, const quantity<Unit2, Rep2>& rhs)
    {
      return !(lhs < rhs);
    }
  };

}  // namespace units
