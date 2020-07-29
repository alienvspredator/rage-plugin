project(external C CXX)

set(BUILD_TESTING_BCKP ${BUILD_TESTING})
set(BUILD_TESTING OFF CACHE BOOL "Force disable of tests for external dependencies" FORCE)

# Replaced in CTestCustom.cmake.in
set(CUSTOM_COVERAGE_EXCLUDE ${CUSTOM_COVERAGE_EXCLUDE} "external" PARENT_SCOPE)

# Use spdlog for logging
add_subdirectory(spdlog EXCLUDE_FROM_ALL)

# JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json EXCLUDE_FROM_ALL)

# Doctest for unit tests
add_library(doctest INTERFACE)
target_include_directories(
	doctest
	INTERFACE
	# note : will expose the parts/ folder...
	doctest/doctest
)
target_compile_features(doctest INTERFACE cxx_std_11)

# Rage SDK
add_subdirectory(rage EXCLUDE_FROM_ALL)

# Set it back to its past value
set(BUILD_TESTING ${BUILD_TESTING_BCKP} CACHE BOOL "Build tests (default variable for CTest)" FORCE)
