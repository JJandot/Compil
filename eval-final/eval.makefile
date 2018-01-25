COMPILO=/home/alexis/gitlab/compilationl3/supports/TP/src/compilo-ext
COMPILO_OPT=-n

NASM=nasm
NASM_OPT=-f elf -g -F dwarf

LD=ld
LD_OPT=-m elf_i386

INPUT_DIR=./input
OUTPUT_DIR=./output

.SECONDARY: # pour ne pas effacer les fichiers intermediaires

all: $(OUTPUT_DIR)/affect.exe \
$(OUTPUT_DIR)/boucle.exe \
$(OUTPUT_DIR)/expression.exe \
$(OUTPUT_DIR)/max.exe \
$(OUTPUT_DIR)/tri.exe \
$(OUTPUT_DIR)/fibo.exe \
$(OUTPUT_DIR)/imbrique.exe \
$(OUTPUT_DIR)/condexp.exe \
$(OUTPUT_DIR)/procedure_arg.exe \
$(OUTPUT_DIR)/procedure_retour.exe \
$(OUTPUT_DIR)/associativite.exe \
$(OUTPUT_DIR)/expression.exe \
$(OUTPUT_DIR)/pgcd.exe \
$(OUTPUT_DIR)/procedure.exe \
$(OUTPUT_DIR)/procedure_varloc.exe \
$(OUTPUT_DIR)/semantique1.exe \
$(OUTPUT_DIR)/semantique2.exe \
$(OUTPUT_DIR)/semantique3.exe \
$(OUTPUT_DIR)/semantique4.exe \
$(OUTPUT_DIR)/semantique5.exe \
$(OUTPUT_DIR)/semantique6.exe

$(OUTPUT_DIR)/%.asm : $(INPUT_DIR)/%.l
	$(COMPILO) $(COMPILO_OPT) $< > $@

$(OUTPUT_DIR)/%.o : $(OUTPUT_DIR)/%.asm
	$(NASM) $(NASM_OPT) $< > $@

$(OUTPUT_DIR)/%.exe : $(OUTPUT_DIR)/%.o
	$(LD) $(LD_OPT) $< -o $@

clean:
	-rm $(OUTPUT_DIR)/*.asm
	-rm $(OUTPUT_DIR)/*.o
	-rm $(OUTPUT_DIR)/*.exe

