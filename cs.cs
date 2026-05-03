using System;
class Program         
{
    static void Main()    
    {
        // 1. Ввод простого текста
        Console.Write("Введите ваше имя: ");
        string name = Console.ReadLine();
        Console.WriteLine($"Привет, {name}!");

        // 2. Ввод размера массива
        Console.Write("Введите количество элементов в массиве: ");
        string inputSize = Console.ReadLine();
        int size = int.Parse(inputSize); // Превращаем строку в число

        int[] numbers = new int[size];

        // 3. Заполнение массива через консоль
        for (int i = 0; i < numbers.Length; i++)
        {
            Console.Write($"Введите число для индекса {i}: ");
            numbers[i] = int.Parse(Console.ReadLine());
        }

        Console.WriteLine("\nВаш заполненный массив:");
        Console.WriteLine(string.Join(", ", numbers));
    }
}