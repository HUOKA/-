#include<stdio.h>
#include<malloc.h>
typedef struct Polynomial
{
	float coef;           //系数
	int expn;			//指数
	struct Polynomial* next;
}*Polyn, Polynomial;               //Polyn为结点指针类型

void Insert(Polyn p, Polyn h)     //整理多项式，按照指数的降序排列
{
	if (p->coef == 0) free(p);       //系数为0的话释放结点
	else
	{
		Polyn q1, q2;
		q1 = h; q2 = h->next;
		while (q2 && p->expn < q2->expn)    //查找插入位置 倒序
		{
			q1 = q2;
			q2 = q2->next;
		}
		if (q2 && p->expn == q2->expn)       //将指数相同相合并
		{
			q2->coef += p->coef;
			free(p);
			if (!q2->coef)               //系数为0的话释放结点
			{
				q1->next = q2->next;
				free(q2);
			}
		}
		else
		{                              //指数为新时将结点插入
			p->next = q2;
			q1->next = p;
		}
	}
}

Polyn Create(Polyn head, int m)   //建立一个头指针为head、项数为m的一元多项式
{
	int i;
	Polyn p;
	p = head = (Polyn)malloc(sizeof(struct Polynomial));
	head->next = NULL;
	for (i = 0; i < m; i++)
	{
		p = (Polyn)malloc(sizeof(struct Polynomial));//建立新结点以接收数据
		printf("请输入第%d项的系数与指数:", i + 1);
		scanf_s("%f %d", &p->coef, &p->expn);
		Insert(p, head);
	}
	return head;
}

void Print(Polyn P) //多项式按幂从高到低排列输出
{
	Polyn q = P->next;
	int flag = 1;
	if (!q)               //若多项式为空，输出0
	{
		putchar('0');
		printf("\n");
		return;
	}
	while (q)
	{
		if (q->coef > 0 && flag != 1) putchar('+'); //系数大于0且不是第一项
		if (q->coef != 1 && q->coef != -1)          //系数非1或-1的普通情况
		{
			printf("%g", q->coef);
			if (q->expn == 1) putchar('X');
			else if (q->expn) printf("X^%d", q->expn);
		}
		else
		{
			if (q->coef == 1)
			{
				if (!q->expn) putchar('1');
				else if (q->expn == 1) putchar('X');
				else printf("X^%d", q->expn);
			}
			if (q->coef == -1)
			{
				if (!q->expn) printf("-1");
				else if (q->expn == 1) printf("-X");
				else printf("-X^%d", q->expn);
			}
		}
		q = q->next;
		flag++;
	}
	printf("\n");
}

void print1(Polyn P) //多项式按整数形式输出
{
	Polyn q = P->next;
	if (!q)          //若多项式为空，输出0
	{
		putchar('0');
		printf("\n");
		return;
	}
	while (q)
	{
		printf(",%g,%d", q->coef, q->expn);
		q = q->next;
	}
	printf("\n");
}

int compare(Polyn a, Polyn b)
{
	if (a && b)
	{
		if (!b || a->expn > b->expn) return 1;
		else if (!a || a->expn < b->expn) return -1;
		else return 0;
	}
	else if (!a && b) return -1;  //a多项式已空，但b多项式非空
	else return 1;             //b多项式已空，但a多项式非空
}

Polyn Add(Polyn pa, Polyn pb)//求解并建立多项式a+b，返回其头指针
{
	Polyn qa = pa->next;
	Polyn qb = pb->next;
	Polyn headc, hc, qc;
	hc = (Polyn)malloc(sizeof(struct Polynomial));//建立头结点
	hc->next = NULL;
	headc = hc;
	while (qa || qb)
	{
		qc = (Polyn)malloc(sizeof(struct Polynomial));
		switch (compare(qa, qb))
		{
		case 1:
		{
			qc->coef = qa->coef;
			qc->expn = qa->expn;
			qa = qa->next;
			break;
		}
		case 0:
		{
			qc->coef = qa->coef + qb->coef;
			qc->expn = qa->expn;
			qa = qa->next;
			qb = qb->next;
			break;
		}
		case -1:
		{
			qc->coef = qb->coef;
			qc->expn = qb->expn;
			qb = qb->next;
			break;
		}
		}
		if (qc->coef != 0)
		{
			qc->next = hc->next;
			hc->next = qc;
			hc = qc;
		}
		else free(qc);//当相加系数为0时，释放该结点
	}
	return headc;
}

Polyn Sub(Polyn pa, Polyn pb)//求解并建立多项式a-b，返回其头指针
{
	Polyn h = pb;
	Polyn p = pb->next;
	Polyn pd;
	while (p)
	{           //将pb的系数取反
		p->coef *= -1;
		p = p->next;
	}
	pd = Add(pa, h);
	for (p = h->next; p; p = p->next)    //恢复pb的系数
		p->coef *= -1;
	return pd;
}



Polyn Derivative(Polyn head) 
{
	//求解并建立导函数多项式，并返回其头指针
	Polyn q = head->next, p1, p2, hd;
	hd = p1 = (Polyn)malloc(sizeof(struct Polynomial));//建立头结点
	hd->next = NULL;
	while (q)
	{
		if (q->expn != 0)
		{               //该项不是常数项时
			p2 = (Polyn)malloc(sizeof(struct Polynomial));
			p2->coef = q->coef * q->expn;
			p2->expn = q->expn - 1;
			p2->next = p1->next;//连接结点
			p1->next = p2;
			p1 = p2;
		}
		q = q->next;
	}
	return hd;
}

Polyn MultiplyPolyn(Polyn pa, Polyn pb) 
{
	//求解并建立多项式a*b，返回其头指针
	Polyn hf, pf;
	Polyn qa = pa->next;
	Polyn qb = pb->next;
	hf = (Polyn)malloc(sizeof(struct Polynomial));//建立头结点
	hf->next = NULL;
	for (; qa; qa = qa->next)
	{
		for (qb = pb->next; qb; qb = qb->next)
		{
			pf = (Polyn)malloc(sizeof(struct Polynomial));
			pf->coef = qa->coef * qb->coef;
			pf->expn = qa->expn + qb->expn;
			Insert(pf, hf);//调用Insert函数以合并指数相同的项
		}
	}
	return hf;
}


float Value(Polyn head, float x)//输入x值，计算并返回多项式的值
{
	Polyn p;
	int i;
	float sum = 0, t;
	for (p = head->next; p; p = p->next)
	{
		t = 1;
		for (i = p->expn; i != 0;)
		{
			if (i < 0) { t /= x; i++; }   //指数小于0，进行除法
			else { t *= x; i--; }      //指数大于0，进行乘法
		}
		sum += p->coef * t;
	}
	return sum;
}

int main()
{
	int m, n, flag = 0, c;
	float x;
	Polyn pa = 0, pb = 0, pc, pd;//定义各式的头指针，pa与pb在使用前付初值NULL
	printf("请输入a的项数:");
	scanf_s("%d", &m);
	pa = Create(pa, m);//建立多项式a
	printf("请输入b的项数:");
	scanf_s("%d", &n);
	pb = Create(pb, n);//建立多项式a
	//输出菜单
	printf("**********************************************\n");
	printf("操作提示：\n\t1.输出多项式a和b\n\t2.建立多项式a+b\n\t3.建立多项式a-b\n");
	printf("\t4.计算多项式a在x处的值\n\t");
	printf("5.退出\n**********************************************\n");
	for (;; flag = 0)
	{
		printf("执行操作");
		scanf_s("%d", &flag);
		if (flag == 1)
		{
			printf("选项：\n\t1.按整数形式输出\n\t2.按数学表达式输出\n");
			printf("我的选择:");
			scanf_s("%d", &c);
			switch (c)
			{
			case 1:
				printf("多项式a：");
				printf("%d", m);
				print1(pa);
				printf("多项式b：");
				printf("%d", n);
				print1(pb);
				break;
			case 2:
				printf("多项式a：");
				Print(pa);
				printf("多项式b：");
				Print(pb);
				break;
			default:break;
			}
			continue;
		}
		if (flag == 2)
		{
			pc = Add(pa, pb);
			printf("多项式a+b：");
			Print(pc);
			continue;
		}
		if (flag == 3)
		{
			pd = Sub(pa, pb);
			printf("多项式a-b：");
			Print(pd);
			continue;
		}
		if (flag == 4)
		{
			printf("请选择你要计算的多项式:\n1.多项式a\n2.多项式b\n");
			scanf_s("%d", &c);
			switch (c)
			{
			case 1:
				printf("输入x的值：x=");
				scanf_s("%f", &x);
				printf("多项式a的值%g\n", Value(pa, x));
				break;
			case 2:
				printf("输入x的值：x=");
				scanf_s("%f", &x);
				printf("多项式b的值%g\n", Value(pb, x));
				break;
			default:break;
			}
			continue;
		}
		if (flag == 5) break;
		if (flag < 1 || flag>5) printf(" Enter Error!!!\n"); continue;
	}
	return 0;
}
