#include "kdtree.h"
#include <stdio.h>
#include <cstring>
#include <cmath>

#define SQ(x)			((x) * (x))

struct SKDTreeNode
{
	double pos[3];
	__int64 id;
	char dir;
	char remove;
	SKDTreeNode* left;
	SKDTreeNode* right;
	SKDTreeNode* up;
};

struct SKDTreeNodeList
{
	SKDTreeNode* node;
	double dist_sq;
	SKDTreeNodeList *next;
};

CKDResult::CKDResult()
{
	rlist = new SKDTreeNodeList();
	rlist->next = nullptr;
	riter = 0;
	size = 0;
}
CKDResult::~CKDResult()
{
	Destroy();
	delete rlist;
	rlist = 0;
}

void CKDResult::Destroy()
{
	SKDTreeNodeList *tmp, *node = rlist->next;

	while(node) 
	{
		tmp = node;
		node = node->next;
		delete tmp;
	}

	rlist->next = 0;
	size = 0;
	riter = 0;
}

/* inserts the item. if dist_sq is >= 0, then do an ordered insert */
bool CKDResult::Insert(SKDTreeNode* item, double dist_sq)
{
	SKDTreeNodeList* rnode = new SKDTreeNodeList();

	if(!rnode) 
		return false;

	rnode->node = item;
	rnode->dist_sq = dist_sq;

	SKDTreeNodeList* list = rlist;

	if(dist_sq >= 0.0) 
	{
		while(list->next && list->next->dist_sq < dist_sq) 
		{
			list = list->next;
		}
	}
	rnode->next = list->next;
	list->next = rnode;
	riter = rlist->next;
	size++;
	return true;
}

SKDTreeNode* CKDResult::First() 
{ 
	riter = rlist->next; 
	return riter ? riter->node : 0;
}

/* advances the result set iterator, returns non-zero on success, zero if
 * there are no more elements in the result set.
 */
SKDTreeNode* CKDResult::Next()
{
	riter = riter->next;
	return riter ? riter->node : 0;
}

__int64 CKDResult::Index( SKDTreeNode* node)
{
	return node ? node->id : -1;
}

//////////////////////////////////////////////////////////////////////////
//CKDTree

CKDTree::CKDTree( int dim )
{
	m_root = nullptr;
	m_iter = nullptr;
	m_min[0] = m_min[1] = m_min[2] = 0.0;
	m_max[0] = m_max[1] = m_max[2] = 0.0;
	m_dim = dim;
}

CKDTree::~CKDTree()
{
	Destroy();
}

void CKDTree::Create( int dim )
{
	Destroy();
	m_dim = dim;
}

void CKDTree::Destroy()
{
	m_min[0] = m_min[1] = m_min[2] = 0.0;
	m_max[0] = m_max[1] = m_max[2] = 0.0;
	SKDTreeNode* node = m_root;
	while( node )
	{
		if( node->left )
		{
			node = node->left;
			node->up->left = 0;
		}
		else if( node->right )
		{
			node = node->right;
			node->up->right = 0;
		}
		else
		{
			SKDTreeNode* tmp = node;
			node = node->up;
			delete tmp;
		}
	}
	m_root = NULL;
	m_iter = NULL;
}

int CKDTree::Insert(const double* pos, void *element_id)
{
	if(!m_root) 
	{
		memcpy( m_min, pos, m_dim * sizeof(double) );
		memcpy( m_max, pos, m_dim * sizeof(double) );
	} 
	int rc = InsertRec(&m_root, 0, pos, (__int64)element_id, 0);
	m_iter = m_root;
	return rc;
}

int CKDTree::Insert(const double* pos, int element_id)
{
	if(!m_root) 
	{
		memcpy( m_min, pos, m_dim * sizeof(double) );
		memcpy( m_max, pos, m_dim * sizeof(double) );
	} 
	int rc = InsertRec(&m_root, 0, pos, (__int64)element_id, 0); 
	m_iter = m_root;
	return rc;
}

bool CKDTree::InsertRec(SKDTreeNode **nptr, SKDTreeNode *parent, const double *pos, __int64 element_id, int dir)
{
	//выполняем спуск по дереву с определением нового направления
	while(*nptr)
	{
		parent = *nptr;
		dir = (parent->dir + 1) % m_dim;
		nptr = ( pos[parent->dir] < parent->pos[parent->dir]) ? &parent->left : &parent->right;
	}
	SKDTreeNode *node = new SKDTreeNode();
	if(!node) //не хватило памяти
		return false;

	memcpy(node->pos, pos, m_dim * sizeof(double));
	node->id = element_id;
	node->dir = dir;
	node->left = node->right = 0;
	node->up = parent;
	node->remove = 0;
	*nptr = node;

	if(m_min[0] > pos[0])
		m_min[0] = pos[0];
	else if( m_max[0] < pos[0] )
		m_max[0] = pos[0];

	if(m_min[1] > pos[1])
		m_min[1] = pos[1];
	else if( m_max[1] < pos[1] )
		m_max[1] = pos[1];

	if( m_dim > 2 )
	{
		if(m_min[2] > pos[2])
			m_min[2] = pos[2];
		else if( m_max[2] < pos[2] )
			m_max[2] = pos[2];
	}
	return true;
}

static void NearestHelper(int dim, SKDTreeNode* node, const double* pos, SKDTreeNode** result, double* result_dist_sq, double* min, double* max)
{
	int dir = node->dir;
	int i, side;
	double dummy, dist_sq;
	SKDTreeNode *nearer_subtree, *farther_subtree;
	double *nearer_hyperrect_coord, *farther_hyperrect_coord;

	/* Decide whether to go left or right in the tree */
	dummy = pos[dir] - node->pos[dir];
	if (dummy <= 0) 
	{
		nearer_subtree = node->left;
		farther_subtree = node->right;
		nearer_hyperrect_coord = max + dir;
		farther_hyperrect_coord = min + dir;
		side = 0;
	} 
	else 
	{
		nearer_subtree = node->right;
		farther_subtree = node->left;
		nearer_hyperrect_coord = min + dir;
		farther_hyperrect_coord = max + dir;
		side = 1;
	}

	if (nearer_subtree) 
	{
		/* Slice the hyperrect to get the hyperrect of the nearer subtree */
		dummy = *nearer_hyperrect_coord;
		*nearer_hyperrect_coord = node->pos[dir];
		/* Recurse down into nearer subtree */
		NearestHelper(dim, nearer_subtree, pos, result, result_dist_sq, min, max);
		/* Undo the slice */
		*nearer_hyperrect_coord = dummy;
	}

	/* Check the distance of the point at the current node, compare it
	 * with our best so far */
	dist_sq = 0;
	for(i=0; i < dim; i++) 
	{
		dist_sq += SQ(node->pos[i] - pos[i]);
	}
	if (dist_sq < *result_dist_sq) 
	{
		if( !node->remove )
		{
			*result = node;
			*result_dist_sq = dist_sq;
		}
	}

	if (farther_subtree) 
	{
		/* Get the hyperrect of the farther subtree */
		dummy = *farther_hyperrect_coord;
		*farther_hyperrect_coord = node->pos[dir];
		/* Check if we have to recurse down by calculating the closest
		 * point of the hyperrect and see if it's closer than our
		 * minimum distance in result_dist_sq. */
		int i;
		double dist_sq = 0;

		for (i=0; i < dim; i++) 
		{
			if (pos[i] < min[i]) 
			{
				dist_sq += SQ(min[i] - pos[i]);
			} 
			else if (pos[i] > max[i]) 
			{
				dist_sq += SQ(max[i] - pos[i]);
			}
		}

		if ( dist_sq < *result_dist_sq) 
		{
			/* Recurse down into farther subtree */
			NearestHelper( dim, farther_subtree, pos, result, result_dist_sq, min, max);
		}
		/* Undo the slice on the hyperrect */
		*farther_hyperrect_coord = dummy;
	}
}

static int FindNearestHelper(SKDTreeNode* node, const double* pos, double range, CKDResult& rset, int ordered, int dim)
{
	double dist_sq, dx;
	int i, ret, added_res = 0;

	if(!node) 
		return 0;

	dist_sq = 0;
	for(i=0; i<dim; i++) 
	{
		dist_sq += SQ(node->pos[i] - pos[i]);
	}

	if(dist_sq <= SQ(range) && !node->remove) 
	{
		if( !node->remove )
		{
			if(!rset.Insert( node, ordered ? dist_sq : -1.0)) 
			{
				return -1;
			}
		}
		added_res = 1;
	}

	dx = pos[node->dir] - node->pos[node->dir];

	ret = FindNearestHelper(dx <= 0.0 ? node->left : node->right, pos, range, rset, ordered, dim);
	if(ret >= 0 && std::fabs(dx) < range) 
	{
		added_res += ret;
		
		ret = FindNearestHelper(dx <= 0.0 ? node->right : node->left, pos, range, rset, ordered, dim);
	}
	if(ret == -1) 
	{
		return -1;
	}
	added_res += ret;

	return added_res;
}

SKDTreeNode* CKDTree::Nearest(const double *pos, double* dist /*= NULL*/)
{
	double box_min[3];
	double box_max[3];
	SKDTreeNode* result = 0;
	double dist_sq;
	if( !dist )
		dist = &dist_sq;
	*dist = 1e20;
	int i;

	if( !m_root )
		return 0;

	/* Duplicate the bounding hyperrectangle, we will work on the copy */
	memcpy(box_min, m_min, m_dim*sizeof(double));
	memcpy(box_max, m_max, m_dim*sizeof(double));

	/* Our first guesstimate is the root node */
	if( m_root->remove == 0 )
	{
		result = m_root;
		*dist = 0;
		for (i = 0; i < m_dim; i++)
			*dist += SQ(result->pos[i] - pos[i]);
	}

	/* Search for the nearest neighbour recursively */
	NearestHelper( m_dim, m_root, pos, &result, dist, box_min, box_max);
	m_iter = result;
	return result;
}

int CKDTree::NearestRange(const double *pos, double range, CKDResult& rset)
{
	FindNearestHelper(m_root, pos, range, rset, 1, m_dim);
	return  rset.Count(); 
}

bool CKDTree::Remove( SKDTreeNode*& node )
{
	node->remove = 1;
	return true;
}

bool CKDTree::Restore( SKDTreeNode*& node )
{
	node->remove = 0;
	return true;
}

SKDTreeNode* CKDTree::First()
{
	m_iter = m_root;
	return m_iter;
}

SKDTreeNode* CKDTree::Last()
{
	m_iter = m_root;
	return m_iter;
/*

	m_iter = m_root;
	if(!m_iter)
		return m_iter;
	while( m_iter->right )
		m_iter = m_iter->right;
	return m_iter;

*/
}

SKDTreeNode* CKDTree::Prev()
{
	if( !m_iter )
		return 0;
	if( m_iter->right )
	{
		m_iter = m_iter->right;
		return m_iter;
	}
	if( m_iter->left )
	{
		m_iter = m_iter->left;
		return m_iter;
	}
	while( m_iter )
	{
		if( m_iter->up && m_iter->up->left && (m_iter->up->left != m_iter) )
		{
			m_iter = m_iter->up->left;
			break;
		}
		m_iter = m_iter->up;
	}
	return m_iter;
}

SKDTreeNode* CKDTree::Next()
{
	if( !m_iter )
		return 0;
	if( m_iter->left )
	{
		m_iter = m_iter->left;
		return m_iter;
	}
	if( m_iter->right )
	{
		m_iter = m_iter->right;
		return m_iter;
	}
	while( m_iter )
	{
		if( m_iter->up && m_iter->up->right && (m_iter->up->right != m_iter) )
		{
			m_iter = m_iter->up->right;
			break;
		}
		m_iter = m_iter->up;
	}
	return m_iter;
}

__int64 CKDTree::Index(SKDTreeNode* node)
{
	return node ? node->id : -1;
}


//bool Intersect( int dim, const double* boxmin, const double* boxmax, 
//			   const double* eye, const double* dir, double tol, double* tmin, double* tmax );

