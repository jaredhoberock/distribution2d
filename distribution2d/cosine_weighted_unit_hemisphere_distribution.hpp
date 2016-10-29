#pragma once

#include "concentric_unit_disk_distribution.hpp"
#include <tuple>
#include <utility>
#include <algorithm>
#include <cmath>
#include <type_traits>

namespace dist2d
{


// a cosine-weighted distribution of points on the unit hemisphere
template<class Point = std::tuple<float,float,float>>
class cosine_weighted_unit_hemisphere_distribution
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
      concentric_unit_disk_distribution<std::pair<real_type1,real_type2>> unit_disk;

      real_type1 x;
      real_type2 y;
      std::tie(x,y) = unit_disk(urn1,urn2);

      real_type3 z = std::sqrt(std::max(real_type3(0), real_type3(1) - x*x - y*y));

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
      // p must not be in the -z hemisphere
      if(std::get<2>(p) < 0) return false;

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
      return real_type(2) * pi;
    }
};


} // end dist2d


