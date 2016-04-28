private := $(shell if [ -d "private" ]; then echo "private"; else echo ""; fi;)

all:
	make -C gpl
ifneq ($(strip $(private)),)
	make -C private
endif

clean:
	make -C gpl clean
ifneq ($(strip $(private)),)
	make -C private clean
endif
