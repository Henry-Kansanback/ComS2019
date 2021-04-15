package cs311;

public class Node {
	public static final int BLACK = 1;
	public static final int RED = 0;
	Node parent;
	Node left;
	Node right;
	Endpoint key  = new Endpoint();
	int p;
	int val;
	int color; //0 = red 1 = black
	int maxval;
	int id;
	Endpoint emax;
	
	public Node getParent() {
		return parent;
	}
	
	public Node getLeft() {
		return left;
	}
	
	public Node getRight() {
		return right;
	}
	
	public int getKey() {
		return key.getValue();
	}
	
	public int getP() {
		return p;
	}
	
	public int getVal() {
		return val;
	}
	
	public int getMaxVal() {
		return maxval;
	}
	
	public Endpoint getEndpoint() {
		return key;
	}
	
	public Endpoint getEmax() {
		return emax;
	}
	
	public int getColor() {
		return color;
	}
	
	public void setKey(int v) {
		key.val = v;
	}

	
}