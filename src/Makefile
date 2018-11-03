TARGET=org-tangle
OBJS:= main.o block.o reference.o parser.o block-fsm.o block-config-fsm.o ref-fsm.o

all: dep $(TARGET)

$(TARGET): $(OBJS)
	gcc $(OBJS) -o $@

%.o:%.c
	gcc -g -c -Wall -O3 $< -o $@

block-fsm.c: ../script/block-fsm.txt
	fsm-generator.py $< -d . --prefix block --style table

block-config-fsm.c: ../script/block-config-fsm.txt
	fsm-generator.py $< -d . --prefix block_config --style table

ref-fsm.c: ../script/ref-fsm.txt
	fsm-generator.py $< -d . --prefix reference --style table

clean:
	rm -rf *.o *.d
	rm -rf *-fsm.*
	rm org-tangle

dep:


%.d : %.c
	rm -f $@;
	gcc -MM $< > $@;
	sed -i 's,/($*/)/.o[ :]*,/1.o $@ : ,g' $@;

sinclude $(OBJS:.o=.d)

.PHONY: all clean
