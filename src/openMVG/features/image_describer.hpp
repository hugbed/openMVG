
// Copyright (c) 2015 Pierre MOULON.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OPENMVG_FEATURES_IMAGE_DESCRIBER_HPP
#define OPENMVG_FEATURES_IMAGE_DESCRIBER_HPP

#include "openMVG/features/regions.hpp"
#include "openMVG/numeric/numeric.h"

#include <cereal/cereal.hpp> // Serialization

#include <memory>

namespace openMVG {

namespace image {
  template<typename Type>
  class Image;
}

namespace features {

enum EDESCRIBER_PRESET
{
  NORMAL_PRESET,
  HIGH_PRESET,
  ULTRA_PRESET
};
/// A pure virtual class for image description computation
class Image_describer
{
public:
  Image_describer() = default ;
  virtual ~Image_describer() = default ;

  /**
  @brief Use a preset to control the number of detected regions
  @param preset The preset configuration
  @return True if configuration succeed.
  */
  virtual bool Set_configuration_preset
  (
    EDESCRIBER_PRESET preset
  ) = 0;

  /**
  @brief Detect regions on the image and compute their attributes (description)
  @param image Image.
  @param regions The detected regions and attributes
  @param mask 8-bit gray image for keypoint filtering (optional).
     Non-zero values depict the region of interest.
  */
  bool Describe
  (
    const image::Image<unsigned char> & image,
    std::unique_ptr<Regions> &regions,
    const image::Image<unsigned char> * mask = nullptr
  )
  {
    regions.reset(DescribeImpl(image, mask));
    return regions != nullptr;
  }

  /**
  @brief Detect regions on the image and compute their attributes (description)
  @param image Image.
  @param mask 8-bit gray image for keypoint filtering (optional).
     Non-zero values depict the region of interest.
  @return The detected regions and attributes
  */
  std::unique_ptr<Regions> Describe(
    const image::Image<unsigned char> & image,
    const image::Image<unsigned char> * mask = nullptr
  )
  {
    return std::unique_ptr<Regions>(DescribeImpl(image, mask));
  }

  /// Allocate regions depending of the Image_describer
  std::unique_ptr<Regions> Allocate()
  {
    return std::unique_ptr<Regions>(AllocateImpl());
  }

  //--
  // IO - one file for region features, one file for region descriptors
  //--

  virtual bool Load
  (
    Regions * regions,
    const std::string& sfileNameFeats,
    const std::string& sfileNameDescs
  ) const
  {
    return regions->Load(sfileNameFeats, sfileNameDescs);
  }

  virtual bool Save
  (
    const Regions * regions,
    const std::string& sfileNameFeats,
    const std::string& sfileNameDescs
  ) const
  {
    return regions->Save(sfileNameFeats, sfileNameDescs);
  };

  virtual bool LoadFeatures
  (
    Regions * regions,
    const std::string& sfileNameFeats
  ) const
  {
    return regions->LoadFeatures(sfileNameFeats);
  }

protected:
  // Return raw pointers to take advantage of covariant return types
  // Pointer can then be wrapped in smart pointer to derived or base class
  // public non-virtual interface provides this wrapping
  virtual Regions* AllocateImpl() const = 0;

  virtual Regions* DescribeImpl(
    const image::Image<unsigned char> & image,
    const image::Image<unsigned char> * mask = nullptr
  ) = 0;
};

} // namespace features
} // namespace openMVG

#endif // OPENMVG_FEATURES_IMAGE_DESCRIBER_HPP
