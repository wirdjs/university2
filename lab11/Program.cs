    using System;
using System.Globalization;
using System.Numerics;

class Program
    {
        static void Main()
        {
            Console.WriteLine("enter the size of array: ");
            int n = int.Parse(Console.ReadLine());   
        
        
            int[] A = new int[n];
            

            Console.Write("enter the mode: ");
            int mode =int.Parse(Console.ReadLine());

            if (mode == 1){
                Random rand = new Random();
            for (int i = 0; i < A.Length; i++){
                A[i] = rand.Next(1, 20);
            }
            }
            else
            {
                for (int i = 0; i < A.Length; i++)
                {
                    A[i] = int.Parse(Console.ReadLine());
                }
            }


            if (mode ==1)Console.WriteLine("\nGenerated array:");
            else Console.WriteLine("\nEntered array:");
            Console.WriteLine(string.Join(", ", A));
    
            int uniq = 0;
    int[] B = new int[uniq];

    for (int i = 0; i < A.Length; i++)
    {
        bool isUnique = true;

        int j = 0;
            while (j < uniq && isUnique)
            {
                if(A[i] == B[j])
                {
                    isUnique = false;
                }
                j++;
            }
        if (isUnique)
        {
            Array.Resize(ref B, uniq + 1);
            B[uniq] = A[i];
            uniq++;
        }
    }

    Console.WriteLine("\nUnique elements:");
    Console.WriteLine(string.Join(", ", B));
    Console.WriteLine($"\nNumber of unique elements: {uniq}");
        }
    }
