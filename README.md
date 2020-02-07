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
constexpr quantity<int> kilometre(1000);
constexpr quantity<> d(kilometre);
static_assert(d.count() == 1000);

static_assert((++quantity<int>(kilometre)).count() == 1001);
static_assert((quantity<int>(kilometre)++).count() == 1000);
static_assert((--quantity<int>(kilometre)).count() == 999);
static_assert((quantity<int>(kilometre)--).count() == 1000);

static_assert(quantity<int>(kilometre) % 10 == quantity<int>(0));
```

1. Add converting constructor

    ```cpp
    template<class Rep2>
    constexpr quantity(const quantity<Rep2>& q);
    ```

2. Both converting constructors should participate in the overload resolution only if:
    - destination `Rep` is convertible from source `Rep`
    - either destination `Rep` is of floating point type or source `Rep` is not of floating
      point type

    (that is, a `quantity` with an integer representation cannot be constructed from a
    floating-point value, but a `quantity` with a floating-point representation can be
    constructed from an integer value)

3. Add pre- and post- increment and decrement operators.

4. Add support for modulo operators and ensure that they do not participate in overload
    resolution for floating-point representations.

5. Ensure that all commented checks in `tests.cpp` produce a compile-time error.
