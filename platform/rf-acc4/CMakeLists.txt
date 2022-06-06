file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(platform-rf-acc4
ethernet.cpp
display.cpp
${HEADERS})

target_include_directories(platform-rf-acc4 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(riscv-vp-rf-acc4
        main.cpp)

target_link_libraries(riscv-vp-rf-acc4 rv32 platform-rf-acc4 platform-common gdb-mc ${Boost_LIBRARIES} ${SystemC_LIBRARIES} pthread)

INSTALL(TARGETS riscv-vp RUNTIME DESTINATION bin)
