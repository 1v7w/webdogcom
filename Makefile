CONFIG = webdogcom.conf

all: $(CONFIG)
	$(MAKE) -C src

$(CONFIG):
	cp webdogcom_sample.conf $(CONFIG)

clean:
	$(MAKE) -C src clean
