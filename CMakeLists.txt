cmake_minimum_required(VERSION 3.13)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

project(
	CCDPlanet
	VERSION 0.1.0
	LANGUAGES CXX
)

# Modules and scripts

# Standard CMake modules
include(CTest)
include(CMakeDependentOption)
include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)
include(CMakePackageConfigHelpers)
include(ExternalProject)

# Custom modules and scripts

# Make our cmake scripts available
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(LTO)
include(Warnings)
include(CopyDllsForDebug)
include(Coverage)

# Options

# You should try to give as much control over the project setup to the user.
# When modifying compile flags for example, if they are not mandatory, provide
# an option.
option(CCD_USE_DOXYGEN "Add a doxygen target to generate the documentation" ON)
option(CCD_USE_ADDITIONAL_SOURCEFILE "Use the additional source file" ON)

# Use your own option for tests, in case people use your library through
# add_subdirectory
cmake_dependent_option(CCD_BUILD_TESTS
	# By default we want tests if CTest is enabled
	"Enable ${PROJECT_NAME} project tests targets" ON
	# Stay coherent with CTest variables
	"BUILD_TESTING" OFF
)

# External dependencies
add_subdirectory(external EXCLUDE_FROM_ALL)

# It is always easier to navigate in an IDE when projects are organized in
# folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# When building a shared library, you do not want to export all symbols by
# default gcc (and hence clang) are wrong about this.
#
# For more information, see https://gcc.gnu.org/wiki/Visibility and
# https://www.youtube.com/embed/m0DwB4OvDXk
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

# Project

# Check for LTO support (needs to be after project(...))
find_lto(CXX)

# CCD Library

file(GLOB libccd_include ${PROJECT_SOURCE_DIR}/include/ccd/*.h)
file(GLOB libccd_source ${PROJECT_SOURCE_DIR}/source/ccd/*.h ${PROJECT_SOURCE_DIR}/source/ccd/*.cpp)

# We use both the include and source folders for headers, based on wether they
# are part of the public API or not. For more information about this rationale,
# see https://github.com/vector-of-bool/pitchfork
add_library(libccd
	SHARED
	${libccd_include}
	${libccd_source}
	# source/ccd/api.cpp
	# include/ccd/api.h
	# source/ccd/console.cpp
	# include/ccd/console.h
	# source/ccd/entity.cpp
	# include/ccd/entity.h
	# source/ccd/player.cpp
	# include/ccd/player.h
	# source/ccd/plugin.cpp
	# include/ccd/plugin.h
	# source/ccd/pluginAttachment.cpp
	# include/ccd/pluginAttachment.h
	# source/ccd/resource.cpp
	# include/ccd/resource.h
	# source/ccd/vector.cpp
	# include/ccd/vector.h
	# # source/ccd/callable.cpp
	# include/ccd/callable.h
	# # source/ccd/vehicle.cpp
	# include/ccd/vehicle.h
	# # source/ccd/exception.cpp
	# include/ccd/exception.h
)

# target_set_warnings(libccd
# 	DISABLE ALL
# )

set_target_properties(libccd
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES
		CXX_EXTENSIONS NO
)

# Since we put the public interface headers in the include directory, we need to
# tell the compiler so that we can #include <file>.
target_include_directories(libccd
	# The folder must be used in the include path for any target using this
	# library
	PUBLIC
	# Due to the way installation work, we only want this path set when
	# building, not once installed
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

find_package(Microsoft.GSL CONFIG)

# We tell CMake what are the target dependencies
target_link_libraries(libccd
	PRIVATE
		# Use the namespaced version to make sure we have the target and not the
		# static lib only (which doesn't have transitive properties)
		spdlog::spdlog
		nlohmann_json::nlohmann_json
		rage::rage
		Microsoft.GSL::GSL
)
# Give a 'namespaced' name to libraries targets, as it can't be mistaken with
# system libraries
add_library(ccd::CCD ALIAS libccd)

# Doxygen

if(CCD_USE_DOXYGEN AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.9)
	find_package(Doxygen
		OPTIONAL_COMPONENTS dot mscgen dia
	)
	if(DOXYGEN_FOUND)
		set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
		doxygen_add_docs(
			doc
			README.md source
			COMMENT "Generate man pages"
		)
	endif()
endif()


# Packaging

# Let users choose where to install the cmake package descriptions For that we
# make use of the CMake Cache
set(${PROJECT_NAME}_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE STRING "Path to install ${PROJECT_NAME} Config*.cmake files to.")
set(${PROJECT_NAME}_MODULE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake"               CACHE STRING "Path to install ${PROJECT_NAME}'s .cmake  module files to.")

# Use version checking helper provided by CMake so that users can safely use a
# version number in their find_package calls
write_basic_package_version_file(
	# The name of the version file needed by find_package.
	${PROJECT_NAME}ConfigVersion.cmake
	# The version of the project, already set by the `project` command at the
	# top of this file
	VERSION ${PROJECT_VERSION}
	# We use semantic versioning, backward compatibity is only guaranteed for a
	# same major version
	COMPATIBILITY SameMajorVersion
)

# We will need our own file if we have our own dependencies or want some special
# behavior when the user calls find_package otherwise we could simply install
# the exports as the ${PROJECT_NAME}Config.cmake
configure_package_config_file(
	# This is your template file
	${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
	# This is the resulting file
	${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
	# This is where the file will be installed
	INSTALL_DESTINATION ${${PROJECT_NAME}_INSTALL_CMAKEDIR}
	# List of paths that needs to be relocated once installed For example if the
	# variable containing the path is named MY_PATH, all instances of
	# @PACKAGE_MY_PATH@ in the template will be replaced by the relocated
	# version of the path This is mostly only needed when you want to install
	# cmake modules or have an unusual layout that cmake is not aware of.
	
	# This will be exposed as @PACKAGE_BoilerPlate_MODULE_INSTALL_DIR@ in the
	# template file
	PATH_VARS ${PROJECT_NAME}_MODULE_INSTALL_DIR
	# Imported targets do not require the following macros
	NO_SET_AND_CHECK_MACRO
	NO_CHECK_REQUIRED_COMPONENTS_MACRO 
)

# So far we only installed the exported targets, now install the package config
# files. If you do not list headers in the PUBLIC_HEADER property, you will need
# to copy them using `install(FILES)` or `install(DIRECTORY)` too. In that case,
# you can use CMAKE_INSTALL_INCLUDEDIR as the base destination path.
install(FILES 
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake 
	DESTINATION
		${${PROJECT_NAME}_INSTALL_CMAKEDIR}
)
