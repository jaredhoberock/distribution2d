#pragma once

#include "morton/morton.hpp"
#include "unit_interval_distribution.hpp"
#include <utility>
#include <tuple>
#include <limits>
#include <type_traits>

namespace dist2d
{


// a uniform distribution of points in [0,1)^2
template<class Point = std::pair<float,float>>
class unit_square_distribution
{
  public:
    using result_type = Point;
    using real_type = typename std::tuple_element<0,result_type>::type;

  private:
    using real_type1 = typename std::tuple_element<0,result_type>::type;
    using real_type2 = typename std::tuple_element<1,result_type>::type;

  public:
    template<class Integer1, class Integer2>
    result_type operator()(Integer1 x, Integer2 y) const
    {
      real_type1 u = unit_interval_distribution<real_type1>()(x);
      real_type2 v = unit_interval_distribution<real_type2>()(y);

      return result_type{u, v};
    }

    template<class Integer,
             class = typename std::enable_if<
               std::is_integral<Integer>::value
             >::type>
    result_type operator()(Integer i) const
    {
      auto xy = decode_morton_2d(i);
      return operator()(xy.first, xy.second);
    }

    // XXX this only makes sense if g generates numbers which span the entire range of the result type
    //     some random number generators don't do that
    template<class Generator,
             class = typename std::enable_if<
               detail::is_integral_generator<Generator>::value
             >::type>
    result_type operator()(Generator& g) const
    {
      return operator()(g());
    }

    static bool contains(const result_type& p)
    {
      const auto& u = std::get<0>(p);
      const auto& v = std::get<1>(p);

      return real_type1(0) <= u && u < real_type1(1) && real_type2(0) <= v && v < real_type2(1);
    }

    // if !contains(p) the result is undefined
    static real_type probability_density(const result_type& p)
    {
      return real_type(1);
    }

    static real_type area()
    {
      return real_type(1);
    }
};


} // end dist2d

