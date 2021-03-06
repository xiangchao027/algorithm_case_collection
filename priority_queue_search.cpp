#include "base.h"
#include <iostream>

using namespace std;

struct node
{
	int data;
	node *parent;
	int chn;	// number of children
	node *chd;	// children list
	int pos;	// current child position
};

typedef node* pnode;

void n_create(node *n)
{
	n->parent = NULL;
	n->chn = 0;
	n->chd = NULL;
	n->pos = -1;
}

void n_destroy(node *n)
{
	//delete[] (node*)(n->chd);
	delete (node*)n;
}

// double linked tree
struct tree
{
	node *root;	// root of tree
	int depth;	// depth of tree
	node *flag;	// pointer of current node
	int level;	// current search level
	int count;	// number of nodes in tree
};

// double linked queue
struct queue
{
	node *head;	// head of queue
	node *flag;	// current pointer
	node *tail;	// tail of queue
	int count;	// length of nodes in queue
};

// queue creation
void q_create(queue *q)
{
	q->head = new node;
	n_create(q->head);
	q->flag = q->tail = q->head;
	q->count = 0;
}

void q_destroy(queue *q)
{
	while(q->count>0)
	{
		q->flag = q->tail->parent;
		n_destroy(q->tail);
		q->tail = q->flag;
		q->count --;
	}
	// remove the head
	n_destroy(q->head);
	delete (queue*)q;
}

void t_create(tree *t)
{
	t->root = new node;
	n_create(t->root);
	t->depth = 0;
	t->flag = t->root;
	t->level = 0;
	t->count = 0;
}

// add new nodes to tree, at the position of where flag is.
// put node to be one of the son of what pointer flag referring to.
void t_add(tree *t, node *chd, int chn)
{
       	// set children's parent
	for(int i=0; i<chn; i++)
	        chd[i].parent = t->flag;
	// set father's children
	t->flag->chd = chd;
	t->flag->chn = chn;
	t->flag->pos ++;
	t->flag = t->flag->chd + t->flag->pos;
	t->depth ++;
	t->level ++;
	t->count += chn;
}

bool t_up(tree *t)
{
	if(t->level==0)
		return false;
	t->flag = t->flag->parent;
	t->level -= 1;
	return true;
}

bool t_down(tree *t)
{
        cout<<__LINE__<<endl;
	if(t->flag->pos+1==t->flag->chn) // include situation that satisfy [chn=0]
		return false;
	cout<<__LINE__<<endl;
	t->flag->pos += 1;
	t->flag = t->flag->chd + t->flag->pos;
	t->level += 1;
	cout<<__LINE__<<endl;
	return true;
}

// queue add
void q_add(queue *q, node *e)
{
	q->tail->chd = e;
	e->parent = q->tail;
	q->tail = e;
	q->count ++;
}

// queue pop
node* q_pop(queue *q)
{
	if(q->count<=0)
		return NULL;
	q->flag = q->head->chd;
	q->head->chd = q->head->chd->chd;
	q->head->chd->parent = q->head;
	q->count --;
	return q->flag;
}

void t_destroy(tree *t)
{
        node *f = t->root; // flag of current node
        int count = t->count;
        cout<<__LINE__<<endl;
        while(count>0)
        {
                if(f==NULL)
                        break;
		cout<<__LINE__<<endl;
                if(f->chn>0)
                        f = f->chd;
                else
                {
                        while(1)
                        {
				cout<<__LINE__<<endl;
                                f = f->parent; // roll up
                                delete[] (node*)f->chd; 
                                count -= f->chn; // update count
                                
                                // what if it is the last child ?
                                if(
									f==f->parent->chd+(
										f->parent->chn-1)
								)
                                        continue;
				else
				{
					cout<<__LINE__<<endl;
					// moves to next sibling
					f ++;
					break;
				}
                        }
                }
        }
	cout<<__LINE__<<endl;
        delete (node*)t->root;
	delete (tree*)t;
}

// define priority-queue tree search
// d : distance of each two point
// s : solution to return with
// n : dimension of s
int priority_queue_search(int *d, int *s, int n)
{
	// push a root node
	int cost = 0;
	tree *t = new tree;
	queue *q = new queue;
	cout<<__LINE__<<endl;
	t_create(t);
	q_create(q);
	cout<<__LINE__<<endl;
	// constructing search-tree
	// 's' is for mark use now
	for(int i=0; i<n; i++)
		s[i] = 0; // mark as not in route
	// then add them to tree
	node *chd = new node[n];
	for( int i=0; i<n; i++ )
		chd[i].data = i;
	t_add(t, chd, n);
	s[t->flag->data] = 1; // mark the current node as searched

	cout<<__LINE__<<endl;
	// unmarked candidates of the next level
	int *candidates = new int[n];
	while(1)
	{
		cout<<__LINE__<<endl;
		// leaf ?
		if(t->level==n)
		{
			cout<<__LINE__<<endl;
			bool outer_break =false;
			bool outer_continue = false;
			while(1)
			{
				cout<<__LINE__<<endl;
				// roll up
				s[t->flag->data] = 0; // unmark child
				if(!t_up(t))
				{
				        cout<<__LINE__<<endl;
					outer_break = true; // outer loop is over too
					break;
				}
				else
				{
				        cout<<__LINE__<<endl;
					// chase down
					if(!t_down(t))
                                                continue;
					else
					{
					        cout<<__LINE__<<endl;
						s[t->flag->data] = 1; // marked as searched
						outer_continue = true; // outer loop is TBC
						break;
					}
					cout<<__LINE__<<endl;
				}
			}
			cout<<__LINE__<<endl;
			if(outer_break)
				break;
			if(outer_continue)
				continue;
		}
		cout<<__LINE__<<endl;
		// break only if all sequences are listed out
		// on this state, flag is at top level, and all 
		// sequence elements are marked
		int unmarked = 0;
		for(int i=0; i<n; i++)
		{
			if(s[i]==0) // unmarked element found
			{
				candidates[unmarked] = i; // log its index
				unmarked ++; // increment of unmarked count
			}
		}
		// if no unmarked found: error occurs
		if(!unmarked)
		{
#ifdef _DEBUG_
			cout<<"Error at line: "<<__LINE__<<endl;
			cout<<"Program Error: logic contradiction detected!"<<endl;
#endif
			break;
		}
		cout<<__LINE__<<endl;
		// construct sub-tree
		node *chd = new node[unmarked];
		t_add(t, chd, unmarked); // naturally chase down
		s[t->flag->data] = 1; // mark as in route now
		cout<<__LINE__<<endl;
	} // tree construction done
	cout<<__LINE__<<endl;
	// second step is: search the tree with a priority queue

	// finalizing
	delete[] (int*)candidates;
        t_destroy(t);
        cout<<__LINE__<<endl;
        q_destroy(q);
	cout<<__LINE__<<endl;
	return cost;
}


int main()
{
	int dist[25];// 5 array of 5 elements
	// dialog line
	// distance is unlimited
	dist[0*5+0] = -1;
	dist[1*5+1] = -1;
	dist[2*5+2] = -1;
	dist[3*5+3] = -1;
	dist[4*5+4] = -1;

	// the first row
	dist[1] = 20;
	dist[2] = 30;
	dist[3] = 10;
	dist[4] = 11;
	
	// second row
	dist[7] = 16;
	dist[8] = 4;
	dist[9] = 2;
	
	// third row
	dist[13] = 6;
	dist[14] = 7;
	
	// the fourth row
	dist[19] = 12;

	// the search result
	int solution[5];
	// now search the route to get the shortest path of loop
	cout<<__LINE__<<endl;
	int result = priority_queue_search(dist, solution, 5);
	cout<<__LINE__<<endl;
	// print out answer
	cout<<"the shortest path is : ";
	for(int i=0; i<5; i++)
		cout<<solution[i]<<"-";
	cout<<"the shortest path distance : "<<result<<endl;
	cout<<"done"<<endl;

	return 0;
}
