#include <bits/stdc++.h>
#include "Points_and_Lines.hpp"
#include "AVLTree.hpp"
#include "Event.hpp"
using namespace std;

#define UPPER_HORIZONTAL_P 0
#define LOWER_HORIZONTAL_P 1
#define UPPER_END_P 2
#define LOWER_END_P 3
#define INTERSECT_P 4
#define INTERSECTION 100000
#define eps 0.00000000000002
long double Segment::sweepLine = 1000000000;
class Bentley
{
	public :
	vector <pair<long double,long double>> output_intersections;/**< the set of intersections to be reported as output*/
	vector<Segment> reinsert;/**< the segments which need to be reinserted when sweep line is moved downwards*/
	vector<Segment> all_segments;/**< the segments intersecting at a particular event point*/
	vector<Point> check_both_neighbours;/**< those Points whose left neighbours have to be tested for intersection for finding new event*/
	vector<Segment> input;/**< input vector of segments*/
	Segment * horizontal = NULL;/**< pointer to an active horizontal segment*/
	Segment * next_horizontal = NULL;/**< pointer to another horizontal segment in case its left point coincides with the right end point af the active horizontal segment*/
	AVLTree<Event> Q;/**< Event Queue*/
	AVLTree<Segment> status;/**< Status Datastructure*/

	
	/** function to read input
	*/
	void read_input()
	{
		int n;
		cin >> n;
		input.resize(n);
		for(int  i = 0;i < n;i++)
		{
			Point a,b;
			cin >> a.x >> a.y >> b.x >> b.y;
			if(abs(a.y-b.y) > eps)
			{
				if(a.y < b.y)
					swap(a,b);
			}
			else
				if(a.x > b.x)
					swap(a,b);
			Segment s(a,b,i);
			
			input[i] = s;
		}
	}
	
	/** function to check if  intersection event point already exists in event queue
	*@param pt the intersection point
	*@return 0 or 1 depending on existence of intersection point
	*/
	bool check_intersection_pt(Point pt)
	{
		//return 0;
		pt.x += eps;
		Event dup_event = Event(pt,INTERSECTION,INTERSECT_P);
		pt.x -= eps;

		Event * ans;
		ans = Q.successor(dup_event);
		if(ans != NULL)
		{
			if(abs(ans->P.x - pt.x) < eps and abs(ans->P.y - pt.y) < eps and ans->type == INTERSECT_P) return 1;

		}
		return 0;
	}
	/** function to insert segment into status
	*@param s the segment to be inserted
	*/
	void insert(Segment &s)
	{
		/*
		find left and right neighbours and check for intersection
		and insert intersection point in event queue if it does not already exist.
		*/
		Segment temp_seg = Segment(s);
		temp_seg.idx = INTERSECT_P + 1;

		Segment  * left_neighbour = status.successor(temp_seg);
		temp_seg.idx = -1;
		Segment * right_neighbour = status.predecessor(temp_seg);

		
		pair<bool,Point> ans;
		//printSegment(s);

		if(left_neighbour!=NULL)
		{
			ans = s.intersection_with_segment(*left_neighbour);
			if(ans.first and ans.second.y < s.sweepLine - eps and check_intersection_pt(ans.second) == 0)
			{
				//cerr << "I " << ans.second.x << " " << ans.second.y << " " << left_neighbour->idx << endl;
				Q.insert(Event(ans.second,INTERSECTION,INTERSECT_P));
			}

		}
		if(right_neighbour!=NULL)
		{

			ans = s.intersection_with_segment(*right_neighbour);
			if(ans.first and ans.second.y < s.sweepLine - eps and check_intersection_pt(ans.second) == 0)
			{
				//cerr << "I " << ans.second.x << " " << ans.second.y << " "  << right_neighbour->idx << endl;
				Q.insert(Event(ans.second,INTERSECTION,INTERSECT_P));
			}
			
		}
		status.insert(s);

		return ;
	}
	/** to check for intersection of left and right neighbours(segments) of a point and insert the intersection point if it exists 
	*@param pt the point whose left and right neighbours(segments) need to be checked
	*@param temp a segment object used for querying left and right neighbour
	*/
	void find_new_event(Point pt,Segment &temp)
	{
		temp.A.x = pt.x - 0.1;temp.A.y = pt.y + 0.1;
		temp.B.x = pt.x + 0.1;temp.B.y = pt.y - 0.1;
		Segment  * left_neighbour = status.successor(temp);
		Segment * right_neighbour = status.predecessor(temp);
		pair<bool,Point> ans;
		if(left_neighbour != NULL and right_neighbour != NULL)
		{
			ans = left_neighbour->intersection_with_segment(*right_neighbour);
			if(ans.first and ans.second.y < temp.sweepLine - eps and check_intersection_pt(ans.second) == 0)
			{
				//cerr << "I " << ans.second.x << " " << ans.second.y << " "  << right_neighbour->idx << endl;
				Q.insert(Event(ans.second,INTERSECTION,INTERSECT_P));
			}
		} 

	}
	/** function to perform all required actions to change the sweep line status
	 * @param temp a segment temp
	*/ 
	void change_sweep_status(Segment &temp)
	{

		
		for(auto seg : reinsert)
		{
			insert(seg);
		}
		for(auto pt : check_both_neighbours)
		{
			find_new_event(pt,temp);
		}
		check_both_neighbours.clear();
		reinsert.clear();//we are moving the sweep line below
	}

	/** making a query segment at point x,sweepLine(y)
	*@param x the x coordinate
	*@param the segment which will be used for querying
	*/
	void query_node(long double x,Segment &temp)
	{
		/*
		make a segment with slope -1 whose intersection with sweep
		line is just less than than given given x 
		*/
		x = x - eps;
		temp.A.x = x - 1;
		temp.B.x = x + 1;
		temp.A.y = temp.sweepLine + 1;
		temp.B.y = temp.sweepLine - 1; 
		return ;
	}

	/** find and remove all event points coincident with the given event point
	*@param e the event point  
	*/
	void remove_same_event_pts(Event e)
	{
		e.idx = -1;
		e.type = -1;
		Event * query_ans;
		
		while((query_ans = Q.upper_bound(e)) != NULL)
		{

			if(abs(query_ans->P.x - e.P.x) > eps or abs(query_ans->P.y - e.P.y) > eps)
				break;
			else
			{
				//handle HORIZONTAL_P
				
				if(query_ans->type == UPPER_HORIZONTAL_P)
				{
					next_horizontal = &input[query_ans->idx];
				}
				if(query_ans->type == UPPER_END_P)
				{
					status.insert(input[query_ans->idx]);
				}
				Q.deletion(*query_ans);
			}
		}
	}
	/** find all segments passing through a given point(the y coordinate is just the sweepLine's value)
	*@param x the x coordinate of the point
	*@param temp the segment used for querying to find all segments required 
	*/
	void find_segments(long double x,Segment &temp)
	{
		Segment * ans;
		query_node(x,temp);
		while((ans = status.upper_bound(temp)) != NULL)
		{

			long double x_intersect = ans->intersection_with_sweep_line();
			
			if(abs(x_intersect - x) > eps)
					break;

			Segment temp_seg = Segment(*ans);

			status.upper_bound_and_deletion(temp);

			all_segments.push_back(temp_seg);
		}
	}
	
	/** to handle all the segments passing through the given point
	*@param x the x coordinate
	*@param y the y coordinate 
	*/
	void process_segments(long double x,long double y)
	{
		int horizontal_flag = 0;
		if(horizontal != NULL)
		{
			horizontal_flag = 1;
		}
		if(next_horizontal != NULL)
			horizontal_flag++;

		if(all_segments.size() + horizontal_flag > 1)
		{
			intersection_pts(x,y);
		}

		
		reverse(all_segments.begin(),all_segments.end());
		
		bool only_lower = 1;
		for(auto s : all_segments)
		{

			if(abs(s.B.y - s.sweepLine) >= eps)
			{
				only_lower = 0;
				reinsert.push_back(s);
			}
			
		}
		if(only_lower)
			check_both_neighbours.push_back(Point(x,y));
		all_segments.clear();
	}

	/** check if the segment is horzizontal
	*@param s the segment which needs to be checked
	*@return 1 or 0 depending on whether segment is horizontal or not
	*/
	bool check_horizontal_segment(Segment &s)
	{
		if(abs(s.A.y - s.B.y) < eps)
			return 1;
		else
			return 0;
	}
	/** find intersection of segments in status with the active horizontal segment
	*/ 
	void intersections_with_horizontal_segment()
	{
		Segment temp_seg = Segment(*horizontal);//probably make it non horizontal
		temp_seg.idx = -1;
		Segment * ans;
		while((ans = status.predecessor(temp_seg))!=NULL)
		{
			long double x_intersect = ans->intersection_with_sweep_line();
			if(x_intersect < horizontal->B.x - eps)
			{
				all_segments.push_back(*ans);
				intersection_pts(x_intersect,temp_seg.sweepLine);
				all_segments.clear();
				temp_seg.A.x = ans->A.x;
				temp_seg.B.x = ans->B.x;
				temp_seg.A.y = ans->A.y;
				temp_seg.B.y = ans->B.y; 
			}
			else
				break;
		}
	}
	/** output function to report the intersection point
	*@param x the x coordinate
	*@param y the y coordinate
	*/
	void intersection_pts(long double x,long double y)
	{
		

		output_intersections.push_back({x,y});
	
		cout << "-----------------\n";
		cout << x << " " << y << endl;
		for(auto it:all_segments)
			cout << it.idx + 1 << " ";
		if(next_horizontal != NULL)
			cout << next_horizontal->idx + 1 << endl;
		if(horizontal != NULL)
			cout << horizontal->idx + 1 << " " ;
		cout << "\n-----------------\n";
	}
	/** the function which initiates the algorithm and process all event points
	*/
	void algo()
	{
		cout << fixed << setprecision(4) ;
		Segment temp = Segment();
		temp.sweepLine = 10000000000;

		read_input();
		int N = input.size();
		for(int i = 0;i < N;i++)
		{
					
			if(check_horizontal_segment(input[i]))
			{
				Event e1(input[i].A,i,UPPER_HORIZONTAL_P);
				Event e2(input[i].B,i,LOWER_HORIZONTAL_P);
				Q.insert(e1);
				Q.insert(e2);
			}
			else
			{
				Event e1(input[i].A,i,UPPER_END_P);
				Event e2(input[i].B,i,LOWER_END_P);
				Q.insert(e1);
				Q.insert(e2);
			}		
		}

		

		while(Q._size)
		{
			Event cur_event = *Q.get_min();
			if(abs(cur_event.P.y - temp.sweepLine) > eps)
			{
				temp.sweepLine -= eps;
				change_sweep_status(temp);	
			}

			cur_event = *Q.get_min();//just extraction not deletion
			cur_event = Event(cur_event);//create duplicate
			temp.sweepLine = cur_event.P.y;

			bool check_nullify = 0;
			
			//cout << "E " << cur_event.P.x << " " << cur_event.P.y << endl;

			if(cur_event.type == LOWER_HORIZONTAL_P or cur_event.type == UPPER_HORIZONTAL_P )
			{
				if(horizontal == NULL)
				{
					horizontal = &input[cur_event.idx];
					Q.deletion(cur_event);
					continue;
				}
				else // end of horizontal seg
				{
					intersections_with_horizontal_segment();
					check_nullify = 1;
					 
				}
			}		


			remove_same_event_pts(cur_event);
			find_segments(cur_event.P.x,temp);
			process_segments(cur_event.P.x,temp.sweepLine);

			if(check_nullify)
			{
				horizontal = NULL;
				if(next_horizontal != NULL)
					horizontal = &*next_horizontal;
				next_horizontal = NULL;
			}

		}

	}	
};


	
int main(){
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	Bentley ob;
	ob.algo();
}


