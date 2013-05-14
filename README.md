# pgRouting - Documentation

## Publish on GH-Pages

pgRouting documentation is hosted on [GitHub Pages](http://pages.github.com) with custom domain settings.

The following steps are needed to update the documentation:

1. Build documentation with CMake
2. Checkout `gh-pages` branch
3. Copy the documentation from the `build` directory into root
4. Update `index.html` if required, ie. paths
5. Commit and push the changes

The `index.html` documentation landing page needs to be edited manually. 
It uses Sphinx template styles, but it is not generated automatically.

