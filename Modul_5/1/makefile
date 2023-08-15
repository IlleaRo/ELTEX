CC = gcc

LIB_SRC = division.c multiply.c
STATIC_LIB_OBJ = $(LIB_SRC:.c=_static.o)
DYNAMIC_LIB_OBJ = $(LIB_SRC:.c=_dynamic.o)

STATIC_LIB_NAME = libstatarithmetic.a
DYNAMIC_LIB_NAME = libdynarithmetic.so

MAIN_SRC = main.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)
DYN_MAIN_SRC = dyn_main.c
DYN_MAIN_OBJ = $(DYN_MAIN_SRC:.c=.o)

all : static dynamic_stat dynamic clean

# Решение со статической библиотекой
static: $(MAIN_OBJ) $(STATIC_LIB_NAME)
	$(CC) -o static $(MAIN_OBJ) -L. $(STATIC_LIB_NAME)

$(STATIC_LIB_NAME): $(STATIC_LIB_OBJ)
	ar rcs $(STATIC_LIB_NAME) $(STATIC_LIB_OBJ)


%_static.o : %.c
	$(CC) -c  -o $@ $<

$(MAIN_OBJ): $(MAIN_SRC)
	$(CC) -c $(MAIN_SRC)

# Решение с динамической библиотекой, но подгрузкой во время линковки

dynamic_stat: $(MAIN_OBJ) $(DYNAMIC_LIB_NAME)
	$(CC) -o dynamic_stat $(MAIN_OBJ) -L. $(DYNAMIC_LIB_NAME)
	
$(DYNAMIC_LIB_NAME) : $(DYNAMIC_LIB_OBJ)
	$(CC) -shared -o $(DYNAMIC_LIB_NAME) $(DYNAMIC_LIB_OBJ)


%_dynamic.o : %.c
	$(CC) -c -fPIC -o $@ $<


# Решение с динамической библиотекой и загрузкой
dynamic: $(DYNAMIC_LIB_NAME)
	$(CC) -o dynamic $(DYN_MAIN_SRC) -ldl


clean:
	rm -f $(STATIC_LIB_NAME) $(MAIN_OBJ) $(STATIC_LIB_OBJ) $(STATIC_LIB_OBJ) $(DYNAMIC_LIB_OBJ)