#include "distribution2d/unit_disk_distribution.hpp"
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

  dist2d::unit_disk_distribution<> dist;

  // integrate over the area
  size_t n = 1;
  float sum = 0;
  for(int i = 0; i < n; ++i)
  {
    auto p = dist(rng);
    assert(dist.contains(p));

    float pdf = dist.probability_density(p);
    assert(pdf != 0);

    sum += 1.f / pdf;
  }

  sum /= n;

  // XXX isn't the integral of pdf supposed to integrate to 1?
  assert(almost_equal(dist.area(), sum));

  std::cout << "OK" << std::endl;

  return 0;
}

