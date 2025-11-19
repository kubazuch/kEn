#pragma once

#include <string>

namespace kEn {  // NOLINT

class file_dialog {
 public:
  static std::string open_file(const char* filter, int filter_index = 1);
  static std::string save_file(const char* filter, int filter_index = 1);
  static std::string open_image_file();
};

}  // namespace kEn