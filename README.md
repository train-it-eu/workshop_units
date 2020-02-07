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
using metre = unit<std::ratio<1>>;
using kilometre = unit<std::ratio<1000>>;

constexpr quantity<metre, int> d1(1);
constexpr quantity<kilometre, int> d2(1);
//  constexpr quantity<metre, int> d3 = d1 + d2; // should not compile (for now)
constexpr quantity<metre, int> d3(d1.count() + d2.count() * 1000);
```

1. In a new header `unit.h` add the unit definition:

    ```cpp
    template<typename Ratio>
    struct unit;
    ```

    - `unit` class should provide `ratio` member type in its interface
    - make sure that `Ratio` argument of `unit` class template is a specialization of `std::ratio`
    - make sure that value provided to `Ratio` is positive (non-negative)

2. Convert `quantity` to the following class template

    ```cpp
    template<typename Unit, typename Rep = double>
    class quantity;
    ```

    - update the `quantity` class to provide `unit` member type in its interface
    - make sure that `Unit` argument of `quantity` class template is a specialization of `units::unit`

3. All binary functions using `quantity` for both arguments should use the same `Unit` for both of them.
4. Ensure that all commented checks in `tests.cpp` produce a compile-time error.
