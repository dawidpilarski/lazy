//
// Created by pilarski on 23/09/2019.
//

#ifndef LAZY_TYPE__LAZY_HPP_
#define LAZY_TYPE__LAZY_HPP_

#include <optional>
#include <functional>
#include <type_traits>
#include <cassert>

namespace lazy {

template<typename T>
class lazy : private std::optional<T> {
 public:
  using value_type = T;
  using initializer_type = std::function<value_type()>;
 private:
  static constexpr bool is_initializer_nothrow_constructible = std::is_nothrow_move_constructible_v<initializer_type>;
 public:

  using std::optional<T>::optional;

  constexpr explicit lazy(initializer_type initializer) noexcept(std::is_nothrow_move_constructible_v<initializer_type>)
      : initializer_(std::move(initializer)) {
    assert(initializer_);
  }

  constexpr lazy(lazy &&rhs) noexcept(is_initializer_nothrow_constructible
      and std::is_nothrow_move_constructible_v<value_type>) :
      std::optional<T>(std::move(rhs.optional_value_)),
      initializer_(std::move(rhs.initializer_)) {
    rhs.optional_value_.reset();
  }

  constexpr lazy(const lazy &rhs) = delete;

  constexpr T &value() &{
    if (not has_value() and initializer_) {
      initialize();
    }

    return std::optional<T>::value();
  }

  constexpr T &&value() &&{
    return std::move(value());
  }

  constexpr T *operator->() {
    return &value();
  }

  constexpr T &operator*() &{
    return value();
  }

  constexpr T &&operator*() &&{
    return std::move(value());
  }

  using std::optional<T>::has_value;
  using std::optional<T>::operator bool;
  using std::optional<T>::emplace;
  using std::optional<T>::swap;
  using std::optional<T>::reset;

  void initialize() {
    emplace(initializer_());
  }

 private:
  initializer_type initializer_;
};

}

#endif //LAZY_TYPE__LAZY_HPP_
