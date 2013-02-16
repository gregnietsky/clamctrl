PREFIX=/usr
BINDIR=$(PREFIX)/bin

CC=gcc
INSTALL=ginstall

PROGNAME=clamctrl
LIBS=


all:	$(PROGNAME)
distclean:	clean

clean:
	rm $(PROGNAME) || true

install: all
	$(INSTALL) -D $(PROGNAME) $(DESTDIR)$(BINDIR)/$(PROGNAME)

$(PROGNAME):
	$(CC) $(PROGNAME).c -o $(PROGNAME) $(LIBS)
