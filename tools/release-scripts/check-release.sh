#!/bin/bash

set -e

#---------------------------------------------------------------------
# Functions used in the script
function error_msg() {

    echo "*****************************************************"
    echo "*****************************************************"
    echo "*****************************************************"
    echo
    echo
    echo "FATAL: $1"
    echo
    echo
    echo
    echo "*****************************************************"
    echo "*****************************************************"
}

function git_no_change {

if [[ $(git status | grep 'Changes not staged for commit:') ]]; then
    error_msg " There are changes staged on git"
    echo
    echo "HINT: Verify the changes and commit them"
    echo
    echo
    sleep 5
    git status
    echo "*****************************************************"
    echo "*****************************************************"
    exit 1
fi

}

function test_file {

if [ -f "sql/sigs/pgrouting--$1.sig" ]
then
    echo "- [x] sql/sigs/pgrouting--$1.sig"
else
    error_msg "  FATAL: sql/sigs/pgrouting--$1.sig Not found"
    exit 1
fi
}


#---------------------------------------------------------------------


MAYOR=3
MINOR=1
MICRO=1
RC=""
PREV_REL=3.0.3
PREV_RELS="
    3.0.3 3.0.2 3.0.1 3.0.0
    2.6.3 2.6.2 2.6.1 2.6.0
    "
# These releases are not for update
OLD_RELS="
    2.5.4 2.5.3 2.5.2 2.5.1 2.5.0
    2.4.2 2.4.1 2.4.0
    2.3.2 2.3.1 2.3.0
    2.2.4 2.2.3 2.2.2 2.2.1 2.2.0
    2.1.0
    2.0.1 2.0.0"
DEBUG=$1


if [[ -z  "$DEBUG" ]]; then
    git_no_change
fi

echo - [x] No files changed before execution.
echo


#---------------------------------------------------------------------
echo
echo "### Verify typos"
echo
#---------------------------------------------------------------------

if [[ -n $DEBUG ]]; then
    echo
    echo "\`\`\`"
    echo sh tools/scripts/fix_typos.sh
    echo "\`\`\`"
    echo
fi

sh tools/scripts/fix_typos.sh

if [[ -z  "$DEBUG" ]]; then
    git_no_change
fi

echo "- [x] No typos found by script"


#---------------------------------------------------------------------
echo
echo "### Verify release_notes.rst & NEWS"
echo
#---------------------------------------------------------------------

if [[ -n $DEBUG ]]; then
    echo
    echo "\`\`\`"
    echo "grep $MAYOR.$MINOR.$MICRO doc/src/release_notes.rst | grep ref"
    echo "grep $PREV_REL doc/src/release_notes.rst | grep ref"
    echo "tools/release-scripts/notes2news.pl"
    echo "\`\`\`"
    echo
fi

if ! grep $MAYOR.$MINOR.$MICRO doc/src/release_notes.rst > /dev/null; then
    rror_msg "Section $MAYOR.$MINOR.$MICRO in release_notes.rst file is missing"
    exit 1
else
    echo "- [x] release_notes.rst section $MAYOR.$MINOR.$MICRO exists"
fi


for r in $PREV_RELS $OLD_RELS
do
    if ! grep "${r}" doc/src/release_notes.rst > /dev/null; then
        error_msg "Section $r in release_notes.rst file is missing"
        exit 1
    else
        echo "- [x] release_notes.rst section $r exists"
    fi
done


tools/release-scripts/notes2news.pl

if [[ -z  "$DEBUG" ]]; then
    git_no_change
fi

echo "- [x] NEWS is up to date"



#---------------------------------------------------------------------
echo
echo "## Check version information"
echo
#---------------------------------------------------------------------
#---------------------------------------------------------------------
echo
echo "- CMakeLists"
#---------------------------------------------------------------------

if [[ -n $DEBUG ]]; then
    echo "\`\`\`"
    echo "grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt"
    echo "cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_DEV \"$RC\")'"
    echo "\`\`\`"
fi

if [[ $(grep -Po '(?<=project\(PGROUTING VERSION )[^;]+' CMakeLists.txt) != "$MAYOR.$MINOR.$MICRO" ]]; then
    error_msg "FATAL: PGROUTING_VERSION is not '$MAYOR.$MINOR.$MICRO' ... Verify CMakeLists.txt"
    exit 1
else
    echo "  - [x] $MAYOR.$MINOR.$MICRO information is OK"
fi


if [[ $(grep 'set(PGROUTING_VERSION_DEV' CMakeLists.txt ) !=  "set(PGROUTING_VERSION_DEV \"$RC\")" ]]; then
    error_msg "FATAL: PGROUTING_VERSION_DEV is not '$RC' ... Verify CMakeLists.txt"
    exit 1
else
    echo "  - [x] Check dev information is OK"
fi


#---------------------------------------------------------------------

echo "- "docqueries/version/doc-version.result

#---------------------------------------------------------------------

if [[ -n $DEBUG ]]; then
    echo "\`\`\`"
    echo "grep '$MAYOR.$MINOR.$MICRO$RC' docqueries/version/doc-version.result"
    echo "\`\`\`"
fi

if [[ $(grep "$MAYOR.$MINOR.$MICRO$RC" docqueries/version/doc-version.result) != " $MAYOR.$MINOR.$MICRO$RC" ]]; then
    error_msg "test/common/doc-pgr_version.result is not $MAYOR.$MINOR.$MICRO$RC"
    exit 1
else
    echo "  - [x]  $MAYOR.$MINOR.$MICRO$RC in test/common/doc-pgr_version.result"
fi


#---------------------------------------------------------------------
echo
echo "### Checking signature files exist"
echo
#---------------------------------------------------------------------
test_file "$MAYOR.$MINOR.$MICRO"
for r in $PREV_RELS
do
    test_file "$r"
done


#---------------------------------------------------------------------
echo "### checking the signature files dont change"
#---------------------------------------------------------------------

bash tools/release-scripts/compile-release.sh
bash tools/release-scripts/get_signatures.sh
if [[ -z $(git diff "sql/sigs/pgrouting--$MAYOR.$MINOR.$MICRO.sig") ]]; then
    echo signature changed at: "sql/sigs/pgrouting--$MAYOR.$MINOR.$MICRO.sig"
fi

echo
echo - [x] signature file "sql/sigs/pgrouting--$MAYOR.$MINOR.$MICRO.sig" is OK
echo

#---------------------------------------------------------------------
echo "### Locally run the update tester"
#---------------------------------------------------------------------
echo "\`\`\`"
echo bash tools/testers/update-tester.sh
echo "\`\`\`"

for r in ${PREV_RELS}
do
    if ! bash tools/testers/update-tester.sh "$r"; then
        echo "FATAL updating from $r"
        exit 1
    fi
done
echo - [x] completed update testing


#---------------------------------------------------------------------
echo "### execute the documentation queries"
#---------------------------------------------------------------------
echo "\`\`\`"
echo tools/testers/doc_queries_generator.pl -documentation
echo git status
echo "\`\`\`"
tools/testers/doc_queries_generator.pl -documentation

if [[ -z  "$DEBUG" ]]; then
    git_no_change
elif [[ $(git status | grep 'Changes not staged for commit:') ]]; then
    echo "DEBUG WARNING: at least one file changed"
    git status
    exit 1
fi

echo - [x] No files changed

#---------------------------------------------------------------------
echo
echo "### Locally make a clean build as Release"
echo
#---------------------------------------------------------------------


if [[ -z  "$DEBUG" ]]; then
    echo "\`\`\`"
    echo "bash tools/release-scripts/compile-release.sh"
    echo "\`\`\`"
fi
bash tools/release-scripts/compile-release.sh

echo - [x] completed local builds

echo "End of check"
