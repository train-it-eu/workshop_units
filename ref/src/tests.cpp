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

  // constructors

  static_assert(quantity().count() == 0);
  static_assert(quantity(1).count() == 1);
  static_assert(quantity(1000).count() == 1000);
  static_assert(quantity(quantity(1000)).count() == quantity(1000).count());

  // assignment operator

  auto test_assign = [](auto l1, auto l2) {
    return l1 = l2;
  };
  static_assert(test_assign(quantity(2), quantity(1)).count() == 1);

  // static member functions

  static_assert(quantity::zero().count() == 0);
  static_assert(quantity::min().count() == std::numeric_limits<double>::lowest());
  static_assert(quantity::max().count() == std::numeric_limits<double>::max());

  // unary member operators

  static_assert((+quantity(1000)).count() == 1000);
  static_assert((-quantity(1000)).count() == -1000);
  static_assert((+(-quantity(1000))).count() == -1000);
  static_assert((-(-quantity(1000))).count() == 1000);

  // compound assignment

  static_assert((quantity(1) += quantity(1)).count() == 2);
  static_assert((quantity(2) -= quantity(1)).count() == 1);
  static_assert((quantity(1) *= 2).count() == 2);
  static_assert((quantity(2) /= 2).count() == 1);

  // non-member arithmetic operators

  static_assert((quantity(1) + quantity(1000)).count() == 1001);
  static_assert((quantity(1000) - quantity(1)).count() == 999);
  static_assert((quantity(2) * 2).count() == 4);
  static_assert((3 * quantity(3)).count() == 9);
  static_assert((quantity(4) / 2).count() == 2);
  static_assert(quantity(4) / quantity(2) == 2);

  // comparators

  static_assert(quantity(2) + quantity(1) == quantity(3));
  static_assert(!(quantity(2) + quantity(2) == quantity(3)));
  static_assert(quantity(2) + quantity(2) != quantity(3));
  static_assert(!(quantity(2) + quantity(2) != quantity(4)));
  static_assert(quantity(2) > quantity(1));
  static_assert(!(quantity(1) > quantity(1)));
  static_assert(quantity(1) < quantity(2));
  static_assert(!(quantity(2) < quantity(2)));
  static_assert(quantity(2) >= quantity(1));
  static_assert(quantity(2) >= quantity(2));
  static_assert(!(quantity(2) >= quantity(3)));
  static_assert(quantity(1) <= quantity(2));
  static_assert(quantity(2) <= quantity(2));
  static_assert(!(quantity(3) <= quantity(2)));

}  // namespace
