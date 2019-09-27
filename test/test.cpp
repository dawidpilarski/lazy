//
// Created by pilarski on 23/09/2019.
//

#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include <lazy/lazy.hpp>

using milli::lazy;

TEST_CASE("constructors type_traits"){
  {lazy<int> test = {};} // should compile because of default constructor
  REQUIRE(std::is_default_constructible_v<lazy<int>>);

  struct Test{Test() = delete;};
  REQUIRE(std::is_nothrow_default_constructible_v<lazy<Test>>);
  REQUIRE(std::is_copy_constructible_v<lazy<Test>>);
  REQUIRE(std::is_move_constructible_v<lazy<Test>>);

  auto void_lambda = []{};
  auto int_lambda = []() -> int {return 42;};
  REQUIRE_FALSE(std::is_constructible_v<lazy<Test>, decltype(void_lambda)>);
  REQUIRE(std::is_constructible_v<lazy<int>, decltype(int_lambda)>);
  REQUIRE(std::is_constructible_v<lazy<int>, std::optional<int>>);
  REQUIRE(std::is_constructible_v<lazy<int>, int>);
}

TEST_CASE("operator type_traits"){
  REQUIRE(std::is_convertible_v<lazy<int>, std::optional<int>>);
  REQUIRE_FALSE(std::is_convertible_v<std::optional<int>, lazy<int>>);
  REQUIRE_FALSE(std::is_convertible_v<const lazy<int>, const std::optional<int>&>);
  REQUIRE(std::is_convertible_v<const lazy<int>, const std::optional<int>&>);
}