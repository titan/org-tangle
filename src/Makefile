TARGET := org-tangle
OBJS := main.o parser.o block.o reference.o block-fsm-actions.o block-fsm.o block-config-fsm-actions.o block-config-fsm.o ref-fsm-actions.o ref-fsm.o

all: dep $(TARGET)

$(TARGET): $(OBJS)
	gcc $(OBJS) -o $@

%.o:%.c
	gcc -g -c -Wall -O3 $< -o $@

block-fsm.c block-fsm.h: ../script/block-fsm.txt
	naive-fsm-generator.py $< -d . --prefix block --style table

block-config-fsm.c block-config-fsm.h: ../script/block-config-fsm.txt
	naive-fsm-generator.py $< -d . --prefix block_config --style table

ref-fsm.c ref-fsm.h: ../script/ref-fsm.txt
	naive-fsm-generator.py $< -d . --prefix reference --style table

clean:
	rm -rf *.o *.d
	rm -rf *-fsm.*
	rm -rf $(TARGET)

dep:

%.d : %.c
	rm -f $@;
	gcc -MM $< > $@;
	sed -i 's,/($*/)/.o[ :]*,/1.o $@ : ,g' $@;

sinclude $(OBJS:.o=.d)

.PHONY: all clean
