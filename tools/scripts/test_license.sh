#!/usr/bin/env bash

# This test checks that all source files correctly have license headers

EXCLUDE_LIST="txt|sig|png|jpeg|_static|md|control|html|cfg"

mylicensecheck() {
    licensecheck -r --copyright -l 30 --tail 0 -i "$EXCLUDE_LIST" "$1"
}

DIR=$(git rev-parse --show-toplevel)

pushd "${DIR}" > /dev/null || exit
missing=$(! { mylicensecheck src & mylicensecheck  sql &  mylicensecheck include;}  | grep "No copyright\|UNKNOWN")
missing1=$(mylicensecheck doc  | grep "No copyright")
missing2=$(grep --files-without-match 'Creative Commons' doc/*/*.rst)
popd > /dev/null || exit

#mylicensecheck doc
error=0
if [[ $missing ]]; then
  echo " ****************************************************"
  echo " *** Found source files without valid license headers"
  echo " ****************************************************"
  echo "$missing"
  error=1
fi

if [[ $missing1 ]]; then
  echo " ****************************************************"
  echo " *** Found documentation files without copyright"
  echo " ****************************************************"
  echo "$missing1"
  error=1
fi

if [[ $missing2 ]]; then
  echo " ****************************************************"
  echo " *** Found documentation files without valid license headers"
  echo " ****************************************************"
  echo "$missing2"
  error=1
fi
exit $error

