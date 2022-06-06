file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(platform-rf-acc2
ethernet.cpp
display.cpp
${HEADERS})

target_include_directories(platform-rf-acc2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(riscv-vp-rf-acc2
        main.cpp)

target_link_libraries(riscv-vp-rf-acc2 rv32 platform-rf-acc2 platform-common gdb-mc ${Boost_LIBRARIES} ${SystemC_LIBRARIES} pthread)

INSTALL(TARGETS riscv-vp RUNTIME DESTINATION bin)
