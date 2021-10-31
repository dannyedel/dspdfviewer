---
title: Installation on Debian Jessie
---

[Back to Debian overview][debian]

`dspdfviewer` is included in `jessie-backports`.  Information on what
`backports` is and how to use it is found on [backports instructions],
but to get started, try these commands (make sure to replace
*deb.debian.org* with your preferred debian mirror!)

As a side effect, you can now access more recent versions.

<div class="root">
{% highlight bash %}
# Activate jessie-backports repository
tee -a /etc/apt/sources.list.d/jessie-backports.list <<< "deb http://deb.debian.org/debian jessie-backports main"
# ... and reload the list of available packages
apt-get update

# Install dspdfviewer
apt-get install -t jessie-backports dspdfviewer
{% endhighlight %}
</div>

<span class="root">
If you already had jessie-backports activated, a
simple `apt-get install dspdfviewer` is enough.
</span>

[debian]: /installation/debian.html
[backports instructions]: http://backports.debian.org/Instructions/
