---
title: Installation and compilation from source
layout: default
---

You can choose if you want to install the bleeding-edge development version
(may eat your firstborn, plain out crash, or do other unexpected things) or the
last stable release.

## Verifying release checksums

Especially if you're including the package in a distritbution, you should
include some kind of checksum verification, based on the git tag I signed
with my gpg key.

Please read my [reproducing checksum] article for further details.

[reproducing checksum]: http://blog.danny-edel.de/2015/07/31/reproducing-github-tarball-checksum/

## Stable release

Note: At the time of writing, the current release is *v1.12*. You can check at
https://github.com/dannyedel/dspdfviewer/releases
if a newer version was released since then.
	Please adapt the version numbers as needed.

{% highlight bash %}
curl -L https://github.com/dannyedel/dspdfviewer/archive/v1.12.tar.gz | tar xzv
cd dspdfviewer-1.12
mkdir build
cd build
cmake ..
make
make install
{% endhighlight %}

Note that there is no uninstall script, so please consider replacing the final
`make install` with something more appropriate for your distribution.
It will respect the usual DESTDIR parameter.

## Current development (git)

You will need a c++11 compiler (for example `g++` or `clang`), `cmake`,
the `boost` and `poppler-qt4` libraries installed.

### Initial installation

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

### Updating

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
