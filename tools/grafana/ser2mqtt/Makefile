PREFIX = /usr/local

SYSD = $(wildcard *.service)
PRGS = $(SYSD:.service=.py)
DSTS = $(SYSD:.service=)
NAME = $(DSTS:adcs-=)

all: ser2mqtt mqttserial

$(DSTS): $(PRGS) $(SYSD)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $@.py $(DESTDIR)$(PREFIX)/bin/$@
	cp $@.service /etc/systemd/system/
	systemctl daemon-reload
	systemctl enable $@
	systemctl restart $@

mqttserial:
	cp $@.sh $(DESTDIR)$(PREFIX)/bin/$@

.PHONY: all $(DSTS)
