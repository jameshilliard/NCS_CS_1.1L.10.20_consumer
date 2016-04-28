SCSH = @SCSH@

EXTRA_DIST=make_class.in

bin_SCRIPTS=make_class

all: make_class

make_class: make_class.in Makefile
	sed -e "s,_SCSH_,$(SCSH)," make_class.in >make_class
	chmod +x make_class
