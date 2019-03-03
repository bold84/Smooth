function(set_compile_options target)
    # for std::to_string (until using xtensa-gcc 8)
    target_compile_definitions(${target} PRIVATE -D_GLIBCXX_USE_C99)

    target_compile_options(${target} PRIVATE -Werror -Wall -Wextra)
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:CXX>: -Wnon-virtual-dtor>)
    target_compile_options(${target} PRIVATE $<$<COMPILE_LANGUAGE:CXX>: -Wuseless-cast>)
    target_compile_options(${target} PRIVATE -Wcast-align -Wunused )
#    target_compile_options(${target} PRIVATE -Wconversion -Wsign-conversion)
#    target_compile_options(${target} PRIVATE -Wlogical-op -Wdouble-promotion -Wformat=2)

    if( NOT ESP_PLATFORM )
        target_compile_options(${PROJECT_NAME} PRIVATE -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wnull-dereference)
    endif()
endfunction()