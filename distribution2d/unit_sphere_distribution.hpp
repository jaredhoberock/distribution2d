#pragma once

#include "unit_square_distribution.hpp"
#include <tuple>
#include <utility>
#include <algorithm>
#include <cmath>
#include <type_traits>

namespace dist2d
{


// a uniform distribution of points on the unit sphere
template<class Point = std::tuple<float,float,float>>
class unit_sphere_distribution
{
  public:
    using result_type = Point;

  private:
    using real_type1 = typename std::tuple_element<0,result_type>::type;
    using real_type2 = typename std::tuple_element<1,result_type>::type;
    using real_type3 = typename std::tuple_element<2,result_type>::type;

  public:
    using real_type = typename std::common_type<real_type1, real_type2, real_type3>::type;

  private:
    static constexpr real_type pi = 3.14159265;
    static constexpr real_type two_pi = real_type(2) * pi;

  public:
    template<class Integer1, class Integer2>
    result_type operator()(Integer1 urn1, Integer2 urn2) const
    {
      unit_square_distribution<std::pair<real_type,real_type>> square;

      real_type u1;
      real_type u2;
      std::tie(u1,u2) = square(urn1,urn2);

      real_type3 z = real_type3(1) - real_type3(2)*u1;
      real_type r = std::sqrt(std::max(real_type(0), real_type(1) - z*z));
      real_type phi = two_pi * u2;
      real_type1 x = r * std::cos(phi);
      real_type2 y = r * std::sin(phi);

      return result_type{x,y,z};
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

    template<class Generator>
    result_type operator()(Generator& g) const
    {
      return operator()(g());
    }

    static bool contains(const result_type& p)
    {
      real_type radius_squared = std::get<0>(p) * std::get<0>(p) + std::get<1>(p) * std::get<1>(p) + std::get<2>(p) * std::get<2>(p);

      return std::fabs(real_type(1) - radius_squared) < 0.000005f;
    }

    // if !contains(p) the result is undefined
    constexpr static real_type probability_density(const result_type& p)
    {
      return real_type(1) / area();
    }

    constexpr static real_type area()
    {
      return real_type(4) * pi;
    }
};


} // end dist2d

