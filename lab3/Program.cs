using System;
class lab3
{
     static void Main()
    {
        Console.WriteLine("Enter string (min 6 symbols): ");
        string s = Console.ReadLine();
        while (s.Length < 6)
        {
            Console.WriteLine("Enter string (min 6 symbols): ");
            s = Console.ReadLine();
        }

        int middlePos = s.Length / 2;
        
        char[] c = new char[s.Length];

        for (int i = 0; i < s.Length; i++)
        {
            string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            c[i] = chars[new Random().Next(chars.Length)]; 
        }
        Console.WriteLine("Generated random string:");
        Console.WriteLine(string.Join("", c));

        Console.WriteLine("after insertion:");
        string insert = new string(c);
        string res = s.Insert(middlePos, insert);
        Console.Write(res);
    }
}   