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
template<typename... Types>
struct type_list;

static_assert(std::is_same_v<type_list_push_front<type_list<long>, int>, type_list<int, long>>);
static_assert(std::is_same_v<type_list_push_back<type_list<int>, long>, type_list<int, long>>);

using split = type_list_split<type_list<int, long, double>, 2>;
static_assert(std::is_same_v<split::first_list, type_list<int, long>>);
static_assert(std::is_same_v<split::second_list, type_list<double>>);
```

1. In a new header `type_list.h` based on the below hint implement `type_list_push_front`
    to insert additional types at the front of existing `type_list`:

    ```cpp
    namespace detail {

        template<typename List, typename... Types>
        struct type_list_push_front_impl;

        // partial specialization here ...

    }

    template<TypeList List, typename... Types>
    using type_list_push_front = typename detail::type_list_push_front_impl<List, Types...>::type;
    ```

2. Add `type_list_push_back`
3. Define `type_list_split` primary class template and its partial specialization that will provide
    `first_list` and `second_list` member types:

    ```cpp
    template<typename TypeList, std::size_t N>
    struct type_list_split;
    ```

   - `first_list` should contain all the types from a `TypeList` with index less than `N`
   - `second_list` should provide the types from a `TypeList` with index equal or greater than `N`
   - Hint: partial specialization may use the following helper class template:

        ```cpp
        template<template<typename...> typename List, std::size_t Idx, std::size_t N, typename... Types>
        struct split_impl;

        template<template<typename...> typename List, std::size_t Idx, std::size_t N>
        struct split_impl<List, Idx, N> {
          using first_list = List<>;
          using second_list = List<>;
        };

        template<template<typename...> typename List, std::size_t Idx, std::size_t N, typename T, typename... Rest>
        struct split_impl<List, Idx, N, T, Rest...> : split_impl<List, Idx + 1, N, Rest...> {
          // ...
        };
        ```
