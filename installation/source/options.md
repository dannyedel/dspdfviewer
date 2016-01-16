---
title: Compile-time options and dependencies
---

At the `cmake` step, you can customize a few things by using cmake defines.
The generic syntax is:

```bash
cd $(mktemp -d) # some empty(!) build directory
cmake /path/to/dspdfviewer/source -DSomeOption=ON -DSomethingElse=OFF
make # builds the app
cmake . -DSomeOption=OFF # change from inside the build dir
make # rebuild with changed option
```

Boolean options in `cmake` are specified with `-DSomeOpt=ON` and `OFF`, respectively.
For string-type options you can specify an arbitrary string, just make sure to correctly
escape it for your shell.

## Options for v1.14

By default, you will need the following external software:

* `boost`
* `cmake`
* `qt4`
* `poppler` with the Qt4 bindings
* For the test suite (activated by default):
  * `pdflatex` with `latex-beamer` package installed
  * On Linux: `xvfb`

The following `cmake` options manipulate the compilation and dependency list:

* `DSPDFVIEWER_VERSION` string
  * Completely overrides the version reported by `dspdfviewer --version`.
  Use this if you're packaging `dspdfviewer` for a distribution and want
  to include a dist-specific version.
  * By default, this is auto-detected using `git describe`, with a hardcoded
  fallback version number (such as, when not building from a git clone).
* `UseQtFive` boolean, default OFF
  * If this is ON, `dspdfviewer` will be built against `Qt5` and the
  `poppler-qt5` bindings, instead of the Qt4 version.
  * If this options is enabled, dspdfviewer depends on Qt5 and libpoppler-qt5,
  and no longer depends on Qt4 or libpoppler-qt4.
* `UpdateTranslations` boolean, default OFF
  * If you want to work on the translation, you can activate this flag
  to make `cmake` regenerate the `.ts` files.
  **Be warned** that cmake now considers the `.ts` file an output and
  will delete it if you call `make clean`.
* `BuildTests` boolean, default ON
  * Builds unit tests.  These tests will require PDFs to render, which
  can either be generated during the build or downloaded from the internet
  (see next option)
  * If this option is enabled, dspdfviewer also depends on the boost unit
  test framework.
* `BoostStaticLink` boolean, default OFF
  * Link against the `boost` libraries statically (ON) or dynamically (OFF)
  * If your platform supports dynamic linking, ideally through a package manager
  that takes care of installing and updating the libraries, it is highly
  recommended to stick to dynamic linking.
  * If you link statically, please ensure that you recompile when the boost
  libraries get updated.
* `DownloadTestPDF` boolean, default OFF
  * If `BuildTests` is off, the value of this option is *ignored*.
  * If this option is off, dspdfviewer depends on a working `pdflatex` installation
  that includes `latex-beamer`.
  * If you want to avoid the rather large dependency chain resulting from this, you can switch
  this to ON and it will download the test PDFs from `http://danny-edel.de/`.
* `CodeCoverage` boolean, default OFF
  * This flag adds the necessary flags to record code coverage (currently
  only supported with gcc).  Only of interest for developers, this massively
  slows down the code.  Don't use it for production builds.

Additionally, `dspdfviewer` always depends on:

* `boost` (component `program_options`)

---

back to [compile from source](/installation/)
