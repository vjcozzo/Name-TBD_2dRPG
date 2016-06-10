
add_executable(TBD main.cpp)


find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(TBD ${SFML_LIBRARIES})
endif()