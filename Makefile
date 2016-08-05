default: bin/mini-shell

# Compiler:
CC := gcc

# Dependency information directory and flags
DEPDIR = .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

# Compile command includes gcc flags for dependency generation:
COMPILE = $(CC) $(DEPFLAGS) $(CFLAGS) -c

# Move temporary generated dependecies to permanent location post-compilation:
POSTCOMPILE = mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d

# List of .c and .o files:
C_FILES = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ_FILES = $(addprefix obj/, $(notdir $(C_FILES:.c=.o)))

run: bin/mini-shell
	./bin/mini-shell

# Executable depends on all .o files:
bin/mini-shell: $(OBJ_FILES)
	$(CC) -O2 -Wall -o $@ $(OBJ_FILES)

obj/%.o: src/%.c
obj/%.o: src/%.c $(DEPDIR)/%.d
	#$(CC) -c -Wall $< -o $@
	$(COMPILE) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

obj/%.o: src/*/%.c
obj/%.o: src/*/%.c $(DEPDIR)/%.d
	#$(CC) -c -Wall $< -o $@
	$(COMPILE) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

# Can be used to print variable: "make print-COMPILE"
print-%  : ; @echo $* = $($*)

clean:
	rm -rf bin
	rm -rf obj
	mkdir bin
	mkdir obj

.PHONY: clean run

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d bin/mini-shell

-include $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS)))
