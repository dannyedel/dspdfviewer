---
title: Installation on Ubuntu
layout: default
---

## Installation from release binaries

Add the repository `ppa:dannyedel/dspdfviewer` to your system source list
and install from it.

You can do this by running

{% highlight bash %}
sudo add-apt-repository ppa:dannyedel/dspdfviewer
# the system will ask for your password and confirmation
sudo apt-get update
sudo apt-get install dspdfviewer
{% endhighlight %}

## Installation from development (daily) binaries

If you want to help development by testing new features, please
use the `ppa:dannyedel/dspdfviewer-daily` package source.

You can do this by running

{% highlight bash %}
sudo add-apt-repository ppa:dannyedel/dspdfviewer-daily
# the system will ask for your password and confirmation
sudo apt-get update
sudo apt-get install dspdfviewer
{% endhighlight %}


## Installation from sourcecode

Compiling from source is identical to
[compiling from source on debian][debian], please
read the instructions over there.

[debian]: installation/debian.html#installation-from-source
