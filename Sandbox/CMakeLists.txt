include(../cmake/configure_binary.cmake)

configure_binary(NAME sandbox
    DEPS_PUBLIC mEn kEn
)

add_custom_command(TARGET sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini"
        "$<TARGET_FILE_DIR:sandbox>/imgui.ini"
    COMMENT "Copying imgui.ini to output directory"
)
