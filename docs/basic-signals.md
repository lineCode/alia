Basic Usage
===========

Signals are the main mechanic for modeling dataflow in alia, and they're
introduced on the [Key Features](key-features.md) page. If you haven't already,
you should read that before continuing on here.

Signal Object Lifetime
----------------------

It's important to understand that although signals are conceptually values that
vary over time and persist across frames of an application, actual alia signal
objects are generally just transient interfaces to the underlying signal. It's
common to create signal objects on the stack, when and where they're needed, and
even adapt/compose them on the spot to create more interesting signals. For
example, assuming we have some floating point variable `x` that represents some
persistent application state, we might write code like this:

```cpp
do_input(ctx, scale(direct(x), 100));
```

`direct(x)` creates a signal object that directly exposes the variable `x` as a
signal. `scale(direct(x), 100)` presents a view of `x` scaled up by a factor of
100 (as another signal object), which is then passed into `do_input` to allow
the user to edit it. (This might be done because the user wants to edit `x` as a
percentage, while in code we want it as a simple ratio.)

Remember that that line of code lives in a function that is reinvoked on every
update, so while `x` persists across the lifetime of the application, the signal
objects created by `direct()` and `scale()` *do not.* They're created on the
stack solely for the purpose of exposing `x` to `do_input`, and they cease to
exist between updates. And, of course, `x` is the only real *state* here, so `x`
is all that really *needs* to persist between frames.

It's a general principle in alia that objects shouldn't persist across frames
without a good reason &ndash; after all, it would just mean more objects to keep
synchronized &ndash; so this is the most common way to work with signals in
alia. For the sake of brevity, this documentation refers to these transient
signal objects simply as 'signals', and from a conceptual standpoint, it's
perfectly fine to think of `scale(direct(x), 100)` as representing a persistent
signal that changes over time, but for the purpose of actually understanding
what's going on in your code, it's also helpful to be aware that there's no
actual persistent C++ object associated with that signal.

Basic Constructors
------------------

The following functions allow you to construct basic signals from raw C++
values. These are perfect for working with small values: booleans, numbers,
small strings, and perhaps small data structures. However, all of them rely on
making copies and invoking the equality operator to detect changes in the values
they carry, so they are generally not the best choice for large data structures
unless those structures are especially efficient at these operations.

<dl>

<dt>value(T x)</dt><dd>

Returns a *read-only* signal carrying the value `x`.

Internally, the signal stores a *copy* of the value.
<dd>

<dt>value(char const* x)</dt><dd>

Returns a *read-only* signal carrying a `std::string` initialized with `x`.

The value ID logic for this signal assumes that this overload is only used for
**string literals** (i.e., that the contents of the string will never change).
If you're doing real C-style string manipulations, you should convert them to
`std::string` first or use a custom signal.
</dd>

<dt>direct(T& x)</dt><dd>

Returns a *bidirectional* signal carrying the value `x`.

Internally, the signal stores a *reference* to the value.
</dd>

<dt>direct(T const& x)</dt><dd>

Returns a *read-only* signal carrying the value `x`.

Internally, the signal stores a *reference* to the value.
</dd>

</dl>

The Empty Signal
----------------

Occasionally, it's useful to create a signal that never carries a value.

<dl>

<dt>empty&lt;T&gt;()</dt><dd>

Returns a signal that type checks as a readable signal carrying a value of type
`T` but never actually provides a value.

</dd>

</dl>