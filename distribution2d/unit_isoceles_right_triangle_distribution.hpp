#pragma once

#include "unit_square_distribution.hpp"
#include <utility>
#include <tuple>
#include <limits>
#include <iostream>
#include <random>
#include <type_traits>

namespace dist2d
{
namespace detail
{


template<class T>
using is_integral_generator = typename std::is_integral<
  typename std::result_of<T&()>::type
>::type;


} // end detail


// a uniform distribution of points on the isoceles right triangle with unit side length.
template<class Point = std::pair<float,float>>
class unit_isoceles_right_triangle_distribution
{
  public:
    using result_type = Point;

  private:
    using real_type1 = typename std::tuple_element<0,result_type>::type;
    using real_type2 = typename std::tuple_element<1,result_type>::type;

  public:
    using real_type = typename std::common_type<real_type1,real_type2>::type;

    template<class Float1, class Float2>
    typename std::enable_if<
      std::is_floating_point<Float1>::value && std::is_floating_point<Float2>::value,
      result_type
    >::type
      operator()(Float1 u1, Float2 u2) const
    {
      // (u1, u2) in [0,1)^2
      real_type1 su1 = std::sqrt(u1);

      // su1 in [0,1)

      // x varies from (0, 1]
      real_type1 x = real_type1(1) - su1;

      // y varies from [0, 1 - x)
      real_type2 y = u2 * su1;

      return result_type{x, y};
    }

    template<class Integer1, class Integer2>
    typename std::enable_if<
      std::is_integral<Integer1>::value && std::is_integral<Integer2>::value,
      result_type
    >::type
      operator()(Integer1 urn1, Integer2 urn2) const
    {
      real_type1 u1;
      real_type2 u2;
      unit_square_distribution<std::pair<real_type1,real_type2>> square;

      std::tie(u1,u2) = square(urn1, urn2);

      return operator()(u1,u2);
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
      const real_type1& x = std::get<0>(p);
      const real_type2& y = std::get<1>(p);

      return (real_type1(0) < x && x <= real_type1(1)) && (real_type2(0) <= y && y <= real_type1(1) - x);
    }

    // if !contains(p) the result is undefined
    static real_type probability_density(const result_type&)
    {
      return real_type(1) / area();
    }

    static real_type area()
    {
      return 0.5;
    }
};


} // end dist2d

