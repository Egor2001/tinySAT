cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

find_program(CTAGS ctags)

if(CTAGS)
    set(TAGS_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

    add_custom_target(tinySAT-tags)
    add_custom_target(dpll-tags)

    set(TAGS ${TAGS_PREFIX}/tinySAT.tags)
    add_custom_command(TARGET tinySAT-tags
        PRE_BUILD
        COMMAND ${CTAGS}
        ARGS -Rf ${TAGS}
        BYPRODUCTS ${TAGS}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/include
        )

    set(TAGS ${TAGS_PREFIX}/dpll.tags)
    add_custom_command(TARGET dpll-tags
        PRE_BUILD
        COMMAND ${CTAGS}
        ARGS -Rf ${TAGS}
        BYPRODUCTS ${TAGS}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dpll/include
        )
else()
    message("error: can't find CTags program")
endif()
