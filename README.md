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
template<int Id, int Value>
using e = exp<dim_id<Id>, Value>;

static_assert(std::is_same_v<dimension_multiply<dimension<e<0, 1>, e<1, 1>, e<2, 1>>, dimension<e<1, 1>>>,
                            dimension<e<0, 1>, e<1, 2>, e<2, 1>>>);
static_assert(std::is_same_v<dimension_multiply<dimension<e<0, 1>, e<1, 1>, e<2, 1>>, dimension<e<1, -1>>>,
                            dimension<e<0, 1>, e<2, 1>>>);

static_assert(std::is_same_v<dimension_divide<dimension<e<0, 1>>, dimension<e<1, 1>>>,
                            dimension<e<0, 1>, e<1, -1>>>);
static_assert(std::is_same_v<dimension_divide<dimension<e<0, 1>>, dimension<e<0, 1>>>,
                            dimension<>>);
```

1. Add `dimension_multiply` and `dimension_divide` to `dimension.h`

    ```cpp
    template<typename D1, typename D2>
    using dimension_multiply = /* ... */;
    ```

    ```cpp
    template<typename D1, typename D2>
    using dimension_divide = /* ... */;
    ```

    Assume that `D1` and `D2` are sorted already.
