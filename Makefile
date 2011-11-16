export THIS_MAKEFILE := Makefile.FeOS

all: source/parse.c include/parse.h source/lex.c
	@make -f Makefile.FeOS

source/lex.c : lex.l
	@flex -L -o $@ $<

source/parse.c include/parse.h: parse.y
	@bison -l -o source/parse.c --defines=include/parse.h $<

clean:
	@rm -f source/parse.c include/parse.h source/lex.c
	@make -f Makefile.FeOS clean

ifneq ($(strip $(DEST)),)
install: all
	@cp ini.fx2   $(DEST)/data/FeOS/bin/ini.fx2   || exit 1
	@cp ini.imp.a $(DEST)/data/FeOS/lib/ini.imp.a || exit 1
endif
