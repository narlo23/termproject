#include <iostream>
#include <string>
#include <cmath>

using namespace std;

template <typename T>
class Stack {		
private:
	int top;
	T* st;
public:
	Stack() {
		top = -1;
		st = new T[100];
	}
	bool isEmpty() {				//stack�� ����ִ��� Ȯ���ϴ� �Լ�
		if (top == -1) {
			return 1;
		}
		else {
			return 0;
		}
	}
	int size() {
		return top + 1;
	}
	T pop() {						//stack�� top�� ����Ű�� ��ġ���� ���� �ϳ� ������
		if (!isEmpty()) {
			return st[top--];
		}
	}
	int toppriority() {				//top�� ��ġ�� �ش��ϴ� �� ��ȯ
		return st[top];
	}
	void push(T data) {				//stack�� �� �߰��ϱ�
		st[++top] = data;		
	}
};

class calcModel {
private:
	Stack<int>* stack;
	Stack<char>* op;
	string input;
	int isok;
	int i;

public:
	calcModel() {
		this->stack = new Stack<int>();
		this->op = new Stack<char>();
		this->i = 0;
	}
	Stack<int>* getstack() {
		return this->stack;
	}

	void setstr(string str) {
		this->input = str;
	}

	string getstr() {
		return this->input;
	}

	void setisok(int isok) {
		this->isok = isok;
	}

	int getisok() {
		return this->isok;
	}

	int calc(int op1, char op, int op2) {
		switch (op) {
		case '+':
			return op2 + op1;
		case '-':
			return op2 - op1;
		case '*':
			return op2 * op1;
		case '/':
			return op2 / op1;
		default:
			return -1;
		}
	}

	int isop(char c) {
		switch (c) {
		case '+':
		case '-':
			return 1;
		case '*':
		case '/':
			return 2;
		case '(':
			return 0;
		case ')':
			return -1;
		default:
			return -2;
		}
	}

	int chartoint(char c) {
		if (c == 'x') {
			return 16;
		}
		else {
			return 2;
		}
	}

	void calculate() {
		while (input[i] != NULL) {
			if (input[i] == '0') {		//0���� �����ϴ� ���
				if (input[i + 1] == 'x') {	//���� ���� x�̸� 16����
					i += 2;
					int cnt = 0;
					int num = 0;
					while (input[i + cnt] != NULL && isop(input[i + cnt]) == -2) {
						cnt++;
					}
					for (int j = 0;j < cnt;j++) {
						int tmp = (int)input[i + cnt - 1 - j];
						if (tmp >= '0' && tmp <= '9') {
							num += (tmp - '0') * pow(chartoint(input[i - 1]), j);
						}
						else if (tmp >= 'A' && tmp <= 'F') {
							num += (tmp - 'A' + 10) * pow(chartoint(input[i - 1]), j);
						}
						else {
							isok = 0;
							break;
						}
					}
					stack->push(num);
					i += cnt;
				}
				else if (input[i + 1] == 'b') {	//���� ���� b�̸� 2����
					i += 2;
					int cnt = 0;
					int num = 0;
					while (input[i + cnt] != NULL && isop(input[i + cnt]) == -2) {
						cnt++;
					}
					for (int j = 0;j < cnt;j++) {
						int tmp = (int)input[i + cnt - 1 - j];
						if (tmp == '0' || tmp == '1') {
							num += (tmp - '0') * pow(chartoint(input[i - 1]), j);
						}
						else {
							isok = 0;
							break;
						}
					}
					stack->push(num);
					i += cnt;
				}
				else {	//���� ���� x��, b�� �ƴ϶�� �׳� ���� 0�� �ǹ�
					stack->push((int)input[i] - '0');
					i++;
				}
			}
			else {	//0���� �������� �ʴ´ٸ�
				int priority = isop(input[i]);	//�켱������ ����غ���
				if (priority == -2) {	//�켱������ -2�̸� ��, ������ �̿��� ���̸� 10������ ����Ͽ� stack�� �־��ֱ�
					if (isdigit(input[i])) {
						int cnt = 0;
						int dec = 0;
						while (input[i + cnt] != NULL && isop(input[i + cnt]) == -2) {
							cnt++;
						}
						for (int j = 0;j < cnt;j++) {
							dec += (((int)input[i + cnt - 1 - j] - 48) * pow(10, j));
						}
						stack->push(dec);
						i += cnt;
					}
					else {
						isok = 0;
						break;
					}
				}
				else {	//�������� ���
					if (op->isEmpty() || priority == 0) {	//������ stack�� ����ְų� �켱������ 0, ���� ��ȣ�� ���
						op->push(input[i]);		//������ stack�� �ֱ�
					}
					else {
						if (priority == -1) {	//�켱���� -1, �ݴ°�ȣ�� ���
							int topop = isop(op->toppriority());
							int ispair = 0;
							while (!op->isEmpty()) {
								if (topop == 0)	//���°�ȣ�� ������ break
								{
									ispair = 1;
									break;
								}
								else {
									int op1 = stack->pop();
									int op2 = stack->pop();
									char tmpop = op->pop();
									if (tmpop == '/' && op1 == 0) {
										isok = 0;
										break;
									}
									else {
										stack->push(calc(op1, tmpop, op2));
										topop = isop(op->toppriority());
									}
								}
							}
							op->pop();
							if (!ispair) {
								isok = 0;
								break;
							}
						}
						else {	//�ݴ� ��ȣ�� �ƴ� ���
							if (op->isEmpty()) {
								op->push(input[i]);
							}
							else {
								int topop = isop(op->toppriority());
								if (topop >= priority) {
									if (stack->size() < 2) {
										isok = 0;
										break;
									}
									else {
										int op1 = stack->pop();
										int op2 = stack->pop();
										char tmpop = op->pop();
										if (tmpop == '/' && op1 == 0) {
											isok = 0;
											break;
										}
										else {
											stack->push(calc(op1, tmpop, op2));
											topop = isop(op->toppriority());
										}
									}
								}
								op->push(input[i]);
							}
						}
					}
					i++;
				}
			}
		}

		while (!stack->isEmpty() && !op->isEmpty()) {
			if (stack->size() < 2) {
				isok = 0;
				break;
			}
			else {
				int op1 = stack->pop();
				int op2 = stack->pop();
				char tmpop = op->pop();
				if (tmpop == '/' && op1 == 0) {
					isok = 0;
					break;
				}
				else {
					stack->push(calc(op1, tmpop, op2));
				}
			}
		}
	}
};

class calcView {
public:
	string inputop() {
		string input;
		getline(cin, input);
		input.erase(remove(input.begin(), input.end(), ' '), input.end());
		return input;
	}

	void printstartmsg() {
		cout << "�����Դϴ�. ������ �Է����ּ���." << endl;
	}

	void printresultmsg(string result) {
		cout << result << endl;
	}
};

class calcController {
private:
	calcModel* model;
	calcView* view;
	int isok = 1;

public:
	calcController(calcModel* model, calcView* view) {
		this->model = model;
		this->view = view;
		this->model->setisok(isok);
		view->printstartmsg();
	}

	void printresult() {
		string state;
		if (model->getisok()) {
			state = "���� ��� : " + to_string(model->getstack()->pop());
		}
		else {
			state = "�߸��� �Է�/���� �Դϴ�.(���İ� ��ȣ���� ����� �ۼ��Ǿ�����, ������ ���� 0�� �ƴ��� Ȯ�����ּ���.)";
		}
		view->printresultmsg(state);
	}

	void setinput() {
		model->setstr(view->inputop());
	}

	void calcresult() {
		model->calculate();
	}
};



int main() {
	calcModel* model = new calcModel();
	calcView* view = new calcView();

	calcController* controller = new calcController(model, view);

	controller->setinput();

	controller->calcresult();

	controller->printresult();
}