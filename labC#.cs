using System;
class Program
{
    static void Main()
    {
        Console.WriteLine("enter a size of array:");
        int n = int.Parse(Console.ReadLine());
        int[] A = new int[n];
        int rand = new Random();
        foreach (int arr in A )
        {
          arr = rand.Next(1, 100);
        }
        for(int i = 0; i < A.Length; i++)
            {
                Console.WriteLine(A[i]);
            }
    }
}