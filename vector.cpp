#include<iostream>
#include"vector.h"
#include<cstdlib>
template<class T> //元素类型
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)    //以数组区间A[lo, hi]为蓝本复制向量
{
	_elem = new T[_capacity = 2 * (hi - lo)];   _size = 0;    //分配空间 ，规模清零
	while (lo < hi) //A[lo, hi]内的元素逐一
		_elem[_size++] = A[lo++];  //复制至_elem[0, hi -low]
}
template<class T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V)    //重载
{
	if (_elem)delete[]_elem; //释放原有内容
	copyFrom(V._elem, 0, V._size()); //整体复制
	return *this;     //返回当前对象的引用，以便链式赋值
}
template<class T>
void Vector<T>::expand()
{
	if (_size < _capacity)return;    //尚未满员，不必扩容
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;   //不低于最小容量
	T*oldElem = _elem; _elem = new T[2 * _capacity];          //容量加倍
	for (int i = 0; 0 < _size; i++)
		_elem[i] = oldElem[i];     //复制原向量内容
	delete[]oldElem;  //释放原空间
}
template<class T>
void Vector<T>::shrink()            //装填因子过小时压缩向量所占空间
{
	if (_capacity < DEFAULT_CAPACITY<<1)return;    //不至收缩到DEFAULT_CAPACITY以下
	if (_size << 2 > _capacity)return;     //以25%为界
	T* oldElem = _elem; _elem = new T[_capacity >>= 1]; //容量减半
	for (int i = 0; i < _size; i++) _elem[i] = oldElem[i];    //复制原向量内容
	delete[]oldElem;
}
template<class T>
T& Vector<T>::operator[](Rank r)const
{
	return _elem[r];     //assert:0<=r<_size
}
template<class T>
void permute(Vector<T>& V)      //随机置乱向量，使个元素概率出现于个位置
{
	for (int i = V.size(); i > 0; i--)    //自后向前
		swap(V._elem[i-1], V._elem[rand() % i]);    //V[i-1] 与V[0,i] 中某一随机元素交换
}
template<class T>
void Vector<T>::unsort(Rank lo, Rank hi)         //equality odds 随机置乱区间[lo,hi]
{
	T* V = _elem + lo;   //将子向量_elem[lo,hi]视作另一向量V[0,hi-lo]
	for (Rank i = hi - lo; i > 0; i--)   //自后向前
		swap(V[i - 1], V[rand() % i]);   //将V[i-1]与V[0,i]中某一元素随机交换
}
template<class T>
Rank Vector<T>::find(T const& e,Rank lo, Rank hi)const      //无序向量(只支持比对操作)的顺序查找：返回最后一个元素e的位置；失效时，返回lo-1
{
	while ((lo < hi--) && (e != _elem[hi]));
	return hi;
}
template<class T>
Rank Vector<T>::insert(Rank r, T const& e)  //将e插入到 秩为r
{
	expand();         //若有必要，扩容
	for (int i = _size; i > r; i--) _elem[i] = _elem[i - 1];
	_elem[r] = e; _size++;     //插入新元素并更新容量
	return r;  //返回秩
}
template<class T>
int Vector<T>::remove(Rank lo, Rank hi)     //删除区间[lo,hi]  ，直接把不删除的元素移动到删除区间
{
	if (lo == hi) return 0;    //出于效率考虑，单独处理退化情况，比如remove[0,0]
	while (hi < _size)_elem[lo++] = _elem[hi++];   //把[hi,size]的元素放到[lo++],顺次前移hi-lo
	_size = lo;   //更新规模，直接丢弃尾部[lo,size = hi]
	shrink();   //若有必要，则缩容
	return hi - lo;  //返回被删除元素的数目
}
template<class T>
T Vector<T>::remove(Rank r) //删除向量中秩为r的元素   0 <= r < size
{
	T e = _elem[r];  //备份被删除元素
	remove(r, r + 1);    //调用区间删除算法，等效于对区间[r,r+1]的删除
	return e;         //返回被删除元素
}
template<class T>
int Vector<T>::deduplicate()    //删除无序向量中重复元素(高效版)
{
	int oldsize = _size;    //record original size
	Rank i = 1;             //从_elem[1]开始
	while (i < _size)          //自前向后逐一考察个元素
		(find(_elem[i], 0, i)) ?   //在其前缀中寻找与之雷同者(至多一个)
		i++ : remove(i);    //若无雷同则继续考察其后继，否则删除雷同者
	return oldsize - _size;    //向量规模变化量，即被删除元素总数
}
template<class T>void Vector<T>::traverse(void (*visit)(T&))    //借助函数指针机制
{
	for (int i = 0; i < _size; i++)visit(_elem[i]);
}    //遍历向量
template<class T>template<class VST>  //元素类型操作器
void Vector<T>::traverse(VST& visit)   //借助函数对象机制
{for (int i = 0; i < _size; i++)visit(_elem[i]);}
template<class T>int Vector<T>::disordered()const         //返回向量元素中的逆序相邻元素对的总数
{
	int n = 0;      //计数器
	for (int i = 1; i < _size; i++)
		if (_elem[i - 1] > _elem[i]) n++;     //逆序则计数
	return n;   //向量有序当且仅当n=0
}
template <class T> int Vector<T>::uniquify()   //有序向量重复元素剔除法（低效版）
{
	Rank i = 0, j = 0;  //各对互异元素的秩
	while (++j < _size)
		if (_elem[i] != _elem[j])   //跳过雷同者
			_elem[++i] = _elem[j];  //发现不同元素时，向前移至紧邻与前者右侧
	_size = ++i; shrink();  //直接截除尾部多余元素
	return j - i;   //向量规模变化量，即被删除元素总数
}
template<class T>static Rank binSearch(T* A, T const& e, Rank lo, Rank hi)      //二分查找 版本A
{
	while (lo < hi)
	{
		Rank mi = (lo + hi) >> 1;   //以中心为轴点
		if (e < A[mi]) hi = mi;    //深入前半段[lo,mi] 继续查找
		else if (A[mi] < e) lo = mi + 1;
		else    return mi;     //在mi处命中
	}   //成功查找可以提前终止
	return -1;    //查找失败
}   //有多个元素命中时 ，不能保证返回秩最大者； 查找失败时，简单地返回-1，而不能指示失败的位置
template<class T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi)const    //assert:0<=lo<hi<=_size
{
	return(rand() % 2) ?  //按各50%的概率随机使用二分查找或Fibonacci查找
		binSearch(_elem, e, lo, hi) : fibSearch(_elem,e, lo, hi);
}
template<class T>
static Rank binSearch(T* A, T const& e, Rank lo, Rank hi)        //二分查找 ，版本B
{
	while (1 < hi - lo)   //每步迭代仅需做一次比较判断 ，有两个分支,成功查找不能提前终止
	{
		Rank mi = (lo + hi) / 2;
		(e < A[mi]) ? hi = mi : lo = mi;      //经比较后确定深入[lo.mi]or [mi,hi]
	}  //出口时hi = lo+1 ,查找区间仅含一个元素A[lo]
	return(e == A[lo]) ? lo; -1;  //查找成功时返回对应的秩，否则返回-1
}   //有多个元素命中时。不能保证返回秩最大者;查找失败时，简单的返回-1，而不是指示失败的位置
template<class T>
static Rank binSearch(T* A, T const& e, Rank lo, Rank hi)    //二分查找 版本C
{
	while (lo < hi)    //每步迭代仅需做一次判断，有两个分支
	{
		Rank mi = (lo + hi) / 2;
		(e < A[mi]) ? hi = mi:lo = mi+1
	}
	return --lo;   //循环结束时，lo为大于e的元素的最小秩，故lo-1即不大于e的元素的最大秩
}       //有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置
template<class T>
void Vector<T>::sort(Rank lo, Rank hi)
{
	switch (rand() % 5)   //随机选取排序算法，可根据具体问题的特点灵活选取或扩充
	{
	case 1:bubbleSort(lo, hi); break;   //气泡排序
	case 2:selectionSort(lo, hi); break;  //选择排序
	case 3:mergeSort(lo, hi); break;    //归并排序
	case 4:heapSort(lo, hi);  break;   //堆排序
	default:quickSort(lo, hi); break;   //快速排序
	}
}
template<class T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
	while (!bubble(lo, hi--));   //逐趟做扫描交换，直至全序
}
template<class T>
bool Vector<T>::bubble(Rank lo, Rank hi)
{
	bool sorted = true;    //整体有序标志
	while (++lo < hi)          
	{
		if (_elem[lo - 1] > _elem[lo])   //若逆序则
			sorted = false;
		swap(_elem[lo - 1], _elem[lo]);   //通过交换使局部有序
	}
	return sorted; //返回有序标志
}
template<class T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
	if (hi - lo < 2)return;   //单元素区间自然有序 ，否则..
	int mi = (lo + hi) / 2;  //以中心为界
	mergeSort(lo, mi);  mergeSort(mi, hi);   //分别排序
	merge(lo, mi, hi);    //归并
}
template<class T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)   //各自有序的子向量[lo,mi],[mi,hi]
{
	T* A = _elem + lo;  //合并后的向量A[0,hi-lo]  = _elem[lo,hi]
	int lb = mi - lo; T* B = new T[lb]; //前子向量B[0,lb] = _elem[lo,mi]
	for (Rank i = 0; i < lb; B[i] = A[i++]);   //复制前子向量
	int lc = hi - mi; T* C = _elem + mi;  //后子向量[0,lc] = [mi,hi]
	for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc);)   //B[j]和C[k]中的小者续至A末尾
	{
		if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
		if ((k < lc) && (!(j < lb) || (C[k] < b[j]))) A[i++] = C[k++];
	}
	delete [] B;
}//归并后得到完整的有序向量