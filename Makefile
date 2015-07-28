
.PHONY: manpage.html

manpage.html:
	( cd ../dspdfviewer && \
	git show master:docs/dspdfviewer.1 ) \
	| groff -mandoc -T html > $@
