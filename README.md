# Document Page Segmentation
(term project for semester Jan - Jul , '18 )
## purpose
It segments the document provided recursively in a stores the Segments blocks in a tree like structure to match for layout similarity from a dataset.
## Dependenices 
opencv2 : for reading and manipulating documents.
## functions
xy_cut.cpp : segments a a document and creates a tree of blocks.
use custom documents and pass them in to arguments to check functionality.
Result blocks are stored as binary in result folder and resultant segmented image stored as result.jpg in root folder.
requires library opencv
