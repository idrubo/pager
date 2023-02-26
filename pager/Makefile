CFLAGS := -Wall -g -O2

pager := pager
mPager := mPager

all: $(pager) $(mPager)

$(pager):
$(mPager): $(mPager).c memory.c memory.h
	$(CC) $(CFLAGS) $< memory.c -o $@

.PHONY: clean

clean:
	rm -f core
	rm -f $(pager)
	rm -f $(mPager)

