# Physical Units Library Workshop

## Overview

The purpose of that workshop is to create a simple physical units library for C++.

It will support:
- several physical dimensions,
- automatic conversions between different units,
- compile time expressions verification,
- literals to easily work with different dimensions and their various units.

Above features will be provided without any runtime performance overhead.

Example:

```cpp
static_assert(1000 / 1_s == 1_kHz);
static_assert(1_h == 3600_s);
static_assert(1_km + 1_m == 1001_m);
static_assert(10_km / 5_km == 2);
static_assert(10_km / 2 == 5_km);
static_assert(1_km / 1_s == 1000_mps);
static_assert(2_kmph * 2_h == 4_km);
static_assert(2_km / 2_kmph == 1_h);
```


## How to proceed?

1. While solving workshop tasks please do not modify the code in the `ref` directory to not hit
   merge conflicts when moving to the next task.
2. The initial state of the task is always provided in the `ref/include` directory. If you want to
   use the reference implementation for the next tasks (instead using the one you wrote) than
   copy headers from `ref/include` to `include` before starting the assignment.
3. Reference unit tests and examples are always in `ref/src` directory. The target of each task is
   to make that code compile and pass without any issues.
4. If you want to add additional unit tests than please do so in `src/tests.cpp` file.


## Task

```cpp
static_assert(std::is_same_v<type_list_split_half<type_list<int, long, double, float>>::first_list, type_list<int, long>>);
static_assert(std::is_same_v<type_list_split_half<type_list<int, long, double, float>>::second_list, type_list<double, float>>);

template<int UniqueValue>
using dim_id = std::integral_constant<int, UniqueValue>;

template<typename D1, typename D2>
struct dim_id_less : std::bool_constant<D1::value < D2::value> {};

static_assert(std::is_same_v<type_list_merge_sorted<type_list<dim_id<27>, dim_id<38>>, type_list<dim_id<3>, dim_id<43>>, dim_id_less>,
                             type_list<dim_id<3>, dim_id<27>, dim_id<38>, dim_id<43>>>);
```

1. Create `type_list_split_half` class template that is a helper wrapper around `type_list_split`

    ```cpp
    template<typename TypeList>
    struct type_list_split_half;
    ```

   - it should split the `type_list` to 2 parts,
   - in case of odd number of types on the list `first_list` should store more types than the `second_list`.

2. Create `type_list_merge_sorted` class template that will merge 2 provided sorted type lists into
   one big sorted `type_list`. Sorting predicate should be provided as a `Pred` class template argument.

    ```cpp
    namespace detail {

      template<typename SortedList1, typename SortedList2, template<typename, typename> typename Pred>
      struct type_list_merge_sorted_impl;

      template<template<typename...> typename List, typename... Lhs, template<typename, typename> typename Pred>
        struct type_list_merge_sorted_impl<List<Lhs...>, List<>, Pred> {
        using type = List<Lhs...>;
      };

      template<template<typename...> typename List, typename... Rhs, template<typename, typename> typename Pred>
      struct type_list_merge_sorted_impl<List<>, List<Rhs...>, Pred> {
        using type = List<Rhs...>;
      };

      template<template<typename...> typename List, typename Lhs1, typename... LhsRest, typename Rhs1, typename... RhsRest,
               template<typename, typename> typename Pred>
      struct type_list_merge_sorted_impl<List<Lhs1, LhsRest...>, List<Rhs1, RhsRest...>, Pred> {
        // ...
      };

    }

    template<TypeList SortedList1, TypeList SortedList2, template<typename, typename> typename Pred>
    using type_list_merge_sorted = typename detail::type_list_merge_sorted_impl<SortedList1, SortedList2, Pred>::type;
    ```
