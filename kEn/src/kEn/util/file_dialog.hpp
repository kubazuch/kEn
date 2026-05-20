#pragma once

#include <expected>
#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <string_view>

#include <kEn/core/core.hpp>

namespace kEn {

struct FileDialogError {
  std::string message;
};

template <class T>
using FileDialogResult = std::expected<std::optional<T>, FileDialogError>;

struct FileFilter {
  std::string_view name;
  std::string_view spec;
};

struct OpenOptions {
  std::span<const FileFilter> filters;
  std::filesystem::path default_path = {};
};

struct SaveOptions {
  std::span<const FileFilter> filters;
  std::filesystem::path default_path = {};
  std::string default_name;
};

[[nodiscard]] FileDialogResult<std::filesystem::path> open_file(const OpenOptions& options = {});

[[nodiscard]] FileDialogResult<std::vector<std::filesystem::path>> open_files(const OpenOptions& options = {});

[[nodiscard]] FileDialogResult<std::filesystem::path> save_file(const SaveOptions& options = {});

[[nodiscard]] FileDialogResult<std::filesystem::path> select_folder(const std::filesystem::path& default_path = {});

}  // namespace kEn
