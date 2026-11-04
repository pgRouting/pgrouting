#!/bin/bash
# This file is part of the pgRouting project.
# Copyright (c) 2021-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

DIR=$(git rev-parse --show-toplevel)

pushd "${DIR}" > /dev/null || exit 1

mkdir -p build
pushd build > /dev/null || exit 1
cmake -DBUILD_HTML=OFF -DBUILD_LOCALE=ON ..
make locale
popd > /dev/null || exit 1

# Remove obsolete entries #~ from .po files
find locale -type f -name '*.po' -exec sh -c '
    msgattrib --no-wrap --no-obsolete -o "$1" "$1"
  ' sh {} \;

# Stage only the locale files that actually changed (msgmerge + msgattrib).
# (sphinx-intl's locale_changes.txt is no longer generated.)
git status --porcelain -- locale | \
    awk '{print $NF}' | \
    grep -E '\.(po|pot)$' | \
    grep -v '/index\.po$' | \
    while read -r f; do git add "$f"; done

git restore --staged locale/*/LC_MESSAGES/index.po
git restore locale/*/LC_MESSAGES/index.po

popd > /dev/null || exit 1
