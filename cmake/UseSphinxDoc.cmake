# https://github.com/dev-cafe/cmake-cookbook/blob/v1.0/chapter-12/recipe-02/cxx-example/cmake/UseSphinxDoc.cmake
find_package(PythonInterp REQUIRED)
find_package(Sphinx REQUIRED)

function(add_sphinx_doc
        SPHINX_SOURCE_DIR
        SPHINX_BUILD_DIR
        SPHINX_CACHE_DIR
        SPHINX_HTML_DIR
        SPHINX_CONF_FILE
        SPHINX_TARGET_NAME
        SPHINX_COMMENT)

    MESSAGE(STATUE ${SPHINX_CONF_FILE})
    configure_file(
            ${SPHINX_CONF_FILE}
            ${SPHINX_BUILD_DIR}/conf.py
    )

    add_custom_target(${SPHINX_TARGET_NAME}
            COMMAND
            ${SPHINX_EXECUTABLE}
            -q
            -b html
            -c ${SPHINX_BUILD_DIR}
            -d ${SPHINX_CACHE_DIR}
            ${SPHINX_SOURCE_DIR}
            ${SPHINX_HTML_DIR}
            COMMENT
            "Building ${SPHINX_COMMENT} with Sphinx"
            VERBATIM
            )

    message(STATUS "Added ${SPHINX_TARGET_NAME} [Sphinx] target to build documentation")
endfunction()
