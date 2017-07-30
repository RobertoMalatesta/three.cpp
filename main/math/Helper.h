//
// Created by byter on 30.07.17.
//

#ifndef THREECPP_HELPER_H
#define THREECPP_HELPER_H

#include <cmath>
#include <algorithm>

namespace three {
namespace math {

static const double DEG2RAD = M_PI / 180;
static const double RAD2DEG = 180 / M_PI;

template <typename T> T clamp(T value, T min, T max)
{
  return std::max(min, std::min(max, value));
}

// compute euclidian modulo of m % n
// https://en.wikipedia.org/wiki/Modulo_operation
template <typename T> T euclideanModulo( T n, T m)
{
  return (( n % m ) + m) % m;
}

// Linear mapping from range <a1, a2> to range <b1, b2>
template <typename T> T mapLinear(T x, T a1, T a2, T b1, T b2 ) 
{
  return b1 + ( x - a1 ) * ( b2 - b1 ) / ( a2 - a1 );
}

// https://en.wikipedia.org/wiki/Linear_interpolation
template <typename T> T lerp(T x, T y, T t) {

  return ( 1 - t ) * x + t * y;

}

// http://en.wikipedia.org/wiki/Smoothstep
template <typename T> T smoothstep(T x, T min, T max ) {
  
  if ( x <= min ) return 0;
  if ( x >= max ) return 1;

  x = ( x - min ) / ( max - min );

  return x * x * ( 3 - 2 * x );
}

template <typename T> T smootherstep(T x, T min, T max )
{
  if ( x <= min ) return 0;
  if ( x >= max ) return 1;

  x = ( x - min ) / ( max - min );

  return x * x * x * ( x * ( x * 6 - 15 ) + 10 );
}

// Random integer from <low, high> interval
int rand(int low, int high)
{
  return low + (int)std::floor(std::rand() * (high - low + 1));
}

// Random float from <low, high> interval

float rand(float low, float high)
{
  return low + std::rand() * ( high - low );
}

// Random float from <-range/2, range/2> interval
float randSpread(float range) {

  return range * ( 0.5f - std::rand() );
}

double degToRad(unsigned short degrees)
{
  return degrees * DEG2RAD;
}

unsigned short radToDeg(double radians)
{
  return (unsigned short)(radians * RAD2DEG);
}

bool isPowerOfTwo(int value )
{
  return (value & (value - 1)) == 0 && value != 0;
}

int nearestPowerOfTwo(int value)
{
  return std::pow( 2, std::round( std::log( value ) / M_LN2 ) );
}

int nextPowerOfTwo(int value )
{
  value --;
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  value ++;

  return value;
}

}
}
#endif //THREECPP_HELPER_H
