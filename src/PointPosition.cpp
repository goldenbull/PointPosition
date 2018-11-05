// PointPosition.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ObsFile.h"
#include "NavFile.h"
#include "Position.h"

int main(int argc, char* argv[])
{
	CObsFile obsFile("bjfs2690.08o");
	CNavFile navFile("bjfs2690.08n");
    CPosition pointPosition;
	CCoor sitCoor;
	double Tr;
	pointPosition.PointPosition(obsFile,navFile,sitCoor,Tr);
	cout << "¼ÆËãÍê±Ï£¡" << endl;
	return 0;
}
