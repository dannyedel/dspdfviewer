---
title: Compile-time Dependencies
---

You will need a c++11 compiler, cmake, and the
"development" editions of the libraries used (boost, poppler, qt5).

The exact names of those packages will vary based on your operating system
and/or distribution, but development versions often end in `-dev`,
`-headers` or similar.

If your distribution does not yet have a package for `dspdfviewer`,
you might want to consider packaging it,
so that in the future, you (and other users of your distribution)
can simply install the package.

## Debian and Ubuntu based systems

The following package list works on current debian-based systems:

{% highlight bash %}
sudo apt install libboost-program-options-dev libboost-test-dev \
     libpoppler-qt5-dev qtbase5-dev qttools5-dev qttools5-dev-tools
{% endhighlight %}

## Fedora

Fedora dependencies, thanks to [LorhanSohaky](https://github.com/LorhanSohaky):

{% highlight bash %}
sudo dnf install cmake boost-devel poppler-qt5-devel qt5-qtbase-devel qt5-linguist
{% endhighlight %}
