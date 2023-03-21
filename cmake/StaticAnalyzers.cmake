# based on https://github.com/lefticus/cpp_starter_project/

if( ENABLE_CPPCHECK )
	find_program( CPPCHECK cppcheck )
	if( CPPCHECK )
		set( CMAKE_CXX_CPPCHECK
				${CPPCHECK}
				--suppress=missingInclude
				--enable=all
				--inline-suppr
				--inconclusive
			)
	else( )
		message( SEND_ERROR "cppcheck requested but executable not found")
	endif( )
endif( )

if( ENABLE_CLANG_TIDY )
	find_program( CLANGTIDY clang-tidy )
	if( CLANGTIDY )
		set( CMAKE_CXX_CLANG_TIDY
			${CLANGTIDY}
			-extra-arg=-Wno-unknown-warning-option
		)
	else( )
		message( SEND_ERROR "clang-tidy requested but executable not found")
	endif( )
endif( )

if( ENABLE_INCLUDE_WHAT_YOU_USE )
	find_program( INCLUDE_WHAT_YOU_USE include-what-you-use )
	if( INCLUDE_WHAT_YOU_USE )
		set( CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${INCLUDE_WHAT_YOU_USE})
	else( )
		message( SEND_ERROR "include-what-you-use requested but executable not found")
	endif( )
endif( )
