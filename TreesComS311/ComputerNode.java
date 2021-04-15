/**
 * @author Noah Chicchelly, Luke Stoll, Henry Kansanback
 */


import java.util.List;

public class ComputerNode {
int ID;
int timestamp;
int reachable = 0;
//int c1;
//int c2;

@Override
public int hashCode() {
    return this.ID*31 + this.timestamp;
}

@Override
public boolean equals( Object obj) {
    if (this == obj)
        return true;
    if(!(obj instanceof ComputerNode)) {
    	return false;
    }
    ComputerNode c = (ComputerNode) obj;
    
    return (Double.compare(ID, c.ID) == 0) && (Double.compare(timestamp, c.timestamp) == 0);
}


/**
 * Returns the ID of the associated computer.
 *
 * @return Associated Computer's ID
 */
public int getID()
{
	
	return ID;
}


/**
 * Returns the timestamp associated with this node.
 *
 * @return Timestamp for the node
 */
public int getTimestamp()
{
	
	return timestamp;
}

/**
 * Returns a list of ComputerNode objects to which there is outgoing edge from this ComputerNode object.
 *
 * @return a list of ComputerNode objects that have an edge from this to the nodes in the list.
 */
public List<ComputerNode> getOutNeighbors()
{	
	return CommunicationsMonitor.list.get(this);
	
}

}
