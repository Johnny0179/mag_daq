# add sources here
set(src_list 
    ad7616_libiio.c
    )

set(elf_file ${project_name})

set(INC_DIR 
    src
    )

# add include
include_directories(${INC_DIR})

#add sources to executable
add_executable(${elf_file} ${src_list})

# link libiio library to project
# A full path to a library file. only in this way can work!
target_link_libraries(${elf_file} PUBLIC ${PROJECT_SOURCE_DIR}/lib/libiio/libiio.so)

# add pthread support
find_package(Threads)
target_link_libraries(${elf_file} PUBLIC ${CMAKE_THREAD_LIBS_INIT})