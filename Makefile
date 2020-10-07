
export ARCH ?= arm
export CROSS_COMPILE ?= /opt/arm-rpi4/bin/arm-linux-

#export ARCH ?= x86

all:
	@make -C libmpshm
	@make -C examples
	#ssh -l pi rpi -q -t "sudo pkill -9 example_*"
	@scp -r target pi@rpi:/home/pi/

clean:
	@rm -f target/*
	@make -C libmpshm clean
	@make -C examples clean
