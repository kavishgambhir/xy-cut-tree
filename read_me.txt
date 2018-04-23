1. xy_cut.cpp = segments a a document and creates a tree of blocks.
2 sample documents are provided to test on.
Result blocks are stored as binary in result folder and resultant segmented image stored as result.jpg in root folder.
requires library opencv 
following line to be added to make file=

add_executable(OpenCVTest ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( OpenCVTest ${OpenCV_LIBS} )
include_directories(/*opencv directory*/)
TARGET_LINK_LIBRARIES( BTP opencv_core opencv_imgproc opencv_video opencv_imgcodecs opencv_core
        opencv_highgui ${EXTERNAL_LIBS})


2. tree_reduce_sample.cpp = takes a sample tree with attributes described in paper and reduces it with the rules to print all possible trees formed .
to be compile with c++11.