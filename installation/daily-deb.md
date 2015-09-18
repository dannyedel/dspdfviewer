---
title: Installation of daily debs, directly from git
---


If you want to stay up-to-date from git (for example because you want to help test features
in development), you can install the daily debs that my [jenkins] generates and signs for you.

Jenkins' packages are signed with gpg key [0x61E9E242].

<div class="root">
{% highlight bash %}
echo 'deb http://jenkins.danny-edel.de/jenkins-deb dspdfviewer-YOURDISTRIBUTION main' >> /etc/apt/sources.list.d/jenkins.list
apt-key adv --keyserver hkp://hkps.pool.sks-keyservers.net --recv-keys 0xC86A1F0861E9E242
apt-get update
apt-get install dspdfviewer
{% endhighlight %}
</div>

[jenkins]: http://danny-edel.de/jenkins
[0x61E9E242]: https://sks-keyservers.net/pks/lookup?op=get&search=0xC86A1F0861E9E242

### Upgrading

Since the package repository is set up, your normal `apt-get update && apt-get upgrade` will
include updates to dspdfviewer. This also means that using the (even graphical) apt frontend
of your choosing will see and install updates.

You can swap between normal and daily repositories, but note that since apt doesn't do *down*grades by
default, you will either have to wait for the next release when swapping "down" from daily
to release, or do an uninstallation followed by a fresh download of dspdfviewer.
