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
      real_type result = real_type(i) / (real_type(std::numeric_limits<Integer>::max()) - std::numeric_limits<Integer>::min());

      // the above expression produces 1, so clamp the result to 1 - epsilon
      result = std::min(real_type(1) - std::numeric_limits<real_type>::epsilon(), result);

      return result;
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


} // end dist2d

