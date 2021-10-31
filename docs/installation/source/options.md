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

## Options for v1.15

By default, you will need the following external software:

* `boost`
* `cmake`
* `qt5`
* `poppler` with the Qt5 bindings
* If you want to compile the test PDFs during build (activated by default):
  * `pdflatex` with `latex-beamer` package installed

The following `cmake` options manipulate the compilation and dependency list:

* `DSPDFVIEWER_VERSION` string
  * Completely overrides the version reported by `dspdfviewer --version`.
  Use this if you're packaging `dspdfviewer` for a distribution and want
  to include a dist-specific version.
  * By default, this is auto-detected using `git describe`, with a hardcoded
  fallback version number (such as, when not building from a git clone).
* `UseQtFive` boolean, default ON
  * If this is OFF, `dspdfviewer` will be built against `Qt4` and the
  `poppler-qt4` bindings, instead of the Qt5 version.
  * If this options is disabled, dspdfviewer depends on Qt4 and libpoppler-qt4,
  and no longer depends on Qt5 or libpoppler-qt5.
  * The Qt4 support will get dropped eventually.  Please use the Qt5
  version and report bugs if you experience regressions compared to Qt4.
* `UpdateTranslations` boolean, default OFF
  * If you want to work on the translation, you can activate this flag
  to make `cmake` regenerate the `.ts` files.
  **Be warned** that when this switch is ON, cmake sees the `.ts` files
  as *output* files and will **delete** them if you call `make clean`.
* `BuildTests` boolean, default ON
  * Builds unit tests.  These tests will require PDFs to function, which
  are included in the tarball, but can be (re)generated during the build.
  * `RunDualScreenTests` boolean, default ON
    * Include unit tests that require two screens to be connected.
  * `RunTestsOnBigEndian` boolean, default OFF
    * Run the unit tests on a big-endian system.  By default, tests are
      only run on little-endian systems.
  * `UsePrerenderedPDF` boolean, default OFF
    * Use pre-rendered PDFs for the test suite, instead of compiling them
      on-the-fly during the build.
    * Enable this to speed up build time and reduce dependencies. 
    * The default value will probably switch to ON in a future version.
* `BoostStaticLink` boolean, default OFF
  * Link against the `boost` libraries statically (ON) or dynamically (OFF)
  * If your platform supports dynamic linking, ideally through a package manager
  that takes care of installing and updating the libraries, it is highly
  recommended to stick to dynamic linking.
  * If you link statically, please ensure that you recompile when the boost
  libraries get updated.
* `WindowsStaticLink` boolean, default ON
  * Only relevant for Windows builds using the MS Visual C++ Compiler.
  * Sets the `/MT` instead of the `/MD` compiler flags.
* `CodeCoverage` boolean, default OFF
  * This flag adds the necessary flags to record code coverage (currently
  only supported with gcc).  Only of interest for developers, this massively
  slows down the code.  Don't use it for production builds.


---

## Historical options (no longer present in current version)

* `DownloadTestPDF`
  * This option is ignored since v1.15,
    because the pre-rendered PDF files are now distributed in
    the release tarball.

back to [compile from source](/installation/)
