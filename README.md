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
using millimetre = unit<std::milli>;
using metre = unit<std::ratio<1>>;
using kilometre = unit<std::kilo>;

static_assert(quantity<metre, int>(quantity<kilometre, int>(1)) == quantity<metre, int>(1000));
static_assert(quantity<metre, int>(1) + quantity<kilometre, int>(1) == quantity<millimetre, int>(1'001'000));
```

1. Modify converting constructor to take `quantity` of any `Ratio2` as a its argument. Use
   `quantity_cast` to convert value of source `quantity` to the destination one.
2. If `Rep2` template argument of a converting constructor is not a floating-point type than the
   converting constructor should not participate in the overload resolution unless `Ratio2` is a
   multiplicity of `ratio`.
3. Convert all binary functions to take `quantity` with any `Ratio2` and to return
   `common_quantity`:

    ```cpp
    template<typename Q1, typename Q2, typename Rep = std::common_type_t<typename Q1::rep, typename Q2::rep>>
    using common_quantity = quantity<unit<common_ratio<typename Q1::unit::ratio, typename Q2::unit::ratio>>, Rep>;
    ```

    Hint:

    ```cpp
    template<typename Unit2, typename Rep2>
    [[nodiscard]] friend constexpr auto operator+(const quantity& lhs, const quantity<Unit2, Rep2>& rhs)
        -> common_quantity<quantity, quantity<Unit2, Rep2>, decltype(lhs.count() + rhs.count())>
    {
      using ret = common_quantity<quantity, quantity<Unit2, Rep2>, decltype(lhs.count() + rhs.count())>;
      return ret(ret(lhs).count() + ret(rhs).count());
    }
    ```

4. Ensure that all commented checks in `tests.cpp` produce a compile-time error.
