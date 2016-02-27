---
title: Installation on Windows
layout: default
---

## Use a ready-to-run binary (recommended for most users)

[@projekter][] provides ready-to-install windows binaries for you.
You can download them at [@projekters release page][].
Download the setup.exe, start it and follow the on-screen-instructions.

## Advanced users: Compile dspdfviewer from source

In the course of adding a Windows build to the CI tests,
[@projekter][] created [archives of poppler and its dependencies][deps],
compiled with MSVC 2013.
This can of course also be used to quickly get a development environment set up
on a windows machine, without needing to spend much time on compiling
dependencies.

*Warning: The dependency archive is provided as-is;
do not expect to receive any kind of update support.
Please only use it as a quickstart mechanism,
and don't expect the included DLLs to be continually
updated with patches from upstream.
If you plan to distribute a dspdfviewer binary,
do yourself a favor and re-compile the
most recent stable versions of all dependencies.*

In addition to the dependency archive,
you will also need the following programs and libraries on your system:

* MSVC 2013 (also known as version 12.0)
* Qt 5.x (make sure you pick the version for MSVC 2013 32bit)
* Boost (also make sure you pick the version for MSVC 12.0 32 bit)
* CMake
* git
* Something to extract the archive with (7z for example)

To use the prebuilt DLLs, extract the archives directly to `c:\` and
make sure to set `-DWindowsStaticLink=OFF` at cmake time,
and it is probably easier to use `-DBoostStaticLink=ON` on a Windows build.
You will also need to set the `PATH` environment to contain the directories of the
DLLs unless you want to copy them to the build dir.

On a debug build, you must make sure that the debug version of `poppler-qt5.dll` is loaded,
for example by copying it into the executable's directory.
See [buildme.bat][] or the [AppVeyor CI configuration][appveyor]
for examples of how to set up and compile a Debug configuration.

[@projekter]: https://github.com/projekter
[@projekters release page]: https://github.com/projekter/dspdfviewer/releases
[deps]: https://github.com/projekter/dspdfviewer/releases/tag/v1.14-42-g4acfb31
[buildme.bat]: https://gist.github.com/dannyedel/549e57aa73936d7d7a7f
[appveyor]: https://github.com/dannyedel/dspdfviewer/blob/master/.appveyor.yml
