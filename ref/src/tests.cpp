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

  using namespace units;

  // class invariants

//  constexpr quantity<quantity<int>> error(quantity<int>(0));  // should trigger a static_assert

  // member types

  static_assert(std::is_same_v<quantity<int>::rep, int>);
  static_assert(std::is_same_v<quantity<float>::rep, float>);
 
   // constructors

  static_assert(quantity<>().count() == 0);
  static_assert(quantity<int>(1).count() == 1);
  static_assert(quantity<int>(1000).count() == 1000);
  
  static_assert(quantity<int>(1).count() == 1);
//  static_assert(quantity<int>(1.0).count() == 1);   // should not compile
  static_assert(quantity<float>(1.0).count() == 1.0);
  static_assert(quantity<float>(1).count() == 1.0);
  static_assert(quantity<float>(3.14f).count() == 3.14f);

  static_assert(quantity<int>(quantity<int>(1000)).count() == 1000);
//  static_assert(quantity<int>(quantity<float>(1000.0)).count() == 1000);   // should not compile
  static_assert(quantity<float>(quantity<float>(1000.0)).count() == 1000.0);

  // assignment operator

  auto test_assign = [](auto l1, auto l2) {
    return l1 = l2;
  };
  static_assert(test_assign(quantity<>(2), quantity<>(1)).count() == 1);

  // static member functions

  static_assert(quantity<>::zero().count() == 0);
  static_assert(quantity<>::min().count() == std::numeric_limits<double>::lowest());
  static_assert(quantity<>::max().count() == std::numeric_limits<double>::max());
  static_assert(quantity<int>::zero().count() == 0);
  static_assert(quantity<int>::min().count() == std::numeric_limits<int>::lowest());
  static_assert(quantity<int>::max().count() == std::numeric_limits<int>::max());

  // unary member operators

  static_assert((+quantity<int>(1000)).count() == 1000);
  static_assert((-quantity<int>(1000)).count() == -1000);
  static_assert((+(-quantity<int>(1000))).count() == -1000);
  static_assert((-(-quantity<int>(1000))).count() == 1000);

  // compound assignment

  static_assert((quantity<>(1) += quantity<>(1)).count() == 2);
  static_assert((quantity<>(2) -= quantity<>(1)).count() == 1);
  static_assert((quantity<>(1) *= 2).count() == 2);
  static_assert((quantity<>(2) /= 2).count() == 1);

  // non-member arithmetic operators

  static_assert(std::is_same_v<decltype(quantity<int>() + quantity<float>()), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<float>() + quantity<int>()), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<float>() - quantity<int>()), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<int>() - quantity<float>()), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<int>() * 1.0f), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<float>() * 1), quantity<float>>);
  static_assert(std::is_same_v<decltype(1.0f * quantity<int>()), quantity<float>>);
  static_assert(std::is_same_v<decltype(1 * quantity<float>()), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<int>() / 1.0f), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<float>() / 1), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<int>() / quantity<float>()), float>);
  static_assert(std::is_same_v<decltype(quantity<float>() / quantity<int>()), float>);
  static_assert((quantity<>(1) + quantity<int>(1000)).count() == 1001);
  static_assert((quantity<int>(1000) - quantity<>(1)).count() == 999);
  static_assert((quantity<>(2) * 2).count() == 4);
  static_assert((3 * quantity<>(3)).count() == 9);
  static_assert((quantity<>(4) / 2).count() == 2);
  static_assert(quantity<>(4) / quantity<>(2) == 2);

  // comparators

  static_assert(quantity<>(2) + quantity<>(1) == quantity<>(3));
  static_assert(!(quantity<>(2) + quantity<>(2) == quantity<>(3)));
  static_assert(quantity<>(2) + quantity<>(2) != quantity<>(3));
  static_assert(!(quantity<>(2) + quantity<>(2) != quantity<>(4)));
  static_assert(quantity<>(2) > quantity<>(1));
  static_assert(!(quantity<>(1) > quantity<>(1)));
  static_assert(quantity<>(1) < quantity<>(2));
  static_assert(!(quantity<>(2) < quantity<>(2)));
  static_assert(quantity<>(2) >= quantity<>(1));
  static_assert(quantity<>(2) >= quantity<>(2));
  static_assert(!(quantity<>(2) >= quantity<>(3)));
  static_assert(quantity<>(1) <= quantity<>(2));
  static_assert(quantity<>(2) <= quantity<>(2));
  static_assert(!(quantity<>(3) <= quantity<>(2)));

  static_assert(quantity<int>(3) == quantity<float>(3.0));
  static_assert(quantity<int>(3) != quantity<float>(3.14));
  static_assert(quantity<int>(2) > quantity<float>(1.0));
  static_assert(quantity<float>(1.0) < quantity<int>(2));
  static_assert(quantity<float>(2.0) >= quantity<int>(1));
  static_assert(quantity<int>(1) <= quantity<float>(2.0));

}  // namespace
