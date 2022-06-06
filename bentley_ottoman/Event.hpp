#ifndef Event_H
#define Event_H
#include <bits/stdc++.h>
#include "Points_and_Lines.hpp"
using namespace std;
#define EPS 0.00000000000002
class Event
{
public:
	Point P;/**< Event Point*/
	int idx;/**< index of Event*/
	int type;/**< type of Event*/
	/**default constructor makes a event with idx = 0 and type = - 1
	*/
	Event() 
	{
		
		idx = 0, type = -1;
	}
	/** constructor to make an Event out of the parameters passed
	*@param _P event point
	*@param  _idx index of the event
	*@param _type type of event
	*/
	Event(Point _P, int _idx, int _type)
	{
		P = _P, idx = _idx, type = _type;
	}
	//use eps
	/** compares if the event is less than the parameter event based primarily on y co-oordinate of Point P
	*@param E the event to be compared with
	*@return 1 if this event is less than parameter event otherwise 0
	*/
	bool operator < (const Event& E) const	
	{
		if(P.y != E.P.y)
			return P.y > E.P.y;
		else
			if(P.x != E.P.x)
				return P.x < E.P.x;
			else
				if(type != E.type)
					return type < E.type;
				else
					return idx < E.idx;
	}
	/** compares if the event is greater than the parameter event based primarily on y co-oordinate of Point P
	*@param E the event to be compared with
	*@return 1 if this event is greater than parameter event
	*/
	bool operator > (const Event& E) const	
	{
		if(P.y != E.P.y)
			return P.y < E.P.y;
		else
			if(P.x != E.P.x)
				return P.x > E.P.x;
			else
				if(type != E.type)
					return type > E.type;
				else
					return idx > E.idx;
	}
	/** compares the segments for inequality
	*@param E the event to be compared with
	*@return 1 if they are different otherwise 0
	*/
	bool operator != (Event &E) 
	{
		if(P == E.P and type == E.type and idx == E.idx)
			return false;
		else
			return true;
	}
	/** compares the segments for equality
	*@param E the event to be compared with
	*@return 1 if they are equal otherwise 0
	*/
	bool operator == (Event &E) 
	{
		if(P == E.P and type == E.type and idx == E.idx)
			return true;
		else
			return false;
	}
};
#endif