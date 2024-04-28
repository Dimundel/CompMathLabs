import math
import pandas as pd
from io import StringIO
import numpy as np
import matplotlib.pyplot as plt


def sum(t, x, n):
    res = x*t*t
    for i in range(0, n):
        l = math.pi * (1 / 2 + i)
        res += 2 / (l**6) * (math.exp(-l*l*t) + l*l*t - 1) * math.cos(l*x)
    return res

# analytical_sol_50 = []

# for t in np.arange(0, 10.02, 0.01):
#     for x in np.linspace(0, 1, 50):
#         analytical_sol_50.append(sum(t, x, 100))



# analytical_sol_75 = []

# for t in np.arange(0, 10.02, 0.01):
#     for x in np.linspace(0, 1, 75):
#         analytical_sol_75.append(sum(t, x, 100))

# analytical_sol_100 = []

# for t in np.arange(0, 10.02, 0.01):
#     for x in np.linspace(0, 1, 100):
#         analytical_sol_100.append(sum(t, x, 100))

# analytical_sol_200 = []

# for t in np.arange(0, 10.02, 0.01):
#     for x in np.linspace(0, 1, 200):
#         analytical_sol_200.append(sum(t, x, 100))

# analytical_sol_500 = []

# for t in np.arange(0, 10.02, 0.01):
#     for x in np.linspace(0, 1, 500):
#         analytical_sol_500.append(sum(t, x, 100))

# analytical_sol_1000 = []

# for t in np.arange(0, 10.02, 0.01):
#     for x in np.linspace(0, 1, 1000):
#         analytical_sol_1000.append(sum(t, x, 100))

# analytical_sol_2000 = []

# for t in np.arange(0, 10.02, 0.01):
#     for x in np.linspace(0, 1, 50):
#         analytical_sol_2000.append(sum(t, x, 100))

def read_blocks(file_path):
    # Открываем файл для чтения
    with open(file_path, 'r') as file:
        content = file.read()
    
    # Разделяем содержимое на блоки
    blocks = content.strip().split('\n\n')

    all_data = []  # список для хранения всех вторых чисел

    # Обрабатываем каждый блок
    for block in blocks:
        lines = block.strip().split('\n')[1:]  # Пропускаем первую строку каждого блока

        # Добавляем второе число каждой строки блока в общий список
        for line in lines:
            parts = line.split()
            if len(parts) > 1:  # Убедимся, что в строке есть хотя бы два элемента
                all_data.append(float(parts[1]))  # Сохраняем только второе число

    return all_data

# Используем функцию
# data_50 = read_blocks("data50.txt")
data_75 = read_blocks("data75.txt")
# data_100 = read_blocks("data100.txt")
# data_200 = read_blocks("data200.txt")
# data_500 = read_blocks("data500.txt")
# data_1000 = read_blocks("data1000.txt")
# data_2000 = read_blocks("data2000.txt")

def max_abs_difference(array1, array2):
    if len(array1) != len(array2):
        raise ValueError("Массивы должны быть одинаковой длины")

    # Вычисляем модули разностей элементов двух массивов
    differences = [abs(a - b) for a, b in zip(array1, array2)]

    # Возвращаем максимальное значение из полученных модулей разностей
    return max(differences)

# print(max_abs_difference(analytical_sol_50, data_50))
# print(max_abs_difference(analytical_sol_75, data_75))
# print(max_abs_difference(analytical_sol_100, data_100))
# print(max_abs_difference(analytical_sol_200, data_200))
# print(max_abs_difference(analytical_sol_500, data_500))
# print(max_abs_difference(analytical_sol_1000, data_1000))
# print(max_abs_difference(analytical_sol_50, data_50))

# Вывод результатов для проверки
# print("Данные каждого блока:")
# for index, block in enumerate(all_data):
#     print(f"Блок {index + 1}:")
#     for line in block:
#         print(line)
#     print("-" * 40)

def get_max_error(nx, file):
    analytical_sol = []
    for t in np.arange(0, 10.02, 0.01):
        for x in np.linspace(0, 1, nx):
            analytical_sol.append(sum(t, x, 100))
    data = read_blocks(file)
    return max_abs_difference(analytical_sol, data)

# print(get_max_error(85, "data85.txt"))

max_errors = np.array([0.003863264830977986, 0.0013532650364793675, 0.0005965983013078713, 0.00046741730154131744, 0.0003506826468751001, 0.00024071412145509896])
h = [1 / 30, 1 / 50, 1/75, 1 / 85, 1 / 100, 1/125]
k = np.polyfit(np.log(h), np.log(max_errors), 1)

plt.scatter(np.log(h), np.log(max_errors))
plt.grid()
plt.xlabel("Логарифм шага")
plt.ylabel("Логарифм ошибки")
plt.show()

print(k)