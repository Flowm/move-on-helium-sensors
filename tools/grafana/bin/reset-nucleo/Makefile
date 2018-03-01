PREFIX = /usr/local

PRGS = $(wildcard *.py)
DSTS = $(PRGS:.py=)

install: $(DSTS)

$(DSTS): $(PRGS)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $@.py $(DESTDIR)$(PREFIX)/bin/$@

.PHONY: install $(DSTS)
