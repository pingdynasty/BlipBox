XSLTPROC = xsltproc --xinclude

TARGETS = polynome.html blipbox.html contact.html taquito.html blipbox-functionality.html about.html
all: $(TARGETS)
	cp blipbox.html index.html

%.html: %.xml sidebar.xml web.xsl
	$(XSLTPROC) web.xsl $*.xml > $@

clean:
	rm -f $(TARGETS)
