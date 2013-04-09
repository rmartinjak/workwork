DESTDIR  ?= ~

CPPFLAGS ?=
CFLAGS   ?= -Wall -Wextra -std=c89 -O2
LDFLAGS  ?=
LIBS     ?=

PINGCMD  ?= workwork
STARTCMD ?= readytowork
STOPCMD  ?= jobsdone
CMDS := -DPINGCMD=\"$(PINGCMD)\" -DSTARTCMD=\"$(STARTCMD)\" -DSTOPCMD=\"$(STOPCMD)\"
CMDS += -DWORKING=\'+\' -DNOTWORKING=\'-\'

all: $(PINGCMD) $(STARTCMD) $(STOPCMD)

$(PINGCMD): workwork.c
	@$(CC) $(CMDS) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

$(STARTCMD) $(STOPCMD): $(PINGCMD)
	@ln -s $^ $@

clean:
	@rm -f $(PINGCMD) $(STARTCMD) $(STOPCMD)

install: $(PINGCMD)
	@install -m 755 $(PINGCMD) $(DESTDIR)/bin
	@ln -sf $(PINGCMD) $(DESTDIR)/bin/$(STARTCMD)
	@ln -sf $(PINGCMD) $(DESTDIR)/bin/$(STOPCMD)

uninstall:
	@rm -f $(DESTDIR)/bin/$(PINGCMD)
	@rm -f $(DESTDIR)/bin/$(STARTCMD)
	@rm -f $(DESTDIR)/bin/$(STOPCMD)

.PHONY: clean install uninstall
