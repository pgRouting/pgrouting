#!/usr/bin/env bash
# Runs clang-tidy only on changed files

set -e

readonly BASE=$1
readonly BUILD_DIR=build

MODIFIED_FILES=$(git diff-tree --no-commit-id --diff-filter=d --name-only -r "$BASE" HEAD | grep '\.c')
MODIFIED_HEADERS=$(git diff-tree --no-commit-id --diff-filter=d --name-only -r "$BASE" HEAD | grep '\.h')
POSTGRES_SERVER=$(grep -o -m1 '\-isystem .*' "${BUILD_DIR}/compile_commands.json" | head -1 | awk '{print $1" "$2}')

CHECKS="-checks=clang-analyzer-*"

if [ ${#MODIFIED_FILES[@]} = 0 ] && [ ${#MODIFIED_HEADERS[@]} = 0 ]; then
  echo "No paths modified"
  exit 0
fi

# C/C++ use the compile command
if [ ${#MODIFIED_FILES[@]} != 0 ] ; then
    for f in ${MODIFIED_FILES}
    do
        echo "${f}"
        clang-tidy -p "${BUILD_DIR}" "${CHECKS}" -header-filter="^$(pwd).*" "${f}"
       done
fi

# H/HPP do not have a compile command
# workaround: add the includes
if [ ${#MODIFIED_HEADERS[@]} != 0 ] ; then
    for f in ${MODIFIED_HEADERS}
    do
        echo "${f}"
        clang-tidy "${CHECKS}" -header-filter=.* "${f}" \
            -- -I./include "${POSTGRES_SERVER}"
    done
fi
