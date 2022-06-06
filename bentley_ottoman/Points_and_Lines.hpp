#ifndef Points_and_Lines_H
#define Points_and_Lines_H
#include <bits/stdc++.h>
using namespace std;
#define EPS 0.00000000000002

class Point
{
	public :
	long double x;/**< x coordinate of point*/
	long double y;/**< y coordinate of point*/

	public :
	/**default constructor initializes everything to zero 
	*/
	Point() 
	{
		x = 0, y = 0;
	}
	/**parameterized constructor
	*@param x 
	*@param y
	*/
	Point(long double _x, long double _y)
	{
		x = _x, y = _y;
	}
	/**overloading equal to operator
	*@return 0 or 1
	*/
	bool operator == (Point &p)
	{
		return abs(x-p.x) < EPS and abs(y - p.y) < EPS;
		//return (x == p.x and y == p.y);
	}
	/**function to check whether the point lies lies_between P and Q
	*@param P
	*@param Q
	*@return 0 or 1
	*/
	bool lies_between(Point &P, Point &Q)
 	{
 		bool checkX = x < max(P.x, Q.x) + EPS && x + EPS > min(P.x, Q.x);
 		bool checkY = y < max(P.y, Q.y) + EPS && y + EPS > min(P.y, Q.y);
 		return checkX && checkY;
 	}
};

class Segment
{
	public :
	Point A;/**< Upper End Point of Segment*/
	Point B;/**< Lower End Point of Segment*/
	int idx;/**< index of Segment*/

	static long double sweepLine;/**< sweepLine which is shared among all objects and represents sweep line status*/
	
	/**default constructor makes a segment with idx = -1
	*/
	Segment()
	{
		idx = -1;
	}
	/** constructor to make an object which has same values as the parameter
	*@param _S segment whose fields need to be copied
	*/
	Segment(const Segment &seg)
	{
		A = seg.A, B = seg.B, idx =  seg.idx;
	}
	/** constructor to make a Segment out of the parameters passed
	*@param _A point A
	*@param _B point B
	*@param _idx index
	*/
	Segment(Point _A, Point _B, int _idx)
	{
		A = _A, B = _B, idx = _idx;
	}
	/** computes and returns the intersection of segment with sweepLine
	*@return the intersection of segment with sweep line
	*/
	long double intersection_with_sweep_line()
	{
		//vertical segment
		if(abs(A.x - B.x) < EPS)
			return A.x;
		
		//horizontal segment means overlapping segment
		if(abs(A.y - B.y) < EPS)
			return A.x;

		long double slope = (A.y - B.y) / (A.x - B.x);
		return (sweepLine -  (A.y - slope * A.x)) / slope;

	}
	/** compares if the segment is less than the parameter segment based on intersection of them with sweepLine
	*@param S the segment to be compared with
	*@return 1 if this segment is less than parameter segment otherwise 0
	*/
	bool operator < ( Segment &S) 
	{
		long double x1 = intersection_with_sweep_line();
		long double x2 = S.intersection_with_sweep_line();
		return x1 < x2 + EPS;
	}
	/** compares if the segment is greater than the parameter segment based on intersection of them with sweepLine
	*@param S the segment to be compared with
	*@return 1 if this segment is greater than the parameter segment
	*/
	bool operator > ( Segment &S) 
	{
		long double x1 = intersection_with_sweep_line();
		long double x2 = S.intersection_with_sweep_line();

		return x1 + EPS > x2; 

	}
	/** compares the segments for equality
	*@param S the segment to be compared with
	*@return 1 if they are equal otherwise 0
	*/
	bool operator == (Segment &S)
	{
		if(A == S.A  and B == S.B)
			return true;
		else
			return false;
	}
	/** compares the segments for inequality
	*@param S the segment to be compared with
	*@return 1 if they are different otherwise 0
	*/
	bool operator != (Segment &S)
	{
		if(A == S.A  and B == S.B)
			return false;
		else
			return true;
	}
	/** computes intersection of this segment with the parameter segment
	*@param S the segment with which intersection has to be found
	*@return a pair of <bool,Point> where the first will indicate whether the intersection exists or not and second field will contain the Point in case of existenc
	*/
	pair<bool,Point> intersection_with_segment(Segment &S)
	{
		long double slope,slope_S,b,_b,c,_c;
		if(abs(A.x - B.x) < EPS)
		{
			slope = 1;
			b = 0;
			c = -A.x;
		}
		else
		{
			slope = (A.y - B.y) / (B.x - A.x);
			b = 1.0;
			c = -(slope * A.x + A.y);
		}

		if(abs(S.A.x - S.B.x) < EPS)
		{
			slope_S = 1;
			_b = 0;
			_c = -S.A.x;
		} 
		else
		{
			slope_S = (S.A.y - S.B.y) / (S.B.x - S.A.x);
			_b = 1.0;
			_c = -(slope_S * S.A.x + S.A.y);
		}
		if(abs(slope-slope_S) < EPS and abs(b-_b) < EPS)
		{
			//parallel at end points
			return {0,Point()};
		}
		else
		{
			long double x = (b * _c - c * _b) / (slope * _b - b * slope_S);
			long double y;
			if(abs(b) < EPS)
				y = -slope_S * x - _c;
			else
				y = -slope * x - c;
			
			Point intersect = Point(x,y);
			if(intersect.lies_between(A,B) and intersect.lies_between(S.A,S.B))
				return {1,intersect};
			else
				return {0,Point()};
		}

	}

};

#endif