#!/bin/bash

# This runs shellcheck on all sh files


DIR=$(git rev-parse --show-toplevel)

pushd "${DIR}" > /dev/null || exit
read -ra files < <(git ls-files | grep '\.sh')

result=$(shellcheck "${files[@]}")
if [[ $result ]]; then
  echo "$result"
  echo " *** shellcheck found script errors"
  exit 1
fi
