#pragma once

#include "unit_square_distribution.hpp"
#include <utility>
#include <tuple>
#include <limits>
#include <iostream>
#include <cmath>

namespace dist2d
{


// a uniform distribution of points on the unit disk
template<class Point = std::pair<float,float>>
class unit_disk_distribution
{
  public:
    using result_type = Point;

  private:
    using real_type1 = typename std::tuple_element<0,result_type>::type;
    using real_type2 = typename std::tuple_element<1,result_type>::type;

  public:
    using real_type = typename std::common_type<real_type1, real_type2>::type;

  private:
    static constexpr real_type pi = 3.14159265;

  public:
    template<class Integer1, class Integer2>
    result_type operator()(Integer1 x, Integer2 y) const
    {
      unit_square_distribution<Point> square;

      real_type1 u;
      real_type2 v;
      std::tie(u,v) = square(x,y);

      real_type1 r = std::sqrt(u);
      real_type2 theta = real_type2(2) * pi * v;

      return result_type{r * std::cos(theta), real_type2(r) * std::sin(theta)};
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
      real_type radius_squared = std::get<0>(p) * std::get<0>(p) + std::get<1>(p) * std::get<1>(p);

      return real_type(0) <= radius_squared && radius_squared < real_type(1);
    }

    // if !contains(p) the result is undefined
    constexpr static real_type probability_density(const result_type& p)
    {
      return real_type(1) / area();
    }

    constexpr static real_type area()
    {
      return pi;
    }
};


} // end dist2d

