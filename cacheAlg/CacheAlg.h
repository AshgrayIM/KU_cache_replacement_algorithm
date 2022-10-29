#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <random>

using namespace std;

class CacheAlg {
private:
	int* data;
	int* slot;
	int* count;
	int* countOrder;
	int dataNum;
	int slotNum;
public:
	CacheAlg();
	~CacheAlg();
	void play();

	int getEmptySlotNum();
	int getMaxDataIndex();

	double LRU();
	void showLRU();
	double RDM();
	void showRDM();
	double FIFO();
	void showFIFO();
	double LFU();
	void showLFU();
	double NEW();
	void showNEW();
};

class Circularqueue {
private:
	int front;
	int rear;
	int size;
	int* qslot;
public:
	Circularqueue() {
		front = 0;
		rear = 0;
	}
	Circularqueue(int len) {
		size = len;
		qslot = new int[size];

		front = 0;
		rear = 0;
	}
	~Circularqueue() {
		delete[] qslot;
	}
	bool isEmpty() { return front == rear; }
	bool isFull() { return (rear + 1) % size == front; }
	bool isThere(int val) {
		bool thereis = false;

		for (int i = front + 1; ((i - 1) % size) != rear; i++)
			if (qslot[i%size] == val)
				thereis = true;

		return thereis;
	}
	int distance(){
		if (rear > front)
			return rear - front;
		else
			return rear + size - front;
	}
	void enqueue(int val) {
		if (isFull())
			cout << "큐가 가득 차 있습니다." << endl;
		else {
			rear = (rear + 1) % size;
			qslot[rear] = val;
		}
	}
	int dequeue() {
		if ( isEmpty() ) {
			cout << "큐가 비어 있습니다." << endl;
			return -1;
		}
		else {
			front = (front + 1) % size;
			return qslot[front];
		}
	}
	void display() {	
		cout << "| ";
		for (int i = front + 1; ((i-1)%size)!= rear; i++)
			cout << qslot[i%size] << " ";
		cout << "|";
	}
};