---
title: Installation on Trusty and Precise via PPA
---

[Back to Ubuntu installation](/installation/ubuntu.html)

Add the repository `ppa:dannyedel/dspdfviewer` to your system source list
and install from it.

You can do this by running

{% highlight bash %}
sudo add-apt-repository ppa:dannyedel/dspdfviewer
# the system will ask for your password and confirmation
sudo apt-get update
sudo apt-get install dspdfviewer
{% endhighlight %}

## Versions overview

To see which version of `dspdfviewer` is included in which Ubuntu release,
check out the following pages:

* [Ubuntu official distributions][launchpad]
* PPAs for...
  * [precise][precise]
  * [trusty][trusty]

[launchpad]: https://launchpad.net/ubuntu/+source/dspdfviewer
[precise]: https://launchpad.net/~dannyedel/+archive/ubuntu/dspdfviewer?field.series_filter=precise
[trusty]: https://launchpad.net/~dannyedel/+archive/ubuntu/dspdfviewer?field.series_filter=trusty
