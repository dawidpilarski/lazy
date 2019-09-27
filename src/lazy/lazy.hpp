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
class lazy : public std::optional<T> {
 public:
  using value_type = T;
  using initializer_type = std::function<value_type()>;

  //todo: why does special member inheriting does not work?
//  using std::optional<T>::optional;
//  using std::optional<T>::operator=;

  constexpr lazy() noexcept = default;
  constexpr lazy(const lazy&) = default;
  constexpr lazy(lazy&&) = default;

  constexpr explicit lazy(const initializer_type& initializer) noexcept : initializer_(initializer) {}
  constexpr explicit lazy(initializer_type&& initializer) noexcept : initializer_(std::move(initializer)) {}

  constexpr lazy(const std::optional<T>& optional) : std::optional<T>{optional}{}
  constexpr lazy(std::optional<T>&& optional) : std::optional<T>{std::move(optional)}{}

  template <typename U, typename std::enable_if_t<std::is_constructible_v<std::optional<T>, U> && not std::is_constructible_v<initializer_type, U>>* = nullptr>
  constexpr explicit lazy(U&& value) : std::optional<U>(std::forward<U>(value)){}

  template <typename U, typename Initializer>
  constexpr lazy(U&& value_initializer, Initializer&& initializer) : std::optional<T>{std::forward<U>(value_initializer)}, initializer_{std::forward<Initializer>(initializer)}{}

  constexpr lazy& operator=(const lazy&) = default;
  constexpr lazy& operator=(lazy&&) noexcept(std::is_nothrow_move_constructible_v<value_type>) = default;
  constexpr lazy& operator=(std::nullopt_t) noexcept(std::is_nothrow_destructible_v<value_type>) {
    initializer_ = nullptr;
    *this = std::nullopt;
    return *this;
  }

  template <typename U>
  constexpr lazy& operator=(U&& rhs){
    std::optional<T>::operator=(std::forward<U>(rhs));
    return *this;
  }

  [[nodiscard]] constexpr T &value() &{
    if (not this->has_value() and initializer_) {
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

  [[nodiscard]] constexpr bool has_initializer() const noexcept {
    return initializer_;
  }

  constexpr void initialize() {
    emplace(initializer_());
  }

 private:
  initializer_type initializer_;
};

}

#endif //LAZY_TYPE__LAZY_HPP_
