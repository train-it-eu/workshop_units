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
template<typename T>
class my_value;

namespace units {

  template<typename T>
  inline constexpr bool treat_as_floating_point<my_value<T>> = std::is_floating_point_v<T>;

  template<typename T>
  struct quantity_values<my_value<T>> {
    static constexpr my_value<T> zero() { return my_value<T>(0); }
    static constexpr my_value<T> max() { return std::numeric_limits<T>::max(); }
    static constexpr my_value<T> min() { return std::numeric_limits<T>::lowest(); }
  };

}  // namespace units

constexpr quantity<my_value<int>> d1(1), d2(2);
constexpr quantity<int> d3 = d1 + d2;
static_assert(d3.count() == 3);

constexpr quantity<float> d4(3.0);
constexpr quantity<my_value<float>> d5 = d4 + d3;
static_assert(d5.count() == 6.0);
```

1. Add the following customization points to `quantity`
    - `units::treat_as_floating_point<T>` that allows the user to specify that his own type provided as
      `Rep` behaves like a floating point type

        ```cpp
        template<typename T>
        inline constexpr bool treat_as_floating_point;
        ```

    - `units::quantity_values<T>` that allows providing custom `zero`, `min`, and `max` for `Rep`

        ```cpp
        template<typename Rep>
        struct quantity_values {
          static constexpr Rep zero();
          static constexpr Rep max();
          static constexpr Rep min();
        };
        ```
