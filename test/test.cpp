//
// Created by pilarski on 23/09/2019.
//

#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include <lazy/lazy.hpp>

using milli::lazy;

struct throwing { throwing() noexcept(false) {}};

TEST_CASE("is nothrow default constructible") {
  lazy<int> test = {};
  REQUIRE(std::is_default_constructible_v<lazy<int>>);
  REQUIRE(std::is_nothrow_default_constructible_v<lazy<int>>);
  REQUIRE(std::is_nothrow_default_constructible_v<lazy<throwing>>);

  REQUIRE_FALSE(test.has_value());
  REQUIRE_FALSE(test.has_initializer());
}

TEST_CASE("is constructible with optional"){
  REQUIRE(std::is_constructible_v<lazy<int>, std::optional<int>>);
  REQUIRE(std::is_constructible_v<lazy<int>, int>);

  SECTION("optional is empty"){
    lazy<int> mylazy{std::optional<int>{}};
    REQUIRE_FALSE(mylazy.has_value());
    REQUIRE_FALSE(mylazy.has_initializer());
  }

  SECTION("optional is not empty"){
    lazy<int> mylazy{std::optional<int>{5}};
    REQUIRE(mylazy.has_value());
    REQUIRE_FALSE(mylazy.has_initializer());
  }
}

TEST_CASE("construction with initializer"){

  SECTION("initializer is empty"){
    lazy<int> mylazy{std::function<int()>{}};
    REQUIRE_FALSE(mylazy.has_initializer());
    REQUIRE_FALSE(mylazy.has_value());
  }

  SECTION("initializer is not empty"){
    lazy<int> mylazy{[](){return 42;}};
    REQUIRE(mylazy.has_initializer());
    REQUIRE_FALSE(mylazy.has_value());
  }

  SECTION("initializer return type must match stored type"){
    auto void_lambda = [] {};
    auto int_lambda = []() -> int { return 42; };
    REQUIRE_FALSE(std::is_constructible_v<lazy<int>, decltype(void_lambda)>);
    REQUIRE(std::is_constructible_v<lazy<int>, decltype(int_lambda)>);
  }
}

TEST_CASE("constructible variants") {
  REQUIRE(std::is_copy_constructible_v<lazy<int>>);
  REQUIRE(std::is_move_constructible_v<lazy<int>>);

  SECTION("copy constructible with empty lazy") {
    const lazy<int> empty{};
    lazy<int> copy = empty;
    lazy<int> move = std::move(empty);

    REQUIRE_FALSE(copy.has_initializer());
    REQUIRE_FALSE(copy.has_value());
    REQUIRE_FALSE(move.has_initializer());
    REQUIRE_FALSE(move.has_value());
  }

  SECTION("constructible with empty optional and set initializer") {
    const lazy<int> empty_optional{[](){return 42;}};
    lazy<int> copy = empty_optional;
    lazy<int> move = std::move(empty_optional);

    REQUIRE(copy.has_initializer());
    REQUIRE_FALSE(copy.has_value());
    REQUIRE(move.has_initializer());
    REQUIRE_FALSE(move.has_value());
  }

  SECTION("copy constructible with set optional and empty initializer"){
    const lazy<int> empty_initializer{42};
    lazy<int> copy = empty_initializer;
    lazy<int> move = std::move(empty_initializer);


    REQUIRE(copy.has_value());
    REQUIRE_FALSE(copy.has_initializer());
    REQUIRE(move.has_value());
    REQUIRE_FALSE(move.has_initializer());
  }

  SECTION("copy constructible with set optional and set initializer"){
    const lazy<int> fully_initialized{43,[](){return 42;} };
    lazy<int> copy = fully_initialized;
    lazy<int> move = std::move(fully_initialized);

    REQUIRE(copy.has_value());
    REQUIRE(copy.has_initializer());
    REQUIRE(move.has_value());
    REQUIRE(move.has_initializer());
  }
}

TEST_CASE("is compatible with std::optional") {
  REQUIRE(std::is_convertible_v<lazy<int>, std::optional<int>>);
  REQUIRE_FALSE(std::is_convertible_v<std::optional<int>, lazy<int>>);
  REQUIRE(std::is_convertible_v<const lazy<int>, const std::optional<int>&>);
}