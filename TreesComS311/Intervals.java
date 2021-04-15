package cs311;
public class Intervals {
	RBTree T;
	int globalCount = 0;
	public Intervals() {
		T = new RBTree();
	}
	
	public void intervalInsert(int a, int b) {
		Node n1 = new Node();
		Node n2 = new Node();
		
		n1.setKey(a);
		n1.id = generateId();
		n1.p = 1;
		n2.p = -1;
		n2.setKey(b);
		n2.id = generateId();
		
		
		insert(n1);
		insert(n2);
		
		updateMax(T.root, 0);
		updateEMax(T.root);
	}
	
	public boolean intervalDelete(int intervalID) {
		return false;
	}
	
	public int findPOM( ) {
		return T.root.emax.val;
	}
	
	public RBTree getRBTree() {
		return T;
	}
	
	public int generateId() {
		return globalCount++;
	}
	
	public void insert(Node z) {
		Node y = T.getNILNode();
		Node x = T.root;
		
		while (x != T.getNILNode()) {
			y=x;
			if (z.key.getValue()  < x.key.getValue()) {
				x = x.left;
			}
			else {
				x = x.right;
			}
			
		}
		z.parent = y;
		
		if (y == T.getNILNode()) {
			T.root = z;
		}
		else if (z.getKey() < y.getKey()) {
			y.left  = z;
		}
		else {
			y.right = z;
		}
		z.left = T.getNILNode();
		z.right = T.getNILNode();
		z.color = Node.RED;
		insertFix(z);	
	}
	
	public void delete(Node n) {
		
	}
	
	private int updateMax(Node v, int sum) {
		if(v == T.getNILNode())
		{
			v.maxval = 0;
			return 0;
		}
		else
		{
			int l = updateMax(v.left, sum);
			int maxhold = l > (l + v.p) ? l : (l + v.p);
			int r = updateMax(v.right, sum + l + v.p);
			v.maxval = maxhold > r ? maxhold : r;
			
			return sum + l + r + v.p;
		}
	}
	
	private Node updateEMax(Node v) {
		if(v == T.getNILNode()) {
			v.emax = v.key;
			return v;
		}
		Node l = updateEMax(v.left);
		Node r = updateEMax(v.left);
		
		Node max = l.maxval >= v.maxval ? l : v;
		max = max.maxval >= r.maxval ? max : r;
		v.emax = max.key;
		return max;
	}
	
	private void insertFix(Node z) {
		Node y;
		while (z.parent.color == Node.RED) {
			if (z.parent == z.parent.parent.left) {
				y = z.parent.parent.right;
				if (y.color == Node.RED) {
					z.parent.color = Node.BLACK;
					y.color = Node.BLACK;
					z.parent.parent.color = Node.RED;
					z = z.parent.parent;
				}
				else {
					if (z == z.parent.right) {
						z = z.parent;
						leftRotate(z);
					}
					z.parent.color = Node.BLACK;
					z.parent.parent.color = Node.RED;
					rightRotate(z.parent.parent);
				}
			}
			else {
				y = z.parent.parent.left;
				if (y.color == Node.RED) {
					z.parent.color = Node.BLACK;
					y.color = Node.BLACK;
					z.parent.parent.color = Node.RED;
					z = z.parent.parent;
				}
				else {
					if (z == z.parent.left) {
						z = z.parent;
						rightRotate(z);
					}
					z.parent.color = Node.BLACK;
					z.parent.parent.color = Node.RED;
					leftRotate(z.parent.parent);
				}
			}
		}
	}
	
	private void deleteFix() {
		
	}
	
	private void leftRotate(Node x) {
		Node y = x.getRight();
		x.right = y.getLeft();
		if (y.left != null) {
			y.left.parent = x;
		}
		y.parent = x.parent;
		if (x.parent == null) {
			T.root = y;
		}
		else if (x == x.parent.left) {
			x.parent.left = y;
		}
		else {
			x.parent.right = y;
		}
		y.left = x;
		x.parent = y;
	}
	
	private void rightRotate(Node x) {
		Node y = x.getLeft();
		x.left = y.getRight();
		if (y.right != null) {
			y.right.parent = x;
		}
		y.parent = x.parent;
		if (x.parent == null) {
			T.root = y;
		}
		else if (x == x.parent.right) {
			x.parent.right = y;
		}
		else {
			x.parent.left = y;
		}
		y.right = x;
		x.parent = y;
	}

}