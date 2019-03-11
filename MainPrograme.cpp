/*
Переменные:
Base - Клетки являющиеся базисными
Have - Запасы
Need - Заявки
Table - Кол-во товара
Price - Стоимость
ps_Price - Псевдостоимость
aP - Потенциал альфа
bP - Потенциал бета
K - Минимальный элемент из (x[i,j]-)
Y - Гамма, Price-ps_Price=Y
Yij - Массив индексов замкнутой ломаной линии
Z - Текущая стоимость плана
_Z - Ожидаемая стоимость плана
H - Промежуточное значение запасов
N - Промежуточное значение заяввок
*/

#include <iostream>
#include <clocale>
using namespace std;

void ShowTable();							//Отображает таблицу
bool CheckOpPlan();							//Проверка опорности плана
bool CheckOptimal();						//Проверка оптимальности плана
int** CreateInt2Array(int n, int m);		//Создание целочисленного двумерного динамического массива
bool** CreateBool2Array(int n, int m);		//Создание булевого двумерного динамического массива
void Delete2Array(int **A, int n);			//Удачение целочисленного двумерного динамического массива
void Delete2Array(bool **A, int n);			//Удаление булевого двумерного динамического массива
void Potencials();							//Расстановка потенциалов и псевдостоимости
void Answer(int Z);							//Вывод ответа
int** Figure(int** A, int i, int j, int g);	//Построение замкнутой ломаной линии
bool CheckXY(int** A, int i, int j, int g);	//Проверка не правильных поворотов линии

int  inp, outp;
int **Price, **ps_Price, **Table, **Yij;
int *Have, *Need, *aP, *bP;
bool **Base;

int main() {
	int i=0, j=0, g=0, K=0, Y=0, Z=0, _Z=0, H=0, N=0;
	bool Zero;
	system("color F0");
	setlocale(LC_ALL,"Rus");
	cout<<"Кол-во пунктов отправления: ";
	cin>>inp;
	cout<<"Кол-во пунктов назначения: ";
	cin>>outp;
	Price = CreateInt2Array(inp,outp);
	Table = CreateInt2Array(inp,outp);
	ps_Price = CreateInt2Array(inp,outp);
	Base = CreateBool2Array(inp,outp);
	Yij = CreateInt2Array(inp+outp,3);
	aP = new int[inp];
	bP = new int[outp];
	Have = new int[inp];
	Need = new int[outp];
	for(i=0; i<inp; i++) {
		aP[i]=100;
		Have[i]=0;
		for(j=0; j<=outp; j++) {
			Price[i][j] = 0;
			Table[i][j] = 0;
			ps_Price[i][j] = 0;
			Base[i][j] = false;

		}
	}
	for(i=0; i<outp; i++) {
		bP[i]=100;
		Need[i]=0;
	}
	for(i=0; i<inp; i++) {
		for(j=0; j<outp; j++) {
			printf("C[%d,%d] = ",i+1,j+1);
			cin>>Price[i][j];
		}
	}
	for(i=0; i<outp; i++) {
		printf("B[%d] = ",i+1);
		cin>>Need[i];
	}
	for(j=0; j<inp; j++) {
		printf("A[%d] = ",j+1);
		cin>>Have[j];
	}
	i=0;
	j=0;
	while(!Base[inp-1][outp-1]) {
		if(H==0)
			H=Have[i];
		if(N==0)
			N=Need[j];
		if(H!=N) {
			if(H<N) {
				Table[i][j]=H;
				N-=H;
				H=0;
				Base[i][j]=true;
				i++;
			} else {
				Table[i][j]=N;
				H-=N;
				N=0;
				Base[i][j]=true;
				j++;
			}
		} else {
			Table[i][j]=H;
			Base[i][j]=true;
			j++;
			Base[i][j]=true;
			i++;
			H=0;
			N=0;
		}

	}
	if(CheckOpPlan()) {
		Potencials();
		ShowTable();
		int k=0;
		printf("План является опорным\n");
		for(i=0; i<inp; i++)
			for(j=0; j<outp; j++)
				Z+=Price[i][j]*Table[i][j];
		while(!CheckOptimal() && k<2){
			Zero=false;
			K=0;
			for(i=0; i<inp; i++)
				for(j=0; j<3; j++)
					Yij[i][j]=0;
			Potencials();
			for(i=0; i<inp; i++)
				for(j=0; j<outp; j++)
					if((Price[i][j]-ps_Price[i][j])<Y) {
						Y=Price[i][j]-ps_Price[i][j];
						Yij[0][0]=i;
						Yij[0][1]=j;
					}
			Base[Yij[0][0]][Yij[0][1]]=true;
			//Figure(Yij,Yij[0][0],Yij[0][1],1);
			/*do {
				i-0;
				j=0;
				//K-? Yij-?
				//Цикл до тех пока я не вернусь в начальную точку
			} while(!(Yij[i][0]==Yij[0][0] && Yij[i][1]==Yij[0][1]));
			for(i=0; i<inp; i++)
				if(!(Table[Yij[i][0]][Yij[i][1]]+=Yij[i][2]*K) && !Zero) {
					Base[Yij[i][0]][Yij[i][1]]=false;
					Zero=true;
				}*/
			_Z=Z+K*Y;
			cout<<"K="<<K<<"  Y="<<Y<<endl;
			cout<<"Zожид="<<_Z<<endl;
			ShowTable();
			cout<<"Zтек="<<Z<<endl;
			k++;

		}
		Answer(Z);
	}

	system("pause");
	Delete2Array(Yij,inp+outp);
	Delete2Array(Price,inp);
	Delete2Array(Table,inp);
	Delete2Array(ps_Price,inp);
	Delete2Array(Base,inp);
	delete Have;
	delete Need;
	delete aP;
	delete bP;

	return 0;
}
//-----------------------------------ОПИСАНИЕ-ФУНКЦИЙ------------------------------------------
void ShowTable() {
	int i,j;
	setlocale(LC_ALL,"C");
	printf("%c%c%c%c%c%c%c%c",201,205,205,205,205,205,205,205);
	for(i=0; i<outp; i++)
		printf("%c%c%c%c%c%c%c%c",203,205,205,205,205,205,205,205);
	for(i=0; i<2; i++)
		printf("%c%c%c%c%c%c%c",203,205,205,205,205,205,205);
	printf("%c\n",187);

	printf("%c",186);
	setlocale(LC_ALL,"Rus");
	printf(" ПО\\ПН ");
	setlocale(LC_ALL,"C");
	printf("%c",186);
	for(i=0; i<outp; i++)
		printf("  B%d%4c",i+1,186);
	printf("  Ai%3c alfa%2c\n",186,186);
	for(i=0; i<inp; i++) {
		printf("%c%c%c%c%c%c%c%c",204,205,205,205,205,205,205,205);
		for(j=0; j<outp; j++)
			if(!Base[i][j])
				printf("%c%c%c%c%c%c%c%c",206,205,205,209,205,209,205,205);
			else printf("%c%c%c%c%c%c%c%c",206,205,205,205,205,209,205,205);
		for(j=0; j<2; j++)
			printf("%c%c%c%c%c%c%c",206,205,205,205,205,205,205);
		printf("%c",185);

		printf("\n");

		printf("%c%8c",186,186);
		for(j=0; j<outp; j++) {
			if(!Base[i][j])
				printf("%2d%c",ps_Price[i][j],179);
			else printf("   ");
			printf("%2c%2d%c",179,Price[i][j],186);
		}
		printf("%7c%7c",186,186);

		printf("\n");

		printf("%c  A%2d",186,i+1);

		for(j=0; j<outp; j++)
			if(j==0) {
				if(!Base[i][j])
					printf("%3c%c%c%c %c%c%c",199,196,196,217,192,196,196);
				else printf("%3c   %2c%c%c",186,192,196,196);
			} else {
				if(!Base[i][j])
					printf("%c%c%c%c %c%c%c",215,196,196,217,192,196,196);
				else if(j==0)printf("%c   %2c%c%c",182,192,196,196);
				else printf("%c   %2c%c%c",182,192,196,196);
			}
		printf("%c %3d%3c %3d%3c",182,Have[i],186,aP[i],186);

		printf("\n");

		printf("%c%8c",186,186);
		for(j=0; j<outp; j++)
			if(Base[i][j])
				printf("  %3d%3c",Table[i][j],186);
			else printf("%8c",186);
		printf("%7c%7c",186,186);

		printf("\n");
	}
	printf("%c%c%c%c%c%c%c%c",204,205,205,205,205,205,205,205);
	for(i=0; i<outp; i++)
		printf("%c%c%c%c%c%c%c%c",206,205,205,205,205,205,205,205);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",206,205,205,205,205,205,205,202,205,205,205,205,205,205,188);
	printf("%c  Bi%4c",186,186);
	for(i=0; i<outp; i++)
		printf("  %3d%3c",Need[i],186);
	printf("\n%c%c%c%c%c%c%c%c",204,205,205,205,205,205,205,205);
	for(i=0; i<outp; i++)
		printf("%c%c%c%c%c%c%c%c",206,205,205,205,205,205,205,205);
	printf("%c\n%c Beta%3c",185,186,186);
	for(i=0; i<outp; i++)
		printf("  %3d%3c",bP[i],186);
	printf("");
	printf("\n");
	printf("%c%c%c%c%c%c%c%c",200,205,205,205,205,205,205,205);
	for(i=0; i<outp; i++)
		printf("%c%c%c%c%c%c%c%c",202,205,205,205,205,205,205,205);
	printf("%c\n",188);
	setlocale(LC_ALL,"Rus");
}
bool CheckOptimal() {
	int i,j;
	for(i=0; i<inp; i++)
		for(j=0; j<outp; j++)
			if(ps_Price[i][j]>Price[i][j]) {
				printf("Не оптимально\n");
				return false;
			}
	printf("Оптимально\n");
	return true;
}
bool CheckOpPlan() {

	int i,j,k=0;
	for(i=0; i<inp; i++)
		for(j=0; j<outp; j++)
			if(Base[i][j]!=false)
				k++;
	if(k==i+j-1)
		return true;
	ShowTable();
	printf("План не является опорным\n");
	return false;
}
int** CreateInt2Array(int n, int m) {
	int **A;
	A = new int*[n];
	for(int i=0; i<n; i++)
		A[i] = new int[m];
	return A;
}
bool** CreateBool2Array(int n, int m) {
	bool **A;
	A = new bool*[n];
	for(int i=0; i<n; i++)
		A[i] = new bool[m];
	return A;
}
void Delete2Array(int **A, int n) {
	for(int i=0; i<n; i++)
		delete [] A[i];
	delete [] A;
}
void Delete2Array(bool **A, int n) {
	for(int i=0; i<n; i++)
		delete [] A[i];
	delete [] A;
}
void Potencials() {
	int i,j,g;
	aP[0]=0;
	for(g=0; g<inp+outp-1; g++) {
		for(i=0; i<inp; i++)
			for(j=0; j<outp; j++)
				if((aP[i]==100 xor bP[j]==100)&&Base[i][j]) {
					if(aP[i]==100)aP[i]=Price[i][j]-bP[j];
					if(bP[j]==100)bP[j]=Price[i][j]-aP[i];
				}
	}

	for(i=0; i<inp; i++)
		for(j=0; j<outp; j++)
			if(!Base[i][j])ps_Price[i][j]=aP[i]+bP[j];
}
void Answer(int Z) {
	int i,j;
	printf("Ответ: Zтек=%d\n",Z);
	setlocale(LC_ALL,"C");
	printf("  %c%c%c%c",218,196,196,196);
	for(i=0; i<outp-2; i++)
		printf("   ");
	printf("  %c%c%c%c",196,196,196,191);
	for(i=0; i<inp; i++) {
		if(i==0)
			printf("\nX=%c",179);
		else printf("  %c",179);
		for(j=0; j<outp; j++) {
			if(Base[i][j])
				printf("%3d",Table[i][j]);
			else printf(" __");
		}
		printf("  %c\n",179);
	}
	printf("  %c%c%c%c",192,196,196,196);
	for(i=0; i<outp-2; i++)
		printf("   ");
	printf("  %c%c%c%c\n",196,196,196,217);

	setlocale(LC_ALL,"Rus");
}
int** Figure(int** A, int i, int j, int g) {
	bool R;
	if((A[g-2][0]-i>0 && j==A[g-2][1])||CheckXY(A,i,j,g))
		return 0;
	else
		do {
			i++;
			if(Base[i][j]) {
				A[g][0]=i;
				A[g][1]=j;
				g++;
				if(i==A[0][0] && j==A[0][1])
					return A;
				R=Figure(A,i,j,g);
			}
		} while(i<inp);
	if (!R)
	return 0;
	if((A[g-2][0]-i<0 && j==A[g-2][1])||CheckXY(A,i,j,g))
		do {
			i--;
			if(Base[i][j]) {
				A[g][0]=i;
				A[g][1]=j;
				g++;
				if(i==A[0][0] && j==A[0][1])
					return A;
				R=Figure(A,i,j,g);
			}
		} while(i>0);
	if(!R)
	return 0;

	if((A[g-2][0]==i && A[g-2][1]-j>0)||CheckXY(A,i,j,g))
		return 0;
	else
		do {
			j++;
			if(Base[i][j]) {
				A[g][0]=i;
				A[g][1]=j;
				g++;
				if(i==A[0][0] && j==A[0][1])
					return A;
				R=Figure(A,i,j,g);
			}
		} while(j<outp);
	if(!R)
	return 0;
	if((A[g-2][0]==i && A[g-2][1]-j<0)||CheckXY(A,i,j,g))
		do {
			j--;
			if(Base[i][j]) {
				A[g][0]=i;
				A[g][1]=j;
				g++;
				if(i==A[0][0] && j==A[0][1])
					return A;
				R=Figure(A,i,j,g);
			}
		} while(j>0);
	if(!R)
	return 0;
	return A;
}
bool CheckXY(int** A, int i, int j, int g) {
	for(int q=0; q<g+1; q++) {
		if(A[q][0]==i && A[q][1]==j)
			return true;
	}
	return false;
}

/*int** Figure(int** A,int i,int j,int g){

	while(!Base[i][j])
	i++;
	A=Figure(A,i,j);
	while(!Base[i][j])
	j++;
	A=Figure(A,i,j);
	while(!Base[i][j])
	i--;
	A=Figure(A,i,j);
	while(!Base[i][j])
	j--;
	A=Figure(A,i,j);
	if(i==Yij[0][0] && j==Yij[0][1]){
		return A;
	}
	A[g][0]=i;
	A[g][1]=j; 
	A=Figure(A,i,j);

}*/


