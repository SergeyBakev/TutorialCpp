#ifndef __KDTREE_H_INCLUDED__
#define __KDTREE_H_INCLUDED__

class CKDTree;
class CKDResult;

struct SKDTreeNode;
struct SKDTreeNodeList;

class CKDResult
{
public:
	CKDResult();
	~CKDResult();
	void Destroy();
	bool Insert(SKDTreeNode* item, double dist_sq);
	int Count() { return size; };

	SKDTreeNode* First();
	/* advances the result set iterator, returns non-zero on success, zero if
	 * there are no more elements in the result set.
	 */
	SKDTreeNode* Next();
	static __int64 Index(SKDTreeNode*);
private:
//	CKDTree* m_tree;
	SKDTreeNodeList* rlist;
	SKDTreeNodeList* riter;
	int size;
};

class CKDTree
{
public:
	CKDTree( int dim = 2);
	~CKDTree();
	void Create( int dim );
	void Destroy();

	/* insert a node, specifying its position ( dim = 2|3), and element_id */
	int Insert(const double* pos, void *element_id);
	int Insert(const double* pos, int element_id);

	bool Remove( SKDTreeNode*& node);
	bool Restore( SKDTreeNode*& node);

	/* Find one of the nearest nodes from the specified point.
	 *
	 * This function returns a pointer to a result set with at most one element.
	 */
	SKDTreeNode* Nearest(const double *pos, double* dist = nullptr);

	/* Find any nearest nodes from the specified point within a range.
	 *
	 * This function returns a pointer to a result set, which can be manipulated
	 * by the kd_res_* functions.
	 * The returned pointer can be null as an indication of an error. Otherwise
	 * a valid result set is always returned which may contain 0 or more elements.
	 * The result set must be deallocated with kd_res_free, after use.
	 */
	int NearestRange(const double *pos, double range, CKDResult& rset);


	SKDTreeNode* First();
	SKDTreeNode* Last();
	SKDTreeNode* Prev();
	SKDTreeNode* Next();
	static __int64 Index(SKDTreeNode*);

private:
	bool InsertRec(SKDTreeNode **nptr, SKDTreeNode* parent, const double *pos, __int64 element_id, int dir);

private:
	int m_dim;
	double m_min[3];
	double m_max[3];
	SKDTreeNode* m_root;
	SKDTreeNode* m_iter;
};

#endif	/* __KDTREE_H_INCLUDED__ */
