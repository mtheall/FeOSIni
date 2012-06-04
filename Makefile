export THIS_MAKEFILE := Makefile.FeOS

all: source/parse.c include/parse.h source/lex.c
	@make -f Makefile.FeOS

source/lex.c : lex.l
	@flex -L -o $@ $<

source/parse.c include/parse.h: parse.y
	@bison -l -o source/parse.c --defines=include/parse.h $<

clean:
	@make -f Makefile.FeOS clean

install: all
	@cp ini.fx2 $(FEOSDEST)/data/FeOS/lib/ini.fx2 || exit 1
