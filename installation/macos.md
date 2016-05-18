---
title: Installation on MacOS
layout: default
---

## Homebrew (recommended)
If you use a mac with Homebrew, there already is a formula to install
dspdfviewer

{% highlight bash %}
brew install dspdfviewer
{% endhighlight %}

The previous command installs the latest release. If you want to test out the
newest features, provide the head option:

{% highlight bash %}
brew install dspdfviewer --HEAD
{% endhighlight %}

## Manually (advanced users)

If you want to compile from source, please read the
[generic install instructions](/installation/) about
the available cmake-time switches.

If you installed Qt5 via Homebrew, you will have
to pass its installation prefix to `cmake`:

```bash
cmake /path/to/source -DSomeOption=ON -DCMAKE_PREFIX_PATH=$(brew --prefix qt5)
```
