### CMake Fragment: Include version number

### Version-number inclusion logic.

# Rationale: dspdfviewer --version should print something meaningful
# Especially on builds from git, it should include the git revision.

if( DSPDFVIEWER_VERSION )
  # Not-Empty version given on the command line. This has absolute priority.
  message(STATUS "Embedding the version number ${DSPDFVIEWER_VERSION} specified on the command line.")
endif()

if( NOT DSPDFVIEWER_VERSION AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git" )
	# We don't have a version number yet, but
	# this looks like a checkout from git.
	# Ask "git describe" for a version number.
	message(STATUS "Building from a git clone, using git describe for a version number.")
	execute_process(COMMAND git describe
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_DESCRIBE_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
	# Check if it gave something back
	if( NOT "${GIT_DESCRIBE_VERSION}" MATCHES "^$" )
		message(STATUS "Embedding version number ${GIT_DESCRIBE_VERSION} as defined by git-describe.")
		set(DSPDFVIEWER_VERSION ${GIT_DESCRIBE_VERSION})
	endif()
endif()

if( NOT DSPDFVIEWER_VERSION )
	# We still don't know version number to embed.
	# Use default

	# TODO: Keep me updated!
	set(DSPDFVIEWER_VERSION "1.14-unreleased")
	message(STATUS "Embedding version number ${DSPDFVIEWER_VERSION}. If you want to override this, "
		"for example to embed the git revision you built from, please pass "
		"-DDSPDFVIEWER_VERSION=1.2.3.4.5 to the cmake command.")
endif()

if( NOT "${DSPDFVIEWER_VERSION}" MATCHES "^$" )
  add_definitions(-DDSPDFVIEWER_VERSION="${DSPDFVIEWER_VERSION}")
endif()
