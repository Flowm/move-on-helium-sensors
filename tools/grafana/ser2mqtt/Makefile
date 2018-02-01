PREFIX = /usr/local

SYSD = $(wildcard *.service)
PRGS = $(SYSD:.service=.py)
DSTS = $(SYSD:.service=)
NAME = $(DSTS:adcs-=)

install: ser2mqtt
	cp mqttserial.sh $(DESTDIR)$(PREFIX)/bin/mqttserial
	cp serial_wrap.py $(DESTDIR)$(PREFIX)/bin/serial_wrap.py

$(DSTS): $(PRGS) $(SYSD)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $@.py $(DESTDIR)$(PREFIX)/bin/$@
	cp $@.service /etc/systemd/system/
	systemctl daemon-reload
	systemctl enable $@
	systemctl restart $@



.PHONY: install $(DSTS)
