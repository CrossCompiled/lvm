include(ExternalProject)

option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)
option(BUILD_GMOCK "Build GMock (GTest will build also)" ON)
option(BUILD_GTEST "Build GTest" OFF)
option(gtest_force_shared_crt "Use shared (DLL) run-time lib even when Google Test is built as static lib." OFF)
option(gtest_disable_pthreads "Disable uses of pthreads in gtest." ON)

mark_as_advanced(BUILD_GMOCK)
mark_as_advanced(BUILD_GTEST)
mark_as_advanced(gtest_force_shared_crt)

ExternalProject_Add(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.8.0
        CMAKE_ARGS 
                -DBUILD_GMOCK=${BUILD_GMOCK}
                -DBUILD_GTEST=${BUILD_GTEST}
                -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}
                -Dgtest_force_shared_crt=${gtest_force_shared_crt}
				-Dgtest_disable_pthreads=${gtest_disable_pthreads}
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON
)

if (${BUILD_SHARED_LIBS})
	set(LIBRARY_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
	set(LIBRARY_PREFIX ${CMAKE_SHARED_LIBRARY_PREFIX})
else()
	set(LIBRARY_SUFFIX ${CMAKE_STATIC_LIBRARY_SUFFIX})	
	set(LIBRARY_PREFIX ${CMAKE_STATIC_LIBRARY_PREFIX})
endif()

if (MSVC)
	set(SUBDIR ${CMAKE_BUILD_TYPE})
else()
	set(SUBDIR "")
endif()

ExternalProject_Get_Property(googletest source_dir binary_dir)

set(GTEST_FOUND             TRUE)
set(GTEST_LIBRARY           GTest::GTest)
set(GTEST_MAIN_LIBRARY      GTest::Main)
set(GMOCK_LIBRARY           GMock::GMock)
set(GMOCK_MAIN_LIBRARY      GMock::Main)

set(GTEST_INCLUDE_PATH      ${source_dir}/googletest/include)
set(GTEST_LIBRARY_PATH      ${binary_dir}/googlemock/gtest/${SUBDIR}/${LIBRARY_PREFIX}gtest${LIBRARY_SUFFIX})
set(GTEST_MAIN_LIBRARY_PATH ${binary_dir}/googlemock/gtest/${SUBDIR}/${LIBRARY_PREFIX}gtest_main${LIBRARY_SUFFIX})

set(GMOCK_INCLUDE_PATH      ${source_dir}/googlemock/include)
set(GMOCK_LIBRARY_PATH      ${binary_dir}/googlemock/${SUBDIR}/${LIBRARY_PREFIX}gmock${LIBRARY_SUFFIX})
set(GMOCK_MAIN_LIBRARY_PATH ${binary_dir}/googlemock/${SUBDIR}/${LIBRARY_PREFIX}gmock_main${LIBRARY_SUFFIX})


if (NOT EXISTS ${GTEST_INCLUDE_PATH})
	file(MAKE_DIRECTORY ${GTEST_INCLUDE_PATH})
endif()

if (NOT EXISTS ${GMOCK_INCLUDE_PATH})
	file(MAKE_DIRECTORY ${GMOCK_INCLUDE_PATH})
endif()

include(CMakeFindDependencyMacro)
find_dependency(Threads)

if(NOT TARGET ${GTEST_LIBRARY})
	add_library(${GTEST_LIBRARY} UNKNOWN IMPORTED)
	set_target_properties(${GTEST_LIBRARY} PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES     "${GTEST_INCLUDE_PATH}"
		IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
		IMPORTED_LOCATION                 "${GTEST_LIBRARY_PATH}"
	)
	if(NOT gtest_disable_pthreads)
		set_target_properties(${GTEST_LIBRARY} PROPERTIES
			INTERFACE_LINK_LIBRARIES          "Threads::Threads"
		)
	endif()
	add_dependencies(${GTEST_LIBRARY} googletest)

endif()

if(NOT TARGET ${GTEST_MAIN_LIBRARY})
	add_library(${GTEST_MAIN_LIBRARY} UNKNOWN IMPORTED)
	set_target_properties(${GTEST_MAIN_LIBRARY} PROPERTIES
		INTERFACE_LINK_LIBRARIES          "${GTEST_LIBRARY}"
		IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
		IMPORTED_LOCATION                 "${GTEST_MAIN_LIBRARY_PATH}"
	)
	add_dependencies(${GTEST_MAIN_LIBRARY} googletest)
endif()

if(NOT TARGET ${GMOCK_LIBRARY})
	add_library(${GMOCK_LIBRARY} UNKNOWN IMPORTED)
	set_target_properties(${GMOCK_LIBRARY} PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES     "${GMOCK_INCLUDE_PATH}"
			INTERFACE_LINK_LIBRARIES          "${GTEST_LIBRARY}"
			IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
			IMPORTED_LOCATION                 "${GMOCK_LIBRARY_PATH}"
			)
	add_dependencies(${GMOCK_LIBRARY} googletest)
endif()

if(NOT TARGET ${GMOCK_MAIN_LIBRARY})
	add_library(${GMOCK_MAIN_LIBRARY} UNKNOWN IMPORTED)
	set_target_properties(${GMOCK_MAIN_LIBRARY} PROPERTIES
			INTERFACE_LINK_LIBRARIES          "${GMOCK_LIBRARY}"
			IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
			IMPORTED_LOCATION                 "${GMOCK_MAIN_LIBRARY_PATH}"
			)
	add_dependencies(${GMOCK_MAIN_LIBRARY} googletest)
endif()


