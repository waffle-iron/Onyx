MODULE:= dummyeth
MOD_OBJS:=$(wildcard *.c)
MODDIR:=/lib/modules
all: $(MODULE)
	mkdir -p $(DESTDIR)$(MODDIR)/
	cp $(MODULE).kmod $(DESTDIR)$(MODDIR)/$(MODULE).kmod
$(MODULE): $(MOD_OBJS)
	echo [LD] $@.kmod
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@.kmod
clean:
	rm $(MODULE).kmod
