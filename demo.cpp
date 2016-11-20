#include "distribution2d/unit_isoceles_right_triangle_distribution.hpp"
#include <random>
#include <cmath>
#include <cassert>
#include <iostream>

bool almost_equal(float x, float y, float epsilon = 0.001f)
{
  float relative_difference = std::fabs((x - y) / x);
  return relative_difference <= epsilon;
}

int main()
{
  std::mt19937_64 rng;

  dist2d::unit_isoceles_right_triangle_distribution<> dist;

  // compute a Monte Carlo estimate of the area of the distribution 
  float estimate = 0;
  for(unsigned int i = 0; i < 10000; ++i)
  {
    auto p = dist(rng);
    assert(dist.contains(p));

    float pdf = dist.probability_density(p);
    assert(pdf != 0);

    // update estimate robustly
    estimate += ((1.f / pdf) - estimate)/(i+1);
  }

  assert(almost_equal(dist.area(), estimate));

  std::cout << "OK" << std::endl;

  return 0;
}

