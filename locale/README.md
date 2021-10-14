# Locale

Currently the setup is for  `es`

## Building the LOCALE

This task needs to be done when the English changes.

Although it is automatically done in the github action from time to time it
might be desirable to build the locale locally.

```bash
tools/transifex/update_locale.sh
```

Only files that changed need to be added to the commit. Sometimes the change is
only a timestamp those files are not to be commited.

Note: This is done automatically on the action.

```bash
for line in `cat build/doc/locale_changes_po_pot.txt`; do git add "$line"; done
```

After the files are added and before the commit, restore the files that only
have changes on timestamps

```bash
git restore .
```

## MANAGERS: Interaction with transifex

Resources are updated on transifex automatically from the develop branch.

### Pull the resources from transifex

Be patient takes time (I like the `-d` flag just to know what is being
downloaded)

* this pulls all the translations

```bash
tx -d pull -f
```

* this pulls the Spanish translations

```bash
tx -d pull -f -l es
```

## TRANSLATORS

For this example the translator is translating `pgr_createVerticesTable` to
`Spanish`

**Step 1:** Build a local documentation

```bash
cd build
rm -rf *
cmake -D HTML=ON -D ES=ON ..
make html-es
```

**Step 2:** Navigate to the page you are translating:

On the Browser go to:
```bash
file:///path/to/build/doc/_build/html/es/pgr_createVerticesTable.html
```

**Step 3:** Pull the translation & build the documentation & refresh browser

```bash
tx pull -r pgrouting.pgr_createVerticesTable -l es
make html-es
```

`Refresh browser`

## Building the documentation

NOTE: in any case English is always build

### Building all languages

```bash
cmake -D HTML=ON -D WITH_ALL_LANG=ON ..
make doc
```

### Building a particular language

This example shows Spanish:

```bash
cmake -DWITH_DOC=ON -DES=ON ..
make html
```

## References

* https://pypi.python.org/pypi/sphinx-intl
* https://docs.transifex.com/client/introduction
* http://www.sphinx-doc.org/en/stable/intl.html
