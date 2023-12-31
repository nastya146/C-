# Vector
Шаблонный класс Vector с единственным шаблонным параметром - типом хранящихся элементов
Шаблонный класс MyIterator

**Детали реализации Vector**
* Конструктор по умолчанию<br>
* Явный конструктор от числа - создает массив заданного размера заполненный объектами, сконструированными по умолчанию<br>
* Конструктор, принимающий size и value <br>
* Шаблонный конструктор, принимающий пару итераторов - создает копию переданного диапазона (конструктор работает только в случае, когда на вход приходят два Forward итератора)<br>
* Конструктор от `std::initializer_list`<br>
* Правило "пяти"<br>
* Методы Size, Capacity, Empty <br>
* Константный и неконстантный оператор доступа по индексу []<br>
* Константный и неконстантный метод доступа по индексу At. При выходе за границы массива должен бросать исключение std::out_of_range <br>
* Методы `Front()` и `Back()`<br>
* Метод `Data()` возвращает указатель на начало массива <br>
* Метод Swap(other)
* Метод Resize(new_size) изменяет размер на new_size. Если вместимость не позволяет хранить столько элементов, то выделяется новый буфер с вместимостью new_size. Недостающие элементы конструируются по умолчанию.<br>
* Метод Resize(new_size, value)
* Метод Reserve(new_cap) изменяет вместимость на max(new_cap, текущая вместимость). Размер при этом не изменяется<br>
* Метод ShrinkToFit() уменьшает capacity до size. <br>
* Метод Clear()
* Методы PushBack(const T&) и PushBack(T&&)
* Метод PopBack()
* Операции сравнения (<, >, <=, >=, ==, !=), задающие лексикографический порядок.

**Детали реализации MyIterator**

Поддержка итераторов и методов для работы с ними: begin(), end(), cbegin(), cend(), rbegin(), rend(), crbegin(), crend(). begin()-end(), rbegin()-rend() имеют две версии, возвращающие константные и неконстантные итераторы.

**Безопасность**

Методы удовлетворяют строгой гарантии безопасности. То есть все методы либо работают без сбоев, либо не изменяют исходного состояния вектора. 

При решении  предполагается, что конструкторы перемещения, перемещающие присваивания и деструкторы объектов никогда не бросают исключений

