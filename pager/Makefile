CFLAGS := -Wall -g -O2

mPager := mPager

all: $(mPager)

$(mPager): $(mPager).c memory.c memory.h
	$(CC) $(CFLAGS) $< memory.c -o $@

.PHONY: clean

clean:
	rm -f core
	rm -f $(mPager)

