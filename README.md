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
//  static_assert(quantity<meter, int>(1_s).count() == 1);   // should not compile 
//  static_assert(1_s == 1_m);   // should not compile
static_assert(1_h == 3600_s);
static_assert(1_km + 1_m == 1001_m);
static_assert(10_km / 5_km == 2);
static_assert(10_km / 2 == 5_km);
```

1. In a `base_dimensions.h` header define identifiers for base physical dimensions you
    plan to support, i.e.:

    ```cpp
    struct base_dim_length : dim_id<0> {};
    ```

2. Add dimensions support to `unit`

    ```cpp
    template<typename Dimension, typename Ratio>
    struct unit;
    ```

    - add `dimension` member type
    - verify that `Dimension` template parameter is an instantiation of `units::dimension`
    - make sure that `quantity` binary operators do not participate in the overload
        resolution if `Unit2` has different dimension than `quantity::unit`

3. In `length.h` add `dimension_length` alias to a result of `make_dimension` for `base_dim_length`.
4. Update length units to use `dimension_length`.
5. Add all time specific definitions to `time.h`.
