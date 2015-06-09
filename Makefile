MODULES=\
		01-basic-module

default: $(MODULES)

%: src/%
	gcc -o $@ $</*.c
