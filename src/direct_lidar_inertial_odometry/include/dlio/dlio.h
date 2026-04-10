/***********************************************************
 *                                                         *
 * Copyright (c)                                           *
 *                                                         *
 * The Verifiable & Control-Theoretic Robotics (VECTR) Lab *
 * University of California, Los Angeles                   *
 *                                                         *
 * Authors: Kenny J. Chen, Ryan Nemiroff, Brett T. Lopez   *
 * Contact: {kennyjchen, ryguyn, btlopez}@ucla.edu         *
 *                                                         *
 ***********************************************************/

// SYSTEM
#include <atomic>

#ifdef HAS_CPUID
#include <cpuid.h>
#endif

#include <ctime>
#include <fstream>
#include <future>
#include <iomanip>
#include <ios>
#include <iostream>
#include <mutex>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/times.h>
#include <thread>

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

// BOOST
#include <boost/format.hpp>

// PCL
#define PCL_NO_PRECOMPILE

// DLIO
#include <nano_gicp/nano_gicp.h>

namespace dlio {
  enum class SensorType { OUSTER, VELODYNE, HESAI, LIVOX, UNKNOWN };

  class OdomNode;
  class MapNode;

  struct Point {
    Point(): data{0.f, 0.f, 0.f, 1.f} {
      intensity = 0.f;
      range = 0;
      ring = 0;
      column = 0;
      t = 0;
    }

    PCL_ADD_POINT4D;
    float intensity; // intensity
    std::uint32_t range;
    union {
      std::uint32_t t;   // (Ouster) time since beginning of scan in nanoseconds
      float time;        // (Velodyne) time since beginning of scan in seconds
      double timestamp;  // (Hesai) absolute timestamp in seconds
                         // (Livox) absolute timestamp in (seconds * 10e9)
    };
    std::uint16_t ring;    // row index (0..H-1)
    std::uint16_t column;  // column index (0..W-1)
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  } EIGEN_ALIGN16;
}

POINT_CLOUD_REGISTER_POINT_STRUCT(dlio::Point,
                                 (float, x, x)
                                 (float, y, y)
                                 (float, z, z)
                                 (float, intensity, intensity)
                                 (std::uint32_t, range, range) 
                                 (std::uint32_t, t, t)
                                 (float, time, time)
                                 (double, timestamp, timestamp)
                                 (std::uint16_t, ring, ring)
                                 (std::uint16_t, column, column))

typedef dlio::Point PointType;
