Устанавливаем необходимые пакеты:
* OpenCV
* CMake
* libconfig++ (optional)
* Qt4 (optional)

Сборка:
$ cd Object-tracking
$ mkdir build
$ cd build
$ cmake .. -DBUILD_QOPENTLD=ON -DUSE_SYSTEM_LIBS=OF
$ make

Запуск:
$ cd bin
$ ./opentld -d VID -i <path to video file> -s
