---
title: Compiling from git
---

You will need a c++11 compiler (for example `g++` or `clang`), `cmake`,
and `boost` installed.

Please take a look at [compile options] for the available options.

[compile options]: /installation/source/options.html

## Initial installation

1. (download sourcecode)
   `git clone git://github.com/dannyedel/dspdfviewer.git`
2. (enter directory)
   `cd dspdfviewer`
3. (make a build sub-directory)
   `mkdir build ; cd build`
4. (create Makefile)
   `cmake ..`
   (*note: this step should tell you about missing build-deps*)
5. (build)
   `make`
6. (install)
   <span class="root">`make install`</span>
   (optional)

Be advised that there is *no* automatic uninstallation,
so please consider packaging
the software using your distribution's preferred packaging method.

The `cmake` step accepts `--prefix=` and the generated
`make install` accepts the `DESTDIR` setting, so your packager can
redirect the files where it needs them.

## Updating

1. (enter directory)
   `cd .../dspdfviewer/build/`
2. (update from git)
   `git pull`
3. (build)
   `make`
4. (install system-wide)
   <span class="root">`make install`</span>
   (optional)

Again, please use your distribution's preferred packaging wrapper around `make install`.
