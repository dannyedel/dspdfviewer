---
title: Installation and compilation from source
layout: default
---

You can choose if you want to install the bleeding-edge development version
(may eat your firstborn, plain out crash, or do other unexpected things) or the
last stable release.

## Latest release

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

(once) Clone the current development branch and create a build directory.

{% highlight bash %}
git clone git://github.com/dannyedel/dspdfviewer.git
cd dspdfviewer
mkdir build
cd build
cmake ..
make
{% endhighlight %}

From now on, you can pull updates with `git pull` and build with `make`.
You can install with `make install` (this will require root permission).
Note that there is *no* uninstallation, so please consider packaging
the software using your distributions native packaging method.
