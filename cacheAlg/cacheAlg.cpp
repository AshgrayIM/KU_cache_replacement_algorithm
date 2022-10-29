#include "CacheAlg.h"

CacheAlg::CacheAlg() {
	cout << "�Է��� ������ ������ �Է��ϼ��� : ";
	cin >> dataNum;
	cout << "������ ������ �Է��ϼ��� : ";
	cin >> slotNum;

	data = new int[dataNum];
	count = new int[slotNum];
	countOrder = new int[slotNum];
	slot = new int[slotNum];

	cout << "�����͸� �Է��� �ּ��� : ";
	for (int i = 0; i < dataNum; i++) //������ ���� ������ŭ �Է¹���
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
	fill_n(slot, slotNum, -1);//�ʱ�ȭ
	fill_n(count, slotNum, -1);

	int hit = 0;//����Ƚ��

	cout << "ĳ�ó��� : " << endl;

	for (int i = 0; i < dataNum; i++) {
		int nowData = data[i];
		if (!i) {//i=0 ���
			slot[0] = nowData;
			count[0] = 0;
		}
		else if (getEmptySlotNum() > 0) //�� ������ �ִ� ���
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//������ ���
					hit++;
					for (int k = 0; k < slotNum; k++)
						count[k]++;
					count[j] = 0;//������ ī��Ʈ�� ����, ���� �� ī��Ʈ�� 0
					break;
				}
				else if (j == slotNum - 1) //�̽��� ���
					for (int k = 0; k < slotNum; k++) {
						if (slot[k] < 0) {//�� ���Կ� ����
							slot[k] = nowData;
							for (int l = 0; l < slotNum; l++)
								count[l]++;
							count[k] = 0;//������ ī��Ʈ�� ����, ���� �� ī��Ʈ�� 0
							break;
						}
					}
			}
		else //�� ������ ���� ���
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//������ ���
					hit++;
					for (int k = 0; k < slotNum; k++)
						count[k]++;
					count[j] = 0;//������ ī��Ʈ�� ����, ���� �� ī��Ʈ�� 0
					break;
				}
				else if (j == slotNum - 1) {//�̽��� ���
					slot[getMaxDataIndex()] = nowData;
					for (int k = 0; k < slotNum; k++)
						count[k]++;
					count[getMaxDataIndex()] = 0;//������ ī��Ʈ�� ����, ���� �� ī��Ʈ�� 0
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

	cout << endl << "���ĳ�� : | ";
	for (int j = 0; j < slotNum; j++)
		cout << slot[j] << " ";
	cout << "| ";
	return (double)hit / (double)dataNum;
}

double CacheAlg::RDM() {
	fill_n(slot, slotNum, -1);//�ʱ�ȭ
	fill_n(count, slotNum, -1);
	random_device rdev;
	default_random_engine gen(rdev());
	uniform_int_distribution<> dist(0, slotNum - 1);

	int hit = 0;//����Ƚ��

	cout << "ĳ�ó��� : ";

	for (int i = 0; i < dataNum; i++) {
		int nowData = data[i];
		if (!i) {//i=0 ���
			slot[0] = nowData;
			count[0] = 0;
		}
		else if (getEmptySlotNum() > 0) //�� ������ �ִ� ���
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//������ ���
					hit++;
					break;
				}
				if (j == slotNum - 1) //�̽��� ���
					for (int k = 0; k < slotNum; k++) {
						if (slot[k] < 0) {//�� ���Կ� ����
							slot[k] = nowData;
							break;
						}
					}
			}
		else //�� ������ ���� ���
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//������ ���
					hit++;
					break;
				}
				if (j == slotNum - 1) {//�̽��� ���
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

	cout << endl << "���ĳ�� : | ";
	for (int j = 0; j < slotNum; j++)
		cout << slot[j] << " ";
	cout << "| ";
	return (double)hit / (double)dataNum;
}

double CacheAlg::FIFO(){

	int hit = 0;//����Ƚ��

	Circularqueue que(slotNum+2);

	cout << "ĳ�ó��� : ";

	for (int i = 0; i < dataNum; i++) {

		int nowData = data[i];

		if (!i) {//i=0 ���
			que.enqueue(nowData);
		}
		else if (que.isThere(nowData)) {  //���߽�
			hit++;
		}
		else {    //�����߽�
			if (que.distance() < slotNum) { //���� �� ���� �ʴٸ�
				que.enqueue(nowData);
			}
			else { //���� �� �ִٸ�
				que.enqueue(nowData);
				que.dequeue();
			}

		}
		que.display();
	}

	cout << "\n���ĳ�� : ";
	que.display();

	return (double)hit / (double)dataNum;
}

double CacheAlg::LFU() {
	fill_n(slot, slotNum, -1);//�ʱ�ȭ
	fill_n(count, slotNum, -1);
	fill_n(countOrder, slotNum, -1);

	int hit = 0;//����Ƚ��

	cout << "ĳ�ó��� : ";

	for (int i = 0; i < dataNum; i++) {
		int nowData = data[i];
		if (!i) {//i=0 ���
			slot[0] = nowData;
			count[0] = 1;//���� �� ī��Ʈ�� 1
			countOrder[0] = 0;//���� ���� ���� ���� 0, ������ ��á�� �� ���� �ʰ� ���� ���� slotNum - 1 �� �������� ����
		}
		else if (getEmptySlotNum() > 0) //�� ������ �ִ� ���
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//������ ���
					hit++;
					count[j]++;//���� �� ī��Ʈ�� ����
					break;
				}
				if (j == slotNum - 1) //�̽��� ���
					for (int k = 0; k < slotNum; k++) {
						if (slot[k] < 0) {//�� ���Կ� ����
							countOrder[k] = slotNum - getEmptySlotNum();//���� ���� ���� ����
							slot[k] = nowData;
							count[k] = 1;//���� �� ī��Ʈ�� 1
							break;
						}
					}
			}
		else //�� ������ ���� ���
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//������ ���
					hit++;
					count[j]++;//���� �� ī��Ʈ�� ����
					break;
				}
				else if (j == slotNum - 1) {//�̽��� ���
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

					if (countCountMin == 1) {//count���� �ּڰ��� ������ �ε����� 1���� ���
						slot[minIndexArr[0]] = nowData;
						count[minIndexArr[0]] = 1;
						for (int k = 0; k < slotNum; k++)
							if (countOrder[k] > countOrder[minIndexArr[0]])
								countOrder[k]--;//���� �ε������� �ʰ� ���� ���� countOrder�� --
						countOrder[minIndexArr[0]] = slotNum - 1;//�߰��� ���� countOrder���� �ִ��� ����
					}
					else {//�ּڰ��� ������ �ε����� �ټ��� ���
						int countOrderMin = slotNum -1;

						for (int k = 0; k < countCountMin; k++)//�ּڰ��� ������ �ε����� countOrder���� �ּڰ��� ����
							if (countOrder[minIndexArr[k]] < countOrderMin)
								countOrderMin = countOrder[minIndexArr[k]];

						for (int k = 0; k < countCountMin; k++)//countOrder������ �ּڰ��� ������ �ε����� �߰�
							if (countOrder[minIndexArr[k]] == countOrderMin) {
								slot[minIndexArr[k]] = nowData;
								count[minIndexArr[k]] = 1;
								for (int l = 0; l < slotNum; l++)
									if (countOrder[l] > countOrder[minIndexArr[k]])
										countOrder[l]--;//���� �ε������� �ʰ� ���� ���� countOrder�� --

								countOrder[minIndexArr[k]] = slotNum - 1;//�߰��� ���� countOrder���� �ִ��� ����
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

	cout << endl << "���ĳ�� : | ";
	for (int j = 0; j < slotNum; j++)
		cout << slot[j] << " ";
	cout << "| ";
	return (double)hit / (double)dataNum;
}

double CacheAlg::NEW() {
	fill_n(slot, slotNum, -1);//�ʱ�ȭ
	fill_n(count, slotNum, -1);
	fill_n(countOrder, slotNum, -1);

	int hit = 0;//����Ƚ��

	cout << "ĳ�ó��� : ";

	for (int i = 0; i < dataNum; i++) {
		int nowData = data[i];
		if (!i) {//i=0 ���
			slot[0] = nowData;
			count[0] = 1;//���� �� ī��Ʈ�� 1
			countOrder[0] = 0;//���� ���� ���� ���� 0, ������ ��á�� �� ���� �ʰ� ���� ���� slotNum - 1 �� �������� ����
		}
		else if (getEmptySlotNum() > 0) //�� ������ �ִ� ���
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//������ ���
					hit++;
					count[j]++;//���� �� ī��Ʈ�� ����
					break;
				}
				if (j == slotNum - 1) //�̽��� ���
					for (int k = 0; k < slotNum; k++) {
						if (slot[k] < 0) {//�� ���Կ� ����
							countOrder[k] = slotNum - getEmptySlotNum();//���� ���� ���� ����
							slot[k] = nowData;
							count[k] = 1;//���� �� ī��Ʈ�� 1
							break;
						}
					}
			}
		else //�� ������ ���� ���
			for (int j = 0; j < slotNum; j++) {
				if (slot[j] == nowData) {//������ ���
					hit++;
					count[j]++;//���� �� ī��Ʈ�� ����
					break;
				}
				else if (j == slotNum - 1) {//�̽��� ���
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

					if (countCountMax == 1) {//count���� �ּڰ��� ������ �ε����� 1���� ���
						slot[maxIndexArr[0]] = nowData;
						count[maxIndexArr[0]] = 1;
						for (int k = 0; k < slotNum; k++)
							if (countOrder[k] > countOrder[maxIndexArr[0]])
								countOrder[k]--;//���� �ε������� �ʰ� ���� ���� countOrder�� --
						countOrder[maxIndexArr[0]] = slotNum - 1;//�߰��� ���� countOrder���� �ִ��� ����
					}
					else {//�ּڰ��� ������ �ε����� �ټ��� ���
						int countOrderMin = slotNum - 1;

						for (int k = 0; k < countCountMax; k++)//�ּڰ��� ������ �ε����� countOrder���� �ּڰ��� ����
							if (countOrder[maxIndexArr[k]] < countOrderMin)
								countOrderMin = countOrder[maxIndexArr[k]];

						for (int k = 0; k < countCountMax; k++)//countOrder������ �ּڰ��� ������ �ε����� �߰�
							if (countOrder[maxIndexArr[k]] == countOrderMin) {
								slot[maxIndexArr[k]] = nowData;
								count[maxIndexArr[k]] = 1;
								for (int l = 0; l < slotNum; l++)
									if (countOrder[l] > countOrder[maxIndexArr[k]])
										countOrder[l]--;//���� �ε������� �ʰ� ���� ���� countOrder�� --

								countOrder[maxIndexArr[k]] = slotNum - 1;//�߰��� ���� countOrder���� �ִ��� ����
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

	cout << endl << "���ĳ�� : | ";
	for (int j = 0; j < slotNum; j++)
		cout << slot[j] << " ";
	cout << "| ";
	return (double)hit / (double)dataNum;
}

void CacheAlg::showLRU() {
	cout << "LRU--------------------------------------------------------------------" << endl;
	auto start = chrono::high_resolution_clock::now();
	cout << "\t���߷� : " << LRU() << "\t";
	auto end = chrono::high_resolution_clock::now();
	cout << "����ð� : " << (end - start).count() / (double)1000000 << "ms" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
}

void CacheAlg::showRDM() {
	cout << "RDM--------------------------------------------------------------------" << endl;
	auto start = chrono::high_resolution_clock::now();
	cout << "\t���߷� : " << RDM() << "\t";
	auto end = chrono::high_resolution_clock::now();
	cout << "����ð� : " << (end - start).count() / (double)1000000 << "ms" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
}

void CacheAlg::showFIFO()
{
	cout << "FIFO--------------------------------------------------------------------" << endl;
	auto start = chrono::high_resolution_clock::now();
	cout << "\t���߷� : " << FIFO() << "\t";
	auto end = chrono::high_resolution_clock::now();
	cout << "����ð� : " << (end - start).count() / (double)1000000 << "ms" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
}

void CacheAlg::showLFU() {
	cout << "LFU--------------------------------------------------------------------" << endl;
	auto start = chrono::high_resolution_clock::now();
	cout << "\t���߷� : " << LFU() << "\t";
	auto end = chrono::high_resolution_clock::now();
	cout << "����ð� : " << (end - start).count() / (double)1000000 << "ms" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
}

void CacheAlg::showNEW() {
	cout << "NEW--------------------------------------------------------------------" << endl;
	auto start = chrono::high_resolution_clock::now();
	cout << "\t���߷� : " << NEW() << "\t";
	auto end = chrono::high_resolution_clock::now();
	cout << "����ð� : " << (end - start).count() / (double)1000000 << "ms" << endl;
	cout << "-----------------------------------------------------------------------" << endl;
}

void CacheAlg::play() {
	showLRU();
	showRDM();
	showFIFO();
	showLFU();
	showNEW();
}