/************************************************************
 Header-only library providing smart `cmp` function for C++11

 This is header file of cpp-cmp


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

#ifndef CPP_CMP_HEADER
#define CPP_CMP_HEADER

#if __cplusplus < 201103L
#error "this header require C++11 support of your compiler"
#endif

#include <tuple>
#include <functional>
#include <cstddef>

namespace cppcmp
{
    template<typename T>
        inline int cmp(const T& a, const T& b);

    namespace impl
    {
        template<std::size_t remain, typename ... Args>
            struct unified_tuple_comparator
            {
                unified_tuple_comparator() {}
                int operator() (const std::tuple<Args...>& a, const std::tuple<Args...> &b) const
                {
                    const std::size_t pos = sizeof...(Args) - remain;
                    auto t = cmp(std::get<pos>(a), std::get<pos>(b));
                    if (t == 0) 
                    {
                        unified_tuple_comparator<remain - 1, Args...> next;
                        return next(a, b);
                    } else {
                        return t;
                    }
                }
            };

        template<typename ... Args>
            struct unified_tuple_comparator<0, Args...>
            {
                unified_tuple_comparator() {}
                int operator() (const std::tuple<Args...>& a, const std::tuple<Args...> &b) const
                {
                    (void)(a);
                    (void)(b);
                    return 0;
                }
            };

        template<typename T, typename U = decltype(std::declval<T>() < std::declval<T>()),
            typename V = decltype(std::declval<T>() == std::declval<T>())>
                inline int cmp_impl(long, const T& a, const T & b)
                {
                    if (a<b)
                        return -1;
                    else
                        return a == b ? 0 : 1;
                }

        template<typename ... Args>
            inline int cmp_impl(int, const std::tuple<Args...>& a, const std::tuple<Args...> &b)
            {
                const impl::unified_tuple_comparator<sizeof...(Args), Args...> comparator;
                return comparator(a, b);
            }

        template<typename T, typename = decltype(std::declval<const T>().cmp(std::declval<const T>()))>
            inline int cmp_impl(char, const T& a, const T& b)
            {
                return a.cmp(b);
            }
    }

    template<typename T>
        inline int cmp(const T& a, const T& b)
        {
            return impl::cmp_impl('\0', a, b);
        }

}
#endif
