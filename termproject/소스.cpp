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
	bool isEmpty() {				//stack이 비어있는지 확인하는 함수
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
	T pop() {						//stack의 top이 가리키는 위치에서 값을 하나 꺼내기
		if (!isEmpty()) {
			return st[top--];
		}
	}
	int toppriority() {				//top의 위치에 해당하는 값 반환
		return st[top];
	}
	void push(T data) {				//stack에 값 추가하기
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
			if (input[i] == '0') {		//0으로 시작하는 경우
				if (input[i + 1] == 'x') {	//다음 값이 x이면 16진수
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
				else if (input[i + 1] == 'b') {	//다음 값이 b이면 2진수
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
				else {	//다음 값이 x도, b도 아니라면 그냥 숫자 0을 의미
					stack->push((int)input[i] - '0');
					i++;
				}
			}
			else {	//0으로 시작하지 않는다면
				int priority = isop(input[i]);	//우선순위를 계산해보고
				if (priority == -2) {	//우선순위가 -2이면 즉, 연산자 이외의 값이면 10진수로 계산하여 stack에 넣어주기
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
				else {	//연산자인 경우
					if (op->isEmpty() || priority == 0) {	//연산자 stack이 비어있거나 우선순위가 0, 여는 괄호인 경우
						op->push(input[i]);		//무조건 stack에 넣기
					}
					else {
						if (priority == -1) {	//우선순위 -1, 닫는괄호인 경우
							int topop = isop(op->toppriority());
							int ispair = 0;
							while (!op->isEmpty()) {
								if (topop == 0)	//여는괄호를 만나면 break
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
						else {	//닫는 괄호가 아닌 경우
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
		cout << "계산기입니다. 수식을 입력해주세요." << endl;
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
			state = "연산 결과 : " + to_string(model->getstack()->pop());
		}
		else {
			state = "잘못된 입력/수식 입니다.(수식과 괄호쌍이 제대로 작성되었는지, 나누는 수가 0은 아닌지 확인해주세요.)";
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