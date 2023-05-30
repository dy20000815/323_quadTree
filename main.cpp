#include <iostream>
#include <fstream>
using namespace std;
class QtNode {
public:
	int color;
	int upperR;
	int upperC;
	int size;
	QtNode* NWkid;
	QtNode* NEkid;
	QtNode* SWkid;
	QtNode* SEkid;

	QtNode(int cl,int upR, int upC, int s, QtNode* NW, QtNode* NE, QtNode* SW, QtNode* SE) {
		color = cl;
		upperR = upR;
		upperC = upC;
		size = s;
		NWkid = NW;
		NEkid = NE;
		SWkid = SW;
		SEkid = SE;
	}

	void printQtNode(ofstream &out) {
		if (NWkid == NULL) {
			out<< "(" << color << ", " << upperR << ", " << upperC << ",NULL, NULL, NULL, NULL)" << endl;
		}
		else {
			out << "(" << color << ", " << upperR << ", " << upperC << ", " << NWkid->color << ", " << NEkid->color << ", " << SWkid->color << ", " << SEkid->color << ")" << endl;
		}
	}
};

class QuadTree {
public:
	QtNode* QtRoot;
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	int power2Size;
	int** imgAry;


	QuadTree(int r, int c, int min, int max) {
		numRows = r;
		numCols = c;
		minVal = min;
		maxVal = max;
		power2Size = computePower2();
		imgAry = new int* [power2Size];
		for (int i = 0; i < power2Size; i++) {
			imgAry[i] = new int[power2Size];
		}
	}

	int computePower2() {
		int size = max(numRows, numCols);
		int power2 = 2;
		while (size > power2) {
			power2 *= 2;
		}return power2;
	}

	void loadImage(ifstream &in) {
		int input;
		for (int i = 0; i < numRows; i++) {
			for (int j = 0; j < numCols; j++) {
				in >> input;
				imgAry[i][j] = input;
			}
		}
	}

	void zero2DAry() {
		for (int i = 0; i <power2Size; i++) {
			for (int j = 0; j < power2Size; j++) {
				imgAry[i][j] = 0;
			}
		}
	}

	QtNode* buildQuadTree(int** ary, int upR, int upC, int size, ofstream &output) {
		QtNode* node = new QtNode(-1, upR, upC, size, NULL, NULL, NULL, NULL);
		node->printQtNode(output);
		if (size == 1) {
			node->color = imgAry[upR][upC];
		}
		else {
			int halfSize = size / 2;
			node->NWkid = buildQuadTree(ary, upR, upC, halfSize, output);
			node->NEkid = buildQuadTree(ary, upR, upC + halfSize, halfSize, output);
			node->SWkid = buildQuadTree(ary, upR + halfSize, upC, halfSize, output);
			node->SEkid = buildQuadTree(ary, upR + halfSize, upC + halfSize, halfSize, output);
			int sumColor = addKidsColor(node);
			if (sumColor == 0) {
				node->color = 0;
				node->NWkid = NULL;
				node->NEkid = NULL;
				node->SWkid = NULL;
				node->SEkid = NULL;
			}
			else if (sumColor == 4) {
				node->color = 1;
				node->NWkid = NULL;
				node->NEkid = NULL;
				node->SWkid = NULL;
				node->SEkid = NULL;
			}
			else {
				node->color = 5;
			}
		}return node;
	}

	int addKidsColor(QtNode* n) {
		int sum = 0;
		sum += n->NWkid->color;
		sum += n->NEkid->color;
		sum += n->SWkid->color;
		sum += n->SEkid->color;
		return sum;
	}

	bool isLeaf(QtNode* n) {
		return (n->color == 1 || n->color == 0);
	}

	void preOrder(QtNode* r, ofstream &outfile1) {
		if (isLeaf(r) == true) {
			r->printQtNode(outfile1);
		}
		else {
			r->printQtNode(outfile1);
			preOrder(r->NWkid,outfile1);
			preOrder(r->NEkid, outfile1);
			preOrder(r->SWkid, outfile1);
			preOrder(r->SEkid, outfile1);
		}
	}

	void postOrder(QtNode* r, ofstream& outfile1) {
		if (isLeaf(r) == true) {
			r->printQtNode(outfile1);
		}
		else {
			preOrder(r->NWkid, outfile1);
			preOrder(r->NEkid, outfile1);
			preOrder(r->SWkid, outfile1);
			preOrder(r->SEkid, outfile1);
			r->printQtNode(outfile1);
		}
	}

};

int main(int argc, char* argv[]) {
	string infile = argv[1];
	string outfile1 = argv[2];
	string outfile2 = argv[3];
	ifstream input(infile);
	ofstream output1(outfile1);
	ofstream output2(outfile2);
	int row, col, min, max;
	input >> row >> col >> min >> max;
	QuadTree* tree = new QuadTree(row, col, min, max);
	output2 << "Power2Size is " << tree->power2Size << endl;
	tree->zero2DAry();
	tree->loadImage(input);
	output2 << "Print imgAry: " << endl;
	for (int j = 0; j < tree->power2Size; j++) {
		for (int k = 0; k < tree->power2Size; k++) {
			output2 << tree->imgAry[j][k];
		}output2 << endl;
	}
	output2 << "Debug for buildQuadTree: " << endl;
	tree->QtRoot = tree->buildQuadTree(tree->imgAry, 0, 0, tree->power2Size, output2);
	output1 << "PreOrder: " << endl;
	tree->preOrder(tree->QtRoot, output1);
	output1 << "PostOrder: " << endl;
	tree->postOrder(tree->QtRoot, output1);
}