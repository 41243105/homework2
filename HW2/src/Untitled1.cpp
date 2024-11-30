#include <iostream>
#include <cmath>
#include<ctime> 
#include <iomanip>
using namespace std;

class Term {
    friend class Polynomial;
public:
    float coef; // 係數
    int exp;    // 次方
};

class Polynomial {
	friend ostream& operator<<(ostream& os, const Polynomial& p);
	friend istream& operator>>(istream& is, Polynomial &p);
private:
    Term *termarray;
    int capacity; // 陣列容量
    int terms;    // 項數

public:
    Polynomial() : capacity(2), terms(0) {//預設陣列容量2 項數0 
        termarray = new Term[capacity];
    }
    ~Polynomial() {
        delete[] termarray;
    }

	void addcapacity() {		//增加新的陣列容量
        capacity *= 2;			//增加原先的2倍 
        Term *newarray = new Term[capacity];
        for (int i = 0; i < terms; i++) {	//複製原先的元素到新的那裏 
            newarray[i] = termarray[i];
        }
        delete[] termarray;		//刪除原先的陣列 
        termarray = newarray;	//指向新陣列 
    }
    
    void newterm(float coef, int exp) {		//合併多項式 
        if (coef == 0) return; 				// 忽略係數為 0 的項目
        for (int i = 0; i < terms; i++) {	
            if (termarray[i].exp == exp) {	//如果次方一樣 
                termarray[i].coef += coef; // 把次方一樣的系數加起來 
                if (termarray[i].coef == 0) { // 如果合併後係數為 0
                    for (int j = i; j < terms - 1; j++) {
                        termarray[j] = termarray[j + 1]; 	// 到下一個項目 
                    }
                    terms--; // 項目數-1 
                }
                return; // 完成後返回
            }
        }
        if (terms >= capacity) {	// 如果項目>=陣列容量  陣列容量不夠了 
            addcapacity(); 			// 增加陣列容量
        }
        termarray[terms].exp = exp;
        termarray[terms].coef = coef;
        terms++;
    }

    Polynomial add(const Polynomial& p) const {	//相加 
    Polynomial result; 							// 建立一個新對象
    for (int i = 0; i < terms; i++) {
        result.newterm(termarray[i].coef, termarray[i].exp); // 複製當前的多項式 
    }
    for (int i = 0; i < p.terms; i++) {
        result.newterm(p.termarray[i].coef, p.termarray[i].exp); // 加入另一個多項式到newterm合併 
    }
    return result; //把答案回傳回去
	}


    Polynomial mult(const Polynomial& p) const { 	// 相乘
        Polynomial result;							// 建立一個新對象
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < p.terms; j++) {
                float newcoef = termarray[i].coef * p.termarray[j].coef; // 係數相乘 
                int newexp = termarray[i].exp + p.termarray[j].exp; // 指數相加
                result.newterm(newcoef, newexp); 	// 新多項式到newterm合併
            }
        }
        return result;//把答案回傳回去 
    }

    double eval(double f) { // 帶數字進去
        double result = 0.0f;
        for (int i = 0; i < terms; i++) {
            result += termarray[i].coef * pow(f, termarray[i].exp);//把傳入的數字帶到多項式中 運算每個x結果加起來 
        }
        return result;//把答案回傳回去 
    }
};

	istream& operator>>(istream& is, Polynomial& p) {
    	while (true) {
        	float coef=0.0;
        	int exp=0;
        	char a, b;
        	is >> coef; // 輸入係數
        	is.get(a);  // 輸入 'x' 或其他字元
        	if (a == 'x') { // 如果是 'x'
        	    is.get(b);  // 輸入 '^' 或其他字元
        	    if (b == '^') {
        	        is >> exp; // 如果是 '^'輸入次方
        	    } else {
        	        exp = 1; // 如果有 'x' 但沒有 '^'次方就是 1
        	    }
        	} else {
        	    exp = 0; // 如果沒有 'x'，指數為 0
        	}
        	p.newterm(coef, exp); // 將係數與次方傳到 `newterm` 進行合併
        	if (is.peek() == '\n') break; // 遇到換行符結束輸入
    	}
    	return is;
	}

	ostream& operator<<(ostream& os, const Polynomial& p) {
	    if (p.terms == 0) {
	        os << "0"; // 如果沒有多項式顯示 0
	    } 
		else {
	        for (int i = 0; i < p.terms; i++) {
	            if (i > 0 && p.termarray[i].coef > 0) {
	                os << "+"; // 如果係數和項數都 >0顯示 '+'
	            }
	            os << p.termarray[i].coef; // 輸出係數
	            if (p.termarray[i].exp > 0) { // 次方 >0 則輸出 'x'
	                os << "x";
	            }
	            if (p.termarray[i].exp > 1) { // 次方 >1 則輸出 '^' 和次方
	                os << "^" << p.termarray[i].exp;
	            }
	        }
	    }
	    return os;
	}

int main() {
	clock_t start,finish;//計算時間的 
    Polynomial p1, p2;	//2個多項式 
    cout << "p1=";		// 3x^2+2x+1
    cin>>p1;
    cout << "p2="; 		// 2x^3+4
    cin>>p2;

    cout << "新增新的項目" << endl;
    cout << "項目的系數:"; 
    int exp;
    float coef;
    cin >> coef;			// 5
    cout << "項目的指數:"; 
    cin >> exp;				// 2
    p1.newterm(coef, exp);	//把新加的項目加到p1中 

    cout << "\n更新後的p1:"; 
    cout<<p1<<endl;			// 8x^2+2x+1

    cout << "\nadd" << endl;	
    start=clock();					//計算時間開始 
    Polynomial sum = p1.add(p2); 	//p1+p2 			
    cout << "p1+p2="<<sum<<endl;	// 8x^2+2x+5+2x^3
    finish=clock();					//計算時間結束 
    cout << "add() 需時: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;//計算add所需的時間 
    
    cout << "\nmult" << endl;
    cout << "p1*p2=";
    start=clock();					//計算時間開始 
    Polynomial xx = p1.mult(p2); 	//p1*p2 
	cout<<xx<<endl;					// 16x^5+32x^2+4x^4+8x+2x^3+4
	finish=clock();					//計算時間結束 
    cout << "mult() 需時: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;//計算mult所需的時間 


    double f = 0.0f;
    cout << "\neval" << endl;
    cout << "f的值:"; 				//輸入要帶進p1中的值
    start = clock();				//計算時間開始 
    cin >> f;						//1
    cout << "p1(" << f << ")=" << p1.eval(f) << endl;	// 11
    finish = clock();				//計算時間結束 
    cout << "eval() 需時: " << (double)(finish - start) / CLOCKS_PER_SEC << " s" << endl;//計算eval所需的時間 
    return 0;
}





