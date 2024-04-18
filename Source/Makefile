build: main.o matrix.o math.o
	gcc $^ -lm

buildlib: matrix.o math.o
	gcc -o FlexirtaM.so -shared -fPIC $^

debug: mainDebug.o matrixDebug.o mathDebug.o
	gcc $^ -lm -g

main.o: main.c FlexirtaM.h buildoption.h
matrix.o: matrix.c FlexirtaM.h buildoption.h
math.o: math.c FlexirtaM.h buildoption.h

%.o: %.c
	gcc -c $<

%.o: %.s
	gcc -c $<

clean:
ifeq ($(OS), Windows_NT)
	del *.o *.a *.exe
else
	rm *.o *.a *.out
endif
