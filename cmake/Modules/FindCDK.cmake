# This module tries to find the Curses Development Kit 
# Based on the LIBCDK_PATH variable
#
# The module defines the following variables if successfull:
#   LIBCDK_LIBRARIES - path to the cdk libraries

#=============================================================================
# Copyright 2012 Minze Zwerver / van Ovost Automatisering BV
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

IF(NOT LIBCDK_PATH) 
SET (LIBCDK_PATH CACHE PATH 
"What is the basepath for the CDK ?" 
FORCE)
ENDIF() 

#IF(LIBCDK_PATH)
	FIND_PATH(LIBCDK_INCLUDE_DIR cdk/cdk.h
		HINTS
		${LIBCDK_PATH}/include
	)

	FIND_LIBRARY(LIBCDK_LIBRARIES_RELEASE NAMES cdk
		HINTS 
		${LIBCDK_PATH}
		PATH_SUFFIXES lib 
		)
		
	FIND_LIBRARY(LIBCDK_LIBRARIES_DEBUG NAMES cdk
		HINTS 
		${LIBCDK_PATH}
		PATH_SUFFIXES lib 
		)
		
		IF (LIBCDK_LIBRARIES_RELEASE AND LIBCDK_LIBRARIES_DEBUG)
			IF (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE) 
				SET(LIBCDK_LIBRARIES optimized ${LIBCDK_LIBRARIES_RELEASE} 
				debug ${LIBCDK_LIBRARIES_DEBUG})
			ELSE()
				SET(LIBCDK_LIBRARIES ${LIBCDK_LIBRARIES_RELEASE} )
			ENDIF()
		ENDIF()
		
		IF (LIBCDK_LIBRARIES_RELEASE AND NOT LIBCDK_LIBRARIES_DEBUG)
			SET(LIBCDK_LIBRARIES ${LIBCDK_LIBRARIES_RELEASE} )
		ENDIF()
		
		IF (NOT LIBCDK_LIBRARIES_RELEASE AND LIBCDK_LIBRARIES_DEBUG)
			SET(LIBCDK_LIBRARIES ${LIBCDK_LIBRARIES_DEBUG} )
		ENDIF()
		IF(LIBCDK_LIBRARIES)
			SET(LIBCDK_LIBRARIES  "${LIBCDK_LIBRARIES}" CACHE FILEPATH "CursesDevelopmentKit libraries" )
		ENDIF()
#ENDIF()


