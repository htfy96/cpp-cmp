#include <cassert>
#include <tuple>
#include <string>
#include "cmp.hpp"

#define ASSERT_EQ(X, Y) assert((X == Y) && (#X "should == " #Y))
void test_simple_type()
{
    using namespace cppcmp;
    ASSERT_EQ(-1, cmp(-3, 5));
    ASSERT_EQ(0, cmp(1.0, 1.0));
    ASSERT_EQ(-1, cmp('a', 'b'));
    ASSERT_EQ(0, cmp(false, false));
    ASSERT_EQ(1, cmp(100L, 99L));
}

void test_tuple()
{
    using namespace cppcmp;
    ASSERT_EQ(-1, cmp(std::forward_as_tuple(10, 20),
                    std::forward_as_tuple(10, 30)));
    ASSERT_EQ(1, cmp(std::forward_as_tuple(1, 'Z'),
                    std::forward_as_tuple(1, 'K')));
    ASSERT_EQ(0, cmp(std::forward_as_tuple(std::string("ababab"),
                        std::make_pair(25, 1.0)),
                    std::forward_as_tuple(std::string("ababab"),
                        std::make_pair(25, 1.0))));
}

namespace
{
    template<typename T>
    struct user_defined_type
    {
        T x_;
        user_defined_type(const T& x): x_(x)
        {
        }
        bool operator<(const user_defined_type& other) const
        {
            return x_ < other.x_;
        }
        bool operator==(const user_defined_type& other) const
        {
            return x_ == other.x_;
        }
    };

    template<typename T>
        struct user_defined_type_cmp
        {
            T x_;
            user_defined_type_cmp(const T& x): x_(x)
            {
            }
            int cmp(const user_defined_type_cmp& other) const
            {
                if (x_ == other.x_) return 0;
                return x_ < other.x_ ? -1 : 1;
            }
        };
}

void test_user_defined_type()
{
    using namespace cppcmp;
    user_defined_type<int> a(23), b(16);
    ASSERT_EQ(1, cmp(a, b));
    ASSERT_EQ(0, cmp(b, b));
    ASSERT_EQ(-1, cmp(b,a));

    user_defined_type_cmp<int> c(-10), d(42);
    ASSERT_EQ(-1, cmp(c, d));
    ASSERT_EQ(0, cmp(d, d));
    ASSERT_EQ(1, cmp(d, c));
}


int main()
{
    test_simple_type();
    test_tuple();
    test_user_defined_type();
}

