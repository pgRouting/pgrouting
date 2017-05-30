

# Locale

To get latest pot files
```
cd build
cmake  -DWITH_DOC=ON -DBUILD_LOCALE=ON ..
make locale
```

list pot files
```
ls doc/_build/gettext
```

build the locale files
```
sphinx-intl update -p doc/_build/gettext -d doc/_build/locale -l es,en 
```

Currently the setup is for `en` and `es`

## Change only what is needed

* Copy the pot files that changed and all the corresponding po files in other languages

```
cp doc/_build/gettext/pgr_createVerticesTable.pot ../locale/pot
cp doc/_build/locale/en/LC_MESSAGES/pgr_createVerticesTable.po ../locale/en/LC_MESSAGES
cp doc/_build/locale/es/LC_MESSAGES/pgr_createVerticesTable.po ../locale/es/LC_MESSAGES
```


### Push the resource to transifex

```
tx push --source -r pgrouting.pgr_createVerticesTable
```

### Pull transtlated strings

```
tx pull -r pgrouting.pgr_createVerticesTable -l es
```

Note: if the file is skip `-f` forces the pull but basically it means:

* locally the po file changed and no push of the resource was done.
* A translation has being downloaded and no further translations on the transifex file have being done 

### clean the build & build the documentation:

```
rm -rf *
cmake  -DWITH_DOC=ON -DBUILD_HTML=ON ..
```

## Which resources need change:

The translation strings are in the po files.
The Engish transtlations should not change unless the documentation changed

### English

Check the English differences with the commited files
```
diff -r doc/_build/locale/en ../locale/en
```

Sample output on a file:
```
diff -r doc/_build/locale/en/LC_MESSAGES/withPoints-family.po ../locale/en/LC_MESSAGES/withPoints-family.po
11c11
< "POT-Creation-Date: 2017-05-30 08:52-0500\n"
---
> "POT-Creation-Date: 2017-05-30 08:27-0500\n"
```

### Other languages

Check the English differences with the commited files
```
diff -r doc/_build/locale/es ../locale/es
```

* there are no differences when the file has no translation

Sample output on a translated file:
```
36c39
< msgstr ""
---
> msgstr "Sinopsis"
```

## The pot files

Check the differences with the commited files
```
diff doc/_build/gettext ../locale/pot
```

Sample output on a file:
```
diff doc/_build/gettext/VRP-category.pot ../locale/pot/VRP-category.pot
11c11
< "POT-Creation-Date: 2017-05-30 08:52-0500\n"
---
> "POT-Creation-Date: 2017-05-30 08:03-0500\n"
```

# Push to transifex only what is needed


References:

* https://pypi.python.org/pypi/sphinx-intl

