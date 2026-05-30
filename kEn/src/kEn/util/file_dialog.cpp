#include "file_dialog.hpp"

#include <nfd/nfd.h>

#include <expected>  // NOLINT(misc-include-cleaner)
#include <filesystem>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <utility>
#include <vector>

// NOLINTBEGIN(bugprone-suspicious-stringview-data-usage)

namespace kEn {

namespace {

struct DialogPathDeleter {
  void operator()(nfdu8char_t* path) const noexcept {
    if (path) {
      NFD_FreePathU8(path);
    }
  }
};

struct PathSetDeleter {
  void operator()(const nfdpathset_t* paths) const noexcept {
    if (paths) {
      NFD_PathSet_Free(paths);
    }
  }
};

struct PathSetPathDeleter {
  void operator()(nfdu8char_t* path) const noexcept {
    if (path) {
      NFD_PathSet_FreePathU8(path);
    }
  }
};

using dialog_path_ptr   = std::unique_ptr<nfdu8char_t, DialogPathDeleter>;
using path_set_ptr      = std::unique_ptr<const nfdpathset_t, PathSetDeleter>;
using path_set_path_ptr = std::unique_ptr<nfdu8char_t, PathSetPathDeleter>;

struct PathSetEnumGuard {
  nfdpathsetenum_t value{};
  bool active{false};

  ~PathSetEnumGuard() noexcept {
    if (active) {
      NFD_PathSet_FreeEnum(&value);
    }
  }

  PathSetEnumGuard() = default;

  PathSetEnumGuard(const PathSetEnumGuard&)            = delete;
  PathSetEnumGuard& operator=(const PathSetEnumGuard&) = delete;
};

FileDialogError current_error() {
  const char* msg = NFD_GetError();
  return {msg ? msg : "unknown nativefiledialog-extended error"};
}

std::vector<nfdu8filteritem_t> make_filter_items(std::span<const FileFilter> filters) {
  std::vector<nfdu8filteritem_t> items;
  items.reserve(filters.size());

  for (const auto& item : filters) {
    items.push_back(nfdu8filteritem_t{
        .name = item.name.data(),
        .spec = item.spec.data(),
    });
  }

  return items;
}

std::string path_to_utf8_string(const std::filesystem::path& path) {
  const auto u8 = path.u8string();
  return {reinterpret_cast<const char*>(u8.data()), u8.size()};
}

std::optional<std::string> optional_path_to_utf8_string(const std::filesystem::path& path) {
  if (path.empty()) {
    return std::nullopt;
  }

  return path_to_utf8_string(path);
}

const char* nullable_c_str(const std::optional<std::string>& value) noexcept {
  if (!value || value->empty()) {
    return nullptr;
  }

  return value->c_str();
}

const char* nullable_c_str(const std::string& value) noexcept { return value.empty() ? nullptr : value.c_str(); }

std::filesystem::path path_from_utf8(const char* utf8) {
  return std::filesystem::path{reinterpret_cast<const char8_t*>(utf8)};
}

FileDialogResult<std::filesystem::path> single_path_result(nfdresult_t r, nfdu8char_t* raw) {
  const dialog_path_ptr path{raw};

  if (r == NFD_CANCEL) {
    return std::optional<std::filesystem::path>{};
  }

  if (r == NFD_ERROR) {
    return std::unexpected(current_error());
  }

  if (!path) {
    return std::unexpected(FileDialogError{
        "nativefiledialog-extended returned success with a null path",
    });
  }

  return std::optional<std::filesystem::path>{
      path_from_utf8(path.get()),
  };
}

FileDialogResult<std::vector<std::filesystem::path>> path_set_result(nfdresult_t r, const nfdpathset_t* raw) {
  const path_set_ptr paths{raw};

  if (r == NFD_CANCEL) {
    return std::optional<std::vector<std::filesystem::path>>{};
  }

  if (r == NFD_ERROR) {
    return std::unexpected(current_error());
  }

  if (!paths) {
    return std::unexpected(FileDialogError{
        "nativefiledialog-extended returned success with a null path set",
    });
  }

  PathSetEnumGuard enumerator;

  if (NFD_PathSet_GetEnum(paths.get(), &enumerator.value) != NFD_OKAY) {
    return std::unexpected(current_error());
  }

  enumerator.active = true;
  std::vector<std::filesystem::path> out;
  while (true) {
    nfdu8char_t* raw_path = nullptr;

    if (NFD_PathSet_EnumNextU8(&enumerator.value, &raw_path) != NFD_OKAY) {
      return std::unexpected(current_error());
    }

    if (!raw_path) {
      break;
    }

    const path_set_path_ptr path{raw_path};
    out.push_back(path_from_utf8(path.get()));
  }

  return std::optional<std::vector<std::filesystem::path>>{
      std::move(out),
  };
}

class Runtime {
 public:
  Runtime() {
    if (NFD_Init() != NFD_OKAY) {
      ok_    = false;
      error_ = current_error();
      return;
    }

    ok_ = true;
  }

  ~Runtime() noexcept {
    if (ok_) {
      NFD_Quit();
    }
  }

  Runtime(const Runtime&)            = delete;
  Runtime& operator=(const Runtime&) = delete;

  [[nodiscard]] std::expected<void, FileDialogError> ensure_ready() const {
    if (!ok_) {
      return std::unexpected(error_);
    }

    return {};
  }

 private:
  bool ok_{false};
  FileDialogError error_{};
};

std::expected<void, FileDialogError> ensure_initialized() {
  const static Runtime kInstance;
  return kInstance.ensure_ready();
}

}  // namespace

FileDialogResult<std::filesystem::path> open_file(const OpenOptions& options) {
  if (auto init = ensure_initialized(); !init) {
    return std::unexpected(init.error());
  }

  const auto filters      = make_filter_items(options.filters);
  const auto default_path = optional_path_to_utf8_string(options.default_path);

  nfdopendialogu8args_t args{};
  args.filterList  = filters.empty() ? nullptr : filters.data();
  args.filterCount = static_cast<nfdfiltersize_t>(filters.size());
  args.defaultPath = nullable_c_str(default_path);

  nfdu8char_t* raw_path    = nullptr;
  const auto native_result = NFD_OpenDialogU8_With(&raw_path, &args);

  return single_path_result(native_result, raw_path);
}

FileDialogResult<std::vector<std::filesystem::path>> open_files(const OpenOptions& options) {
  if (auto init = ensure_initialized(); !init) {
    return std::unexpected(init.error());
  }

  const auto filters      = make_filter_items(options.filters);
  const auto default_path = optional_path_to_utf8_string(options.default_path);

  nfdopendialogu8args_t args{};
  args.filterList  = filters.empty() ? nullptr : filters.data();
  args.filterCount = static_cast<nfdfiltersize_t>(filters.size());
  args.defaultPath = nullable_c_str(default_path);

  const nfdpathset_t* raw_paths = nullptr;
  const auto native_result      = NFD_OpenDialogMultipleU8_With(&raw_paths, &args);

  return path_set_result(native_result, raw_paths);
}

FileDialogResult<std::filesystem::path> save_file(const SaveOptions& options) {
  if (auto init = ensure_initialized(); !init) {
    return std::unexpected(init.error());
  }

  const auto filters      = make_filter_items(options.filters);
  const auto default_path = optional_path_to_utf8_string(options.default_path);

  nfdsavedialogu8args_t args{};
  args.filterList  = filters.empty() ? nullptr : filters.data();
  args.filterCount = static_cast<nfdfiltersize_t>(filters.size());
  args.defaultPath = nullable_c_str(default_path);
  args.defaultName = nullable_c_str(options.default_name);

  nfdu8char_t* raw_path    = nullptr;
  const auto native_result = NFD_SaveDialogU8_With(&raw_path, &args);

  return single_path_result(native_result, raw_path);
}

FileDialogResult<std::filesystem::path> select_folder(const std::filesystem::path& path) {
  if (auto init = ensure_initialized(); !init) {
    return std::unexpected(init.error());
  }

  const auto default_path = optional_path_to_utf8_string(path);

  nfdpickfolderu8args_t args{};
  args.defaultPath = nullable_c_str(default_path);

  nfdu8char_t* raw_path    = nullptr;
  const auto native_result = NFD_PickFolderU8_With(&raw_path, &args);

  return single_path_result(native_result, raw_path);
}

}  // namespace kEn

// NOLINTEND(bugprone-suspicious-stringview-data-usage)
