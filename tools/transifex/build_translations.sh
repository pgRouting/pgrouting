#!/bin/bash
# ------------------------------------------------------------------------------
# pgRouting Scripts
# Copyright(c) pgRouting Contributors
#
# Download translations and build localized documentation
# ------------------------------------------------------------------------------

ROOT=$(pwd)
CONFIG="build/doc/_build"
DOCDIR="doc/i18n"

LANGUAGES='de es ja'

if [ $1 ]; then
	LANGUAGES=$1
fi

echo "*************************************************************************"
echo "Download translations from Transifex (>1% translated)"
echo "*************************************************************************"
for i in ${LANGUAGES}; do
	tx pull -l "${i}" -f --minimum-perc=1
done

echo "*************************************************************************"
echo "Build PO/MO files"
echo "*************************************************************************"
for i in ${LANGUAGES}; do
	sphinx-intl build -l "${i}" -c "${CONFIG}/conf.py" -p "${DOCDIR}/pot" -d "${DOCDIR}"
done

echo "*************************************************************************"
echo "Build HTML documentation"
echo "*************************************************************************"
for i in ${LANGUAGES}; do
	sphinx-build -b html -a -E -D language="${i}" -c "${CONFIG}" ${ROOT} build/doc/html/${i}
done

echo "*************************************************************************"
echo "Build LATEX documentation"
echo "*************************************************************************"
for i in ${LANGUAGES}; do
	DESTINATION="build/doc/latex/${i}"
	sphinx-build -b html -a -E -D language="${i}" -c "${CONFIG}" ${ROOT} ${DESTINATION}
	cd "${DESTINATION}"
	pdflatex -interaction=nonstopmode pgRoutingDocumentation.tex > /dev/null 2>&1
	cd "${ROOT}"
done

echo "*************************************************************************"
echo "Build MAN documentation"
echo "*************************************************************************"
for i in ${LANGUAGES}; do
	sphinx-build -b html -a -E -D language="${i}" -c "${CONFIG}" ${ROOT} build/doc/man/${i}
done
