using System;
using System.Collections.Generic;

class Program
{
    static List<int> list = new List<int>();
    static Random rand = new Random();

    static void Main()
    {
        bool running = true;
        while (running)
        {
            PrintMenu();
            Console.Write("Choice: ");
            string input = Console.ReadLine();

            switch (input)
            {
                case "1":  AddItem();       break;
                case "2":  InsertItem();    break;
                case "3":  RemoveItem();    break; 
                case "4":  RemoveAtItem();  break;
                case "5":  ContainsItem();  break;
                case "6":  IndexOfItem();   break;
                case "7":  FindItem();      break;
                case "8":  SortList();      break;
                case "9":  ReverseList();   break;
                case "10": ClearList();     break;
                case "11": GenerateList();  break;
                case "0":  running = false; Console.WriteLine("Goodbye."); break;
                default:   Console.WriteLine("Unknown command."); break;
            }
        }
    }

    
    static void PrintMenu()
    {
        Console.WriteLine("\n--------------------------------");
        Console.WriteLine("        MENU — List<int>       ");
        Console.WriteLine("----------------------------------");
        Console.WriteLine(" 1. Add        — add item     ");
        Console.WriteLine(" 2. Insert     — insert at i  ");
        Console.WriteLine(" 3. Remove     — remove value ");
        Console.WriteLine(" 4. RemoveAt   — remove at i  ");
        Console.WriteLine(" 5. Contains   — contains?    ");
        Console.WriteLine(" 6. IndexOf    — get index    ");
        Console.WriteLine(" 7. Find       — find fist x > val");
        Console.WriteLine(" 8. Sort       — sort     ");
        Console.WriteLine(" 9. Reverse    — reverse      ");
        Console.WriteLine(" 10. Clear     — clear all    ");
        Console.WriteLine(" 11. Generate   — fill with random");
        Console.WriteLine(" 0. Exit                      ");
        Console.WriteLine("--------------------------------");
        Console.WriteLine($"List: [{string.Join(", ", list)}]  Count={list.Count}");
    }

    
    static bool ReadInt(string prompt, out int value)
    {
        Console.Write(prompt);
        if (int.TryParse(Console.ReadLine(), out value))
            return true;
        Console.WriteLine("Invalid input.");
        return false;
    }

    static bool ReadIndex(string prompt, out int index, bool allowEqualCount = false)
    {
        if (!ReadInt(prompt, out index)) return false;
        int max = allowEqualCount ? list.Count : list.Count - 1;
        if (index < 0 || index > max)
        {
            Console.WriteLine("Index out of range.");
            return false;
        }
        return true;
    }

   
    static void AddItem()
    {
        if (!ReadInt("Enter value: ", out int val)) return;
        list.Add(val);
        Console.WriteLine($"Add({val}) ");
    }

    static void InsertItem()
    {
        if (!ReadIndex("Insert at index: ", out int idx, allowEqualCount: true)) return;
        if (!ReadInt("Enter value: ", out int val)) return;
        list.Insert(idx, val);
        Console.WriteLine($"Insert({idx}, {val}) ");
    }

    static void RemoveItem()
    {
        if (!ReadInt("Value to remove: ", out int val)) return;
        bool removed = list.Remove(val);
        Console.WriteLine(removed ? $"Remove({val}) " : $"{val} not found.");
    }

    static void RemoveAtItem()
    {
        if (!ReadIndex("Index to remove: ", out int idx)) return;
        int val = list[idx];
        list.RemoveAt(idx);
        Console.WriteLine($"RemoveAt({idx}) → removed {val} ");
    }

    static void ContainsItem()
    {
        if (!ReadInt("Check value: ", out int val)) return;
        Console.WriteLine($"Contains({val}) = {list.Contains(val)}");
    }

    static void IndexOfItem()
    {
        if (!ReadInt("Find index of: ", out int val)) return;
        Console.WriteLine($"IndexOf({val}) = {list.IndexOf(val)}");
    }

    static void FindItem()
    {
        if (!ReadInt("Find first element greater than: ", out int threshold)) return;
        int found   = list.Find(x => x > threshold);
        Console.WriteLine($"Find(x > {threshold}) = {found}");
    }

    static void SortList()
    {
        list.Sort();
        Console.WriteLine("Sort()");
    }

    static void ReverseList()
    {
        list.Reverse();
        Console.WriteLine("Reverse()");
    }

    static void ClearList()
    {
        list.Clear();
        Console.WriteLine("Clear()");
    }
    static void GenerateList()
    {
        if (!ReadInt("How many random items to generate? ", out int count)) return;
        list.Clear();
        for (int i = 0; i < count; i++)
            list.Add(rand.Next(0, 100));
        Console.WriteLine($"Generated {count} random items.");
    }
}