FILE=main
OLD=old
CUR=cur
MOVS=movs
CUSTOM=custom
LIB=libLab2.c
FLAGS=-O0 


all: $(OLD) $(CUR) $(MOVS) $(CUSTOM) 

$(OLD):
	gcc  $(FLAGS) -DOLD_MEMCPY -o $@ $(LIB) $(FILE).c

$(CUR):
	gcc  $(FLAGS) -DCUR_MEMCPY -o $@ $(LIB) $(FILE).c

$(MOVS):
	gcc  $(FLAGS) -DMOVS_MEMCPY -o $@ $(LIB) $(FILE).c

$(CUSTOM):
	gcc  $(FLAGS) -DCUSTOM_MEMCPY -o $@ $(LIB) $(FILE).c

clean:
	rm $(OLD) $(CUR) $(MOVS) $(CUSTOM) *.o ./log_*
