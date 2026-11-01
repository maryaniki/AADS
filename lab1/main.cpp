import std;

template <typename T>
class Set
{
private:
    T* data;
    size_t size;
    size_t capacity;

    void resize(size_t newCapacity) 
    {
        T* newData = new T[newCapacity];

        for (size_t i = 0; i < size; ++i)
        {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    void remove(const T& value)
    {
        for (size_t i = 0; i < size; ++i)
        {
            if (data[i] == value)
            {
                for (size_t j = i; j + 1 < size; ++j)
                {
                    data[j] = data[j + 1];
                }
                --size;
                return;
            }
        }
    }

public:
    Set() : data(nullptr), size(0), capacity(0) {}

    Set(const T* arr, size_t count) 
    {
        if (count == 0) 
        {
            return;
        }
        capacity = count;
        size = 0;
        data = new T[capacity];
        for (size_t i = 0; i < count; ++i)
        {
            add(arr[i]);
        }
    }

    Set(size_t count, const T& min, const T& max): data(nullptr), size(0), capacity(0)
    {
        std::random_device r;
        std::default_random_engine generator(r());

        for (size_t i = 0; i < count; ++i)
        {
            if constexpr (std::is_integral_v<T>)
            {
                std::uniform_int_distribution<T> distribution(min,max);
                add(distribution(generator));
            }
            else if constexpr (std::is_floating_point_v<T>)
            {
                std::uniform_real_distribution<T> distribution(min,max);
                add(distribution(generator));
            }
        }
    }

    Set(size_t count, size_t min, size_t max): data(nullptr), size(0), capacity(0)
    {
        std::random_device r;
        std::default_random_engine generator(r());
        std::uniform_int_distribution<size_t> lengthDistribution(min, max);
        std::uniform_int_distribution<int> characterDistribution(0, 25);
        for (size_t i = 0; i < count; ++i)
        {
            size_t length = lengthDistribution(generator);
            std::string value;
            for (size_t j = 0; j < length; ++j)
            {
                char character = static_cast<char>('a' + characterDistribution(generator));
                value += character;
            }
            add(value);
        }
    }

    Set(size_t count, const std::pair<int,int>& value_int, const std::pair<double, double>& value_double): data(nullptr), size(0), capacity(0)
    {
        std::random_device r;
        std::default_random_engine generator(r());
        std::uniform_int_distribution<int> intDistribution(value_int.first,value_int.second);
        std::uniform_real_distribution<double> doubleDistribution(value_double.first, value_double.second);

        for (size_t i = 0; i < count; ++i)
        {
            int first = intDistribution(generator);
            double second = doubleDistribution(generator);
            add({first, second});
        }
    }

    Set(const Set& other) : data(nullptr), size(other.size), capacity(other.capacity)
    {
        if (capacity > 0)
        {
            data = new T[capacity];

            for (size_t i = 0; i < size; ++i)
            {
                data[i] = other.data[i];
            }
        }
    }

    ~Set()
    {
        delete[] data;
    }

    size_t getSize() const
    {
        return size;
    }

    bool contains(const T& value) const
    {
        if (size == 0) {
            return false;
        }

        for (size_t i = 0; i < size; ++i)
        {
            if (data[i] == value)
            {
                return true;
            }
        }

        return false;
    }

    void add(const T& value)
    {
        if (contains(value))
        {
            return;
        }
        if (size == capacity)
        {   
            size_t newCapacity = (capacity == 0) ? 2 : capacity * 2;
            resize(newCapacity);
        }
        data[size] = value;
        ++size;
    }

    T& operator [](size_t index) {
        if (index >= size){
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& operator[](size_t index) const
    {
        if (index >= size)
        {
            throw std::out_of_range("Index out of range");
        }

        return data[index];
    }


    Set operator+(const Set& other) const
    {
        Set result(*this);
        for (size_t i = 0; i < other.size; ++i)
        {
            result.add(other.data[i]);
        }

        return result;
    }

    Set operator-(const Set& other) const
    {
        Set result(*this);

        for (size_t i = 0; i < other.size; ++i)
        {
            result.remove(other.data[i]);
        }

        return result;
    }


};

template <typename T>
std::ostream& operator<<(std::ostream& out, const Set<T>& set)
{
    out << "{";
    for (size_t i = 0; i < set.getSize(); ++i)
    {
        if (i > 0)
        {
            out << ", ";
        }

        if constexpr (std::is_same_v<T, std::pair<int, double>>)
        {
            out << '(' << set[i].first << ", " << set[i].second << ')';
        }
        else
        {
            out << set[i];
        }
    }
    out << "}";
    return out;
}

int main() {
    std::cout << "INT\n";
    Set<int> numbers;
    numbers.add(10);
    numbers.add(20);
    numbers.add(30);
    numbers.add(20);
    std::cout << "Set: " << numbers << '\n';

    int values_int[] = { 1, 2, 3, 2, 4 };
    Set<int> test_int(values_int, 5);
    test_int.add(4);
    test_int.add(5);
    test_int.add(6);
    std::cout << "Other set: " << test_int << '\n';

    Set<int> rand_numbers(5, 1, 100);
    std::cout << "Random int: " << rand_numbers << '\n';

    int arr1[] = { 1,2,3 };
    Set<int> first(arr1, 3);
    int arr2[] = { 3,4,5 };
    Set<int> second(arr2, 3);
    Set<int> plusSet = first + second;
    Set<int> minusSet = first - second;
    std::cout << "A: " << first << '\n';
    std::cout << "B: " << second << '\n';
    std::cout << "A + B: " << plusSet << '\n';
    std::cout << "A - B: " << minusSet << '\n';



    std::cout << "\nFLOAT\n";
    Set<float> floats;
    floats.add(10.5f);
    floats.add(20.25f);
    floats.add(30.75f);
    floats.add(20.25f);
    std::cout << "Set: " << floats << '\n';

    float values_float[] = { 1.5f, 2.5f, 3.5f, 2.5f, 4.5f };
    Set<float> test_float(values_float, 5);
    test_float.add(4.5f);
    test_float.add(5.5f);
    test_float.add(6.5f);
    std::cout << "Other set: " << test_float << '\n';

    Set<float> rand_floats(5, 1.0f, 50.0f);
    std::cout << "Random floats: " << rand_floats << '\n';
 



    std::cout << "\nSTRING\n";
    Set<std::string> words;
    words.add("apple");
    words.add("banana");
    words.add("orange");
    words.add("banana");
    std::cout << "Set: " << words << '\n';

    std::string values_string[] = { "cat", "dog", "cat"};
    Set<std::string> test_string(values_string, 3);
    test_string.add("pig");
    test_string.add("cow");
    std::cout << "Other set: " << test_string << '\n';

    Set<std::string> rand_words(5, 1, 6);
    std::cout << "Random string: " << rand_words << '\n';



    std::cout << "\nPAIR\n";
    Set<std::pair<int, double>> pairs;
    pairs.add({ 1, 2.3 });
    pairs.add({ 2, 4.5 });
    pairs.add({ 3, 6.7 });
    pairs.add({ 2, 4.5 });
    std::cout << "Set: " << pairs << '\n';

    std::pair<int,double> values_pairs[] =
    {
        {5, 2.8},
        {6, 7.6},
        {7, 1.8},
        {6, 7.6}
    };
    Set<std::pair<int, double>> test_pairs(values_pairs, 4);
    std::cout << "Other set: " << test_pairs << '\n';

    Set<std::pair<int, double>> rand_pairs(5, std::pair<int, int>{1, 10}, std::pair<double, double>{1.0,10.0});
    std::cout << "Random pairs: " << rand_pairs << '\n';



    return 0;

}