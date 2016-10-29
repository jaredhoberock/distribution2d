#pragma once

#include "unit_square_distribution.hpp"
#include <utility>
#include <tuple>
#include <limits>
#include <iostream>
#include <cmath>
#include <cassert>

namespace dist2d
{


// a uniform distribution of points on the unit disk
// this distribution better preserves distances between nearby points
// than does unit_disk_distribution
template<class Point = std::pair<float,float>>
class concentric_unit_disk_distribution
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
    result_type operator()(Integer1 urn1, Integer2 urn2) const
    {
      unit_square_distribution<Point> square;

      real_type1 u1;
      real_type2 u2;
      std::tie(u1,u2) = square(urn1,urn2);

      // map uniform random variables to [-1, 1) x [-1, 1)
      real_type1 sx = real_type1(2) * u1 - real_type2(1);
      real_type2 sy = real_type2(2) * u2 - real_type2(1);

      // map square to (r, theta)
      real_type r = 0;
      real_type theta = 0;

      // handle degeneracy at the origin
      if(sx == 0 && sy == 0)
      {
        return result_type{0,0};
      } // end if

      if(sx >= -sy)
      {
        if(sx > sy)
        {
          // first region of disk
          r = sx;
          if(sy > 0)
          {
            theta = sy / r;
          }
          else
          {
            theta = real_type(8) + sy / r;
          }
        }
        else
        {
          // second region of disk
          r = sy;
          theta = real_type(2) - sx/r;
        }
      }
      else
      {
        if(sx <= sy)
        {
          // third region of disk
          r = -sx;
          theta = real_type(4) - sy/r;
        }
        else
        {
          // fourth region of disk
          r = -sy;
          theta = real_type(6) + sx/r;
        }
      }

      theta *= real_type(0.25) * pi;

      return result_type{r * std::cos(theta), r * std::sin(theta)};
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
      real_type radius = std::sqrt(radius_squared);
      return real_type(0) <= radius && radius <= real_type(1);
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

