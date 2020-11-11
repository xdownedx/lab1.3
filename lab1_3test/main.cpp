//
//  main.cpp
//  lab1_3 lab 1,4
//
//  Created by Максим Палёхин on 27.10.2020.
//
// Название шаблонов разные, потому что xcode обращается к первому объявлению и выдает ошибку
// в строках
#include <iostream>
#include <fstream>

using namespace std;


class SchoolMan {

private:
    string name;
    string lastName;
    string sex;
    int group;
    string dateOfBorn;
    string adress;
public:
    SchoolMan(){}

    SchoolMan(string name, string lastName, string sex, int group, string dateOfBorn, string adress) {
        this->name = name;
        this->lastName = lastName;
        this->sex = sex;
        this->group = group;
        this->dateOfBorn = dateOfBorn;
        this->adress = adress;

    }

    string getName() {
        return name;
    }
    string getLastName() {
        return lastName;
    }
    string getSex() {
        return sex;
    }
    int getGroup() {
        return group;
    }
    string getDateOfBorn() {
        return dateOfBorn;
    }
    string getAdress() {
        return adress;
    }

    friend ostream& operator<<(ostream& os, const SchoolMan& schoolMan)
    {
        os << "" << schoolMan.name << " | " << schoolMan.lastName << " | " << schoolMan.sex << " | "
            << schoolMan.group << " | " << schoolMan.dateOfBorn << " | " << schoolMan.adress << "\n";
        return os;
    }
    bool operator==(SchoolMan const& other) const {
        return ((name == other.name) && (lastName == other.lastName) && (sex == other.sex) &&
            (group == other.group) && (dateOfBorn == other.dateOfBorn) && (adress == other.adress));
    }
    bool operator!=(SchoolMan const& other) const {
        return ((name != other.name) && (lastName != other.lastName) && (sex != other.sex) &&
            (group != other.group) && (dateOfBorn != other.dateOfBorn) && (adress != other.adress));
    }
    
    friend bool operator > (SchoolMan const& first, SchoolMan const& second){
        return first.group>second.group;
    };

    friend bool operator < (SchoolMan const& first, SchoolMan const & second){
        return first.group<second.group;
    };
    friend bool operator < (SchoolMan const& first, int const& second){
        return first.group<second;
    };

    friend bool operator >= (SchoolMan const& first, SchoolMan const& second){
        return first.group>=second.group;
    };
    friend bool operator <= (SchoolMan const& first, SchoolMan const& second){
        return first.group<=second.group;
    };
    friend int operator%(SchoolMan const& first, SchoolMan const& second){
        return first.group%second.group;
    };

};


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
    
    ustream << "\nLength: " << obj.num << "\n";
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
    Stack() : IteratedLinkedList<T>() { std::cout << "\nStack constructor\n"; }
    virtual ~Stack() { std::cout << "\nStack destructor\n"; }
    
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
public:
    virtual Element<T>* push(T value)
    {
        Element<T>* newElem = new Element<T>();
        newElem->setValue(value);
        newElem->setNext(NULL);
        newElem->setPrevious(NULL);

        if (LinkedListParent<T>::num == 0)
        {//Если список пустой
            LinkedListParent<T>::head = LinkedListParent<T>::tail = newElem;
        }
        else
        {//Вставка при "нормальном" списке
            Element<T>* cur = LinkedListParent<T>::head;
            while (cur != LinkedListParent<T>::tail&& cur->getValue() > value)
            {//Прогоняемся до тех пор, пока не поймем, куда вставлять
                cur = cur->getNext();
            }

            if (cur == LinkedListParent<T>::head&& cur == LinkedListParent<T>::tail)
            {//Если в списке один элемент
                if (cur->getValue() < value)
                {//Если надо вставить влево
                    newElem->setNext(LinkedListParent<T>::head);
                    LinkedListParent<T>::head->setPrevious(newElem);
                    LinkedListParent<T>::head = newElem;

                    LinkedListParent<T>::num++;
                    return LinkedListParent<T>::tail;
                }
                if (cur->getValue() >= value)
                {//Если надо ставить вправо
                    LinkedListParent<T>::tail->setNext(newElem);
                    newElem->setPrevious(LinkedListParent<T>::tail);
                    LinkedListParent<T>::tail = newElem;

                    LinkedListParent<T>::num++;
                    return LinkedListParent<T>::tail;
                }
            }

            if (cur == LinkedListParent<T>::head)
            {//Если в списке 2 или более элемента, и необходимо вставить около головы
                if (cur->getValue() < value)
                {//Вставить слева от гловы
                    newElem->setNext(LinkedListParent<T>::head);
                    LinkedListParent<T>::head->setPrevious(newElem);
                    LinkedListParent<T>::head = newElem;

                    LinkedListParent<T>::num++;
                    return LinkedListParent<T>::tail;
                }
                if (cur->getValue() >= value)
                {//Вставить справа от головы
                    newElem->setPrevious(LinkedListParent<T>::head);
                    newElem->setNext(LinkedListParent<T>::head->getNext());
                    LinkedListParent<T>::head->getNext()->setPrevious(newElem);
                    LinkedListParent<T>::head->setNext(newElem);

                    LinkedListParent<T>::num++;
                    return LinkedListParent<T>::tail;
                }
            }
            if (cur == LinkedListParent<T>::tail)
            {//Если в списке 2 или более элемента, и необходимо вставить около хвоста
                if (cur->getValue() < value)
                {//Если надо вставить слева
                    newElem->setPrevious(LinkedListParent<T>::tail->getPrevious());
                    LinkedListParent<T>::tail->getPrevious()->setNext(newElem);
                    newElem->setNext(LinkedListParent<T>::tail);
                    LinkedListParent<T>::tail->setPrevious(newElem);

                    LinkedListParent<T>::num++;
                    return LinkedListParent<T>::tail;
                }
                if (cur->getValue() >= value)
                {//Если надо вставить вправо
                    newElem->setPrevious(LinkedListParent<T>::tail);
                    LinkedListParent<T>::tail->setNext(newElem);
                    LinkedListParent<T>::tail = newElem;

                    LinkedListParent<T>::num++;
                    return LinkedListParent<T>::tail;
                }
            }
            //Если вставлять надо в середину
            Element<T>* temp = cur->getPrevious();
            cur->getPrevious()->setNext(newElem);
            newElem->setNext(cur);
            cur->setPrevious(newElem);
            newElem->setPrevious(temp);
            
        }
        LinkedListParent<T>::num++;
        return LinkedListParent<T>::tail;
    }
    virtual bool predicat(T elem, T value) {
        return elem%value==0;
    }

    virtual overriddenStack filter(T value) {
        overriddenStack<T> res;
        Element<T>* cur = LinkedListParent<T>::head;
        while (cur != LinkedListParent<T>::tail)
        {//Прогоняемся до тех пор, пока не поймем, куда вставлять
            if (predicat(cur->getValue(), value))
                res.push(cur->getValue());
            cur = cur->getNext();
        }
        return res;
        
    }
};

template <class T>
class overriddenStackSchoolman : public overriddenStack<T>
{
public:
    overriddenStackSchoolman() {  }
    virtual ~overriddenStackSchoolman() {  }

    
    virtual Element<T>* push(T value)
    {
        Element<T>* newElem = new Element<T>();
        newElem->setValue(value);
        newElem->setNext(NULL);
        newElem->setPrevious(NULL);

        if (LinkedListParent<T>::num == 0)
        {
            LinkedListParent<T>::head = LinkedListParent<T>::tail = newElem;
        }
        else
        {
            Element<T>* cur = LinkedListParent<T>::head;
            while (cur != LinkedListParent<T>::tail && cur->getValue() > value)
            {
                cur = cur->getNext();
            }

            if (cur == LinkedListParent<T>::head && cur == LinkedListParent<T>::tail)
            {
                if (cur->getValue() < value)
                {
                    newElem->setNext(LinkedListParent<T>::head);
                    LinkedListParent<T>::head->setPrevious(newElem);
                    LinkedListParent<T>::head = newElem;

                    LinkedListParent<T>::num++;
                    return LinkedListParent<T>::tail;
                    
                }
                if (cur->getValue() >= value)
                {
                    LinkedListParent<T>::tail->setNext(newElem);
                    newElem->setPrevious(LinkedListParent<T>::tail);
                    LinkedListParent<T>::tail = newElem;

                    LinkedListParent<T>::num++;
                    return LinkedListParent<T>::tail;
                }
            }

            if (cur == LinkedListParent<T>::head)
            {
                if (cur->getValue() < value)
                {
                    newElem->setNext(LinkedListParent<T>::head);
                    LinkedListParent<T>::head->setPrevious(newElem);
                    LinkedListParent<T>::head = newElem;

                    LinkedListParent<T>::num++;
                    return LinkedListParent<T>::tail;
                }
                if (cur->getValue() >= value)
                {
                    newElem->setPrevious(LinkedListParent<T>::head);
                    newElem->setNext(LinkedListParent<T>::head->getNext());
                    LinkedListParent<T>::head->getNext()->setPrevious(newElem);
                    LinkedListParent<T>::head->setNext(newElem);

                    LinkedListParent<T>::num++;
                    return LinkedListParent<T>::tail;
                }
            }
            if (cur == LinkedListParent<T>::tail)
            {
                if (cur->getValue() < value)
                {
                    newElem->setPrevious(LinkedListParent<T>::tail->getPrevious());
                    LinkedListParent<T>::tail->getPrevious()->setNext(newElem);
                    newElem->setNext(LinkedListParent<T>::tail);
                    LinkedListParent<T>::tail->setPrevious(newElem);

                    LinkedListParent<T>::num++;
                    return LinkedListParent<T>::tail;
                }
                if (cur->getValue() >= value)
                {
                    newElem->setPrevious(LinkedListParent<T>::tail);
                    LinkedListParent<T>::tail->setNext(newElem);
                    LinkedListParent<T>::tail = newElem;

                    LinkedListParent<T>::num++;
                    return LinkedListParent<T>::tail;
                }
            }


            Element<T>* temp = cur->getPrevious();
            cur->getPrevious()->setNext(newElem);
            newElem->setNext(cur);
            cur->setPrevious(newElem);
            newElem->setPrevious(temp);
        }
        LinkedListParent<T>::num++;
        return LinkedListParent<T>::tail;
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
    overriddenStack<int> V = D.filter(3);
    cout << "\nРабота фильтра: "<< "\n";
    cout << V << "\n\n";
    cout << "\nРабота pop(): "<< "\n";
    cout << "\nDelete element = " << el->getValue();
    cout << D << "\n\n";
    cout << "Лаба1.4" << "\n";
    cout << "\nИтераторы:\n";
    ListIterator<int> p = D.begin();
    while (p != D.end())
    {
    cout << (*p).getValue() << " ";
    p++;
    }
    cout << (*p).getValue() << " ";

    cout << "\nИтераторы в обратном порядке:\n";
    p = D.end();
    while (p != D.begin())
    {
        cout << (*p).getValue() << " ";
        p--;
    }
    cout << (*p).getValue() << " ";

    cout << "\nЛаба 1.5:\n";

    SchoolMan scm1("Виктория", "Шуманская", "Женский", 10, "07.04.1999", "Рязань");
    SchoolMan scm2("Максим", "Палёхин", "Мужской", 1, "20.12.2001", "Москва");
    SchoolMan scm3("Антон", "Дунаевский", "Мужской", 8, "12.11.2001", "Павловский пасад");
    SchoolMan scm4("Анатолий", "Красавин", "Мужской", 4, "01.02.2002", "Ростов");
    SchoolMan scm5("Александр", "Краснов", "Мужской", 3, "08.11.2001", "Ростов");
    SchoolMan scm6("Александр", "Сычев", "Мужской", 2, "04.01.2001", "Электросталь");
    SchoolMan scm8("Романов", "Алексей", "Мужской", 9, "23.05.2001", "Домодедово");
    
    SchoolMan top("","","",3,"","");
    overriddenStackSchoolman<SchoolMan> DSchoolman;
    overriddenStackSchoolman<SchoolMan> DSchoolmanFiltred;

    DSchoolman.push(scm1);
    DSchoolman.push(scm2);
    DSchoolman.push(scm3);
    DSchoolman.push(scm4);
    DSchoolman.push(scm5);
    DSchoolman.push(scm6);
    DSchoolman.push(scm8);

    cout<<DSchoolman<<endl;

    return 0;
}

