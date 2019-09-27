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

  lazy() noexcept = default;
  lazy(const lazy&) = default;
  lazy(lazy&&) = default;
  lazy(std::nullopt_t) noexcept : lazy(){}


  explicit lazy(const initializer_type& initializer) noexcept : initializer_(initializer) {}
  explicit lazy(initializer_type&& initializer) noexcept : initializer_(std::move(initializer)) {}
  template <typename U, typename = std::enable_if_t<std::is_constructible_v<std::optional<T>, U> && not std::is_constructible_v<initializer_type, U>>>
  explicit lazy(U&& value) : std::optional<T>(std::forward<U>(value)){}
  template <typename U, typename Initializer>
  lazy(U&& optional_initializer, Initializer&& initializer) : std::optional<T>{std::forward<U>(optional_initializer)}, initializer_{std::forward<Initializer>(initializer)}{}

  lazy& operator=(const lazy&) = default;
  lazy& operator=(lazy&&) noexcept(std::is_nothrow_move_constructible_v<value_type>) = default;
  lazy& operator=(std::nullopt_t) noexcept(std::is_nothrow_destructible_v<value_type>) {
    initializer_ = nullptr;
    *this = std::nullopt;
    return *this;
  }

  template <typename U, typename = std::enable_if_t<std::is_constructible_v<std::optional<T>, U> && not std::is_constructible_v<initializer_type, U>>>
  lazy& operator=(U&& rhs){
    std::optional<T>::operator=(std::forward<U>(rhs));
    return *this;
  }

  [[nodiscard]] T &value() &{
    if (not this->has_value() and initializer_) {
      initialize();
    }

    return std::optional<T>::value();
  }

  [[nodiscard]] T &&value() &&{
    return std::move(value());
  }

  [[nodiscard]] T *operator->() {
    return &value();
  }

  [[nodiscard]] T &operator*() &{
    return value();
  }

  [[nodiscard]] T &&operator*() &&{
    return std::move(value());
  }

  [[nodiscard]] bool has_initializer() const noexcept {
    return static_cast<bool>(initializer_);
  }

  void set_initializer(const initializer_type& initializer) noexcept(std::is_nothrow_copy_assignable_v<initializer_type>) {initializer_ = initializer;}
  void set_initializer(initializer_type&& initializer) noexcept (std::is_nothrow_move_assignable_v<initializer_type>) {initializer_ = std::move(initializer);}

  void initialize() {
    emplace(initializer_());
  }

 private:
  initializer_type initializer_;
};

}

#endif //LAZY_TYPE__LAZY_HPP_
