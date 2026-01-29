#!/bin/bash
# This file is part of the pgRouting project.
# Copyright (c) 2020-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

# This test checks that all source files correctly have license headers

DIR=$(git rev-parse --show-toplevel)
pushd "${DIR}" > /dev/null || exit


# extensions
EXCLUDE_LIST="png|jpeg|gif|ico|md|sig|mo|xml"
# directories
EXCLUDE_LIST="${EXCLUDE_LIST}|_static|_templates|git-tools|doctrees"
# files
EXCLUDE_LIST="${EXCLUDE_LIST}|forward|README|Doxyfile|allPairs-performance|typos_whitelist|CPPLINT|gitignore"
mylicensecheck() {
    licensecheck --copyright -r -m -l 30 --tail 0 -i "$EXCLUDE_LIST" "$1"
}


error=0
copyr=""
missing=""
NAMES=("doc" "src" "include" "docqueries" "pgtap" "sql" "tools" "ci" "cmake" "doxygen")
for n in "${NAMES[@]}" ; do
    results=$(mylicensecheck "${n}")
    tmp=$(grep UNKNOWN <<< "${results}")
    if [ -n "${tmp}" ] ; then
        error=1
        missing="$missing ${tmp}"
    fi
    tmp=$(grep "No copyright" <<< "${results}")
    if [ -n "${tmp}" ] ; then
        error=1
        copyr="${copyr} ${tmp}"
    fi
done

if [[ ${copyr} ]]; then
  echo " ****************************************************"
  echo " *** Found source files without copyright           *"
  echo " ****************************************************"
  echo "${copyr}"
fi

if [[ $missing ]]; then
  echo " ****************************************************"
  echo " *** Found source files without valid license headers"
  echo " ****************************************************"
  echo "$missing"
fi

echo the pop
popd > /dev/null || exit ${error}
echo the pop
exit $error
