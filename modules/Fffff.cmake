include(ExternalProject)

MESSAGE(STATUS "PREPE")
foreach(COMPONENT ${Boost_FIND_COMPONENTS})
MESSAGE(STATUS ${COMPONENT})
endforeach()
MESSAGE(STATUS "POSTE")

function(ADD_COMP name)

ExternalProject_Add(
        "boost${name}"
        GIT_REPOSITORY "https://github.com/boostorg/${name}.git"
        GIT_TAG boost-1.61.0
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON
)

ExternalProject_Get_Property("boost${name}" source_dir)


set(BOOST_${name}_FOUND             TRUE)
set(BOOST_${name}_LIBRARY           Boost::${name})
set(BOOST_${name}_INCLUDE_PATH      ${source_dir}/include)

if (NOT EXISTS ${BOOST_${name}_INCLUDE_PATH})
	file(MAKE_DIRECTORY ${BOOST_${name}_INCLUDE_PATH})
endif()

if(NOT TARGET ${BOOST_${name}_LIBRARY})
	add_library(${BOOST_${name}_LIBRARY} INTERFACE IMPORTED)
	set_target_properties(${BOOST_${name}_LIBRARY} PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${BOOST_${name}_INCLUDE_PATH}"
	)
	add_dependencies(${BOOST_${name}_LIBRARY} "boost${name}")
endif()
target_link_libraries(Boost::Boost ${BOOST_${name}_LIBRARY})
add_dependencies(Boost::Boost ${BOOST_${name}_LIBRARY})
endfunction()

ADD_COMP(config)
ADD_COMP(integer)
