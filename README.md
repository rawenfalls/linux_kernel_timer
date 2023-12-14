# Написать модуль ядра линукс, который каждую минуту сохраняет текущее время в файле /tmp/current_time в формате hh:mm (например, 11:58 или 22:07).
## Версия ядра (6.2.0-39-generic)
## Ссылка на youtube: 
https://www.youtube.com/watch?v=4d-wZJQlpfU
```
uname -r
```
### Сборка
```
sudo make
```
### Очистка 
```
make clean
```
### Добавление модуля в ядро
```
sudo insmod my_time_kernel.ko
```
### Удаление модуля из ядра
```
sudo rmmod my_time_kernel.ko
```
### Поиск добавленного модуля ядра
```
sudo lsmod | grep my_time_kernel
```
### Посмотреть вывод модуля
```
sudo journalctl --since "1 hour ago" | grep kernel
```
### Вывести в консоль файл в который записывается время
```
cat /tmp/current_time
```