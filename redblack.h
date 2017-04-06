#ifndef   _RBTREE_H
#define  _RBTREE_H
/***************************建立一颗红黑树，与二叉搜索树类似，只是当树的条件不满足红黑树要求时，进行调整*****************************/
#include "rotate.h"

template<typename T>
void visitRB(RBTNode<T>* &node)
{
	cout << "(" << node->element << " , ";
	if (node->RB == Red)
		cout << "Red)  ";
	else
		cout << "Black)  ";
}
template<typename T>
class RBTree :public Rotate<T>
{
private:
	RBTNode<T>* root;
	int Treesize;

	void Delete(RBTNode<T>* &Tree); //具体删除方法
	//处理出现双红结点（插入时）
	void  DoubleRed(RBTNode<T>*  &s, RBTNode<T>*  &p);
	//处理出现双黑结点(删除时)
	void  DoubleBlack(RBTNode<T>*  &pa, bool lr);
	//寻找父节点
	RBTNode<T> * Parent(RBTNode<T>*  s) const;
	//返回它的叔叔结点
	RBTNode<T> *  Uncle(RBTNode<T>*  s) const;
	//返回它的祖父结点
	RBTNode<T> *  Grandfather(RBTNode<T>*  s) const;
public:
	RBTree(){ root = NULL; Treesize = 0; }

	int  size() const{ return Treesize; }
	RBTNode<T> * & Root(){ return root; }
	//递归遍历方法
	void preOrder(RBTNode <T>* &node, void(*visit)(RBTNode<T>* &));
	void inOrder(RBTNode <T>* &node, void(*visit)(RBTNode<T>* &));
	void postOrder(RBTNode <T>* &node, void(*visit)(RBTNode<T>* &));

	bool SearchBST(RBTNode<T>* &Tree, T key, RBTNode<T>* f, RBTNode<T>* &p);

	//在红黑树中插入元素e,与二叉搜索树类似，只是当不符合红黑树特性时，及时调整。
	bool Insert(T e); 

	//从红黑树中删除key,并重接它的左子树和右子树
	bool DeleteRBT(RBTNode<T>* &Tree, T key);  //调用Delete函数
	
};
/*************************************递归前序，中序，后序遍历********************************/
template<typename T>
void RBTree<T>::preOrder(RBTNode <T>* &node,void(*visit)(RBTNode<T>* &))
{
	if (node != NULL)
	{
		visit(node);
		preOrder(node->lchild, visit);
		preOrder(node->rchild, visit);
	}
}
template<typename T>
void RBTree<T>::inOrder(RBTNode<T>* &node, void(*visit)(RBTNode<T>* &))
{
	if (node != NULL)
	{
		inOrder(node->lchild, visit);
		visit(node);
		inOrder(node->rchild, visit);
	}
}
template<typename T>
void RBTree<T>::postOrder(RBTNode <T>* &node,void(*visit)(RBTNode<T>* &))
{
	if (node != NULL)
	{
		postOrder(node->lchild, visit);
		postOrder(node->rchild, visit);
		visit(node);
	}
}
/*****************************获得结点的父节点，叔叔结点，祖父节点*********************************************/
template<typename T>
RBTNode<T> *  RBTree<T>::Parent(RBTNode<T>*  s) const
{
	if (root == s)  //当为根结点时父节点为NULL
		return NULL;
	RBTNode<T> * q = root;
	while (q != NULL)
	{
		if (q->element>s->element)                     //在左子树寻找
			if (q->lchild == s)
				return q;
			else
				q = q->lchild;
		else                                                                //在右子树寻找
			if (q->rchild == s)
				return q;
			else
				q = q->rchild;
	}
	return NULL;
}
template<typename T>
RBTNode<T> *  RBTree<T>::Uncle(RBTNode<T>*  s) const
{
	RBTNode<T> * g, *p = Parent(s);
	if (p != NULL)
	{
		g = Parent(p);
		if (g != NULL)
		{
			if (g->lchild == p)
				return g->rchild;
			else
				return g->lchild;
		}
	}
	return NULL;
}
template<typename T>
RBTNode<T> *  RBTree<T>::Grandfather(RBTNode<T>*  s) const
{
	RBTNode<T> * p = Parent(s);
	if (p != NULL)
		p = Parent(p);
	return p;
}
/************************************查找函数，与搜索二叉树中类似*******************************************/
template<typename T>
bool  RBTree<T>::SearchBST(RBTNode<T>* &Tree, T key, RBTNode<T>* f, RBTNode<T>* &p)
{
	if (!Tree)
	{
		p = f;
		return false;
	}
	else if (key == Tree->element)
	{
		p = Tree;
		return true;
	}
	else if (key < Tree->element)
		return SearchBST(Tree->lchild, key, Tree, p);
	else
		return SearchBST(Tree->rchild, key, Tree, p);
}
/******************在红黑树中插入元素e, 与二叉搜索树类似，只是当不符合红黑树特性时，及时调整，处理双红的情况*************************/
template<typename T>
bool  RBTree<T>::Insert(T e)
{
	RBTNode<T> *p, *s;
	if (!SearchBST(root, e, NULL, p))
	{
		s = new RBTNode<T>;
		s->element = e;
		s->lchild = s->rchild = NULL;
		if (p == NULL)   //当树为空时，插入的结点e为根结点并设置颜色为黑
		{
			root = s;
			root->RB = Black;
		}
		else        //当树不为空时
		{
			if (e<p->element)
				p->lchild = s;
			else
				p->rchild = s;
			s->RB = Red;     //插入的结点颜色设置为红色，因为如果是黑色肯定不符合红黑树的性质
			if (p->RB == Red)   //插入结点的父节点为黑结点，则符合红黑树的性质，不用修改，否则出现双红问题，修改。
				DoubleRed(s, p);
		}
		Treesize++;
		return true;
	}
	return false;
}
/************************从红黑树中删除key, 并重接它的左子树和右子树,调用函数Delete（）************************************/
template<typename T>
bool  RBTree<T>::DeleteRBT(RBTNode<T>* &Tree, T key)
{
	if (Tree == NULL)
		return false;
	else
	{
		if (key == Tree->element)
		{
			Delete(Tree);   //调用函数void Delete(BiTnode<T> * &Tree)
			return true;
		}
		else if (key < Tree->element)
			return DeleteRBT(Tree->lchild, key);
		else
			return DeleteRBT(Tree->rchild, key);

	}
}
template<typename T>
void RBTree<T>::Delete(RBTNode<T>* &Tree)
{
	RBTNode<T>* pa, *s, *p = Tree;
	bool lr, deleflag = false;//为双黑调整的标志位
	bool rootflag = (Tree == root); //当Tree指向根结点时为标志为1
	if (Tree->rchild == NULL || Tree->lchild == NULL)   //当删除结点只有左子树或者右子树时
	{
		if (Tree->RB == Black &&Tree != root) //当待删除的结点为黑色的结点，要进行调整
		{
			deleflag = true;
			pa = Parent(Tree);
			if (Tree->element < pa->element) //当删除的是pa的左孩子，兄结点为右
				lr = true;
			else                            //当删除为pa的右孩子，兄结点为左
				lr = false;
		}
		if (Tree->rchild == NULL)
			Tree = Tree->lchild;
		else
			Tree = Tree->rchild;
		if (rootflag&&Tree != NULL)//当待删除的是根结点，且删除后树不为空时设置新的根结点
			Tree->RB = Black;
		delete p;
	}
	else                    //当删除结点既有左子树又有右子树，此处是找待删除结点的直接前驱
	{
		s = Tree->lchild;
		while (s->rchild != NULL)   //s指向直接前驱
		{
			p = s;
			s = s->rchild;
		}
		if (s->RB == Black)  //当实际删除的结点为黑色时，需要调整
		{
			deleflag = true;
			pa = Parent(s);
			if (s->element < pa->element) //当删除结点为pa的左孩子，则兄结点为右孩子
				lr = true;
			else                         //当删除结点为pa的右孩子，则兄结点为左孩子
				lr = false;
		}
		Tree->element = s->element;
		if (Tree == p)                 //待删除结点的左孩子p没有右子树
			p->lchild = s->lchild;
		else
			p->rchild = s->lchild;
		delete s;
	}
	if (deleflag) //处理双黑结点
		DoubleBlack(pa, lr);
	Treesize--;
}
/*****************************插入处理双红情况***************************************/
template<typename T>
void  RBTree<T>::DoubleRed(RBTNode<T>*  &s, RBTNode<T>*  &p)//此处p为s的父节点
{
	RBTNode<T> * u=Uncle(s);
	RBTNode<T> * g= Grandfather(s);
	RBTNode<T> * gp= Parent(g);   //gp为曾祖父*gp;
	int flag;
	if (g == root)
		flag = 0;
	else
	{
		if (g->element<gp->element)
			flag = 1;    //g为gp的左孩子
		else
			flag = 2;
	}
	if (u == NULL || u->RB == Black)  //叔叔结点为黑或为空时
	{
		if (g->element>p->element)   //父节点为祖父的左结点
		{
			if (p->element>s->element)   //当前结点为父节点的左结点
				R_Rotate(g);
			else
				LR_Rotate(g);
		}
		else   //父节点为祖父的右结点
		{
			if (p->element<s->element)
				L_Rotate(g);
			else
				RL_Rotate(g);
		}
		g->RB = Black;
		g->lchild->RB = g->rchild->RB = Red;
		switch(flag)
		{
         case 0:root = g; break;
		 case 1:gp->lchild = g; break;
		 case 2:gp->rchild = g; break;
		}
	}
	else                                          //叔叔结点为红色
	{
		p->RB = u->RB = Black;
		if (flag>0)                           //祖父结点不为根结点
		{
			g->RB = Red;
			u = Parent(g);
			if (u->RB == Red)
				DoubleRed(g, u);
		}
	}
}
/***********************************删除处理双黑情况***************************************************/
template<typename T>
void RBTree<T>::DoubleBlack(RBTNode<T>*  &pa, bool lr)  //pa的左左子树或者右子树深度减一，所以进行调整
{
	RBTNode<T> *g = Parent(pa);
	int flag;
	if (pa == root) 
		flag = 0;    //当删除结点的父节点pa为根结点
	else           
	{
		if (pa->element < g->element)
			flag = 1;  //当删除结点的父节点pa为祖父的左结点
		else
			flag = 2;  //当删除结点的父节点pa为祖父的右结点
	}
	if (lr)            //当删除结点为pa的左孩子，则兄结点为右孩子，左黑高减一
	{
		//删除重接后不是双黑的情况
		if (pa->lchild != NULL&&pa->lchild->RB ==Red)  
			pa->lchild->RB = Black;

		//删除重接后是双黑的情况调整，分四种情况（算法导论）
		else
		{
			//情况1：当兄结点为红色，向左旋
			if (pa->rchild != NULL&&pa->rchild->RB == Red)
			{
				pa->RB = Red;
				pa->rchild->RB = Black;
				L_Rotate(pa);//向左旋，并且指向新的pa
				switch (flag)
				{
				    case 0: root = pa; break;       //重新设置新的根结点
					case 1: g->lchild = pa; break;  //重接左子树
					case 2: g->rchild = pa; break;  //重接右子树
				}
				DoubleBlack(pa->lchild, true);//旋转后左右黑高不变，所以要进行递归处理
			}
			//当兄结点为黑色
			else if (pa->rchild != NULL&&pa->rchild->RB == Black)
			{
				//此处三种情况所说的黑结点包括为空的情况，为了条件简单，所以先列出了红色结点的情况，剩下就为黑或空的情况

				//情况4：当兄结点为黑色，且兄结点的右孩子为红色
				if (pa->rchild->rchild != NULL&&pa->rchild->rchild->RB == Red)
				{
					pa->rchild->RB = pa->RB;    //令右孩子的颜色与pa同色
					pa->RB = pa->rchild->rchild->RB = Black;   //修改pa为黑色，同时兄结点的右孩子为黑色
					L_Rotate(pa); //向左旋转
					switch (flag)
					{
					case 0: root = pa; break;      
					case 1: g->lchild = pa; break;  
					case 2: g->rchild = pa; break; 
					}

				}
				//情况3：当兄结点为黑色，且兄结点的左孩子为红色，此时兄结点的右孩子为黑色或者为空（非红色）
				else if (pa->rchild->lchild != NULL&&pa->rchild->lchild->RB == Red)
				{
					pa->rchild->lchild->RB = pa->RB;//因为要双旋，所以令兄结点的左孩子与pa颜色一样
					pa->RB = pa->rchild->RB = Black;
					RL_Rotate(pa); //双旋，先右后左
					switch (flag)
					{
					 case 0: root = pa; break;
					 case 1: g->lchild = pa; break;
					 case 2: g->rchild = pa; break;
					}
				}
				//情况2：当兄结点为黑色，且兄结点的左右孩子都为黑色或空（非红色）
				else
				{
					pa->rchild->RB = Red;
					if (pa->RB == Red)
						pa->RB = Black;
					else  ////当pa颜色为黑，向上递归
					{
						switch (flag)
						{
						case 1:DoubleBlack(g, true); break;
						case 2:DoubleBlack(g, false); break;
						}
					}
				}
			}
		}
	}
	else                //当删除结点为pa的右孩子，则兄结点为左孩子
	{
		//删除重接后不是双黑的情况
		if (pa->rchild != NULL&&pa->rchild->RB == Red)
			pa->rchild->RB = Black;

		//删除重接后是双黑的情况调整，分四种情况（算法导论）
		else
		{
			//情况1：当兄结点为红色，向右旋
			if (pa->lchild != NULL&&pa->lchild->RB == Red)
			{
				pa->RB = Red;
				pa->lchild->RB = Black;
				R_Rotate(pa);//向右旋，并且指向新的pa
				switch (flag)
				{
				case 0: root = pa; break;       //重新设置新的根结点
				case 1: g->lchild = pa; break;  //重接左子树
				case 2: g->rchild = pa; break;  //重接右子树
				}
				DoubleBlack(pa->rchild, true);//旋转后左右黑高不变，所以要进行递归处理
			}
			//当兄结点为黑色
			else if (pa->lchild != NULL&&pa->lchild->RB == Black)
			{
				//此处三种情况所说的黑结点包括为空的情况，为了条件简单，所以先列出了红色结点的情况，剩下就为黑或空的情况

				//情况4：对称情况（当兄结点为黑色，且兄结点的左孩子为红色）
				if (pa->lchild->lchild != NULL&&pa->lchild->lchild->RB == Red)
				{
					pa->lchild->RB = pa->RB;    //令左孩子的颜色与pa同色
					pa->RB = pa->lchild->lchild->RB = Black;   //修改pa为黑色，同时兄结点的左孩子为黑色
					R_Rotate(pa); //向右旋转
					switch (flag)
					{
					case 0: root = pa; break;
					case 1: g->lchild = pa; break;
					case 2: g->rchild = pa; break;
					}
				}
				//情况3：当兄结点为黑色，且兄结点的右孩子为红色，此时兄结点的左孩子为黑色或者为空（非红色）
				else if (pa->lchild->rchild != NULL&&pa->lchild->rchild->RB == Red)
				{
					pa->lchild->rchild->RB = pa->RB;//因为要双旋，所以令兄结点的右孩子与pa颜色一样
					pa->RB = pa->lchild->RB = Black;
					LR_Rotate(pa); //双旋，先左后右
					switch (flag)
					{
					case 0: root = pa; break;
					case 1: g->lchild = pa; break;
					case 2: g->rchild = pa; break;
					}
				}
				//情况2：当兄结点为黑色，且兄结点的左右孩子都为黑色或空（非红色）
				else
				{
					pa->lchild->RB = Red;
					if (pa->RB == Red)
						pa->RB = Black;
					else  //当pa颜色为黑，向上递归
					{
						switch (flag)
						{
						case 1:DoubleBlack(g, true); break;
						case 2:DoubleBlack(g, false); break;
						}
					}
				}
			}
		}
	}
}
#endif // _RBTREE_H
