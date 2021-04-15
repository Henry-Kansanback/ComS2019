/**
 * @author Noah Chicchelly, Luke Stoll, Henry Kansanback
 */

public class Triple {

	int c1;
	int c2;
	int timestamp;
	
	public Triple(int c1, int c2, int timestamp) {
		this.c1 = c1;
		this.c2 = c2;
		this.timestamp = timestamp;		
	}
	
	public int getTimestamp() {
		return timestamp;
	}
	
	public int getC1() {
		return c1;
	}
	
	public int getC2() {
		return c2;
	}
}
