---
title: Installation on Debian
layout: default
---

If you're running Debian, it is recommended to use the version included in your
distribution. Depending on which Debian release you have, installing `dspdfviewer` is
a little bit different.

## Debian stretch and newer (incl. sid)

`dspdfviewer` is already included on the official mirrors. Simply call
<span class="root">`apt-get install dspdfviewer`</span> and you're ready to go.

## Debian jessie

`dspdfviewer` is included in `jessie-backports`.  Information on what
`backports` is and how to use it is found on [backports instructions],
but to get started, try these commands (make sure to replace
*ftp.de.debian.org* with your preferred debian mirror!)

<div class="root">
{% highlight bash %}
# Activate jessie-backports
tee -a /etc/apt/sources.list.d/jessie-backports.list <<< "deb http://ftp.de.debian.org/debian jessie-backports main"
apt-get update

# Install dspdfviewer
apt-get install dspdfviewer
{% endhighlight %}
</div>

<span class="root">
If you already had jessie-backports activated, a
simple `apt-get install dspdfviewer` is enough.
</span>


[backports instructions]: http://backports.debian.org/Instructions/

## Debian wheezy

[@dannyedel] maintains a third-party deb repository for wheezy.
If you want to use it, please follow the instructions in [deb-repository].


## Versions overview

To see which versions are available in Debian, please take a look at the
[package tracker].

## Advanced users: Follow the development version

If you want to follow the current development version, take a look at the
[daily debs] for prebuilt binaries.

[package tracker]: https://tracker.debian.org/pkg/dspdfviewer
[deb-repository]: /installation/deb-repository.html
[daily debs]: /installation/daily-deb.html
[@dannyedel]: https://github.com/dannyedel
