#pragma once

#include <string>

#include <kEn/core/core.hpp>

namespace kEn {

class FileDialog {
 public:
  static std::string open_file(const char* filter, int filter_index = 1);
  static std::string save_file(const char* filter, int filter_index = 1);
  static std::string open_image_file();
};

}  // namespace kEn