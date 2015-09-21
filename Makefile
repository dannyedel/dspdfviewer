
RELEASE=v1.13.1

.PHONY: manpage.html

manpage.html:
	( cd ../dspdfviewer && \
	git show master:docs/dspdfviewer.1 ) \
	| groff -mandoc -T html > $@

manpage-release.html:
	( cd ../dspdfviewer && \
	git show $(RELEASE):docs/dspdfviewer.1 ) \
	| groff -mandoc -T html > $@
