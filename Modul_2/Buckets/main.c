#include <stdio.h>
#include <stdlib.h>

#define INPUT_SIZE 30
#define LONG_MAX 2147483647
#define LONG_MIN (-2147483647)

void readC(long*, char*); // Чтение
bool check(const long*, const long*); // Проверка выполнения условия
void bubbleSort(long*, int); // Сортировка пузырьком



int main(){
    long buckets[3];
    long R;

    readC(&buckets[0],"a");
    readC(&buckets[1],"b");
    readC(&buckets[2],"c");
    readC(&R,"R");
    bubbleSort(buckets, 3);

    long states[] = {0, 0, buckets[2]};
    printf("Начиная со средней:\n0) %ld, %ld, %ld\n", states[0], states[1], states[2]);
    if (check(states,&R) || states[2] < R){
        printf("Already complete or incorrect tusk!\n");
        return 0;
    } // Начальная проверка наличия решения
    long counter = 0;
    bool isFirst = true;
    bool isLoop;
    while(true){
        if (counter>100){
            printf("Incorrect task! Answer doesn't exist!");
            return 0; // Чепуха какая-то!!!
        }
        if (!isFirst) {
            isLoop = true;
            for (int i = 0; i < 3; i+=2)
                if (states[i] != buckets[i]) {
                    isLoop = false;
                    break;
                }
            if (states[0]==buckets[0] && states[2]==buckets[2]-buckets[0]) isLoop = true;
            if (isLoop){
                printf("Incorrect task! Answer doesn't exist!");
                return 0;
            }
        }
        isFirst = false;
        //Переливаем в среднюю
        if (states[2] > (buckets[1] - states[1])){
            states[2] -= (buckets[1] - states[1]);
            states[1] = buckets[1];
        }
        else{
            states[1] += states[2];
            states[2] = 0;
        }
        counter++;
        printf("%ld) %ld, %ld, %ld\n", counter, states[0], states[1], states[2]);
        if (check(states,&R)) break;
        while(true){
            //Переливаем в самую маленькую
            if (states[1] > buckets[0] - states[0]){
                states[1] -= (buckets[0] - states[0]);
                states[0] = buckets[0];
            }
            else{
                states[0] += states[1];
                states[1] = 0;
            }
            counter++;
            printf("%ld) %ld, %ld, %ld\n", counter, states[0], states[1], states[2]);
            if (check(states,&R)) break;
            if (states[1]==0) break;
            //Переливаем из самой маленькой в самую большую

            states[2]+=states[0];
            states[0] = 0;
            counter++;
            printf("%ld) %ld, %ld, %ld\n", counter, states[0], states[1], states[2]);
            if (check(buckets,&R)) break;
        }
        if (check(states,&R)) break;
    }

    states[0] = 0; states[1] = 0; states[2] = buckets[2];
    printf("Начиная с меньшей\n0) %ld, %ld, %ld\n", states[0], states[1], states[2]);
    counter = 0;
    while(true){
        while (states[1]!=buckets[1]){
            //Переливаем в меньшую
            if (states[2] > (buckets[0] - states[0])){
                states[2] -= (buckets[0] - states[0]);
                states[0] = buckets[0];
            }
            else{
                states[0] = states[2];
                states[2] = 0;
            }
            counter++;
            printf("%ld) %ld, %ld, %ld\n", counter, states[0], states[1], states[2]);
            if (check(states,&R)) break;

            //Переливаем в среднюю

            if (states[0] > buckets[1] - states[1]){
                states[0] -= (buckets[1] - states[1]);
                states[1] = buckets[1];
            }
            else{
                states[1]+= states[0];
                states[0] = 0;
            }
            counter++;
            printf("%ld) %ld, %ld, %ld\n", counter, states[0], states[1], states[2]);
            if (check(states,&R)) return 0;
        }
        if (check(states,&R)) break;
        states[2]+=states[1];
        states[1] = 0;
        counter++;
        printf("%ld) %ld, %ld, %ld\n", counter, states[0], states[1], states[2]);
        if (check(buckets,&R)) break;

        states[1] = states[0];
        states[0] = 0;
        counter++;
        printf("%ld) %ld, %ld, %ld\n", counter, states[0], states[1], states[2]);
        if (check(buckets,&R)) break;
    }


    return 0;
}

void readC(long* number_to_read, char* name){
    char *ptr;
    char str[INPUT_SIZE];
    while (true)
    {
        printf("%s = ", name);
        fgets(str,INPUT_SIZE,stdin);
        *number_to_read = strtol(str, &ptr, 10);
        if (*number_to_read != LONG_MAX && *number_to_read != LONG_MIN && *number_to_read!=0) break;
        else printf("Incorrect input! Try again!");
    }
}
bool check(const long* buckets, const long* value){
    for (int i = 0; i < 3; ++i)
        if (buckets[i]==*value) return true;
    return false;
}

void bubbleSort(long* A, int size){
    long tmp;
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size - 1; ++j)
            if (A[j]>A[j+1]){
                tmp = A[j];
                A[j] = A[j+1];
                A[j+1] = tmp;
            }
}
