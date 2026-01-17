# This file is part of the pgRouting project.
# Copyright (c) 2017-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
foreach (f ${MINORS})
    configure_file(${PROJECT_NAME_LOWER}--${f}.sig ${PROJECT_NAME_LOWER}--${f}.sig COPYONLY)
    list(APPEND SIGNATURE_FILES ${PROJECT_NAME_LOWER}--${f}.sig)
endforeach()

