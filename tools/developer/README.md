# How to use the scripts

## addNewVersionLink.sh

All the documentation files have a history of older versions.
When a new mayor or minor is created the top lines need to change

For example: the one in parenthesis is the latest version which is 3.2
```md
  (`3.2 <https://docs.pgrouting.org/3.2/en/pgr_bipartite.html>`__)
  `3.1 <https://docs.pgrouting.org/3.2/en/pgr_bipartite.html>`__
  ...
```
And needs to be 3.3:
```md
  (`3.3 <https://docs.pgrouting.org/3.3/en/pgr_bipartite.html>`__)
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_bipartite.html>`__
  `3.1 <https://docs.pgrouting.org/3.2/en/pgr_bipartite.html>`__
  ...
```

To achieve, the change: from the root of the repository:
```bash
./tools/developer/addNewVersionLink.sh "3\.2" "3\.3"'
```

## commitByDirectory.sh

Some times legwork is done on a directory, for example when using the
`addNewVersionLink.sh` script

And for reviewing the pull request it is easier to do it by directory

From the root of the repository:
```bash
./tools/developer/commitByDirectory.sh doc "Updating links"
```
sub-directories without changes will be ignored

It will create a series of commits on sub-direcotries that were changed
for example the following commit messages will be created:
```bash
    [doc/allpairs] Updating links
    [doc/alpha_shape] Updating links
    [doc/astar] Updating links
    [doc/bdAstar] Updating links
    ...
    [doc/transitiveClosure] Updating links
    [doc/traversal] Updating links
    [doc/trsp] Updating links
    [doc/tsp] Updating links
    [doc/version] Updating links
    [doc/vrp_basic] Updating links
    [doc/withPoints] Updating links
```
