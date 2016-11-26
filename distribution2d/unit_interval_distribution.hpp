#pragma once

#include <limits>
#include <random>
#include <type_traits>
#include <algorithm>

namespace dist2d
{
namespace detail
{


template<class T>
using is_integral_generator = typename std::is_integral<
  typename std::result_of<T&()>::type
>::type;


} // end detail


// a uniform distribution of points in [0,1)
template<class Point = float>
class unit_interval_distribution
{
  public:
    using real_type = Point;

    template<class Integer>
    real_type operator()(Integer i) const
    {
      // see https://xor0110.wordpress.com/2010/09/24/how-to-generate-floating-point-random-numbers-efficiently/
      union
      {
        Integer i;
        float f;
      } u;

      u.i = (i & 0x007fffff) | 0x3f800000;
      return u.f-1.f;
    }

    // XXX this only makes sense if g generates numbers which span the entire range of the result type
    //     some random number generators don't do that
    template<class Generator,
             class = typename std::enable_if<
               detail::is_integral_generator<Generator>::value
             >::type>
    real_type operator()(Generator& g) const
    {
      return operator()(g());
    }

    static bool contains(const real_type& x)
    {
      return real_type(0) <= x && x < real_type(1);
    }

    // if !contains(p) the result is undefined
    static real_type probability_density(const real_type& p)
    {
      return real_type(1);
    }

    static real_type area()
    {
      return 1.f;
    }
};


constexpr unit_interval_distribution<> u01f{};


} // end dist2d

