
#ifndef _wsmatrix_h_
#define _wsmatrix_h_
namespace namespace_wslibs{
	namespace namespace_wsbaswidgets{
class WsMatrix
{
public:
	WsMatrix();
	WsMatrix(int row,int col);
	WsMatrix(int row,int col,int *_data);
	WsMatrix & operator =(const WsMatrix &matrix);
	~WsMatrix();
	int get(int row,int col);
	void set(int row,int col,int value);
	inline int row(){return m_row;}
	inline int col(){return m_col;}
private:	
	int m_row;
	int m_col;
	int *m_data;	

};
	}//wslibs
}//wsbaswidgets
#endif
