AT ?= @
dirs := $(shell find . -maxdepth 1 -type d)
dirs := $(filter-out ., $(patsubst ./%,%,$(dirs)))
dirs := $(filter-out $(exclude_dirs),$(dirs))
dirs_mk := $(addsuffix ._mk_,$(dirs) )
dirs_clean := $(addsuffix ._clean_,$(dirs) )
dirs_install := $(addsuffix ._install_,$(dirs) )

all: $(dirs_mk)
clean: $(dirs_clean)
install: $(dirs_install)

%._mk_:%
	$(AT)echo Building dir $< .....
	$(AT)$(MAKE) -C $<

%._clean_:%
	$(AT)echo Cleanning dir $< .....
	$(AT)$(MAKE) -C $< clean

%._install_:%
	$(AT)echo Installing dir $< .....
	$(AT)$(MAKE) -C $< install

.PHONY: $(dir_mk) $(dir_clean) $(dir_install)
