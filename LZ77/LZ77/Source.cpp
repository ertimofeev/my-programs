#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;
/*
	считывание с нерационального файла (реализовать возможность выбора файла для считывания)
	ссчитывание с файла построенного по алгоритму LZ77
*/
struct element {
	char ch;
	element* next;
	element* previous;
};
struct stack_ {
	element* first;
	unsigned long size = 0;
};
struct result {
	unsigned long pos;
	unsigned long a;
	unsigned long b;
	element* el;
};
struct bufur {
	element* first;
	element* last;
	unsigned long size = 0;
};
struct word {
	element* first;
	//unsigned long size = 0;
};


/*stack_* create_stack(stack_* st) {
	ifstream fin("C:\\Users\\user\\source\\repos\\LZ77\\LZ771.txt");
	element* el1;
	element* el2 = new element;

	cout << "Вводите текст \n" << "Чтобы окончить ввод текста введите # и нажмите Enter" << endl;

	scanf_s("%c", &el2->ch);
	st->first = el2;
	while (el2->ch != '#') {
		el1 = el2;
		scanf_s("%c", &el2->ch);
		el1->next = el2;
		el2->previous = el1;
		++st->size;
	}
	
	delete el2;

	fin.close();
	return st;
}*/

stack_* read_stack(stack_* st) {

	ifstream fin("C:\\Users\\user\\source\\repos\\LZ77\\LZ771.txt");
	cout << fin.tellg() << endl;
	element* el1;
	element* el2 = new element;

	do {
		if (st->size == 0) {
			el2->ch = fin.get();
			st->first = el2;
		}
		else {
			el1 = el2;
			el2 = new element;
			el2->ch = fin.get();
			el1->next = el2;
			el2->previous = el1;
		}
		++st->size;
	} while (el2->ch != EOF);
	cin.ignore();
	cout << "\n";
	delete el2;
	--st->size;

	return st;
}

stack_* create_stack(stack_* st) {
	cout << "Вводите текст \n"<< "Чтобы окончить ввод текста введите # и нажмите Enter" << endl;
	
	element* el1;
	element* el2 = new element;

	do {
		if (st->size == 4294967295) {
			cout << "Извините, но мы не работаем с текстами большего размера";
			el2 = new element;
			el2->ch = '#';
		}
		else {
			if (st->size == 0) {
				scanf_s("%c", &el2->ch);
				st->first = el2;
			}
			else {
				el1 = el2;
				el2 = new element;
				scanf_s("%c", &el2->ch);
				el1->next = el2;
				el2->previous = el1;
			}
		}
		++st->size;
	} while (el2->ch != '#');
	cin.ignore();
	delete el2;
	--st->size;

	return st;
}

result LZ77(element* el1, unsigned long word, unsigned long bufur, unsigned long pozition) {
	result res;
	res.a = 0;
	res.b = 0;
	bool k = 0;
	unsigned long m;
	cout << el1->ch << endl;

	while ((word != 0) && (!k)) {
		element* el2 = el1;
		m = 0;
		cout << "Word = " << word << endl;
		for (unsigned long i = 0; i < word - 1; i++) {
			el2 = el2->next;
		}
		element* el3 = el1->previous;
		cout << el2->ch << endl;
		for (long long i = pozition - 1; i >= pozition - (bufur - word) - 1; i--) { //unsigned long 0 - 1 = 4294967295
			cout << "i = " << i << endl;
			cout << el3->ch << endl;
			if (el2->ch == el3->ch) {
				m = 1;
				cout << "'";
				for (unsigned long j = 1; j < word; j++) {
					el3 = el3->previous;
					el2 = el2->previous;
					if (el3->ch == el2->ch) {
						m++;
					}
					else {
						j = word;
					}
				}
			}
			cout << " m = " << m << endl;
			if (m == word) {
				res.a = word - (i - pozition) - 1;
				res.b = word;
				k = 1;
				i = -1;
			}
			else {
				m = 0;
			}
			el3 = el3->previous;
		}
		if (!k) {
			--word;
		}
	}
	res.pos = pozition + word + 1;
	for (unsigned long i = 0; i < word; i++) {
		el1 = el1->next;
	}
	res.el = el1;
	return res;
}

void fout(stack_* st) {
	unsigned long word = 0, bufur = 0;
	element* el1 = st->first;
	result res;
	ofstream fout("C:\\Users\\user\\source\\repos\\LZ77\\LZ77.txt", ios::binary);
	//ofstream fout("LZ77.txt", ios::binary);
	cout << "Enter size of dictionary and bufur\n";
	do {
		cout << "dictionary must be less than buffer\n";
		cin >> word >> bufur;
	} while (word > bufur);
	cout << endl;
	//fout << word << bufur;
	fout.write((char*)&bufur, sizeof(bufur));
	fout.write((char*)&word, sizeof(word));
	fout.write((char*)&st->size, sizeof(st->size));

	for (unsigned long i = 0; i < st->size; ) {
		cout << i << endl;
		if (i < bufur) {
			cout << "1";
			if (st->size - i <= word) {
				cout << ".1\n";
				res = LZ77(el1, st->size - i - 1, i, i);
			}
			else if (i < word) {
				cout << ".2\n";
				res = LZ77(el1, i, i, i);
			}
			else {
				cout << ".3\n";
				res = LZ77(el1, word, i, i);
			}
		}
		else if (st->size - i <= word) {
			cout << "2\n";
			res = LZ77(el1, st->size - i - 1, bufur, i);
		}
		else {
			cout << "3\n";
			res = LZ77(el1, word, bufur, i);
		}
		i = res.pos;
		//fout << res.a << res.b << res.el->ch;
		//fout.write((char*)&res, sizeof(res));
		fout.write((char*)&res.a, sizeof(res.a));
		fout.write((char*)&res.b, sizeof(res.b));
		fout.write((char*)&res.el->ch, sizeof(res.el->ch));
		cout << res.a << res.b << res.el->ch << endl;
		cout << endl;
		el1 = res.el->next;
	}

	fout.close();
}

void decipher() {
	ifstream fin("C:\\Users\\user\\source\\repos\\LZ77\\LZ77.txt", ios::binary);
	ofstream fout("LZ771.txt");
	bool k = 0;
	unsigned long buf = 0, w = 0;
	unsigned long b_=0, w_ = 0;
	unsigned long size_ = 0;
	char c_;
	fin.read((char*)&buf, sizeof(buf));
	fin.read((char*)&w, sizeof(w));
	fin.read((char*)&size_, sizeof(size_));
	cout << buf << w << size_ << "\n";
	bufur* bufur_ = new bufur;
	word* word_ = new word;
	element* el1;
	element* el2;
	element* el3 = new element;
	do {
		fin.read((char*)&b_, sizeof(b_));
		fin.read((char*)&w_, sizeof(w_));
		fin.read((char*)&c_, sizeof(c_));
		cout << b_ << w_ << c_;
		if ((w_ > b_) || (b_ > bufur_->size) || (w_ > w) || (b_ > buf)) {
			k = 1;
			cout << "Этот файл не является продуктом алгоритма" << endl;
		}
		else {
			if (bufur_->size < buf) {
				if (bufur_->size == 0) {
					el3->ch = c_;
					bufur_->first = el3;
					bufur_->last = el3;
					word_->first = el3;
				}
				else {
					el1 = bufur_->last;
					for (unsigned long i = 1; i < w_; i++) {
						el1 = el1->previous;
					}
					el2 = new element;
					word_->first = el2;
					for (unsigned long i = 0; i < w_; i++) {//считывание word_ с bufur_
						el2->ch = el1->ch;
						if (i == 0) {
							word_->first = el2;
						}
						else {
							el2->previous = el3;
							el3->next = el2;
						}
						el3 = el2;
						el1 = el1->next;
						el2 = new element;
					}
					el2->ch = c_;//добавление c_ в конец word
					el2->previous = el3;
					el3->next = el2;

					bufur_->last->next = word_->first; //добавление word_ в конец bufur_
					word_->first->previous = bufur_->last;
					bufur_->last = el2;

				}

				bufur_->size += w_ + 1;

				///////запись в файл
				for (unsigned long i = 0; i < w_; i++) {
					fout << word_->first->ch;
					word_->first = word_->first->next;
				}

				if (bufur_->size > buf) {
					for ( ; buf < bufur_->size; buf--) {//очистка bufur_ от лишних элементов
						el1 = bufur_->first;
						bufur_->first = bufur_->first->next;
						delete el1;
					}
					bufur_->size = buf;
				}
			}
			else {
				el1 = bufur_->last;
				for (unsigned long i = 1; i < w_; i++) {
					el1 = el1->previous;
				}
				/*for (unsigned long i = 0; i < w_; i++) {//считывание word_ с bufur_
					el2 = new element;
					el2->ch = el1->ch;
					if (i == 0) {
						word_->first = el2;
					}
					else {
						el2->previous = el3;
						el3->next = el2;
					}
					el3 = el2;
					el1 = el1->next;
				}

				el1 = word_->first;
				for (unsigned long i = 0; i < w_; i++) {//добавление word_ в конец bufur_
					el1->previous = bufur_->last;
					bufur_->last->next = el1;
					bufur_->last = el1;
				}
				el1 = new element; //добавление c_ в bufur_
				el1->ch = c_;
				el1->previous = bufur_->last;
				bufur_->last->next = el1;
				bufur_->last = el1;*/
				el2 = new element;
				word_->first = el2;
				for (unsigned long i = 0; i < w_; i++) {//считывание word_ с bufur_
					el2->ch = el1->ch;///////////////////////////////////////////////////
					if (i == 0) {
						word_->first = el2;
					}
					else {
						el2->previous = el3;
						el3->next = el2;
					}
					el3 = el2;
					el1 = el1->next;
					el2 = new element;
				}
				el2->ch = c_;//добавление c_ в конец word
				el2->previous = el3;
				el3->next = el2;

				bufur_->last->next = word_->first; //добавление word_ в конец bufur_
				word_->first->previous = bufur_->last;
				bufur_->last = el2;

				///////запись в файл
				for (unsigned long i = 0; i < w_; i++) {
					fout << word_->first->ch;
					//cout << word_->first->ch;
					word_->first = word_->first->next;
				}

				for (unsigned long i = 0; i < w_ + 1; i++) {//очистка bufur_ от w_ + 1 элементов
					el1 = bufur_->first;
					bufur_->first = bufur_->first->next;
					delete el1;
				}
			}

		}
		
		size_ -= w_ + 1;
	} while ((size_ > 0)&&(!k));
	cout << "\nКонец дешифровки\n\n";

	fin.close();
	fout.close();
}

int main() {
	setlocale(LC_ALL, "rus");
	stack_* st = new stack_;
	element* el1;
	char choise;
	do {
		choise = '\0';
		cout << "Что вы хотите сделать? \n" << "Введите цифру, соответствующую пункту из предложенного списка\n\n";
		cout << "1 - зашифровать файл \n";
		cout << "2 - зашифровать текст, введенный с клавиатуры \n";
		cout << "3 - дешифровать текст в файл\n";
		cout << "4 - закончить работу программы\n";

		do {
			if (choise != '\0') {
				cout << "Вы ввели неверные данные" << endl;
			}
			scanf_s("%c", &choise);
		} while ((choise < '1') || (choise > '4'));
		cin.ignore();

		switch (choise)
		{
		case '1':
			st = read_stack(st);
			fout(st);
			break;
		case '2':
			st = create_stack(st);
			el1 = st->first;
			for (unsigned long i = 0; i < st->size; i++) {
				cout << el1->ch;
				el1 = el1->next;
			}
			cout << endl << st->size << "\n\n";
			fout(st);
			break;
		case '3':
			decipher();
			break;
		}
		if ((choise == '1') || (choise == '2')) {
			for (unsigned long i = 1; i < st->size; i++) {
				st->first = st->first->next;
				delete st->first->previous;
			}
			st->size = 0;
			delete st->first;
		}
	} while (choise != '4');
	cout << "The end";
	delete st;
	return 0;
}