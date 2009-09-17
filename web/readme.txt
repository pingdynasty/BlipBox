to generate web pages using the Makefile (which uses xsltproc):
cd blipbox/web
make clean all

to upload (publish) to the live server:
make upload

links are kept in sidebar.xml and blipbox-menubar.xml

when adding a page, make sure to add it to the targets in the Makefile 
