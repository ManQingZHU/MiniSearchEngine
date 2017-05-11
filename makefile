CC = g++ -std=c++11
DEBUG = -g
LFLAGS = -Wall $(DEBUG) -o

SOURCE = *.cpp *.h makefile *.txt documents ReadMe $(EXE_1)
NAME = SearchEngine

EXE_1 = SearchEngine_iox
EXE_1_OBJ = main.o InvFileInd.o PreciseSearch.o RoughSearch.o Stemming.o 

EXES = $(EXE_1)  

OBJ_1 = main.o
OBJ_1_HEAD = InvFileInd.h PreciseSearch.h RoughSearch.h Stemming.h
OBJ_2 = InvFileInd.o 
OBJ_2_HEAD = InvFileInd.h Stemming.h
OBJ_3 = PreciseSearch.o
OBJ_3_HEAD = PreciseSearch.h
OBJ_4 = RoughSearch.o
OBJ_4_HEAD = RoughSearch.h
OBJ_5 = Stemming.o
OBJ_5_HEAD = Stemming.h

all : $(EXES)
.PHONY: all

$(EXE_1) : $(EXE_1_OBJ)
	$(CC) $(LFLAGS) $(EXE_1) $(EXE_1_OBJ)
	
$(OBJ_1) : $(OBJ_1_HEAD)
$(OBJ_2) : $(OBJ_2_HEAD)
$(OBJ_3) : $(OBJ_3_HEAD)
$(OBJ_4) : $(OBJ_4_HEAD)
$(OBJ_5) : $(OBJ_5_HEAD)

.PHONY : tar
tar :
	tar cfv $(NAME).tar $(SOURCE)
.PHONY : clean
clean :
	-rm $(EXES) *.o 
	



