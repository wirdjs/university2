using System;
class Program
{
    static void Main()
    {
        int n ;
       Console.WriteLine("enter a size of array:");

     while(!int.TryParse(Console.ReadLine(), out n)|| n <= 0){
            Console.WriteLine("Incorrect input. Enter a positive integer:");
        }
       
    
        int[] A = new int[n];
        Random rand = new Random();

        for (int i = 0; i < A.Length; i++)
        {
            A[i] = rand.Next(1, 100);
        }   

        Console.WriteLine("\nGenerated array:");
        Console.WriteLine(string.Join(", ", A));
   
        int uniq = 0;
int[] B = new int[uniq];

for (int i = 0; i < A.Length; i++)
{
    bool isUnique = true;

    for (int j = 0; j < B.Length; j++)
    {
        if (A[i] == B[j])
        {
            isUnique = false;
            break;
        }
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
