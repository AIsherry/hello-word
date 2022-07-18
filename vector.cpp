#include<iostream>
#include"vector.h"
#include<cstdlib>
template<class T> //Ԫ������
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)    //����������A[lo, hi]Ϊ������������
{
	_elem = new T[_capacity = 2 * (hi - lo)];   _size = 0;    //����ռ� ����ģ����
	while (lo < hi) //A[lo, hi]�ڵ�Ԫ����һ
		_elem[_size++] = A[lo++];  //������_elem[0, hi -low]
}
template<class T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V)    //����
{
	if (_elem)delete[]_elem; //�ͷ�ԭ������
	copyFrom(V._elem, 0, V._size()); //���帴��
	return *this;     //���ص�ǰ��������ã��Ա���ʽ��ֵ
}
template<class T>
void Vector<T>::expand()
{
	if (_size < _capacity)return;    //��δ��Ա����������
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;   //��������С����
	T*oldElem = _elem; _elem = new T[2 * _capacity];          //�����ӱ�
	for (int i = 0; 0 < _size; i++)
		_elem[i] = oldElem[i];     //����ԭ��������
	delete[]oldElem;  //�ͷ�ԭ�ռ�
}
template<class T>
void Vector<T>::shrink()            //װ�����ӹ�Сʱѹ��������ռ�ռ�
{
	if (_capacity < DEFAULT_CAPACITY<<1)return;    //����������DEFAULT_CAPACITY����
	if (_size << 2 > _capacity)return;     //��25%Ϊ��
	T* oldElem = _elem; _elem = new T[_capacity >>= 1]; //��������
	for (int i = 0; i < _size; i++) _elem[i] = oldElem[i];    //����ԭ��������
	delete[]oldElem;
}
template<class T>
T& Vector<T>::operator[](Rank r)const
{
	return _elem[r];     //assert:0<=r<_size
}
template<class T>
void permute(Vector<T>& V)      //�������������ʹ��Ԫ�ظ��ʳ����ڸ�λ��
{
	for (int i = V.size(); i > 0; i--)    //�Ժ���ǰ
		swap(V._elem[i-1], V._elem[rand() % i]);    //V[i-1] ��V[0,i] ��ĳһ���Ԫ�ؽ���
}
template<class T>
void Vector<T>::unsort(Rank lo, Rank hi)         //equality odds �����������[lo,hi]
{
	T* V = _elem + lo;   //��������_elem[lo,hi]������һ����V[0,hi-lo]
	for (Rank i = hi - lo; i > 0; i--)   //�Ժ���ǰ
		swap(V[i - 1], V[rand() % i]);   //��V[i-1]��V[0,i]��ĳһԪ���������
}
template<class T>
Rank Vector<T>::find(T const& e,Rank lo, Rank hi)const      //��������(ֻ֧�ֱȶԲ���)��˳����ң��������һ��Ԫ��e��λ�ã�ʧЧʱ������lo-1
{
	while ((lo < hi--) && (e != _elem[hi]));
	return hi;
}
template<class T>
Rank Vector<T>::insert(Rank r, T const& e)  //��e���뵽 ��Ϊr
{
	expand();         //���б�Ҫ������
	for (int i = _size; i > r; i--) _elem[i] = _elem[i - 1];
	_elem[r] = e; _size++;     //������Ԫ�ز���������
	return r;  //������
}
template<class T>
int Vector<T>::remove(Rank lo, Rank hi)     //ɾ������[lo,hi]  ��ֱ�ӰѲ�ɾ����Ԫ���ƶ���ɾ������
{
	if (lo == hi) return 0;    //����Ч�ʿ��ǣ����������˻����������remove[0,0]
	while (hi < _size)_elem[lo++] = _elem[hi++];   //��[hi,size]��Ԫ�طŵ�[lo++],˳��ǰ��hi-lo
	_size = lo;   //���¹�ģ��ֱ�Ӷ���β��[lo,size = hi]
	shrink();   //���б�Ҫ��������
	return hi - lo;  //���ر�ɾ��Ԫ�ص���Ŀ
}
template<class T>
T Vector<T>::remove(Rank r) //ɾ����������Ϊr��Ԫ��   0 <= r < size
{
	T e = _elem[r];  //���ݱ�ɾ��Ԫ��
	remove(r, r + 1);    //��������ɾ���㷨����Ч�ڶ�����[r,r+1]��ɾ��
	return e;         //���ر�ɾ��Ԫ��
}
template<class T>
int Vector<T>::deduplicate()    //ɾ�������������ظ�Ԫ��(��Ч��)
{
	int oldsize = _size;    //record original size
	Rank i = 1;             //��_elem[1]��ʼ
	while (i < _size)          //��ǰ�����һ�����Ԫ��
		(find(_elem[i], 0, i)) ?   //����ǰ׺��Ѱ����֮��ͬ��(����һ��)
		i++ : remove(i);    //������ͬ������������̣�����ɾ����ͬ��
	return oldsize - _size;    //������ģ�仯��������ɾ��Ԫ������
}
template<class T>void Vector<T>::traverse(void (*visit)(T&))    //��������ָ�����
{
	for (int i = 0; i < _size; i++)visit(_elem[i]);
}    //��������
template<class T>template<class VST>  //Ԫ�����Ͳ�����
void Vector<T>::traverse(VST& visit)   //���������������
{for (int i = 0; i < _size; i++)visit(_elem[i]);}
template<class T>int Vector<T>::disordered()const         //��������Ԫ���е���������Ԫ�ضԵ�����
{
	int n = 0;      //������
	for (int i = 1; i < _size; i++)
		if (_elem[i - 1] > _elem[i]) n++;     //���������
	return n;   //���������ҽ���n=0
}
template <class T> int Vector<T>::uniquify()   //���������ظ�Ԫ���޳�������Ч�棩
{
	Rank i = 0, j = 0;  //���Ի���Ԫ�ص���
	while (++j < _size)
		if (_elem[i] != _elem[j])   //������ͬ��
			_elem[++i] = _elem[j];  //���ֲ�ͬԪ��ʱ����ǰ����������ǰ���Ҳ�
	_size = ++i; shrink();  //ֱ�ӽس�β������Ԫ��
	return j - i;   //������ģ�仯��������ɾ��Ԫ������
}
template<class T>static Rank binSearch(T* A, T const& e, Rank lo, Rank hi)      //���ֲ��� �汾A
{
	while (lo < hi)
	{
		Rank mi = (lo + hi) >> 1;   //������Ϊ���
		if (e < A[mi]) hi = mi;    //����ǰ���[lo,mi] ��������
		else if (A[mi] < e) lo = mi + 1;
		else    return mi;     //��mi������
	}   //�ɹ����ҿ�����ǰ��ֹ
	return -1;    //����ʧ��
}   //�ж��Ԫ������ʱ �����ܱ�֤����������ߣ� ����ʧ��ʱ���򵥵ط���-1��������ָʾʧ�ܵ�λ��
template<class T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi)const    //assert:0<=lo<hi<=_size
{
	return(rand() % 2) ?  //����50%�ĸ������ʹ�ö��ֲ��һ�Fibonacci����
		binSearch(_elem, e, lo, hi) : fibSearch(_elem,e, lo, hi);
}
template<class T>
static Rank binSearch(T* A, T const& e, Rank lo, Rank hi)        //���ֲ��� ���汾B
{
	while (1 < hi - lo)   //ÿ������������һ�αȽ��ж� ����������֧,�ɹ����Ҳ�����ǰ��ֹ
	{
		Rank mi = (lo + hi) / 2;
		(e < A[mi]) ? hi = mi : lo = mi;      //���ȽϺ�ȷ������[lo.mi]or [mi,hi]
	}  //����ʱhi = lo+1 ,�����������һ��Ԫ��A[lo]
	return(e == A[lo]) ? lo; -1;  //���ҳɹ�ʱ���ض�Ӧ���ȣ����򷵻�-1
}   //�ж��Ԫ������ʱ�����ܱ�֤�����������;����ʧ��ʱ���򵥵ķ���-1��������ָʾʧ�ܵ�λ��
template<class T>
static Rank binSearch(T* A, T const& e, Rank lo, Rank hi)    //���ֲ��� �汾C
{
	while (lo < hi)    //ÿ������������һ���жϣ���������֧
	{
		Rank mi = (lo + hi) / 2;
		(e < A[mi]) ? hi = mi:lo = mi+1
	}
	return --lo;   //ѭ������ʱ��loΪ����e��Ԫ�ص���С�ȣ���lo-1��������e��Ԫ�ص������
}       //�ж������Ԫ��ʱ�����ܱ�֤����������ߣ�����ʧ��ʱ���ܹ�����ʧ�ܵ�λ��
template<class T>
void Vector<T>::sort(Rank lo, Rank hi)
{
	switch (rand() % 5)   //���ѡȡ�����㷨���ɸ��ݾ���������ص����ѡȡ������
	{
	case 1:bubbleSort(lo, hi); break;   //��������
	case 2:selectionSort(lo, hi); break;  //ѡ������
	case 3:mergeSort(lo, hi); break;    //�鲢����
	case 4:heapSort(lo, hi);  break;   //������
	default:quickSort(lo, hi); break;   //��������
	}
}
template<class T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
	while (!bubble(lo, hi--));   //������ɨ�轻����ֱ��ȫ��
}
template<class T>
bool Vector<T>::bubble(Rank lo, Rank hi)
{
	bool sorted = true;    //���������־
	while (++lo < hi)          
	{
		if (_elem[lo - 1] > _elem[lo])   //��������
			sorted = false;
		swap(_elem[lo - 1], _elem[lo]);   //ͨ������ʹ�ֲ�����
	}
	return sorted; //���������־
}
template<class T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
	if (hi - lo < 2)return;   //��Ԫ��������Ȼ���� ������..
	int mi = (lo + hi) / 2;  //������Ϊ��
	mergeSort(lo, mi);  mergeSort(mi, hi);   //�ֱ�����
	merge(lo, mi, hi);    //�鲢
}
template<class T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)   //���������������[lo,mi],[mi,hi]
{
	T* A = _elem + lo;  //�ϲ��������A[0,hi-lo]  = _elem[lo,hi]
	int lb = mi - lo; T* B = new T[lb]; //ǰ������B[0,lb] = _elem[lo,mi]
	for (Rank i = 0; i < lb; B[i] = A[i++]);   //����ǰ������
	int lc = hi - mi; T* C = _elem + mi;  //��������[0,lc] = [mi,hi]
	for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)   //B[j]��C[k]�е�С������Aĩβ
	{
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
		if ((k < lc) && (!(j < lb) || (C[k] < b[j]))) A[i++] = C[k++];
	}
	delete [] B;
}//�鲢��õ���������������