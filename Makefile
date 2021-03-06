default: debug

# Compiler:
CC := gcc

# GNU Make dependecies
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/

# Dependency information directory and flags
DEPDIR = .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

DBGFLAGS = -Wall -D DEBUG -g
RELFLAGS = -D NDEBUG -g
CFLAGS = -O2 -std=c99


# List of .c and .o files:
C_FILES = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ_FILES = $(addprefix obj/, $(notdir $(C_FILES:.c=.o)))

debug: CFLAGS += $(DBGFLAGS)
debug: bin/mini-shell

release: CFLAGS += $(RELFLAGS)
release: bin/mini-shell

# Compile command includes gcc flags for dependency generation:
COMPILE = $(CC) $(DEPFLAGS) $(CFLAGS) -c

# Move temporary generated dependecies to permanent location post-compilation:
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

test: debug
	echo "echo Hello | rev" | ./bin/mini-shell

run: debug
	./bin/mini-shell

# Executable depends on all .o files:
bin/mini-shell: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES)

obj/%.o: src/%.c
obj/%.o: src/%.c $(wildcard src/*.h) $(DEPDIR)/%.d
	$(COMPILE) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

obj/%.o: src/*/%.c
obj/%.o: src/*/%.c $(wildcard src/*/*.h) $(DEPDIR)/%.d
	#$(CC) -c -Wall $< -o $@
	$(COMPILE) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

clean:
	rm -rf bin
	rm -rf obj
	mkdir bin
	mkdir obj

valgrind: debug
	valgrind --tool=memcheck --leak-check=full ./bin/mini-shell

.PHONY: clean run debug release valgrind test

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d bin/mini-shell

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
