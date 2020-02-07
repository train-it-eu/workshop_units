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

#include "quantity.h"
#include <limits>
#include <utility>

namespace {

  template<typename T>
  class my_value {
    T value_{};
  public:
    my_value() = default;
    constexpr my_value(T v) : value_(std::move(v)) {}
    constexpr my_value& operator+=(const my_value& other) { value_ += other.value_; return *this; }
    constexpr my_value& operator-=(const my_value& other) { value_ -= other.value_; return *this; }
    constexpr my_value& operator*=(const my_value& other) { value_ *= other.value_; return *this; }
    constexpr my_value& operator/=(const my_value& other) { value_ /= other.value_; return *this; }
    constexpr operator const T&() const & { return value_; }
  };

} // namespace

namespace units {

  template<typename T>
  inline constexpr bool treat_as_floating_point<my_value<T>> = std::is_floating_point_v<T>;

  template<typename T>
  struct quantity_values<my_value<T>> {
    static constexpr my_value<T> zero() { return my_value<T>(0); }
    static constexpr my_value<T> max() { return std::numeric_limits<T>::max(); }
    static constexpr my_value<T> min() { return std::numeric_limits<T>::lowest(); }
  };

}  // namespace units

namespace {

  using namespace units;

  using metre = unit<std::ratio<1>>;
  using kilometre = unit<std::ratio<1000>>;

  template<typename Rep = double>
  using length = quantity<Rep>;

  constexpr quantity<metre, int> m{1};
  constexpr quantity<metre, int> km{1000};

  // class invariants

//  constexpr quantity<quantity<metre, int>> error(quantity<metre, int>(0));  // should trigger a static_assert
//  constexpr quantity<int, float> error(0);  // should trigger a static_assert
//  constexpr quantity<int, std::ratio<-1, 1>> error(0);  // should trigger a static_assert
//  constexpr quantity<int, std::ratio<1, -1>> error(0);  // should trigger a static_assert

  // member types

  static_assert(std::is_same_v<quantity<metre, int>::rep, int>);
  static_assert(std::is_same_v<quantity<metre, float>::rep, float>);
  static_assert(std::is_same_v<quantity<metre, int>::unit::ratio, std::ratio<1, 1>>);
  static_assert(std::is_same_v<quantity<kilometre, int>::unit::ratio, std::ratio<1000, 1>>);
 
   // constructors

  static_assert(quantity<metre>().count() == 0);
  static_assert(m.count() == 1);
  static_assert(km.count() == 1000);
  static_assert(quantity<metre>(km).count() == km.count());

  static_assert(quantity<metre, int>(1).count() == 1);
  static_assert(quantity<metre, int>(my_value<int>(1)).count() == 1);
  static_assert(quantity<metre, my_value<int>>(1).count() == 1);
//  static_assert(quantity<metre, int>(1.0).count() == 1);   // should not compile
//  static_assert(quantity<metre, int>(my_value<float>(1.0)).count() == 1); // should not compile
//  static_assert(quantity<metre, my_value<int>>(1.0).count() == 1);   // should not compile
  static_assert(quantity<metre, float>(1.0).count() == 1.0);
  static_assert(quantity<metre, float>(my_value<float>(1.0)).count() == 1.0);
  static_assert(quantity<metre, float>(1).count() == 1.0);
  static_assert(quantity<metre, float>(my_value<int>(1)).count() == 1.0);
  static_assert(quantity<metre, float>(3.14f).count() == 3.14f);
  static_assert(quantity<metre, my_value<float>>(1.0).count() == 1.0);
  static_assert(quantity<metre, my_value<float>>(1).count() == 1.0);
  static_assert(quantity<metre, my_value<float>>(3.14f).count() == 3.14f);

  static_assert(quantity<metre, int>(km).count() == 1000);
//  static_assert(quantity<metre, int>(quantity<metre, float>(3.14)).count() == 3);   // should not compile
  static_assert(quantity<metre, int>(quantity_cast<quantity<metre, int>>(quantity<metre, float>(3.14))).count() == 3);
//  static_assert(quantity<metre, int>(quantity<metre, my_value<float>>(1000.0)).count() == 1000);   // should not compile
//  static_assert(quantity<metre, my_value<int>>(quantity<metre, float>(1000.0)).count() == 1000);   // should not compile
  static_assert(quantity<metre, float>(quantity<metre, float>(1000.0)).count() == 1000.0);
  static_assert(quantity<metre, float>(quantity<metre, my_value<float>>(1000.0)).count() == 1000.0);
  static_assert(quantity<metre, my_value<float>>(quantity<metre, float>(1000.0)).count() == 1000.0);
  static_assert(quantity<metre, float>(km).count() == 1000.0);
  static_assert(quantity<metre, my_value<float>>(km).count() == 1000.0);

  // assignment operator

  auto test_assign = [](auto l1, auto l2) {
    return l1 = l2;
  };
  static_assert(test_assign(quantity<metre>(2), quantity<metre>(1)).count() == 1);

  // static member functions

  static_assert(quantity<metre>::zero().count() == 0);
  static_assert(quantity<metre>::min().count() == std::numeric_limits<double>::lowest());
  static_assert(quantity<metre>::max().count() == std::numeric_limits<double>::max());
  static_assert(quantity<metre, int>::zero().count() == 0);
  static_assert(quantity<metre, int>::min().count() == std::numeric_limits<int>::lowest());
  static_assert(quantity<metre, int>::max().count() == std::numeric_limits<int>::max());
  static_assert(quantity<metre, my_value<int>>::zero().count() == 0);
  static_assert(quantity<metre, my_value<int>>::min().count() == std::numeric_limits<int>::lowest());
  static_assert(quantity<metre, my_value<int>>::max().count() == std::numeric_limits<int>::max());
  static_assert(quantity<metre, my_value<float>>::zero().count() == 0.0);
  static_assert(quantity<metre, my_value<float>>::min().count() == std::numeric_limits<float>::lowest());
  static_assert(quantity<metre, my_value<float>>::max().count() == std::numeric_limits<float>::max());

  // unary member operators

  static_assert((+km).count() == 1000);
  static_assert((-km).count() == -1000);
  static_assert((+(-km)).count() == -1000);
  static_assert((-(-km)).count() == 1000);

  // binary member operators

  auto test_incr = [](auto v, auto func) {
    auto vv = func(v);
    return std::make_pair(v, vv);
  };
  static_assert(test_incr(km, [](auto& v) { return v++; }) == std::make_pair(quantity<metre, int>(1001), quantity<metre, int>(1000)));
  static_assert(test_incr(km, [](auto& v) { return ++v; }) == std::make_pair(quantity<metre, int>(1001), quantity<metre, int>(1001)));
  static_assert(test_incr(km, [](auto& v) { return v--; }) == std::make_pair(quantity<metre, int>(999), quantity<metre, int>(1000)));
  static_assert(test_incr(km, [](auto& v) { return --v; }) == std::make_pair(quantity<metre, int>(999), quantity<metre, int>(999)));

  // compound assignment

  static_assert((quantity<metre>(1) += quantity<metre>(1)).count() == 2);
  static_assert((quantity<metre>(2) -= quantity<metre>(1)).count() == 1);
  static_assert((quantity<metre>(1) *= 2).count() == 2);
  static_assert((quantity<metre>(2) /= 2).count() == 1);
  static_assert((quantity<metre, int>(7) %= 2).count() == 1);
  static_assert((quantity<metre, int>(7) %= quantity<metre, int>(2)).count() == 1);
//  static_assert((quantity<metre>(7.) %= 2).count() == 1);                         // should not compile
//  static_assert((quantity<metre, int>(7) %= 2.).count() == 1);                    // should not compile
//  static_assert((quantity<metre>(7.) %= quantity<metre, int>(2)).count() == 1);   // should not compile
//  static_assert((quantity<metre, int>(7) %= quantity<metre>(2.)).count() == 1);   // should not compile

  // non-member arithmetic operators

  static_assert(std::is_same_v<decltype(quantity<metre, int>() + quantity<metre, float>()), quantity<metre, float>>);
  static_assert(std::is_same_v<decltype(quantity<metre, float>() + quantity<metre, int>()), quantity<metre, float>>);
  static_assert(std::is_same_v<decltype(quantity<metre, float>() - quantity<metre, int>()), quantity<metre, float>>);
  static_assert(std::is_same_v<decltype(quantity<metre, int>() - quantity<metre, float>()), quantity<metre, float>>);
  static_assert(std::is_same_v<decltype(quantity<metre, int>() * 1.0f), quantity<metre, float>>);
  static_assert(std::is_same_v<decltype(quantity<metre, float>() * 1), quantity<metre, float>>);
  static_assert(std::is_same_v<decltype(1.0f * quantity<metre, int>()), quantity<metre, float>>);
  static_assert(std::is_same_v<decltype(1 * quantity<metre, float>()), quantity<metre, float>>);
  static_assert(std::is_same_v<decltype(quantity<metre, int>() / 1.0f), quantity<metre, float>>);
  static_assert(std::is_same_v<decltype(quantity<metre, float>() / 1), quantity<metre, float>>);
  static_assert(std::is_same_v<decltype(quantity<metre, int>() / quantity<metre, float>()), float>);
  static_assert(std::is_same_v<decltype(quantity<metre, float>() / quantity<metre, int>()), float>);
  static_assert(std::is_same_v<decltype(quantity<metre, int>() % short(1)), quantity<metre, int>>);
  static_assert(std::is_same_v<decltype(quantity<metre, int>() % quantity<metre, short>(1)), quantity<metre, int>>);
  static_assert((quantity<metre>(1) + km).count() == 1001);
  static_assert((km - quantity<metre>(1)).count() == 999);
  static_assert((quantity<metre>(2) * 2).count() == 4);
  static_assert((3 * quantity<metre>(3)).count() == 9);
  static_assert((quantity<metre>(4) / 2).count() == 2);
  static_assert(quantity<metre>(4) / quantity<metre>(2) == 2);
  static_assert((quantity<metre, int>(7) % 2).count() == 1);
  static_assert((quantity<metre, int>(7) % quantity<metre, int>(2)).count() == 1);
//  static_assert((quantity<metre>(7.) % 2).count() == 1);                        // should not compile
//  static_assert((quantity<metre, int>(7) % 2.).count() == 1);                   // should not compile
//  static_assert((quantity<metre>(7.) % quantity<metre, int>(2)).count() == 1);  // should not compile
//  static_assert((quantity<metre, int>(7) % quantity<metre>(2.)).count() == 1);  // should not compile

  // comparators

  static_assert(quantity<metre>(2) + quantity<metre>(1) == quantity<metre>(3));
  static_assert(!(quantity<metre>(2) + quantity<metre>(2) == quantity<metre>(3)));
  static_assert(quantity<metre>(2) + quantity<metre>(2) != quantity<metre>(3));
  static_assert(!(quantity<metre>(2) + quantity<metre>(2) != quantity<metre>(4)));
  static_assert(quantity<metre>(2) > quantity<metre>(1));
  static_assert(!(quantity<metre>(1) > quantity<metre>(1)));
  static_assert(quantity<metre>(1) < quantity<metre>(2));
  static_assert(!(quantity<metre>(2) < quantity<metre>(2)));
  static_assert(quantity<metre>(2) >= quantity<metre>(1));
  static_assert(quantity<metre>(2) >= quantity<metre>(2));
  static_assert(!(quantity<metre>(2) >= quantity<metre>(3)));
  static_assert(quantity<metre>(1) <= quantity<metre>(2));
  static_assert(quantity<metre>(2) <= quantity<metre>(2));
  static_assert(!(quantity<metre>(3) <= quantity<metre>(2)));

  static_assert(quantity<metre, int>(3) == quantity<metre, float>(3.0));
  static_assert(quantity<metre, int>(3) != quantity<metre, float>(3.14));
  static_assert(quantity<metre, int>(2) > quantity<metre, float>(1.0));
  static_assert(quantity<metre, float>(1.0) < quantity<metre, int>(2));
  static_assert(quantity<metre, float>(2.0) >= quantity<metre, int>(1));
  static_assert(quantity<metre, int>(1) <= quantity<metre, float>(2.0));

  // common_ratio

  static_assert(std::is_same_v<common_ratio<std::ratio<1>, std::kilo>, std::ratio<1>>);
  static_assert(std::is_same_v<common_ratio<std::kilo, std::ratio<1>>, std::ratio<1>>);
  static_assert(std::is_same_v<common_ratio<std::ratio<1>, std::milli>, std::milli>);
  static_assert(std::is_same_v<common_ratio<std::milli, std::ratio<1>>, std::milli>);

  // quantity_cast

//  static_assert(quantity_cast<int>(quantity<kilometre, int>(2)).count() == 2000);  // should not compile
  static_assert(quantity_cast<quantity<metre, int>>(quantity<kilometre, int>(2)).count() == 2000);
  static_assert(quantity_cast<quantity<kilometre, int>>(quantity<metre, int>(2000)).count() == 2);

}  // namespace
