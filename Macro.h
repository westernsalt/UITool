#ifndef MACRO_H_
#define MACRO_H_

#define SAFE_DELETE(p) {if(p) delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p) {if(p) delete [] p; p = NULL;}

#define DECLARE_SINGLETON(p) private: static p* m_pInstance;\
	public:static void Initialize() { if(!m_pInstance) m_pInstance = new p; }\
	static p* GetInstance() { if(m_pInstance) return m_pInstance; return NULL; }\
	static void Destroy() {if(m_pInstance) SAFE_DELETE(m_pInstance); }\

#define IMPLEMENT_SINGLETON(p) p* p::m_pInstance = NULL;
#define RECT_INITIALIZE(rect) { rect.left = 0; rect.top = 0; rect.right = 0; rect.bottom = 0; }
#define RECT_COPY(rect, src) { rect.left = src.left; rect.top = src.top; rect.right = src.right; rect.bottom = src.bottom; }

#endif