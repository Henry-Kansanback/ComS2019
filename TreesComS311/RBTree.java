package cs311;
public class RBTree {
	
	public Node root;
	private Node nil = new Node();
	public RBTree() {
		nil.color = Node.BLACK;
		root = nil;
		root.p = 0;
		}
	
	public Node getRoot() {
		return root;
	}
	
	public Node getNILNode() {
		return nil;
	}
	
	public int getSize() {
		return getSizeHelper(root);
	}
	
	private int getSizeHelper(Node n) {
		if (n == nil) {
			return 0;
		}
		
		int l = getSizeHelper(n.getLeft());
		int r = getSizeHelper(n.getRight());
		
		return (l+r+1);
	}
	
	public int getHeight() {
		return getHeightHelper(root);
	}
	
	private int getHeightHelper(Node n) {
		if (n == nil) {
			return 0;
		}
		int l = getSizeHelper(n.getLeft());
		int r = getSizeHelper(n.getRight());
		
		int max = (l > r) ? l : r;
		
		return max + 1;
	}
	
}