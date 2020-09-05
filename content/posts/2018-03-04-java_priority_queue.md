---
layout: post
date: 2018-03-04
title: Java Util's Priority Queue Mishap
---

I recently found out by doing [Hackerrank's Breadth First Search: Shortest
Reach](https://www.hackerrank.com/challenges/bfsshortreach) that Java's
Priority Queue does not maintain the heap property if you modify the priority
of one of the elements in the queue. The only moment where the heap property
is maintained is when the queue is being populated, i.e:  when elements are
added to the queue.

This is better shown with a small example.

Here's a simple PriorityQueue used as a min priority queue, of Vertex, which
represents a vertex (node) of a Graph and its members are its index and its
distance to the source vertex.

The relevant parts of the implementation are:

```
class Vertex {
    private int distance;
    private int index;
    
    public int getDistance() {
        return distance;
    }

    public void setDistance(int d) {
        distance = d;
    }
.
.
.

import java.util.PriorityQueue;

class Example {
    public static void main(String[] args) {
        PriorityQueue<Vertex> queue = new PriorityQueue<Vertex>(n, new Comparator<Vertex>() {
            @Override
            public int compare(Vertex o1, Vertex o2) {
                if (o1.getDistance() < o2.getDistance()) {
                    return -1;
                } else if (o1.getDistance() > o2.getDistance()) {
                    return 1;
                }
                return 0;
            }
        });
        
        Vertex[] vertices = new Vertex[10];
        
        // initialize the vertices and add them to the queue
        for (int i = 0; i < 10; i++) {
            vertices[i] = new Vertex(i);
            vertices[i].setDistance(i + 1);
            queue.add(v);
        }
    }
    
.
.
.
}
```

The constructor that takes a Comparator is necessary to establish the priority
of the elements and the structure of the heap in the backend.
In our case, the priority is given by the distance of the Vertex.

The problem surges in that if we later do:


``` 
    v[i].setDistance(0);
```

The position in the queue will not change, i.e **the heap in the backend
remains in the same state as it was before the distance for *v[i]* was changed**.
The only moment where the priority queue actually runs the comparator is when
is when the queue is being acted upon i.e: either when using *add* or *remove*
methods.


```
    \\ initial state
    v[i].setDistance(0);
    \\ same state as 'initial state'!
```

The only way to achieve the expected result is to
do this:

``` 
    // initial state i.e: v[i] is in position P
    v[i].setDistance(0);
    // same state as initial state i.e: v[i] is in position P
    queue.remove(v[i]);
    queue.add(v[i]);
    // different state i.e: v[i] is in position Q, in a different from the position it was in `initial state`
```

It's crazy to think I spent the better part of two hours thinking and looking
into the code just to start doubting everything I know and then realize that
the issue is in that the implementation doesn't work as one might expect.

I don't suppose everyone thinks the same way I do, but it would have been nice
if the class contained some method that could *reheapify* the priority queue
or something in that regard.

Oh well, I've learned my lesson now, Java Class Library. *Never make any
assumption about your behaviour*

