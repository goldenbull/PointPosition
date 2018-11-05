//CoorStruct.h

/*

  Definition of coordinate struct

*/

struct CoorCartesian{
	double X;
	double Y;
	double Z;
};

struct CoorGeodesy{
	double L;      //维度
	double B;      //经度
	double H;      //高程
};

struct CoorSite{
	double N;   //北方向坐标
	double E;   //东方向坐标
	double U;   //上方向坐标
};

struct CoorSitePolar{
	double rho;      //向径长
	double alpha;    //方位角
	double beta;     //高度角
};