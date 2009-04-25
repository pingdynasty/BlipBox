XSLTPROC = xsltproc --xinclude

TARGETS = blipbox.html blipbox-functionality.html blipbox-order.html contact.html taquito.html polynome.html about.html
all: $(TARGETS)
	cp blipbox.html index.html

%.html: %.xml sidebar.xml web.xsl
	$(XSLTPROC) web.xsl $*.xml > $@

upload:
	rsync -rav -e ssh *.html images mars@blipbox.org:/var/www/vhosts/www.blipbox.org/

clean:
	rm -f $(TARGETS)
