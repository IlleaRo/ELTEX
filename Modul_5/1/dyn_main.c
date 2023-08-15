#include <stdio.h>
#include <string.h>

/* заголовочный файл для работы с динамическими библиотеками */
#include <dlfcn.h>

#define A 63
#define B 45


int main(int argc, char* argv[]){
    if (argc < 2 || argc > 2){
        printf("Incorrect number of arguments!\n");
        return 1;
    }

    int a = A;
    int b = B;

    void *ext_library;	// хандлер внешней библиотеки
    if ((ext_library = dlopen("/home/illearo/Documents/Repositories/ELTEX/Modul_5/1/libdynarithmetic.so",
        RTLD_LAZY)) == NULL){
        fprintf(stderr, "dlopen() error: %s\n", dlerror());
		return 1;
    }

    if (strcmp(argv[1],"division") == 0){
        float (*pFunc)(int a, int b);	// переменная для хранения адреса функции
        if ((pFunc = dlsym(ext_library, argv[1])) == NULL){
            fprintf(stderr, "dlsym() error: %s\n", dlerror());
		    return 1;
        }
        printf("We have two number:"
        "\n1) %d\n2) %d\n%s: %f\n",
        a,b, argv[1], (*pFunc)(a,b));
        dlclose(ext_library);
    } else if (strcmp(argv[1],"multiply") == 0){
        long (*pFunc)(int a, int b);	// переменная для хранения адреса функции
        if ((pFunc = dlsym(ext_library, argv[1])) == NULL){
            fprintf(stderr, "dlsym() error: %s\n", dlerror());
		    return 1;
        }
        printf("We have two number:"
        "\n1) %d\n2) %d\n%s: %ld\n",
        a,b, argv[1], (*pFunc)(a,b));
        dlclose(ext_library);
    } else {
        printf("Incorrect name of function!\n");
        return 1;
    }
    
    return 0;
}