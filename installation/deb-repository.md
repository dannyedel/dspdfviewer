---
title: Installation on Debian wheezy release
layout: default
---

[Go back to debian installation](/installation/debian.html)

-----

If you are using Debian wheezy, please consider that it is now
`oldstable` and will therefore stop receiving security updates soon.

Please consider upgrading to the current stable release, especially
on a desktop system.

-----

That said, I (Danny Edel <mail@danny-edel.de>) maintain a
third-party repository to install `dspdfviewer` on wheezy.

Although this not required, you are encouraged to send an e-mail to me
if you use the repository, to show there is an active userbase.

Please follow these steps to add my repository to your system sources.
Once you have done that, you will get updates to dspdfviewer using
<span class="root">
standard `apt-get update && apt-get upgrade` calls.
</span>

The packages are built by [@dannyedel] and signed with his GPG key
[0x7183343C].

[@dannyedel]: https://github.com/dannyedel
[0x7183343C]: https://sks-keyservers.net/pks/lookup?op=get&search=0xF132F84C7183343C

### Initial setup (once)

These initial steps will *require root permissions.* Depending on your system,
either `su -c 'command --arguments'` or `sudo command --arguments`
will run a command as root. An interactive root shell (note that this will change working
directory to root's home) can be opened with `su -` or `sudo -i`.

The main problem is that until you have installed my gpg key, apt will tell you
(and rightfully so), that it cannot verify the integrity of my packages.
This will be remedied *after* you install my keying *and* update package sources.

Apt will obviously ask you if you want to continue without authentication.

<div class="root">
{% highlight bash %}
echo 'deb http://danny-edel.de/deb wheezy main' >> /etc/apt/sources.list.d/danny-edel.list
echo 'deb-src http://danny-edel.de/deb wheezy main' >> /etc/apt/sources.list.d/danny-edel.list
apt-get update    # note: apt-get will complain about the repository signing key
apt-get install danny-edel-keyring    # note: apt-get will complain again
apt-get update    # Now all is good.
apt-get install dspdfviewer
{% endhighlight %}
</div>
