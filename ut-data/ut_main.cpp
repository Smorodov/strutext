
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE_1_0.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#include <catch2/catch_session.hpp>
#include <catch2/internal/catch_compiler_capabilities.hpp>
#include <catch2/internal/catch_leak_detector.hpp>
#include <catch2/internal/catch_platform.hpp>
#include <locale>
#ifdef _WIN32
#include <windows.h>
#endif
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h" // support for loading levels from the environment variable

namespace Catch {
CATCH_INTERNAL_START_WARNINGS_SUPPRESSION
CATCH_INTERNAL_SUPPRESS_GLOBALS_WARNINGS
LeakDetector leakDetector;
CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION
}  // namespace Catch

#if defined(CATCH_CONFIG_WCHAR) && defined(CATCH_PLATFORM_WINDOWS) && defined(_UNICODE) && !defined(DO_NOT_USE_WMAIN)
// Standard C/C++ Win32 Unicode wmain entry point
extern "C" int wmain(int argc, wchar_t* argv[], wchar_t*[]) {
#else
// Standard C/C++ main entry point
int main(int argc, char* argv[]) {
#endif

  setlocale(LC_ALL, "ru_RU.UTF8");
  #ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  #endif
  spdlog::info(u8"Версия логгера {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
  // We want to force the linker not to discard the global variable
  // && its constructor, as it (optionally) registers leak detector
  (void)&Catch::leakDetector;

  return Catch::Session().run(argc, argv);
}
