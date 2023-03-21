# based on https://github.com/lefticus/cpp_starter_project/

# treat the value of the CXX_STANDARD target property as a requirement
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# generate compile_commands.json to make it easier to work with clang based tools
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# enable link time optimization
include( CheckIPOSupported )
check_ipo_supported(
	RESULT ipo_supported
	OUTPUT ipo_output
)
if( ipo_supported )
	set( CMAKE_INTERPROCEDURAL_OPTIMIZATION ON )
else( )
	message( WARNING "IPO is not supported: ${ipo_output}" )
endif( )

# colored compiler output
if( CMAKE_CXX_COMPILER_ID MATCHES ".*Clang" )
	add_compile_options( -fcolor-diagnostics )
elseif( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" )
	add_compile_options( -fdiagnostics-color=always )
else( )
	message( STATUS "No colored compiler diagnostic set for '${CMAKE_CXX_COMPILER_ID}' compiler." )
endif( )

