---
title: Installation and compilation from source
---

You can choose if you want to install the bleeding-edge development version
(may eat your firstborn, plain out crash do other unexpected things) or the
last stable release.

## Current development (git)

You will need a c++11 compiler (for example `g++` or `clang`), `cmake`,
the `boost` and `poppler-qt4` libraries installed.

(once) Clone the current development branch and create a build directory.

```
git clone git://github.com/dannyedel/dspdfviewer.git
cd dspdfviewer
mkdir build
cmake ..
```

From now on, you can pull updates with `git pull` and build with `make`.
You can install with `make install` (this will require root permission).
Note that there is *no* uninstallation, so please consider packaging
the software using your distributions native packaging method.

