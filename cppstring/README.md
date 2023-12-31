# String
Упрощенный аналог стандартного класса `std::string`

Детали реализации класса String со следующим интерфейсом:
* Конструктор, принимающий С-style строку и создающий на ее основе строку<br>
* Конструктор, принимающий массив char и количество первых символов (size), которые нужно скопировать<br>
* Правило "трех"<br>
* Константный и неконстантный оператор доступа по индексу []<br>
* Константный и неконстантный метод доступа по индексу At. При выходе за границы строки должен бросать исключение StringOutOfRange<br>
* Методы `Front()` и `Back()` (тоже по две версии)<br>
* Методы `CStr()` и `Data()`<br>
* Метод `Empty()`<br>
* Методы `Size()` и `Length(()`<br>
размер строки.
* Метод `Capacity()`
* Метод `Clear()` (удаления выделенной памяти при НЕ происходит)<br>
* Метод `Swap(String& other)`<br>
* Метод `PopBack()`<br>
* Метод `PushBack(symbol)` (при необходимости строка расширяется)<br>
* `operator+=(const String& other)`<br>
* Метод `Resize(new_size, symbol)`<br>
* Метод `Reserve(new_capacity)` изменяет вместимость на max(new_capacity, текущая вместимость)<br>
* Метод `ShrinkToFit()`<br>
* Конкатенация с помощью операции +<br>
* Операции сравнения (<, >, <=, >=, ==, !=), задающие лексикографический порядок<br>
* Операция вывода в поток. <br>
