CTL_DIR=${CURDIR}
export CTL_DIR
#export DIR_RSDK=/home/frank/Project/WECB/rtl819x/toolchain/rsdk-1.3.6-5281-EB-2.6.30-0.9.30
private := $(shell if [ -d "libs/private" ]; then echo "private"; else echo ""; fi;)

revj:
	./target/revj/config_revj.sh
	make -C libs 
	make -C apps
	cd ./target/revj/ && ./install.sh

x86:
	./target/x86/config_x86.sh
	make -C libs 
	make -C apps
	cd ./target/x86/ && ./install.sh

wecb_mk_lib:
	./target/wecb/config_wecb.sh
	make -C libs 

wecb_mk_app:
	make -C apps
ifneq ($(strip $(private)),)
	make -C libs/private/framework/tr69_core mk_app
	make -C libs/private/wifi mk_app
endif
	cd ./target/wecb/ && ./install.sh

wecbarm:
	./target/wecbarm/config_wecb.sh
	make -C libs 
	make -C apps
	cd ./target/wecbarm/ && ./install.sh

clean:
# make clean PLAT=xxx
# e.g. make clean PLAT=revj
ifeq ($(PLAT),)
	@echo " "
	@echo "        Please use make clean like this :"
	@echo " "
	@echo "                \"make clean PLAT=yourplatform\""
	@echo " "
	@echo "                 e.g. make clean PLAT=revj"
	@echo " "
	@echo "        NOTE: \"yourplatform\" are all in lowercase."
	@echo " "
else
	./target/${PLAT}/config_${PLAT}.sh
	make -C libs clean 
	make -C apps clean 
	rm -f ./target/${PLAT}/lib/*
	rm -f ./target/${PLAT}/bin/*
	rm -f global.def
endif
