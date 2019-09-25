//
// Created by pilarski on 23/09/2019.
//

#ifndef LAZY_TYPE__LAZY_HPP_
#define LAZY_TYPE__LAZY_HPP_

#include <optional>
#include <functional>
#include <type_traits>
#include <cassert>

namespace milli {

template<typename T>
class lazy : private std::optional<T> {
 public:
  using value_type = T;
  using initializer_type = std::function<value_type()>;

  using std::optional<T>::optional;
  using std::optional<T>::has_value;
  using std::optional<T>::operator bool;
  using std::optional<T>::emplace;
  using std::optional<T>::swap;
  using std::optional<T>::reset;
  using std::optional<T>::operator=;

  constexpr lazy() noexcept = default;
  constexpr explicit lazy(initializer_type initializer) noexcept
      : initializer_(std::move(initializer)) {}

  constexpr lazy& operator=(const lazy&) = default;
  constexpr lazy& operator=(lazy&&) noexcept(std::is_nothrow_move_constructible_v<value_type>) = default;
  constexpr lazy& operator=(std::nullopt_t) noexcept(std::is_nothrow_destructible_v<value_type>) {
    initializer_ = nullptr;
    *this = std::nullopt;
  }

  lazy& operator=(const std::optional<T>&){

  }

  lazy& operator=(std::optional<T>&&){

  }

  template <typename U = T>
  lazy& operator=(U&& value){

  }

  template <typename U>
  lazy& operator=(const std::optional<U>&){

  }

  template <typename U>
  lazy& operator=(std::optional<U>&&){

  }

  [[nodiscard]] constexpr T &value() &{
    if (not has_value() and initializer_) {
      initialize();
    }

    return std::optional<T>::value();
  }

  [[nodiscard]] constexpr T &&value() &&{
    return std::move(value());
  }

  [[nodiscard]] constexpr T *operator->() {
    return &value();
  }

  [[nodiscard]] constexpr T &operator*() &{
    return value();
  }

  [[nodiscard]] constexpr T &&operator*() &&{
    return std::move(value());
  }

  [[nodiscard]] bool has_initializer() const noexcept {
    return initializer_;
  }

  void initialize() {
    emplace(initializer_());
  }

  explicit operator std::optional<T> &() noexcept {
    return *this;
  }

  explicit operator const std::optional<T> &() const noexcept {
    return *this;
  }

 private:
  initializer_type initializer_;
};

}

#endif //LAZY_TYPE__LAZY_HPP_
