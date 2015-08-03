
.PHONY: manpage.html

manpage.html:
	( cd ../dspdfviewer && \
	git show master:docs/dspdfviewer.1 ) \
	| groff -mandoc -T html > $@

manpage-release.html:
	( cd ../dspdfviewer && \
	git show v1.13:docs/dspdfviewer.1 ) \
	| groff -mandoc -T html > $@
