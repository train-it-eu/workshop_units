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
#include "time.h"
#include "frequency.h"
#include "velocity.h"
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
//  static_assert(quantity<metre, int>(1_s).count() == 1);   // should not compile
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

  // type_list_split_half

  static_assert(std::is_same_v<type_list_split_half<type_list<int>>::first_list, type_list<int>>);
  static_assert(std::is_same_v<type_list_split_half<type_list<int>>::second_list, type_list<>>);

  static_assert(std::is_same_v<type_list_split_half<type_list<int, long>>::first_list, type_list<int>>);
  static_assert(std::is_same_v<type_list_split_half<type_list<int, long>>::second_list, type_list<long>>);

  static_assert(std::is_same_v<type_list_split_half<type_list<int, long, double>>::first_list, type_list<int, long>>);
  static_assert(std::is_same_v<type_list_split_half<type_list<int, long, double>>::second_list, type_list<double>>);

  static_assert(std::is_same_v<type_list_split_half<type_list<int, long, double, float>>::first_list, type_list<int, long>>);
  static_assert(std::is_same_v<type_list_split_half<type_list<int, long, double, float>>::second_list, type_list<double, float>>);

  // type_list_merge_sorted

  template<int UniqueValue>
  using dim_id = std::integral_constant<int, UniqueValue>;

  template<typename D1, typename D2>
  struct dim_id_less : std::bool_constant<D1::value < D2::value> {};

  static_assert(std::is_same_v<type_list_merge_sorted<type_list<dim_id<0>>, type_list<dim_id<1>>, dim_id_less>,
                               type_list<dim_id<0>, dim_id<1>>>);
  static_assert(std::is_same_v<type_list_merge_sorted<type_list<dim_id<1>>, type_list<dim_id<0>>, dim_id_less>,
                               type_list<dim_id<0>, dim_id<1>>>);

  static_assert(std::is_same_v<type_list_merge_sorted<type_list<dim_id<27>, dim_id<38>>, type_list<dim_id<3>, dim_id<43>>, dim_id_less>,
                               type_list<dim_id<3>, dim_id<27>, dim_id<38>, dim_id<43>>>);
  static_assert(std::is_same_v<type_list_merge_sorted<type_list<dim_id<9>, dim_id<82>>, type_list<dim_id<10>>, dim_id_less>,
                               type_list<dim_id<9>, dim_id<10>, dim_id<82>>>);

  // type_list_sort

  template<typename TypeList>
  using dim_sort = type_list_sort<TypeList, dim_id_less>;

  static_assert(std::is_same_v<dim_sort<type_list<dim_id<0>>>, type_list<dim_id<0>>>);
  static_assert(std::is_same_v<dim_sort<type_list<dim_id<0>, dim_id<1>>>,
                               type_list<dim_id<0>, dim_id<1>>>);
  static_assert(std::is_same_v<dim_sort<type_list<dim_id<1>, dim_id<0>>>,
                               type_list<dim_id<0>, dim_id<1>>>);
  static_assert(std::is_same_v<dim_sort<type_list<dim_id<38>, dim_id<27>, dim_id<43>, dim_id<3>, dim_id<9>, dim_id<82>, dim_id<10>>>,
                               type_list<dim_id<3>, dim_id<9>, dim_id<10>, dim_id<27>, dim_id<38>, dim_id<43>, dim_id<82>>>);

  // exp_less

  template<int Id, int Value>
  using e = exp<dim_id<Id>, Value>;

  template<typename TypeList>
  using exp_sort = type_list_sort<TypeList, exp_less>;

  static_assert(std::is_same_v<exp_sort<dimension<e<0, 1>>>, dimension<e<0, 1>>>);
  static_assert(std::is_same_v<exp_sort<dimension<e<0, 1>, e<1, -1>>>, dimension<e<0, 1>, e<1, -1>>>);
  static_assert(std::is_same_v<exp_sort<dimension<e<1, 1>, e<0, -1>>>, dimension<e<0, -1>, e<1, 1>>>);

  // exp_invert

  static_assert(std::is_same_v<exp_invert<e<0, 1>>, e<0, -1>>);
  static_assert(std::is_same_v<exp_invert<e<1, -1>>, e<1, 1>>);

  // make_dimension

  static_assert(std::is_same_v<make_dimension<e<0, 1>>, dimension<e<0, 1>>>);
  static_assert(std::is_same_v<make_dimension<e<0, 1>, e<1, 1>>, dimension<e<0, 1>, e<1, 1>>>);
  static_assert(std::is_same_v<make_dimension<e<1, 1>, e<0, 1>>, dimension<e<0, 1>, e<1, 1>>>);
  static_assert(std::is_same_v<make_dimension<e<1, 1>, e<1, 1>>, dimension<e<1, 2>>>);
  static_assert(std::is_same_v<make_dimension<e<1, 1>, e<1, -1>>, dimension<>>);

  static_assert(std::is_same_v<make_dimension<e<0, 1>, e<1, 1>, e<0, 1>, e<1, 1>>, dimension<e<0, 2>, e<1, 2>>>);
  static_assert(std::is_same_v<make_dimension<e<0, -1>, e<1, -1>, e<0, -1>, e<1, -1>>, dimension<e<0, -2>, e<1, -2>>>);

  static_assert(std::is_same_v<make_dimension<e<0, 1>, e<1, 1>, e<1, -1>>, dimension<e<0, 1>>>);
  static_assert(std::is_same_v<make_dimension<e<0, 1>, e<0, -1>, e<1, 1>>, dimension<e<1, 1>>>);
  static_assert(std::is_same_v<make_dimension<e<0, 1>, e<1, 1>, e<0, -1>>, dimension<e<1, 1>>>);
  static_assert(std::is_same_v<make_dimension<e<0, 1>, e<1, 1>, e<0, -1>, e<1, -1>>, dimension<>>);

  // dimension_multiply

  static_assert(std::is_same_v<dimension_multiply<dimension<e<0, 1>>, dimension<e<1, 1>>>,
                               dimension<e<0, 1>, e<1, 1>>>);
  static_assert(std::is_same_v<dimension_multiply<dimension<e<0, 1>, e<1, 1>, e<2, 1>>, dimension<e<3, 1>>>,
                               dimension<e<0, 1>, e<1, 1>, e<2, 1>, e<3, 1>>>);
  static_assert(std::is_same_v<dimension_multiply<dimension<e<0, 1>, e<1, 1>, e<2, 1>>, dimension<e<1, 1>>>,
                               dimension<e<0, 1>, e<1, 2>, e<2, 1>>>);
  static_assert(std::is_same_v<dimension_multiply<dimension<e<0, 1>, e<1, 1>, e<2, 1>>, dimension<e<1, -1>>>,
                               dimension<e<0, 1>, e<2, 1>>>);

  // dimension_divide

  static_assert(std::is_same_v<dimension_divide<dimension<e<0, 1>>, dimension<e<1, 1>>>,
                               dimension<e<0, 1>, e<1, -1>>>);
  static_assert(std::is_same_v<dimension_divide<dimension<e<0, 1>>, dimension<e<0, 1>>>,
                               dimension<>>);

  // time

//  static_assert(1_s == 1_m);  // should not compile
  static_assert(2000_ms == 2_s);
  static_assert(1_h == 3600_s);

  // length

  static_assert(1_km == 1000_m);
  static_assert(1_km + 1_m == 1001_m);
  static_assert(10_km / 5_km == 2);
  static_assert(10_km / 2 == 5_km);

  // frequency

  static_assert(1 / 1_s == 1_Hz);
  static_assert(1000 / 1_s == 1_kHz);
  
  // velocity
  
  static_assert(10_m / 5_s == 2_mps);
  static_assert(10 / 5_s * 1_m == 2_mps);
  static_assert(1_km / 1_s == 1000_mps);
  static_assert(2_kmph * 2_h == 4_km);
  static_assert(10_Hz * 10_s == 100);
  static_assert(2_km / 2_kmph == 1_h);

}  // namespace
