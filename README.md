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
static_assert(10_m / 5_s == 2_mps);
static_assert(1_km / 1_s == 1000_mps);
static_assert(2_kmph * 2_h == 4_km);
static_assert(10_Hz * 10_s == 100);
static_assert(2_km / 2_kmph == 1_h);
```

1. Add definitions to `velocity.h` header (at least `mps` and `kmph`).

2. Add the following operator overloads to `quantity` interface
    - `rep operator*(quantity, quantity)`
    - `quantity operator*(quantity, quantity)`
    - `quantity operator/(quantity, quantity)`
   for different dimensions

3. `rep operator*(quantity, quantity)` should participate in the overload resolution only if
    the following are true:
    - `same_dim<typename Unit::dimension, dim_invert<typename Unit2::dimension>>`

4. `quantity operator*(quantity, quantity)` should participate in the overload resolution only if
    the following are true:
    - `!same_dim<typename Unit::dimension, dim_invert<typename Unit2::dimension>>`
    - `treat_as_floating_point<decltype(std::declval<Rep>() * std::declval<Rep2>())>` or
        `std::ratio_multiply<typename Unit::ratio, typename Unit2::ratio>::den == 1`

5. `quantity operator/(quantity, quantity)` should participate in the overload resolution only if
    the following are true:
    - `!same_dim<typename Unit::dimension, dim_invert<typename Unit2::dimension>>`
    - `treat_as_floating_point<decltype(std::declval<Rep>() / std::declval<Rep2>())>` or
        `std::ratio_divide<typename Unit::ratio, typename Unit2::ratio>::den == 1`
