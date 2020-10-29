#include "main.h"

struct stTestStruct {
	int m_iVar;
	float m_fVar;
	double m_dVar;
	int m_iszLength;
	char* m_szVar;
};

int main() {
	struct stTestStruct st;
	st.m_iVar = 21;
	st.m_fVar = -0.27;
	st.m_dVar = 74195034726.1;
	st.m_iszLength = strlen("China");
	st.m_szVar = (char*)malloc(st.m_iszLength);
	strcpy(st.m_szVar, "China");

	ByteStream bs;
	bs.Write(st.m_iVar);
	bs.Write(st.m_fVar);
	bs.Write(st.m_dVar);
	bs.Write(st.m_iszLength);
	bs.Write(st.m_szVar, st.m_iszLength);

	struct stTestStruct st2;
	bs.Read(st2.m_iVar);
	bs.Read(st2.m_fVar);
	bs.Read(st2.m_dVar);
	bs.Read(st2.m_iszLength);
	bs.Read(st2.m_szVar, st2.m_iszLength);

	return 0;
}