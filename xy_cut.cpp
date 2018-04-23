
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <vector>
#include <string>
#include <stack>

using namespace std;
using namespace cv;

//Tree Structure Definition
typedef struct STreeNode* pSTreeNode; //pointer to a STreeNode
typedef int TreeDataType;
typedef pair<std::vector<Mat>,std::vector<Rect> > MatRect;

struct STreeNode{
    TreeDataType data; //int data
    Rect boundingRect;
    pSTreeNode pFirstChild;
    pSTreeNode pNextBrother;
    pSTreeNode pParent;


    STreeNode(TreeDataType Value,Rect rect){
        data = Value;
        boundingRect = rect;
        pFirstChild = NULL;
        pNextBrother = NULL;
        pParent = NULL;
    }
};

class CTree{
public:
    CTree();
    CTree(TreeDataType Value,Rect rect);
    //~CTree();
public:
    void Insert(TreeDataType parentValue,TreeDataType Value,Rect bRect);
    void InsertBrother(pSTreeNode pParentNode,TreeDataType Value,Rect bRect,pSTreeNode parentNode);
    pSTreeNode Search(pSTreeNode pNode,TreeDataType Value);
    pSTreeNode SearchParent(pSTreeNode pNode,TreeDataType Value);
    void print();
    void print(pSTreeNode node, int num);
public:
    pSTreeNode pRoot;
};

CTree::CTree(){
    pRoot = NULL;
}

CTree::CTree(TreeDataType Value,Rect rect){
    pRoot = new STreeNode(Value,rect);
    if(pRoot == NULL) //pointer to root
        return;
}

void CTree::Insert(TreeDataType parentValue,TreeDataType Value,Rect bRect){
    if(pRoot == NULL)
        return;
    pSTreeNode pFindNode = Search(pRoot,parentValue); //store pointer to the parent
    if(pFindNode == NULL) //if given parent value is not in the tree
        return;
    if(pFindNode->pFirstChild == NULL){ //if there is no child to the provided parent node
        pFindNode->pFirstChild = new STreeNode(Value,bRect); //create a new child and store address in pFirstChild
        pFindNode->pFirstChild->pParent = pFindNode; //assign current node as the parent of newly created child
        return;
    }else{
        InsertBrother(pFindNode->pFirstChild,Value,bRect,pFindNode);
        return;
    }
}

void CTree::InsertBrother(pSTreeNode pBrotherNode,TreeDataType Value,Rect bRect,pSTreeNode parentNode){
    if(pBrotherNode->pNextBrother != NULL) //recursively call InsertBrother till next brother is NULL
        InsertBrother(pBrotherNode->pNextBrother,Value,bRect,parentNode);
    else{
        pBrotherNode->pNextBrother = new STreeNode(Value,bRect); //create a new brother and store value to it
        pBrotherNode->pNextBrother->pParent = parentNode;
        return;
    }
}

pSTreeNode CTree::Search(pSTreeNode pNode,TreeDataType Value){
    if(pNode == NULL)
        return NULL;

    if(pNode->data == Value) //if node data equals value to be searched
        return pNode;

    if(pNode->pFirstChild == NULL && pNode->pNextBrother == NULL) //if the node has no more children or brothers
        return NULL;
    else{ //recursively check for value at the successive nodes( children and brothers)
        if(pNode->pFirstChild != NULL){
            pSTreeNode pNodeTemp = Search(pNode->pFirstChild,Value);
            if(pNodeTemp != NULL) //if value being searched is found in the node's child
                return pNodeTemp;
            else{
                return Search(pNode->pNextBrother,Value);
            }
        }else
            return Search(pNode->pNextBrother,Value);
    }
}

pSTreeNode CTree::SearchParent(pSTreeNode pNode,TreeDataType Value){
    if(pNode == NULL)
        return NULL;
    if(pNode->data == Value)
        return pNode;
    if(pNode->pFirstChild == NULL && pNode->pNextBrother == NULL)
        return NULL;
    else{
        if(pNode->pFirstChild != NULL){
            pSTreeNode pNodeTemp = Search(pNode->pFirstChild,Value);
            if(pNodeTemp != NULL)
                return pNode->pFirstChild;
            else{
                return Search(pNode->pNextBrother,Value);
            }

        }else
            return Search(pNode->pNextBrother,Value);
    }
}

void CTree::print()
{
    print(pRoot, 0);
}

void printSpace(int num)
{
    int i = 0;
    for(i = 0; i < num-3; i++)
        cout << " ";

    for(; i < num-2; ++i)
        cout << "|";
    for(; i < num; ++i)
        cout << "_";
}

void CTree::print(pSTreeNode node, int num)
{
    if(node != NULL){
        printSpace(num);
        cout << node->data << endl;
        print(node->pFirstChild, num+4);
        print(node->pNextBrother, num);
    }
}

// Tree Structure Definition end!

Mat loadImage(string filepath){
    Mat image = imread(filepath);

    if (image.empty()) {
        cout<<"read image failed"<<endl;
        return Mat();
    }

    cvtColor(image, image, CV_BGR2RGB);
    return image;
}

Mat convert2gray(Mat input){
    Mat grayImage( input.size(), CV_8UC1 );
    cvtColor ( input, grayImage, CV_RGB2GRAY );
    return grayImage;
}

Mat binaryzation(Mat grayImage){
    if(grayImage.channels() != 1){
        cout<<"binarylize must be the gray image"<<endl;
        return Mat();
    }
    Mat binaryImage;

    threshold(grayImage,binaryImage,125,255,THRESH_BINARY);
    cout<<binaryImage.rows<<" "<<binaryImage.cols<<endl;
    return binaryImage;
}

std::vector<int> calcHistVer(Mat binaryImage){
    assert(binaryImage.channels() == 1);

    Mat paintX = Mat::zeros(binaryImage.rows,binaryImage.cols,CV_8UC1);
    std::vector<int> histVer;
    histVer.reserve(binaryImage.cols);
    for(int col = 0;col < binaryImage.cols;col++){
        histVer.push_back(0);
    }

    for(int col = 0;col < binaryImage.cols;col++){

        for(int row = 0;row < binaryImage.rows;row++){
            const uchar* ptr = (const uchar*) binaryImage.ptr(row);
            if(ptr[col] > 0){
                histVer[col]++;
            }

        }
    }
    cout<<histVer.size()<<endl;

    for(int col = 0;col < binaryImage.cols;col++){
        for(int row = 0;row < histVer[col];row++){
            uchar * ptr_x = paintX.ptr<uchar>(row);
            ptr_x[col] = 255;
        }
    }
    // imshow("axisX!",paintX);
    // waitKey(-1);
    return histVer;
}

std::vector<int> calcHistHor(Mat binaryImage){ //returns number of 255's or white pixels for all rows (horizontally)
    assert(binaryImage.channels() == 1);

    Mat paintY = Mat::zeros(binaryImage.rows,binaryImage.cols,CV_8UC1);
    std::vector<int> histHor;
    histHor.reserve(binaryImage.rows);
    for(int row = 0;row < binaryImage.rows;row++){
        histHor.push_back(0);
    }
    for(int row = 0;row < binaryImage.rows;row++){
        const uchar* ptr = (const uchar*) binaryImage.ptr(row); //returns pointer to the row 's first element for column iteration
        for(int col = 0;col < binaryImage.cols;col++){
            if(ptr[col] > 0){
                histHor[row]++; //counts number of 255's or white pixels at a given row
            }
        }
    }

    for(int row = 0;row<binaryImage.rows;row++){
        uchar* ptr_y = paintY.ptr<uchar>(row);
        for(int col = 0;col < histHor[row];col++){
            ptr_y[col] = 255;
        }
    }

//     imshow("axisY!",paintY); //shows histogram of a given segment
//     waitKey(-1);

    return histHor;
}
//checked no errors
vector<int> shrinkHist(vector<int>& hist,int& cursor1,int& cursor2){
    vector<int> thinHist;
    thinHist.reserve(hist.size()); //number of rows

    for(vector<int>::const_iterator it1 = hist.begin();it1 != hist.end();it1++){
        if(*it1 != 0){ //if there are any white pixels in that row
            break;
        }
        cursor1++;
    }
    //cursor1 points to the row from the start  which has any white pixel .
    for(vector<int>::const_iterator it2 = hist.end();it2 != hist.begin();it2--){
        if(*it2 != 0){//if there are any white pixels in that row
            break;
        }
        cursor2--;
    }
    //cursor2 points to the row for the end which has any white pixel .

    for(int i = cursor1+1;i < cursor2;i++){
        thinHist.push_back(hist[i]); // we store the historgram from the reduced range and return it
    }

    return thinHist;
}

void countZero(vector<int>& hist,std::vector<int>& segments1,std::vector<int>& segments2,int top,int bottom){
    int cursor1 = 0;
    int cursor2 = 0;
    int preBlock = 0;
    int preBlock_end = 0;

    while((cursor2 != hist.size()) && (cursor1 != hist.size())){
        if(hist[cursor1] == 0){//0 histogram value == !(full white row or column in actual image)
            cursor2 = cursor1;
            preBlock_end = cursor1 - 1; // store the end of previous block
            while((hist[cursor2] == 0) && (cursor2 != hist.size())){
                cursor2++; //increment cursor 2 till there are no white pixels(black in actual) in a row
            }
//            cursor2 points to a row which has a white pixel(black in actual)
            if((cursor2 - cursor1 + 1) >= 12){ //cursor1 to cursor2 makes a segment if it is >=15 pixels
                segments1.push_back(top + preBlock - 1);
                segments2.push_back(top + preBlock_end + 1);
                preBlock = cursor2; //change preblock for next iteration
            }
            cursor1 = cursor2; //assign cursor1 at the end of segmented block

        }else{
            cursor1 ++;
        }
    }
//    if(segments1.size()==0&&segments2.size()==0)
//    {
//        segments1.push_back(top);
//        segments2.push_back(bottom);
////        cout<<"ye chal gaya"<<endl;
//    }

    //add the last segments to vector
    // cout<<"cursor2: "<<cursor2<<endl;
    // segments1.push_back(top + cursor2 - 1);
    if(cursor1 - preBlock >= 3){
        segments1.push_back(top + preBlock - 1);
        segments2.push_back(top + cursor1 + 1);
    }
     cout<<"start: "<<segments1[0]<<"    end: "<<segments2[0]<<endl;
    cout<<"segments1' size is: "<<segments1.size()<<endl;
    cout<<"segments2' size is: "<<segments2.size()<<endl;

}
//char classify(Mat x)
//{
//
//}
std::vector<Mat> cropImage(Mat binaryImage/*,Mat input*/,bool tricker,int index,std::vector<Rect>& cropRects){

    std::vector<Mat> cropMats;
    // std::vector<Rect> cropRects;
    // std::vector<Point> blockPoints;

    if(tricker){
        std::vector<int> histHor = calcHistHor(binaryImage); //creates a histogram corresponding to the rows
        int cursor1 = 0;
        int cursor2 = histHor.size(); //number of rows
        std::vector<int> thinHist = shrinkHist(histHor,cursor1,cursor2); //minimises the size of the vertical block

        std::vector<int> segments1;
        std::vector<int> segments2;

        countZero(thinHist,segments1,segments2,cursor1,cursor2);
        // cout<<"countZero end!"<<endl;

        if((segments1.size() > 0) && (segments2.size() > 0) && (segments1.size() == segments2.size())){
            if(segments1.size() == segments2.size()){
                for(int i = 0;i < segments1.size();i++){
                    int y = segments1[i];
                    int x = 0;
                    int height = segments2[i] - segments1[i];
                    int width = binaryImage.cols;

                    Rect cropRect(x,y,width,height);
                    cropRects.push_back(cropRect);
                    // Mat image_cut = Mat(input,cropRect);
                    Mat image_cut = Mat(binaryImage,cropRect);
                    cropMats.push_back(image_cut); //insert the segmented matrix into cropMats
                }
            }else{
                cout<<"error! 2 vectors' size are not equal!"<<endl;
            }
        }
    }else{
        std::vector<int> histVer = calcHistVer(binaryImage);
        int cursor1 = 0;
        int cursor2 = histVer.size();
        std::vector<int> thinHist = shrinkHist(histVer,cursor1,cursor2);
        std::vector<int> segments1;
        std::vector<int> segments2;

        countZero(thinHist,segments1,segments2,cursor1,cursor2);

        if((segments1.size() > 0) && (segments2.size() > 0) && (segments1.size() == segments2.size())){
            for(int i = 0;i < segments1.size();i++){
                int x = segments1[i];
                int y = 0;
                int height = binaryImage.rows;
                int width = segments2[i] - segments1[i];
                Rect cropRect(x,y,width,height);
                cropRects.push_back(cropRect);
                Mat image_cut = Mat(binaryImage,cropRect);
                cropMats.push_back(image_cut);
            }
        }else{
            cout<<"error! 2 vectors' size are not equal!"<<endl;
        }
    }

    if(cropMats.size() == 1){
        imwrite("result/" + to_string(index) + ".jpg",cropMats[0]); //saving each segment as binary in result folder
//        char class=classify(cropMats[0]);
//        Mat t =cropMats[0] (*(cropRects.end()-1)) ;
//        imshow(to_string((index)),cropMats[0]);
//        waitKey();
    }
    /*
    if((cropMats.size() == 1) && (cropRects.size() == 1)){
        rectangle(input,cropRects[0].tl(),cropRects[0].br(),cv::Scalar(0,0,255),1,1,0);
    }*/

    cout<<"here the end of FUNC-cropImage cropMats' size is: "<<cropMats.size()<<endl;
    return cropMats;
}

void makeTree(Mat binaryImage,TreeDataType parentNode,CTree& docTree,int& totalNode,bool& tricker,std::vector<Mat>& leafBlocks,std::vector<int>& leafNodes){
    // imshow("makeTree...",binaryImage);
    // waitKey(-1);
    // CTree docTree = CTree(1);
    // int totalNode = 1;
    std::vector<Rect> childBrects;

    //cropImage stores Mat of cropped images after segmenting it
    std::vector<Mat> childImages = cropImage(binaryImage,tricker,totalNode,childBrects); //childBrects is also updated.
//    for(auto it:childBrects) cout<< it<<endl;
    // std::vector<Mat> childImages2 = cropImage(binaryImage,!tricker,totalNode,childBrects);
    // bool rowcol = true;
    // std::vector<Mat> leafImages;
    tricker = !tricker; // row & col alternating
    TreeDataType parentIndex = totalNode;
    if(childImages.size() > 1){
        // rowcol = true;
        int index = 0;
        for(std::vector<Mat>::const_iterator itc = childImages.begin();itc != childImages.end();itc++){

            docTree.Insert(parentIndex,++totalNode,childBrects[index]); //inserting each segment in a tree structure
            bool innerTrick = tricker;
            // now recursively calling maketree for all the childImages (iniital segments), now segmenting alternately
            makeTree(*itc,totalNode,docTree,totalNode,innerTrick,leafBlocks,leafNodes);
            index++;
        }
    }/*else if(childImages2.size() > 1){
		rowcol = false;
		int index = 0;
		for(std::vector<Mat>::const_iterator itc = childImages2.begin();itc != childImages2.end();itc++){

			docTree.Insert(parentIndex,++totalNode,childBrects[index]);

			bool innerTrick = tricker;
			// cout<<"tricker: "<<tricker<<endl;
			makeTree(*itc,totalNode,docTree,totalNode,innerTrick,leafBlocks,leafNodes);
			index++;
		}
	}*/else{ //if its a single segment of that iteration
        //if(rowcol){
        leafBlocks.push_back(childImages[0]); //that segment Mat is added to the vector
        leafNodes.push_back(totalNode); //node number of that segment is pushed into leafNodes
        return;
        //}else{
        //	leafBlocks.push_back(childImages2[0]);
        //	leafNodes.push_back(totalNode);
        //	return;
        // }
    }

}

std::vector<Rect> boundBlock(CTree& docTree,std::vector<int>& leafNodes){
    std::vector<Rect> boundRects;
    for(std::vector<int>::const_iterator it = leafNodes.begin();it != leafNodes.end();it++){
        cout<<"Now process "<<*it<<endl;
        pSTreeNode leafTreeNode = docTree.Search(docTree.pRoot,*it);

        int x = leafTreeNode->boundingRect.x;
        int y = leafTreeNode->boundingRect.y;
        int width = leafTreeNode->boundingRect.width;
        int height = leafTreeNode->boundingRect.height;

        // pSTreeNode parentTreeNode = leafTreeNode->pParent;
        pSTreeNode parentTreeNode = leafTreeNode;

        // x += parentTreeNode->boundingRect.x;
        // y += parentTreeNode->boundingRect.y;

        while(parentTreeNode->pParent != NULL){
            parentTreeNode = parentTreeNode->pParent;
            cout<<"parent is: "<<parentTreeNode->data<<endl;
            x += parentTreeNode->boundingRect.x;
            y += parentTreeNode->boundingRect.y;
        }
        Rect targetRect(x,y,width,height);
        boundRects.push_back(targetRect);
    }
    cout<<"boundingRect's size is: "<<boundRects.size()<<endl;
    return boundRects;
}

/*
void boundBlock(CTree& docTree){
	std::vector<Rect> bRects;
	stack<pSTreeNode> s;
	pSTreeNode root = docTree.pRoot;
	s.push(root);
	while(){
		pSTreeNode top = s.top();
		if(top.pFirstChild == NULL){
			bRects.push_back(pFirstChild.boundingRect);
			s.pop();
		}

	}

}*/

int main(int argc, char ** argv){

    argv[1] = "sample 1.jpg";
//    if(argc != 2){
//        cout<<"usage: ./X_YCut XXX.jpg"<<endl;
//        return -1;
//    }

    // load the input image
    string filepath = argv[1];
    // preprocess gray-scale binarylization
     //Mat input = loadImage("116.jpg");
 Mat input = loadImage(filepath); //loading image
    Mat input_clone = input.clone(); // copying our image to input_clone
    Mat grayImage = convert2gray(input);
    Mat binaryImage = binaryzation(grayImage);
    Mat binaryImageReg = ~binaryImage;
    // Mat boundInput = input.clone();
    // warning!
    // recursive construct the document tree
    Rect rootRect(0,0,input.cols,input.rows); //dimensions of full page
    CTree docTree = CTree(1,rootRect);
    int totalNode = 1; //first node (initially total =1 )
    bool tricker = true;

    std::vector<Mat> leafBlocks; //store Mat of all the segments
    std::vector<int> leafNodes; // total number of nodes pushed incrementally in maketree
    makeTree(binaryImageReg,1,docTree,totalNode,tricker,leafBlocks,leafNodes);

    if(leafBlocks.size() <= 1){ //if whole document contains a single segment vertically

        Rect rootRect2(0,0,input.cols,input.rows);
        CTree docTree2 = CTree(1,rootRect2);
        int totalNode2 = 1;
        bool tricker2 = false;
        std::vector<Mat> leafBlocks2;
        std::vector<int> leafNodes2;
        //document can have horizontal segments
        makeTree(binaryImageReg,1,docTree2,totalNode2,tricker2,leafBlocks2,leafNodes2); //creates Horizontal segments if any
        docTree2.print();
        cout<<"leaf's size is: "<<leafBlocks2.size()<<endl;
        std::vector<Rect> blockRects = boundBlock(docTree2,leafNodes2);
        for(std::vector<Rect>::const_iterator itr = blockRects.begin();itr!= blockRects.end();itr++){
            rectangle(input_clone,itr->tl(),itr->br(),cv::Scalar(0,0,255),1,1,0);
        }
        imshow("result!",input_clone);
        waitKey(-1);
        imwrite("result.jpg",input_clone);
    }else{
        docTree.print();
        cout<<"leaf's size is: "<<leafBlocks.size()<<endl;
        std::vector<Rect> blockRects = boundBlock(docTree,leafNodes);

        for(std::vector<Rect>::const_iterator itr = blockRects.begin();itr!= blockRects.end();itr++){
            rectangle(input_clone,itr->tl(),itr->br(),cv::Scalar(0,0,255),1,1,0);
        }
        imshow("result!",input_clone);
        waitKey(-1);
        imwrite("result.jpg",input_clone);
        int y=1;

    }


}