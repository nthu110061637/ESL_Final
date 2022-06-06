file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(platform-rf1mc-acc
ethernet.cpp
display.cpp
${HEADERS})

target_include_directories(platform-rf1mc-acc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(riscv-vp-rf1mc-acc
        main.cpp)

target_link_libraries(riscv-vp-rf1mc-acc rv32 platform-rf1mc-acc platform-common gdb-mc ${Boost_LIBRARIES} ${SystemC_LIBRARIES} pthread)

INSTALL(TARGETS riscv-vp RUNTIME DESTINATION bin)
