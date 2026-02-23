//Найти в массиве самую длинную убывающую последовательность,
//расположенную до минимального элемента включительно.Вывести на
//экран номера ее первого и последнего элементов.

#include<stdio.h>

int main()
{
 int i, j, s = 0, maxdl=0,dl=0, maxind=0, ind=0, a=0; 
 int min = 0;
 int n, x[30];
 printf("n=");
 scanf("%d",&n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &x[i]);
    }
 for (i = 0; i<n-1; i++)
 {
  if (x[i] < min)
  {
   min = x[i];
   a = i;
  }
 }
 for (maxdl = 0, i = 0; i < a; i++)
 {
  for (dl = 1, ind = i; i < a && x[i] > x[i + 1]; dl++, i++)
  {
   if (dl > maxdl)
   {
    maxdl = dl;
    maxind = ind;
   }
  }
 }
 printf("max dlina = %d nomer pervogo = %d  nomer poclednego = %d \n", maxdl, maxind, maxind+maxdl);
 

}