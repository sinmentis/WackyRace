APPDIRS = $(wildcard wacky-apps test-apps)
MAKEDIRS = $(addsuffix .all, $(APPDIRS))
CLEANDIRS = $(addsuffix .clean, $(APPDIRS))

all:: $(MAKEDIRS)

clean:: $(CLEANDIRS)

.PHONY: $(MAKEDIRS)
$(MAKEDIRS):
	-@$(MAKE) -C $(subst .all,,$@)

.PHONY: $(CLEANDIRS)
$(CLEANDIRS):
	-@$(MAKE) -C $(subst .clean,,$@) clean
