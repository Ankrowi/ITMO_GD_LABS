Код игры разбит на 5 основных классов:
  1) God - синглтон, отвечающий за рандомизацию всех значений в игре
  2) FileSystem - отвечает за запись и чтение файлов сохранений, имеет собственно определенную структуру входных\выходных файлов.
  3) OutputHandler - отвечает за вывод текстовой информации в консоль
  4) InputHandler - отвечает за ввод текстовой информации из консоли, при вводе использует класс OutputHandler.
  5) GameManager - класс, содержащий основную игровую логику, использующий 4 вышеуказанных класса.
