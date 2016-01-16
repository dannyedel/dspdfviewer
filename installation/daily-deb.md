---
title: Installation of daily debs, directly from git
---


If you want to stay up-to-date from git (for example because you want to help test features
in development), but do not feel comfortable compiling from source yourself,
you can install the daily debs that my [jenkins] generates and signs for you.

Jenkins' daily packages are signed with gpg key [0x61E9E242],
and they are *only* available for the `amd64` architectures of
`jessie`, `sid`, `trusty` and `xenial`.

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

### Unsupported Repository

This is an ***unsupported third-party repository*** reflecting current git status.
You can not expect the same quality as from a "release" repository,
but feedback on the not-yet-released functions can be very useful.

Please **do not** report issues to the official Debian bug tracking system.

Please send any feedback directly to the [github issues] system.

[github issues]: https://github.com/dannyedel/dspdfviewer/issues

### Upgrading

Once the package repository is set up, your normal
<span class="root">`apt-get update && apt-get upgrade`</span>
will include updates to dspdfviewer.
This also means that using the (even graphical) apt frontend
of your choosing will see and install updates.

You can swap back to the release version any time,
but note that since apt doesn't do *down*grades by default,
it is recommended that you run
<span class="root">`apt-get remove dspdfviewer && apt-get install dspdfviewer`</span>
after you disable the repository to go back to the release version.
