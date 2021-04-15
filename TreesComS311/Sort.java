/**
 * @author Noah Chicchelly, Luke Stoll, Henry Kansanback
 */

import java.util.Comparator;

public class Sort implements Comparator<Triple> {

	@Override
	public int compare(Triple a, Triple b) {
		return (a.getTimestamp() - b.getTimestamp());
	}

	
	
}
