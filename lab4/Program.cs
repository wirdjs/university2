using System;

namespace CircleProject
{
    // Basic properties of any shape
    interface IShape
    {
        double Area();
        double Perimeter();
    }

    // Input and output
    interface IPrintable
    {
        void Input();
        void Print();
    }

    // All operations for a circle
    interface ICircleOperations : IShape, IPrintable
    {
        double X { get; }
        double Y { get; }
        double R { get; }

        void MultiplyByNumber(double factor);
        bool ContainsPoint(double px, double py);
        bool Intersects(ICircleOperations other);
        bool IsInsideOf(ICircleOperations other);
        double InscribedRadiusInIntersection(ICircleOperations other);
    }

    // ─── Circle Class ─────────────────────────────────────────
    class Circle : ICircleOperations
    {
        private double x, y, r;

        // 1) Default constructor
        public Circle()
        {
            x = 0; y = 0; r = 1;
        }

        // 1) Constructor with parameters
        public Circle(double x, double y, double r)
        {
            this.x = x;
            this.y = y;
            this.r = r > 0 ? r : 1;
        }

        public double X => x;
        public double Y => y;
        public double R => r;

        // 2) Input
        public void Input()
        {
            Console.Write("Enter center x: ");
            while (!double.TryParse(Console.ReadLine(), out x))
                Console.Write("Invalid input. Enter center x again: ");

            Console.Write("Enter center y: ");
            while (!double.TryParse(Console.ReadLine(), out y))
                Console.Write("Invalid input. Enter center y again: ");

            Console.Write("Enter radius: ");
            while (!double.TryParse(Console.ReadLine(), out r) || r <= 0)
                Console.Write("Invalid input. Enter a positive radius: ");
        }

        // 2) Output
        public void Print() =>
            Console.WriteLine($"Circle: center ({x}, {y}), radius {r}");

        // 3) Area
        public double Area() => Math.PI * r * r;

        // 4) Circumference
        public double Perimeter() => 2 * Math.PI * r;

        // 5) Multiply by number
        public void MultiplyByNumber(double factor) => r *= factor;

        // 6) Compare by radius
        public static bool operator ==(Circle a, Circle b)
        {
            if (ReferenceEquals(a, null) && ReferenceEquals(b, null)) return true;
            if (ReferenceEquals(a, null) || ReferenceEquals(b, null)) return false;
            return Math.Abs(a.r - b.r) < 1e-9;
        }
        public static bool operator !=(Circle a, Circle b) => !(a == b);
        public override bool Equals(object obj) =>
            obj is Circle c && this == c;
        public override int GetHashCode() => r.GetHashCode();


        // 7) Point inside circle
        public bool ContainsPoint(double px, double py)
        {
            double d = Math.Sqrt((px - x) * (px - x) + (py - y) * (py - y));
            return d <= r;
        }

        // 8) Circle intersection
        public bool Intersects(ICircleOperations other)
        {
            double d = Math.Sqrt((x - other.X) * (x - other.X) + (y - other.Y) * (y - other.Y));
            return d < r + other.R && d > Math.Abs(r - other.R);
        }

        // 9) One circle inside another
        public bool IsInsideOf(ICircleOperations other)
        {
            double d = Math.Sqrt((x - other.X) * (x - other.X) + (y - other.Y) * (y - other.Y));
            return d + r <= other.R;
        }

        // 10) Radius of inscribed circle in intersection
        public double InscribedRadiusInIntersection(ICircleOperations other)
        {
            double d = Math.Sqrt((x - other.X) * (x - other.X) + (y - other.Y) * (y - other.Y));

            if (!this.Intersects(other))
                throw new InvalidOperationException("Circles do not intersect.");

            if (this.IsInsideOf(other) || other.IsInsideOf(this))
                throw new InvalidOperationException("One circle is completely inside another.");

            return (r + other.R - d) / 2.0;
        }
    }

    // ─── Utility Class ────────────────────────────────────────
    static class CircleUtils
    {
        public static void PrintShapeInfo(IShape shape)
        {
            Console.WriteLine($" Area = {shape.Area():F4}");
            Console.WriteLine($" Perimeter = {shape.Perimeter():F4}");
        }

        public static void PrintObject(IPrintable obj) => obj.Print();

        
        public static void CompareCircles(Circle a, Circle b)
        {
            Console.WriteLine($" a == b : {a == b}");
            Console.WriteLine($" a != b : {a != b}");
        }
    }

    // ─── Program ─────────────────────────────────────────────
    class Program
    {
        static void Main()
        {
            Console.WriteLine("=== Circle Class Testing ===\n");

            // // 2) Output and Input via IPrintable
            // Console.WriteLine("\n--- 2) Output via IPrintable ---");
            // IPrintable p = new Circle();
            // p.Input();
            // p.Print();

            // // 5) Multiply by number
            // Console.WriteLine("\n--- 5) Multiply by number ---");
            // ICircleOperations c5 = new Circle();
            // c5.Input();
            // Console.Write("Before: ");
            // c5.Print();
            // Console.Write("Enter multiply: ");
            // int i = int.Parse(Console.ReadLine());
            // c5.MultiplyByNumber(i);
            // Console.Write($"After {i}*: ");
            // c5.Print();


            // //6) Compare circles    
            // Console.WriteLine("\n--- 6) Compare circles ---");
            // Circle c6a = new Circle();
            // Circle c6b = new Circle();
            // Console.WriteLine("Circle A:");
            // c6a.Input();    
            // Console.WriteLine("Circle B:");
            // c6b.Input();
            // CircleUtils.CompareCircles(c6a, c6b);

            // //7) Point inside circle
            // Console.WriteLine("\n--- 7) Point inside circle ---");
            // Circle c7 = new Circle();
            // c7.Input();
            // Console.Write("Enter point x: ");
            // double px = double.Parse(Console.ReadLine());
            // Console.Write("Enter point y: ");
            // double py = double.Parse(Console.ReadLine());
            // Console.WriteLine($"Point ({px}, {py}) is inside circle: {c7.ContainsPoint(px, py)}");

            //8) Circle intersection
        //     Console.WriteLine("\n--- 8) Circle intersection ---");  
        //     Circle c8a = new Circle();
        //     Circle c8b = new Circle();
        //     Console.WriteLine("Circle A:");
        //     c8a.Input();
        //     Console.WriteLine("Circle B:");
        //     c8b.Input();
        //     Console.WriteLine($"Circles intersect: {c8a.Intersects(c8b)}");
        // // 9) One circle inside another
        //     Console.WriteLine("\n--- 9) One circle inside another ---");
        //     Circle c9a = new Circle();
        //     Circle c9b = new Circle();
        //     Console.WriteLine("Circle A:");
        //     c9a.Input();
        //     Console.WriteLine("Circle B:");
        //     c9b.Input();
        //     Console.WriteLine($"A is inside B: {c9a.IsInsideOf(c9b)}");
        //     Console.WriteLine($"B is inside A: {c9b.IsInsideOf(c9a)}");
        // 10) Inscribed circle radius in intersection
        // Console.WriteLine("\n--- 10) Inscribed circle radius in intersection ---");
        // Circle c10a = new Circle(); 
        // Circle c10b = new Circle();
        // Console.WriteLine("Circle A:");
        // c10a.Input();
        // Console.WriteLine("Circle B:");
        // c10b.Input();

        // if (!c10a.Intersects(c10b))
        // {
        //     Console.WriteLine("Error: Circles do not intersect.");
        // }
        // else if (c10a.IsInsideOf(c10b) || c10b.IsInsideOf(c10a))
        // {
        //     Console.WriteLine("Error: One circle is completely inside another.");
        // }
        // else
        // {
        //     double inscR = c10a.InscribedRadiusInIntersection(c10b);
        //     Console.WriteLine($"Inscribed circle radius: {inscR:F4}");
        // }
        // }

    }
}