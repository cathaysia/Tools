#include(FindDoxygenBinary)
#find_package(DoxygenBinary REQUIRED)
find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

function(ADD_BREATHE_DOC
        SOURCE_DIR # ${PROJECT_SOURCE_DIR}
        BUILD_DIR # doxygen 和 sphinx 的生成路径
        DOXY_FILE # Doxyfile.in
        CONF_FILE # conf.py.in
        TARGET_NAME # sphinx-build -b 的参数（一般为 html, latex, latexpdf）
        COMMENT
        )
    configure_file(
            ${DOXY_FILE}
            ${BUILD_DIR}/Doxyfile
    )
    configure_file(
            ${CONF_FILE}
            ${BUILD_DIR}/conf.py
    )
    add_custom_target(
            doc-${TARGET_NAME}
            COMMAND ${DOXYGEN_EXECUTABLE} ${BUILD_DIR}/Doxyfile
            COMMAND ${SPHINX_EXECUTABLE} -q -b ${TARGET_NAME} ${SOURCE_DIR}/doc ${BUILD_DIR}/html -c ${BUILD_DIR}
            COMMENT "构建文档目标： ${TARGET_NAME}"
    )
endfunction()
