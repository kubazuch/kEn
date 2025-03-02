#include <GLFW/glfw3.h>
#include <commdlg.h>

#include <kEn/util/platform_utils.hpp>
#include <kenpch.hpp>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <kEn/core/application.hpp>

#define IMAGE_FILE_TYPES(X)              \
  X("JPEG", "*.jpg;*.jpeg;*.jpe;*.jfif") \
  X("PNG", "*.png")                      \
  X("TGA", "*.tga;*.tpic")               \
  X("Bit maps", "*.bmp")                 \
  X("PSD", "*.psd")                      \
  X("GIF", "*.gif")                      \
  X("PIC", "*.pic")                      \
  X("PNM", "*.ppm;*.pgm")

namespace kEn {

std::string file_dialog::open_file(const char* filter, int filter_index) {
  OPENFILENAMEA ofn;
  CHAR szFile[260]     = {0};
  CHAR currentDir[256] = {0};
  ZeroMemory(&ofn, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner   = glfwGetWin32Window((GLFWwindow*)application::instance().main_window().native_window());
  ofn.lpstrFile   = szFile;
  ofn.nMaxFile    = sizeof(szFile);
  if (GetCurrentDirectoryA(256, currentDir)) ofn.lpstrInitialDir = currentDir;
  ofn.lpstrFilter  = filter;
  ofn.nFilterIndex = filter_index;
  ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

  if (GetOpenFileNameA(&ofn) == TRUE) return ofn.lpstrFile;

  return std::string();
}

std::string file_dialog::open_image_file() {
#define INDIVIDUAL(NAME, EXT) NAME " (" EXT ")\0" EXT "\0"
#define COMBINED(NAME, EXT) EXT ";"
#define COUNT(NAME, EXT) +1

  static constexpr std::string_view filter =
      IMAGE_FILE_TYPES(INDIVIDUAL) "All image files\0" IMAGE_FILE_TYPES(COMBINED) "\0All files\0*.*\0";
  return open_file(filter.data(), IMAGE_FILE_TYPES(COUNT) + 1);
}

std::string file_dialog::save_file(const char* filter, int filter_index) {
  OPENFILENAMEA ofn;
  CHAR szFile[260]     = {0};
  CHAR currentDir[256] = {0};
  ZeroMemory(&ofn, sizeof(OPENFILENAME));
  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.hwndOwner   = glfwGetWin32Window((GLFWwindow*)application::instance().main_window().native_window());
  ofn.lpstrFile   = szFile;
  ofn.nMaxFile    = sizeof(szFile);
  if (GetCurrentDirectoryA(256, currentDir)) ofn.lpstrInitialDir = currentDir;
  ofn.lpstrFilter  = filter;
  ofn.nFilterIndex = filter_index;
  ofn.Flags        = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

  ofn.lpstrDefExt = strchr(filter, '\0') + 1;

  if (GetSaveFileNameA(&ofn) == TRUE) return ofn.lpstrFile;

  return std::string();
}

}  // namespace kEn
