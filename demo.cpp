#include "distribution2d/unit_hemisphere_distribution.hpp"
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
  std::default_random_engine rng;

  dist2d::unit_hemisphere_distribution<> dist;

  // compute a Monte Carlo estimate of the area of the distribution 
  size_t n = 10000;
  float estimate = 0;
  for(int i = 0; i < n; ++i)
  {
    auto p = dist(rng);
    assert(dist.contains(p));

    float pdf = dist.probability_density(p);
    assert(pdf != 0);

    estimate += 1.f / pdf;
  }

  estimate /= n;

  assert(almost_equal(dist.area(), estimate));

  std::cout << "OK" << std::endl;

  return 0;
}

