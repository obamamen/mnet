set(SOURCES
        src/mnet.c
)

set(HEADERS
        include/mnet.h
)

add_library(mnet ${SOURCES})
target_include_directories(mnet PUBLIC include)

if(WIN32)
    target_link_libraries(mnet PRIVATE ws2_32)
elseif(UNIX)
    target_link_libraries(mnet PRIVATE pthread)
endif()