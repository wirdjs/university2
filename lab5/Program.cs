using System;
using System.Collections.Generic;

class Program
{
    static void Main()
    {
        // ── СОЗДАНИЕ ──────────────────────────────────────────────
        var list = new List<int>();                          // пустой список
        var listWithCapacity = new List<int>(10);           // с начальной ёмкостью
        var listFromArray = new List<int>(new[] { 1, 2, 3 }); // из коллекции

        // ── ДОБАВЛЕНИЕ ───────────────────────────────────────────
        list.Add(10);                          
        list.Add(20);
        list.Add(30);
        list.Add(40);
        list.Add(50);

        list.Insert(2, 99);                    // вставить на индекс 2
        // [10, 20, 99, 30, 40, 50]

        list.AddRange(new[] { 60, 70, 80 });   // добавить коллекцию в конец
        // [10, 20, 99, 30, 40, 50, 60, 70, 80]

        list.InsertRange(2, new[] { 1, 2, 3 }); // вставить коллекцию с индекса 2
        // [1, 2, 3, 10, 20, 99, 30, 40, 50, 60, 70, 80]

        // ── ДОСТУП ───────────────────────────────────────────────
        int first = list[0];                   // доступ по индексу → 1
        int count = list.Count;                // количество элементов
        int capacity = list.Capacity;          // текущая ёмкость внутреннего массива

        Console.WriteLine($"Первый: {first}, Кол-во: {count}, : {capacity}");

        // ── ПОИСК ────────────────────────────────────────────────
        bool has99 = list.Contains(99);        // есть ли элемент? → true
        int idx = list.IndexOf(99);            // первый индекс элемента
        int lastIdx = list.LastIndexOf(99);    // последний индекс элемента
        int idxFrom = list.IndexOf(10, 2);     // поиск начиная с индекса 2

        list.Sort();                           // сортировка нужна перед BinarySearch!
        int binIdx = list.BinarySearch(30);    // бинарный поиск (список должен быть отсортирован)

        Console.WriteLine($"Содержит 99: {has99}, Индекс: {idx}, BinarySearch(30): {binIdx}");

        // ── ПОИСК С ПРЕДИКАТОМ ───────────────────────────────────
        int found    = list.Find(x => x > 50);          // первый элемент > 50
        int foundLast = list.FindLast(x => x > 50);     // последний элемент > 50
        int foundIdx = list.FindIndex(x => x > 50);     // индекс первого > 50
        int foundLastIdx = list.FindLastIndex(x => x > 50); // индекс последнего > 50
        List<int> foundAll = list.FindAll(x => x > 50); // все элементы > 50
        bool exists = list.Exists(x => x > 100);        // есть ли хоть один > 100?
        bool trueForAll = list.TrueForAll(x => x > 0);  // все ли элементы > 0?

        Console.WriteLine($"Find > 50: {found}, Exists > 100: {exists}, TrueForAll > 0: {trueForAll}");

        // ── СОРТИРОВКА И ПОРЯДОК ─────────────────────────────────
        list.Sort();                                     // сортировка по умолчанию (ascending)
        list.Sort((a, b) => b.CompareTo(a));             // сортировка с компаратором (descending)
        list.Reverse();                                  // перевернуть весь список
        list.Reverse(0, 3);                              // перевернуть 3 элемента с индекса 0

        // ── УДАЛЕНИЕ ─────────────────────────────────────────────
        list.Remove(99);                       // удалить первое вхождение значения 99
        list.RemoveAt(0);                      // удалить по индексу
        list.RemoveRange(0, 2);                // удалить 2 элемента начиная с индекса 0
        list.RemoveAll(x => x % 2 == 0);      // удалить все чётные числа

        Console.WriteLine($"После удалений: [{string.Join(", ", list)}]");

        // ── КОПИРОВАНИЕ ──────────────────────────────────────────
        int[] arr = new int[list.Count];
        list.CopyTo(arr);                      // скопировать в массив
        list.CopyTo(arr, 0);                   // скопировать в массив начиная с позиции 0
        // CopyTo(int index, T[] array, int arrayIndex, int count)
        list.CopyTo(0, arr, 0, list.Count);    // скопировать count элементов начиная с index

        // ── ПОЛУЧЕНИЕ ЧАСТИ СПИСКА ───────────────────────────────
        List<int> range = list.GetRange(0, 2); // получить подсписок (2 элемента с индекса 0)

        // ── ПРЕОБРАЗОВАНИЕ ───────────────────────────────────────
        List<string> strList = list.ConvertAll(x => x.ToString()); // преобразовать каждый элемент
        int[] toArray = list.ToArray();                            // в массив
        list.ForEach(x => Console.Write(x + " "));                // для каждого элемента

        Console.WriteLine();

        // ── ЁМКОСТЬ И ОПТИМИЗАЦИЯ ────────────────────────────────
        list.TrimExcess();                     // уменьшить Capacity до Count (освободить память)
        list.EnsureCapacity(100);              // гарантировать минимальную ёмкость (C# 8+)

        // ── ОЧИСТКА ──────────────────────────────────────────────
        list.Clear();                          // удалить все элементы (Count = 0)
        Console.WriteLine($"После Clear — Count: {list.Count}");
    }
}