#!/bin/bash
# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Download translations and build localized documentation
# ------------------------------------------------------------------------------

LANGUAGES='de es ja fr'

if [ "$1" ]; then
	LANGUAGES=$1
fi

if [ "$2" ]; then
	RESOURCE='-r'
fi

echo "*************************************************************************"
echo "Download translations from Transifex (>1% translated)"
echo "*************************************************************************"
for i in ${LANGUAGES}; do
	tx pull "$RESOURCE" "$2" -l "${i}" -f --minimum-perc=1
done

