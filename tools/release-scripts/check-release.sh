#!/bin/bash

set -e

if [[ -z  $1 ]]; then
    echo "Mayor missing";
    echo "Usage"
    echo "tools/release-scripts/release-check.sh Mayor Minor Micro Last [branch]";
    exit 1;
fi
if [[ -z  $2 ]]; then
    echo "Minor missing";
    echo "Usage"
    echo "tools/release-scripts/release-check.sh Mayor Minor Micro Last [branch]";
    exit 1;
fi

if [[ -z  $3 ]]; then
    echo "Micro missing";
    echo "Usage"
    echo "tools/release-scripts/release-check.sh Mayor Minor Micro Last [branch]";
    exit 1;
fi

if [[ -z  $4 ]]; then
    echo "Last Micro missing";
    echo "Usage"
    echo "tools/release-scripts/release-check.sh Mayor Minor Micro Last [branch]";
    exit 1;
fi


MAYOR=$1
MINOR=$2
MICRO=$3
PREV_REL=$4
RC=""

if [[ -z  $5 ]]; then
    BRANCH="release/$MAYOR.$MINOR"
else
    BRANCH=$5
fi

#---------------------------------------------------------------------
echo "### Verify branch to be $BRANCH"
echo
#---------------------------------------------------------------------

GIT_BRANCH="$(git rev-parse --abbrev-ref HEAD)"
echo "- [x] Working Branch: $BRANCH";
echo

if [[ "$GIT_BRANCH" == "$BRANCH" ]]; then
   echo "  - [x] Already in branch $BRANCH";
else
   echo "  - Current Branch: $BRANCH";
   echo "\`\`\`"
   echo git checkout $BRANCH
   echo "\`\`\`"
   exit 1
fi

#---------------------------------------------------------------------
echo
echo "### Verify typos"
echo
#---------------------------------------------------------------------

echo "\`\`\`"
echo sh tools/scripts/fix_typos.sh
echo "\`\`\`"

sh tools/scripts/fix_typos.sh

if [[ $? != 0 ]]; then
    echo "FATAL: Typos were found"
    exit 1
else
    echo "- [x] Typos checked: OK"
fi


#---------------------------------------------------------------------
echo
echo "### Verify Current NEWS & CHANGLOG"
echo
#---------------------------------------------------------------------
echo
echo "- [x] Checking NEWS section $MAYOR.$MINOR.$MICRO exists"
echo "\`\`\`"
echo "grep $MAYOR.$MINOR.$MICRO NEWS"
echo "\`\`\`"
CURRENTNEWS=$(grep $MAYOR.$MINOR.$MICRO NEWS)
if [[ $? != 0 ]]; then
    echo "FATAL: Section $MAYOR.$MINOR.$MICRO in NEWS file is missing"
    exit 1
else
    echo 
fi


echo "- [x] Checking release_notes.rst file section $MAYOR.$MINOR.$MICRO exists"
echo "\`\`\`"
echo "grep $MAYOR.$MINOR.$MICRO doc/src/release_notes.rst | grep ref"
echo "\`\`\`"
CURRENTNEWS=$(grep $MAYOR.$MINOR.$MICRO doc/src/release_notes.rst | grep ref)
if [[ $? != 0 ]]; then
    echo "FATAL: Section $MAYOR.$MINOR.$MICRO in release_notes.rst file are missing"
    exit 1
else
    echo 
fi



#---------------------------------------------------------------------
echo
echo "### Verify $PREV_REL NEWS & CHANGLOG"
echo
#---------------------------------------------------------------------

echo "- [x] Checking NEWS section $PREV_REL exists"
echo "\`\`\`"
echo "grep $PREV_REL NEWS"
echo "\`\`\`"
OLDNEWS=$(grep $PREV_REL NEWS)
if [[ $? != 0 ]]; then
    echo "$PREV_REL NEWS are missing in NEWS file"
    exit 1
else
    echo 
fi


echo "- [x] Checking release_notes.rst section $PREV_REL exists"
echo "\`\`\`"
grep $PREV_REL doc/src/release_notes.rst | grep ref
echo "\`\`\`"
OLDNEWS=$(grep $PREV_REL doc/src/release_notes.rst | grep ref)
if [[ $? != 0 ]]; then
    echo "$PREV_REL NEWS are missing in release_notes.rst file"
    exit 1
else
    echo 
fi




#---------------------------------------------------------------------
echo
echo "### Check version information"
echo 
#---------------------------------------------------------------------
echo "- [x] Check mayor information"
echo "\`\`\`"
echo "cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_MAJOR \"$MAYOR\")'"
echo "\`\`\`"
if [[ $(cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_MAJOR' | grep $MAYOR) != "set(PGROUTING_VERSION_MAJOR \"$MAYOR\")" ]]; then
    echo "FATAL: PGROUTING_VERSION_MAJOR is not $MAYOR"
    exit 1
else
    echo
fi


echo "- [x] Check minor information"
echo "\`\`\`"
echo "cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_MINOR \"$MINOR\")'"
echo "\`\`\`"
if [[ $(cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_MINOR' | grep $MINOR) !=  "set(PGROUTING_VERSION_MINOR \"$MINOR\")" ]]; then
    echo "FATAL: PGROUTING_VERSION_MINOR is not $MINOR"
    exit 1
else
    echo
fi

echo "- [x] Check patch information"
echo "\`\`\`"
echo "cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_PATCH \"$MICRO\")'"
echo "\`\`\`"
if [[ $(cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_PATCH' | grep $MICRO) !=  "set(PGROUTING_VERSION_PATCH \"$MICRO\")" ]]; then
    echo "FATAL: PGROUTING_VERSION_PATCH is not $MICRO"
    exit 1
else
    echo
fi

echo "\`\`\`"
echo "cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_DEV \"$RC\")'"
echo "\`\`\`"
if [[ $(cat CMakeLists.txt | grep 'set(PGROUTING_VERSION_DEV' ) !=  "set(PGROUTING_VERSION_DEV \"$RC\")" ]]; then
    echo "FATAL: PGROUTING_VERSION_DEV is not $RC"
    exit 1
else
    echo "- [x] Check dev information"
    echo 
fi


#---------------------------------------------------------------------
echo
echo "### Check pgr_version test result"
echo 
#---------------------------------------------------------------------

echo "\`\`\`"
echo "cat doc/test/utilities-any.result | grep \"$MAYOR.$MINOR.$MICRO\""
echo "\`\`\`"
if [[ $(cat doc/test/utilities-any.result | grep "$MAYOR.$MINOR.$MICRO") != "$MAYOR.$MINOR.$MICRO" ]]; then
    echo "FATAL: doc/test/utilities-any.result is not $MAYOR.$MINOR.$MICRO"
    exit 1
else
    echo "- [x] pgr_version result: OK"
    echo 
fi

echo "\`\`\`"
echo "cat VERSION | grep \"$BRANCH\""
echo "\`\`\`"
if [[ $(cat VERSION | grep "$BRANCH") != *"$BRANCH" ]]; then
    echo "FATAL: VERSION branch should be $BRANCH"
    exit 1
else
    echo "  -[x] VERSION file branch: OK"
    echo 
fi

#---------------------------------------------------------------------
function test_file {                                                                                                                                                        

if [ -f tools/sigs/pgrouting--$1.sig ]
then
    echo "- [x] tools/sigs/pgrouting--$1.sig"
else
    echo "  FATAL: tools/sigs/pgrouting--$1.sig Not found"
fi
}

#---------------------------------------------------------------------
echo "### Checking signature files"
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
echo "### Locally make a clean build as Release"
#---------------------------------------------------------------------
echo "\`\`\`"
echo "bash tools/release-scripts/compile-release.sh 5   $MAYOR.$MINOR $MICRO"
echo "bash tools/release-scripts/compile-release.sh 4.9 $MAYOR.$MINOR $MICRO"
echo "bash tools/release-scripts/compile-release.sh 4.6 $MAYOR.$MINOR $MICRO"
echo "bash tools/release-scripts/compile-release.sh 4.8 $MAYOR.$MINOR $MICRO"
echo "\`\`\`"


bash tools/release-scripts/compile-release.sh 5   $MAYOR.$MINOR $MICRO
bash tools/release-scripts/compile-release.sh 4.9 $MAYOR.$MINOR $MICRO
bash tools/release-scripts/compile-release.sh 4.6 $MAYOR.$MINOR $MICRO
bash tools/release-scripts/compile-release.sh 4.8 $MAYOR.$MINOR $MICRO

echo - [x] completed local builds

#---------------------------------------------------------------------
echo "### checking the signature files dont change"
#---------------------------------------------------------------------

sh tools/release-scripts/get_signatures.sh 2.4.1 ___sig_generate___ curr-sig >> build/tmp_sigs.txt
sh tools/release-scripts/get_signatures.sh 2.4.0 ___sig_generate___ sigs >> build/tmp_sigs.txt
sh tools/release-scripts/get_signatures.sh 2.3.2 ___sig_generate___ sigs >> build/tmp_sigs.txt
sh tools/release-scripts/get_signatures.sh 2.3.1 ___sig_generate___ sigs >> build/tmp_sigs.txt
sh tools/release-scripts/get_signatures.sh 2.3.0 ___sig_generate___ sigs >> build/tmp_sigs.txt
sh tools/release-scripts/get_signatures.sh 2.2.4 ___sig_generate___ sigs >> build/tmp_sigs.txt
sh tools/release-scripts/get_signatures.sh 2.2.3 ___sig_generate___ sigs >> build/tmp_sigs.txt
sh tools/release-scripts/get_signatures.sh 2.2.2 ___sig_generate___ sigs >> build/tmp_sigs.txt
sh tools/release-scripts/get_signatures.sh 2.2.1 ___sig_generate___ sigs >> build/tmp_sigs.txt
sh tools/release-scripts/get_signatures.sh 2.2.0 ___sig_generate___ sigs >> build/tmp_sigs.txt
sh tools/release-scripts/get_signatures.sh 2.1.0 ___sig_generate___ sigs >> build/tmp_sigs.txt
#version 2.0.1 can not be upgraded
#sh tools/release-scripts/get_signatures.sh 2.0.1 ___sig_generate___ sigs >> build/tmp_sigs.txt
sh tools/release-scripts/get_signatures.sh 2.0.0 ___sig_generate___ sigs >> build/tmp_sigs.txt

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
tools/testers/algorithm-tester.pl -documentation > build/tmp_generate_doc.txt
if [[ $? != 0 ]]; then
    echo "FATAL errors found generating documentation result files"
    exit 1
fi

if [[ $(git status | grep 'Changes not staged for commit:') ]]; then
    echo "FATAL: at least one result file changed"
    exit 1
fi

echo - [x] No files changed


echo "End of check"
