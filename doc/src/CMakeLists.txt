
SET(LOCAL_FILES
    index.rst
    pgRouting-concepts.rst
    pgRouting-installation.rst
    pgRouting-introduction.rst
    proposed.rst
    experimental.rst
    release_notes.rst
    routingFunctions.rst
    sampledata.rst
    support.rst
    migration.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

# For the release notes substitution of the links for issues and milestones
file(READ ${PGR_DOCUMENTATION_SOURCE_DIR}/release_notes.rst FILE_CONTENTS)
string(REGEX REPLACE
    ":issue:`([0-9]*)`"
    "`#\\1 <https://github.com/pgRouting/pgrouting/issues/\\1>`__"
    FILE_CONTENTS "${FILE_CONTENTS}")
string(REGEX REPLACE
    ":milestone:`([0-9]*\.[0-9]*\.[0-9]*)`"
    "`Github milestone for \\1 <https://github.com/pgRouting/pgrouting/issues?utf8=%E2%9C%93&q=milestone%3A%22Release%20\\1%22>`__"
    FILE_CONTENTS "${FILE_CONTENTS}")
file(WRITE ${PGR_DOCUMENTATION_SOURCE_DIR}/release_notes.rst "${FILE_CONTENTS}")


add_subdirectory(images)
set(PROJECT_DOC_FILES ${PROJECT_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
set(PROJECT_IMG_FILES ${PROJECT_IMG_FILES} PARENT_SCOPE)
