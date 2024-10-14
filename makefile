PROJECT_NAME:= turtix.out
CC			:= g++ --std=c++11
SRCDIR		:= src
INCDIR		:= inc
BUILDDIR	:= build
BINDIR		:= bin
EXE_FILE	:= $(BINDIR)/$(PROJECT_NAME)
SOURCES		:= $(shell find $(SRCDIR) -type f -name *.c*)
HEDEARS		:= $(shell find $(INCDIR) -type f -name *.h*)
OBJECTS		:= $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(addsuffix .o,$(basename $(SOURCES))))
LIB			:= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
INC			:= -I inc

$(EXE_FILE): $(BUILDDIR) $(BINDIR) $(OBJECTS)
	$(CC) -o $(EXE_FILE) $(OBJECTS) $(LIB)

$(BUILDDIR) :
	mkdir $(BUILDDIR)

$(BINDIR):
	mkdir $(BINDIR)
	
$(BUILDDIR)/%.o: $(SRCDIR)/%.c* $(INCDIR)/%.h*
	@mkdir -p $(dir $@)
	$(CC) $(INC) -c $< -o $@
	@$(CC) $(INC) -M $< -MT $@ > $(@:.o=.td)
	@cp $(@:.o=.td) $(@:.o=.d); 
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.td) >> $(@:.o=.d); 
	@rm -f $(@:.o=.td)

clean:
	rm -r $(BUILDDIR) $(BINDIR)