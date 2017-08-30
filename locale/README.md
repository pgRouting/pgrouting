

# Locale

Currently the setup is for  `es` `ru` `ja` `it` `de` `fr` 

## Building the LOCALE:

Use this when the English documentation changed.

```
cd build
#rm -rf *   #BE VERY CAREFULL
cmake -D LOCALE=ON ..
make locale
cd ..
sphinx-intl update -d locale
```

review
```
for f in locale/pot/*; do echo $f; grep $f .tx/config; done
for f in `grep '\.pot' .tx/config | sed 's/^.*pot\/\(.*\)$/\1/' | grep '\.pot'` ; do  echo $f; ls locale/pot/* | grep $f  ; done 
```

commit changes and push


## MANAGERS: Interaction with transifex:

### Push the resource to transifex

Push a New or changed resource:

* New resource

Add the resource to the `.tx/config` located at the root of the repository
(Use as example the other resources)

```
vim ../.tx/config
```

* Push the new resource
```
tx push --source -r pgrouting.pgr_createVerticesTable

```
Note: Do not put the file extension

NOTE: INFORM: A documentation frezze to let translators translate

### Pull the resources from transifex

Be patient takes time (I like the `-d` flag just to know what is being downloaded)

* this pulls all the translations
```
tx -d pull -f
```

* this pulls the Spanish translations
```
tx -d pull -f -l es
```



## TRANSLATORS

For this example the translator is translating `pgr_createVerticesTable` to `Spanish`

* Step 1: Build a local documentation

```
cd build
rm -rf *
cmake -D HTML=ON -D ES=ON ..
make html-es
```

* Step 2: Navigate to the page you are translating:

On the Browser go to:
```
file:///path/to/build/doc/_build/html/es/pgr_createVerticesTable.html
```

* Step 3: Pull the translation & build the documentation & refresh browser

```
tx pull -r pgrouting.pgr_createVerticesTable -l es
make html-es
```
`Refresh browser`

## Building the documentation:

NOTE: in any case English is always build

* Building all languages

```
cmake -D HTML=ON -D WITH_ALL_LANG=ON ..
make doc
```

* Building a a particular language

This example shows Spanish:

```
cmake -D HTML=ON -D ES=ON ..
make html
```

* Building a particular language

This example shows Spanish:

```
make -D HTML=ON -D SINGLEHTML=OM -D ES=ON ..
make html
make singlehtml
# OR to build both:
make doc
```

# References

* https://pypi.python.org/pypi/sphinx-intl
* https://docs.transifex.com/client/introduction
* http://www.sphinx-doc.org/en/stable/intl.html
