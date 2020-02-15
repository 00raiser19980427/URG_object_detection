#include<stdio.h>
#include<math.h>


/***object_detection()内の設定**********************************************************/
#define toleRange 5				//フィルタのパラメータ[mm]
#define teeRadias 10			//ティーの半径単位[mm]
#define useData_firstNum 0		//使用するデータの番号の範囲を決める //最小
#define useData_finalNum 10		//                〃                 //最大
#define dataVal (useData_finalNum - useData_firstNum)
/***************************************************************************************/


float urgData[1080] = { 2, 1, 8, 5, 4, 7, 9, 0, 6, 3}; //URGのデータ1080個
float teePosi_X;	//ティーのx座標		//単位[mm] 
float teePosi_Y;	//ティーのy座標		//単位[mm] 

//-----------------------------------------絶対値start-----------------------------------------------------
float myAbs(float XX) {
	if (XX < 0) { XX = -XX; }
	return XX;
}
//-----------------------------------------絶対値end-------------------------------------------------------


//-----------------------------------------クイックソートstart---------------------------------------------

/* 値を入れ替える関数(距離データ,float用) */
void swap(float* x, float* y) {
	float temp;		// 値を一時保存する変数

	temp = *x;
	*x = *y;
	*y = temp;
}

/* 値を入れ替える関数(データ番号,int用) */
void swap_num(int* x, int* y) {
	int temp;		// 値を一時保存する変数

	temp = *x;
	*x = *y;
	*y = temp;
}

/***
* pivotを決め、
* 全データをpivotを境目に振り分け、
* pivotの添え字を返す
***/
int partition(float array[],int arrayNum[], int left, int right) {
	int i, j, pivot;
	i = left;
	j = right + 1;
	pivot = left;   // 先頭要素をpivotとする

	do {
		do { i++; } while (array[i] < array[pivot]);
		do { j--; } while (array[pivot] < array[j]);
		
		// pivotより小さいものを左へ、大きいものを右へ
		if (i < j) {
			swap(&array[i], &array[j]);				//データの中身を入れ替え
			swap_num(&arrayNum[i], &arrayNum[j]);		//番号を交換
		}
	} while (i < j);

	swap(&array[pivot], &array[j]);				//pivotを更新
	swap_num(&arrayNum[pivot], &arrayNum[j]);		//更新したpivotの番号も入れ替え
	

	return j;
}

/* クイックソート */
void quick_sort(float array[], int arrayNum[], int left, int right) {
	int pivot;

	if (left < right) {
		pivot = partition(array,arrayNum, left, right);
		quick_sort(array,arrayNum, left, pivot - 1);		// pivotを境に再帰的にクイックソート
		quick_sort(array,arrayNum, pivot + 1, right);
	}
}

//-----------------------------------------クイックソートend-----------------------------------------------

//-----------------------------------------オブジェクト検知start-----------------------------------------------

void object_detection(float array[] ) {
	int myi = 0;
	float theta;
	float sortData[dataVal];	//使用するデータのみの配列
	int dataNum[dataVal];		//データの番号用の配列
	

	for(int i = 0; i < dataVal; i++) {							//使用するデータのみコピー
			sortData[i] = array[ i + useData_firstNum];
		}
	for (int i = 0; i < dataVal; i++) {							//使用するデータと同じ数だけ番号を振る
		dataNum[i] = i;
	}
	quick_sort(sortData, dataNum, 0, dataVal - 1);						//クイックソートを実行

	while (1) {
		
		//フィルタ	//隣り合う3点で判断
		if ((myAbs(array[dataNum[myi] + useData_firstNum - 1] - sortData[myi]) < toleRange) && (myAbs(array[dataNum[myi] + useData_firstNum + 1] - sortData[myi]) < toleRange)) {
			
			theta = dataNum[myi] * 3.1415926535 / 720;	//URGデータの角度の分解能...0.25° //番号×0.25×PI/180
			teePosi_X = -(sortData[myi]+ teeRadias) * cos(theta);
			teePosi_Y =  (sortData[myi]+ teeRadias) * sin(theta);
			break;
		}
		else myi++;
	}
}
//-----------------------------------------オブジェクト検知end-----------------------------------------------

int main(void) {

	object_detection(urgData);
	return 0;
}