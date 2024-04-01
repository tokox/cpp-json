# C++ JSON object import/export

This library contains a *class* to represent **JSON object** and **functions to parse/format it**.

- This library is **header-only** *(no precompiled files)*. Just include `json.hpp` in your project. \
  \**There are `.cpp` files, but they are included from the header file.*
- It **does not** use any *external libraries* (only *standard C++ libraries*).
- Test program called `test.cpp` is just *parsing* and *formatting* a **JSON object**.

## Versions & Releases

- All versions are tagged in the format `vMAJOR.MINOR(-PATCH)?`. \
  -PATCH is optional. \
  *Example: `v1.0`, `v1.1`, `v1.2-2`.*
- Special tag `vlatest` always points to the latest version.

- Releases are made automatically on Github for every new version. \
  They are available in the **Releases** tab.
- All releases automatically build on **Fedora COPR** in [`tokox/cpp-json` repo](https://copr.fedorainfracloud.org/coprs/tokox/cpp-json/). \
  Release **is not** made if build fails. \
  All built packages are uploaded as assets to the release.
- Releases may be also available in *AUR* or *PPA* in the future. \
  *I'm not using them, so I don't know how and where to do it and I am not really interested in these. If you want to help, contact me.*
- I am not planning on any Windows or MacOS support.

## Documentation *(in progress)*

- Documentation will be created using *Doxygen*.
- It will be automatically generated and pushed to `gh-pages` branch on every commit.
- `gh-pages` branch will be binded to Github Pages and available at [https://tokox.github.io/cpp-json/](https://tokox.github.io/cpp-json/).
- Please wait a moment for the documentation to be available.

## Some useless license information

This library is licensed under the terms of the [**MIT license**](LICENSE.md).
