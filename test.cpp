/************************************************************
 Header-only library providing smart `cmp` function for C++11

 This is test file of cpp-cmp


 Copyright 2016 Zheng Luo(vicluo96@gmail.com)

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 ************************************************************/

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


namespace
{
    template<typename T>
        struct user_defined_type_nontemplate_cmp
    {
        T x_;
        user_defined_type_nontemplate_cmp(const T& x): x_(x)
        {
        }
    };
}

namespace cppcmp
{
    template<typename T>
        int cmp(const user_defined_type_nontemplate_cmp<T>& a,
                    const user_defined_type_nontemplate_cmp<T>& b)
        {
            return cppcmp::cmp(a.x_, b.x_);
        }
}

void test_nontemplate_cmp()
{
    using namespace cppcmp;
    user_defined_type_nontemplate_cmp<int> a(20), b(-10);
    ASSERT_EQ(1, cmp(a, b));
}

namespace
{
    struct precedence_tester
    {
        bool operator<(const precedence_tester&) const
        {
            return false;
        }
        bool operator==(const precedence_tester&) const
        {
            return true;
        }

        int cmp(const precedence_tester&) const
        {
            return 10;
        }
    };

    struct precedence_tester2
    {
        bool operator<(const precedence_tester2&) const
        {
            return false;
        }
        bool operator==(const precedence_tester2&) const
        {
            return true;
        }

        int cmp(const precedence_tester2&) const
        {
            return 10;
        }
    };
}

namespace cppcmp
{
    int cmp(const precedence_tester& a, const precedence_tester &b)
    {
        (void) (a); 
        (void) (b);
        return -10;
    }
}

void test_precedence()
{
    using namespace cppcmp;
    precedence_tester a, b;
    ASSERT_EQ(-10, cmp(a, b));

    precedence_tester2 c, d;
    ASSERT_EQ(10, cmp(c, d));
}


int main()
{
    test_simple_type();
    test_tuple();
    test_user_defined_type();
    test_nontemplate_cmp();
    test_precedence();
}

