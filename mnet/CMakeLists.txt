set(SOURCES
    mnet.c
)

set(HEADERS
    mnet.h
)

add_library(mnet ${SOURCES})

target_include_directories(mnet PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    target_link_libraries(mnet PRIVATE ws2_32)
elseif(UNIX)
    target_link_libraries(mnet PRIVATE pthread)
endif()