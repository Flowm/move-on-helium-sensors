PREFIX = /usr/local

PRGS = $(wildcard *.sh)
DSTS = $(PRGS:.sh=)

install: $(DSTS)

$(DSTS): $(PRGS)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $@.sh $(DESTDIR)$(PREFIX)/bin/$@

.PHONY: install $(DSTS)
