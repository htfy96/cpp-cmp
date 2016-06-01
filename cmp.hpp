#ifndef CPP_CMP_HEADER

#if __cplusplus < 201103L
#error "this header require C++11 support of your compiler"
#endif

#include <tuple>
#include <functional>

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
