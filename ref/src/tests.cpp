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

#include "length.h"
#include "type_list.h"
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

namespace std {

  template<typename T, typename U>
  struct common_type<my_value<T>, my_value<U>> : my_value<common_type<T, U>> {};

  template<typename T, typename U>
  struct common_type<my_value<T>, U> : common_type<T, U> {};

  template<typename T, typename U>
  struct common_type<T, my_value<U>> : common_type<T, U> {};

}

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
  using namespace units::literals;

  template<typename Rep = double>
  using length = quantity<Rep>;

  constexpr auto m = 1_m;
  constexpr auto km = 1000_m;

  // class invariants

//  constexpr quantity<quantity<metre, int>> error(0_m);  // should trigger a static_assert
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
//  static_assert(quantity<metre, int>(3.14_m).count() == 3);   // should not compile
  static_assert(quantity<metre, int>(quantity_cast<quantity<metre, int>>(3.14_m)).count() == 3);
//  static_assert(quantity<metre, int>(quantity<metre, my_value<float>>(1000.0)).count() == 1000);   // should not compile
//  static_assert(quantity<metre, my_value<int>>(quantity<metre, float>(1000.0)).count() == 1000);   // should not compile
  static_assert(quantity<metre, float>(quantity<metre, float>(1000.0)).count() == 1000.0);
  static_assert(quantity<metre, float>(quantity<metre, my_value<float>>(1000.0)).count() == 1000.0);
  static_assert(quantity<metre, my_value<float>>(quantity<metre, float>(1000.0)).count() == 1000.0);
  static_assert(quantity<metre, float>(km).count() == 1000.0);
  static_assert(quantity<metre, my_value<float>>(km).count() == 1000.0);
  static_assert(quantity<metre, int>(1_km).count() == 1000);
//  static_assert(quantity<kilometre, int>(1010_m).count() == 1);   // should not compile
  static_assert(quantity<kilometre, int>(quantity_cast<quantity<kilometre, int>>(1010_m)).count() == 1);

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
  static_assert(test_incr(km, [](auto& v) { return v++; }) == std::make_pair(1001_m, 1000_m));
  static_assert(test_incr(km, [](auto& v) { return ++v; }) == std::make_pair(1001_m, 1001_m));
  static_assert(test_incr(km, [](auto& v) { return v--; }) == std::make_pair(999_m, 1000_m));
  static_assert(test_incr(km, [](auto& v) { return --v; }) == std::make_pair(999_m, 999_m));

  // compound assignment

  static_assert((1_m += 1_m).count() == 2);
  static_assert((2_m -= 1_m).count() == 1);
  static_assert((1_m *= 2).count() == 2);
  static_assert((2_m /= 2).count() == 1);
  static_assert((7_m %= 2).count() == 1);
  static_assert((7_m %= 2_m).count() == 1);
//  static_assert((7._m %= 2).count() == 1);
//  static_assert((7_m %= 2.).count() == 1);
//  static_assert((7._m %= 2_m).count() == 1);
//  static_assert((7_m %= 2._m).count() == 1);

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

  static_assert((1_m + km).count() == 1001);
  static_assert((1_m + 1_km).count() == 1001);
  static_assert((km - 1_m).count() == 999);
  static_assert((1_km - 1_m).count() == 999);
  static_assert((2_m * 2).count() == 4);
  static_assert((3 * 3_m).count() == 9);
  static_assert((4_m / 2).count() == 2);
  static_assert(4_m / 2_m == 2);
  static_assert(4_km / 2000_m == 2);
  static_assert((7_m % 2).count() == 1);
  static_assert((7_m % 2_m).count() == 1);
  static_assert((7_km % 2000_m).count() == 1000);
//  static_assert((7._m % 2).count() == 1);
//  static_assert((7_m % 2.).count() == 1);
//  static_assert((7._m % 2_m).count() == 1);
//  static_assert((7_m % 2._m).count() == 1);

  // comparators

  static_assert(2_m + 1_m == 3_m);
  static_assert(!(2_m + 2_m == 3_m));
  static_assert(2_m + 2_m != 3_m);
  static_assert(!(2_m + 2_m != 4_m));
  static_assert(2_m > 1_m);
  static_assert(!(1_m > 1_m));
  static_assert(1_m < 2_m);
  static_assert(!(2_m < 2_m));
  static_assert(2_m >= 1_m);
  static_assert(2_m >= 2_m);
  static_assert(!(2_m >= 3_m));
  static_assert(1_m <= 2_m);
  static_assert(2_m <= 2_m);
  static_assert(!(3_m <= 2_m));

  static_assert(3_m == 3.0_m);
  static_assert(3_m != 3.14_m);
  static_assert(2_m > 1.0_m);
  static_assert(1.0_m < 2_m);
  static_assert(2.0_m >= 1_m);
  static_assert(1_m <= 2.0_m);

  static_assert(1000_m == 1_km);
  static_assert(1001_m != 1_km);
  static_assert(1001_m > 1_km);
  static_assert(999_m < 1_km);
  static_assert(1000_m >= 1_km);
  static_assert(1000_m <= 1_km);

  // common_ratio

  static_assert(std::is_same_v<common_ratio<std::ratio<1>, std::kilo>, std::ratio<1>>);
  static_assert(std::is_same_v<common_ratio<std::kilo, std::ratio<1>>, std::ratio<1>>);
  static_assert(std::is_same_v<common_ratio<std::ratio<1>, std::milli>, std::milli>);
  static_assert(std::is_same_v<common_ratio<std::milli, std::ratio<1>>, std::milli>);

  // quantity_cast

  // static_assert(quantity_cast<int>(2_km).count() == 2000);  // should not compile
  static_assert(quantity_cast<quantity<metre, int>>(2_km).count() == 2000);
  static_assert(quantity_cast<quantity<kilometre, int>>(2000_m).count() == 2);

  // type_list_push_front

  template<typename... Types>
  struct type_list;

  static_assert(std::is_same_v<type_list_push_front<type_list<>, int>, type_list<int>>);
  static_assert(std::is_same_v<type_list_push_front<type_list<>, int, long, double>, type_list<int, long, double>>);
  static_assert(std::is_same_v<type_list_push_front<type_list<double>, int, long>, type_list<int, long, double>>);

  // type_list_push_back

  static_assert(std::is_same_v<type_list_push_back<type_list<>, int>, type_list<int>>);
  static_assert(std::is_same_v<type_list_push_back<type_list<>, int, long, double>, type_list<int, long, double>>);
  static_assert(std::is_same_v<type_list_push_back<type_list<double>, int, long>, type_list<double, int, long>>);

  // type_list_split

  static_assert(std::is_same_v<type_list_split<type_list<int>, 0>::first_list, type_list<>>);
  static_assert(std::is_same_v<type_list_split<type_list<int>, 0>::second_list, type_list<int>>);

  static_assert(std::is_same_v<type_list_split<type_list<int>, 1>::first_list, type_list<int>>);
  static_assert(std::is_same_v<type_list_split<type_list<int>, 1>::second_list, type_list<>>);

  static_assert(std::is_same_v<type_list_split<type_list<int, long>, 0>::first_list, type_list<>>);
  static_assert(std::is_same_v<type_list_split<type_list<int, long>, 0>::second_list, type_list<int, long>>);

  static_assert(std::is_same_v<type_list_split<type_list<int, long>, 1>::first_list, type_list<int>>);
  static_assert(std::is_same_v<type_list_split<type_list<int, long>, 1>::second_list, type_list<long>>);

  static_assert(std::is_same_v<type_list_split<type_list<int, long>, 2>::first_list, type_list<int, long>>);
  static_assert(std::is_same_v<type_list_split<type_list<int, long>, 2>::second_list, type_list<>>);

  static_assert(std::is_same_v<type_list_split<type_list<int, long, double>, 1>::first_list, type_list<int>>);
  static_assert(std::is_same_v<type_list_split<type_list<int, long, double>, 1>::second_list, type_list<long, double>>);

  static_assert(std::is_same_v<type_list_split<type_list<int, long, double>, 2>::first_list, type_list<int, long>>);
  static_assert(std::is_same_v<type_list_split<type_list<int, long, double>, 2>::second_list, type_list<double>>);

}  // namespace
