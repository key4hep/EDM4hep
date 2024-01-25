# Additional target to run python linters and formatters on python scripts
#
# Requires black/flake8 to be available in the environment


# Get all our Python files
file(GLOB_RECURSE ALL_PYTHON_FILES ${PROJECT_SOURCE_DIR}/*.py)

# Black is rather simple because there are no options...
find_program(BLACK_EXECUTABLE black)
if(BLACK_EXECUTABLE)
    add_custom_target(
            black
            COMMAND black
            ${ALL_PYTHON_FILES}
    )
    set_target_properties(black PROPERTIES EXCLUDE_FROM_ALL TRUE)
else()
    message(STATUS "Failed to find black executable - no target to run black can be set")
endif()

find_program(FLAKE8_EXECUTABLE flake8)
if(FLAKE8_EXECUTABLE)
    add_custom_target(
        flake8
        COMMAND flake8
        --config=${CMAKE_CURRENT_SOURCE_DIR}/.flake8
        ${ALL_PYTHON_FILES}
    )
    set_target_properties(flake8 PROPERTIES EXCLUDE_FROM_ALL TRUE)
else()
    message(STATUS "Failed to find flake8 executable - no target to run flake8 can be set")
endif()

