PREFIX = /usr/local

PRGS = $(wildcard *.cpp)
DSTS = $(PRGS:.cpp=)

install: $(DSTS)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/

.PHONY: install
