MODULES=$(notdir $(wildcard src/*))

default: $(MODULES)

%: src/% Makefile
	gcc -g -Wall -Wno-unused-function -o $@ $</*.c

clean:
	rm -f $(MODULES) src/*/*.o
