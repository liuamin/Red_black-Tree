#ifndef  _ROTATE_H
#define  _ROTATE_H
/******************************当树不平衡或者不符条件时进行旋转************************************/
enum Color{ Red, Black };
//定义红黑树的结点
template<typename T>
struct RBTNode
{
	T  element;
	Color  RB;
	RBTNode<T>  *lchild, *rchild;
};
template <typename T>
class Rotate 
{
protected:
	void  R_Rotate(RBTNode<T>*   &p);                    //单旋，向右旋转，
	void  LR_Rotate(RBTNode<T>*  &p);                   //双旋，先向左再向右旋转

	void  L_Rotate(RBTNode<T>*   &p);                  //单旋，向左旋转
	void  RL_Rotate(RBTNode<T>*   &p);                 //双旋，先向右再向左旋转
};
template<typename T>
void Rotate<T>::R_Rotate(RBTNode<T>* &p)   //单旋，向右旋转，
{
	RBTNode<T>* L = p->lchild;
	p->lchild = L->rchild;
	L->rchild = p;
	p = L;                    //p指向旋转后新的（根）结点
}
template<typename T>
void Rotate<T>::LR_Rotate(RBTNode<T> * &p)                //双旋，先向左再向右旋转
{
	RBTNode<T>*  L = p->lchild;
	p->lchild = L->rchild->rchild;
	L->rchild->rchild = p;
	p = L->rchild;
	L->rchild = p->lchild;
	p->lchild = L;
}
template<typename T>
void Rotate<T>::L_Rotate(RBTNode<T>* &p)   //单旋，向左旋转
{
	RBTNode<T> *  R = p->rchild;
	p->rchild = R->lchild;
	R->lchild = p;
	p = R;                    //p指向旋转后新的（根）结点
}
template<typename T>
void Rotate<T>::RL_Rotate(RBTNode<T>* &p)                //双旋，先向右再向左旋转
{
	RBTNode<T>  * R = p->rchild;
	p->rchild = R->lchild->lchild;
	R->lchild->lchild = p;
	p = R->lchild;
	R->lchild = p->rchild;
	p->rchild = R;
}
#endif // _ROTATE_H
