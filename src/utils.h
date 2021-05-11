#ifndef _UTILS_H_
#define _UTILS_H_

class Point
{
public:
    int x_ = -1;
    int y_ = -1;
    Point(int x, int y);
    Point();
    bool operator==(const Point &);
};

int Sign(int a);

template <class T>
class MyVector
{
public:
    MyVector(int size = 0);
    MyVector(const MyVector &other);
    MyVector<T> &operator=(MyVector &other);
    ~MyVector();
    void reServe(int newCapacity);
    void push_back(T val);
    T &operator[](int i);
    int size();
    void clear();
    bool erase_at(int i);

private:
    const int SPACE_CAPACITY = 8;
    int theCapacity;
    int theSize;
    T *data;
};

template <class T>
MyVector<T>::MyVector(int size) : theSize(size), theCapacity(size + SPACE_CAPACITY)
{
    data = new T[theCapacity];
}
template <class T>
MyVector<T>::MyVector(const MyVector &other) : theSize(other.theSize), theCapacity(other.theCapacity)
{
    data = new T[theCapacity];
    for (int i = 0; i < theSize; i++)
    {
        data[i] = other.data[i];
    }
}

template <class T>
MyVector<T> &MyVector<T>::operator=(MyVector<T> &other)
{
    if (this == &other)
        return *this;
    else
    {
        delete[] data;
        theSize = other.theSize;
        theCapacity = other.theCapacity;
        data = new T[theCapacity];

        for (int i = 0; i < theSize; ++i)
        {
            data[i] = other.data[i];
        }
        return *this;
    }
}
template <class T>
MyVector<T>::~MyVector(void)
{
    delete[] data;
}
template <class T>
void MyVector<T>::reServe(int newCapacity)
{
    if (newCapacity <= theCapacity)
        return;

    T *temp = data;
    data = new T[newCapacity];
    for (int i = 0; i < theSize; ++i)
        data[i] = temp[i];
    delete[] temp;
}
template <class T>
void MyVector<T>::push_back(T val)
{
    if (theSize == theCapacity)
        reServe(2 * theCapacity + 1);
    data[theSize++] = val;
}
template <class T>
T &MyVector<T>::operator[](int i)
{
    return data[i];
}
template <class T>
int MyVector<T>::size()
{
    return theSize;
}
template <class T>
void MyVector<T>::clear()
{
    delete[] data;
    data = new T[theCapacity];
    theSize = 0;
}
template <class T>
bool MyVector<T>::erase_at(int i)
{
    if (i > theSize || i < 0)
    {
        return false;
    }
    for (int j = i; j < theSize - 1; j++)
    {
        data[j] = data[j + 1];
    }
    theSize -= 1;
    return true;
}

#endif //_UTILS_H_