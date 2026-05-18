using System;

//Дана целочисленная матрица размера m×n. Определить k – количество различных элементов матрицы (т.е. повторяющиеся элементы считать один раз).
class Lab_2
{
   
    static void Main()
    {
        Console.WriteLine("enter a size of raw:");
        int m = int.Parse(Console.ReadLine());
        Console.WriteLine("enter a size of colums:");    
        int n = int.Parse(Console.ReadLine());
       
        
        int[,] A = new int[m,n];
        Console.WriteLine("enter the mode: ");
        int mode = int.Parse(Console.ReadLine());
        if (mode ==1){
        Random rand = new Random();
        for (int i =0 ; i < m; i++)  
        {
            for (int j =0; j < n; j++)
            {
                A[i,j] = rand.Next(1, 10);
            }
        }
        }
        else
        {
            for (int i =0 ; i < m; i++)  
            {
                for (int j =0; j < n; j++)  
                {
                    A[i,j] = int.Parse(Console.ReadLine());
                }
            }
        }

       if (mode ==1)Console.WriteLine("\nGenerated array:");
       else Console.WriteLine("\nEntered array:");

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
                int k = 0;
                while(k < uniq && isUnique)
                {
                    if (A[i, j] == B[k])
                    {
                        isUnique = false;
                    }
                    k++;
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