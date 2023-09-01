CONFIG = webdogcom.conf
TOPDIR = $(CURDIR)
TARGET = webdogcom

export TOPDIR

all: $(CONFIG)
	$(MAKE) -C src

$(CONFIG):
	cp webdogcom_sample.conf $(CONFIG)

clean:
	$(MAKE) -C src clean