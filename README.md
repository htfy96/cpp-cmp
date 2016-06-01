# Cpp-cmp

Header-only library providing smart `cmp` function for C++11.

## Usage

```cpp
#include "cmp.hpp"
```

```
using namespace cppcmp;
// cppcmp::cmp
assert(-1 == cmp(-3, 5));
//assert(1 == cmp(100L, '\0')); // must be of the same type


assert(-1 == cmp( std::forward_as_tuple(std::string("AAA"), 20),
                std::forward_as_tuple(std::string("AAA"), 30)
              ));

std::string x("abcd");
assert(0 == cmp( std::forward_as_tuple(x.size(), x),
                std::forward_as_tuple(x.size(), x)
                ));


struct user_defined_type
{
    // ...
    // User defined type should provide < and ==
    bool operator<(const user_defined_type& other) const
    {
        return x_ < other.x_;
    }
    bool operator==(const user_defined_type& other) const
    {
        return x_ == other.x_;
    }
};

user_defined_type a(23), b(16);
assert(1 == cmp(a, b));


struct user_defined_type_cmp
{
    // ...
    // you can also provide 'cmp' member function, which
    // will have highest priority
    int cmp(const user_defined_type_cmp& other) const
    {
        if (x_ == other.x_) return 0;
        return x_ < other.x_ ? -1 : 1;
    }
};
user_defined_type_cmp c(-10), d(42);
assert(-1, cmp(c, d));
```

## License

Apache License 2.0
