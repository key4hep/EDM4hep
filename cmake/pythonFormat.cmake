# Additional target to run python linters and formatters on python files
#
# Requires ruff to be available in the environment

# Get all our Python files
file(GLOB_RECURSE ALL_PYTHON_FILES ${PROJECT_SOURCE_DIR}/python/*.py
  ${PROJECT_SOURCE_DIR}/scripts/*.py ${PROJECT_SOURCE_DIR}/test/*.py)

find_program(RUFF_EXECUTABLE ruff)
if(RUFF_EXECUTABLE)
    add_custom_target(
        ruff
        COMMAND ${RUFF_EXECUTABLE}
        check --force-exclude ${ALL_PYTHON_FILES}
    )
    set_target_properties(ruff PROPERTIES EXCLUDE_FROM_ALL TRUE)
    add_custom_target(
        ruff-format
        COMMAND ${RUFF_EXECUTABLE}
        format --force-exclude ${ALL_PYTHON_FILES}
    )
    set_target_properties(ruff-format PROPERTIES EXCLUDE_FROM_ALL TRUE)
else()
    message(STATUS "Failed to find ruff executable - no target to run ruff can be set")
endif()
