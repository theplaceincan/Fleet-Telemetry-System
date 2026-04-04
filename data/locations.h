#ifndef LOCATIONS_H
#define LOCATIONS_H

#include <vector>
#include "common/types.h"

inline const std::vector<Location> BASES = {
  {36.1699, -115.1398, "Main Launch Base - Downtown Las Vegas"},
  {36.0840, -115.1537, "West Operations Base - Las Vegas"},
  {36.0627, -115.0467, "East Operations Base - Sunrise Manor"}
};

inline const std::vector<Location> DESTINATIONS = {
  {36.1716, -115.1469, "Fremont Street, Las Vegas, NV"},
  {36.1126, -115.1767, "Allegiant Stadium, Las Vegas, NV"},
  {36.1024, -115.1745, "T-Mobile Arena, Las Vegas, NV"},
  {36.1215, -115.1739, "Bellagio, Las Vegas, NV"},
  {36.1362, -115.1634, "Las Vegas Convention Center, Las Vegas, NV"},
  {36.1475, -115.1566, "University of Nevada, Las Vegas, NV"},
  {36.1256, -115.3157, "Red Rock Area, Las Vegas, NV"},
  {36.2008, -115.2806, "Summerlin Area, Las Vegas, NV"},
  {36.0800, -115.1522, "South Las Vegas Residential Dropoff"},
  {36.1902, -115.0621, "Northeast Las Vegas Delivery Zone"}
};

#endif