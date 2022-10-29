#include "CacheAlg.h"

CacheAlg::CacheAlg() {
	cout << "입력할 데이터 개수를 입력하세요 : ";
	cin >> dataNum;
	cout << "슬롯의 개수를 입력하세요 : ";
	cin >> slotNum;

	data = new int[dataNum];
	count = new int[slotNum];
	countOrder = new int[slotNum];
	slot = new int[slotNum];

	cout << "데이터를 입력해 주세요 : ";
	for (int i = 0; i < dataNum; i++) //데이터 제한 개수만큼 입력받음
	{
		cin >> data[i];
	}
}

CacheAlg::~CacheAlg() {
	delete[] data;
	delete[] count;
	delete[] countOrder;
	delete[] slot;
}

int CacheAlg::getEmptySlotNum() {
	int negCount = 0;
	for (int i = 0; i < slotNum; i++)
		if (slot[i] < 0)
			negCount++;
	return negCount;
}

int CacheAlg::getMaxDataIndex() {
	int max = 0;
	int i = 0;
	for (int j = 0; j < slotNum; j++)\
		if (count[j] > max)
		{
			max = count[j];
			i = j;
		}
	return i;
}

double CacheAlg::LRU() {
	fill_n(slot, slotNum, -1);//초기화
	fill_n(count, slotNum, -1);

	int hit = 0;//적중횟수

	cout << "캐시내부 : " << endl;

	for (int i = 0; i < dataNum; i++) {
		int nowData = data[i];
		if (!i) {//i=0 경우
			slot[0] = nowData;
			count[0] = 0;
		}
		else if (getEmptySlotNum() > 0) //빈 슬롯이 있는 경우
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//적중한 경우
					hit++;
					for (int k = 0; k < slotNum; k++)
						count[k]++;
					count[j] = 0;//나머지 카운트값 증가, 현재 값 카운트값 0
					break;
				}
				else if (j == slotNum - 1) //미스인 경우
					for (int k = 0; k < slotNum; k++) {
						if (slot[k] < 0) {//빈 슬롯에 대입
							slot[k] = nowData;
							for (int l = 0; l < slotNum; l++)
								count[l]++;
							count[k] = 0;//나머지 카운트값 증가, 현재 값 카운트값 0
							break;
						}
					}
			}
		else //빈 슬롯이 없는 경우
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//적중한 경우
					hit++;
					for (int k = 0; k < slotNum; k++)
						count[k]++;
					count[j] = 0;//나머지 카운트값 증가, 현재 값 카운트값 0
					break;
				}
				else if (j == slotNum - 1) {//미스인 경우
					slot[getMaxDataIndex()] = nowData;
					for (int k = 0; k < slotNum; k++)
						count[k]++;
					count[getMaxDataIndex()] = 0;//나머지 카운트값 증가, 현재 값 카운트값 0
				}
			}

		cout << "| ";
		for (int j = 0; j < slotNum; j++) {
			if (slot[j] >= 0)
				cout << slot[j] << " ";
		}
		cout << "|\t";
		if (i % 5 == 4)
			cout << endl;
	}

	cout << endl << "결과캐시 : | ";
	for (int j = 0; j < slotNum; j++)
		cout << slot[j] << " ";
	cout << "| ";
	return (double)hit / (double)dataNum;
}

double CacheAlg::RDM() {
	fill_n(slot, slotNum, -1);//초기화
	fill_n(count, slotNum, -1);
	random_device rdev;
	default_random_engine gen(rdev());
	uniform_int_distribution<> dist(0, slotNum - 1);

	int hit = 0;//적중횟수

	cout << "캐시내부 : ";

	for (int i = 0; i < dataNum; i++) {
		int nowData = data[i];
		if (!i) {//i=0 경우
			slot[0] = nowData;
			count[0] = 0;
		}
		else if (getEmptySlotNum() > 0) //빈 슬롯이 있는 경우
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//적중한 경우
					hit++;
					break;
				}
				if (j == slotNum - 1) //미스인 경우
					for (int k = 0; k < slotNum; k++) {
						if (slot[k] < 0) {//빈 슬롯에 대입
							slot[k] = nowData;
							break;
						}
					}
			}
		else //빈 슬롯이 없는 경우
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//적중한 경우
					hit++;
					break;
				}
				if (j == slotNum - 1) {//미스인 경우
					slot[dist(gen)] = nowData;
				}
			}

		cout << "| ";
		for (int j = 0; j < slotNum; j++) {
			if (slot[j] >= 0)
				cout << slot[j] << " ";
		}
		cout << "|\t";
		if (i % 5 == 4)
			cout << endl;
	}

	cout << endl << "결과캐시 : | ";
	for (int j = 0; j < slotNum; j++)
		cout << slot[j] << " ";
	cout << "| ";
	return (double)hit / (double)dataNum;
}

double CacheAlg::FIFO(){

	int hit = 0;//적중횟수

	Circularqueue que(slotNum+2);

	cout << "캐시내부 : ";

	for (int i = 0; i < dataNum; i++) {

		int nowData = data[i];

		if (!i) {//i=0 경우
			que.enqueue(nowData);
		}
		else if (que.isThere(nowData)) {  //적중시
			hit++;
		}
		else {    //노적중시
			if (que.distance() < slotNum) { //가득 차 있지 않다면
				que.enqueue(nowData);
			}
			else { //가득 차 있다면
				que.enqueue(nowData);
				que.dequeue();
			}

		}
		que.display();
	}

	cout << "\n결과캐시 : ";
	que.display();

	return (double)hit / (double)dataNum;
}

double CacheAlg::LFU() {
	fill_n(slot, slotNum, -1);//초기화
	fill_n(count, slotNum, -1);
	fill_n(countOrder, slotNum, -1);

	int hit = 0;//적중횟수

	cout << "캐시내부 : ";

	for (int i = 0; i < dataNum; i++) {
		int nowData = data[i];
		if (!i) {//i=0 경우
			slot[0] = nowData;
			count[0] = 1;//현재 값 카운트값 1
			countOrder[0] = 0;//가장 먼저 들어온 값이 0, 슬롯이 꽉찼을 때 가장 늦게 들어온 값이 slotNum - 1 과 같은값을 가짐
		}
		else if (getEmptySlotNum() > 0) //빈 슬롯이 있는 경우
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//적중한 경우
					hit++;
					count[j]++;//현재 값 카운트값 증가
					break;
				}
				if (j == slotNum - 1) //미스인 경우
					for (int k = 0; k < slotNum; k++) {
						if (slot[k] < 0) {//빈 슬롯에 대입
							countOrder[k] = slotNum - getEmptySlotNum();//값이 들어온 순서 저장
							slot[k] = nowData;
							count[k] = 1;//현재 값 카운트값 1
							break;
						}
					}
			}
		else //빈 슬롯이 없는 경우
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//적중한 경우
					hit++;
					count[j]++;//현재 값 카운트값 증가
					break;
				}
				else if (j == slotNum - 1) {//미스인 경우
					int countMin = dataNum;
					int countCountMin = 0;
					int* minIndexArr;

					for (int x = 0; x < slotNum; x++) {
						if (count[x] < countMin) {
							countMin = count[x];
						}
					}
					for (int x = 0; x < slotNum; x++) {
						if (count[x] == countMin) {
							countCountMin++;
						}
					}
					minIndexArr = new int[countCountMin];
					int z = 0;
					for (int x = 0; x < slotNum; x++) {
						if (count[x] == countMin) {
							minIndexArr[z++] = x;
						}
					}

					if (countCountMin == 1) {//count값이 최솟값을 가지는 인덱스가 1개인 경우
						slot[minIndexArr[0]] = nowData;
						count[minIndexArr[0]] = 1;
						for (int k = 0; k < slotNum; k++)
							if (countOrder[k] > countOrder[minIndexArr[0]])
								countOrder[k]--;//기존 인덱스보다 늦게 들어온 값의 countOrder값 --
						countOrder[minIndexArr[0]] = slotNum - 1;//추가한 값의 countOrder값은 최댓값을 가짐
					}
					else {//최솟값을 가지는 인덱스가 다수인 경우
						int countOrderMin = slotNum -1;

						for (int k = 0; k < countCountMin; k++)//최솟값을 가지는 인덱스중 countOrder값의 최솟값을 구함
							if (countOrder[minIndexArr[k]] < countOrderMin)
								countOrderMin = countOrder[minIndexArr[k]];

						for (int k = 0; k < countCountMin; k++)//countOrder값으로 최솟값을 가지는 인덱스에 추가
							if (countOrder[minIndexArr[k]] == countOrderMin) {
								slot[minIndexArr[k]] = nowData;
								count[minIndexArr[k]] = 1;
								for (int l = 0; l < slotNum; l++)
									if (countOrder[l] > countOrder[minIndexArr[k]])
										countOrder[l]--;//기존 인덱스보다 늦게 들어온 값의 countOrder값 --

								countOrder[minIndexArr[k]] = slotNum - 1;//추가한 값의 countOrder값은 최댓값을 가짐
								break;
							}
					}

					delete[] minIndexArr;
				}
			}

		cout << "| ";
		for (int j = 0; j < slotNum; j++) {
			if (slot[j] >= 0)
				cout << slot[j] << " ";
		}
		cout << "|\t";
		if (i % 5 == 4)
			cout << endl;
	}

	cout << endl << "결과캐시 : | ";
	for (int j = 0; j < slotNum; j++)
		cout << slot[j] << " ";
	cout << "| ";
	return (double)hit / (double)dataNum;
}

double CacheAlg::NEW() {
	fill_n(slot, slotNum, -1);//초기화
	fill_n(count, slotNum, -1);
	fill_n(countOrder, slotNum, -1);

	int hit = 0;//적중횟수

	cout << "캐시내부 : ";

	for (int i = 0; i < dataNum; i++) {
		int nowData = data[i];
		if (!i) {//i=0 경우
			slot[0] = nowData;
			count[0] = 1;//현재 값 카운트값 1
			countOrder[0] = 0;//가장 먼저 들어온 값이 0, 슬롯이 꽉찼을 때 가장 늦게 들어온 값이 slotNum - 1 과 같은값을 가짐
		}
		else if (getEmptySlotNum() > 0) //빈 슬롯이 있는 경우
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//적중한 경우
					hit++;
					count[j]++;//현재 값 카운트값 증가
					break;
				}
				if (j == slotNum - 1) //미스인 경우
					for (int k = 0; k < slotNum; k++) {
						if (slot[k] < 0) {//빈 슬롯에 대입
							countOrder[k] = slotNum - getEmptySlotNum();//값이 들어온 순서 저장
							slot[k] = nowData;
							count[k] = 1;//현재 값 카운트값 1
							break;
						}
					}
			}
		else //빈 슬롯이 없는 경우
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//적중한 경우
					hit++;
					count[j]++;//현재 값 카운트값 증가
					break;
				}
				else if (j == slotNum - 1) {//미스인 경우
					int countMax = 1;
					int countCountMax = 0;
					int* maxIndexArr;

					for (int x = 0; x < slotNum; x++) {
						if (count[x] > countMax) {
							countMax = count[x];
						}
					}
					for (int x = 0; x < slotNum; x++) {
						if (count[x] == countMax) {
							countCountMax++;
						}
					}
					maxIndexArr = new int[countCountMax];
					int z = 0;
					for (int x = 0; x < slotNum; x++) {
						if (count[x] == countMax) {
							maxIndexArr[z++] = x;
						}
					}

					if (countCountMax == 1) {//count값이 최솟값을 가지는 인덱스가 1개인 경우
						slot[maxIndexArr[0]] = nowData;
						count[maxIndexArr[0]] = 1;
						for (int k = 0; k < slotNum; k++)
							if (countOrder[k] > countOrder[maxIndexArr[0]])
								countOrder[k]--;//기존 인덱스보다 늦게 들어온 값의 countOrder값 --
						countOrder[maxIndexArr[0]] = slotNum - 1;//추가한 값의 countOrder값은 최댓값을 가짐
					}
					else {//최솟값을 가지는 인덱스가 다수인 경우
						int countOrderMin = slotNum - 1;

						for (int k = 0; k < countCountMax; k++)//최솟값을 가지는 인덱스중 countOrder값의 최솟값을 구함
							if (countOrder[maxIndexArr[k]] < countOrderMin)
								countOrderMin = countOrder[maxIndexArr[k]];

						for (int k = 0; k < countCountMax; k++)//countOrder값으로 최솟값을 가지는 인덱스에 추가
							if (countOrder[maxIndexArr[k]] == countOrderMin) {
								slot[maxIndexArr[k]] = nowData;
								count[maxIndexArr[k]] = 1;
								for (int l = 0; l < slotNum; l++)
									if (countOrder[l] > countOrder[maxIndexArr[k]])
										countOrder[l]--;//기존 인덱스보다 늦게 들어온 값의 countOrder값 --

								countOrder[maxIndexArr[k]] = slotNum - 1;//추가한 값의 countOrder값은 최댓값을 가짐
								break;
							}
					}

					delete[] maxIndexArr;
				}
			}

		cout << "| ";
		for (int j = 0; j < slotNum; j++) {
			if (slot[j] >= 0)
				cout << slot[j] << " ";
		}
		cout << "|\t";
		if (i % 5 == 4)
			cout << endl;
	}

	cout << endl << "결과캐시 : | ";
	for (int j = 0; j < slotNum; j++)
		cout << slot[j] << " ";
	cout << "| ";
	return (double)hit / (double)dataNum;
}

void CacheAlg::showLRU() {
	cout << "LRU--------------------------------------------------------------------" << endl;
	auto start = chrono::high_resolution_clock::now();
	cout << "\t적중률 : " << LRU() << "\t";
	auto end = chrono::high_resolution_clock::now();
	cout << "실행시간 : " << (end - start).count() / (double)1000000 << "ms" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
}

void CacheAlg::showRDM() {
	cout << "RDM--------------------------------------------------------------------" << endl;
	auto start = chrono::high_resolution_clock::now();
	cout << "\t적중률 : " << RDM() << "\t";
	auto end = chrono::high_resolution_clock::now();
	cout << "실행시간 : " << (end - start).count() / (double)1000000 << "ms" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
}

void CacheAlg::showFIFO()
{
	cout << "FIFO--------------------------------------------------------------------" << endl;
	auto start = chrono::high_resolution_clock::now();
	cout << "\t적중률 : " << FIFO() << "\t";
	auto end = chrono::high_resolution_clock::now();
	cout << "실행시간 : " << (end - start).count() / (double)1000000 << "ms" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
}

void CacheAlg::showLFU() {
	cout << "LFU--------------------------------------------------------------------" << endl;
	auto start = chrono::high_resolution_clock::now();
	cout << "\t적중률 : " << LFU() << "\t";
	auto end = chrono::high_resolution_clock::now();
	cout << "실행시간 : " << (end - start).count() / (double)1000000 << "ms" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
}

void CacheAlg::showNEW() {
	cout << "NEW--------------------------------------------------------------------" << endl;
	auto start = chrono::high_resolution_clock::now();
	cout << "\t적중률 : " << NEW() << "\t";
	auto end = chrono::high_resolution_clock::now();
	cout << "실행시간 : " << (end - start).count() / (double)1000000 << "ms" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
}

void CacheAlg::play() {
	showLRU();
	showRDM();
	showFIFO();
	showLFU();
	showNEW();
}