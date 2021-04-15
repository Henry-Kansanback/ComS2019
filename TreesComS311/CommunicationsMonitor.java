/**
 * @author Noah Chicchelly, Luke Stoll, Henry Kansanback
 */


import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;


public class CommunicationsMonitor {
	int created = 0;
	List<ComputerNode> nodes;
	List<Triple> triples;
	HashMap<Integer, List<ComputerNode>> map;
	static HashMap<ComputerNode, List<ComputerNode>> list;
	
	

	public CommunicationsMonitor() {
		triples = new ArrayList<Triple>();
        nodes = new ArrayList<ComputerNode>();
	}

	/**
	 * Takes as input two integers c1, c2, and a timestamp. This triple represents
	 * the fact that the computers with IDs c1 and c2 have communicated at the given
	 * timestamp. This method should run in O(1) time. Any invocation of this method
	 * after createGraph() is called will be ignored.
	 *
	 * @param c1        First ComputerNode in the communication pair.
	 * @param c2        Second ComputerNode in the communication pair.
	 * @param timestamp Time the communication took place.
	 */
	public void addCommunication(int c1, int c2, int timestamp) {

		if (created != 1) {

//			// Need extra check if these nodes (triple) already exists.
//			ComputerNode temp1 = new ComputerNode();
//			ComputerNode temp2 = new ComputerNode();
//			temp1.ID = c1;
//			temp2.ID = c2;
//			temp1.timestamp = timestamp;
//			temp2.timestamp = timestamp;
//
//			// ComputerNode temp = new ComputerNode();
//			// temp.c1 = c1;
//			// temp.c2 = c2;
//			// temp.timestamp = timestamp;
//			nodes.add(temp1);
//			nodes.add(temp2);

			// Create triple
			Triple temp = new Triple(c1, c2, timestamp);
			triples.add(temp);

		}
	}

	public void createGraph() {
		created = 1;
		ComputerNode check1 = new ComputerNode();
		ComputerNode check2 = new ComputerNode();
		int i = 0;

		//Sort triples by nondecreasing timestamp (m log m time)
		Collections.sort(triples, new Sort());

		//Rest is O(n) ==> O(n+ m log m)
		
		//Key = C1,C2, etc -- Value = List of ComputerNodes
		map = new HashMap<>();
		//Key = ComputerNode from map -- Value = List of ComputerNodes that they've touched
		list = new HashMap<>();

		//For each triple..
		while (i < triples.size()) {
			check1.ID = triples.get(i).getC1();
			check1.timestamp = triples.get(i).getTimestamp();
			check2.ID = triples.get(i).getC2();
			check2.timestamp = triples.get(i).getTimestamp();
			
			//Create nodes if they do not already exist
			if(!nodes.contains(check1)){
				nodes.add(check1);
			}
			
			if(!nodes.contains(check2)) {
				nodes.add(check2);
			}
			
			//Add directed edge from c1,t to c2,t and c2,t to c1,t
			if(!list.containsKey(check1)) {
				list.put(check1, new ArrayList<ComputerNode>());
				list.get(check1).add(check2);	
			}else {
				list.get(check1).add(check2);	
			}
			if(!list.containsKey(check2)) {
				list.put(check2, new ArrayList<ComputerNode>());
				list.get(check2).add(check1);		
			}else {
				list.get(check2).add(check1);
			}
			
			//Append reference to c1,t / c2,t from map. (referring to the key of list)
			if(!map.containsKey(check1.getID())){
				map.put(check1.getID(), new ArrayList<ComputerNode>());
				map.get(check1.getID()).add(check1);
			}else {
				if(!map.get(check1.getID()).contains(check1))
					map.get(check1.getID()).add(check1);
			}
			if(!map.containsKey(check2.getID())){
				map.put(check2.getID(), new ArrayList<ComputerNode>());
				map.get(check2.getID()).add(check2);
			}else {
				if(!map.get(check2.getID()).contains(check2))
					map.get(check2.getID()).add(check2);
			}
			
			//If C1,C2,t is not first triple involving c1
			if(!map.get(check1.getID()).contains(check1)) {
				int size = map.get(check1.getID()).size() - 1;
				list.get(map.get(check1.getID()).get(size)).add(check1);
				map.get(check1.getID()).add(check1);
			}
			
			//Same thing for c2
			if(!map.get(check2.getID()).contains(check2)) {
				int size = map.get(check2.getID()).size() - 1;
				list.get(map.get(check2.getID()).get(size)).add(check2);
				map.get(check2.getID()).add(check2);
			}	
			
			check1 = new ComputerNode();
			check2= new ComputerNode();
			i++;
		}
	}

	/**
	 * Determines whether computer c2 could be infected by time y if computer c1 was
	 * infected at time x. If so, the method returns an ordered list of ComputerNode
	 * objects that represents the transmission sequence. This sequence is a path in
	 * graph G. The first ComputerNode object on the path will correspond to c1.
	 * Similarly, the last ComputerNode object on the path will correspond to c2. If
	 * c2 cannot be infected, return null.
	 * <p>
	 * Example 3. In Example 1, an infection path would be (C1, 4), (C2, 4), (C2,
	 * 8), (C4, 8), (C3, 8)
	 * <p>
	 * This method can assume that it will be called only after createGraph() and
	 * that x <= y. This method must run in O(m) time. This method can also be
	 * called multiple times with different inputs once the graph is constructed
	 * (i.e., once createGraph() has been invoked).
	 *
	 * @param c1 ComputerNode object to represent the Computer that is
	 *           hypothetically infected at time x.
	 * @param c2 ComputerNode object to represent the Computer to be tested for
	 *           possible infection if c1 was infected.
	 * @param x  Time c1 was hypothetically infected.
	 * @param y  Time c2 is being tested for being infected.
	 * @return List of the path in the graph (infection path) if one exists, null
	 *         otherwise.
	 */
		int fnd = 0;
	public List<ComputerNode> queryInfection(int c1, int c2, int x, int y) {
			fnd = 0;

		//Rough Draft.
		
		Stack<ComputerNode> stk = new Stack<>();
		ComputerNode n = new ComputerNode();
		stk.push(map.get(c1).get(0));
		int i =0;
		List<ComputerNode> lns = new ArrayList<ComputerNode>();
		List<ComputerNode> tmp = new ArrayList<ComputerNode>();
		while(i < map.size())
		{

			n = stk.peek();
			
			//lns.add(n);
			tmp = n.getOutNeighbors();
			if((map.get(n.getID()).size() <= i) && map.get(n.getID()).get(i).reachable == 0)
			{
				map.get(n.getID()).get(i).reachable = 1;
			}
			
			Iterator<ComputerNode> iterating = map.get(n.getID()).iterator();
			int j = 0;
			while(iterating.hasNext() && j < map.size())
			{
				ComputerNode next = iterating.next();
				if(map.get(next.getID()).get(j).reachable != 1)
				{
					stk.push(next);
					//lns.add(next);
					//lns.add(tmp.get(j));
					if(map.get(next.getID()).get(j).getID() == c2 && map.get(next.getID()).get(j).getTimestamp() <= y)
					{
						fnd = 1;
						break;
					}
				}
				j++;
			}
			if(fnd == 1)
			{
				break;
			}

			i++;
		}
		while(stk.empty() == false)
			lns.add(stk.pop());
		
		return lns;
	}

/*	private List<ComputerNode> DFS(int c1, int c2,int y)
	{
		List<ComputerNode> check = getComputerMapping(c1);
		List<ComputerNode> checking = new ArrayList<ComputerNode>();

		//List<ComputerNode> fin = new ArrayList<ComputerNode>();
		for(int i = 0; i < check.size(); i++)
		{
			
			checking = check.get(i).getOutNeighbors();
			
			for(int j = 0; j < checking.size(); j++)
			{
				if(checking.get(j).reachable == 0)
				{
					DFSV(checking, y, c2);
					//fin = DFSV(checking, c2, fnd);
				}
			}
		}
		//return fin;
		return checking;
	}
	
	private List<ComputerNode> DFSV(List<ComputerNode> items, int y, int c2)
	{
		items.get(0).reachable = 1;
		map.get(items.get(0).getID()).get(0).reachable = 1;
		List<ComputerNode> outnode = new ArrayList<ComputerNode>();
		for(int j = 0; j < items.size(); j++)
		{
			if(items.get(j).reachable == 0)
			{
				outnode = items.get(j).getOutNeighbors();
				if((items.get(j).getID() == c2) && (y >= items.get(j).getTimestamp()))
				{
					fnd = 1;
				}
				DFSV(outnode, y,  c2);
			}
		}
		items.get(0).reachable = 2;
		map.get(items.get(0).getID()).get(0).reachable = 2;
		return items;
	}*/

	/**
	* Returns a HashMap that represents the mapping between an Integer and a list of ComputerNode objects. The Integer
	* represents the ID of some computer Ci, while the list consists of pairs (Ci, t1),(Ci, t2),..., (Ci, tk),
	* represented by ComputerNode objects, that specify that Ci has communicated with other computers at times
	* t1, t2,...,tk. The list for each computer must be ordered by time; i.e., t1\<t2\<...\<tk.
	*
	*
	* Refer to post @418
	*
	* @return HashMap representing the mapping of an Integer and ComputerNode objects.
	*/
	public HashMap<Integer, List<ComputerNode>> getComputerMapping()
	{
		return map;
	}


	/**
	* Returns the list of ComputerNode objects associated with computer c by performing a lookup in the mapping.
	*
	* Refer to post @513
	*
	* @param c ID of computer
	* @return ComputerNode objects associated with c.
	*/
	public List<ComputerNode> getComputerMapping(int c)
	{
		List<ComputerNode> compMap = new ArrayList<ComputerNode>();
		//ComputerNode check1 = new ComputerNode();
		
		//check1.ID = triples.get(c).getC1();
		//check1.timestamp = triples.get(c).getTimestamp();
		
		//compMap.addAll(list.get(check1));
		
		compMap = map.get(c);
		return compMap;
	}

}
