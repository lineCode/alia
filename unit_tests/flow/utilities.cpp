#define ALIA_LOWERCASE_MACROS

#include <alia/flow/utilities.hpp>

#include <catch.hpp>

#include <alia/signals/basic.hpp>

#include "traversal.hpp"

using namespace alia;

using std::string;

TEST_CASE("make_returnable_ref", "[flow][for_each]")
{
    alia::system sys;

    auto function_that_returns = [](context ctx) -> readable<string> {
        return make_returnable_ref(ctx, value(string("something")));
    };

    auto controller = [&](context ctx) {
        auto s = function_that_returns(ctx);
        REQUIRE(signal_is_readable(s));
        REQUIRE(read_signal(s) == "something");
    };

    do_traversal(sys, controller);
}
