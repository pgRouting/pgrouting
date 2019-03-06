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

if [ -f sql/sigs/pgrouting--$1.sig ]
then
    echo "- [x] sql/sigs/pgrouting--$1.sig"
else
    error_msg "  FATAL: sql/sigs/pgrouting--$1.sig Not found"
    exit 1
fi
}


#---------------------------------------------------------------------


MAYOR=3
MINOR=0
MICRO=0
RC="-dev"
PREV_REL=2.6.0
DEBUG=$1
BRANCH="release/$MAYOR.$MINOR"


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

CURRENTNEWS=$(grep $MAYOR.$MINOR.$MICRO doc/src/release_notes.rst | grep ref)
if [[ $? != 0 ]]; then
    error_msg "Section $MAYOR.$MINOR.$MICRO in release_notes.rst file is missing"
    exit 1
else
    echo "- [x] release_notes.rst section $MAYOR.$MINOR.$MICRO exists"
fi


OLDNEWS=$(grep $PREV_REL doc/src/release_notes.rst | grep ref)
if [[ $? != 0 ]]; then
    error_msg "Section $PREV_REL in release_notes.rst file is missing"
    exit 1
else
    echo "- [x] release_notes.rst section $PREV_REL exists"
fi


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
    echo "cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_MAJOR \"$MAYOR\")'"
    echo "cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_MINOR \"$MINOR\")'"
    echo "cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_PATCH \"$MICRO\")'"
    echo "cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_DEV \"$RC\")'"
    echo "\`\`\`"
fi

if [[ $(grep 'set(PGROUTING_VERSION_MAJOR' CMakeLists.txt | grep $MAYOR) != "set(PGROUTING_VERSION_MAJOR \"$MAYOR\")" ]]; then
    error_msg "FATAL: PGROUTING_VERSION_MAJOR is not '$MAYOR' ... Verify CMakeLists.txt"
    exit 1
else
    echo "  - [x] mayor information is OK"
fi


if [[ $(grep 'set(PGROUTING_VERSION_MINOR' CMakeLists.txt | grep $MINOR) !=  "set(PGROUTING_VERSION_MINOR \"$MINOR\")" ]]; then
    error_msg "FATAL: PGROUTING_VERSION_MINOR is not '$MINOR' ... Verify CMakeLists.txt"
    exit 1
else
    echo "  - [x] Check minor information is OK"
fi

if [[ $(grep 'set(PGROUTING_VERSION_PATCH' CMakeLists.txt | grep $MICRO) !=  "set(PGROUTING_VERSION_PATCH \"$MICRO\")" ]]; then
    error_msg "FATAL: PGROUTING_VERSION_PATCH is not '$MICRO' ... Verify CMakeLists.txt"
    exit 1
else
    echo "  - [x] Check patch information is OK"
fi

if [[ $(grep 'set(PGROUTING_VERSION_DEV' CMakeLists.txt ) !=  "set(PGROUTING_VERSION_DEV \"$RC\")" ]]; then
    error_msg "FATAL: PGROUTING_VERSION_DEV is not '$RC' ... Verify CMakeLists.txt"
    exit 1
else
    echo "  - [x] Check dev information is OK"
fi


#---------------------------------------------------------------------

echo "- src/common/test/doc-pgr_version.result"

#---------------------------------------------------------------------

if [[ -n $DEBUG ]]; then
    echo "\`\`\`"
    echo "cat test/common/doc-pgr_version.result | grep \"$MAYOR.$MINOR.$MICRO\""
    echo "\`\`\`"
fi

if [[ $(grep "$MAYOR.$MINOR.$MICRO" test/common/doc-pgr_version.result) != " $MAYOR.$MINOR.$MICRO" ]]; then
    error_msg "test/common/doc-pgr_version.result is not $MAYOR.$MINOR.$MICRO"
    exit 1
else
    echo "  - [x]  src/common/test/doc-pgr_version.result"
fi

#---------------------------------------------------------------------

echo "- VERSION"

#---------------------------------------------------------------------

if [[ -n $DEBUG ]]; then
    echo "\`\`\`"
    echo "grep \"release/$MAYOR.$MINOR\" VERSION"
    echo "\`\`\`"
fi

if [[ $(grep "release/$MAYOR.$MINOR" VERSION) != *"release/$MAYOR.$MINOR" ]]; then
    error_msg "VERSION should have 'release/$MAYOR.$MINOR'"
    exit 1
fi
echo "  -[x] VERSION file branch: OK"

#---------------------------------------------------------------------
echo
echo "### Checking signature files exist"
echo
#---------------------------------------------------------------------
test_file 3.0.0
#test_file 2.6.1
test_file 2.6.0
test_file 2.5.3
test_file 2.5.2
test_file 2.5.1
test_file 2.5.0
test_file 2.4.2
test_file 2.4.1
test_file 2.4.0
test_file 2.3.2
test_file 2.3.1
test_file 2.3.0
test_file 2.2.4
test_file 2.2.3
test_file 2.2.2
test_file 2.2.1
test_file 2.2.0
test_file 2.1.0
test_file 2.0.1
test_file 2.0.0


#---------------------------------------------------------------------
echo
echo "### Locally make a clean build as Release"
echo
#---------------------------------------------------------------------

echo "\`\`\`"

if [[ -z  "$DEBUG" ]]; then
    echo "bash tools/release-scripts/compile-release.sh 5   $MAYOR.$MINOR $MICRO"
    echo "bash tools/release-scripts/compile-release.sh 4.9 $MAYOR.$MINOR $MICRO"
    echo "bash tools/release-scripts/compile-release.sh 4.6 $MAYOR.$MINOR $MICRO"
    bash tools/release-scripts/compile-release.sh 4.9 $MAYOR.$MINOR $MICRO
    bash tools/release-scripts/compile-release.sh 4.6 $MAYOR.$MINOR $MICRO
    bash tools/release-scripts/compile-release.sh 5 $MAYOR.$MINOR $MICRO
fi
echo "bash tools/release-scripts/compile-release.sh 4.8 $MAYOR.$MINOR $MICRO"
echo "\`\`\`"
bash tools/release-scripts/compile-release.sh 4.8 $MAYOR.$MINOR $MICRO

echo - [x] completed local builds

#---------------------------------------------------------------------
echo "### checking the signature files dont change"
#---------------------------------------------------------------------

sh tools/release-scripts/get_signatures.sh $MAYOR.$MINOR.$MICRO ___sig_generate___ sql/sigs >> build/tmp_sigs.txt

if [[ -z  "$DEBUG" ]]; then
    git_no_change
fi

echo
echo - [x] completed check: OK
echo

#---------------------------------------------------------------------
echo "### Locally run the update tester"
#---------------------------------------------------------------------
echo "\`\`\`"
echo bash tools/testers/update-tester.sh
echo "\`\`\`"
bash tools/testers/update-tester.sh
if [[ $? != 0 ]]; then
    echo "FATAL on the update-tester"
    exit 1
fi
echo - [x] completed update testing


#---------------------------------------------------------------------
echo "### execute the documentation queries"
#---------------------------------------------------------------------
echo "\`\`\`"
echo tools/testers/algorithm-tester.pl -documentation
echo git status
echo "\`\`\`"
tools/testers/algorithm-tester.pl -documentation
if [[ $? != 0 ]]; then
    echo "FATAL errors found generating documentation result files"
    exit 1
fi

if [[ -z  "$DEBUG" ]]; then
    git_no_change
elif [[ $(git status | grep 'Changes not staged for commit:') ]]; then
    echo "DEBUG WARNING: at least one file changed"
    git status
    exit 1
fi

echo - [x] No files changed


echo "End of check"
