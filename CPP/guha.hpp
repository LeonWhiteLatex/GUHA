#pragma once
#include <string>
#include <vector>

///@{
extern std::vector<std::string> rules, /**<Письменное представление правил, можно прочитать напрямую из вектора*/names; /**<Наименование атрибутов*/
///@}
///@{
extern std::vector<int> nodes, /**<Объекты*/msk; /**<Маски атрибутов*/
///@}

///@{
extern float all, /**<Пороговое количество пересечений*/character, /**<Пороговое значение отношения для ассоциации*/depends; /**<Пороговое значение разницы при формировании зависимостей*/
///@}

void read_file(std::string);

void rnd(std::vector<std::string>, int); /**<Генератор случайных объектов. Функция задействует @ref mask_gen "генерацию масок" и формирует некоторое количество объектов для сгенерированных масок в @ref nodes "наборе объектов".
@param a - Набор строк (названия масок)
@param o - Количество объектов*/
void mask_gen(std::vector<std::string>); /**<Генератор масок по названиям атрибутов. Функция вносит ввод в вектор @ref names "имен" и формирует маски в векторе @ref msk "масок".
@param n - Набор строк названий масок*/
void guha(unsigned int p=0); /**<Поиск правил. Функция использует вектор @ref msk "масок", чтобы прогнать вектор @ref nodes "объектов", и внести результаты в вектор @ref rules "правил".
@param p - Ограничение глубины прохода*/

//void out(unsigned int);
std::vector<int> get_node(int);

std::vector<std::string> get_names(long unsigned int);
