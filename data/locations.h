#ifndef LOCATIONS_H
#define LOCATIONS_H

#include <vector>
#include "common/types.h"
#include <random>
#include <string>

// Las Vegas street grid
inline const std::vector<std::string> STREET_NAMES = {
  "S Las Vegas Blvd", "W Charleston Blvd", "E Flamingo Rd",
  "S Maryland Pkwy", "W Sahara Ave", "E Tropicana Ave",
  "S Decatur Blvd", "W Spring Mountain Rd", "E Desert Inn Rd",
  "S Eastern Ave", "W Warm Springs Rd", "E Sunset Rd",
  "S Rainbow Blvd", "W Cheyenne Ave", "E Bonanza Rd",
  "S Jones Blvd", "W Lake Mead Blvd", "E Carey Ave",
  "S Nellis Blvd", "W Craig Rd", "E Stewart Ave",
  "S Pecos Rd", "W Ann Rd", "E Owens Ave"
};

inline std::vector<Location> generateDestinations(int count) {
  std::mt19937 gen(42);
  std::uniform_real_distribution<double> latDist(36.05, 36.28);
  std::uniform_real_distribution<double> lngDist(-115.35, -115.05);
  std::uniform_int_distribution<int> streetDist(0, STREET_NAMES.size() - 1);
  std::uniform_int_distribution<int> numDist(100, 9999);

  std::vector<Location> destinations;
  destinations.reserve(count);

  for (int i = 0; i < count; i++) {
    double lat = latDist(gen);
    double lng = lngDist(gen);
    std::string addr = std::to_string(numDist(gen)) + " " + STREET_NAMES[streetDist(gen)] + ", Las Vegas, NV";
    destinations.push_back({lat, lng, addr});
  }

  return destinations;
}

inline const std::vector<Location> BASES = {
  {36.1147, -115.1728, "South Strip Fulfillment Center"},
  {36.1862, -115.1373, "North Las Vegas Warehouse"},
  {36.1500, -115.2500, "West Side Distribution Hub"}
};

inline const std::vector<Location> DESTINATIONS = generateDestinations(1000);

#endif


// For testing
// inline const std::vector<Location> BASES = {
//   {36.1147, -115.1728, "South Strip Fulfillment Center"},
//   {36.1862, -115.1373, "North Las Vegas Warehouse"},
//   {36.0960, -115.1670, "Airport Adjacent Hub"}
// };

// inline const std::vector<Location> DESTINATIONS = {
//   {36.1716, -115.1469, "Fremont Street, Las Vegas, NV"},
//   {36.1126, -115.1767, "Allegiant Stadium, Las Vegas, NV"},
//   {36.1024, -115.1745, "T-Mobile Arena, Las Vegas, NV"},
//   {36.1215, -115.1739, "Bellagio, Las Vegas, NV"},
//   {36.1362, -115.1634, "Las Vegas Convention Center, Las Vegas, NV"},
//   {36.1475, -115.1566, "University of Nevada, Las Vegas, NV"},
//   {36.1256, -115.3157, "Red Rock Area, Las Vegas, NV"},
//   {36.2008, -115.2806, "Summerlin Area, Las Vegas, NV"},
//   {36.0800, -115.1522, "South Las Vegas Residential Dropoff"},
//   {36.1902, -115.0621, "Northeast Las Vegas Delivery Zone"}
// };