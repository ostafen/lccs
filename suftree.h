// suftree.h

int *computePD(int *x, int m);
void slowScan(Node p, int k, Node *resp, int *resk, String y, int n);
Node fastScan(Node r, int j, int k, String y);
int buildSufTree(String y, int n, int m);
