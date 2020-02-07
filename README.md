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

We are about to implement [merge sort](https://en.wikipedia.org/wiki/Merge_sort) algorithm on types

![img](https://upload.wikimedia.org/wikipedia/commons/thumb/e/e6/Merge_sort_algorithm_diagram.svg/300px-Merge_sort_algorithm_diagram.svg.png)

```cpp
template<int UniqueValue>
using dim_id = std::integral_constant<int, UniqueValue>;

template<typename D1, typename D2>
struct dim_id_less : std::bool_constant<D1::value < D2::value> {};

template<typename TypeList>
using dim_sort = type_list_sort<TypeList, dim_id_less>;

static_assert(std::is_same_v<dim_sort<type_list<dim_id<38>, dim_id<27>, dim_id<43>, dim_id<3>, dim_id<9>, dim_id<82>, dim_id<10>>>,
                             type_list<dim_id<3>, dim_id<9>, dim_id<10>, dim_id<27>, dim_id<38>, dim_id<43>, dim_id<82>>>);
```

1. Create `type_list_sort` that will implement merge sort algorithm on `TypeList` using the
    predicate `Pred` and the tools we've implemented already:

    ```cpp
    namespace detail {

        template<typename List, template<typename, typename> typename Pred>
        struct type_list_sort_impl;

        template<template<typename...> typename List, template<typename, typename> typename Pred>
        struct type_list_sort_impl<List<>, Pred> {
        using type = List<>;
        };

        // ...
    }

    template<typename List, template<typename, typename> typename Pred>
    using type_list_sort = typename detail::type_list_sort_impl<List, Pred>::type;
    ```
