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

static_assert(quantity_cast<quantity<metre, int>>(quantity<kilometre, int>(2)).count() == 2000);
static_assert(quantity_cast<quantity<kilometre, int>>(quantity<metre, int>(2000)).count() == 2);
```

1. Copy `ref/include/common_ratio.h` to the `include` directory and include in your
    `quantity.h` header.
2. Implement `quantity_cast` function template

    ```cpp
    template<typename To, typename Unit, typename Rep,
    [[nodiscard]] constexpr To quantity_cast(const quantity<Unit, Rep>& q);
    ```

    - it should multiply the value provided by `q` with `c_ratio` that is a result of division
    of `Unit::ratio` and `To::unit::ratio`

    The purpose of `quantity_cast` is to allow the user to perform lossy conversions, e.g.

    ```cpp
    //  static_assert(quantity<metre, int>(quantity<metre, float>(3.14)).count() == 3);   // should not compile
    static_assert(quantity<metre, int>(quantity_cast<quantity<metre, int>>(quantity<metre, float>(3.14))).count() == 3);
    ```

3. `quantity_cast` should participate in the overload resolution only if `To` template parameter
   is of `quantity` type.
4. Bonus points for making `quantity_cast` perform division and multiplication operations only when
   needed (the value of operation argument is different than `1`).

    Hint:

    ```cpp
    template<typename To, typename CRatio, typename CRep, bool NumIsOne = false, bool DenIsOne = false>
    struct quantity_cast_impl {
      template<typename Unit, typename Rep>
      static constexpr To cast(const quantity<Unit, Rep>& q)
    };
    ```

5. Ensure that all commented checks in `tests.cpp` produce a compile-time error.
