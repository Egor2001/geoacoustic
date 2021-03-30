if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.10)
    cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
    project(geoacoustic VERSION 0.1 LANGUAGES C CXX)
else()
    cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
    project(geoacoustic)
endif()

if(CMAKE_VERSION VERSION_LESS 3.10)
    message(AUTHOR_WARNING
        "You are using an old version of CMake."
        "Some options such as sanitizers support are unavailable."
        )
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

    if(${GEO_BENCH})
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
    endif()

    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.10)
        set(CMAKE_C_FLAGS_DEBUG 
            "${CMAKE_C_FLAGS_DEBUG}-fno-omit-frame-pointer -fsanitize=address")
        set(CMAKE_CXX_FLAGS_DEBUG 
            "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
        set(CMAKE_LINKER_FLAGS_DEBUG 
            "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer")
        set(CMAKE_LINKER_FLAGS_DEBUG 
            "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
    endif()

    add_compile_options(-mavx -Wall -Wextra -pedantic)

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -debug all")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

    if(${GEO_BENCH})
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
    endif()

    add_compile_options(-Wall -w3 
                        -Warray-bounds 
                        -Wchar-subscripts 
                        -Wcomment 
                        -Wenum-compare 
                        -Wformat 
                        -Wuninitialized 
                        -Wmaybe-uninitialized 
                        -Wmain -Wnarrowing 
                        -Wnonnull -Wparentheses 
                        -Wpointer-sign 
                        -Wreorder 
                        -Wreturn-type 
                        -Wsign-compare 
                        -Wsequence-point 
                        -Wtrigraphs 
                        -Wunused-function 
                        -Wunused-but-set-variable 
                        -Wunused-variable 
                        -Wwrite-strings
                        )

elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR 
       CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

    if(${GEO_BENCH})
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
    endif()

    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.10)
        set(CMAKE_C_FLAGS_DEBUG 
            "${CMAKE_C_FLAGS_DEBUG}-fno-omit-frame-pointer -fsanitize=address")
        set(CMAKE_CXX_FLAGS_DEBUG 
            "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
        set(CMAKE_LINKER_FLAGS_DEBUG 
            "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer")
        set(CMAKE_LINKER_FLAGS_DEBUG 
            "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
    endif()

    # TODO: to fix all disabled warnings
    add_compile_options(-mavx -Weverything -pedantic
                        -Wno-sign-conversion
                        -Wno-global-constructors
                        -Wno-exit-time-destructors
                        -Wno-c++98-compat-pedantic
                        -Wno-padded
                        -Wno-weak-vtables
                        -Wno-documentation
                        -Wno-unknown-pragmas
                        )

else()
    message(FATAL_ERROR 
            "${CMAKE_CXX_COMPILER_ID} is not currently supported."
            "Use on of these instead:"
            "GNU gcc"
            "[Intel icc] - CURRENTLY UNAVAILABLE TO USE, FIX IS IN PROCESS"
            "LLVM Clang"
            "Apple Clang"
            )

endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

include_directories(PUBLIC src/ src/static/)

add_subdirectory(src)

# TODO(@geome_try): 
# add_subdirectory(docs)
