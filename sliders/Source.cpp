#include <iostream>
using namespace std;
const int hight = 5;
const int width = 6;
struct coords {
	int x = -1;
	int y = -1;
};

//проверка ввода
int input();

//проверка позиции (выход за пределы/заполненность другим ненулевым значением)
bool check_position(int** array, coords* coord);

//выбор направления хода бота
int bots_direction(int** array, coords* begin, coords* end, int* max_deep);

double minmax(int** array, coords* begin, coords* end, int deep, int* max_deep);

//оценка позиций в которые нас привел минимакс
double appraisal(int** array, coords* end, coords* begin);

//выделение доступных ячеек с позиций в которые нас привел минимакс
int available_cells(int** array, coords* begin, coords* end);

//считывание хода игрока
int read_move(int** array, coords* begin, coords* end, int mod = 1);

//передввижение конца/начала ползунка в заданном направлении
int move(int** array, coords* begin, coords* end, int direction);

//для создания кнопок в GUI
void access(coords* coord);

void start(int** array, coords* begin, coords* end, int* max_deep, bool priority);

int main() {
	int max_deep = 5;
	coords* begin = new coords;
	coords* end = new coords;
	int** array = new int* [hight];
	for (int i = 0; i < hight; i++) {
		array[i] = new int[width];
		for (int j = 0; j < width; j++) {
			array[i][j] = 0;
		}
	}
	bool priority = false;
	int endgame = 0;

	//выбор кто начинает игру
	cout << "Select difficulty level:" << endl;
	cout << "Enter 0, start the game yourself" << endl;
	cout << "Enter 1, to let the bot start the game" << endl;
	do {
		max_deep = input();
	} while ((max_deep < 0) || (max_deep > 2));

	switch (max_deep) {
	case 0:
		priority = false;
		break;
	case 1:
		priority = true;
		break;
	}

	//выбор сложности
	cout << "Select difficulty level:" << endl;
	cout << "Enter 0 to select an Easy level" << endl;
	cout << "Enter 1 to select an Medium level" << endl;
	cout << "Enter 2 to select an Hard level" << endl;
	do {
		max_deep = input();
	} while ((max_deep < 0) || (max_deep > 2));
	switch (max_deep) {
	case 0:
		max_deep = 1;
		break;
	case 1:
		max_deep = 3;
		break;
	case 2:
		max_deep = 5;
		break;
	}

	//первые два хода
	start(array, begin, end, &max_deep, priority);
	//насмешка
	cout << "You alwase can surrendder - just Enter \'-2\' \n)" << endl;
	//основная часть
	do {
		if (priority) {
			endgame = move(array, begin, end, bots_direction(array, begin, end, &max_deep));
			priority = false;
		}
		else {
			//вывод на экран матрицы
			cout << endl;
			for (int i = 0; i < hight; i++) {
				for (int j = 0; j < width; j++) {
					if ((j == begin->x) && (i == begin->y)) {
						cout << 'b';
					}else if ((j == end->x) && (i == end->y)) {
						cout << 'e';
					}
					else {
						cout << array[i][j];
					}
				}
				cout << endl;
			}
			cout <<"end: " << end->x << " " << end->y << "  begin: " << begin->x << " " << begin->y << endl;
			endgame = move(array, begin, end, read_move(array, begin, end, 1));
			priority = true;
		}

	} while (endgame == 0);

	//вывод результатов игры
	if ((priority) && (endgame != 2)) {
		//loser
		cout << "Oh NO!" << endl;
	}
	else {
		//win
		cout << "HAha im the best" << endl;
	}
}

int input() {
	int value = 0;
	while (true)
	{
		cin >> value;
		if (cin.good())
		{
			cin.ignore(10, '\n');
			break;
		}
		cin.clear();
		cout << "Incorrect input" << endl;
		cout << "try again" << endl;
		cin.ignore(10, '\n');
	}
	return value;
}

bool check_position(int** array, coords* coord) {
	if ((coord->x >= width) || (coord->y >= hight) || (coord->x < 0) || (coord->y < 0)) {
		return false;
	}
	return (array[coord->y][coord->x] == 0) ? true : false;
}

void start(int** array, coords* begin, coords* end, int* max_deep, bool priority) {
	if (priority) {
		//генерация начальных координат
		srand(time(NULL));
		end->x = rand() % width;
		srand(time(NULL));
		end->y = rand() % hight;
		array[end->y][end->x] = 1;

		for (int i = 0; i < hight; i++) {
			for (int j = 0; j < width; j++) {
				cout << array[i][j];
			}
			cout << endl;
		}
		cout << endl;
		//считывание второй координаты ползунка
		switch (read_move(array, begin, end, 1)) {
		case 0:
			begin->x = end->x + 1;
			begin->y = end->y;
			break;
		case 1:
			begin->x = end->x;
			begin->y = end->y + 1;
			break;
		case 2:
			begin->x = end->x - 1;
			begin->y = end->y;
			break;
		case 3:
			begin->x = end->x;
			begin->y = end->y - 1;
			break;
		}
	}
	else {
		//выбор начальной координаты
		int choise = read_move(array, begin, end, 0);
		end->x = choise % width;
		end->y = choise / width;
		array[end->y][end->x] = 1;
		//определение второй координаты
		switch (bots_direction(array, begin, end, max_deep)) {
		case 0:
			begin->x = end->x + 1;
			begin->y = end->y;
			break;
		case 1:
			begin->x = end->x;
			begin->y = end->y + 1;
			break;
		case 2:
			begin->x = end->x - 1;
			begin->y = end->y;
			break;
		case 3:
			begin->x = end->x;
			begin->y = end->y - 1;
			break;
		}
	}
	array[begin->y][begin->x] = 1;
}

int bots_direction(int** array, coords* begin, coords* end, int* max_deep) {
	int choise = -1;
	double best_chances = 0;
	for (int i = 0; i < 8; i++) {
		double local_res = -1;
		switch (i) {
		case 0:
			end->x++;
			if (check_position(array, end)) {
				array[end->y][end->x] = 1;
				local_res = minmax(array, begin, end, 1, max_deep);
				array[end->y][end->x] = 0;
			}
			end->x--;
			break;
		case 1:
			end->y++;
			if (check_position(array, end)) {
				array[end->y][end->x] = 1;
				local_res = minmax(array, begin, end, 1, max_deep);
				array[end->y][end->x] = 0;
			}
			end->y--;
			break;
		case 2:
			end->x--;
			if (check_position(array, end)) {
				array[end->y][end->x] = 1;
				local_res = minmax(array, begin, end, 1, max_deep);
				array[end->y][end->x] = 0;
			}
			end->x++;
			break;
		case 3:
			end->y--;
			if (check_position(array, end)) {
				array[end->y][end->x] = 1;
				local_res = minmax(array, begin, end, 1, max_deep);
				array[end->y][end->x] = 0;
			}
			end->y++;
			break;
		case 4:
			begin->x++;
			if (check_position(array, begin)) {
				array[begin->y][begin->x] = 1;
				local_res = minmax(array, begin, end, 1, max_deep);
				array[begin->y][begin->x] = 0;
			}
			begin->x--;
			break;
		case 5:
			begin->y++;
			if (check_position(array, begin)) {
				array[begin->y][begin->x] = 1;
				local_res = minmax(array, begin, end, 1, max_deep);
				array[begin->y][begin->x] = 0;
			}
			begin->y--;
			break;
		case 6:
			begin->x--;
			if (check_position(array, begin)) {
				array[begin->y][begin->x] = 1;
				local_res = minmax(array, begin, end, 1, max_deep);
				array[begin->y][begin->x] = 0;
			}
			begin->x++;
			break;
		case 7:
			begin->y--;
			if (check_position(array, begin)) {
				array[begin->y][begin->x] = 1;
				local_res = minmax(array, begin, end, 1, max_deep);
				array[begin->y][begin->x] = 0;
			}
			begin->y++;
			break;
		}
		//определение лучшего направления
		if (local_res > best_chances) {
			best_chances = local_res;
			choise = i;
		}
		else if ((local_res == best_chances) && (local_res > 0)) {
			coords* challenger = new coords;
			coords* current = new coords;

			switch (i) {
			case 0:
				challenger->x = end->x + 1;
				challenger->y = end->y;
				break;
			case 1:
				challenger->x = end->x;
				challenger->y = end->y + 1;
				break;
			case 2:
				challenger->x = end->x - 1;
				challenger->y = end->y;
				break;
			case 3:
				challenger->x = end->x;
				challenger->y = end->y - 1;
				break;
			case 4:
				challenger->x = begin->x + 1;
				challenger->y = begin->y;
				break;
			case 5:
				challenger->x = begin->x;
				challenger->y = begin->y + 1;
				break;
			case 6:
				challenger->x = begin->x - 1;
				challenger->y = begin->y;
				break;
			case 7:
				challenger->x = begin->x;
				challenger->y = begin->y - 1;
				break;
			}

			switch (choise) {
			case 0:
				current->x = end->x + 1;
				current->y = end->y;
				break;
			case 1:
				current->x = end->x;
				current->y = end->y + 1;
				break;
			case 2:
				current->x = end->x - 1;
				current->y = end->y;
				break;
			case 3:
				current->x = end->x;
				current->y = end->y - 1;
				break;
			case 4:
				current->x = begin->x + 1;
				current->y = begin->y;
				break;
			case 5:
				current->x = begin->x;
				current->y = begin->y + 1;
				break;
			case 6:
				current->x = begin->x - 1;
				current->y = begin->y;
				break;
			case 7:
				current->x = begin->x;
				current->y = begin->y - 1;
				break;
			}
			int challenger_appraisal = appraisal(array, challenger, challenger);
			array[challenger->y][challenger->x] = 0;
			int current_appraisal = appraisal(array, current, current);
			array[current->y][current->x] = 0;
			choise = (challenger_appraisal > current_appraisal) ? i : choise;
		}

	}

	return choise;
}

double minmax(int** array, coords* begin, coords* end, int deep, int* max_deep) {
	if (deep == *max_deep) {
		return appraisal(array, end, begin);
	}

	double res;
	double local_res;
	res = (deep % 2) ? 0 : 1;
	for (int i = 0; i < 8; i++) {
		local_res = -1;
		switch (i) {
		case 0:
			end->x++;
			if (check_position(array, end)) {
				array[end->y][end->x] = 1;
				local_res = minmax(array, begin, end, deep + 1, max_deep);
				array[end->y][end->x] = 0;
			}
			end->x--;
			break;
		case 1:
			end->y++;
			if (check_position(array, end)) {
				array[end->y][end->x] = 1;
				local_res = minmax(array, begin, end, deep + 1, max_deep);
				array[end->y][end->x] = 0;
			}
			end->y--;
			break;
		case 2:
			end->x--;
			if (check_position(array, end)) {
				array[end->y][end->x] = 1;
				local_res = minmax(array, begin, end, deep + 1, max_deep);
				array[end->y][end->x] = 0;
			}
			end->x++;
			break;
		case 3:
			end->y--;
			if (check_position(array, end)) {
				array[end->y][end->x] = 1;
				local_res = minmax(array, begin, end, deep + 1, max_deep);
				array[end->y][end->x] = 0;
			}
			end->y++;
			break;
		case 4:
			begin->x++;
			if (check_position(array, begin)) {
				array[begin->y][begin->x] = 1;
				local_res = minmax(array, begin, end, deep + 1, max_deep);
				array[begin->y][begin->x] = 0;
			}
			begin->x--;
			break;
		case 5:
			begin->y++;
			if (check_position(array, begin)) {
				array[begin->y][begin->x] = 1;
				local_res = minmax(array, begin, end, deep + 1, max_deep);
				array[begin->y][begin->x] = 0;
			}
			begin->y--;
			break;
		case 6:
			begin->x--;
			if (check_position(array, begin)) {
				array[begin->y][begin->x] = 1;
				local_res = minmax(array, begin, end, deep + 1, max_deep);
				array[begin->y][begin->x] = 0;
			}
			begin->x++;
			break;
		case 7:
			begin->y--;
			if (check_position(array, begin)) {
				array[begin->y][begin->x] = 1;
				local_res = minmax(array, begin, end, deep + 1, max_deep);
				array[begin->y][begin->x] = 0;
			}
			begin->y++;
			break;
		}
		//выбор лучшего значения
		if (local_res != -1) {
			//ход бота выбираем лучшее значение
			if (deep % 2) {
				if (local_res > res) { res = local_res; }
				//досрочный выход из минимакса
				if (res == 1) { break; }
			}
			//ход игрока выбираем худшее значение
			else {
				if (local_res < res) { res = local_res; }
				//досрочный выход из минимакса
				if (res == 0) { break; }
			}
		}
	}
	return res;
}

double appraisal(int** array, coords* end, coords* begin) {
	double res = 0;
	int count = 0;

	double end_res = 0;
	int end_count = 0;
	double begin_res = 0;
	int begin_count = 0;
	double* local_res;
	int* local_count;

	for (int i = 0; i < 8; i++) {
		int available_cells_count = 0;
		switch (i) {
		case 0:
			end->x++;
			if (check_position(array, end)) { available_cells_count = available_cells(array, begin, end); }
			end->x--;
			break;
		case 1:
			end->y++;
			if (check_position(array, end) && array[end->y][end->x] != 2) { available_cells_count = available_cells(array, begin, end); }
			end->y--;
			break;
		case 2:
			end->x--;
			if (check_position(array, end) && array[end->y][end->x] != 2) { available_cells_count = available_cells(array, begin, end); }
			end->x++;
			break;
		case 3:
			end->y--;
			if (check_position(array, end) && array[end->y][end->x] != 2) { available_cells_count = available_cells(array, begin, end); }
			end->y++;
			break;
		case 4:
			begin->x++;
			if (check_position(array, begin) && array[begin->y][begin->x] != 2) { available_cells_count = available_cells(array, begin, end); }
			begin->x--;
			break;
		case 5:
			begin->y++;
			if (check_position(array, begin) && array[begin->y][begin->x] != 2) { available_cells_count = available_cells(array, begin, end); }
			begin->y--;
			break;
		case 6:
			begin->x--;
			if (check_position(array, begin) && array[begin->y][begin->x] != 2) { available_cells_count = available_cells(array, begin, end); }
			begin->x++;
			break;
		case 7:
			begin->y--;
			if (check_position(array, begin) && array[begin->y][begin->x] != 2) { available_cells_count = available_cells(array, begin, end); }
			begin->y++;
			break;
		}
		local_res = (i <= 3) ? (&end_res) : (&begin_res);
		local_count = (i <= 3) ? (&end_count) : (&begin_count);
		//подсчет доступных координат
		if (available_cells_count != 0) {
			*local_count += 1;
			if (available_cells_count > 10) {
				*local_res += 0.5;
			}
			else {
				if (available_cells_count % 2) {
					if (available_cells_count > 5) {
						*local_res += 0.8;
					}
					else {
						*local_res += 1;
					}
				}
				else {
					if (available_cells_count > 5) {
						*local_res += 0.2;
					}
				}
			}
		}
	}
	count = end_count + begin_count;
	if (count != 0) {
		//если оба направления проигрышные и не связаны между собой, то игрок зайдет в тупик
		if ((end_res == 0) && (end_count != 0) && (begin_res == 0) && (begin_count != 0)) {
			res = 1;
		}
		//иначе берем среднюю вероятность
		else {
			res = (end_res + begin_res) / count;
		}
	}
	//если это тупик, то бот проиграл
	else {
		res = 0;
	}
	//очистка масива
	for (int i = 0; i < hight; i++) {
		for (int j = 0; j < width; j++) {
			if (array[i][j] == 2) {
				array[i][j] = 0;
			}
		}
	}
	if (check_position(array, begin)) {
		array[begin->y][begin->x] = 1;
	}
	if (check_position(array, end)) {
		array[end->y][end->x] = 1;
	}
	return res;
}

int available_cells(int** array, coords* begin, coords* end) {
	int res = 1;
	if (check_position(array, begin)) {
		array[begin->y][begin->x] = 2;
	}
	if (check_position(array, end)) {
		array[end->y][end->x] = 2;
	}
	for (int i = 0; i < 8; i++) {
		switch (i) {
		case 0:
			end->x++;
			if (check_position(array, end)) { res += available_cells(array, begin, end); }
			end->x--;
			break;
		case 1:
			end->y++;
			if (check_position(array, end)) { res += available_cells(array, begin, end); }
			end->y--;
			break;
		case 2:
			end->x--;
			if (check_position(array, end)) { res += available_cells(array, begin, end); }
			end->x++;
			break;
		case 3:
			end->y--;
			if (check_position(array, end)) { res += available_cells(array, begin, end); }
			end->y++;
			break;
		case 4:
			begin->x++;
			if (check_position(array, begin)) { res += available_cells(array, begin, end); }
			begin->x--;
			break;
		case 5:
			begin->y++;
			if (check_position(array, begin)) { res += available_cells(array, begin, end); }
			begin->y--;
			break;
		case 6:
			begin->x--;
			if (check_position(array, begin)) { res += available_cells(array, begin, end); }
			begin->x++;
			break;
		case 7:
			begin->y--;
			if (check_position(array, begin)) { res += available_cells(array, begin, end); }
			begin->y++;
			break;
		}
	}
	return res;
}

int read_move(int** array, coords* begin, coords* end, int mod) {
	int res = 0;
	int imp = 0;
	switch (mod) {
	case 0:
		//выбор начальных координат
		cout << "Enter coords of sliders begin" << endl;
		do {
			cout << "Enter x-coords 0-" << width - 1 << endl;
			imp = input();
		} while ((imp < 0) || (imp >= width));
		res += imp;
		do {
			cout << "Enter y-coords 0-" << hight - 1 << endl;
			imp = input();
		} while ((imp < 0) || (imp >= hight));
		res += imp * width;
		break;
	case 1:
		bool directions[8];
		bool can_move = false;
		//вывод доступных ходов
		for (int i = 0; i < 8; i++) {
			switch (i) {
			case 0:
				end->x++;
				if (check_position(array, end)) {
					cout << "Enter " << i << " to move the sliders END to the RIGHT" << endl;
					directions[i] = true;
					can_move = true;
				}
				end->x--;
				break;
			case 1:
				end->y++;
				if (check_position(array, end)) {
					cout << "Enter " << i << " to move the sliders END DOWN" << endl;
					directions[i] = true;
					can_move = true;
				}
				end->y--;
				break;
			case 2:
				end->x--;
				if (check_position(array, end)) {
					cout << "Enter " << i << " to move the sliders END to the LEFT" << endl;
					directions[i] = true;
					can_move = true;
				}
				end->x++;
				break;
			case 3:
				end->y--;
				if (check_position(array, end)) {
					cout << "Enter " << i << " to move the sliders END UP" << endl;
					directions[i] = true;
					can_move = true;
				}
				end->y++;
				break;
			case 4:
				begin->x++;
				if (check_position(array, begin)) {
					cout << "Enter " << i << " to move the sliders BEGIN to the RIGHT" << endl;
					directions[i] = true;
					can_move = true;
				}
				begin->x--;
				break;
			case 5:
				begin->y++;
				if (check_position(array, begin)) {
					cout << "Enter " << i << " to move the sliders BEGIN DOWN" << endl;
					directions[i] = true;
					can_move = true;
				}
				begin->y--;
				break;
			case 6:
				begin->x--;
				if (check_position(array, begin)) {
					cout << "Enter " << i << " to move the sliders BEGIN to the LEFT" << endl;
					directions[i] = true;
					can_move = true;
				}
				begin->x++;
				break;
			case 7:
				begin->y--;
				if (check_position(array, begin)) {
					cout << "Enter " << i << " to move the sliders BEGIN UP" << endl;
					directions[i] = true;
					can_move = true;
				}
				begin->y++;
				break;
			}
		}
		if (can_move) {
			do {
				res = input();
			} while (!((res >= 0) && (res < 8) && (directions[res]) || (res == -2)));
		}
		else {
			res = -1;
		}
		break;
	}
	return res;
}

int move(int** array, coords* begin, coords* end, int direction) {
	switch (direction) {
	case -2:
		return 2;
	case -1:
		return 1;
	case 0:
		end->x++;
		break;
	case 1:
		end->y++;
		break;
	case 2:
		end->x--;
		break;
	case 3:
		end->y--;
		break;
	case 4:
		begin->x++;
		break;
	case 5:
		begin->y++;
		break;
	case 6:
		begin->x--;
		break;
	case 7:
		begin->y--;
		break;
	}
	array[begin->y][begin->x] = 1;
	array[end->y][end->x] = 1;
	return 0;
}