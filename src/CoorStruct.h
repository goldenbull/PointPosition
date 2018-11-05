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
	double L;      //ά��
	double B;      //����
	double H;      //�߳�
};

struct CoorSite{
	double N;   //����������
	double E;   //����������
	double U;   //�Ϸ�������
};

struct CoorSitePolar{
	double rho;      //�򾶳�
	double alpha;    //��λ��
	double beta;     //�߶Ƚ�
};