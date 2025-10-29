class String
{ char *str;
 int len;
public:
 String(int l = 0);
 String(const char *);
 String(String &);
 ~String() {
 if(str) delete []str; str = NULL;
 }
 int strLength();
 String & operator = (const String &);
char &operator [](int);
 bool operator == (String &);
 bool operator != (String &);
 String operator + (const String &);
 String & operator += (const String &);
 friend istream & operator >> (istream &, String &);
 friend ostream & operator << (ostream &, String &);
 int BMSearch(String &);
};
// Дополнительно (по желанию).
// 1) Метод класса String.
// String operator()(int i, int j); // выделяет подстроку из исходной строки с i-ого по j-ый символы.
// 2) Задача.
// Есть строка и образец. Удалить из исходной строки первое вхождение образца.
// Использовать БМ-поиск, функции класса String (включая дополнительную), operator +.
// Пример.
// Строка: "На дворе трава на траве дрова"
// Образец: "траве"
// Результат: "На дворе трава на дрова"