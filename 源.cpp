#include<stdio.h>
#include<malloc.h>
typedef struct Polynomial
{
	float coef;           //ϵ��
	int expn;			//ָ��
	struct Polynomial* next;
}*Polyn, Polynomial;               //PolynΪ���ָ������

void Insert(Polyn p, Polyn h)     //�������ʽ������ָ���Ľ�������
{
	if (p->coef == 0) free(p);       //ϵ��Ϊ0�Ļ��ͷŽ��
	else
	{
		Polyn q1, q2;
		q1 = h; q2 = h->next;
		while (q2 && p->expn < q2->expn)    //���Ҳ���λ�� ����
		{
			q1 = q2;
			q2 = q2->next;
		}
		if (q2 && p->expn == q2->expn)       //��ָ����ͬ��ϲ�
		{
			q2->coef += p->coef;
			free(p);
			if (!q2->coef)               //ϵ��Ϊ0�Ļ��ͷŽ��
			{
				q1->next = q2->next;
				free(q2);
			}
		}
		else
		{                              //ָ��Ϊ��ʱ��������
			p->next = q2;
			q1->next = p;
		}
	}
}

Polyn Create(Polyn head, int m)   //����һ��ͷָ��Ϊhead������Ϊm��һԪ����ʽ
{
	int i;
	Polyn p;
	p = head = (Polyn)malloc(sizeof(struct Polynomial));
	head->next = NULL;
	for (i = 0; i < m; i++)
	{
		p = (Polyn)malloc(sizeof(struct Polynomial));//�����½���Խ�������
		printf("�������%d���ϵ����ָ��:", i + 1);
		scanf_s("%f %d", &p->coef, &p->expn);
		Insert(p, head);
	}
	return head;
}

void Print(Polyn P) //����ʽ���ݴӸߵ����������
{
	Polyn q = P->next;
	int flag = 1;
	if (!q)               //������ʽΪ�գ����0
	{
		putchar('0');
		printf("\n");
		return;
	}
	while (q)
	{
		if (q->coef > 0 && flag != 1) putchar('+'); //ϵ������0�Ҳ��ǵ�һ��
		if (q->coef != 1 && q->coef != -1)          //ϵ����1��-1����ͨ���
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

void print1(Polyn P) //����ʽ��������ʽ���
{
	Polyn q = P->next;
	if (!q)          //������ʽΪ�գ����0
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
	else if (!a && b) return -1;  //a����ʽ�ѿգ���b����ʽ�ǿ�
	else return 1;             //b����ʽ�ѿգ���a����ʽ�ǿ�
}

Polyn Add(Polyn pa, Polyn pb)//��Ⲣ��������ʽa+b��������ͷָ��
{
	Polyn qa = pa->next;
	Polyn qb = pb->next;
	Polyn headc, hc, qc;
	hc = (Polyn)malloc(sizeof(struct Polynomial));//����ͷ���
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
		else free(qc);//�����ϵ��Ϊ0ʱ���ͷŸý��
	}
	return headc;
}

Polyn Sub(Polyn pa, Polyn pb)//��Ⲣ��������ʽa-b��������ͷָ��
{
	Polyn h = pb;
	Polyn p = pb->next;
	Polyn pd;
	while (p)
	{           //��pb��ϵ��ȡ��
		p->coef *= -1;
		p = p->next;
	}
	pd = Add(pa, h);
	for (p = h->next; p; p = p->next)    //�ָ�pb��ϵ��
		p->coef *= -1;
	return pd;
}



Polyn Derivative(Polyn head) 
{
	//��Ⲣ��������������ʽ����������ͷָ��
	Polyn q = head->next, p1, p2, hd;
	hd = p1 = (Polyn)malloc(sizeof(struct Polynomial));//����ͷ���
	hd->next = NULL;
	while (q)
	{
		if (q->expn != 0)
		{               //����ǳ�����ʱ
			p2 = (Polyn)malloc(sizeof(struct Polynomial));
			p2->coef = q->coef * q->expn;
			p2->expn = q->expn - 1;
			p2->next = p1->next;//���ӽ��
			p1->next = p2;
			p1 = p2;
		}
		q = q->next;
	}
	return hd;
}

Polyn MultiplyPolyn(Polyn pa, Polyn pb) 
{
	//��Ⲣ��������ʽa*b��������ͷָ��
	Polyn hf, pf;
	Polyn qa = pa->next;
	Polyn qb = pb->next;
	hf = (Polyn)malloc(sizeof(struct Polynomial));//����ͷ���
	hf->next = NULL;
	for (; qa; qa = qa->next)
	{
		for (qb = pb->next; qb; qb = qb->next)
		{
			pf = (Polyn)malloc(sizeof(struct Polynomial));
			pf->coef = qa->coef * qb->coef;
			pf->expn = qa->expn + qb->expn;
			Insert(pf, hf);//����Insert�����Ժϲ�ָ����ͬ����
		}
	}
	return hf;
}


float Value(Polyn head, float x)//����xֵ�����㲢���ض���ʽ��ֵ
{
	Polyn p;
	int i;
	float sum = 0, t;
	for (p = head->next; p; p = p->next)
	{
		t = 1;
		for (i = p->expn; i != 0;)
		{
			if (i < 0) { t /= x; i++; }   //ָ��С��0�����г���
			else { t *= x; i--; }      //ָ������0�����г˷�
		}
		sum += p->coef * t;
	}
	return sum;
}

int main()
{
	int m, n, flag = 0, c;
	float x;
	Polyn pa = 0, pb = 0, pc, pd;//�����ʽ��ͷָ�룬pa��pb��ʹ��ǰ����ֵNULL
	printf("������a������:");
	scanf_s("%d", &m);
	pa = Create(pa, m);//��������ʽa
	printf("������b������:");
	scanf_s("%d", &n);
	pb = Create(pb, n);//��������ʽa
	//����˵�
	printf("**********************************************\n");
	printf("������ʾ��\n\t1.�������ʽa��b\n\t2.��������ʽa+b\n\t3.��������ʽa-b\n");
	printf("\t4.�������ʽa��x����ֵ\n\t");
	printf("5.�˳�\n**********************************************\n");
	for (;; flag = 0)
	{
		printf("ִ�в���");
		scanf_s("%d", &flag);
		if (flag == 1)
		{
			printf("ѡ�\n\t1.��������ʽ���\n\t2.����ѧ���ʽ���\n");
			printf("�ҵ�ѡ��:");
			scanf_s("%d", &c);
			switch (c)
			{
			case 1:
				printf("����ʽa��");
				printf("%d", m);
				print1(pa);
				printf("����ʽb��");
				printf("%d", n);
				print1(pb);
				break;
			case 2:
				printf("����ʽa��");
				Print(pa);
				printf("����ʽb��");
				Print(pb);
				break;
			default:break;
			}
			continue;
		}
		if (flag == 2)
		{
			pc = Add(pa, pb);
			printf("����ʽa+b��");
			Print(pc);
			continue;
		}
		if (flag == 3)
		{
			pd = Sub(pa, pb);
			printf("����ʽa-b��");
			Print(pd);
			continue;
		}
		if (flag == 4)
		{
			printf("��ѡ����Ҫ����Ķ���ʽ:\n1.����ʽa\n2.����ʽb\n");
			scanf_s("%d", &c);
			switch (c)
			{
			case 1:
				printf("����x��ֵ��x=");
				scanf_s("%f", &x);
				printf("����ʽa��ֵ%g\n", Value(pa, x));
				break;
			case 2:
				printf("����x��ֵ��x=");
				scanf_s("%f", &x);
				printf("����ʽb��ֵ%g\n", Value(pb, x));
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
