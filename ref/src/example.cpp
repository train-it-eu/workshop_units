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

#include "type_list.h"

namespace {

  using namespace units;

  template<typename... Types>
  struct type_list;

  static_assert(std::is_same_v<type_list_split_half<type_list<int, long, double, float>>::first_list, type_list<int, long>>);
  static_assert(std::is_same_v<type_list_split_half<type_list<int, long, double, float>>::second_list, type_list<double, float>>);

  template<int UniqueValue>
  using dim_id = std::integral_constant<int, UniqueValue>;

  template<typename D1, typename D2>
  struct dim_id_less : std::bool_constant<D1::value < D2::value> {};

  static_assert(std::is_same_v<type_list_merge_sorted<type_list<dim_id<27>, dim_id<38>>, type_list<dim_id<3>, dim_id<43>>, dim_id_less>,
                               type_list<dim_id<3>, dim_id<27>, dim_id<38>, dim_id<43>>>);

}  // namespace
