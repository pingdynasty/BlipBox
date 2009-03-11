XSLTPROC = xsltproc --xinclude

TARGETS = polynome.html blipbox.html contact.html taquito.html blipbox-functionality.html about.html
all: $(TARGETS)
	cp blipbox.html index.html

%.html: %.xml sidebar.xml web.xsl
	$(XSLTPROC) web.xsl $*.xml > $@

upload:
	rsync -rav -e ssh *.html images mars@blipbox.org:/var/www/vhosts/www.blipbox.org/

clean:
	rm -f $(TARGETS)
