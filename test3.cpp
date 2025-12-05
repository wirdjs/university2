#include <stdio.h>

int main() {
    int i, j, s = 0, maxdl = 0, dl = 0, maxind = 0, ind = 0, a = 0;
    int n, x[30];

    printf("n=");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        scanf("%d", &x[i]);
    }

    // Инициализация минимального значения
    int min = x[0];
    for (i = 0; i < n; i++) {
        if (x[i] < min) {
            min = x[i];
            a = i;
        }
    }

    // Поиск самой длинной убывающей последовательности до минимального элемента
    for (maxdl = 0, i = 0; i <= a; i++) { // Изменено на i <= a
        for (dl = 1, ind = i; i < a && x[i] > x[i + 1]; dl++, i++) {
            if (dl > maxdl) {
                maxdl = dl;
                maxind = ind;
            }
        }
        // Проверка последнего элемента
        if (i == a && dl > maxdl) {
            maxdl = dl;
            maxind = ind;
        }
    }

    printf("max dlina = %d nomer pervogo = %d  nomer poslednego = %d \n", maxdl, maxind, maxind + maxdl - 1);

    return 0;
}