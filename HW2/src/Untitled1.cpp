#include <iostream>
#include <cmath>
#include<ctime> 
#include <iomanip>
using namespace std;

class Term {
    friend class Polynomial;
public:
    float coef; // �Y��
    int exp;    // ����
};

class Polynomial {
	friend ostream& operator<<(ostream& os, const Polynomial& p);
	friend istream& operator>>(istream& is, Polynomial &p);
private:
    Term *termarray;
    int capacity; // �}�C�e�q
    int terms;    // ����

public:
    Polynomial() : capacity(2), terms(0) {//�w�]�}�C�e�q2 ����0 
        termarray = new Term[capacity];
    }
    ~Polynomial() {
        delete[] termarray;
    }

	void addcapacity() {		//�W�[�s���}�C�e�q
        capacity *= 2;			//�W�[�����2�� 
        Term *newarray = new Term[capacity];
        for (int i = 0; i < terms; i++) {	//�ƻs�����������s������ 
            newarray[i] = termarray[i];
        }
        delete[] termarray;		//�R��������}�C 
        termarray = newarray;	//���V�s�}�C 
    }
    
    void newterm(float coef, int exp) {		//�X�֦h���� 
        if (coef == 0) return; 				// �����Y�Ƭ� 0 ������
        for (int i = 0; i < terms; i++) {	
            if (termarray[i].exp == exp) {	//�p�G����@�� 
                termarray[i].coef += coef; // �⦸��@�˪��t�ƥ[�_�� 
                if (termarray[i].coef == 0) { // �p�G�X�֫�Y�Ƭ� 0
                    for (int j = i; j < terms - 1; j++) {
                        termarray[j] = termarray[j + 1]; 	// ��U�@�Ӷ��� 
                    }
                    terms--; // ���ؼ�-1 
                }
                return; // �������^
            }
        }
        if (terms >= capacity) {	// �p�G����>=�}�C�e�q  �}�C�e�q�����F 
            addcapacity(); 			// �W�[�}�C�e�q
        }
        termarray[terms].exp = exp;
        termarray[terms].coef = coef;
        terms++;
    }

    Polynomial add(const Polynomial& p) const {	//�ۥ[ 
    Polynomial result; 							// �إߤ@�ӷs��H
    for (int i = 0; i < terms; i++) {
        result.newterm(termarray[i].coef, termarray[i].exp); // �ƻs��e���h���� 
    }
    for (int i = 0; i < p.terms; i++) {
        result.newterm(p.termarray[i].coef, p.termarray[i].exp); // �[�J�t�@�Ӧh������newterm�X�� 
    }
    return result; //�⵪�צ^�Ǧ^�h
	}


    Polynomial mult(const Polynomial& p) const { 	// �ۭ�
        Polynomial result;							// �إߤ@�ӷs��H
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < p.terms; j++) {
                float newcoef = termarray[i].coef * p.termarray[j].coef; // �Y�Ƭۭ� 
                int newexp = termarray[i].exp + p.termarray[j].exp; // ���Ƭۥ[
                result.newterm(newcoef, newexp); 	// �s�h������newterm�X��
            }
        }
        return result;//�⵪�צ^�Ǧ^�h 
    }

    double eval(double f) { // �a�Ʀr�i�h
        double result = 0.0f;
        for (int i = 0; i < terms; i++) {
            result += termarray[i].coef * pow(f, termarray[i].exp);//��ǤJ���Ʀr�a��h������ �B��C��x���G�[�_�� 
        }
        return result;//�⵪�צ^�Ǧ^�h 
    }
};

	istream& operator>>(istream& is, Polynomial& p) {
    	while (true) {
        	float coef=0.0;
        	int exp=0;
        	char a, b;
        	is >> coef; // ��J�Y��
        	is.get(a);  // ��J 'x' �Ψ�L�r��
        	if (a == 'x') { // �p�G�O 'x'
        	    is.get(b);  // ��J '^' �Ψ�L�r��
        	    if (b == '^') {
        	        is >> exp; // �p�G�O '^'��J����
        	    } else {
        	        exp = 1; // �p�G�� 'x' ���S�� '^'����N�O 1
        	    }
        	} else {
        	    exp = 0; // �p�G�S�� 'x'�A���Ƭ� 0
        	}
        	p.newterm(coef, exp); // �N�Y�ƻP����Ǩ� `newterm` �i��X��
        	if (is.peek() == '\n') break; // �J�촫��ŵ�����J
    	}
    	return is;
	}

	ostream& operator<<(ostream& os, const Polynomial& p) {
	    if (p.terms == 0) {
	        os << "0"; // �p�G�S���h������� 0
	    } 
		else {
	        for (int i = 0; i < p.terms; i++) {
	            if (i > 0 && p.termarray[i].coef > 0) {
	                os << "+"; // �p�G�Y�ƩM���Ƴ� >0��� '+'
	            }
	            os << p.termarray[i].coef; // ��X�Y��
	            if (p.termarray[i].exp > 0) { // ���� >0 �h��X 'x'
	                os << "x";
	            }
	            if (p.termarray[i].exp > 1) { // ���� >1 �h��X '^' �M����
	                os << "^" << p.termarray[i].exp;
	            }
	        }
	    }
	    return os;
	}

int main() {
	clock_t start,finish;//�p��ɶ��� 
    Polynomial p1, p2;	//2�Ӧh���� 
    cout << "p1=";		// 3x^2+2x+1
    cin>>p1;
    cout << "p2="; 		// 2x^3+4
    cin>>p2;

    cout << "�s�W�s������" << endl;
    cout << "���ت��t��:"; 
    int exp;
    float coef;
    cin >> coef;			// 5
    cout << "���ت�����:"; 
    cin >> exp;				// 2
    p1.newterm(coef, exp);	//��s�[�����إ[��p1�� 

    cout << "\n��s�᪺p1:"; 
    cout<<p1<<endl;			// 8x^2+2x+1

    cout << "\nadd" << endl;	
    start=clock();					//�p��ɶ��}�l 
    Polynomial sum = p1.add(p2); 	//p1+p2 			
    cout << "p1+p2="<<sum<<endl;	// 8x^2+2x+5+2x^3
    finish=clock();					//�p��ɶ����� 
    cout << "add() �ݮ�: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;//�p��add�һݪ��ɶ� 
    
    cout << "\nmult" << endl;
    cout << "p1*p2=";
    start=clock();					//�p��ɶ��}�l 
    Polynomial xx = p1.mult(p2); 	//p1*p2 
	cout<<xx<<endl;					// 16x^5+32x^2+4x^4+8x+2x^3+4
	finish=clock();					//�p��ɶ����� 
    cout << "mult() �ݮ�: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;//�p��mult�һݪ��ɶ� 


    double f = 0.0f;
    cout << "\neval" << endl;
    cout << "f����:"; 				//��J�n�a�ip1������
    start = clock();				//�p��ɶ��}�l 
    cin >> f;						//1
    cout << "p1(" << f << ")=" << p1.eval(f) << endl;	// 11
    finish = clock();				//�p��ɶ����� 
    cout << "eval() �ݮ�: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;//�p��eval�һݪ��ɶ� 
    return 0;
}





