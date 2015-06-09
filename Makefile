MODULES=$(notdir $(wildcard src/*))

default: $(MODULES)

%: src/%
	gcc -o $@ $</*.c

clean:
	rm -f $(MODULES) src/*/*.o
