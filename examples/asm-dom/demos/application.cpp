#include "demo.hpp"

namespace {

/// [is-prime]
bool
is_prime(int n)
{
    if (n < 2)
        return false;
    int sqrt_n = int(std::round(std::sqrt(n)));
    for (int i = 2; i <= sqrt_n; ++i)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}
/// [is-prime]

} // namespace

namespace simple_apply {

void
do_ui(dom::context ctx, duplex<int> n)
{
    // clang-format off
/// [simple-apply]
dom::do_text(ctx, "Enter N:");
dom::do_input(ctx, n);
auto n_is_prime = apply(ctx, is_prime, n);
dom::do_text(ctx, conditional(n_is_prime, "N is prime!", "N is NOT prime."));
/// [simple-apply]
    // clang-format on
}

void
init_demo(std::string dom_id)
{
    static alia::system the_system;
    static dom::system the_dom;

    initialize(the_dom, the_system, dom_id, [](dom::context ctx) {
        do_ui(ctx, get_state(ctx, empty<int>()));
    });
}

static demo the_demo("simple-apply", init_demo);

} // namespace simple_apply

namespace transform_demo {

void
do_ui(dom::context ctx)
{
    // clang-format off
/// [transform-demo]
// We want to work with a container of integers here, so get the state to
// represent that. (We initialize it to a vector of three 2s.)
auto numbers = get_state(
    ctx, lambda_constant([]() { return std::vector<int>(3, 2); }));

dom::do_text(ctx, "Enter some numbers:");

// Provide an input box for each number.
for_each(ctx, numbers,
    [ ](auto ctx, auto n) { dom::do_input(ctx, n); });

// Transform the vector of numbers to a vector of bools, indicating whether or
// not each number is prime.
auto prime_flags = transform(ctx, numbers,
    [&](auto ctx, auto n) { return apply(ctx, is_prime, n); });

// Count the number of true values.
auto prime_count = apply(ctx,
    [ ](auto flags) { return std::count(flags.begin(), flags.end(), true); },
    prime_flags);

dom::do_text(ctx, printf(ctx, "# of primes: %d", prime_count));

/// [transform-demo]
    // clang-format on
}

void
init_demo(std::string dom_id)
{
    static alia::system the_system;
    static dom::system the_dom;

    initialize(the_dom, the_system, dom_id, do_ui);
}

static demo the_demo("transform-demo", init_demo);

} // namespace transform_demo

namespace metered_transform_demo {

void
do_ui(dom::context ctx)
{
    static int call_count = 0;
    auto counting_is_prime = [&](int n) {
        ++call_count;
        return is_prime(n);
    };

    auto numbers = get_state(
        ctx, lambda_constant([]() { return std::vector<int>(3, 2); }));

    dom::do_text(ctx, "Enter some numbers:");

    for_each(ctx, numbers, [](auto ctx, auto n) { dom::do_input(ctx, n); });

    auto prime_flags = transform(ctx, numbers, lift(counting_is_prime));

    auto prime_count = apply(
        ctx,
        [&](auto flags) {
            return std::count(flags.begin(), flags.end(), true);
        },
        prime_flags);

    dom::do_text(ctx, printf(ctx, "# of primes: %d", prime_count));

    dom::do_text(
        ctx, printf(ctx, "is_prime has been called %d times.", call_count));
}

void
init_demo(std::string dom_id)
{
    static alia::system the_system;
    static dom::system the_dom;

    initialize(the_dom, the_system, dom_id, do_ui);
}

static demo the_demo("metered-transform-demo", init_demo);

} // namespace metered_transform_demo

namespace direct_counting {

void
do_ui(dom::context ctx)
{
    // clang-format off
auto numbers = get_state(
    ctx, lambda_constant([]() { return std::vector<int>(3, 2); }));

dom::do_text(ctx, "Enter some numbers:");

for_each(ctx, numbers,
    [ ](auto ctx, auto n) { dom::do_input(ctx, n); });

/// [direct-counting]
auto prime_count = apply(ctx,
    [ ](auto numbers)
    { return std::count_if(numbers.begin(), numbers.end(), is_prime); },
    numbers);
/// [direct-counting]

dom::do_text(ctx, printf(ctx, "# of primes: %d", prime_count));
    // clang-format on
}

void
init_demo(std::string dom_id)
{
    static alia::system the_system;
    static dom::system the_dom;

    initialize(the_dom, the_system, dom_id, do_ui);
}

static demo the_demo("direct-counting", init_demo);

} // namespace direct_counting

namespace metered_direct_counting {

void
do_ui(dom::context ctx)
{
    auto numbers = get_state(
        ctx, lambda_constant([]() { return std::vector<int>(3, 2); }));

    static int call_count = 0;
    auto counting_is_prime = [&](int n) {
        ++call_count;
        return is_prime(n);
    };

    dom::do_text(ctx, "Enter some numbers:");

    for_each(ctx, numbers, [](auto ctx, auto n) { dom::do_input(ctx, n); });

    auto prime_count = apply(
        ctx,
        [&](auto numbers) {
            return std::count_if(
                numbers.begin(), numbers.end(), counting_is_prime);
        },
        numbers);

    dom::do_text(ctx, printf(ctx, "# of primes: %d", prime_count));

    dom::do_text(
        ctx, printf(ctx, "is_prime has been called %d times.", call_count));
}

void
init_demo(std::string dom_id)
{
    static alia::system the_system;
    static dom::system the_dom;

    initialize(the_dom, the_system, dom_id, do_ui);
}

static demo the_demo("metered-direct-counting", init_demo);

} // namespace metered_direct_counting
