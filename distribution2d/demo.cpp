#include "unit_square_distribution.hpp"
#include <random>
#include <cmath>
#include <cassert>
#include <iostream>

bool almost_equal(float x, float y, float epsilon = 0.00001f)
{
  float relative_difference = std::fabs((x - y) / x);
  return relative_difference <= epsilon;
}

int main()
{
  std::default_random_engine rng;

  dist2d::unit_square_distribution<> unit_square;

  // integrate over the unit square
  size_t n = 10;
  float sum = 0;
  for(int i = 0; i < n; ++i)
  {
    auto p = unit_square(rng);
    assert(unit_square.contains(p));

    float pdf = unit_square.probability_density(p);
    assert(pdf != 0);

    sum += unit_square.area() / pdf;
  }

  sum /= n;

  assert(almost_equal(unit_square.area(), sum));

  std::cout << "OK" << std::endl;

  return 0;
}

