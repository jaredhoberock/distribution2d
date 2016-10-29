#pragma once

#include "morton/morton.hpp"
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


// a uniform distribution of points in [0,1)^2
template<class Point = std::pair<float,float>>
class unit_square_distribution
{
  public:
    using result_type = Point;
    using real_type = typename std::tuple_element<0,result_type>::type;

    template<class Integer1, class Integer2>
    result_type operator()(Integer1 x, Integer2 y) const
    {
      using real_type1 = typename std::tuple_element<0,result_type>::type;
      using real_type2 = typename std::tuple_element<1,result_type>::type;

      real_type1 u = real_type1(x) / ((real_type1(std::numeric_limits<Integer1>::max()) - std::numeric_limits<Integer1>::min()) + Integer1(1));
      real_type2 v = real_type2(y) / ((real_type2(std::numeric_limits<Integer2>::max()) - std::numeric_limits<Integer2>::min()) + Integer2(1));

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

      return 0.f <= u && u < 1.f && 0.f <= v && v < 1.f;
    }

    // if !contains(p) the result is undefined
    static real_type probability_density(const result_type& p)
    {
      return 1.f;
    }

    static real_type area()
    {
      return 1.f;
    }
};


} // end dist2d

