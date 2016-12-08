scripts for developers
----------------------

To be executed from the root of the repository
cd /path/to/repo/

Spell checker
..............

sh tools/scripts/fix_typos.sh

- creates a directory fix_typos:
  - contains the code and dictonaries
- checks the spelling of the:
  - src
  - doc
- does not check:
  - tools


Code linter
...........

Requierements
- python

sh tools/scripts/code_checker.sh <directory>

- if no directory is given:
  - it works on all src/*/src

- output is grouped by extension
