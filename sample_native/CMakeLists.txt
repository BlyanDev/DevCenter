cmake_minimum_required(VERSION 2.8)

PROJECT(SampleNative)

ADD_EXECUTABLE(sample_native
			main.linux.cpp
			std_io.cpp)
			
TARGET_LINK_LIBRARIES(sample_native
			sample)

#FILE(COPY ${PROJECT_SOURCE_DIR}/../demo/display
#	DESTINATION ${EXECUTABLE_OUTPUT_PATH}/)