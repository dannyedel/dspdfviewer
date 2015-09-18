---
title: Installation on Ubuntu
layout: default
---

## Ubuntu wily and newer

`dspdfviewer` is included in wily and newer. Simply call <span class="root">
`apt-get install dspdfviewer`</span>.

## Ubuntu Precise LTS and Trusty LTS

Add the repository `ppa:dannyedel/dspdfviewer` to your system source list
and install from it.

You can do this by running

{% highlight bash %}
sudo add-apt-repository ppa:dannyedel/dspdfviewer
# the system will ask for your password and confirmation
sudo apt-get update
sudo apt-get install dspdfviewer
{% endhighlight %}

## Advanced users: Follow the development version

If you want to help development by testing new features, please
use the `ppa:dannyedel/dspdfviewer-daily` package source.

You can do this by running

{% highlight bash %}
sudo add-apt-repository ppa:dannyedel/dspdfviewer-daily
# the system will ask for your password and confirmation
sudo apt-get update
sudo apt-get install dspdfviewer
{% endhighlight %}
