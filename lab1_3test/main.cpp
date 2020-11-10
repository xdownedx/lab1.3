//
//  main.cpp
//  lab1_3test
//
//  Created by Максим Палёхин on 27.10.2020.
//
// Название шаблонов разные, потому что xcode обращается к первому объявлению и выдает ошибку
// в строках
#include <iostream>
#include <fstream>

using namespace std;

template <class T>
class Element
{
    //элемент связного списка
private:
    //указатель на предыдущий и следующий элемент
    Element* next;
    Element* prev;
    
public:
    T field;
    //доступ к полю *next
    virtual Element* getNext() {return next;}
    virtual void setNext(Element* value) {next = value;}
    
    //доступ к полю *prev
    virtual Element* getPrevious() {return prev;}
    virtual void setPrevious(Element* value) {prev = value;}
    
    //доступ к полю с хранимой информацией field
    virtual T getValue() {return field;}
    virtual void setValue(T value) {field = value;}
    template<class S> friend ostream& operator<< (ostream& ustream, Element<T>& obj);
};

template<class R>
ostream& operator << (ostream& ustream, Element<R>& obj)
{
    ustream << obj.field;
    return ustream;
}

template <class R>
class LinkedListParent
{
protected:
    //достаточно хранить начало и конец
    Element<R>* head;
    Element<R>* tail;
    //для удобства храним количество элементов
    int num;
public:
    
    virtual int Number() { return num; }
    
    virtual Element<R>* getBegin() { return head; }
    
    virtual Element<R>* getEnd() { return tail; }
    
    
    LinkedListParent()
    {
        //конструктор без параметров
        head = NULL;
        num = 0;
    }
    
    //чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать добавление
    virtual Element<R>* push(R value) = 0;
    
    //чисто виртуальная функция: пока не определимся с типом списка, не сможем реализовать удаление
    virtual Element<R>* pop() = 0;
    
    virtual ~LinkedListParent()
    {
        //деструктор - освобождение памяти
        
    }
    
    //получение элемента по индексу - какова асимптотическая оценка этого действия?
    virtual Element<R>* operator[](int i)
    {
        //индексация
        if (i<0 || i>num) return NULL;
        int k = 0;
        
        //ищем i-й элемент - вставем в начало и отсчитываем i шагов вперед
        Element<R>* cur = head;
        for (k = 0; k < i; k++)
        {
            cur = cur->getNext();
        }
        return cur;
    }
    
    template<class T> friend ostream& operator<< (ostream& ustream, LinkedListParent<T>& obj);
    template<class T> friend istream& operator >> (istream& ustream, LinkedListParent<T>& obj);
};

template<class T>
ostream& operator << (ostream& ustream, LinkedListParent<T>& obj)
{
    if (typeid(ustream).name() == typeid(ofstream).name())
    {
        ustream << obj.num << "\n";
        for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext())
        ustream << current->getValue() << " ";
        return ustream;
    }
    
    ustream << "\nLength: " << obj.num+1 << "\n";
    int i = 0;
    for (Element<T>* current = obj.getBegin(); current != NULL; current = current->getNext(), i++)
    ustream << "arr[" << i << "] = " << current->getValue() << "\n";
    
    return ustream;
}

template<class T>
istream& operator >> (istream& ustream, LinkedListParent<T>& obj)
{
    //чтение из файла и консоли совпадают
    int len;
    ustream >> len;
    //здесь надо очистить память под obj, установить obj.num = 0
    int v = 0;
    for (int i = 0; i < len; i++)
    {
        ustream >> v;
        obj.push(v);
    }
    return ustream;
}

//дописать класс итератора по списку
template<typename ValueType>
class ListIterator : public std::iterator<std::input_iterator_tag, ValueType>
{
private:
    
public:
    //конструкторы
    ListIterator() { ptr = NULL; }
    ListIterator(Element<ValueType>* p) { ptr = p; }
    ListIterator(const ListIterator& it) { ptr = it.ptr; }
    
    //методы работы с итераторами
    //присваивание
    ListIterator& operator=(const ListIterator& it) { ptr = it.ptr; return *this; }
    ListIterator& operator=(Element<ValueType>* p) { ptr = p; return *this; }
    
    //проверка итераторов на равенство
    bool operator!=(ListIterator const& other) const { return ptr != other.ptr; }
    bool operator==(ListIterator const& other) const { return ptr == other.ptr; }
    //получить значение
    Element<ValueType>& operator*()
    {
        return *ptr;
    }
    //перемещение с помощью итераторов
    //перемещение с помощью итераторов
    ListIterator& operator++()
    {
        if(ptr->getNext() != NULL && ptr!=NULL)
            ptr = ptr->getNext();
        return *this;
    } //++p Можно ли ptr двигать вперед? Ответ: if(ptr!=tail)...
    ListIterator& operator++(int v)
    {
        if (ptr->getNext() != NULL && ptr != NULL)
            ptr = ptr->getNext();
        return *this;
    } //p++
    
    ListIterator& operator--()
    {
        if (ptr->getPrevious() != NULL && ptr != NULL)
            ptr = ptr->getPrevious();
        return *this;
    } //--p
    ListIterator& operator--(int v)
    {
        if (ptr->getPrevious() != NULL && ptr != NULL)
            ptr = ptr->getPrevious();
        return *this;
    } //p--
private:
    //текущий элемент
    Element<ValueType>* ptr;
};

//класс итерируемый список - наследник связного списка, родитель для Очереди и Стека
template <class T>
class IteratedLinkedList : public LinkedListParent<T>
{
public:
    IteratedLinkedList() : LinkedListParent<T>() { }
    virtual ~IteratedLinkedList() {  }
    
    ListIterator<T> iterator;
    
    ListIterator<T> begin() { ListIterator<T> it = LinkedListParent<T>::head; return it; }
    ListIterator<T> end() { ListIterator<T> it = LinkedListParent<T>::tail; return it; }
};

// Класс стэка
template <class T>
class Stack : public IteratedLinkedList<T>
{
public:
    Stack() : IteratedLinkedList<T>() { std::cout << "\nStack constructor"; }
    virtual ~Stack() { std::cout << "\nStack destructor"; }
    
    virtual Element<T>* pop()
    {//удаление из стэка
        if (LinkedListParent<T>::tail == NULL)
        {
            return NULL;
        }
        Element<T>* res = LinkedListParent<T>::tail;
        if (LinkedListParent<T>::head == LinkedListParent<T>::tail)
        {
            LinkedListParent<T>::head == LinkedListParent<T>::tail == NULL;
            LinkedListParent<T>::num = 0;
        }
        else
        {
            Element<T>* newTail = LinkedListParent<T>::tail->getPrevious();
            newTail->setNext(NULL);
            LinkedListParent<T>::tail->setNext(NULL);
            LinkedListParent<T>::num--;
            LinkedListParent<T>::tail = newTail;
        }
        return res;
    }
    virtual Element<T>* push(T value)
    {//добавление в стэк
        Element<T>* newElem = new Element<T>;
        newElem->setValue(value);
        newElem->setNext(NULL);
        newElem->setPrevious(NULL);
        
        if (LinkedListParent<T>::num == 0)
        {
            LinkedListParent<T>::head = LinkedListParent<T>::tail = newElem;
        }
        else
        {
            newElem->setPrevious(LinkedListParent<T>::tail);
            LinkedListParent<T>::tail->setNext(newElem);
            LinkedListParent<T>::tail = newElem;
            
        }
        LinkedListParent<T>::num++;
        return LinkedListParent<T>::tail;
    }
};

template<class T>
class overriddenStack :public Stack<T>
{
private:
    void left(Element<T>* newElem){//вставляем слева
        newElem->setNext(LinkedListParent<T>::head);
        LinkedListParent<T>::head->setPrevious(newElem);
        LinkedListParent<T>::head = newElem;
        
        LinkedListParent<T>::num++;
    }
    void right(Element<T>* newElem){//вставляем справа
        newElem->setPrevious(LinkedListParent<T>::tail);
        LinkedListParent<T>::tail->setNext(newElem);
        LinkedListParent<T>::tail = newElem;
        
        LinkedListParent<T>::num++;
    }
public:
    virtual Element<T>* push(T value)
    {
        Element<T>* newElem = new Element<T>();
        newElem->setValue(value);
        newElem->setNext(NULL);
        newElem->setPrevious(NULL);
        
        if (LinkedListParent<T>::num == 0)
        {//список пустой
            LinkedListParent<T>::head = LinkedListParent<T>::tail = newElem;
        }
        else
        {
            Element<T>* currentIndex = LinkedListParent<T>::head;
            while (currentIndex != LinkedListParent<T>::tail && currentIndex->getValue() > value)
            {//узнаем индекс вставки
                currentIndex = currentIndex->getNext();
            }
            
            if (currentIndex == LinkedListParent<T>::head && currentIndex == LinkedListParent<T>::tail)
            {//Если в списке один элемент
                if (currentIndex->getValue() < value){left(newElem);}
                if (currentIndex->getValue() >= value){right(newElem);}
            }
            
            if (currentIndex == LinkedListParent<T>::head)
            {//Если в списке больше 2 элементов, и нужно вставить около начала
                if (currentIndex->getValue() < value){left(newElem);}
                if (currentIndex->getValue() >= value){right(newElem);}
            }
            if (currentIndex == LinkedListParent<T>::tail)
            {//Если в списке больше 2 элементов, и нужно вставить около конца
                if (currentIndex->getValue() < value){left(newElem);}
                if (currentIndex->getValue() >= value){right(newElem);}
            }
            //Если вставлять надо в середину
            Element<T>* temp = currentIndex->getPrevious();
            currentIndex->getPrevious()->setNext(newElem);
            newElem->setNext(currentIndex);
            currentIndex->setPrevious(newElem);
            newElem->setPrevious(temp);
            
        }
        LinkedListParent<T>::num++;
        return LinkedListParent<T>::tail;
    }
    virtual bool predicat(T elem, int value) {
        return elem%value==0;
    }
    //virtual void filter(Stack<T>& S, T value) {if (value%3==0) {S.push(value);}}
    virtual overriddenStack filter(Stack<T>& S, T value) {
        overriddenStack<T> res;
        Element<T>* cur = LinkedListParent<T>::head;
        while (cur != LinkedListParent<T>::tail)
        {//Прогоняемся до тех пор, пока не поймем, куда вставлять
            if (predicat(S,(cur->getValue())))
                res.push(cur->getValue());
            cur = cur->getNext();
        }
        res.push(cur->getValue());
        return res;
        if (predicat(S, value)) {
            res.push(value);
        }
    }
};

int main()
{
    overriddenStack<int> D;
    D.push(1);
    D.push(0);
    D.push(2);
    D.push(9);
    D.push(680);
    D.push(120);
    
    Element<int>* el = D.pop();
    
    std::cout << "\nElement = " << el->getValue();
    std::cout << D << "\n\n";
    D.iterator = D.begin();
    while (D.iterator != D.end())
    {
        std::cout << *D.iterator << " ";
        D.iterator++;
    }
    std::cout << *D.iterator;
    std::cout << "\n";
    
    return 0;
}

