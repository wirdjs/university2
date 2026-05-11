using System;
//Дана целочисленная матрица размера m×n. Определить k – количество различных элементов матрицы (т.е. повторяющиеся элементы считать один раз).
class Lab_2
{
    static void Main()
    {
        int m =0;     
        Console.WriteLine("enter a size of array:");
        int.TryParse(Console.ReadLine(),out m);
        int n = 0;
        Console.WriteLine("enter a size of array:");
        int.TryParse(Console.ReadLine(),out n);
        int[,] A = new int[m,n];
        Random rand = new Random();
        for (int i =0 ; i < m; i++)
        {
            for (int j =0; j < n; j++)
            {
                A[i,j] = rand.Next(1, 100);
            }
        }
        Console.WriteLine("\nGenerated array:");

        for (int i =0 ; i < m; i++)
        {
            for (int j =0; j < n; j++)
            {
                Console.Write(A[i,j] + " ");
            }
            Console.WriteLine();
        }
        Console.WriteLine("uniq array:");
        int uniq = 0;
        int[] B = new int[uniq];

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                bool isUnique = true;
                for (int k = 0; k < B.Length; k++)
                {
                    if (A[i, j] == B[k])
                    {
                        isUnique = false;
                        break;
                    }
                }
                    if (isUnique)
                    {
                    Array.Resize(ref B, uniq + 1);
                    B[uniq] = A[i, j];
                    uniq++;
                    }
            }
        }
        Console.WriteLine(string.Join(", ", B));
        Console.WriteLine($"\nNumber of unique elements: {uniq}");
    }
}