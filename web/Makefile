XSLTPROC = xsltproc --xinclude
SERVER = mars@blipbox.org:/var/www/vhosts/www.blipbox.org/
SERVER = mars@artichaut.pingdynasty.com:/var/www/vhosts/www.blipbox.org/
SERVER = mars@192.168.1.77:/srv/www/www.blipbox.org/
TARGETS = blipbox.html blipbox-functionality.html blipbox-order.html contact.html taquito.html polynome.html about.html

all: $(TARGETS)
	cp blipbox.html index.html

%.html: %.xml sidebar.xml web.xsl
	$(XSLTPROC) web.xsl $*.xml > $@

upload:
	rsync -rav -e ssh *.html images $(SERVER)

clean:
	rm -f $(TARGETS)
