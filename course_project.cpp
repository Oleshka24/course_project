#include <iostream>
#include <windows.h>
#include <time.h>
#include <string>
#include <regex>

#define ORDERS "orders.dat"
#define STAFF "staff.dat"
#define SERVICES "services.dat"

#define POS_KOL 3

using namespace std;

struct orders {
	size_t	ID;
	char	fullName[20];
	size_t	phoneNumb[9];
	time_t	dateOfOrder[3];
	size_t	servNumbers[5];
};

struct employee {
	size_t	ID;
	char	fullName[20];
	size_t	phoneNumb[9];
	size_t	position[POS_KOL];
};

struct service {
	size_t	ID;
	size_t	employee;
	size_t	category;
	char	title[40];
	int		price;
	size_t	term;
};

struct position {
	size_t ID;
	char occupation[20];
};

position pos[POS_KOL] = {
	{1, "Дизайн"},
	{2, "Программирование"},
	{3, "SEO-оптимизация"}
};

FILE* dataFile;

void navigation(size_t);
void inputVal(size_t);
void output();

string inputCurrentVal(string*, string*);

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "rus");

	fopen_s(&dataFile, ORDERS, "a+");
	fclose(dataFile);
	fopen_s(&dataFile, STAFF, "a+");
	fclose(dataFile);
	fopen_s(&dataFile, SERVICES, "a+");
	fclose(dataFile);

	int navPos;
	do {
		do {
			cout << "\n\tМеню программы."
				<< "\n\t1 - Работа с заказами."
				<< "\n\t2 - Работа с сотрудниками."
				<< "\n\t3 - Работа с услугами."
				<< "\n\t0 - Закончить программу."
				<< "\n\tВыберите пункт меню: ";
			cin >> navPos;
		} while (navPos < 0 || navPos > 3);

		if (navPos) navigation(navPos);
		else return 0;

	} while (true);

	return 0;
}

void navigation(size_t dataType) {
	int navPos;
	do {
		do {
			cout << "\n\tМеню программы (";

			switch (dataType) {
				case 1: cout << "ЗАКАЗЫ"; break;
				case 2: cout << "СОТРУДНИКИ"; break;
				case 3: cout << "УСЛУГИ"; break;
				case 4: cout << "СПЕЦИАЛИЗАЦИИ"; break;
			}
			cout << ").";

			cout << "\n\t1 - Новая запись."
				<< "\n\t2 - Вывести все записи."
				<< "\n\t3 - Редактировать запись."
				<< "\n\t4 - Удалить запись."
				<< "\n\t5 - Сортировать записи."
				<< "\n\t0 - Вернуться в главное меню."
				<< "\n\tВыберите пункт меню: ";
			cin >> navPos;
		} while (navPos < 0 || navPos > 5);

		switch (navPos) {

		case 1:
			inputVal(dataType);
			break;

		case 2:
			output();
			break;

		default:
			return;
		}
	} while (true);
	return;
}

string inputCurrentVal(string* checkVal, string* titleVal) {
	string temp;
	cmatch result;
	regex valueCheck(*checkVal);

	cin.ignore(cin.rdbuf()->in_avail());
	do {
		cout << *titleVal;
		getline(cin, temp);
	} while (!regex_match(temp.c_str(), result, valueCheck));

	return temp;
}

void inputVal(size_t dataType) {

	orders order;
	employee empl;
	service serv;

	size_t entryNumb = 0;
	string fileName;

	switch (dataType) {
		case 1: fileName = ORDERS; break;
		case 2: fileName = STAFF; break;
		case 3: fileName = SERVICES; break;
	}

	string temp;
	string titleVal;
	string checkVal;

	fopen_s(&dataFile, fileName.c_str(), "rb");
	switch (dataType) {
	case 1:
		while (fread(&order, sizeof(orders), 1, dataFile)) entryNumb++;
		break;
	case 2:
		while (fread(&empl, sizeof(employee), 1, dataFile)) entryNumb++;
		break;
	case 3:
		while (fread(&serv, sizeof(service), 1, dataFile)) entryNumb++;
		break;
	}
	fclose(dataFile);

	do {
		fopen_s(&dataFile, fileName.c_str(), "a+b");

		cout << "\n\tВведите данные " << entryNumb + 1 
			<< ((entryNumb + 1) % 10 == 3 && (entryNumb + 1) % 100 != 13 ? "-ей" : "-ой") 
			<< " записи:\n";

		switch (dataType) {
		case 1:
			// ID
			order.ID = entryNumb++;

			// Full Name
			titleVal = "\tФИО заказчика по шаблону \"Иванов И.И.\": ";
			checkVal = "([А-Я]{1})([а-я]+?) ([А-Я]{1})\.([А-Я]{1})\.";
			strcpy_s(order.fullName, inputCurrentVal(&checkVal, &titleVal).c_str());

			// Phone Number
			titleVal = "\tНомер телефона заказчика: +380";
			checkVal = "([0-9]{9})";
			temp = inputCurrentVal(&checkVal, &titleVal);

			for (size_t i = 0; i < temp.size(); i++)
				order.phoneNumb[i] = stoi(temp.substr(i, 1));

			// Date of order
			titleVal = "\tВведите дату заказа по шаблону (дд/мм/гггг): ";
			checkVal = "([0-2]{1}[1-9]{1}|3[0-1]{1})/(0[1-9]{1}|1[0-2]{1})/20([0-2]{1}[0-9]{1})";
			temp = inputCurrentVal(&checkVal, &titleVal);

			order.dateOfOrder[0] = stoi(temp.substr(0, 2));
			order.dateOfOrder[1] = stoi(temp.substr(3, 2));
			order.dateOfOrder[2] = stoi(temp.substr(6, 4));

			fwrite(&order, sizeof(orders), 1, dataFile);
			break;
		case 2:
			// ID
			empl.ID = entryNumb++;

			// Full Name
			titleVal = "\tФИО сотрудника по шаблону \"Иванов И.И.\": ";
			checkVal = "([А-Я]{1})([а-я]+?) ([А-Я]{1})\.([А-Я]{1})\.";
			strcpy_s(empl.fullName, inputCurrentVal(&checkVal, &titleVal).c_str());

			// Phone Number
			titleVal = "\tНомер телефона сотрудника: +380";
			checkVal = "([0-9]{9})";
			temp = inputCurrentVal(&checkVal, &titleVal);

			for (size_t i = 0; i < temp.size(); i++)
				empl.phoneNumb[i] = stoi(temp.substr(i, 1));

			// Positions
			cout << "\n\tСпециализации:\n";
			for (size_t i = 0; i < POS_KOL; i++) {
				empl.position[i] = 0;
				cout << '\t' << pos[i].ID << ". " << pos[i].occupation << '\n';
			}

			titleVal = "\tПеречислите через запятую номера специализаций: ";
			checkVal.clear();

			for (size_t i = 0; i < POS_KOL; i++) {
				for (size_t j = i; j < POS_KOL; j++) {
					checkVal.append("([1-" + to_string(POS_KOL) + "]),");
				}
				checkVal.erase(checkVal.end() - 1);
				checkVal.append("|");
			}
			checkVal.erase(checkVal.end() - 1);
			temp = inputCurrentVal(&checkVal, &titleVal);

			for (size_t i = 0, j = 0; j < temp.size(); i++, j += 2)
				empl.position[i] = stoi(temp.substr(j, 1));

			fwrite(&empl, sizeof(employee), 1, dataFile);
			break;
		case 3:
			// ID
			serv.ID = entryNumb++;

			// Category
			cout << "\n\tКатегории:\n";
			for (size_t i = 0; i < POS_KOL; i++)
				cout << '\t' << pos[i].ID << ". " << pos[i].occupation << '\n';

			titleVal = "\tВыберите категорию: ";
			checkVal = "([1-" + to_string(POS_KOL) + "])";
			serv.category = stoi(inputCurrentVal(&checkVal, &titleVal));

			// Title
			titleVal = "\tНазвание услуги: ";
			checkVal = "(.+?)";
			strcpy_s(serv.title, inputCurrentVal(&checkVal, &titleVal).c_str());

			// Employee
			FILE* staff_file;
			size_t staffLenght = 0;

			fopen_s(&staff_file, STAFF, "rb");
			while (fread(&empl, sizeof(employee), 1, staff_file)) staffLenght++;
			fclose(staff_file);

			checkVal.clear();
			fopen_s(&staff_file, STAFF, "rb");
			while (fread(&empl, sizeof(employee), 1, staff_file))
				for (size_t i = 0; i < staffLenght; i++)
					if (empl.position[i] == serv.category) {
						checkVal.append("|(" + to_string(empl.ID) + ")");
						cout << "\n\t" << empl.ID << ". " << empl.fullName;
					}
			fclose(staff_file);
			checkVal.erase(checkVal.begin());

			titleVal = "\n\tВыберите специалиста: ";
			serv.employee = stoi(inputCurrentVal(&checkVal, &titleVal));

			// Price
			titleVal = "\tСтоимость услуги (в $): ";
			checkVal = "([0-9]+?)";
			serv.price = stoi(inputCurrentVal(&checkVal, &titleVal));

			// Term
			titleVal = "\tСрок, отведённый на выполнение задачи (в днях): ";
			checkVal = "([0-9]+?)";
			serv.term = stoi(inputCurrentVal(&checkVal, &titleVal));

			fwrite(&serv, sizeof(service), 1, dataFile);
			break;
		}

		fclose(dataFile);

		char inputRepeat;
		do {
			cout << "\n\tДостаточно? (Д/Н), ваш вариант: ";
			cin >> inputRepeat;
		} while (inputRepeat != 'Д' && inputRepeat != 'д' && inputRepeat != 'Н' && inputRepeat != 'н');

		if (inputRepeat == 'Д' || inputRepeat == 'д') break;

	} while (true);
}

void output() {
	//	+-----------------------------------------------------------------------------------------------------------+
	//	|                                         ИНФОРМАЦИЯ О ВСЕХ ЗАКАЗАХ                                         |
	//	+---+-------------+--------------+------------+------------------------------------+-----------+------------+
	//	|   |     ФИО     |    Номер     |    Дата    |                                    |           |    Дата    |
	//	| № |  заказчика  |   телефона   |   заказа   |               Услуги               | Стоимость | исполнения |
	//	+---+-------------+--------------+------------+------------------------------------+-----------+------------+
	//	|   |             |              |            |  4. Петров П.П. - Эксклюзивный...  |           |            |
	//	|   |             |              |            |  8. Сидоров С.С. - Framework       |           |            |
	//	| 1 | Иванов И.И. | +38098765432 | 31/12/2020 | 10. Васечкин В.В. - Внешнее SEO... |     1500$ | 09/02/2021 |
	//	+---+-------------+--------------+------------+------------------------------------+-----------+------------+
}