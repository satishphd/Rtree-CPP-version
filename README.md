# R-Tree

Taken from: http://superliminal.com/sources/sources.htm

I wanted Guttman's Rtree code from the link above to be used from a C++ program. There was linking problem because of function names in the C code being mangled by C++. So, I added extern "C" pragmas around the header files in include.
Moreover, I add some code in test.cpp which uses a hashmap (unordered_map) so that query results can be stored for later use.

## Building

```
$ make
```

## Using

```
$ g++ -std=c++11 -o prog test.cpp libtree.a -lm
Execute by running ./prog
```

```
#include "index.h"

struct Rect rect = {0, 0, 2, 2}; // xmin, ymin, xmax, ymax (for 2 dimensional RTree)

struct Node* root = RTreeNewIndex();

RTreeInsertRect(&rect, 1, &root, 0); // i+1 is rect ID. Note: root can change
int nhits = RTreeSearch(root, &search_rect, MySearchCallback, 0);
```

## Original Readme

This directory contains a C implementation of the R-tree data structure.
The implementation is originally from the test code of the authors, and
later ported to ANSI C on a variety of platforms by Melinda Green
(melinda@superliminal.com).

Paul Brooke (pbrooke@mindscape.com) discovered an interesting anomaly in
the original algorithm which uses the rectangular volumes of nodes as the
fitting and splitting criteria which is that degenerate rectangles (i.e.
flat in one or more dimensions) can appear as attractive candidate nodes
to contain similarly degenerate nodes which are spatially quite distant.
(A goal that R-trees are meant to avoid). For example, in two dimensions
given two rects where one spans the volume (0,1)->(1,2) and the other spans
(1000,0)->1001,0), into which one should we add a third node spanning
(0,0)->1,0)? Clearly it should go into the first one, but that doubles its
volume to two units whereas adding it to the second one leaves it unchanged
at zero. These sorts of degeneracies are not rare cases since data are
often axially aligned.

Brooke suggested using the volume of the bounding sphere as the area metric
for nodes. This has worked quite well and is currently the metric being
used by the code here. Also implemented but not currently used are metrics
using the N-dimensional surface area and the original implementation using
the N-dimensional box volume. There is also a fast approximation to the
spherical volume as suggested by Brooke. To switch to using the original
box volume for example, simply change the calls to RTreeRectSphericalVolume
to use RTreeRectVolume instead. This is clearly an area deserving more
research. The file sphvol.c contains the code used to generate the table
of unit sphere volumes in the first 20 dimensions.
