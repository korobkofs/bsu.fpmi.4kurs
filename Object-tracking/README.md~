1. Установка
Клонируем репозиторий
git clone ssh://<login>@git.promwad.com/repos/prj.promwad.Object-tracking-Internal.git
Устанавливаем необходимые пакеты:
* OpenCV
* CMake
* libconfig++ (optional)
* Qt4 (optional)

2. Сборка
cd prj.promwad.Object-tracking-Internal
mkdir build
cd build
cmake .. -DBUILD_QOPENTLD=ON -DUSE_SYSTEM_LIBS=OF
make

3. Запуск
cd bin
./opentld -d VID -i <path to video file> -s
