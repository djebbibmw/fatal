/*
 *  Copyright (c) 2015, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#include <fatal/type/enum.h>

#include <fatal/test/driver.h>

#include <cstring>

namespace fatal {

FATAL_RICH_ENUM(
  test_enum,
  state0,
  (state1, 4),
  (state2, 97),
  state3
);

enum class custom_enum { field0, field1 = 37, field2 };

struct custom_enum_traits {
  using type = custom_enum;

  struct str {
    FATAL_STR(field0, "field0");
    FATAL_STR(field1, "field1");
    FATAL_STR(field2, "field2");
  };

  using name_to_value = build_type_map<
    str::field0, std::integral_constant<custom_enum, custom_enum::field0>,
    str::field1, std::integral_constant<custom_enum, custom_enum::field1>,
    str::field2, std::integral_constant<custom_enum, custom_enum::field2>
  >;
};

FATAL_REGISTER_ENUM_TRAITS(custom_enum_traits);

///////////
// enums //
///////////

FATAL_TEST(enums, declare_enum) {
  FATAL_EXPECT_EQ(test_enum::state0, static_cast<test_enum>(0));
  FATAL_EXPECT_EQ(test_enum::state1, static_cast<test_enum>(4));
  FATAL_EXPECT_EQ(test_enum::state2, static_cast<test_enum>(97));
  FATAL_EXPECT_EQ(test_enum::state3, static_cast<test_enum>(98));

  FATAL_EXPECT_EQ(custom_enum::field0, static_cast<custom_enum>(0));
  FATAL_EXPECT_EQ(custom_enum::field1, static_cast<custom_enum>(37));
  FATAL_EXPECT_EQ(custom_enum::field2, static_cast<custom_enum>(38));
}

FATAL_TEST(enums, to_string) {
  {
    using traits = enum_traits<test_enum>;

    FATAL_EXPECT_EQ(nullptr, traits::to_string(static_cast<test_enum>(-1)));
    FATAL_EXPECT_EQ("state0", traits::to_string(test_enum::state0));
    FATAL_EXPECT_EQ("state1", traits::to_string(test_enum::state1));
    FATAL_EXPECT_EQ("state2", traits::to_string(test_enum::state2));
    FATAL_EXPECT_EQ("state3", traits::to_string(test_enum::state3));

    FATAL_EXPECT_EQ("", traits::to_string(static_cast<test_enum>(-1), ""));
    FATAL_EXPECT_EQ("state0", traits::to_string(test_enum::state0, ""));
    FATAL_EXPECT_EQ("state1", traits::to_string(test_enum::state1, ""));
    FATAL_EXPECT_EQ("state2", traits::to_string(test_enum::state2, ""));
    FATAL_EXPECT_EQ("state3", traits::to_string(test_enum::state3, ""));
  }

  {
    using traits = enum_traits<custom_enum>;

    FATAL_EXPECT_EQ(nullptr, traits::to_string(static_cast<custom_enum>(-1)));
    FATAL_EXPECT_EQ("field0", traits::to_string(custom_enum::field0));
    FATAL_EXPECT_EQ("field1", traits::to_string(custom_enum::field1));
    FATAL_EXPECT_EQ("field2", traits::to_string(custom_enum::field2));

    FATAL_EXPECT_EQ("", traits::to_string(static_cast<custom_enum>(-1), ""));
    FATAL_EXPECT_EQ("field0", traits::to_string(custom_enum::field0, ""));
    FATAL_EXPECT_EQ("field1", traits::to_string(custom_enum::field1, ""));
    FATAL_EXPECT_EQ("field2", traits::to_string(custom_enum::field2, ""));
  }
}

FATAL_TEST(enums, enum_to_string) {
  FATAL_EXPECT_EQ(nullptr, enum_to_string(static_cast<test_enum>(-1)));
  FATAL_EXPECT_EQ("state0", enum_to_string(test_enum::state0));
  FATAL_EXPECT_EQ("state1", enum_to_string(test_enum::state1));
  FATAL_EXPECT_EQ("state2", enum_to_string(test_enum::state2));
  FATAL_EXPECT_EQ("state3", enum_to_string(test_enum::state3));

  FATAL_EXPECT_EQ("", enum_to_string(static_cast<test_enum>(-1), ""));
  FATAL_EXPECT_EQ("state0", enum_to_string(test_enum::state0, ""));
  FATAL_EXPECT_EQ("state1", enum_to_string(test_enum::state1, ""));
  FATAL_EXPECT_EQ("state2", enum_to_string(test_enum::state2, ""));
  FATAL_EXPECT_EQ("state3", enum_to_string(test_enum::state3, ""));

  FATAL_EXPECT_EQ(nullptr, enum_to_string(static_cast<custom_enum>(-1)));
  FATAL_EXPECT_EQ("field0", enum_to_string(custom_enum::field0));
  FATAL_EXPECT_EQ("field1", enum_to_string(custom_enum::field1));
  FATAL_EXPECT_EQ("field2", enum_to_string(custom_enum::field2));

  FATAL_EXPECT_EQ("", enum_to_string(static_cast<custom_enum>(-1), ""));
  FATAL_EXPECT_EQ("field0", enum_to_string(custom_enum::field0, ""));
  FATAL_EXPECT_EQ("field1", enum_to_string(custom_enum::field1, ""));
  FATAL_EXPECT_EQ("field2", enum_to_string(custom_enum::field2, ""));
}

FATAL_TEST(enums, parse) {
# define CREATE_TEST(e, x) \
  do { \
    std::string const s(FATAL_TO_STR(x)); \
    FATAL_EXPECT_EQ(e::x, enum_traits<e>::parse(s)); \
    FATAL_EXPECT_EQ(e::x, enum_traits<e>::parse(s.begin(), s.end())); \
    FATAL_EXPECT_THROW(std::invalid_argument) { \
      enum_traits<e>::parse(s.begin(), s.begin()); \
    }; \
  } while (false)

  CREATE_TEST(test_enum, state0);
  CREATE_TEST(test_enum, state1);
  CREATE_TEST(test_enum, state2);
  CREATE_TEST(test_enum, state3);

  CREATE_TEST(custom_enum, field0);
  CREATE_TEST(custom_enum, field1);
  CREATE_TEST(custom_enum, field2);

# undef CREATE_TEST

# define CREATE_TEST(e, x) \
  do { \
    std::string const s(FATAL_TO_STR(x)); \
    FATAL_EXPECT_THROW(std::invalid_argument) { \
      enum_traits<e>::parse(s.begin(), std::next(s.begin(), s.size() - 1)); \
    }; \
  } while (false)

  CREATE_TEST(test_enum, state0);
  CREATE_TEST(test_enum, state1);
  CREATE_TEST(test_enum, state2);
  CREATE_TEST(test_enum, state3);

  CREATE_TEST(custom_enum, field0);
  CREATE_TEST(custom_enum, field1);
  CREATE_TEST(custom_enum, field2);

# undef CREATE_TEST

# define CREATE_TEST(e, x) \
  do { \
    std::string const s(FATAL_TO_STR(x) "invalid"); \
    FATAL_EXPECT_THROW(std::invalid_argument) { \
      enum_traits<e>::parse(s); \
    }; \
  } while (false)

  CREATE_TEST(test_enum, state0);
  CREATE_TEST(test_enum, state1);
  CREATE_TEST(test_enum, state2);
  CREATE_TEST(test_enum, state3);

  CREATE_TEST(custom_enum, field0);
  CREATE_TEST(custom_enum, field1);
  CREATE_TEST(custom_enum, field2);

# undef CREATE_TEST

# define CREATE_TEST(e, x) \
  do { \
    std::string const s(FATAL_TO_STR(x) "invalid"); \
    FATAL_EXPECT_THROW(std::invalid_argument) { \
      enum_traits<e>::parse(s.begin(), s.end()); \
    }; \
  } while (false)

  CREATE_TEST(test_enum, state0);
  CREATE_TEST(test_enum, state1);
  CREATE_TEST(test_enum, state2);
  CREATE_TEST(test_enum, state3);

  CREATE_TEST(custom_enum, field0);
  CREATE_TEST(custom_enum, field1);
  CREATE_TEST(custom_enum, field2);

# undef CREATE_TEST

  {
    using traits = enum_traits<test_enum>;

    FATAL_EXPECT_THROW(std::invalid_argument) {
      traits::parse(std::string());
    };
    FATAL_EXPECT_THROW(std::invalid_argument) {
      traits::parse(std::string("invalid"));
    };
  }

  {
    using traits = enum_traits<custom_enum>;

    FATAL_EXPECT_THROW(std::invalid_argument) {
      traits::parse(std::string());
    };
    FATAL_EXPECT_THROW(std::invalid_argument) {
      traits::parse(std::string("invalid"));
    };
  }
}

FATAL_TEST(enums, try_parse) {
# define CREATE_TEST(e, x) \
  do { \
    std::string const s(FATAL_TO_STR(x)); \
    e out = static_cast<e>(-1); \
    FATAL_EXPECT_TRUE(enum_traits<e>::try_parse(out, s)); \
    FATAL_EXPECT_EQ(e::x, out); \
    \
    out = static_cast<e>(-1); \
    FATAL_EXPECT_TRUE(enum_traits<e>::try_parse(out, s.begin(), s.end())); \
    FATAL_EXPECT_EQ(e::x, out); \
    \
    out = static_cast<e>(-1); \
    FATAL_EXPECT_FALSE(enum_traits<e>::try_parse(out, s.begin(), s.begin())); \
    FATAL_EXPECT_EQ(static_cast<e>(-1), out); \
    \
    FATAL_EXPECT_FALSE( \
      enum_traits<e>::try_parse( \
        out, s.begin(), std::next(s.begin(), s.size() - 1) \
      ) \
    ); \
    FATAL_EXPECT_EQ(static_cast<e>(-1), out); \
    \
    auto const i = s + "invalid"; \
    FATAL_EXPECT_FALSE((enum_traits<e>::try_parse(out, i))); \
    FATAL_EXPECT_FALSE((enum_traits<e>::try_parse(out, i.begin(), i.end()))); \
  } while (false)

  CREATE_TEST(test_enum, state0);
  CREATE_TEST(test_enum, state1);
  CREATE_TEST(test_enum, state2);
  CREATE_TEST(test_enum, state3);

  CREATE_TEST(custom_enum, field0);
  CREATE_TEST(custom_enum, field1);
  CREATE_TEST(custom_enum, field2);

# undef CREATE_TEST

  {
    using traits = enum_traits<test_enum>;
    traits::type out;

    FATAL_EXPECT_FALSE(traits::try_parse(out, std::string()));
    FATAL_EXPECT_FALSE(traits::try_parse(out, std::string("invalid")));
  }

  {
    using traits = enum_traits<custom_enum>;
    traits::type out;

    FATAL_EXPECT_FALSE(traits::try_parse(out, std::string()));
    FATAL_EXPECT_FALSE(traits::try_parse(out, std::string("invalid")));
  }
}

} // namespace fatal {
